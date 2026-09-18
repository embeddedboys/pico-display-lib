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

#include "indev.h"
#include "debug.h"

static struct indev_priv g_indev_priv;

static bool __indev_is_pressed(struct indev_priv *priv)
{
    if (priv->ops->is_pressed)
        return priv->ops->is_pressed(priv);
}

bool indev_is_pressed(void)
{
    return __indev_is_pressed(&g_indev_priv);
}

static void swap_float(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

/*
 * Touch pipeline, in one place:
 *
 *   controller -> axis order -> inversion -> board offset -> clamp
 *
 * The controller is expected to report in panel units (every spec in this
 * library sets x_res/y_res to the panel dimensions), so there is no scaling
 * step; what a rotation changes is only which controller axis feeds which
 * panel axis and whether it is inverted.  Recomputing the whole thing from
 * `dir` on every read is what makes indev_set_dir() idempotent -- the old
 * version swapped the ops, the resolutions and the spec fields in place, so
 * calling it twice put the axes back the way they started.
 */
indev_direction_t indev_dir_for_rotation(u8 rotation)
{
    switch (rotation) {
    case TFT_ROTATE_90:
        /* anchored on this module (320x480 native, driven at 480x320): panel
         * x comes from the controller's y, panel y from its inverted x */
        return INDEV_DIR_SWITCH_XY | INDEV_DIR_INVERT_Y;
    case TFT_ROTATE_180:
        return INDEV_DIR_INVERT_X | INDEV_DIR_INVERT_Y;
    case TFT_ROTATE_270:
        return INDEV_DIR_SWITCH_XY | INDEV_DIR_INVERT_X;
    case TFT_ROTATE_0:
    default:
        return INDEV_DIR_NOP;
    }
}

void indev_set_dir(indev_direction_t dir)
{
    struct indev_priv *priv = &g_indev_priv;

    priv->dir = dir;
    priv->switch_xy = (dir & INDEV_DIR_SWITCH_XY) != 0;
    priv->invert_x = (dir & INDEV_DIR_INVERT_X) != 0;
    priv->invert_y = (dir & INDEV_DIR_INVERT_Y) != 0;
}

/* the ops table passes the private struct, the public API does not */
static void indev_ops_set_dir(struct indev_priv *priv, indev_direction_t dir)
{
    (void)priv;
    indev_set_dir(dir);
}

/* One panel axis: `axis` 0 = x (panel width), 1 = y (panel height). */
static u16 indev_axis(struct indev_priv *priv, int axis)
{
    u16 max = (axis == 0) ? (TFT_HOR_RES - 1) : (TFT_VER_RES - 1);
    int16_t offs = (axis == 0) ? priv->spec->x_offs : priv->spec->y_offs;
    bool invert = (axis == 0) ? priv->invert_x : priv->invert_y;
    u16 v;

    if (axis == 0)
        v = priv->switch_xy ? priv->ops->read_y(priv)
                            : priv->ops->read_x(priv);
    else
        v = priv->switch_xy ? priv->ops->read_x(priv)
                            : priv->ops->read_y(priv);

    /* a controller can report one past the last row/column of its active
     * area, and a finger on the bezel can push it further out */
    if (v > max)
        v = max;

    if (invert)
        v = max - v;

    v = (u16)((int32_t)v + offs);
    if (v > max)
        v = max;

    return v;
}

u16 indev_read_x(void)
{
    return indev_axis(&g_indev_priv, 0);
}

u16 indev_read_y(void)
{
    return indev_axis(&g_indev_priv, 1);
}

static void indev_reset(struct indev_priv *priv)
{
    pr_debug("%s\n", __func__);
    dm_gpio_set_value(priv->spec->pin_rst, 1);
    mdelay(10);
    dm_gpio_set_value(priv->spec->pin_rst, 0);
    mdelay(10);
    dm_gpio_set_value(priv->spec->pin_rst, 1);
    mdelay(10);
}


static void indev_merge_ops(struct indev_ops *dst, struct indev_ops *src)
{
    if (src->write_reg)
        dst->write_reg = src->write_reg;
    if (src->read_reg)
        dst->read_reg = src->read_reg;

    if (src->write_reg16)
        dst->write_reg16 = src->write_reg16;
    if (src->read_reg16)
        dst->read_reg16 = src->read_reg16;

    if (src->write_addr16)
        dst->write_addr16 = src->write_addr16;
    if (src->read_addr16)
        dst->read_addr16 = src->read_addr16;

    if (src->init)
        dst->init = src->init;
    if (src->reset)
        dst->reset = src->reset;
    if (src->set_dir)
        dst->set_dir = src->set_dir;
    if (src->is_pressed)
        dst->is_pressed = src->is_pressed;
    if (src->read_x)
        dst->read_x = src->read_x;
    if (src->read_y)
        dst->read_y = src->read_y;
}

int indev_probe(struct indev_spec *spec)
{
    struct indev_priv *priv = &g_indev_priv;

    pr_debug("%s\n", __func__);

    if (!spec->name) {
        printf("ERROR: set indev spec a name!\n");
        return -1;
    }

    printf("%s, %s driver probing...\n", __func__, spec->name);

    priv->spec = spec;

    priv->ops = (struct indev_ops *)malloc(sizeof(struct indev_ops));
    if (!priv->ops) {
        pr_error("failed to allocate memory!\n");
        return -1;
    }

    /* panel dimensions: what the transform clamps to.  sc_x/sc_y are kept for
     * compatibility but the transform works in panel units (see indev_axis) */
    priv->x_res = TFT_HOR_RES;
    priv->y_res = TFT_VER_RES;
    priv->sc_x = 1.0f;
    priv->sc_y = 1.0f;

    priv->ops->reset = indev_reset;
    priv->ops->set_dir = indev_ops_set_dir;

    /* touch follows the display rotation unless the application says
     * otherwise after indev_driver_init() */
    indev_set_dir(indev_dir_for_rotation(TFT_ROTATION));
    pr_debug("%s, rotation %d -> dir 0x%02x", __func__, TFT_ROTATION,
             priv->dir);

    indev_merge_ops(priv->ops, &spec->ops);

    priv->ops->init(priv);

    return 0;
}
