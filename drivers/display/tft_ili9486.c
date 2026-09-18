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

#if TFT_DRV_USE_ILI9486

static int tft_ili9481_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    write_reg(priv, 0x11);
    mdelay(20);

    write_reg(priv, 0xf1, 0x36, 0x04, 0x00, 0x3c, 0x0f, 0x8f);
    write_reg(priv, 0xf2, 0x18, 0xa3, 0x12, 0x02, 0xb2, 0x12, 0xff, 0x10, 0x00);
    write_reg(priv, 0xf8, 0x21, 0x04);
    write_reg(priv, 0xf9, 0x00, 0x08);
    write_reg(priv, 0x36, 0x08);
    write_reg(priv, 0xb4, 0x00);
    write_reg(priv, 0xc1, 0x47);
    write_reg(priv, 0xc5, 0x00, 0xaf, 0x80, 0x00);
    write_reg(priv, 0xe0, 0x0f, 0x1f, 0x1c, 0x0c, 0x0f, 0x08, 0x48, 0x98, 0x37, 0x0a, 0x13, 0x04, 0x11, 0x0d, 0x00);
    write_reg(priv, 0xe1, 0x0f, 0x32, 0x2e, 0x0b, 0x0d, 0x05, 0x47, 0x75, 0x37, 0x06, 0x10, 0x03, 0x24, 0x20, 0x00);
    write_reg(priv, 0x3a, 0x55);
    write_reg(priv, 0x11);
    write_reg(priv, 0x36, 0x0);
    mdelay(120);
    write_reg(priv, 0x29);

    return 0;
}

static struct tft_display ili9481 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_ili9481_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&ili9481);
    return 0;
}

#endif
