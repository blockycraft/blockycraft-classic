#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include "cs488-framework/MathUtils.hpp"
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <random>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <time.h>

#include "settings.h"
#include "cube.h"
#include "block.h"
#include "chunklist.h"
#include "shaderutils.h"
#include "world.h"
#include "occlusion.h"
#include "perlin.h"
#include "mathhelper.h"
#include "player.h"

#include "sound.h"
#include "graphics.h"
#include "clouds.h"
#include "particles.h"
#include "hit.h"
#include "crosshair.h"
#include "input.h"
#include "environment.h"
#include "ui.h"

using namespace glm;

#include "globals.h"

void reset() {
    memset(m_chunks, 0, sizeof(Chunk) * MAX_CHUNKS);
    m_chunk_count = 0;
    m_player = new Player();
    m_flying = 0;
    m_item_index = 0;
    m_graphics.timeOfDay = 1.0f;
    m_fov = 65;
    reseedWorld();
}

void initSettings() {
    is_transparent = true;
    is_textured = true;
    no_particle_move = false;
    no_orientation = false;

    // Keybindings 
    m_keys.forward = GLFW_KEY_W;
    m_keys.backward = GLFW_KEY_S;
    m_keys.left = GLFW_KEY_A;
    m_keys.right = GLFW_KEY_D;

    m_keys.look_up = GLFW_KEY_UP;
    m_keys.look_down = GLFW_KEY_DOWN;
    m_keys.look_left = GLFW_KEY_LEFT;
    m_keys.look_right = GLFW_KEY_RIGHT;

    m_keys.jump = GLFW_KEY_SPACE;
    m_keys.fly = GLFW_KEY_TAB;

    m_keys.reset_world = GLFW_KEY_4;

    m_keys.item_next = GLFW_KEY_G;
    m_keys.item_prev = GLFW_KEY_F;

    m_keys.chunk_inc = GLFW_KEY_C;
    m_keys.chunk_dec = GLFW_KEY_V;

    m_keys.light_it = GLFW_KEY_L;

    // Graphics
    m_graphics.lights = true;
    m_graphics.items = true;
    m_graphics.crosshairs = true;
    m_graphics.clouds = false;
    m_graphics.audio = true;
    m_graphics.particles = false;

    m_graphics.timeOfDay = 1;

    m_graphics.width = 1024;
    m_graphics.height = 768;

    // Chunks
    m_chunkSetting.size = 32;
    m_chunkSetting.ratio = 0.01;
    m_chunkSetting.create_radius = 1;
    m_chunkSetting.render_radius = 8;
}

int main(int argc, char **argv) {
    srand(time(NULL));
    rand();

    initSettings();
    initAudio();

    if (create_glfw_context() == -1) {
        return -1;
    }

    // set interactions
    glfwSetKeyCallback(m_window, onKey);
    glfwSetMouseButtonCallback(m_window, onMouseButtonClick);

    playBackgroundMusic(SONG_OVERWORLD);

    glEnable(GL_DEPTH_TEST);
    glLogicOp(GL_INVERT);
    glClearColor(0.650, 0.764, 0.839, 1.0);

    loadTextures();

    ShaderAttributes block_attrib = {0};
    ShaderAttributes line_attrib = {0};
    setupShaders(&block_attrib, &line_attrib);    
    uploadCloudDataToVbos();
    uploadDustDataToVbos();

    int running = 1;
    while (running) {
        reset();

        Player *player = m_player;
        for (int cx = -m_chunkSetting.create_radius; cx < m_chunkSetting.create_radius; cx++) {
            for (int cz = -m_chunkSetting.create_radius; cz < m_chunkSetting.create_radius; cz++) {
                createChunks(player->x + cx, player->z + cz);
            }
        }
        player->y = getChunkMax(player->x, player->z) + 2;

        m_local_running = true;
        double previous = glfwGetTime();
        while (m_local_running) {
            m_scale = (float) m_width / m_height;
            glfwGetFramebufferSize(m_window, &m_width, &m_height);
            glViewport(0, 0, m_width, m_height);

            double now = glfwGetTime();
            double dt = now - previous;
            dt = MIN(dt, 0.2);
            dt = MAX(dt, 0.0);
            previous = now;

            handleMouseMove();
            handle_movement(dt);
            handleAudio(now, dt);

            Player *player = m_player;                   

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            renderChunks(&block_attrib, player);

            if (m_graphics.clouds) {
                renderClouds(&block_attrib, player, dt);  
            }

            if (m_graphics.particles) {
                renderParticles(&block_attrib, player, now);  
            }
            
            glClear(GL_DEPTH_BUFFER_BIT);
            if (m_graphics.crosshairs) {
                renderCrosshair(&line_attrib);
            }

            if (m_graphics.items) {
                renderUiBlock(&block_attrib, blockTypes[m_item_index]);
            }

            glfwSwapBuffers(m_window);
            glfwPollEvents();
            if (glfwWindowShouldClose(m_window)) {
                running = 0;
                break;
            }
        }
    }

    glfwTerminate();
    Mix_FreeMusic(m_overworld);
    Mix_FreeMusic(m_underworld);
    SDL_Quit();
    return 0;
}
