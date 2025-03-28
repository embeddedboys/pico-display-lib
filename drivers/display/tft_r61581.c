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

#include "tft.h"

#if TFT_DRV_USE_R61581

static int tft_r61581_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    write_reg(priv, 0xB0, 0x00);
    write_reg(priv, 0xB3, 0x02, 0x00, 0x00, 0x00);

    /* Backlight control */

    write_reg(priv, 0xC0, 0x13, 0x3B, 0x00, 0x02, 0x00, 0x01, 0x00, 0x43);
    write_reg(priv, 0xC1, 0x08, 0x16, 0x08, 0x08);
    write_reg(priv, 0xC4, 0x11, 0x07, 0x03, 0x03);
    write_reg(priv, 0xC6, 0x00);
    write_reg(priv, 0xC8, 0x03, 0x03, 0x13, 0x5C, 0x03, 0x07, 0x14, 0x08, 0x00, 0x21, 0x08, 0x14, 0x07, 0x53, 0x0C, 0x13, 0x03, 0x03, 0x21, 0x00);
    write_reg(priv, 0x0C, 0x55);
    write_reg(priv, 0x36, (1 << 6) | (1 << 5));
    write_reg(priv, 0x38);
    write_reg(priv, 0x3A, 0x55);
    write_reg(priv, 0xD0, 0x07, 0x07, 0x1D, 0x03);
    write_reg(priv, 0xD1, 0x03, 0x30, 0x10);
    write_reg(priv, 0xD2, 0x03, 0x14, 0x04);

    write_reg(priv, 0x11);
    mdelay(10);
    write_reg(priv, 0x29);

    return 0;
}

static struct tft_display r61581 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
#if TFT_BUS_TYPE == TFT_BUS_TYPE_SPI
    #error "TFT_BUS_TYPE_SPI not supported"
#elif TFT_BUS_TYPE == TFT_BUS_TYPE_I80

#if TFT_PIN_DB_COUNT == 8
        .write_reg = tft_write_reg8,
#elif TFT_PIN_DB_COUNT == 16
    #error "TFT_PIN_DB_COUNT must be 8"
#endif

#endif
        .init_display = tft_r61581_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&r61581);
    return 0;
}

#endif
