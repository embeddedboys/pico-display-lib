// Copyright (c) 2024 embeddedboys developers
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pico/time.h"
#include "pico/stdio.h"
#include "pico/stdlib.h"
#include "pico/platform.h"
#include "pico/stdio_uart.h"
#include "pico/binary_info.h"

#include "hardware/spi.h"
#include "hardware/pwm.h"

#include "tft.h"
#include "config.h"

#define TAG "tft: "

#define pr_debug(...) printf(TAG __VA_ARGS__)
#define pr_debug_nt(...)

static struct tft_priv g_priv;

static TaskHandle_t xTaskToNotify = NULL;
static const UBaseType_t XArrayIndex = 1;

#if TFT_BUS_TYPE == TFT_BUS_TYPE_SPI
void tft_spi_write_buf_dc(struct tft_priv *priv, void *buf, size_t len, bool dc)
{
    gpio_put(TFT_PIN_DC, dc);

    dm_gpio_set_value(TFT_PIN_CS, 0);
    spi_write_blocking(spi_ifce, buf, len);
    dm_gpio_set_value(TFT_PIN_CS, 1);
}
#endif

#if TFT_BUS_TYPE == TFT_BUS_TYPE_I80
static void fbtft_write_gpio16_wr(struct tft_priv *priv, void *buf, size_t len)
{
    u16 data;
    int i;
#ifndef DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
    static u16 prev_data;
#endif

    /* Start writing by pulling down /WR */
    dm_gpio_set_value(priv->gpio.wr, 1);

    while (len) {
        data = *(u16 *)buf;

        /* Start writing by pulling down /WR */
        dm_gpio_set_value(priv->gpio.wr, 0);

        /* Set data */
#ifndef DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
        if (data == prev_data) {
            dm_gpio_set_value(priv->gpio.wr, 1); /* used as delay */
        } else {
            for (i = 0; i < 16; i++) {
                if ((data & 1) != (prev_data & 1))
                    dm_gpio_set_value(priv->gpio.db[i],
                                      data & 1);
                data >>= 1;
                prev_data >>= 1;
            }
        }
#else
        for (i = 0; i < 16; i++) {
            dm_gpio_set_value(&priv->gpio.db[i], data & 1);
            data >>= 1;
        }
#endif

        /* Pullup /WR */
        dm_gpio_set_value(priv->gpio.wr, 1);

#ifndef DO_NOT_OPTIMIZE_FBTFT_WRITE_GPIO
        prev_data = *(u16 *)buf;
#endif
        buf += 2;
        len -= 2;
    }
}

void fbtft_write_gpio16_wr_rs(struct tft_priv *priv, void *buf, size_t len, bool rs)
{
    dm_gpio_set_value(priv->gpio.rs, rs);
    fbtft_write_gpio16_wr(priv, buf, len);
}
#endif

inline void tft_write_cmd(struct tft_priv *priv, u8 cmd)
{
    write_buf_dc(priv, &cmd, sizeof(cmd), 0);
}

inline void tft_write_data(struct tft_priv *priv, u8 data)
{
    write_buf_dc(priv, &data, sizeof(data), 1);
}

#define define_tft_write_reg(func, reg_type) \
void func(struct tft_priv *priv, int len, ...)  \
{   \
    reg_type *buf = (reg_type *)priv->buf; \
    va_list args;   \
    int i;  \
    \
    va_start(args, len);    \
    *buf = (reg_type)va_arg(args, unsigned int); \
    pr_debug_nt("cmd : 0x%02x\n", *buf); \
    write_buf_dc(priv, buf, sizeof(reg_type), 0); \
    len--;  \
    \
    /* if there no privams */  \
    if (len == 0)  \
        goto exit_no_param; \
    \
    pr_debug_nt(" val :"); \
    for (i = 0; i < len; i++) { \
        pr_debug_nt(" 0x%02x", *buf); \
        *buf++ = (reg_type)va_arg(args, unsigned int); \
    }   \
    pr_debug_nt("\n"); \
    \
    len *= sizeof(reg_type);    \
    write_buf_dc(priv, priv->buf, len, 1);  \
exit_no_param:  \
    va_end(args);   \
}

define_tft_write_reg(tft_write_reg8, u8)
define_tft_write_reg(tft_write_reg16, u16)

static int tft_reset(struct tft_priv *priv)
{
    dm_gpio_set_value(priv->gpio.reset, 1);
    mdelay(10);
    dm_gpio_set_value(priv->gpio.reset, 0);
    mdelay(10);
    dm_gpio_set_value(priv->gpio.reset, 1);
    mdelay(10);
    return 0;
}

