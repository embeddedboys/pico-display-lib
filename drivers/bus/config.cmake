# Bus selection.  Only the choice of PIO library belongs here -- the pins are
# emitted by CMakeLists.txt, next to the code that uses them.
#
# The error names the panel config: a config copied from a panel with a different
# bus is the usual way this goes wrong, and "Invalid TFT bus type!" made the
# reader go looking at the library instead of at their own config.
if(TFT_BUS_TYPE EQUAL 0) # SPI
    message(STATUS "bus: SPI")
    set(PIO_LIB pio_spi_tx)
elseif(TFT_BUS_TYPE EQUAL 1) # I8080
    message(STATUS "bus: I8080")
    set(PIO_LIB pio_i80)
elseif(TFT_BUS_TYPE EQUAL 2) # I2C
    message(FATAL_ERROR
        "panel config '${PUD_CONFIG}': TFT_BUS_TYPE=2 (I2C) has no driver yet")
else()
    message(FATAL_ERROR
        "panel config '${PUD_CONFIG}': TFT_BUS_TYPE must be 0 (SPI), 1 (I8080) or 2 (I2C), got '${TFT_BUS_TYPE}'")
endif()
