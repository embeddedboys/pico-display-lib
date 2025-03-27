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

#if TFT_DRV_USE_GC9A01

static int tft_gc9a01_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    write_cmd(priv, 0xEF);
	write_cmd(priv, 0xEB);
	write_data(priv, 0x14);

    write_cmd(priv, 0xFE);
	write_cmd(priv, 0xEF);

	write_cmd(priv, 0xEB);
	write_data(priv, 0x14);

	write_cmd(priv, 0x84);
	write_data(priv, 0x40);

	write_cmd(priv, 0x85);
	write_data(priv, 0xFF);

	write_cmd(priv, 0x86);
	write_data(priv, 0xFF);

	write_cmd(priv, 0x87);
	write_data(priv, 0xFF);

	write_cmd(priv, 0x88);
	write_data(priv, 0x0A);

	write_cmd(priv, 0x89);
	write_data(priv, 0x21);

	write_cmd(priv, 0x8A);
	write_data(priv, 0x00);

	write_cmd(priv, 0x8B);
	write_data(priv, 0x80);

	write_cmd(priv, 0x8C);
	write_data(priv, 0x01);

	write_cmd(priv, 0x8D);
	write_data(priv, 0x01);

	write_cmd(priv, 0x8E);
	write_data(priv, 0xFF);

	write_cmd(priv, 0x8F);
	write_data(priv, 0xFF);

	write_cmd(priv, 0xB6);
	write_data(priv, 0x00);
	write_data(priv, 0x20);

	write_cmd(priv, 0x36);
	write_data(priv, 0x08);//Set as vertical screen

	write_cmd(priv, 0x3A);
	write_data(priv, 0x05);


	write_cmd(priv, 0x90);
	write_data(priv, 0x08);
	write_data(priv, 0x08);
	write_data(priv, 0x08);
	write_data(priv, 0x08);

	write_cmd(priv, 0xBD);
	write_data(priv, 0x06);

	write_cmd(priv, 0xBC);
	write_data(priv, 0x00);

	write_cmd(priv, 0xFF);
	write_data(priv, 0x60);
	write_data(priv, 0x01);
	write_data(priv, 0x04);

	write_cmd(priv, 0xC3);
	write_data(priv, 0x13);
	write_cmd(priv, 0xC4);
	write_data(priv, 0x13);

	write_cmd(priv, 0xC9);
	write_data(priv, 0x22);

	write_cmd(priv, 0xBE);
	write_data(priv, 0x11);

	write_cmd(priv, 0xE1);
	write_data(priv, 0x10);
	write_data(priv, 0x0E);

	write_cmd(priv, 0xDF);
	write_data(priv, 0x21);
	write_data(priv, 0x0c);
	write_data(priv, 0x02);

	write_cmd(priv, 0xF0);
	write_data(priv, 0x45);
	write_data(priv, 0x09);
	write_data(priv, 0x08);
	write_data(priv, 0x08);
	write_data(priv, 0x26);
 	write_data(priv, 0x2A);

 	write_cmd(priv, 0xF1);
 	write_data(priv, 0x43);
 	write_data(priv, 0x70);
 	write_data(priv, 0x72);
 	write_data(priv, 0x36);
 	write_data(priv, 0x37);
 	write_data(priv, 0x6F);


 	write_cmd(priv, 0xF2);
 	write_data(priv, 0x45);
 	write_data(priv, 0x09);
 	write_data(priv, 0x08);
 	write_data(priv, 0x08);
 	write_data(priv, 0x26);
 	write_data(priv, 0x2A);

 	write_cmd(priv, 0xF3);
 	write_data(priv, 0x43);
 	write_data(priv, 0x70);
 	write_data(priv, 0x72);
 	write_data(priv, 0x36);
 	write_data(priv, 0x37);
 	write_data(priv, 0x6F);

	write_cmd(priv, 0xED);
	write_data(priv, 0x1B);
	write_data(priv, 0x0B);

	write_cmd(priv, 0xAE);
	write_data(priv, 0x77);

	write_cmd(priv, 0xCD);
	write_data(priv, 0x63);

	write_cmd(priv, 0x70);
	write_data(priv, 0x07);
	write_data(priv, 0x07);
	write_data(priv, 0x04);
	write_data(priv, 0x0E);
	write_data(priv, 0x0F);
	write_data(priv, 0x09);
	write_data(priv, 0x07);
	write_data(priv, 0x08);
	write_data(priv, 0x03);

	write_cmd(priv, 0xE8);
	write_data(priv, 0x34);

	write_cmd(priv, 0x62);
	write_data(priv, 0x18);
	write_data(priv, 0x0D);
	write_data(priv, 0x71);
	write_data(priv, 0xED);
	write_data(priv, 0x70);
	write_data(priv, 0x70);
	write_data(priv, 0x18);
	write_data(priv, 0x0F);
	write_data(priv, 0x71);
	write_data(priv, 0xEF);
	write_data(priv, 0x70);
	write_data(priv, 0x70);

	write_cmd(priv, 0x63);
	write_data(priv, 0x18);
	write_data(priv, 0x11);
	write_data(priv, 0x71);
	write_data(priv, 0xF1);
	write_data(priv, 0x70);
	write_data(priv, 0x70);
	write_data(priv, 0x18);
	write_data(priv, 0x13);
	write_data(priv, 0x71);
	write_data(priv, 0xF3);
	write_data(priv, 0x70);
	write_data(priv, 0x70);

	write_cmd(priv, 0x64);
	write_data(priv, 0x28);
	write_data(priv, 0x29);
	write_data(priv, 0xF1);
	write_data(priv, 0x01);
	write_data(priv, 0xF1);
	write_data(priv, 0x00);
	write_data(priv, 0x07);

	write_cmd(priv, 0x66);
	write_data(priv, 0x3C);
	write_data(priv, 0x00);
	write_data(priv, 0xCD);
	write_data(priv, 0x67);
	write_data(priv, 0x45);
	write_data(priv, 0x45);
	write_data(priv, 0x10);
	write_data(priv, 0x00);
	write_data(priv, 0x00);
	write_data(priv, 0x00);

	write_cmd(priv, 0x67);
	write_data(priv, 0x00);
	write_data(priv, 0x3C);
	write_data(priv, 0x00);
	write_data(priv, 0x00);
	write_data(priv, 0x00);
	write_data(priv, 0x01);
	write_data(priv, 0x54);
	write_data(priv, 0x10);
	write_data(priv, 0x32);
	write_data(priv, 0x98);

	write_cmd(priv, 0x74);
	write_data(priv, 0x10);
	write_data(priv, 0x85);
	write_data(priv, 0x80);
	write_data(priv, 0x00);
	write_data(priv, 0x00);
	write_data(priv, 0x4E);
	write_data(priv, 0x00);

    write_cmd(priv, 0x98);
	write_data(priv, 0x3e);
	write_data(priv, 0x07);

	write_cmd(priv, 0x35);
	write_cmd(priv, 0x21);

	write_cmd(priv, 0x11);
	mdelay(120);
	write_cmd(priv, 0x29);
	mdelay(20);

    return 0;
}

static struct tft_display gc9a01 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_gc9a01_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&gc9a01);
    return 0;
}

#endif