static void inline tft_set_addr_win(struct tft_priv *priv, int xs, int ys, int xe, int ye)
{
    /* set column adddress */
    write_reg(priv, 0x2A, xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF);

    /* set row address */
    write_reg(priv, 0x2B, ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF);

    /* write start */
    write_reg(priv, 0x2C);
}

static int tft_clear(struct tft_priv *priv, u16 clear)
{
    u32 width = priv->display->xres;
    u32 height = priv->display->yres;
    int i;

    pr_debug("clearing screen (%d x %d) with color 0x%x\n", width, height, clear);

    priv->tftops->set_addr_win(priv, 0, 0, width - 1, height - 1);

    for (i = 0; i < width * height; i++)
        write_buf_dc(priv, &clear, sizeof(u16), 1);

    return 0;
}

int tft_fill_color(u16 color)
{
    return tft_clear(&g_priv, color);
}

static void tft_video_sync(struct tft_priv *priv, int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    priv->tftops->set_addr_win(priv, xs, ys, xe, ye);
    write_buf_dc(priv, vmem, len, 1);
}

void tft_video_flush(int xs, int ys, int xe, int ye, void *vmem, uint32_t len)
{
    xTaskToNotify = xTaskGetCurrentTaskHandle();

    g_priv.tftops->video_sync(&g_priv, xs, ys, xe, ye, vmem, len);

    xTaskNotifyGiveIndexed(xTaskToNotify, XArrayIndex);

    xTaskToNotify = NULL;
}

portTASK_FUNCTION(video_flush_task, pvParameters)
{
    const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 100 );
    uint32_t ulNotificationValue;
    struct video_frame vf;

    for (;;) {
        /* if lvgl request to draw */
        if (xQueueReceive(xToFlushQueue, &vf, portMAX_DELAY)) {
            // pr_debug("Received video frame to flush\n");
            tft_video_flush(vf.xs, vf.ys, vf.xe, vf.ye, vf.vmem, vf.len);

            /* waiting for notification */
            ulNotificationValue = ulTaskNotifyTakeIndexed(XArrayIndex, pdTRUE, xMaxBlockTime);
            // pr_debug("Received notification, val : %d\n", ulNotificationValue);

            if (ulNotificationValue > 0) {
                /* Notification received */
                frame_counter++;
            } else {
                /* timeout */
            }
        }
    }

    vTaskDelete(NULL);
}

void tft_async_video_push(struct video_frame *vf)
{
    xQueueSend(xToFlushQueue, (void *)vf, portMAX_DELAY);
}

#if TFT_BUS_TYPE == TFT_BUS_TYPE_SPI
static int tft_gpio_init(struct tft_priv *priv)
{
    pr_debug("initializing gpios...\n");

    gpio_init(priv->gpio.cs);
    gpio_init(priv->gpio.dc);
    gpio_init(priv->gpio.reset);

    gpio_set_dir(priv->gpio.cs, GPIO_OUT);
    gpio_set_dir(priv->gpio.dc, GPIO_OUT);
    gpio_set_dir(priv->gpio.reset, GPIO_OUT);

    gpio_put(priv->gpio.cs, 1);
    gpio_put(priv->gpio.reset, 0);
}
#elif TFT_BUS_TYPE == TFT_BUS_TYPE_I80
static int tft_gpio_init(struct tft_priv *priv)
{
#if DISP_OVER_PIO
    gpio_init(priv->gpio.reset);
    gpio_init(priv->gpio.rs);

    gpio_set_dir(priv->gpio.reset, GPIO_OUT);
    gpio_set_dir(priv->gpio.rs, GPIO_OUT);
#else
    int *pp = (int *)&priv->gpio;

    int len = sizeof(priv->gpio)/sizeof(priv->gpio.reset);

    while(len--) {
        gpio_init(*pp);
        gpio_set_dir(*pp, GPIO_OUT);
        pp++;
    }
#endif
    return 0;
}
#endif /* TFT_BUS_TYPE == XXX */

static char *tft_bus_strings[] = {
    [TFT_BUS_TYPE_SPI] = "SPI",
    [TFT_BUS_TYPE_I80] = "I8080",
    [TFT_BUS_TYPE_I2C] = "I2C",
};

