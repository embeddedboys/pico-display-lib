add_subdirectory(epaper)

target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_HOR_RES=${TFT_HOR_RES})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_VER_RES=${TFT_VER_RES})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_X_OFFS=${TFT_X_OFFS})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_Y_OFFS=${TFT_Y_OFFS})
target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_BUS_CLK_KHZ=${TFT_BUS_CLK_KHZ})
target_compile_definitions(${PROJECT_NAME} PUBLIC DISP_OVER_PIO=${DISP_OVER_PIO})

# TFT drivers define

if(TFT_DRV_USE_ILI9341)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ILI9341=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ILI9341=0)
endif()

if(TFT_DRV_USE_ILI9481)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ILI9481=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ILI9481=0)
endif()

if(TFT_DRV_USE_ILI9488)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ILI9488=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ILI9488=0)
endif()

if(TFT_DRV_USE_ST7735)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ST7735=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ST7735=0)
endif()

if(TFT_DRV_USE_ST7789V)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ST7789V=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ST7789V=0)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC
    TFT_MODEL_P169H002=${TFT_MODEL_P169H002}
    TFT_MODEL_YT280S030=${TFT_MODEL_YT280S030}
)

if(TFT_DRV_USE_ST7796)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ST7796=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_ST7796=0)
endif()

if(TFT_DRV_USE_SSD1306)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_SSD1306=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_SSD1306=0)
endif()

if(TFT_DRV_USE_SSD1327)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_SSD1327=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_SSD1327=0)
endif()

if(TFT_DRV_USE_SSD1681)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_SSD1681=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_DRV_USE_SSD1681=0)
endif()
