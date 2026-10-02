set(OVERCLOCK_ENABLED 1)    # 1: enable, 0: disable

if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    # Overclocking profiles
    #      SYS_CLK  | FLASH_CLK | Voltage
    #  1  | 240MHz  |  120MHZ   |  1.10(V) (default, stable, recommended for most devices)
    #  2  | 266MHz  |  133MHz   |  1.10(V)
    #  3  | 360MHz  |  90MHz    |  1.20(V)
    #  4  | 400MHz  |  100MHz   |  1.30(V)
    #  5  | 416MHz  |  104MHz   |  1.30(V)
    set(OVERCLOCK_PROFILE 4)
elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
    # Overclocking profiles
    #      SYS_CLK  | FLASH_CLK | Voltage
    #  1  | 225MHz  |   75MHz   |  1.10(V) (default, stable, recommended for most devices)
    #  2  | 366MHz  |  122MHz   |  1.20(V)
    #  3  | 384MHz  |  128MHz   |  1.20(V)
    set(OVERCLOCK_PROFILE 1)
endif()

set(TFT_BUS_TYPE 0)
#     | Bus type | Description
#  0  |   SPI    | Serial Peripheral Interface
#  1  |  I8080   | Intel 8080 interface
#  2  |   I2C    | Inter-Integrated Circuit (Not implemented yet)

# /* Default TFT pins for SPI interface */
set(TFT_SPIX      0)    # set this if spi controller will be used.
set(TFT_PIN_SCL   18)
set(TFT_PIN_SDA   19)
set(TFT_PIN_RES   15)
set(TFT_PIN_DC    14)
set(TFT_PIN_CS    13)
set(TFT_PIN_BLK   12)

# TFT backlight behavior, 0 : active low, 1 : active high
set(TFT_BLK_ACTIVE_HIGH 1)

set(TFT_HOR_RES   128)
set(TFT_VER_RES   160)

# ST7735 modules put their visible area inside a larger GRAM, and where it sits
# depends on the glass, not the controller.  The offsets below are the ones this
# 128x160 module needs; they are the knob to turn if the image is shifted.
#
#   0, 0   plain 128x160 GRAM                     <- this config
#   2, 1   "green tab" ST7735R: 132x162 GRAM      <- the other common variant
#
# Consequence of getting them wrong is a border of untouched pixels on one or two
# edges, not a broken image, so it is worth trying the other pair before
# suspecting the bus.  A 128x128 or 80x160 module on the same glass is the same
# file with TFT_HOR_RES/TFT_VER_RES changed and the offsets moved accordingly
# (see md144_spi_v04 and fpc-zh096g1321).
set(TFT_X_OFFS  0)
set(TFT_Y_OFFS  0)

# Rotation configuration
set(TFT_ROTATION 0)  # 0: normal, 1: 90 degree, 2: 180 degree, 3: 270 degree

# Swap the 2 bytes of RGB565 color. Useful if the display has an 8-bit interface (e.g. SPI)
set(TFT_COLOR_16_SWAP 1)

# MADCTL bit 3.  This glass is RGB, not BGR: with the default (1) red and blue
# come out exchanged and every other colour is correct, which is the signature of
# this bit rather than of the byte order.
set(TFT_BGR 0)

set(TFT_BUS_CLK_KHZ 75000)
set(DISP_OVER_PIO 1)
set(PIO_USE_DMA 1)

set(TFT_DRV_USE_ST7735 1)
# The model flag picks the *init sequence variant*, not the glass vendor, and on
# this driver the two differ in MADCTL:
#
#   TFT_MODEL_GENERIC       0x36 = 0xE0  (MY|MX|MV) -> MV swaps the axes: landscape
#   TFT_MODEL_MD144_SPI_V04 0x36 = 0xC8  (MY|MX)    -> portrait
#
# A 128x160 portrait module needs the second one; with the first, an upright
# pattern comes out rotated by 90 degrees and part of the panel keeps whatever
# was on it.  The driver has no set_dir(), so TFT_ROTATION cannot compensate --
# the sequence itself is the only place this is decided.
set(TFT_MODEL_MD144_SPI_V04 1)

set(INDEV_BUS_TYPE 0)
#     | Bus type | Description
#  0  |   I2C    | Inter-Integrated Circuit
#  1  |   SPI    | Serial Peripheral Interface (Not implemented yet)

# /* Default Bus pins for Input device */
set(INDEV_DRV_NOT_USED    1)  # 1: not used, 0: used