static int tft_hw_init(struct tft_priv *priv)
{
    int ret;
    pr_debug("TFT bus type: %s\n", tft_bus_strings[TFT_BUS_TYPE]);

#if TFT_BUS_TYPE == TFT_BUS_TYPE_SPI

#if DISP_OVER_PIO
    pio_spi_tx_init(TFT_PIN_SDA, TFT_PIN_SCL);
    bi_decl(bi_2pins_with_func(TFT_PIN_SCL, TFT_PIN_SDA, GPIO_FUNC_PIO0));
#else
    spi_init(spi_ifce, TFT_BUS_CLK_KHZ * 1000);
    gpio_set_function(TFT_PIN_SCL, GPIO_FUNC_SPI);
    gpio_set_function(TFT_PIN_SDA, GPIO_FUNC_SPI);
    bi_decl(bi_2pins_with_func(TFT_PIN_SCL, TFT_PIN_SDA, GPIO_FUNC_SPI));
    pr_debug("spi%d initialized at %d kHz\n", spi_get_index(spi_ifce), spi_get_baudrate(spi_ifce) / 1000 );
#endif

#elif TFT_BUS_TYPE == TFT_BUS_TYPE_I80

#if DISP_OVER_PIO
    i80_pio_init(priv->gpio.db[0], ARRAY_SIZE(priv->gpio.db), priv->gpio.wr);
#endif

#endif

    tft_gpio_init(priv);

    if (!priv->tftops->init_display) {
        printf("tftops->init_display is not implemented");
        return -1;
    }

    pr_debug("initializing display...\n");
    priv->tftops->init_display(priv);

    /* clear screen to black */
    // pr_debug("clearing screen...\n");
    // priv->tftops->clear(priv, 0x0);

    // pr_debug("enbaling backlight...\n");
    // dm_gpio_set_value(priv->gpio.blk, 1);

    return 0;
}

void tft_merge_tftops(struct tft_ops *dst, struct tft_ops *src)
{
    pr_debug("%s\n", __func__);
    if (src->write_reg)
        dst->write_reg = src->write_reg;
    if (src->init_display)
        dst->init_display = src->init_display;
    if (src->reset)
        dst->reset = src->reset;
    if (src->clear)
        dst->clear = src->clear;
    if (src->sleep)
        dst->sleep = src->sleep;
    if (src->set_addr_win)
        dst->set_addr_win = src->set_addr_win;
    if (src->video_sync)
        dst->video_sync = src->video_sync;
}

int tft_probe(struct tft_display *display)
{
    struct tft_priv *priv = &g_priv;
    pr_debug("%s\n", __func__);

    priv->buf = (u8 *)malloc(TFT_REG_BUF_SIZE);
    if (!priv->buf) {
        pr_debug("failed to allocate buffer\n");
        return -1;
    }

    priv->tftops = (struct tft_ops *)malloc(sizeof(struct tft_ops));
    if (!priv->tftops) {
        pr_debug("failed to allocate tftops\n");
        goto exit_free_priv_buf;
    }

    priv->display = display;

#if TFT_BUS_TYPE == TFT_BUS_TYPE_SPI
    priv->gpio.scl   = TFT_PIN_SCL;
    priv->gpio.sda   = TFT_PIN_SDA;
    priv->gpio.reset = TFT_PIN_RES;
    priv->gpio.dc    = TFT_PIN_DC;
    priv->gpio.cs    = TFT_PIN_CS;
    priv->gpio.blk   = TFT_PIN_BLK;
#elif TFT_BUS_TYPE == TFT_BUS_TYPE_I80
    priv->gpio.bl    = TFT_PIN_BLK;
    priv->gpio.reset = TFT_PIN_RES;
    priv->gpio.rs    = TFT_PIN_RS;
    priv->gpio.wr    = TFT_PIN_WR;
    priv->gpio.cs    = TFT_PIN_CS;
#endif

    priv->tftops->reset = tft_reset;
    priv->tftops->set_addr_win = tft_set_addr_win;
    priv->tftops->clear = tft_clear;
    priv->tftops->video_sync = tft_video_sync;

    if (priv->display->need_tx_buf) {
        priv->txbuf.buf = (u8 *)malloc(TFT_TX_BUF_SIZE);
        if (!priv->txbuf.buf) {
            pr_debug("failed to allocate tx buffer\n");
            goto exit_free_tftops;
        }
        priv->txbuf.len = TFT_TX_BUF_SIZE;
    }

    tft_merge_tftops(priv->tftops, &display->tftops);

    tft_hw_init(priv);

    return 0;

exit_free_tftops:
    free(priv->tftops);
exit_free_priv_buf:
    free(priv->buf);
    return -1;
}
