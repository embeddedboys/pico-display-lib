cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)
set(DRIVERS_DIR ${CMAKE_CURRENT_LIST_DIR}/drivers)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "2.0.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 2.0.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# If you want debug output from USB (pass -DPICO_STDIO_USB=1) this ensures you don't lose any debug output while USB is set up
if (NOT DEFINED PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS)
    set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 3000)
endif()

set(DRIVER_SOURCES
    main.c
)

# select the suitable config file
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st7789v.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st7576.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-gc9a01.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-ssd1306.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-ssd1327.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/fpc-zh096g1321.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/md144_spi_v04.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_qd3503728.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_gtm0375hi1t02.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_fpc032mra003.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_yt350s006.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_1p5623.cmake)

include(${DRIVERS_DIR}/backlight/config.cmake)
include(${DRIVERS_DIR}/display/config.cmake)
include(${DRIVERS_DIR}/input/config.cmake)
include(${DRIVERS_DIR}/bus/config.cmake)
include(${DRIVERS_DIR}/clk/config.cmake)

# Initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# set the project name
project(pico-display-template C CXX ASM)

add_executable(${PROJECT_NAME})

add_subdirectory(${DRIVERS_DIR}/backlight)
add_subdirectory(${DRIVERS_DIR}/display)
add_subdirectory(${DRIVERS_DIR}/input)
add_subdirectory(${DRIVERS_DIR}/bus)
add_subdirectory(${DRIVERS_DIR}/clk)

# Note: If you are using a NOR flash like "w25q16". Just keep the following content.
# The maximum speed of "w25q16" is 133MHz, However, the clock speed of XIP QSPI is divided from "sys_clk".
# So, when your "sys_clk" is greater than 266MHz and default PICO_FLASH_SPI_CLKDIV=2, It will exceed the
# maximum speed, because PICO_FLASH_SPI_CLKDIV must be even, So 4 is good for most purpose, Otherwise,
# nothing should be done. These things will only useful when you overclocking the rp2040.
if(${SYS_CLK_KHZ} GREATER 266000)
    set(PICO_FLASH_SPI_CLKDIV 4)
else()
    set(PICO_FLASH_SPI_CLKDIV 2)
endif()
math(EXPR FLASH_CLK_KHZ "${SYS_CLK_KHZ} / ${PICO_FLASH_SPI_CLKDIV}")
target_compile_definitions(bs2_default PRIVATE PICO_FLASH_SPI_CLKDIV=${PICO_FLASH_SPI_CLKDIV})
target_compile_definitions(${PROJECT_NAME} PRIVATE FLASH_CLK_KHZ=${FLASH_CLK_KHZ})

# message(WARNING ${DRIVER_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE
    ${DRIVER_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    pico_multicore
    pico_bootsel_via_double_reset
    hardware_i2c
    hardware_spi
    hardware_pwm
    hardware_clocks
    FreeRTOS-Kernel-Heap3
    ${PIO_LIB}
)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)
pico_add_extra_outputs(${PROJECT_NAME})

# show target size info
add_custom_target(
    print-memory-usage ALL
    COMMAND arm-none-eabi-size -G ${CMAKE_PROJECT_NAME}.elf
    DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
    COMMENT "Print target size info"
    DEPENDS ${PROJECT_NAME}
)

# FIXME: Temporary manual operation:
# First, You need to add `set auto-load safe-path .` in your `~/.config/gdb/gdbinit`
# Then you have to run `openocd -f interface/cmsis-dap.cfg -f target/rp2350.cfg` etc.
# in another window to start the debugger. Finally, in the build directory, run
# `gdb-multiarch ./rp2040-freertos-template.elf` to attach gdb to the debugger.
add_custom_target(
    copy-gdbinit ALL
    COMMAND cp ${CMAKE_CURRENT_LIST_DIR}/.gdbinit ${CMAKE_BINARY_DIR}/
    DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
    COMMENT "Copy .gdbinit to build directory"
    DEPENDS ${PROJECT_NAME}
)

# add a firmware flash target
if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
        add_custom_target(
            flash
            COMMAND openocd -f interface/cmsis-dap.cfg -c "adapter speed 10000"
                            -f target/rp2040.cfg -s tcl -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
            DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            COMMENT "Flashing firmware using CMSIS-DAP Debugger..."
            DEPENDS ${PROJECT_NAME}
        )
elseif(${PICO_BOARD} STREQUAL  "pico2" OR ${PICO_PLATFORM} STREQUAL  "rp2350")
        add_custom_target(
            flash
            COMMAND openocd -f interface/cmsis-dap.cfg -c "adapter speed 10000"
                            -f target/rp2350.cfg -s tcl -c "program ${CMAKE_PROJECT_NAME}.elf verify reset exit"
            DEPENDS ${CMAKE_BINARY_DIR}/${CMAKE_PROJECT_NAME}.elf
            COMMENT "Flashing firmware using CMSIS-DAP Debugger..."
            DEPENDS ${PROJECT_NAME}
        )
endif()
