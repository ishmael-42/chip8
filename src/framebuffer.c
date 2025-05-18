#include "framebuffer.h"
#include <stdint.h>
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

static inline uint8_t get_nth_bit(uint8_t byte, uint8_t n) {
	if (n > 7)
		return 0; // out of range for uint8_t
	return (byte >> (7 - n)) & 1;
}

uint8_t framebuffer_draw_image(framebuffer_t *fb, uint8_t x, uint8_t y,
							   uint8_t nbytes, uint8_t *image_data) {

	uint8_t collision_happened = 0;

	for (int yy = 0; yy < nbytes; yy++) {
		uint8_t current_byte = image_data[yy];
		for (int xx = 0; xx < 8; xx++) {

			uint8_t target_x, target_y;
			target_x = (x + xx) % fb->width;
			target_y = (y + yy) % fb->height;

			uint8_t original_pixel, image_pixel, result_pixel;
			image_pixel = get_nth_bit(current_byte, xx) ? 255 : 0;
			framebuffer_get(fb, target_x, target_y, &original_pixel);
			result_pixel = image_pixel ^ original_pixel;

			framebuffer_set(fb, target_x, target_y, result_pixel);

			// VF flag.
			// See http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#Dxyn
			if (image_pixel && original_pixel)
				collision_happened = 1;
		}
	}
	return collision_happened;
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

void _framebuffer_debug_fill_color(framebuffer_t *fb, uint8_t color) {
	for (int y = 0; y < fb->height; y++) {
		for (int x = 0; x < fb->width; x++) {
			framebuffer_set(fb, x, y, color);
		}
	}
}
