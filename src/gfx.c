#include "gfx.h"
#include "framebuffer.h"
#include "raylib.h"

void renderer_init(renderer_t *render, float screen_width, float screen_height,
				   const framebuffer_t *fb) {

	InitWindow(screen_width, screen_height, "test raylib");
	SetTargetFPS(60);

	Image bwImage;
	bwImage.data = render->framebuffer;
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

void renderer_draw_framebuffer(renderer_t *render, const framebuffer_t *fb) {
	BeginDrawing();

	UpdateTexture(render->renderTexture, fb->pixels);
	Vector2 origin = {0.0, 0.0};
	DrawTexturePro(render->renderTexture, render->framebufferRect,
				   render->displayRect, origin, 0.0, WHITE);

	EndDrawing();
}
