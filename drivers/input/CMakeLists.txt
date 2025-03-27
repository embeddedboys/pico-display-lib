# Indev drivers define

target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_POLLING_PERIOD_MS=${INDEV_POLLING_PERIOD_MS})

if(INDEV_DRV_NOT_USED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_NOT_USED=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_NOT_USED=0)
endif()

if(INDEV_DRV_USE_CST816D)
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_CST816D=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_CST816D=0)
endif()

if(INDEV_DRV_USE_GT911)
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_GT911=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_GT911=0)
endif()

if(INDEV_DRV_USE_TSC2007)
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_TSC2007=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_TSC2007=0)
endif()

if(INDEV_DRV_USE_FT6236)
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_FT6236=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_DRV_USE_FT6236=0)
endif()

target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_PIN_SCL=${INDEV_PIN_SCL})
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_PIN_SDA=${INDEV_PIN_SDA})
target_compile_definitions(${PROJECT_NAME} PUBLIC INDEV_PIN_RST=${INDEV_PIN_RST})