if(OVERCLOCK_ENABLED)

    message(WARNING "Overclocking is enabled. This may damage your device. Use at own risk.")

    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")

        if(OVERCLOCK_PROFILE EQUAL 1)
            set(SYS_CLK_KHZ 240000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        elseif(OVERCLOCK_PROFILE EQUAL 2)
            set(SYS_CLK_KHZ 266000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 3)
            set(SYS_CLK_KHZ 360000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 4)
            set(SYS_CLK_KHZ 400000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 5)
            set(SYS_CLK_KHZ 416000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        else()
            message(FATAL_ERROR "Invalid overclocking profile")
        endif()
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")

        if(OVERCLOCK_PROFILE EQUAL 1)
            set(SYS_CLK_KHZ 225000)             # CPU clock speed
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
        elseif(OVERCLOCK_PROFILE EQUAL 2)
            set(SYS_CLK_KHZ 366000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        elseif(OVERCLOCK_PROFILE EQUAL 3)
            set(SYS_CLK_KHZ 384000)
            set(PERI_CLK_KHZ ${SYS_CLK_KHZ})
        else()
            message(FATAL_ERROR "Invalid overclocking profile")
        endif()
    endif()

else()  # OVERCLOCK_ENABLED
    message(WARNING "Overclocking is disabled.")

    if(${PICO_BOARD} STREQUAL "pico" OR ${PICO_PLATFORM} STREQUAL "rp2040")
        set(SYS_CLK_KHZ 125000) # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    elseif(${PICO_BOARD} STREQUAL "pico2" OR ${PICO_PLATFORM} STREQUAL "rp2350")
        set(SYS_CLK_KHZ 150000) # CPU clock speed
        set(PERI_CLK_KHZ ${SYS_CLK_KHZ})    # Peripheral clock speed
    endif()

endif() # OVERCLOCK_ENABLED

