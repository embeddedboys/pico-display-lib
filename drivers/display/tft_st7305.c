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

#include <string.h>
#include "tft.h"
#include "debug.h"

#if TFT_DRV_USE_ST7305

static u8 buffer[TFT_VER_RES * 21];
static u8 temp_buffer[(TFT_HOR_RES + 24) * 14 * 3];

static int tft_st7305_init_display(struct tft_priv *priv)
{
    printf("%s, writing patched initial sequence...\n", __func__);
    priv->tftops->reset(priv);

    write_cmd(priv, 0xD6); write_data(priv, 0x13); write_data(priv, 0x02);  // NVM Load Control
    write_cmd(priv, 0xD1); write_data(priv, 0x01);  // Booster Enable
    write_cmd(priv, 0xC0); write_data(priv, 0x08); write_data(priv, 0x06);  // Gate Voltage Setting

    write_cmd(priv, 0xC1);  // VSHP Setting (4.8V)
    write_data(priv, 0x3C); write_data(priv, 0x3E); write_data(priv, 0x3C); write_data(priv, 0x3C);

    write_cmd(priv, 0xC2);  // VSLP Setting (0.98V)
    write_data(priv, 0x23); write_data(priv, 0x21); write_data(priv, 0x23); write_data(priv, 0x23);

    write_cmd(priv, 0xC4);  // VSHN Setting (-3.6V)
    write_data(priv, 0x5A); write_data(priv, 0x5C); write_data(priv, 0x5A); write_data(priv, 0x5A);

    write_cmd(priv, 0xC5);  // VSLN Setting (0.22V)
    write_data(priv, 0x37); write_data(priv, 0x35); write_data(priv, 0x37); write_data(priv, 0x37);

    // HPM=31Hz
    // write_reg(priv, 0xD8, 0xA6, 0xE9);

    // HPM=51Hz
    write_reg(priv, 0xD8, 0x80, 0xE9);

    // Frame Rate Control
    write_cmd(priv, 0xB2);
    write_data(priv, 0X02); // HPM=25.5hz  LPM=1hz

    // Update Period Gate EQ Control in HPM
    write_cmd(priv, 0xB3);
    u8 b3_data[] = {0xE5, 0xF6, 0x17, 0x77, 0x77, 0x77, 0x77, 0x77, 0x77, 0x71};
    write_buf_dc(priv, b3_data, sizeof(b3_data), 1);

    // Update Period Gate EQ Control in LPM
    write_cmd(priv, 0xB4);
    u8 b4_data[] = {0x05, 0x46, 0x77, 0x77, 0x77, 0x77, 0x76, 0x45};
    write_buf_dc(priv, b4_data, sizeof(b4_data), 1);

    write_cmd(priv, 0x62); // Gate Timing Control
    u8 g62_data[] = {0x32, 0x03, 0x1F};
    write_buf_dc(priv, g62_data, sizeof(g62_data), 1);

    write_cmd(priv, 0xB7); write_data(priv, 0x13);  // Source EQ Enable
    write_cmd(priv, 0xB0); write_data(priv, 0x60);  // Gate Line Setting: 384 line

    write_cmd(priv, 0x11);  // Sleep out
    mdelay(120);

    write_cmd(priv, 0xC9); write_data(priv, 0x00);  // Source Voltage Select
    write_cmd(priv, 0x36); write_data(priv, 0x00);  // Memory Data Access Control
    write_cmd(priv, 0x3A); write_data(priv, 0x11);  // Data Format Select
    write_cmd(priv, 0xB9); write_data(priv, 0x20);  // Gamma Mode Setting
    write_cmd(priv, 0xB8); write_data(priv, 0x29);  // Panel Setting

    write_cmd(priv, 0x2A); write_data(priv, 0x17); write_data(priv, 0x24);  // Column Address Setting
    write_cmd(priv, 0x2B); write_data(priv, 0x00); write_data(priv, 0xBF);  // Row Address Setting

    write_cmd(priv, 0xD0); write_data(priv, 0xFF);  // Auto power down
    write_cmd(priv, 0x38);  // Frame rate mode : HPM
    // write_cmd(priv, 0x39);  // Frame rate mode : LPM
    write_cmd(priv, 0x29);  // Display on

    return 0;
}

