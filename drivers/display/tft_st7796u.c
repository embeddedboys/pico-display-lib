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

#if TFT_DRV_USE_ST7796

static int tft_st7796_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

#if TFT_MODEL_YT350S006
    write_reg(priv, 0x11);
    mdelay(120);

    write_reg(priv, 0xF0, 0xC3);
    write_reg(priv, 0xF0, 0x96);

    write_reg(priv, 0x36, (1 << 5) | (1 << 3));
    write_reg(priv, 0xB4, 0x01);

    write_reg(priv, 0x3A, 0x05);
    write_reg(priv, 0xE8, 0x40, 0x82, 0x07, 0x18, 0x27, 0x0A, 0xB6, 0x33);
    write_reg(priv, 0xC5, 0x23);
    write_reg(priv, 0xC2, 0xA7);
    write_reg(priv, 0xE0, 0xF0, 0x01, 0x06, 0x0F, 0x12, 0x1D, 0x36, 0x54, 0x44, 0x0C, 0x18, 0x16, 0x13, 0x15);
    write_reg(priv, 0xE1, 0xF0, 0x01, 0x05, 0x0A, 0x0B, 0x07, 0x32, 0x44, 0x44, 0x0C, 0x18, 0x17, 0x13, 0x16);

    write_reg(priv, 0xF0, 0x3C);
    write_reg(priv, 0xF0, 0x69);
    mdelay(120);

    write_reg(priv, 0x29);
#endif

    return 0;
}

static struct tft_display st7796 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_st7796_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&st7796);
    return 0;
}

#endif
