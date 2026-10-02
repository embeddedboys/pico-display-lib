/*
 * PIO 8080 write bandwidth, measured.
 *
 * One metric: how many bytes per second the panel bus can carry.  It writes a
 * full screen repeatedly through tft_video_flush() -- which is the synchronous
 * path, so the time includes the whole transfer -- and reports the rate.
 *
 * No decoder, no USB, no kernel driver, no compositor: this is the bus alone.
 */
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#if !PUD_EXAMPLE_STDIO_USB
#include "pico/stdio_uart.h"

#include "config.h"   /* debug_uart, DEBUG_UART_* pins */
#endif

#include "tft.h"

#define BENCH_FRAMES 32

static uint16_t s_frame[TFT_HOR_RES * TFT_VER_RES];

int main(void)
{
	uint32_t t0, elapsed_us, bytes, i;

#if PUD_EXAMPLE_STDIO_USB
	/* USB CDC: nothing to configure, the SDK brings the port up. */
	stdio_init_all();
#else
	/* The SDK default UART is not the one this board routes to its header,
	 * so the examples initialise it the way the firmware does. */
	stdio_uart_init_full(debug_uart, DEBUG_UART_SPEED, DEBUG_UART_TX_PIN,
	                     DEBUG_UART_RX_PIN);
#endif
	printf("\n[bench] PIO 8080 write bandwidth\n");

	tft_driver_init();
	/* Deliberately bus-agnostic: the data-pin count is an I8080 detail and this
	 * example also has to build against the SPI panel configs. */
	printf("[bench] panel %dx%d, bus type %d, %d kHz\n", TFT_HOR_RES,
	       TFT_VER_RES, TFT_BUS_TYPE, TFT_BUS_CLK_KHZ);

	/* A non-uniform pattern so the result does not depend on a fill shortcut. */
	for (i = 0; i < TFT_HOR_RES * TFT_VER_RES; i++)
		s_frame[i] = (uint16_t)((i * 2654435761u) >> 16);

	/* One frame first: it warms the PIO/DMA path so the loop measures the
	 * steady state rather than the first-transfer setup. */
	tft_video_flush(0, 0, TFT_HOR_RES - 1, TFT_VER_RES - 1, s_frame,
	                sizeof(s_frame));

	t0 = time_us_32();
	for (i = 0; i < BENCH_FRAMES; i++)
		tft_video_flush(0, 0, TFT_HOR_RES - 1, TFT_VER_RES - 1, s_frame,
		                sizeof(s_frame));
	elapsed_us = time_us_32() - t0;

	bytes = (uint32_t)BENCH_FRAMES * TFT_HOR_RES * TFT_VER_RES * 2u;
	printf("[bench] %u frames, %u bytes in %u us\n", BENCH_FRAMES, bytes,
	       elapsed_us);
	printf("[bench] per frame : %u us\n", elapsed_us / BENCH_FRAMES);
	{
		uint64_t bps = (uint64_t)bytes * 1000000u / elapsed_us;

		printf("[bench] BANDWIDTH : %u.%02u MB/s\n",
		       (unsigned)(bps / 1000000u), (unsigned)((bps / 10000u) % 100u));
	}

	for (;;)
		tight_loop_contents();
}
