#pragma once

#include "globals.h"

float time_of_day() {
    return m_graphics.timeOfDay;
}

int create_glfw_context() {
    if (!glfwInit()) {
        return -1;
    }

    int window_width = m_graphics.width;
    int window_height = m_graphics.height;
    m_window = glfwCreateWindow(window_width, window_height, "Blockycraft", NULL, NULL);
    if (!m_window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glewInit() != GLEW_OK) {
        return -1;
    }

    return 0;
}

void draw_triangles_3d_ao(ShaderAttributes *attrib, GLuint buffer, int count) {
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glEnableVertexAttribArray(attrib->position);
    glEnableVertexAttribArray(attrib->normal);
    glEnableVertexAttribArray(attrib->uv);

    glVertexAttribPointer(attrib->position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(attrib->normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(GLfloat) * 3));
    glVertexAttribPointer(attrib->uv, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)(sizeof(GLfloat) * 6));
    glDrawArrays(GL_TRIANGLES, 0, count);

    glDisableVertexAttribArray(attrib->position);
    glDisableVertexAttribArray(attrib->normal);
    glDisableVertexAttribArray(attrib->uv);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint uploadCubeDataToVbos(float x, float y, float z, float n, BlockType type) {
    glm::vec3 pos(x, y, z);

    glm::vec4 occlsn[NUM_FACES] = { 
        glm::vec4(), glm::vec4(), glm::vec4(), 
        glm::vec4(), glm::vec4(), glm::vec4() 
    };

    glm::vec4 lighting[NUM_FACES] = { 
        glm::vec4(0.5), glm::vec4(0.5), glm::vec4(0.5), 
        glm::vec4(0.5), glm::vec4(0.5), glm::vec4(0.5) 
    };

    BlockVisibility visible = BLOCK_FULL_VISIBLE;
    BlockDefinition definition = getBlockDefinition(type);

    std::vector<Vertex> vertices;
    constructCube(vertices, occlsn, lighting, visible, definition, pos, n);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return buffer;
}
