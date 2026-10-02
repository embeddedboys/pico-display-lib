# Standalone entry point for the measurement examples.
#
# The library itself has no main() and is normally added as a subdirectory of
# the firmware, so building one of these means being the top level: import the
# SDK, pick a panel config, then add the library the same way the firmware does.
#
#   cmake -S examples -B build -DPICO_BOARD=pico2 \
#         -DPUD_CONFIG=pico_dm_qd3503728 -DPICO_SDK_PATH=<sdk>
#   cmake --build build -j8
#
# Each executable measures one thing and prints it over the debug UART
# (115200, the pins in the panel config).  Read it with any serial terminal;
# nothing else is needed -- no USB host, no kernel driver.
cmake_minimum_required(VERSION 3.13)

if(NOT DEFINED ENV{PICO_SDK_PATH} AND NOT DEFINED PICO_SDK_PATH)
    message(FATAL_ERROR "set PICO_SDK_PATH")
endif()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico-display-lib-examples C CXX ASM)
pico_sdk_init()

set(PUD_CONFIG "pico_dm_qd3503728" CACHE STRING "panel config from configs/")
set(PUD_CONFIG_DIR ${CMAKE_CURRENT_LIST_DIR}/../configs CACHE PATH "")
include(${PUD_CONFIG_DIR}/${PUD_CONFIG}.cmake)

# The panel config sets the clock the *panel* needs.  The bus benchmarks measure
# what the PIO can drive, which is a different question, so they may push it
# higher than the panel asks for -- the panel is write-only and does not
# throttle the transfer, and a wrong clock shows up as a bad picture rather
# than as a slower number.
set(PUD_BUS_CLK_KHZ "" CACHE STRING "override TFT_BUS_CLK_KHZ, for bus benchmarks")
if(NOT PUD_BUS_CLK_KHZ STREQUAL "")
    set(TFT_BUS_CLK_KHZ ${PUD_BUS_CLK_KHZ})
    message(STATUS "bus clock overridden to ${TFT_BUS_CLK_KHZ} kHz")
endif()

# Same call the firmware makes, so the examples are built against exactly the
# library the firmware gets.
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/.. ${CMAKE_BINARY_DIR}/pico-display-lib)

set(PUD_EXAMPLES
    bench_bus_bandwidth
    bench_screen_fps
    bench_band_flush
)

# Where the examples log to.  The default is the panel config's debug UART; USB
# CDC is for boards whose UART is not wired to anything the host can read (it
# needs no CherryUSB, so it does not collide with the firmware's USB stack).
set(PUD_EXAMPLE_STDIO_USB 0 CACHE STRING "examples: 1 = log over USB CDC, 0 = over the debug UART")

foreach(ex IN LISTS PUD_EXAMPLES)
    add_executable(${ex} ${CMAKE_CURRENT_LIST_DIR}/${ex}.c)
    target_link_libraries(${ex} pico-display-lib pico_stdlib)
    if(PUD_EXAMPLE_STDIO_USB)
        pico_enable_stdio_usb(${ex} 1)
        pico_enable_stdio_uart(${ex} 0)
        # A measurement prints once, and USB CDC does not replay what was sent
        # before the host opened the port -- so the device waits for a reader
        # instead of printing into the void.
        set(PICO_STDIO_USB_CONNECT_WAIT_TIMEOUT_MS 20000)
    else()
        pico_enable_stdio_usb(${ex} 0)
        pico_enable_stdio_uart(${ex} 1)
    endif()
    target_compile_definitions(${ex} PRIVATE PUD_EXAMPLE_STDIO_USB=${PUD_EXAMPLE_STDIO_USB})
    pico_add_extra_outputs(${ex})
endforeach()
