#include "framebuffer.h"
#include "raylib.h"
#include <stdlib.h>

struct renderer_t {
	uint8_t **framebuffer;
	Texture2D renderTexture;

	Rectangle framebufferRect, displayRect;
};

void renderer_init(struct renderer_t **renderr, const uint64_t screen_width,
				   const uint64_t screen_height, const framebuffer_t *fb) {

	InitWindow(screen_width, screen_height, "test raylib");
	SetTargetFPS(60);

	struct renderer_t *render = calloc(sizeof(struct renderer_t), 1);
	*renderr = render;

	Image bwImage;
	bwImage.data = fb->pixels;
	bwImage.format = PIXELFORMAT_UNCOMPRESSED_GRAYSCALE;
	bwImage.width = fb->width;
	bwImage.height = fb->height;
	bwImage.mipmaps = 1;
	render->renderTexture = LoadTextureFromImage(bwImage);

	render->framebufferRect = (Rectangle){
		.x = 0.0f,
		.y = 0.0f,
		.width = fb->width,
		.height = fb->height,
	};

	render->displayRect = (Rectangle){
		.x = 0.0f,
		.y = 0.0f,
		.width = screen_width,
		.height = screen_height,
	};
}

void renderer_draw_framebuffer(struct renderer_t *render,
							   const framebuffer_t *fb) {
	BeginDrawing();

	UpdateTexture(render->renderTexture, fb->pixels);
	Vector2 origin = {0.0, 0.0};
	DrawTexturePro(render->renderTexture, render->framebufferRect,
				   render->displayRect, origin, 0.0, WHITE);

	EndDrawing();
}
