// Copyright (c) 2025 embeddedboys developers
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef __CONFIG_H
#define __CONFIG_H

#ifndef DEBUG_UART_INTERFACE
	#define DEBUG_UART_INTERFACE 0
#endif

#ifndef DEBUG_UART_SPEED
	#define DEBUG_UART_SPEED 115200
#endif

#ifndef DEBUG_UART_TX_PIN
	#define DEBUG_UART_TX_PIN 0
#endif

#ifndef DEBUG_UART_RX_PIN
	#define DEBUG_UART_RX_PIN 1
#endif

#if DEBUG_UART_INTERFACE == 0
	#define debug_uart uart0
#elif DEBUG_UART_INTERFACE == 1
	#define debug_uart uart1
#elif DEBUG_UART_INTERFACE == 2
	#define debug_uart uart2
#else
	#error "Invalid DEBUG_UART_INTERFACE value"
#endif

enum {
    TFT_ROTATE_0    = 0x00,
    TFT_ROTATE_90   = 0x01,
    TFT_ROTATE_180  = 0x02,
    TFT_ROTATE_270  = 0x03,
};

#ifndef TFT_COLOR_16_SWAP
    #define TFT_COLOR_16_SWAP 0
#endif

// Assume that the backlight is active high.
#ifndef TFT_BLK_ACTIVE_HIGH
    #define TFT_BLK_ACTIVE_HIGH 1
#endif

#define TFT_BUS_TYPE_SPI    0x00
#define TFT_BUS_TYPE_I80    0x01
#define TFT_BUS_TYPE_I2C    0x02

#ifndef TFT_X_OFFS
    #define TFT_X_OFFS 0
#endif

#ifndef TFT_Y_OFFS
    #define TFT_Y_OFFS 0
#endif

#define INDEV_BUS_TYPE_I2C  0x00
#define INDEV_BUS_TYPE_SPI  0x01

#ifndef INDEV_POLLING_PERIOD_MS
    #define INDEV_POLLING_PERIOD_MS 33
#endif

#endif /* __CONFIG_H */
