#pragma once

#include "globals.h"
#include "graphics.h"

void setupShaders(ShaderAttributes* block_attrib, ShaderAttributes* line_attrib) {
    GLuint program;

    program = load_program("../shaders/block.vs", "../shaders/block.fs");
    block_attrib->program = program;
    block_attrib->position = glGetAttribLocation(program, "position");
    block_attrib->normal = glGetAttribLocation(program, "normal");
    block_attrib->uv = glGetAttribLocation(program, "uv");
    block_attrib->matrix = glGetUniformLocation(program, "matrix");
    block_attrib->sampler = glGetUniformLocation(program, "sampler");
    block_attrib->extra1 = glGetUniformLocation(program, "texturing");
    block_attrib->extra2 = glGetUniformLocation(program, "daylight");
    block_attrib->extra3 = glGetUniformLocation(program, "transparency");
    block_attrib->camera = glGetUniformLocation(program, "camera");
    block_attrib->timer = glGetUniformLocation(program, "timer");

    program = load_program("../shaders/line.vs", "../shaders/line.fs");
    line_attrib->program = program;
    line_attrib->position = glGetAttribLocation(program, "position");
    line_attrib->matrix = glGetUniformLocation(program, "matrix");
}

void loadTextures() {
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    load_png_texture("../assets/texture.png");
}
