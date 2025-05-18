#include "framebuffer.h"
#include <stdio.h>
#include <stdlib.h>

int framebuffer_init(framebuffer_t *fb, uint8_t width, uint8_t height) {
	if (!fb || width <= 0 || height <= 0)
		return -1;

	fb->width = width;
	fb->height = height;

	fb->pixels = calloc(width * height, sizeof(uint8_t));
	if (!fb->pixels)
		return -1;

	return 0;
}

void framebuffer_free(framebuffer_t *fb) {
	if (!fb || !fb->pixels)
		return;

	free(fb->pixels);
	fb->pixels = NULL;
}

int framebuffer_set(framebuffer_t *fb, uint8_t x, uint8_t y, uint8_t value) {
	if (!fb || !fb->pixels || x < 0 || y < 0 || x >= fb->width ||
		y >= fb->height)
		return -1;

	fb->pixels[y * fb->width + x] = value;
	return 0;
}

int framebuffer_get(framebuffer_t *fb, uint8_t x, uint8_t y, uint8_t *value) {
	if (!fb || !fb->pixels || !value || x < 0 || y < 0 || x >= fb->width ||
		y >= fb->height)
		return -1;

	*value = fb->pixels[y * fb->width + x];
	return 0;
}

void framebuffer_dump_ascii(const framebuffer_t *fb) {
	if (!fb || !fb->pixels)
		return;

	const char *shades = " .:-=+*#%@";

	for (int y = 0; y < fb->height; ++y) {
		for (int x = 0; x < fb->width; ++x) {
			uint8_t val = fb->pixels[y * fb->width + x];
			putchar(shades[val * 10 / 256]); // map 0-255 to 0-9
		}
		putchar('\n');
	}
}

void _framebuffer_debug_chessboard(framebuffer_t *fb) {

	int paint = 0;

	for (int y = 0; y < fb->height; y++) {
		for (int x = 0; x < fb->width; x++) {
			framebuffer_set(fb, x, y, paint ? 255 : 0);
			paint = !paint;
		}
		paint = !paint;
	}
}
