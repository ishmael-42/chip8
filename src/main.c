#include "framebuffer.h"
#include "gfx.h"
#include <stdint.h>
#include <string.h>

#include "raylib.h"
// clang-format off
uint8_t image_0[] = {
	0b11110000,
	0b10010000,
	0b10010000,
	0b10010000,
	0b11110000,
};
// clang-format on

int main(void) {
	const int screenWidth = 960;
	const int screenHeight = 540;

	framebuffer_t fb;
	framebuffer_init(&fb, 64, 32);

	renderer_t renderer;
	renderer_init(&renderer, screenWidth, screenHeight, &fb);
	framebuffer_draw_image(&fb, 20, 20, 5, image_0);
	_framebuffer_debug_fill_color(&fb, 255);

	while (!WindowShouldClose()) {
		renderer_draw_framebuffer(&renderer, &fb);
	}

	CloseWindow();

	return 0;
}
