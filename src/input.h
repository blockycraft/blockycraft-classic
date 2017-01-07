#pragma once

#include "globals.h"
#include "graphics.h"
#include "hit.h"
#include "chunks.h"
#include "sound.h"

void enableLightOnCube() {
    Player* player = m_player;
    glm::vec3 pos(player->x, player->y, player->z);
    Point3D hit = { 0, 0, 0 };

    BlockType type = getBlockHitVector(0, pos, player->rx, player->ry, hit);
    if (hit.y > 0 && hit.y < 256 && isDestructable(type)) {
        int cx = getRelativeChunk(hit.x, m_chunkSetting.size);
        int cz = getRelativeChunk(hit.z, m_chunkSetting.size);
        Chunk *chunk = findChunkByCoord(cx, cz);
        if (chunk != NULL) {
            ChunkDataList *list = &chunk->lights;
            int value = getChunkListItem(list, hit.x, hit.y, hit.z) ? 0 : LIGHT_FACTOR;
            setChunkListItem(list, hit.x, hit.y, hit.z, value);
            markChunkDirty(chunk);
        }
    }
}

void onLeftClick() {
    Player* player = m_player;
    glm::vec3 pos(player->x, player->y, player->z);
    Point3D hit = { 0, 0, 0 };

    BlockType type = getBlockHitVector(0, pos, player->rx, player->ry, hit);
    if (hit.y > 0 && hit.y < 256 && isDestructable(type)) {
        setBlock(hit.x, hit.y, hit.z, EMPTY);
        Mix_PlayChannel (-1, m_walk_dirt[0], 0);
        
        // add particles
        for (int i = 0; i < 3; i++) {
            Particle part;
            part.pos = glm::vec3(hit.x, hit.y, hit.z);
            part.spawn = pos;          
            part.dir = glm::vec3(float_dist(engine), float_dist(engine), float_dist(engine));
            part.time = glfwGetTime();

            m_particles.push_back(part);
        }
    }
}

void onRightClick() {
    Player* player = m_player;
    glm::vec3 pos(player->x, player->y, player->z);
    Point3D hit = { 0, 0, 0 };

    BlockType type = getBlockHitVector(1, pos, player->rx, player->ry, hit);
    if (hit.y > 0 && hit.y < 256 && isObstacle(type)) {
        if (!isBlockCollision(2, pos, hit.x, hit.y, hit.z)) {
            setBlock(hit.x, hit.y, hit.z, blockTypes[m_item_index]);

            Mix_PlayChannel (-1, m_walk_grass[0], 0);
        }
    }
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_RELEASE || action != GLFW_PRESS) {
        return;
    }

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, 1);
    }

    if (key == GLFW_KEY_X) {
        m_graphics.timeOfDay += 0.1f;
        m_graphics.timeOfDay = glm::clamp(m_graphics.timeOfDay, 0.0f, 1.0f);
    }

    if (key == GLFW_KEY_Z) {
        m_graphics.timeOfDay -= 0.1f;
        m_graphics.timeOfDay = glm::clamp(m_graphics.timeOfDay, 0.0f, 1.0f);
    }

    if (key == GLFW_KEY_J) {
        m_chunkSetting.ratio += 0.01f;
    }

    if (key == GLFW_KEY_K) {
        m_chunkSetting.ratio -= 0.01f;
        if (m_chunkSetting.ratio < 0.01f) {
            m_chunkSetting.ratio = 0.01f;
        }
    }

    if (key == GLFW_KEY_P) {
        no_particle_move = !no_particle_move;
    }

    if (key == GLFW_KEY_O) {
        no_orientation = !no_orientation;
    }

    if (key == GLFW_KEY_2) {
        m_graphics.crosshairs = !m_graphics.crosshairs;
        m_graphics.items = !m_graphics.items;
    }

    if (key == GLFW_KEY_3) {
        is_textured = !is_textured;
    }

    if (key == GLFW_KEY_7) {
        is_transparent = !is_transparent;
    }

    if (key == GLFW_KEY_8) {
        m_graphics.clouds = !m_graphics.clouds;
    }

    if (key == GLFW_KEY_9) {
        m_graphics.particles = !m_graphics.particles;
    }

    if (key == m_keys.light_it) {
        enableLightOnCube();
    }

    if (key == m_keys.fly) {
        m_flying = !m_flying;
    }

    if (key ==  m_keys.reset_world) {
        m_local_running = false;
    }

    if (key ==  m_keys.chunk_inc) {        
        m_chunkSetting.create_radius++;
    }

    if (key ==  m_keys.chunk_dec) {
        m_chunkSetting.create_radius--;
        if (m_chunkSetting.create_radius < 1) {
            m_chunkSetting.create_radius = 1;
        }
    }

    if (key == m_keys.item_next) {
        m_item_index = (m_item_index + 1) % blockTypes.size();
    }

    if (key == m_keys.item_prev) {
        m_item_index--;
        if (m_item_index < 0) {
            m_item_index = blockTypes.size() - 1;
        }
    }
}

