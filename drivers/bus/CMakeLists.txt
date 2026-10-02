# Copyright (c) 2024 embeddedboys developers
#
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

panel_emit_value(TFT_BUS_TYPE)

if(TFT_BUS_TYPE EQUAL 0) # SPI

    add_library(pio_spi_tx STATIC pio_spi_tx.c)
    pico_generate_pio_header(pio_spi_tx ${CMAKE_CURRENT_LIST_DIR}/pio_spi_tx.pio)
    target_compile_definitions(pio_spi_tx PRIVATE
        TFT_PIN_DC=${TFT_PIN_DC}
        TFT_PIN_CS=${TFT_PIN_CS}
        DEFAULT_PIO_CLK_KHZ=${PERI_CLK_KHZ}
        TFT_BUS_CLK_KHZ=${TFT_BUS_CLK_KHZ}
        PIO_USE_DMA=${PIO_USE_DMA}
    )
    target_link_libraries(pio_spi_tx PRIVATE
        pico_stdlib
        hardware_pio
        hardware_dma
    )

    panel_emit_value(TFT_PIN_SCL TFT_PIN_SDA TFT_PIN_RES TFT_PIN_DC TFT_PIN_CS)
    panel_require(TFT_PIN_SCL TFT_PIN_SDA TFT_PIN_RES TFT_PIN_DC TFT_PIN_CS)

elseif(TFT_BUS_TYPE EQUAL 1) # I8080

    add_library(pio_i80 STATIC pio_i80.c)
    pico_generate_pio_header(pio_i80 ${CMAKE_CURRENT_LIST_DIR}/pio_i80.pio)
    target_compile_definitions(pio_i80 PRIVATE
        TFT_PIN_DB_COUNT=${TFT_PIN_DB_COUNT}
        TFT_PIN_RS=${TFT_PIN_RS}
        TFT_PIN_CS=${TFT_PIN_CS}
        DEFAULT_PIO_CLK_KHZ=${PERI_CLK_KHZ}
        TFT_BUS_CLK_KHZ=${TFT_BUS_CLK_KHZ}
        PIO_USE_DMA=${PIO_USE_DMA}
    )
    target_link_libraries(pio_i80 PRIVATE
        pico_stdlib
        hardware_pio
        hardware_dma
    )

    panel_emit_value(TFT_PIN_DB_BASE TFT_PIN_DB_COUNT TFT_PIN_CS TFT_PIN_WR TFT_PIN_RS TFT_PIN_RES)
    panel_require(TFT_PIN_DB_BASE TFT_PIN_DB_COUNT TFT_PIN_CS TFT_PIN_WR TFT_PIN_RS TFT_PIN_RES)

else()
    # The library chooses PIO_LIB in drivers/bus/config.cmake, which runs before
    # this, so an unusable bus type is already rejected by the time we get here.
    message(FATAL_ERROR
        "panel config '${PUD_CONFIG}': TFT_BUS_TYPE '${TFT_BUS_TYPE}' has no driver")
endif()
