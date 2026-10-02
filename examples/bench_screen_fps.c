/*
 * Raw full-screen refresh rate, measured.
 *
 * One metric: how many whole 480x320 frames per second the panel itself can
 * take, with nothing else in the way.  This is the ceiling the USB link is
 * nowhere near -- useful as a reference when judging whether a refresh change
 * matters end to end.
 */
#include <stdio.h>

#include "pico/stdlib.h"
#include "pico/time.h"
#if !PUD_EXAMPLE_STDIO_USB
#include "pico/stdio_uart.h"

#include "config.h"   /* debug_uart, DEBUG_UART_* pins */
#endif

#include "tft.h"

#define BENCH_FRAMES 64

static uint16_t s_frame[TFT_HOR_RES * TFT_VER_RES];

/*
 * Eight vertical colour bars.  Unlike the noise pattern the bandwidth benchmark
 * uses, this is recognisable: if the bars come out in this order, left to right,
 * with even widths and no skew, then the bus, the window geometry and the pixel
 * format are all right -- a wrong controller, a swapped axis or a wrong COLMOD
 * turns this into something visibly broken rather than plausible-looking noise.
 */
static void fill_bars(void)
{
	static const uint16_t bars[8] = {
		0xffff, /* white   */
		0xffe0, /* yellow  */
		0x07ff, /* cyan    */
		0x07e0, /* green   */
		0xf81f, /* magenta */
		0xf800, /* red     */
		0x001f, /* blue    */
		0x0000, /* black   */
	};

	for (size_t i = 0; i < TFT_HOR_RES * TFT_VER_RES; i++)
		s_frame[i] = bars[(i % TFT_HOR_RES) * 8 / TFT_HOR_RES];
}

int main(void)
{
	uint32_t t0, elapsed_us, i, fps_x100;

#if PUD_EXAMPLE_STDIO_USB
	/* USB CDC: nothing to configure, the SDK brings the port up. */
	stdio_init_all();
#else
	/* The SDK default UART is not the one this board routes to its header,
	 * so the examples initialise it the way the firmware does. */
	stdio_uart_init_full(debug_uart, DEBUG_UART_SPEED, DEBUG_UART_TX_PIN,
	                     DEBUG_UART_RX_PIN);
#endif
	printf("\n[bench] raw full-screen refresh rate\n");

	tft_driver_init();
	printf("[bench] panel %dx%d, bus %d kHz\n", TFT_HOR_RES, TFT_VER_RES,
	       TFT_BUS_CLK_KHZ);

	fill_bars();
	tft_video_flush(0, 0, TFT_HOR_RES - 1, TFT_VER_RES - 1, s_frame,
	                sizeof(s_frame));

	t0 = time_us_32();
	for (i = 0; i < BENCH_FRAMES; i++)
		tft_video_flush(0, 0, TFT_HOR_RES - 1, TFT_VER_RES - 1, s_frame,
		                sizeof(s_frame));
	elapsed_us = time_us_32() - t0;

	/*
	 * Leave a known pattern on the panel, and flush it *twice* without
	 * rebuilding it.  That second flush is deliberate: with TFT_COLOR_16_SWAP
	 * on, the driver must not leave the caller's buffer modified, or the
	 * colours on screen would depend on how many times the buffer had been
	 * sent.  Correct bar colours here are the regression test for that.
	 */
	fill_bars();
	tft_video_flush(0, 0, TFT_HOR_RES - 1, TFT_VER_RES - 1, s_frame,
	                sizeof(s_frame));
	tft_video_flush(0, 0, TFT_HOR_RES - 1, TFT_VER_RES - 1, s_frame,
	                sizeof(s_frame));

	fps_x100 = (uint32_t)(((uint64_t)BENCH_FRAMES * 100000000u) / elapsed_us);
	printf("[bench] %u frames in %u us -> %u.%02u fps\n", BENCH_FRAMES,
	       elapsed_us, fps_x100 / 100u, fps_x100 % 100u);

	for (;;)
		tight_loop_contents();
}
