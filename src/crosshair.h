#pragma once

#include "globals.h"
#include "graphics.h"

using namespace glm;

GLuint uploadCrosshairDataToVbos() {
    int x = m_width / 2;
    int y = m_height / 2;
    int p = 15 * m_scale;

    // the cross
    glm::vec2 points[] = { 
        vec2(x, y - p), 
        vec2(x, y + p), 
        vec2(x - p, y), 
        vec2(x + p, y) 
    };

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}

void renderCrosshair(ShaderAttributes *attrib) {
    glm::mat4 matrix = glOrtho(0, (float)m_width, 0, (float)m_height, -1, 1);

    glUseProgram(attrib->program);
    glLineWidth(4 * m_scale);
    glEnable(GL_COLOR_LOGIC_OP);
    glUniformMatrix4fv(attrib->matrix, 1, GL_FALSE, value_ptr(matrix));

    GLuint crosshair_buffer = uploadCrosshairDataToVbos();
    glBindBuffer(GL_ARRAY_BUFFER, crosshair_buffer);
    glEnableVertexAttribArray(attrib->position);
    glVertexAttribPointer(attrib->position, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_LINES, 0, 4);
    glDisableVertexAttribArray(attrib->position);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDeleteBuffers(1, &crosshair_buffer);
    glDisable(GL_COLOR_LOGIC_OP);
}
