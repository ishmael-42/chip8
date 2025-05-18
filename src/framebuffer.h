#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

typedef struct {
	int width;
	int height;
	uint8_t *pixels;
} framebuffer_t;

int framebuffer_init(framebuffer_t *fb, uint8_t framebuffer_width,
					 uint8_t framebuffer_height);
void framebuffer_free(framebuffer_t *fb);

int framebuffer_get(framebuffer_t *fb, uint8_t x, uint8_t y, uint8_t *value);
int framebuffer_set(framebuffer_t *fb, uint8_t x, uint8_t y, uint8_t value);

uint8_t framebuffer_draw_image(framebuffer_t *fb, uint8_t x, uint8_t y,
							   uint8_t nbytes, uint8_t *image_data);

void framebuffer_dump_ascii(const framebuffer_t *fb);
void _framebuffer_debug_chessboard(framebuffer_t *fb);
void framebuffer_fill_color(framebuffer_t *fb, uint8_t color);

#endif // FRAMEBUFFER_H
