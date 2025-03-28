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

#if TFT_DRV_USE_ST7735

static int tft_st7735_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    // write_reg(priv, 0x11);
    // mdelay(120);

    // write_reg(priv, 0x36, 0x00);
    // write_reg(priv, 0x3A, 0x55);

    // write_reg(priv, 0xB1, 0x40, 0x00, 0x00);

    // write_reg(priv, 0xC6, 0x05);

    // write_reg(priv, 0x21);

#if TFT_MODEL_GENERIC
    write_reg(priv, 0x29);
    write_reg(priv, 0x11);
    mdelay(120);

    write_reg(priv, 0x36, (1 << 7) | (1 << 6) | (1 << 5));
    write_reg(priv, 0x3A, 0x55);

    write_reg(priv, 0x21);
    write_reg(priv, 0x29);

#elif TFT_MODEL_MD144_SPI_V04
    write_reg(priv, 0x11);
    mdelay(120);

    write_reg(priv, 0xB1, 0x05, 0x3A, 0x3A);
    write_reg(priv, 0xB2, 0x05, 0x3A, 0x3A);
    write_reg(priv, 0xB3, 0x05, 0x3A, 0x3A, 0x05, 0x3A, 0x3A);
    write_reg(priv, 0xB4, 0x03);
    write_reg(priv, 0xC0, 0x62, 0x02, 0x04);
    write_reg(priv, 0xC1, 0xC0);
    write_reg(priv, 0xC2, 0x0D, 0x00);
    write_reg(priv, 0xC3, 0x8D, 0x6A);
    write_reg(priv, 0xC4, 0x8D, 0xEE);
    write_reg(priv, 0xC5, 0x12);
    write_reg(priv, 0xE0, 0x03, 0x1B, 0x12, 0x11, 0x3F, 0x3A, 0x32, 0x34, 0x2F, 0x2B, 0x30, 0x3A, 0x00, 0x01, 0x02, 0x05);
    write_reg(priv, 0xE1, 0x03, 0x1B, 0x12, 0x11, 0x32, 0x2F, 0x2A, 0x2F, 0x2E, 0x2C, 0x35, 0x3F, 0x00, 0x00, 0x01, 0x05);

    write_reg(priv, 0xFC, 0x8C);
    write_reg(priv, 0x3A, 0x05);
    write_reg(priv, 0x36, 0xC8);
    write_reg(priv, 0x29);
#endif
}

static void inline tft_st7735_set_addr_win(struct tft_priv *priv, int xs, int ys, int xe, int ye)
{
    // Apply offset from display spec.
    xs += priv->display->xoffs;
    xe += priv->display->xoffs;
    ys += priv->display->yoffs;
    ye += priv->display->yoffs;

    /* set column adddress */
    write_reg(priv, 0x2A, xs >> 8, xs & 0xFF, xe >> 8, xe & 0xFF);

    /* set row address */
    write_reg(priv, 0x2B, ys >> 8, ys & 0xFF, ye >> 8, ye & 0xFF);

    /* write start */
    write_reg(priv, 0x2C);
}

static struct tft_display st7735 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .xoffs = TFT_X_OFFS,
    .yoffs = TFT_Y_OFFS,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_st7735_init_display,
        .set_addr_win = tft_st7735_set_addr_win,
    },
};

int tft_driver_init(void)
{
    tft_probe(&st7735);
    return 0;
}

#endif
