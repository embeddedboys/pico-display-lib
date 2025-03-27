if(TFT_BUS_TYPE EQUAL 0) # SPI

    message(WARNING "SPI bus type selected.")
    set(PIO_LIB pio_spi_tx)

elseif(TFT_BUS_TYPE EQUAL 1) # I8080

    message(WARNING "I8080 bus type selected.")
    set(PIO_LIB pio_i80)

elseif(TFT_BUS_TYPE EQUAL 2)

    message(FATAL_ERROR "I2C bus not implemented yet.")
    # Default OLED pins for i2c interface
    set(TFT_PIN_SCL   4)
    set(TFT_PIN_SDA   5)

else()
    message(FATAL_ERROR "Invalid TFT bus type!")
endif()

# list(APPEND DRIVER_SOURCES
#     ${CMAKE_CURRENT_LIST_DIR}/pio_i80.c
#     ${CMAKE_CURRENT_LIST_DIR}/pio_spi_tx.c
# )