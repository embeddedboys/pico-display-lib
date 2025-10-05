cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")
set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/FreeRTOS-Kernel)
set(DRIVERS_DIR ${CMAKE_CURRENT_LIST_DIR}/drivers)
set(LIBRARY_NAME "pico-display-lib")

include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/Community-Supported-Ports/GCC/RP2350_ARM_NTZ/FreeRTOS_Kernel_import.cmake)

add_library(${LIBRARY_NAME} STATIC)

set(DRIVER_SOURCES
    ""
)

# select the suitable config file
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st7789v.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st7305.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st7576.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-gc9a01.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-gc9a01-8bit.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-ssd1306.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-ssd1327.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-ili9341.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/generic-st77916.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/wlk1804rctp-14p.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/fpc-zh096g1321.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/md144_spi_v04.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico-restouch-lcd-2.8.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_qd3503728.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_gtm0375hi1t02.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_fpc032mra003.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_yt350s006.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_1p5623.cmake)
# include(${CMAKE_CURRENT_LIST_DIR}/configs/pico_dm_ep4309n.cmake)

include(${DRIVERS_DIR}/backlight/config.cmake)
include(${DRIVERS_DIR}/display/config.cmake)
include(${DRIVERS_DIR}/input/config.cmake)
include(${DRIVERS_DIR}/bus/config.cmake)
include(${DRIVERS_DIR}/clk/config.cmake)
include(${DRIVERS_DIR}/uart/config.cmake)

add_subdirectory(${DRIVERS_DIR}/backlight)
add_subdirectory(${DRIVERS_DIR}/display)
add_subdirectory(${DRIVERS_DIR}/input)
add_subdirectory(${DRIVERS_DIR}/bus)
add_subdirectory(${DRIVERS_DIR}/clk)
add_subdirectory(${DRIVERS_DIR}/uart)

# message(WARNING ${DRIVER_SOURCES})
target_sources(${LIBRARY_NAME} PRIVATE
    ${DRIVER_SOURCES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${LIBRARY_NAME}
    pico_stdlib
    pico_multicore
    # pico_bootsel_via_double_reset
    hardware_i2c
    hardware_spi
    hardware_pwm
    hardware_clocks
    FreeRTOS-Kernel-Heap3
    ${PIO_LIB}
)
