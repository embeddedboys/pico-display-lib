# Backlight.  The pin is only emitted when the panel config names it; the active
# level is a plain 0/1 switch.
panel_emit_value(TFT_PIN_BLK)
panel_emit_flag(TFT_BLK_ACTIVE_HIGH)

panel_require(TFT_PIN_BLK)
