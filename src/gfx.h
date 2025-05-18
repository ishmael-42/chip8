#ifndef INCLUDE_GFX_H
#define INCLUDE_GFX_H

#include "framebuffer.h"
#include "param_annotations.h"
#include <stdint.h>

struct renderer_t;

void renderer_init(struct renderer_t **render, const uint64_t screen_width,
				   const uint64_t screen_height, IN const framebuffer_t *fb);

void renderer_draw_framebuffer(IN struct renderer_t *render,
							   IN const framebuffer_t *fb);

#endif
