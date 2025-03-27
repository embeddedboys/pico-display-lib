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
#else
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

int __time_critical_func(i80_write_buf_rs)(void *buf, size_t len, bool rs)
{
#if !PIO_USE_DMA
    i80_wait_idle(g_pio, g_sm);
#endif

    i80_set_rs_cs(rs, 0);

#if TFT_PIN_DB_COUNT == 8
    i80_write_pio8(g_pio, g_sm, buf, len);
#elif TFT_PIN_DB_COUNT == 16
    i80_write_pio16(g_pio, g_sm, buf, len);
#endif

#if !PIO_USE_DMA
    i80_wait_idle(g_pio, g_sm);
#endif

    i80_set_rs_cs(rs, 1);
    return 0;
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