void onMouseButtonClick(GLFWwindow *window, int button, int action, int mods) {
    if (action != GLFW_PRESS) {
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        onLeftClick();
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        onRightClick();        
    }
}

void handleMouseMove() {
    static double px = 0;
    static double py = 0;

    double mx, my;
    glfwGetCursorPos(m_window, &mx, &my);

    Player* player = m_player;
    if (px != 0 || py != 0) {
        player->rx += (mx - px) * 0.0025;
        player->ry -= (my - py) * 0.0025;
        
        if (player->rx < 0) {
            player->rx += 2 * PI;
        }
        if (player->rx >= 2 * PI) {
            player->rx -= 2 * PI;
        }
        
        player->ry = MAX(player->ry, -PI / 2);
        player->ry = MIN(player->ry, PI / 2);
        px = mx;
        py = my;
    }
    else {
        glfwGetCursorPos(m_window, &px, &py);
    }
}

void handle_movement(double dt) {
    static float dy = 0; 

    Player* player = m_player;  
    int velocity_x = 0, velocity_z = 0;

    float m = dt * 1.0;
    if (glfwGetKey(m_window, m_keys.forward)) { velocity_z--; }
    if (glfwGetKey(m_window, m_keys.backward)) { velocity_z++; }
    if (glfwGetKey(m_window, m_keys.left)) { velocity_x--; }
    if (glfwGetKey(m_window, m_keys.right)) { velocity_x++; }
    if (glfwGetKey(m_window, m_keys.look_left)) { player->rx -= m; }
    if (glfwGetKey(m_window, m_keys.look_right)) { player->rx += m; }
    if (glfwGetKey(m_window, m_keys.look_up)) { player->ry += m; }
    if (glfwGetKey(m_window, m_keys.look_down)) { player->ry -= m; }
    
    glm::vec3 motion = getMotionVector(m_flying, velocity_z, velocity_x, player->rx, player->ry);
    if (glfwGetKey(m_window, m_keys.jump)) {
        if (m_flying) {
            motion.y = 1;
        }
        else if (dy == 0) {
            dy = 8;
        }
    }
    
    float speed = m_flying ? FLY_SPEED : WALK_SPEED;

    glm::vec3 est(powf(motion.x * speed, 2), powf(motion.y * speed + ABS(dy) * 2, 2), powf(motion.z * speed, 2) * dt * 8);
    int estimate = MAX(8, roundf(est.x + est.y + est.z));
    float ut = dt / estimate;

    motion.x = motion.x * ut * speed;
    motion.y = motion.y * ut * speed;
    motion.z = motion.z * ut * speed;
    for (int i = 0; i < estimate; i++) {
        if (m_flying) {
            dy = 0;
        } else {
            dy -= ut * 25;
            dy = MAX(dy, -250);
        }

        player->x += motion.x;
        player->y += motion.y + dy * ut;
        player->z += motion.z;

        if (isBlockCollision(2, &player->x, &player->y, &player->z)) {
            dy = 0;
        }
    }

    if (player->y < 0) {
        player->y = getChunkMax(player->x, player->z) + 2;
    }
}
