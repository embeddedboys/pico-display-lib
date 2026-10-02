# Debug UART.  Interface 0 means "none" and include/config.h defaults to it, so
# it is emitted unconditionally; the pins and speed are only emitted when the
# panel config provides them.
if(NOT DEFINED DEBUG_UART_INTERFACE)
    set(DEBUG_UART_INTERFACE 0)
endif()

panel_emit_value(DEBUG_UART_INTERFACE)
panel_emit_value(DEBUG_UART_SPEED DEBUG_UART_TX_PIN DEBUG_UART_RX_PIN)

if(DEBUG_UART_INTERFACE)
    panel_require(DEBUG_UART_SPEED DEBUG_UART_TX_PIN)
    message(STATUS "debug uart: interface ${DEBUG_UART_INTERFACE}, ${DEBUG_UART_SPEED} baud, tx ${DEBUG_UART_TX_PIN}")
endif()
