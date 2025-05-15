// The MIT License (MIT)
//
// Copyright (c) 2025 embeddedboys developers
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

#if TFT_DRV_USE_ST77916

static int tft_st77916_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

#if TFT_MODEL_GENERIC || TFT_MODEL_W180TE010I_18Z
    write_cmd(priv, 0xF0);
    write_data(priv, 0x28);
    write_cmd(priv, 0xF2);
    write_data(priv, 0x28);
    write_cmd(priv, 0x73);
    write_data(priv, 0xF0);
    write_cmd(priv, 0x7C);
    write_data(priv, 0xD1);
    write_cmd(priv, 0x83);
    write_data(priv, 0xE0);
    write_cmd(priv, 0x84);
    write_data(priv, 0x61);
    write_cmd(priv, 0xF2);
    write_data(priv, 0x82);
    write_cmd(priv, 0xF0);
    write_data(priv, 0x00);
    write_cmd(priv, 0xF0);
    write_data(priv, 0x01);
    write_cmd(priv, 0xF1);
    write_data(priv, 0x01);
    write_cmd(priv, 0xB0);
    write_data(priv, 0x5E);
    write_cmd(priv, 0xB1);
    write_data(priv, 0x55);
    write_cmd(priv, 0xB2);
    write_data(priv, 0x24);
    write_cmd(priv, 0xB3);
    write_data(priv, 0x01);
    write_cmd(priv, 0xB4);
    write_data(priv, 0x87);
    write_cmd(priv, 0xB5);
    write_data(priv, 0x44);
    write_cmd(priv, 0xB6);
    write_data(priv, 0x8B);
    write_cmd(priv, 0xB7);
    write_data(priv, 0x40);
    write_cmd(priv, 0xB8);
    write_data(priv, 0x86);
    write_cmd(priv, 0xB9);
    write_data(priv, 0x15);
    write_cmd(priv, 0xBA);
    write_data(priv, 0x00);
    write_cmd(priv, 0xBB);
    write_data(priv, 0x08);
    write_cmd(priv, 0xBC);
    write_data(priv, 0x08);
    write_cmd(priv, 0xBD);
    write_data(priv, 0x00);
    write_cmd(priv, 0xBE);
    write_data(priv, 0x00);
    write_cmd(priv, 0xBF);
    write_data(priv, 0x07);
    write_cmd(priv, 0xC0);
    write_data(priv, 0x80);
    write_cmd(priv, 0xC1);
    write_data(priv, 0x10);
    write_cmd(priv, 0xC2);
    write_data(priv, 0x37);
    write_cmd(priv, 0xC3);
    write_data(priv, 0x80);
    write_cmd(priv, 0xC4);
    write_data(priv, 0x10);
    write_cmd(priv, 0xC5);
    write_data(priv, 0x37);
    write_cmd(priv, 0xC6);
    write_data(priv, 0xA9);
    write_cmd(priv, 0xC7);
    write_data(priv, 0x41);
    write_cmd(priv, 0xC8);
    write_data(priv, 0x01);
    write_cmd(priv, 0xC9);
    write_data(priv, 0xA9);
    write_cmd(priv, 0xCA);
    write_data(priv, 0x41);
    write_cmd(priv, 0xCB);
    write_data(priv, 0x01);
    write_cmd(priv, 0xCC);
    write_data(priv, 0x7F);
    write_cmd(priv, 0xCD);
    write_data(priv, 0x7F);
    write_cmd(priv, 0xCE);
    write_data(priv, 0xFF);
    write_cmd(priv, 0xD0);
    write_data(priv, 0x91);
    write_cmd(priv, 0xD1);
    write_data(priv, 0x68);
    write_cmd(priv, 0xD2);
    write_data(priv, 0x68);
    write_cmd(priv, 0xF5);
    write_data(priv, 0x00);
    write_data(priv, 0xA5);
    write_cmd(priv, 0xDD);
    write_data(priv, 0x40);
    write_cmd(priv, 0xDE);
    write_data(priv, 0x40);
    write_cmd(priv, 0xF1);
    write_data(priv, 0x10);
    write_cmd(priv, 0xF0);
    write_data(priv, 0x00);
    write_cmd(priv, 0xF0);
    write_data(priv, 0x02);
    write_cmd(priv, 0xE0);
    write_data(priv, 0xF0);
    write_data(priv, 0x10);
    write_data(priv, 0x18);
    write_data(priv, 0x0D);
    write_data(priv, 0x0C);
    write_data(priv, 0x38);
    write_data(priv, 0x3E);
    write_data(priv, 0x44);
    write_data(priv, 0x51);
    write_data(priv, 0x39);
    write_data(priv, 0x15);
    write_data(priv, 0x15);
    write_data(priv, 0x30);
    write_data(priv, 0x34);
    write_cmd(priv, 0xE1);
    write_data(priv, 0xF0);
    write_data(priv, 0x0F);
    write_data(priv, 0x17);
    write_data(priv, 0x0D);
    write_data(priv, 0x0B);
    write_data(priv, 0x07);
    write_data(priv, 0x3E);
    write_data(priv, 0x33);
    write_data(priv, 0x51);
    write_data(priv, 0x39);
    write_data(priv, 0x15);
    write_data(priv, 0x15);
    write_data(priv, 0x30);
    write_data(priv, 0x34);
    write_cmd(priv, 0xF0);
    write_data(priv, 0x10);
    write_cmd(priv, 0xF3);
    write_data(priv, 0x10);
    write_cmd(priv, 0xE0);
    write_data(priv, 0x08);
    write_cmd(priv, 0xE1);
    write_data(priv, 0x00);
    write_cmd(priv, 0xE2);
    write_data(priv, 0x00);
    write_cmd(priv, 0xE3);
    write_data(priv, 0x00);
    write_cmd(priv, 0xE4);
    write_data(priv, 0xE0);
    write_cmd(priv, 0xE5);
    write_data(priv, 0x06);
    write_cmd(priv, 0xE6);
    write_data(priv, 0x21);
    write_cmd(priv, 0xE7);
    write_data(priv, 0x03);
    write_cmd(priv, 0xE8);
    write_data(priv, 0x05);
    write_cmd(priv, 0xE9);
    write_data(priv, 0x02);
    write_cmd(priv, 0xEA);
    write_data(priv, 0xE9);
    write_cmd(priv, 0xEB);
    write_data(priv, 0x00);
    write_cmd(priv, 0xEC);
    write_data(priv, 0x00);
    write_cmd(priv, 0xED);
    write_data(priv, 0x14);
    write_cmd(priv, 0xEE);
    write_data(priv, 0xFF);
    write_cmd(priv, 0xEF);
    write_data(priv, 0x00);
    write_cmd(priv, 0xF8);
    write_data(priv, 0xFF);
    write_cmd(priv, 0xF9);
    write_data(priv, 0x00);
    write_cmd(priv, 0xFA);
    write_data(priv, 0x00);
    write_cmd(priv, 0xFB);
    write_data(priv, 0x30);
    write_cmd(priv, 0xFC);
    write_data(priv, 0x00);
    write_cmd(priv, 0xFD);
    write_data(priv, 0x00);
    write_cmd(priv, 0xFE);
    write_data(priv, 0x00);
    write_cmd(priv, 0xFF);
    write_data(priv, 0x00);
    write_cmd(priv, 0x60);
    write_data(priv, 0x40);
    write_cmd(priv, 0x61);
    write_data(priv, 0x05);
    write_cmd(priv, 0x62);
    write_data(priv, 0x00);
    write_cmd(priv, 0x63);
    write_data(priv, 0x42);
    write_cmd(priv, 0x64);
    write_data(priv, 0xDA);
    write_cmd(priv, 0x65);
    write_data(priv, 0x00);
    write_cmd(priv, 0x66);
    write_data(priv, 0x00);
    write_cmd(priv, 0x67);
    write_data(priv, 0x00);
    write_cmd(priv, 0x68);
    write_data(priv, 0x00);
    write_cmd(priv, 0x69);
    write_data(priv, 0x00);
    write_cmd(priv, 0x6A);
    write_data(priv, 0x00);
    write_cmd(priv, 0x6B);
    write_data(priv, 0x00);
    write_cmd(priv, 0x70);
    write_data(priv, 0x40);
    write_cmd(priv, 0x71);
    write_data(priv, 0x04);
    write_cmd(priv, 0x72);
    write_data(priv, 0x00);
    write_cmd(priv, 0x73);
    write_data(priv, 0x42);
    write_cmd(priv, 0x74);
    write_data(priv, 0xD9);
    write_cmd(priv, 0x75);
    write_data(priv, 0x00);
    write_cmd(priv, 0x76);
    write_data(priv, 0x00);
    write_cmd(priv, 0x77);
    write_data(priv, 0x00);
    write_cmd(priv, 0x78);
    write_data(priv, 0x00);
    write_cmd(priv, 0x79);
    write_data(priv, 0x00);
    write_cmd(priv, 0x7A);
    write_data(priv, 0x00);
    write_cmd(priv, 0x7B);
    write_data(priv, 0x00);
    write_cmd(priv, 0x80);
    write_data(priv, 0x48);
    write_cmd(priv, 0x81);
    write_data(priv, 0x00);
    write_cmd(priv, 0x82);
    write_data(priv, 0x07);
    write_cmd(priv, 0x83);
    write_data(priv, 0x02);
    write_cmd(priv, 0x84);
    write_data(priv, 0xD7);
    write_cmd(priv, 0x85);
    write_data(priv, 0x04);
    write_cmd(priv, 0x86);
    write_data(priv, 0x00);
    write_cmd(priv, 0x87);
    write_data(priv, 0x00);
    write_cmd(priv, 0x88);
    write_data(priv, 0x48);
    write_cmd(priv, 0x89);
    write_data(priv, 0x00);
    write_cmd(priv, 0x8A);
    write_data(priv, 0x09);
    write_cmd(priv, 0x8B);
    write_data(priv, 0x02);
    write_cmd(priv, 0x8C);
    write_data(priv, 0xD9);
    write_cmd(priv, 0x8D);
    write_data(priv, 0x04);
    write_cmd(priv, 0x8E);
    write_data(priv, 0x00);
    write_cmd(priv, 0x8F);
    write_data(priv, 0x00);
    write_cmd(priv, 0x90);
    write_data(priv, 0x48);
    write_cmd(priv, 0x91);
    write_data(priv, 0x00);
    write_cmd(priv, 0x92);
    write_data(priv, 0x0B);
    write_cmd(priv, 0x93);
    write_data(priv, 0x02);
    write_cmd(priv, 0x94);
    write_data(priv, 0xDB);
    write_cmd(priv, 0x95);
    write_data(priv, 0x04);
    write_cmd(priv, 0x96);
    write_data(priv, 0x00);
    write_cmd(priv, 0x97);
    write_data(priv, 0x00);
    write_cmd(priv, 0x98);
    write_data(priv, 0x48);
    write_cmd(priv, 0x99);
    write_data(priv, 0x00);
    write_cmd(priv, 0x9A);
    write_data(priv, 0x0D);
    write_cmd(priv, 0x9B);
    write_data(priv, 0x02);
    write_cmd(priv, 0x9C);
    write_data(priv, 0xDD);
    write_cmd(priv, 0x9D);
    write_data(priv, 0x04);
    write_cmd(priv, 0x9E);
    write_data(priv, 0x00);
    write_cmd(priv, 0x9F);
    write_data(priv, 0x00);
    write_cmd(priv, 0xA0);
    write_data(priv, 0x48);
    write_cmd(priv, 0xA1);
    write_data(priv, 0x00);
    write_cmd(priv, 0xA2);
    write_data(priv, 0x06);
    write_cmd(priv, 0xA3);
    write_data(priv, 0x02);
    write_cmd(priv, 0xA4);
    write_data(priv, 0xD6);
    write_cmd(priv, 0xA5);
    write_data(priv, 0x04);
    write_cmd(priv, 0xA6);
    write_data(priv, 0x00);
    write_cmd(priv, 0xA7);
    write_data(priv, 0x00);
    write_cmd(priv, 0xA8);
    write_data(priv, 0x48);
    write_cmd(priv, 0xA9);
    write_data(priv, 0x00);
    write_cmd(priv, 0xAA);
    write_data(priv, 0x08);
    write_cmd(priv, 0xAB);
    write_data(priv, 0x02);
    write_cmd(priv, 0xAC);
    write_data(priv, 0xD8);
    write_cmd(priv, 0xAD);
    write_data(priv, 0x04);
    write_cmd(priv, 0xAE);
    write_data(priv, 0x00);
    write_cmd(priv, 0xAF);
    write_data(priv, 0x00);
    write_cmd(priv, 0xB0);
    write_data(priv, 0x48);
    write_cmd(priv, 0xB1);
    write_data(priv, 0x00);
    write_cmd(priv, 0xB2);
    write_data(priv, 0x0A);
    write_cmd(priv, 0xB3);
    write_data(priv, 0x02);
    write_cmd(priv, 0xB4);
    write_data(priv, 0xDA);
    write_cmd(priv, 0xB5);
    write_data(priv, 0x04);
    write_cmd(priv, 0xB6);
    write_data(priv, 0x00);
    write_cmd(priv, 0xB7);
    write_data(priv, 0x00);
    write_cmd(priv, 0xB8);
    write_data(priv, 0x48);
    write_cmd(priv, 0xB9);
    write_data(priv, 0x00);
    write_cmd(priv, 0xBA);
    write_data(priv, 0x0C);
    write_cmd(priv, 0xBB);
    write_data(priv, 0x02);
    write_cmd(priv, 0xBC);
    write_data(priv, 0xDC);
    write_cmd(priv, 0xBD);
    write_data(priv, 0x04);
    write_cmd(priv, 0xBE);
    write_data(priv, 0x00);
    write_cmd(priv, 0xBF);
    write_data(priv, 0x00);
    write_cmd(priv, 0xC0);
    write_data(priv, 0x10);
    write_cmd(priv, 0xC1);
    write_data(priv, 0x47);
    write_cmd(priv, 0xC2);
    write_data(priv, 0x56);
    write_cmd(priv, 0xC3);
    write_data(priv, 0x65);
    write_cmd(priv, 0xC4);
    write_data(priv, 0x74);
    write_cmd(priv, 0xC5);
    write_data(priv, 0x88);
    write_cmd(priv, 0xC6);
    write_data(priv, 0x99);
    write_cmd(priv, 0xC7);
    write_data(priv, 0x01);
    write_cmd(priv, 0xC8);
    write_data(priv, 0xBB);
    write_cmd(priv, 0xC9);
    write_data(priv, 0xAA);
    write_cmd(priv, 0xD0);
    write_data(priv, 0x10);
    write_cmd(priv, 0xD1);
    write_data(priv, 0x47);
    write_cmd(priv, 0xD2);
    write_data(priv, 0x56);
    write_cmd(priv, 0xD3);
    write_data(priv, 0x65);
    write_cmd(priv, 0xD4);
    write_data(priv, 0x74);
    write_cmd(priv, 0xD5);
    write_data(priv, 0x88);
    write_cmd(priv, 0xD6);
    write_data(priv, 0x99);
    write_cmd(priv, 0xD7);
    write_data(priv, 0x01);
    write_cmd(priv, 0xD8);
    write_data(priv, 0xBB);
    write_cmd(priv, 0xD9);
    write_data(priv, 0xAA);
    write_cmd(priv, 0xF3);
    write_data(priv, 0x01);
    write_cmd(priv, 0xF0);
    write_data(priv, 0x00);
    write_cmd(priv, 0x3A);
    write_data(priv, 0x55);   //   5 565  6 666
    write_cmd(priv, 0x21);
    write_cmd(priv, 0x11);
    mdelay(120);
    write_cmd(priv, 0x29);
#endif

    return 0;
}

static struct tft_display st77916 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_st77916_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&st77916);
    return 0;
}

#endif
