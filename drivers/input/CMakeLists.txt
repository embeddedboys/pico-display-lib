# Touch input.  The controller selection flags are 0/1 switches; the pins are only
# emitted when the panel config provides them, so the optional ones can fall back
# to the defaults in include/config.h.  INDEV_PIN_IRQ is one of those: not every
# board wires it, and emitting it unset produced `-DINDEV_PIN_IRQ=""` -- a string
# literal that beats the #ifndef default and only fails, confusingly, if the
# driver that uses it is ever enabled.
panel_emit_value(
    INDEV_POLLING_PERIOD_MS
    INDEV_PIN_SCL
    INDEV_PIN_SDA
    INDEV_PIN_RST
    INDEV_PIN_IRQ
)

panel_emit_flag(
    INDEV_DRV_NOT_USED
    INDEV_DRV_USE_CST816D
    INDEV_DRV_USE_GT911
    INDEV_DRV_USE_TSC2007
    INDEV_DRV_USE_FT6236
)

# GT911 needs its interrupt line; asking for it here is what turns "enabled the
# driver, forgot the pin" from a compile error inside a driver into a configure
# error naming the panel config.
if(INDEV_DRV_USE_GT911)
    panel_require(INDEV_PIN_IRQ)
endif()
