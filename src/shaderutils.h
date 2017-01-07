#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
    GLuint program;
    GLuint position;
    GLuint normal;
    GLuint uv;
    GLuint matrix;
    GLuint sampler;
    GLuint camera;
    GLuint timer;
    GLuint extra1;
    GLuint extra2;
    GLuint extra3;
    GLuint extra4;
} ShaderAttributes;

GLuint make_shader(GLenum type, const char *source);
GLuint load_shader(GLenum type, const char *path);

GLuint make_program(GLuint shader1, GLuint shader2);
GLuint load_program(const char *path1, const char *path2);

void load_png_texture(const char *file_name);