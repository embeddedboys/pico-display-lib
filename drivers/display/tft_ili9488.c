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

#if TFT_DRV_USE_ILI9488

static int tft_ili9488_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    // write_reg(priv, 0xE0, 0x00, 0x09, 0x16, 0x0A, 0x1B, 0x0A, 0x41, 0x58, 0x50, 0x02, 0x08, 0x0D, 0x20, 0x24, 0x0F);
    // write_reg(priv, 0xE1, 0x00, 0x1B, 0x1E, 0x04, 0x0D, 0x02, 0x33, 0x23, 0x44, 0x03, 0x11, 0x0A, 0x30, 0x36, 0x0F);

    // write_reg(priv, 0xB0, 0x8A);
    // write_reg(priv, 0xB1, 0xA0);
    // write_reg(priv, 0xB4, 0x02);

    // write_reg(priv, 0x35, 0x00);

    // write_reg(priv, 0xC0, 0x10, 0x10);
    // write_reg(priv, 0xC1, 0x41);
    // write_reg(priv, 0xC5, 0x00, 0x30, 0x80);

    // write_reg(priv, 0x36, 0x08 | (1 << 5));
    // write_reg(priv, 0x3A, 0x06);

    // write_reg(priv, 0xB6, 0x02, 0x02);
    // write_reg(priv, 0xB7, 0xC6);
    // write_reg(priv, 0xBE, 0x00, 0x04);

    // write_reg(priv, 0xE9, 0x00);
    // write_reg(priv, 0xF7, 0xA9, 0x51, 0x2C, 0x82);

    // write_reg(priv, 0x11);
    // mdelay(120);
    // write_reg(priv, 0x29);

    write_reg(priv, 0xE0, 0x00, 0x03, 0x09, 0x08, 0x16, 0x0A, 0x3F, 0x78, 0x4C, 0x09, 0x0A, 0x08, 0x16, 0x1A, 0x0F);

    // Negative Gamma Control
    write_reg(priv, 0xE1, 0x00, 0x16, 0x19, 0x03, 0x0F, 0x05, 0x32, 0x45, 0x46, 0x04, 0x0E, 0x0D, 0x35, 0x37, 0x0F);

    write_reg(priv, 0xC0, 0x17, 0x15);          // Power Control 1
    write_reg(priv, 0xC1, 0x41);                // Power Control 2
    write_reg(priv, 0xC5, 0x00, 0x12, 0x80);    // VCOM Control
    // write_reg(priv, 0x36, 0x28);                // Memory Access Control
    write_reg(priv, 0x3A, 0x55);                // Pixel Interface Format RGB565 8080 16-bit
    write_reg(priv, 0xB0, 0x00);                // Interface Mode Control

    // Frame Rate Control
    // write_reg(priv, 0xB1, 0xD0, 0x11);       // 60Hz
    write_reg(priv, 0xB1, 0xD0, 0x14);          // 90Hz

    write_reg(priv, 0xB4, 0x02);                // Display Inversion Control
    write_reg(priv, 0xB6, 0x02, 0x02, 0x3B);    // Display Function Control
    write_reg(priv, 0xB7, 0xC6);                // Entry Mode Set
    write_reg(priv, 0xF7, 0xA9, 0x51, 0x2C, 0x82);  // Adjust Control 3
    write_reg(priv, 0x11);                      // Exit Sleep
    mdelay(60);
    write_reg(priv, 0x29);                      // Display on

    return 0;
}

static int tft_ili9488_set_dir(struct tft_priv *priv, u8 dir)
{
    printf("setting display rotation to %d\n", dir);

    switch (dir) {
    case TFT_ROTATE_0:
        write_reg(priv, MADCTL, MX | BGR);
        break;
    case TFT_ROTATE_90:
        write_reg(priv, MADCTL, MV | BGR);
        break;
    case TFT_ROTATE_180:
        write_reg(priv, MADCTL, MY | BGR);
        break;
    case TFT_ROTATE_270:
        write_reg(priv, MADCTL, MX | MY | MV | BGR);
        break;
    default:
        break;
    }

    return 0;
}

static struct tft_display ili9488 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 16,
    .backlight = 100,
    .rotate = TFT_ROTATION,
    .tftops = {
#if TFT_BUS_TYPE == TFT_BUS_TYPE_SPI
        .write_reg = tft_write_reg8,
#elif TFT_BUS_TYPE == TFT_BUS_TYPE_I80

#if TFT_PIN_DB_COUNT == 8
        .write_reg = tft_write_reg8,
#elif TFT_PIN_DB_COUNT == 16
        .write_reg = tft_write_reg16,
#else
#error "TFT_PIN_DB_COUNT must be 8 or 16"
#endif

#endif
        .set_dir = tft_ili9488_set_dir,
        .init_display = tft_ili9488_init_display,
    },
};

int tft_driver_init(void)
{
    tft_probe(&ili9488);
    return 0;
}

#endif
