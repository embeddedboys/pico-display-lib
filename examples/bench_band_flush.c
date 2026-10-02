/*
 * Per-band flush rate, measured.
 *
 * One metric: the cost of one band-sized flush, i.e. of the unit the firmware
 * actually sends.  Each flush carries the window command plus the pixels, so it
 * is not the same number as the bus bandwidth divided by the band size -- the
 * command overhead is the point of measuring it.
 */
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#if !PUD_EXAMPLE_STDIO_USB
#include "pico/stdio_uart.h"

#include "config.h"   /* debug_uart, DEBUG_UART_* pins */
#endif

#include "tft.h"

/* One band as the firmware sizes it: whole rows that fit what a host may
 * send.  Kept local because the firmware's own budget is a firmware constant. */
#define BAND_ROWS 8
#define BAND_PIXELS (TFT_HOR_RES * BAND_ROWS)
#define BAND_FRAMES 256

static uint16_t s_band[BAND_PIXELS];

int main(void)
{
	uint32_t t0, elapsed_us, i, j;

#if PUD_EXAMPLE_STDIO_USB
	/* USB CDC: nothing to configure, the SDK brings the port up. */
	stdio_init_all();
#else
	/* The SDK default UART is not the one this board routes to its header,
	 * so the examples initialise it the way the firmware does. */
	stdio_uart_init_full(debug_uart, DEBUG_UART_SPEED, DEBUG_UART_TX_PIN,
	                     DEBUG_UART_RX_PIN);
#endif
	printf("\n[bench] per-band flush rate\n");

	tft_driver_init();
	printf("[bench] panel %dx%d, band %dx%d (%u px)\n", TFT_HOR_RES, TFT_VER_RES,
	       TFT_HOR_RES, BAND_ROWS, BAND_PIXELS);

	for (i = 0; i < BAND_PIXELS; i++)
		s_band[i] = (uint16_t)(i * 7);

	tft_video_flush(0, 0, TFT_HOR_RES - 1, BAND_ROWS - 1, s_band,
	                sizeof(s_band));

	t0 = time_us_32();
	for (j = 0; j < BAND_FRAMES; j++) {
		uint16_t y = (uint16_t)((j * BAND_ROWS) % (TFT_VER_RES - BAND_ROWS));

		tft_video_flush(0, y, TFT_HOR_RES - 1, y + BAND_ROWS - 1, s_band,
		                sizeof(s_band));
	}
	elapsed_us = time_us_32() - t0;

	printf("[bench] %u bands in %u us -> %u us/band, %u bands/s\n", BAND_FRAMES,
	       elapsed_us, elapsed_us / BAND_FRAMES,
	       (uint32_t)((uint64_t)BAND_FRAMES * 1000000u / elapsed_us));
	{
		uint64_t bps = (uint64_t)sizeof(s_band) * BAND_FRAMES * 1000000u / elapsed_us;

		printf("[bench] payload %u B/band -> %u.%02u MB/s\n",
		       (unsigned)sizeof(s_band), (unsigned)(bps / 1000000u),
		       (unsigned)((bps / 10000u) % 100u));
	}
	printf("[bench] whole screen = %u bands -> %u us\n",
	       (TFT_VER_RES + BAND_ROWS - 1) / BAND_ROWS,
	       (elapsed_us / BAND_FRAMES) * ((TFT_VER_RES + BAND_ROWS - 1) / BAND_ROWS));

	for (;;)
		tight_loop_contents();
}
