#pragma once

#include "globals.h"
#include "graphics.h"

#define NUM_CLOUDS 16
#define NUM_CLOUD_FRAMES 10
#define CLOUD_HEIGHT_LEVEL 75.0f
#define CLOUD_MOVE_SPEED 20.0f
#define CLOUD_INC_SPEED 0.1f
#define CLOUD_RANGE 250.0f

void uploadCloudDataToVbos() {
    BlockType type = CLOUD;
    cloud_buffer = uploadCubeDataToVbos(0, 0, 0, 0.5, type);
    for (int i = 0; i < NUM_CLOUDS; i++) {
        glm::vec3 pos(float_dist(engine) * CLOUD_RANGE, float_dist(engine), float_dist(engine) * CLOUD_RANGE);
        float orient = 0.0f;

        Cloud cld;
        cld.x = pos.x;
        cld.y = pos.y;
        cld.z = pos.z;  
        cld.index = 0;     
        cld.time = 0.0f;
        for (int c = 0; c < NUM_CLOUD_FRAMES; c++) {
            pos.x += float_dist(engine) * 100.0f + 25.0f;
            pos.z += (float_dist(engine) + 1.0f) * 10.0f + 25.0f;
            orient += float_dist(engine) * (PI / 2);

            cld.positions.push_back(glm::vec3(pos.x, pos.y, pos.z));
            cld.orientations.push_back(orient);
        }

        clouds.push_back(cld);
    }
}


void renderClouds(ShaderAttributes *attrib, Player *player, double dt) {
    float light = time_of_day();
    float aspect = ((float)m_width) / m_height;
    
    glm::mat4 m_perpsective = glm::perspective(degreesToRadians(m_fov), aspect, 0.125f, m_chunkSetting.render_radius * 32.0f + 64.0f);

    glm::mat4 viewmatrix;
    viewmatrix = glm::rotate(viewmatrix, player->rx, glm::vec3(0, 1, 0));
    viewmatrix = glm::rotate(viewmatrix, -player->ry, glm::normalize(glm::vec3(cosf(player->rx), 0, sinf(player->rx))));
    viewmatrix = glm::translate(viewmatrix, glm::vec3(-player->x, -player->y + CLOUD_HEIGHT_LEVEL, -player->z));  

    viewmatrix = m_perpsective * viewmatrix;

    glUseProgram(attrib->program);
    glUniform3f(attrib->camera, player->x, player->y, player->z);
    glUniform1i(attrib->sampler, 0);
    
    for (int i = 0; i < clouds.size(); i++) {
        glm::mat4 world;
        Cloud cld = clouds[i];

        glm::vec3 curr_p = cld.positions[cld.index];
        glm::vec3 next_p = cld.positions[cld.index + 1];
        float curr_o = cld.orientations[cld.index];
        float next_o = cld.orientations[cld.index + 1];

        float cldx = lerp(cld.time, curr_p.x, next_p.x);
        float cldy = lerp(cld.time, curr_p.y, next_p.y);
        float cldz = lerp(cld.time, curr_p.z, next_p.z);
        float cldo = lerp(cld.time, curr_o, next_o);

        cld.time += CLOUD_INC_SPEED * dt;
        if (cld.time > 1.0f) {
            cld.time = 0.0f;
            cld.index++;

            if (cld.index >= cld.positions.size()) {
                cld.index = 0;
            }
        }

        clouds[i] = cld;

        if (!no_orientation) {
            world = glm::rotate(world, cldo, glm::vec3(0.0f, 1.0f, 0.0f));
        }        
        world = glm::translate(world, glm::vec3(cldx, cldy, cldz)); 
        world = glm::scale(world, glm::vec3(25.0f, 2.0f, 25.0f));
        glUniformMatrix4fv(attrib->matrix, 1, GL_FALSE, value_ptr(viewmatrix * world));
        draw_triangles_3d_ao(attrib, cloud_buffer, 36);
    }
}
