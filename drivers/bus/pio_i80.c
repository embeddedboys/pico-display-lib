// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>

#include "pico/time.h"
#include "pico/stdlib.h"
#include "pico/platform.h"

#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"
#include "hardware/clocks.h"

#include "boards/pico.h"

// Do not modify this header file. It is automatically generated from the pio program.
// you should modify the pio program instead.
#include "pio_i80.pio.h"

static PIO g_pio = pio0;
static uint g_sm = 0;

void __time_critical_func(i80_set_rs_cs)(bool rs, bool cs)
{
    gpio_put_masked((1u << TFT_PIN_RS) | (1u << TFT_PIN_CS), !!rs << TFT_PIN_RS | !!cs << TFT_PIN_CS);
}

void __time_critical_func(i80_set_rs)(bool rs)
{
    gpio_put(TFT_PIN_RS, rs);
}

#if PIO_USE_DMA
/* DMA version */
static uint dma_tx;
static dma_channel_config c;

/*
 * Asynchronous API state (see i80_write_buf_rs_async below). While a transfer
 * is in flight `s_pending` is set and the trailing CS deassert has not
 * happened yet; it is performed by the next write or by i80_write_sync().
 */
static volatile bool s_pending;
static bool s_pending_rs;

/* Complete an in-flight async transfer: wait for the DMA, then release CS.
 * This reproduces exactly the pin timing of the synchronous path -- the wait
 * just happens at the start of the following call instead of inline. */
static inline void i80_finish_pending(void)
{
    if (s_pending) {
        dma_channel_wait_for_finish_blocking(dma_tx);
        i80_set_rs_cs(s_pending_rs, 1);
        s_pending = false;
    }
}

#define define_i80_write_piox(func, buffer_type) \
void func(PIO pio, uint sm, void *buf, size_t len) \
{ \
    dma_channel_configure(dma_tx, &c,   \
                          &pio->txf[sm],    \
                          (buffer_type *)buf,  \
                          len / sizeof(buffer_type),  \
                          true  \
    );  \
    dma_channel_wait_for_finish_blocking(dma_tx);   \
}

/* Same as above but returns as soon as the DMA is running. */
#define define_i80_write_piox_async(func, buffer_type) \
void func(PIO pio, uint sm, void *buf, size_t len) \
{ \
    dma_channel_configure(dma_tx, &c,   \
                          &pio->txf[sm],    \
                          (buffer_type *)buf,  \
                          len / sizeof(buffer_type),  \
                          true  \
    );  \
}
#else
/* Without DMA the CPU performs the transfer itself, so there is nothing to
 * overlap; the async entry points fall back to the synchronous path below. */
#define define_i80_write_piox(func, buffer_type) \
void func(PIO pio, uint sm, void *buf, size_t len) \
{ \
    buffer_type data;   \
    \
    i80_wait_idle(pio, sm); \
    while (len) {   \
        data = *(buffer_type *)buf; \
    \
        i80_put(pio, sm, data); \
    \
        buf += sizeof(buffer_type);   \
        len -= sizeof(buffer_type);   \
    }   \
    i80_wait_idle(pio, sm); \
}
#endif

define_i80_write_piox(i80_write_pio8, uint8_t)
define_i80_write_piox(i80_write_pio16, uint16_t)
#if PIO_USE_DMA
define_i80_write_piox_async(i80_write_pio8_async, uint8_t)
define_i80_write_piox_async(i80_write_pio16_async, uint16_t)
#endif

int __time_critical_func(i80_write_buf_rs)(void *buf, size_t len, bool rs)
{
#if PIO_USE_DMA
    /* an async transfer may still be in flight; finish it first so the pin
     * timing is identical to a purely synchronous sequence */
    i80_finish_pending();
#endif

    i80_wait_idle(g_pio, g_sm);

    i80_set_rs_cs(rs, 0);

#if TFT_PIN_DB_COUNT == 8
    i80_write_pio8(g_pio, g_sm, buf, len);
#elif TFT_PIN_DB_COUNT == 16
    i80_write_pio16(g_pio, g_sm, buf, len);
#endif

    i80_set_rs_cs(rs, 1);
    return 0;
}

/*
 * Asynchronous variant: starts the transfer and returns without waiting for it
 * to finish. The caller must not reuse `buf` until i80_write_sync() (or the
 * next write, which also completes the pending transfer) has returned.
 *
 * Without DMA (PIO_USE_DMA == 0) the CPU does the transfer itself, so this
 * falls back to the synchronous path.
 */
void i80_write_buf_rs_async(void *buf, size_t len, bool rs)
{
#if PIO_USE_DMA
    i80_finish_pending();
    i80_wait_idle(g_pio, g_sm);

    i80_set_rs_cs(rs, 0);

#if TFT_PIN_DB_COUNT == 8
    i80_write_pio8_async(g_pio, g_sm, buf, len);
#elif TFT_PIN_DB_COUNT == 16
    i80_write_pio16_async(g_pio, g_sm, buf, len);
#endif

    s_pending_rs = rs;
    s_pending = true;
#else
    (void)i80_write_buf_rs(buf, len, rs);
#endif
}

/* Wait for any in-flight asynchronous transfer to complete. */
void i80_write_sync(void)
{
#if PIO_USE_DMA
    i80_finish_pending();
#endif
}

int i80_pio_init(uint8_t db_base, uint8_t db_count, uint8_t pin_wr)
{
    printf("i80 PIO initialzing...\n");

#if PIO_USE_DMA
    dma_tx = dma_claim_unused_channel(true);
    c = dma_channel_get_default_config(dma_tx);

#if TFT_PIN_DB_COUNT == 8
    channel_config_set_transfer_data_size(&c, DMA_SIZE_8);
#elif TFT_PIN_DB_COUNT == 16
    channel_config_set_transfer_data_size(&c, DMA_SIZE_16);
#else
    #error "The `TFT_PIN_DB_COUNT` must be 8 or 16"
#endif

    channel_config_set_dreq(&c, pio_get_dreq(g_pio, g_sm, true));
#endif

    uint offset = pio_add_program(g_pio, &i80_program);
    float clk_div = (DEFAULT_PIO_CLK_KHZ / 2.f / TFT_BUS_CLK_KHZ);
    i80_program_init(g_pio, g_sm, offset, db_base, db_count, pin_wr, clk_div);

    return 0;
}

