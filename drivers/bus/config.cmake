if(TFT_BUS_TYPE EQUAL 0) # SPI

    message(STATUS "SPI bus type selected.")
    set(PIO_LIB pio_spi_tx)

elseif(TFT_BUS_TYPE EQUAL 1) # I8080

    message(STATUS "I8080 bus type selected.")
    set(PIO_LIB pio_i80)

elseif(TFT_BUS_TYPE EQUAL 2)

    message(FATAL_ERROR "I2C bus not implemented yet.")
    # Default OLED pins for i2c interface
    set(TFT_PIN_SCL   4)
    set(TFT_PIN_SDA   5)

else()
    message(FATAL_ERROR "Invalid TFT bus type!")
endif()