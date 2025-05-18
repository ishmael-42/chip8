#include "framebuffer.h"
#include "raylib.h"
#include <stdint.h>

typedef struct renderer_t {
	uint8_t **framebuffer;
	Texture2D renderTexture;

	Rectangle framebufferRect, displayRect;
} renderer_t;

void renderer_init(renderer_t *render, uint8_t screen_width,
				   uint8_t screen_height, const framebuffer_t *fb);

void renderer_draw_framebuffer(renderer_t *render, const framebuffer_t *fb);
