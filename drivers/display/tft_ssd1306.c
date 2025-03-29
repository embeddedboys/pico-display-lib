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

#if TFT_DRV_USE_SSD1306

static int tft_ssd1306_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    // write_reg(priv, 0xae); //--turn off oled panel

    // write_reg(priv, 0x00); //--set low column address
    // write_reg(priv, 0x10); //--set high column address

    // write_reg(priv, 0x40); //--set start line address

    // write_reg(priv, 0xb0); //--set page address

    // write_reg(priv, 0x81); //--set contrast control register
    // write_reg(priv, 0xff);

    // write_reg(priv, 0xa1); //--set segment re-map 127 to 0   a0:0 to seg127
    // write_reg(priv, 0xa6); //--set normal display

    // write_reg(priv, 0xc9); //--set com(N-1)to com0  c0:com0 to com(N-1)

    // write_reg(priv, 0xa8); //--set multiples ratio(1to64)
    // write_reg(priv, 0x3f); //--1/64 duty

    // write_reg(priv, 0xd3); //--set display offset
    // write_reg(priv, 0x00); //--not offset

    // write_reg(priv, 0xd5); //--set display clock divide ratio/oscillator frequency
    // write_reg(priv, 0x80); //--set divide ratio

    // write_reg(priv, 0xd9); //--set pre-charge period
    // write_reg(priv, 0xf1);

    // write_reg(priv, 0xda); //--set com pins hardware configuration
    // write_reg(priv, 0x12);

    // write_reg(priv, 0xdb); //--set vcomh
    // write_reg(priv, 0x40);

    // write_reg(priv, 0x8d); //--set chare pump enable/disable
    // write_reg(priv, 0x14); //--set(0x10) disable
    //                     // write_reg(priv, 0x10);	//--set(0x10) disable
    // write_reg(priv, 0xaf); //--turn on oled panel

    // write_reg(priv, 0x20);
    // write_reg(priv, 0x00);

	write_reg(priv, 0xAE); /*display off*/
	write_reg(priv, 0x00); /*set lower column address*/
	write_reg(priv, 0x10); /*set higher column address*/
	write_reg(priv, 0x40); /*set display start line*/
	write_reg(priv, 0xB0); /*set page address*/
	write_reg(priv, 0x81); /*contract control*/
	write_reg(priv, 0xFF); /*128*/
	write_reg(priv, 0xA1); /*set segment remap*/
	write_reg(priv, 0xA6); /*normal / reverse*/
	write_reg(priv, 0xA8); /*multiplex ratio*/
	write_reg(priv, 0x3F); /*duty = 1/64*/
	write_reg(priv, 0xC8); /*Com scan direction*/
	write_reg(priv, 0xD3); /*set display offset*/
	write_reg(priv, 0x00);
	write_reg(priv, 0xD5); /*set osc division*/
	write_reg(priv, 0x80);
	write_reg(priv, 0xD9); /*set pre-charge period*/
	write_reg(priv, 0x1f);
	write_reg(priv, 0xDA); /*set COM pins*/
	write_reg(priv, 0x12);
	write_reg(priv, 0xdb); /*set vcomh*/
	write_reg(priv, 0x30);
	write_reg(priv, 0x8d); /*set charge pump enable*/
	write_reg(priv, 0x14);
	write_reg(priv, 0xAF); /*display ON*/

    write_reg(priv, 0x20);
    write_reg(priv, 0x00);

    return 0;
}

static void tft_ssd1306_set_pos(struct tft_priv *priv, u8 page, u8 col)
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

static u8 dgram[TFT_HOR_RES * TFT_VER_RES / 8];
static void tft_ssd1306_video_sync(struct tft_priv *priv, int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
    // pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    u16 *colorp = (u16 *)vmem;
    size_t remain = len / 2;
    int x, y, i = 0;

    tft_ssd1306_set_pos(priv, 0, 0);

    for (x = xs; x < xe; x++) {
        for (y = ys; y < ye; y++) {
            if(rgb565_to_brightness(colorp[i++]) < 128)
                dgram[(y / 8) * TFT_HOR_RES + x] |= (1 << (y % 8));
            else
                dgram[(y / 8) * TFT_HOR_RES + x] &= ~(1 << (y % 8));
        }
    }

    write_buf_dc(priv, dgram, sizeof(dgram), 1);
}

static struct tft_display ssd1306 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 1,
    .backlight = 100,
    // .need_tx_buf = true,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_ssd1306_init_display,
        .video_sync = tft_ssd1306_video_sync,
    },
};

int tft_driver_init(void)
{
    tft_probe(&ssd1306);
    return 0;
}

#endif