target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_PIN_BLK=${TFT_PIN_BLK})

if (TFT_BLK_ACTIVE_HIGH)
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_BLK_ACTIVE_HIGH=1)
else()
    target_compile_definitions(${PROJECT_NAME} PUBLIC TFT_BLK_ACTIVE_HIGH=0)
endif()