static void convert_buffer(void)
{
    u16 k = 0;
    for (u16 i = 0; i < TFT_VER_RES; i += 2) {
        // Convert 2 columns
        for (u16 j = 0; j < 21; j += 3) {
            for (u8 y = 0; y < 3; y++) {
                u8 b1 = buffer[(j + y) * TFT_VER_RES + i];
                u8 b2 = buffer[(j + y) * TFT_VER_RES + i + 1];

                // First 4 bits
                u8 mix = 0;
                mix |= ((b1 & 0x01) << 7);
                mix |= ((b2 & 0x01) << 6);
                mix |= ((b1 & 0x02) << 4);
                mix |= ((b2 & 0x02) << 3);
                mix |= ((b1 & 0x04) << 1);
                mix |= ((b2 & 0x04) << 0);
                mix |= ((b1 & 0x08) >> 2);
                mix |= ((b2 & 0x08) >> 3);
                temp_buffer[k++] = mix;

                // Second 4 bits
                b1 >>= 4;
                b2 >>= 4;
                mix = 0;
                mix |= ((b1 & 0x01) << 7);
                mix |= ((b2 & 0x01) << 6);
                mix |= ((b1 & 0x02) << 4);
                mix |= ((b2 & 0x02) << 3);
                mix |= ((b1 & 0x04) << 1);
                mix |= ((b2 & 0x04) << 0);
                mix |= ((b1 & 0x08) >> 2);
                mix |= ((b2 & 0x08) >> 3);
                temp_buffer[k++] = mix;
            }
        }
    }
}

static uint8_t rgb565_to_brightness(u16 color)
{
    u8 r = (color >> 11) & 0x1f;
    u8 g = (color >> 5) & 0x3f;
    u8 b = color & 0x1f;

    r = (r * 255 + 15) / 31;
    g = (g * 255 + 31) / 63;
    b = (b * 255 + 15) / 31;

    return (uint8_t)((77 * r + 150 * g + 29 * b) >> 8);
}

static void put_pixel(int x, int y, u16 color, u8 rotation)
{
    u16 new_x, new_y;

    switch (rotation) {
        case 1:  // 90 degrees
            new_x = TFT_VER_RES - y - 1;
            new_y = x;
            break;
        case 2:  // 180 degrees
            new_x = TFT_HOR_RES - x - 1;
            new_y = TFT_VER_RES - y - 1;
            break;
        case 3:  // 270 degrees
            new_x = y;
            new_y = TFT_HOR_RES - x - 1;
            break;
        default: // 0 degrees
            new_x = x;
            new_y = y;
            break;
    }

    u16 byte_idx = (new_y >> 3) * TFT_VER_RES + new_x;
    u8 bit_pos = new_y & 0x07;

    if (rgb565_to_brightness(color) < 128)
        buffer[byte_idx] |= (1 << bit_pos);
    else
        buffer[byte_idx] &= ~(1 << bit_pos);
}

static void tft_st7305_refresh(struct tft_priv *priv)
{
    convert_buffer();

    u8 caset[] = {0x17, 0x17 + 14 - 1};
    u8 raset[] = {0x00, 0x00 + 192 - 1};

    write_reg(priv, 0x2A, caset[0], caset[1]);
    write_reg(priv, 0x2B, raset[0], raset[1]);
    write_reg(priv, 0x2C);

    write_buf_dc(priv, temp_buffer, sizeof(temp_buffer), 1);
}

static int tft_st7567_clear(struct tft_priv *priv, u16 color)
{
    if (rgb565_to_brightness(color) > 128)
        memset(buffer, 0xff, TFT_VER_RES * 21);
    else
        memset(buffer, 0, TFT_VER_RES * 21);

    tft_st7305_refresh(priv);
    return 0;
}

static void tft_st7305_video_sync(struct tft_priv *priv, int xs, int ys, int xe, int ye, void *vmem, size_t len)
{
    pr_debug("video sync: xs=%d, ys=%d, xe=%d, ye=%d, len=%d\n", xs, ys, xe, ye, len);
    u16 *vmem16 = (u16 *)vmem;
    int x, y, i = 0;
    u8 rotation = 3;

    for (y = ys; y < ye; y++) {
        for (x = xs; x < xe; x++) {
            put_pixel(x, y, vmem16[i++], rotation);
        }
    }

    tft_st7305_refresh(priv);
}

static struct tft_display st7305 = {
    .xres = TFT_X_RES,
    .yres = TFT_Y_RES,
    .bpp  = 1,
    .backlight = 100,
    .tftops = {
        .write_reg = tft_write_reg8,
        .init_display = tft_st7305_init_display,
        .clear = tft_st7567_clear,
        .video_sync = tft_st7305_video_sync,
    },
};

int tft_driver_init(void)
{
    tft_probe(&st7305);
    return 0;
}

#endif