set(OVERCLOCK_ENABLED 1)    # 1: enable, 0: disable

if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
    # Overclocking profiles
    #      SYS_CLK  | FLASH_CLK | Voltage
    #  1  | 240MHz  |  120MHZ   |  1.10(V) (default, stable, recommended for most devices)
    #  2  | 266MHz  |  133MHz   |  1.10(V)
    #  3  | 360MHz  |  90MHz    |  1.20(V)
    #  4  | 400MHz  |  100MHz   |  1.30(V)
    #  5  | 416MHz  |  104MHz   |  1.30(V)
    set(OVERCLOCK_PROFILE 1)
elseif(${PICO_BOARD} STREQUAL  "pico2" OR ${PICO_PLATFORM} STREQUAL  "rp2350")
    # Overclocking profiles
    #      SYS_CLK  | FLASH_CLK | Voltage
    #  1  | 225MHz  |   75MHz   |  1.10(V) (default, stable, recommended for most devices)
    #  2  | 366MHz  |  122MHz   |  1.20(V)
    #  3  | 384MHz  |  128MHz   |  1.20(V)
    set(OVERCLOCK_PROFILE 2)
endif()

set(TFT_BUS_TYPE 1)
#     | Bus type | Description
#  0  |   SPI    | Serial Peripheral Interface
#  1  |  I8080   | Intel 8080 interface
#  2  |   I2C    | Inter-Integrated Circuit (Not implemented yet)

# /* Default TFT pins for SPI interface */
# set this if spi controller will be used.
# set(TFT_SPIX      0)
# set(TFT_PIN_SCL   18)
# set(TFT_PIN_SDA   19)
# set(TFT_PIN_RES   15)
# set(TFT_PIN_DC    14)
# set(TFT_PIN_CS    13)

# /* Default TFT Pins for I8080 interface */
set(TFT_PIN_DB_BASE  0)  # 8080 TFT data bus base pin
set(TFT_PIN_DB_COUNT 16) # 8080 TFT data bus pin count
set(TFT_PIN_CS  18)  # 8080 TFT chip select pin
set(TFT_PIN_WR  19)  # 8080 TFT write pin
set(TFT_PIN_RS  20)  # 8080 TFT register select pin
set(TFT_PIN_RES 22)  # 8080 TFT reset pin
set(TFT_PIN_BLK 28)

# TFT backlight behavior, 0 : active low, 1 : active high
set(TFT_BLK_ACTIVE_HIGH 1)

set(TFT_HOR_RES   480)
set(TFT_VER_RES   320)

# Rotation configuration
set(TFT_ROTATION 1)  # 0: normal, 1: 90 degree, 2: 180 degree, 3: 270 degree

set(TFT_BUS_CLK_KHZ 18000)
set(DISP_OVER_PIO 1)
set(PIO_USE_DMA 1)

set(TFT_DRV_USE_1P5623 1)

set(INDEV_BUS_TYPE 0)
#     | Bus type | Description
#  0  |   I2C    | Inter-Integrated Circuit
#  1  |   SPI    | Serial Peripheral Interface (Not implemented yet)

set(INDEV_DRV_NOT_USED    1)  # 1: not used, 0: used
