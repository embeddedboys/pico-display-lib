add_subdirectory(epaper)

panel_require(TFT_HOR_RES TFT_VER_RES TFT_ROTATION TFT_BUS_CLK_KHZ)

# Rotation swaps the axes, so TFT_X_RES/TFT_Y_RES are the *rotated* geometry --
# which is what the C side means by TFT_HOR_RES/TFT_VER_RES.  The config's own
# TFT_HOR_RES/TFT_VER_RES stay native; only the macros are swapped.
if(${TFT_ROTATION} EQUAL 0 OR ${TFT_ROTATION} EQUAL 2)
    set(TFT_X_RES ${TFT_HOR_RES})
    set(TFT_Y_RES ${TFT_VER_RES})
elseif(${TFT_ROTATION} EQUAL 1 OR ${TFT_ROTATION} EQUAL 3)
    set(TFT_X_RES ${TFT_VER_RES})
    set(TFT_Y_RES ${TFT_HOR_RES})
else()
    message(FATAL_ERROR
        "panel config '${PUD_CONFIG}': TFT_ROTATION must be 0..3, got '${TFT_ROTATION}'")
endif()

message(STATUS "panel ${PUD_CONFIG}: ${TFT_X_RES}x${TFT_Y_RES}, rotation ${TFT_ROTATION}")

panel_emit_value(
    TFT_X_RES=TFT_HOR_RES
    TFT_Y_RES=TFT_VER_RES
    TFT_ROTATION
    TFT_X_OFFS
    TFT_Y_OFFS
    TFT_BUS_CLK_KHZ
    DISP_OVER_PIO
    TFT_BGR
)

panel_emit_flag(TFT_COLOR_16_SWAP TFT_MODEL_GENERIC)

# Panel modules within a controller family.  These are plain 0/1 switches like
# every other flag; while they were emitted raw, a config that did not set one
# got `-DTFT_MODEL_X=""` -- a string literal, which disabled the matching #ifndef
# default in config.h and made `#elif TFT_MODEL_X` a preprocessor error.  They
# only compiled because every such site sits inside `#if TFT_DRV_USE_<driver>`
# and those drivers happened to be off.
panel_emit_flag(
    TFT_MODEL_MD144_SPI_V04
    TFT_MODEL_P169H002
    TFT_MODEL_YT280S030
    TFT_MODEL_YT350S006
    TFT_MODEL_W180TE010I_18Z
)

# Controller selection.  One flag per driver, and this list is the single place a
# new controller has to be registered.
panel_emit_flag(
    TFT_DRV_USE_ILI9341
    TFT_DRV_USE_ILI9481
    TFT_DRV_USE_ILI9488
    TFT_DRV_USE_ST7735
    TFT_DRV_USE_ST7789V
    TFT_DRV_USE_ST7796
    TFT_DRV_USE_ST77916
    TFT_DRV_USE_SSD1306
    TFT_DRV_USE_SSD1327
    TFT_DRV_USE_SSD1681
    TFT_DRV_USE_GC9A01
    TFT_DRV_USE_R61581
    TFT_DRV_USE_ST6201
    TFT_DRV_USE_ST7305
    TFT_DRV_USE_ST7576
    TFT_DRV_USE_1P5623
)
