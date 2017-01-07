#pragma once

#include "globals.h"
#include "graphics.h"

void renderUiBlock(ShaderAttributes *attrib, BlockType type) {
    // shrink and put the cube on the bottom left (rotated by a bit)
    glm::mat4 matrix;
    matrix = glm::rotate(matrix, (float)-PI / 4, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, (float)-PI / 10, glm::vec3(1, 0, 0));
    matrix = glm::translate(matrix, glm::vec3(-1.0f, -1.0f, 0.0f));
    matrix = glm::scale(matrix, glm::vec3(0.15f));

    glUseProgram(attrib->program);
    glUniformMatrix4fv(attrib->matrix, 1, GL_FALSE, value_ptr(matrix));
    glUniform3f(attrib->camera, 0, 0, 5);
    glUniform1i(attrib->sampler, 0);
    glUniform1i(attrib->extra3, is_transparent);
    glUniform1f(attrib->timer, time_of_day());

    GLuint buffer = uploadCubeDataToVbos(0, 0, 0, 0.5, type);
    draw_triangles_3d_ao(attrib, buffer, 36);
    glDeleteBuffers(1, &buffer);
}
