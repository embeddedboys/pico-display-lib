add_subdirectory(epaper)

if(${TFT_ROTATION} EQUAL 0 OR ${TFT_ROTATION} EQUAL 2)
    set(TFT_X_RES ${TFT_HOR_RES})
    set(TFT_Y_RES ${TFT_VER_RES})
elseif(${TFT_ROTATION} EQUAL 1 OR ${TFT_ROTATION} EQUAL 3)
    set(TFT_X_RES ${TFT_VER_RES})
    set(TFT_Y_RES ${TFT_HOR_RES})
else()
    message(FATAL_ERROR "ERROR: Invalid display rotation")
endif()

message(STATUS "TFT_X_RES: ${TFT_X_RES}")
message(STATUS "TFT_Y_RES: ${TFT_Y_RES}")
message(STATUS "TFT rotation: ${TFT_ROTATION}   //0: normal, 1: 90 degree, 2: 180 degree, 3: 270 degree")

target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_HOR_RES=${TFT_X_RES})
target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_VER_RES=${TFT_Y_RES})
target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_ROTATION=${TFT_ROTATION})
target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_X_OFFS=${TFT_X_OFFS})
target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_Y_OFFS=${TFT_Y_OFFS})
target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_BUS_CLK_KHZ=${TFT_BUS_CLK_KHZ})
target_compile_definitions(${LIBRARY_NAME} PUBLIC DISP_OVER_PIO=${DISP_OVER_PIO})

if (TFT_COLOR_16_SWAP)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_COLOR_16_SWAP=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_COLOR_16_SWAP=0)
endif()

if(TFT_MODEL_GENERIC)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_MODEL_GENERIC=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_MODEL_GENERIC=0)
endif()

# TFT drivers define

if(TFT_DRV_USE_ILI9341)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ILI9341=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ILI9341=0)
endif()

if(TFT_DRV_USE_ILI9481)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ILI9481=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ILI9481=0)
endif()

if(TFT_DRV_USE_ILI9488)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ILI9488=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ILI9488=0)
endif()

if(TFT_DRV_USE_ST7735)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7735=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7735=0)
endif()

target_compile_definitions(${LIBRARY_NAME} PUBLIC
    TFT_MODEL_MD144_SPI_V04=${TFT_MODEL_MD144_SPI_V04}
)

if(TFT_DRV_USE_ST7789V)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7789V=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7789V=0)
endif()

target_compile_definitions(${LIBRARY_NAME} PUBLIC
    TFT_MODEL_P169H002=${TFT_MODEL_P169H002}
    TFT_MODEL_YT280S030=${TFT_MODEL_YT280S030}
)

if(TFT_DRV_USE_ST7796)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7796=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7796=0)
endif()

target_compile_definitions(${LIBRARY_NAME} PUBLIC
    TFT_MODEL_YT350S006=${TFT_MODEL_YT350S006}
)

if(TFT_DRV_USE_ST77916)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST77916=1)

    if (TFT_MODEL_GENERIC)
        target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_MODEL_GENERIC=1)
    else()
        target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_MODEL_GENERIC=0)
    endif()

    if(TFT_MODEL_W180TE010I_18Z)
        target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_MODEL_W180TE010I_18Z=1)
    else()
        target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_MODEL_W180TE010I_18Z=0)
    endif()

else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST77916=0)
endif()

if(TFT_DRV_USE_SSD1306)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_SSD1306=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_SSD1306=0)
endif()

if(TFT_DRV_USE_SSD1327)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_SSD1327=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_SSD1327=0)
endif()

if(TFT_DRV_USE_SSD1681)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_SSD1681=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_SSD1681=0)
endif()

if(TFT_DRV_USE_GC9A01)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_GC9A01=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_GC9A01=0)
endif()

if(TFT_DRV_USE_R61581)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_R61581=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_R61581=0)
endif()

if(TFT_DRV_USE_ST6201)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST6201=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST6201=0)
endif()

if(TFT_DRV_USE_ST7305)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7305=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7305=0)
endif()

if(TFT_DRV_USE_ST7576)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7576=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_ST7576=0)
endif()

if(TFT_DRV_USE_1P5623)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_1P5623=1)
else()
    target_compile_definitions(${LIBRARY_NAME} PUBLIC TFT_DRV_USE_1P5623=0)
endif()
