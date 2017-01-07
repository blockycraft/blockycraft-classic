#pragma once

#include "globals.h"
#include "graphics.h"

glm::mat4 billboard(glm::mat4 matrix) {
    double d = sqrt(powf(matrix[0][0], 2) + powf(matrix[0][1], 2) + powf(matrix[0][2], 2));

    glm::mat4 result;
    result[0][0] = (float)d;
    result[1][1] = (float)d;
    result[2][2] = (float)d;

    result[3] = matrix[3];
    return result;
}

void renderParticles(ShaderAttributes *attrib, Player *player, double now) {
    double waiting_time = 0.5;
    if (no_particle_move) {
        waiting_time = 5.0;
    }

    for (int i = m_particles.size() - 1; i >= 0; i--) {
        if (now - m_particles[i].time > waiting_time) {
            m_particles.erase(m_particles.begin() + i);
        }
    }

    float light = time_of_day();
    float aspect = ((float)m_width) / m_height;
    
    glm::mat4 m_perpsective = glm::perspective(degreesToRadians(m_fov), aspect, 0.125f, m_chunkSetting.render_radius * 32.0f + 64.0f);

    glm::mat4 m_view;
    m_view = glm::rotate(m_view, player->rx, glm::vec3(0, 1, 0));
    m_view = glm::rotate(m_view, -player->ry, glm::normalize(glm::vec3(cosf(player->rx), 0, sinf(player->rx))));

    glUseProgram(attrib->program);
    glUniform3f(attrib->camera, player->x, player->y, player->z);
    glUniform1i(attrib->sampler, 0);
    glUniform1i(attrib->extra1, is_textured);
    glUniform1f(attrib->extra2, time_of_day());
    glUniform1f(attrib->extra3, is_transparent);
    glUniform1f(attrib->timer, time_of_day());
    
    glm::vec3 pos(player->x, player->y, player->z);
    for (int i = 0; i < m_particles.size(); i++) {
        glm::mat4 world;
        Particle part = m_particles[i];
        double dt = now - part.time;

        world = glm::translate(world, -pos + part.pos); 
        if (!no_particle_move) {
            world = glm::translate(world, glm::vec3(part.dir.x * dt, part.dir.y * dt, part.dir.z * dt)); 
            world = glm::scale(world, glm::vec3(0.15, 0.15, 0.15)); 
        }       
      
        world = m_view * world;
        world = billboard(world);

        glUniformMatrix4fv(attrib->matrix, 1, GL_FALSE, value_ptr(m_perpsective * world));
        draw_triangles_3d_ao(attrib, dust_buffer, 36);
    }
}

void uploadDustDataToVbos() {
    BlockType type = DUST;
    dust_buffer = uploadCubeDataToVbos(0, 0, 0, 0.5, type);
}