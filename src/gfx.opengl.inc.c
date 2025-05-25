#include "glad/glad.h"

// for glfwGetProcAddress
#include "GLFW/glfw3.h"

#include "framebuffer.h"
#include "param_annotations.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct renderer_t {
	GLuint vao, vbo, shader, texture;
};

// Vertex shader source
static const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "layout (location = 1) in vec2 aTexCoord;\n"
    "out vec2 TexCoord;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "    TexCoord = aTexCoord;\n"
    "}\0";

// Fragment shader source
static const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoord;\n"
    "uniform sampler2D ourTexture;\n"
    "void main() {\n"
    "    float gray = texture(ourTexture, TexCoord).r;\n"
    "    FragColor = vec4(gray, gray, gray, 1.0);\n"
    "}\0";

static GLuint compile_shader(GLenum type, const char *source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Check for compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		fprintf(stderr, "Shader compilation failed: %s\n", infoLog);
	}

	return shader;
}

static GLuint create_shader_program() {
	GLuint vertexShader =
	    compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
	GLuint fragmentShader =
	    compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Check for linking errors
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		fprintf(stderr, "Shader program linking failed: %s\n", infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void renderer_init(struct renderer_t **render, const uint64_t screen_width,
                   const uint64_t screen_height, IN const framebuffer_t *fb) {

	struct renderer_t *renderer = calloc(sizeof(struct renderer_t), 1);
	*render = renderer;

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	// Set viewport
	glViewport(0, 0, screen_width, screen_height);

	// clang-format off
	// TODO: AI:
	float vertices[] = {
	    // Positions        // Texture Coords
        -1.0f, -1.0f,    0.0f, 1.0f,  // Bottom left
         1.0f, -1.0f,    1.0f, 1.0f,  // Bottom right
         1.0f,  1.0f,    1.0f, 0.0f,  // Top right
        -1.0f,  1.0f,    0.0f, 0.0f   // Top left
	};
	// clang-format on

	unsigned int indices[] = {
	    0, 1, 2, // First triangle
	    2, 3, 0  // Second triangle
	};

	glGenVertexArrays(1, &renderer->vao);
	glBindVertexArray(renderer->vao);

	glGenBuffers(1, &renderer->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	             GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
	                      (void *)0);
	glEnableVertexAttribArray(0);

	// Texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
	                      (void *)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	renderer->shader = create_shader_program();

	glGenTextures(1, &renderer->texture);
	glBindTexture(GL_TEXTURE_2D, renderer->texture);

	// TODO: AI:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Initialize texture with framebuffer data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fb->width, fb->height, 0, GL_RED,
	             GL_UNSIGNED_BYTE, fb->pixels);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void renderer_draw_framebuffer(IN struct renderer_t *render,
                               IN const framebuffer_t *fb) {

	// Clear the screen
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Bind shader program
	glUseProgram(render->shader);

	// Bind texture and update with new framebuffer data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, render->texture);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, fb->width, fb->height, GL_RED,
	                GL_UNSIGNED_BYTE, fb->pixels);

	// Set texture uniform
	glUniform1i(glGetUniformLocation(render->shader, "ourTexture"), 0);

	// Bind VAO and draw
	glBindVertexArray(render->vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Unbind
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}
