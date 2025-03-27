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

#if TFT_DRV_USE_ILI9341

static int tft_ili9341_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    write_reg(priv, 0x3A, 0x55);
    write_reg(priv, 0xF6, 0x01, 0x33);
    write_reg(priv, 0xB5, 0x04, 0x04, 0x0A, 0x14);
    write_reg(priv, 0x35, 0x00);
    write_reg(priv, 0xCF, 0x00, 0xEA, 0xF0);
    write_reg(priv, 0xED, 0x64, 0x03, 0x12, 0x81);
    write_reg(priv, 0xE8, 0x85, 0x00, 0x78);
    write_reg(priv, 0xCB, 0x39, 0x2C, 0x00, 0x33, 0x06);
    write_reg(priv, 0xF7, 0x20);
    write_reg(priv, 0xEA, 0x00, 0x00);
    write_reg(priv, 0xC0, 0x21);
    write_reg(priv, 0xC1, 0x10);
    write_reg(priv, 0xC5, 0x4F, 0x38);
    write_reg(priv, 0xC7, 0x98);
    write_reg(priv, 0x36, 0x08);
    write_reg(priv, 0xB1, 0x00, 0x13);
    write_reg(priv, 0xB6, 0x0A, 0xA2);
    write_reg(priv, 0xF2, 0x02);
    write_reg(priv, 0xE0, 0x0F, 0x27, 0x24, 0x0C, 0x10, 0x08, 0x55, 0X87, 0x45, 0x08, 0x14, 0x07, 0x13, 0x08, 0x00);
    write_reg(priv, 0XE1, 0x00, 0x0F, 0x12, 0x05, 0x11, 0x06, 0x25, 0x34, 0x37, 0x01, 0x08, 0x07, 0x2B, 0x34, 0x0F);

    write_reg(priv, 0x11);
    mdelay(120);
    write_reg(priv, 0x29);

    return 0;
}

static struct tft_display ili9341 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_ili9341_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&ili9341);
    return 0;
}

#endif
