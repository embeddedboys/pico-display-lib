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

#include <string.h>
#include "tft.h"

#if TFT_DRV_USE_ST7576

static u8 dgram[TFT_HOR_RES * TFT_VER_RES / 8];

static int tft_st7576_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);
    mdelay(50);

    write_cmd(priv, 0xe2);
    mdelay(20);

    write_cmd(priv, 0xa0);
    write_cmd(priv, 0xc8);
    write_cmd(priv, 0xa2);
    write_cmd(priv, 0x2f);
    write_cmd(priv, 0x25);
    write_cmd(priv, 0x81);
    write_cmd(priv, 0x19);
    write_cmd(priv, 0x40);
    write_cmd(priv, 0xaf);

    return 0;
}

static void tft_st7576_set_pos(struct tft_priv *priv, u8 page, u8 col)
{
    write_reg(priv, 0xb0 + page);

    write_reg(priv, 0x00 | (col & 0x0f));
    write_reg(priv, 0x10 | ((col >> 4)));
}

static uint8_t rgb565_to_brightness(u16 color)
{
    u8 r = (color >> 11) & 0x1f;
    u8 g = (color >> 5) & 0x3f;
    u8 b = color & 0x1f;

    return (r * 3 + g + b * 4) >> 3;
}

static void tft_st7576_flush(struct tft_priv *priv)
{
    for (int i=0; i < 8; i++) {
        tft_st7576_set_pos(priv, i, 0);
        for (int j=0; j < TFT_HOR_RES; j++)
            write_data(priv, dgram[(i * TFT_HOR_RES) + j]);
    }
}

static int tft_st7567_clear(struct tft_priv *priv, u16 color)
{
    if (color)
        memset(dgram, 0xff, sizeof(dgram));
    else
        memset(dgram, 0x00, sizeof(dgram));

    tft_st7576_flush(priv);
    return 0;
}

static void tft_st7576_video_sync(struct tft_priv *priv, int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    u16 *colorp = (u16 *)vmem;
    size_t remain = len / 2;
    int x, y, i = 0;

    for (x = xs; x < xe; x++) {
        for (y = ys; y < ye; y++) {
            if(rgb565_to_brightness(colorp[i++]) < 128)
                dgram[(y / 8) * TFT_HOR_RES + x] |= (1 << (y % 8));
            else
                dgram[(y / 8) * TFT_HOR_RES + x] &= ~(1 << (y % 8));
        }
    }

    tft_st7576_flush(priv);
}

static struct tft_display st7576 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 1,
    .backlight = 100,
    // .need_tx_buf = true,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_st7576_init_display,
        .clear = tft_st7567_clear,
        .video_sync = tft_st7576_video_sync,
    },
};

int tft_driver_init(void)
{
    tft_probe(&st7576);
    return 0;
}

#endif