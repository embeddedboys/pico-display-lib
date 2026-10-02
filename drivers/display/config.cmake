include(${CMAKE_CURRENT_LIST_DIR}/epaper/config.cmake)

# Controller selection: flag -> source file.  One table drives both the -D flags
# (drivers/display/CMakeLists.txt) and which source file is compiled.
#
# Only the selected controller is compiled.  Every tft_<model>.c keeps its whole
# body inside `#if TFT_DRV_USE_<model>`, so an unselected one compiles to nothing
# and listing all sixteen was pure build time -- and one more place to forget.
#
# TFT_DRV_USE_ILI9486 is listed for completeness but nothing can select it:
# tft_ili9486.c guards on that flag, the flag is never emitted, and tft.c has no
# dispatch for it, so no panel config reaches that driver.  Left here rather than
# deleted because the file and its init sequence exist and someone may want it.
set(TFT_CONTROLLERS
    TFT_DRV_USE_1P5623  tft_1p5623.c
    TFT_DRV_USE_GC9A01  tft_gc9a01.c
    TFT_DRV_USE_ILI9341 tft_ili9341.c
    TFT_DRV_USE_ILI9481 tft_ili9481.c
    TFT_DRV_USE_ILI9486 tft_ili9486.c
    TFT_DRV_USE_ILI9488 tft_ili9488.c
    TFT_DRV_USE_R61581  tft_r61581.c
    TFT_DRV_USE_SSD1306 tft_ssd1306.c
    TFT_DRV_USE_SSD1327 tft_ssd1327.c
    TFT_DRV_USE_ST6201  tft_st6201.c
    TFT_DRV_USE_ST7305  tft_st7305.c
    TFT_DRV_USE_ST7576  tft_st7576.c
    TFT_DRV_USE_ST7735  tft_st7735s.c
    TFT_DRV_USE_ST7789V tft_st7789v.c
    TFT_DRV_USE_ST77916 tft_st77916.c
    TFT_DRV_USE_ST7796  tft_st7796u.c
)

# tft.c is the dispatcher and is always needed.
list(APPEND DRIVER_SOURCES ${CMAKE_CURRENT_LIST_DIR}/tft.c)

set(_controllers 0)
list(LENGTH TFT_CONTROLLERS _len)
math(EXPR _last "${_len} - 1")
foreach(_i RANGE 0 ${_last} 2)
    list(GET TFT_CONTROLLERS ${_i} _flag)
    math(EXPR _j "${_i} + 1")
    list(GET TFT_CONTROLLERS ${_j} _file)

    if(${_flag})
        list(APPEND DRIVER_SOURCES ${CMAKE_CURRENT_LIST_DIR}/${_file})
        math(EXPR _controllers "${_controllers} + 1")
    endif()
endforeach()

if(_controllers EQUAL 0)
    message(FATAL_ERROR
        "panel config '${PUD_CONFIG}' selects no TFT controller: set one of the "
        "TFT_DRV_USE_* flags listed in drivers/display/config.cmake")
endif()

message(STATUS "panel ${PUD_CONFIG}: ${_controllers} controller driver(s)")
