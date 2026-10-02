cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -Wl,--print-memory-usage")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -Wl,--print-memory-usage")
set(DRIVERS_DIR ${CMAKE_CURRENT_LIST_DIR}/drivers)
set(LIBRARY_NAME "pico-display-lib")

add_library(${LIBRARY_NAME} STATIC)

# The one place a panel config variable becomes a -D macro, and the one place a
# missing variable is turned into a configure error instead of a default.
include(${CMAKE_CURRENT_LIST_DIR}/cmake/panel.cmake)

set(DRIVER_SOURCES
    ""
)

# The panel is chosen by the PUD_CONFIG cache variable (the firmware passes
# -DPUD_CONFIG=<name>, see its configs/ symlink into this directory).  This file
# used to list every config as a commented-out include to be uncommented by hand,
# which dirtied the working tree and was replaced by PUD_CONFIG -- the list was
# left behind and is now gone.
if(PUD_CONFIG)
    if(NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/configs/${PUD_CONFIG}.cmake)
        message(FATAL_ERROR
            "PUD_CONFIG='${PUD_CONFIG}' has no configs/${PUD_CONFIG}.cmake "
            "(available: ${CMAKE_CURRENT_LIST_DIR}/configs)")
    endif()
    include(${CMAKE_CURRENT_LIST_DIR}/configs/${PUD_CONFIG}.cmake)
else()
    message(FATAL_ERROR
        "PUD_CONFIG is not set: the panel is chosen with -DPUD_CONFIG=<name>, "
        "see configs/")
endif()

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
    ${PIO_LIB}
)
