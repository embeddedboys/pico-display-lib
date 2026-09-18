target_compile_definitions(${LIBRARY_NAME} PUBLIC DEFAULT_SYS_CLK_KHZ=${SYS_CLK_KHZ})
target_compile_definitions(${LIBRARY_NAME} PUBLIC DEFAULT_PERI_CLK_KHZ=${PERI_CLK_KHZ})

# Note: If you are using a NOR flash like "w25q16". Just keep the following content.
# The maximum speed of "w25q16" is 133MHz, However, the clock speed of XIP QSPI is divided from "sys_clk".
# The divider is compiled into boot stage 2 and stays in effect after main() raises clk_sys, so it has to
# be chosen for the *overclocked* sys_clk, not for the 150MHz the chip boots at.
# RP2040's SSI needs an even divider: 2 up to 266MHz (240/2 = 120MHz), 4 above it (360/4 = 90MHz).
# RP2350's QMI takes any divider.  Every overclock profile in the board configs
# wants 3 (225/3 = 75MHz, 366/3 = 122MHz, 384/3 = 128MHz, all within the 133MHz
# rating); at the stock 150MHz, 2 keeps the same 75MHz the chip boots with
# instead of needlessly dropping it to 50MHz.
if(PICO_PLATFORM MATCHES "^rp2350" OR PICO_BOARD STREQUAL "pico2")
    if(${SYS_CLK_KHZ} GREATER 150000)
        set(PICO_FLASH_SPI_CLKDIV 3)
    else()
        set(PICO_FLASH_SPI_CLKDIV 2)
    endif()
elseif(${SYS_CLK_KHZ} GREATER 266000)
    set(PICO_FLASH_SPI_CLKDIV 4)
else()
    set(PICO_FLASH_SPI_CLKDIV 2)
endif()
math(EXPR FLASH_CLK_KHZ "${SYS_CLK_KHZ} / ${PICO_FLASH_SPI_CLKDIV}")
message(STATUS "XIP flash clock: ${FLASH_CLK_KHZ} KHz (clk_sys ${SYS_CLK_KHZ} KHz / PICO_FLASH_SPI_CLKDIV ${PICO_FLASH_SPI_CLKDIV})")
target_compile_definitions(bs2_default PRIVATE PICO_FLASH_SPI_CLKDIV=${PICO_FLASH_SPI_CLKDIV})
target_compile_definitions(${LIBRARY_NAME} PUBLIC FLASH_CLK_KHZ=${FLASH_CLK_KHZ})