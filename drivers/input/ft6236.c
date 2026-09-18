// Copyright (c) 2024 embeddedboys developers

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:

// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <stdio.h>

#include "indev.h"
#include "debug.h"

#if INDEV_DRV_USE_FT6236

#define FT6236_ADDR      0x38
#define FT6236_DEF_SPEED 400000

#define CT_MAX_TOUCH  5

#define FT_REG_DEVICE_MODE 	    0x00    // Device mode, 0x00: Normal mode, 0x04: Test mode, 0x03: Factory mode
#define FT_REG_GEST_ID 			0x01    // Gesture ID
#define FT_REG_TD_STATUS 		0x02    // Touch point status

/* TODO: currently support 1 touch point */
#define FT_REG_TOUCH1_XH 		0x03    // Touch point 1 X high 8-bit
#define FT_REG_TOUCH1_XL 		0x04    // Touch point 1 X low 8-bit
#define FT_REG_TOUCH1_YH 		0x05    // Touch point 1 Y high 8-bit
#define FT_REG_TOUCH1_YL 		0x06    // Touch point 1 Y low 8-bit

#define FT_REG_TH_GROUP			0x80
#define FT_REG_PERIODACTIVE	    0x88

#define	FT_REG_LIB_VER_H		0xA1
#define	FT_REG_LIB_VER_L		0xA2
#define FT_REG_CHIPER           0xA3
#define FT_REG_G_MODE 			0xA4
#define FT_REG_FOCALTECH_ID     0xA8
#define FT_REG_RELEASE_CODE_ID  0xAF
#define FT_REG_STATE            0xBC

extern int i2c_bus_scan(i2c_inst_t *i2c);

static void ft6236_write_reg(struct indev_priv *priv, uint8_t reg, uint8_t val)
{
    uint16_t buf = val << 8 | reg;
    i2c_write_blocking(priv->spec->i2c.master, priv->spec->i2c.addr, (uint8_t *)&buf, sizeof(buf), false);
}

static uint8_t ft6236_read_reg(struct indev_priv *priv, uint8_t reg)
{
    uint8_t val;
    i2c_write_blocking(priv->spec->i2c.master, priv->spec->i2c.addr, &reg, 1, true);
    i2c_read_blocking(priv->spec->i2c.master, priv->spec->i2c.addr, &val, 1, false);
    return val;
}

/*
 * XH/YH carry the coordinate in bits [3:0] only: bits [7:6] are the event flag
 * and bits [5:4] the touch id, so both registers have to be masked before the
 * 12-bit coordinate is assembled.  Masking XH with 0x1f (keeping the touch id
 * bit) put a spurious 0x1000 into every sample of a gesture -- measured on the
 * panel as x jumping from 193 to 4136 mid-drag and staying 4096 too high for
 * the rest of it (notes/usb-protocol.md in Pico-USB-Display has the capture).
 */
#define FT_TOUCH_COORD_MASK 0x0f

/* Raw controller reads: the axis order, the rotation's inversion, the offset
 * and the clamp are the core's job (drivers/input/indev.c). */
static uint16_t ft6236_read_x(struct indev_priv *priv)
{
    uint8_t val_h = read_reg(priv, FT_REG_TOUCH1_XH) & FT_TOUCH_COORD_MASK;
    uint8_t val_l = read_reg(priv, FT_REG_TOUCH1_XL);

    return (val_h << 8) | val_l;
}

static uint16_t ft6236_read_y(struct indev_priv *priv)
{
    uint8_t val_h = read_reg(priv, FT_REG_TOUCH1_YH) & FT_TOUCH_COORD_MASK;
    uint8_t val_l = read_reg(priv, FT_REG_TOUCH1_YL);

    return (val_h << 8) | val_l;
}

static bool ft6236_is_pressed(struct indev_priv *priv)
{
    /* bits [3:0] are the number of touch points, the rest is reserved */
    uint8_t val = read_reg(priv, FT_REG_TD_STATUS) & 0x0f;
    return val;
}

static void ft6236_hw_init(struct indev_priv *priv)
{
    pr_debug("%s\n", __func__);

    pr_debug("initialzing i2c controller\n");
    i2c_init(priv->spec->i2c.master, FT6236_DEF_SPEED);

    pr_debug("set gpio i2c function\n");
    gpio_init(priv->spec->i2c.pin_scl);
    gpio_init(priv->spec->i2c.pin_sda);
    gpio_set_function(priv->spec->i2c.pin_scl, GPIO_FUNC_I2C);
    gpio_set_function(priv->spec->i2c.pin_sda, GPIO_FUNC_I2C);

    pr_debug("pull up i2c gpio\n");
    gpio_pull_up(priv->spec->i2c.pin_scl);
    gpio_pull_up(priv->spec->i2c.pin_sda);

    pr_debug("initialzing reset pin\n");
    gpio_init(priv->spec->pin_rst);
    gpio_set_dir(priv->spec->pin_rst, GPIO_OUT);
    gpio_pull_up(priv->spec->pin_rst);

    pr_debug("chip reset\n");
    priv->ops->reset(priv);
    /* the direction comes from the display rotation (indev_probe) */

    // i2c_bus_scan(priv->spec->i2c.master);

    /* registers are read-only */
    // write_reg(priv, FT_REG_DEVICE_MODE, 0x00);
    // write_reg(priv, FT_REG_TH_GROUP, 22);
    // write_reg(priv, FT_REG_PERIODACTIVE, 12);
}

static struct indev_spec ft6236 = {
    .name = "ft6236",
    .type = INDEV_TYPE_POINTER,

    .i2c = {
        .addr = FT6236_ADDR,
        .master = i2c1,
        .speed = FT6236_DEF_SPEED,
        .pin_scl = INDEV_PIN_SCL,
        .pin_sda = INDEV_PIN_SDA,
    },

    .x_res = TFT_HOR_RES,
    .y_res = TFT_VER_RES,

    // .pin_irq = INDEV_PIN_IRQ,
    .pin_rst = INDEV_PIN_RST,

    .ops = {
        .write_reg  = ft6236_write_reg,
        .read_reg   = ft6236_read_reg,
        .init       = ft6236_hw_init,
        .is_pressed = ft6236_is_pressed,
        .read_x     = ft6236_read_x,
        .read_y     = ft6236_read_y,
    }
};

int indev_driver_init(void)
{
    printf("%s\n", __func__);
    indev_probe(&ft6236);
    return 0;
}

#endif