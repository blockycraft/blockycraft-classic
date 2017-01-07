#pragma once

#include "globals.h"
#include "graphics.h"

#define ARRAY_WIDTH (m_chunkSetting.size * 3 + 2)
#define ARRAY_WIDTH_MIN (m_chunkSetting.size)
#define ARRAY_WIDTH_MAX (m_chunkSetting.size * 2 + 1)
#define ARRAY_HEIGHT 258
#define LIGHT_FACTOR 15

int getDistanceFromChunk(Chunk *chunk, int p, int q) {
    return MAX(ABS(chunk->x - p), ABS(chunk->z - q));
}

int getRelativeChunk(float value, int size) {
    return floorf(roundf(value) / size);
}

Chunk* findChunkByCoord(int cx, int cy) {
    for (int i = 0; i < m_chunk_count; i++) {
        Chunk *chunk = m_chunks + i;
        if (chunk->x == cx && chunk->z == cy) {
            return chunk;
        }
    }
    return 0;
}

Chunk* findChunkByCoord(float x, float y) {
    return findChunkByCoord(getRelativeChunk(x, m_chunkSetting.size), getRelativeChunk(y, m_chunkSetting.size));
}

int getChunkMax(float x, float z) {
    int result = -1;

    Chunk *chunk = findChunkByCoord(x, z);
    if (chunk != NULL) {
        ChunkDataList *list = &chunk->list;
        for (unsigned int i = 0; i <= list->length; i++) { 
            ChunkDataListEntry entry = list->entries[i];
            if (EMPTY_ENTRY(entry)) {
                continue;
            }

            BlockType type = (BlockType)entry.value;        
            if (isObstacle(type) && entry.x + list->dx == roundf(x) && entry.z + list->dz == roundf(z)) {
                result = MAX(result, entry.y + list->dy);
            }
        }
    }

    return result;
}

bool isBlockCollision(int height, float *x, float *y, float *z) {
    int cx = getRelativeChunk(*x, m_chunkSetting.size);
    int cy = getRelativeChunk(*z, m_chunkSetting.size);

    Chunk *chunk = findChunkByCoord(cx, cy);
    if (chunk == NULL) {
        return false;
    }
    
    ChunkDataList *list = &chunk->list;
    int nx = roundf(*x);
    int ny = roundf(*y);
    int nz = roundf(*z);
    float px = *x - nx;
    float py = *y - ny;
    float pz = *z - nz;
    float pad = 0.25;

    bool collision = false;
    for (int dy = 0; dy < height; dy++) {
        if (px < -pad && isObstacle((BlockType)getChunkListItem(list, nx - 1, ny - dy, nz))) {
            *x = nx - pad;
        }

        if (px > pad && isObstacle((BlockType)getChunkListItem(list, nx + 1, ny - dy, nz))) {
            *x = nx + pad;
        }

        if (py < -pad && isObstacle((BlockType)getChunkListItem(list, nx, ny - dy - 1, nz))) {
            *y = ny - pad;
            collision = true;
        }

        if (py > pad && isObstacle((BlockType)getChunkListItem(list, nx, ny - dy + 1, nz))) {
            *y = ny + pad;
            collision = true;
        }

        if (pz < -pad && isObstacle((BlockType)getChunkListItem(list, nx, ny - dy, nz - 1))) {
            *z = nz - pad;
        }

        if (pz > pad && isObstacle((BlockType)getChunkListItem(list, nx, ny - dy, nz + 1))) {
            *z = nz + pad;
        }
    }
    return collision;
}

bool isChunkLit(Chunk *chunk) {
    if (!m_graphics.lights) {
        return false;
    }

    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            Chunk *other = chunk;
            if (dp || dq) {
                other = findChunkByCoord(chunk->x + dp, chunk->z + dq);
            }

            if (other == NULL) {
                continue;
            }

            ChunkDataList *list = &other->lights;
            if (list->size) {
                return true;
            }
        }
    }

    return false;
}

void markChunkDirty(Chunk *chunk) {
    chunk->dirty = 1;
    if (isChunkLit(chunk)) {
        for (int dp = -1; dp <= 1; dp++) {
            for (int dq = -1; dq <= 1; dq++) {
                Chunk *other = findChunkByCoord(chunk->x + dp, chunk->z + dq);
                if (other != NULL) {
                    other->dirty = 1;
                }
            }
        }
    }
}

void setLightForArea(bool opaque[], char light[], Point3D point, int strength, int force) {
    if ((point.x + strength < ARRAY_WIDTH_MIN || point.z + strength < ARRAY_WIDTH_MIN)
        || (point.x - strength > ARRAY_WIDTH_MAX || point.z - strength > ARRAY_WIDTH_MAX)
        || (point.y < 0 || point.y >= ARRAY_HEIGHT)
        || (!force && opaque[INDEX3DP(point)])
        || (light[INDEX3DP(point)] >= strength)) {
        return;
    }

    // set lighting and reduce the strength
    light[INDEX3DP(point)] = strength--;

    // set nearby block strengths
    setLightForArea(opaque, light, { point.x - 1, point.y, point.z }, strength, 0);
    setLightForArea(opaque, light, { point.x + 1, point.y, point.z }, strength, 0);
    setLightForArea(opaque, light, { point.x, point.y - 1, point.z }, strength, 0);
    setLightForArea(opaque, light, { point.x, point.y + 1, point.z }, strength, 0);
    setLightForArea(opaque, light, { point.x, point.y, point.z - 1 }, strength, 0);
    setLightForArea(opaque, light, { point.x, point.y, point.z + 1 }, strength, 0);
}

void computeChunk(ChunkGenParams &params) {
    bool opaque[ARRAY_WIDTH * ARRAY_WIDTH * ARRAY_HEIGHT];
    char light[ARRAY_WIDTH * ARRAY_WIDTH * ARRAY_HEIGHT];
    int highest[ARRAY_WIDTH * ARRAY_WIDTH];
    bzero(opaque, sizeof(opaque));
    bzero(light, sizeof(light));
    bzero(highest, sizeof(highest));

    Point3D origin = {
        params.x * m_chunkSetting.size - m_chunkSetting.size - 1,
        -1,
        params.z * m_chunkSetting.size - m_chunkSetting.size - 1
    };

    bool has_light = false;
    if (m_graphics.lights) {
        for (int dx = 0; dx < 3; dx++) {
            for (int dz = 0; dz < 3; dz++) {
                ChunkDataList *list = params.light_maps[dx][dz];
                if (list != NULL && list->size != 0) {
                    has_light = true;
                }
            }
        }
    }

    for (int dx = 0; dx < 3; dx++) {
        for (int dy = 0; dy < 3; dy++) {
            ChunkDataList *list = params.block_maps[dx][dy];
            if (list == NULL) {
                continue;
            }

            for (unsigned int i = 0; i <= list->length; i++) {
                ChunkDataListEntry entry = list->entries[i];
                if (EMPTY_ENTRY(entry)) {
                    continue;
                }

                int x = (entry.x + list->dx) - origin.x;
                int y = (entry.y + list->dy) - origin.y;
                int z = (entry.z + list->dz) - origin.z;
                BlockType type = (BlockType)entry.value;
                
                if (x >= ARRAY_WIDTH || y >= ARRAY_HEIGHT || z >= ARRAY_WIDTH) {
                    continue;
                }

                bool is_opaque = !isTransparent(type);
                opaque[INDEX3D(x, y, z)] = is_opaque;

                if (is_opaque) {
                    highest[INDEX2D(x, z)] = MAX(highest[INDEX2D(x, z)], y);
                }
            }            
        }
    }

    if (has_light) {
        for (int dx = 0; dx < 3; dx++) {
            for (int dy = 0; dy < 3; dy++) {
                ChunkDataList *list = params.light_maps[dx][dy];
                if (list == NULL) {
                    continue;
                }
                
                for (unsigned int i = 0; i <= list->length; i++) {
                    ChunkDataListEntry entry = list->entries[i];
                    if (EMPTY_ENTRY(entry)) {
                        continue;
                    }

                    Point3D point = { (entry.x + list->dx) - origin.x, (entry.y + list->dy) - origin.y, (entry.z + list->dz) - origin.z };
                    setLightForArea(opaque, light, point, entry.value, 1);
                }                
            }
        }
    }

    ChunkDataList *list = params.block_maps[1][1];
    int miny = 256, maxy = 0, faces = 0;
    std::vector<Vertex> vertices;
    vertices.reserve(VERT_PER_FACE * list->size * m_chunkSetting.size);
    for (unsigned int i = 0; i <= list->length; i++) {
        ChunkDataListEntry entry = list->entries[i];
        if (EMPTY_ENTRY(entry) || entry.value <= 0) {
            continue;
        }

        glm::vec3 position(entry.x + list->dx, entry.y + list->dy, entry.z + list->dz);
        int x = (int)position.x - origin.x;
        int y = (int)position.y - origin.y;
        int z = (int)position.z - origin.z;

        BlockVisibility bvis = { 
            !opaque[INDEX3D(x - 1, y, z)], !opaque[INDEX3D(x + 1, y, z)], !opaque[INDEX3D(x, y + 1, z)],
            !opaque[INDEX3D(x, y - 1, z)] && (entry.y + list->dy > 0), 
            !opaque[INDEX3D(x, y, z - 1)], !opaque[INDEX3D(x, y, z + 1)]
        };

        int total = bvis.left + bvis.right + bvis.top + bvis.bottom + bvis.front + bvis.back;
        if (total == 0) {
            continue;
        }

        bool neighbors[NEIGHBOURS] = { false };
        char lightings[NEIGHBOURS] = {0};
        float shades[NEIGHBOURS] = {0};

        // Get from our neighbours (9x9x9) values
        int index = 0;
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                for (int dz = -1; dz <= 1; dz++) {
                    neighbors[index] = opaque[INDEX3D(x + dx, y + dy, z + dz)];
                    lightings[index] = light[INDEX3D(x + dx, y + dy, z + dz)];
                    shades[index] = 0;
                    if (y + dy <= highest[INDEX2D(x + dx, z + dz)]) {
                        for (int my = 0; my < 8; my++) {
                            if (opaque[INDEX3D(x + dx, y + dy + my, z + dz)]) {
                                shades[index] = 1.0 - my * 0.125;
                                break;
                            }
                        }
                    }
                    index++;
                }
            }
        }

        // ---------------
        BlockDefinition cdefintion = getBlockDefinition((BlockType)entry.value);

        glm::vec4 lighting[NUM_FACES];
        glm::vec4 occlus[NUM_FACES];
        occlusion(neighbors, lightings, shades, occlus, lighting);

        BlockVisibility bdef = bvis;      

        constructCube(vertices, occlus, lighting, bdef, cdefintion, position, 0.5);

        miny = MIN(miny, entry.y + list->dy);
        maxy = MAX(maxy, entry.y + list->dy);
        faces += total;
    }

    void* data_copy = (void*)malloc(sizeof(Vertex) * vertices.size());
    memcpy(data_copy, vertices.data(), sizeof(Vertex) * vertices.size());

    params.miny = miny;
    params.maxy = maxy;
    params.faces = faces;
    params.data = data_copy;
}

void uploadChunkDataToVbos(Chunk *chunk) {
    ChunkGenParams params;
    
    params.x = chunk->x;
    params.z = chunk->z;
    for (int dp = -1; dp <= 1; dp++) {
        for (int dq = -1; dq <= 1; dq++) {
            Chunk *other = chunk;
            if (dp != 0 || dq != 0) {
                other = findChunkByCoord(chunk->x + dp, chunk->z + dq);
            }
            if (other) {
                params.block_maps[dp + 1][dq + 1] = &other->list;
                params.light_maps[dp + 1][dq + 1] = &other->lights;
            }
            else {
                params.block_maps[dp + 1][dq + 1] = 0;
                params.light_maps[dp + 1][dq + 1] = 0;
            }
        }
    }

    computeChunk(params);

    chunk->miny = params.miny;
    chunk->maxy = params.maxy;
    chunk->faces = params.faces;
    glDeleteBuffers(1, &chunk->buffer);

    GLuint buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * VERT_PER_FACE * 10 * params.faces, params.data, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    free(params.data);
    chunk->buffer = buffer;

    chunk->dirty = 0;
}

void createChunk(Chunk *chunk, int p, int q) {
    chunk->x = p;
    chunk->z = q;
    chunk->faces = 0;
    chunk->buffer = 0;
    markChunkDirty(chunk);

    ChunkDataList *block_map = &chunk->list;
    ChunkDataList *light_map = &chunk->lights;
    int dx = p * m_chunkSetting.size - 1;
    int dy = 0;
    int dz = q * m_chunkSetting.size - 1;
    createChunkList(block_map, dx, dy, dz, 0x7fff);
    createChunkList(light_map, dx, dy, dz, 0xf);

    ChunkGenParams params;
    params.x = chunk->x;
    params.z = chunk->z;
    params.block_maps[1][1] = &chunk->list;
    params.light_maps[1][1] = &chunk->lights;

    ChunkDataList *c_block_map = params.block_maps[1][1];
    createWorld(params.x, params.z, m_chunkSetting.size, m_chunkSetting.ratio, c_block_map);
}

void createChunks(int p, int q) {
    int r = 1;
    for (int dp = -r; dp <= r; dp++) {
        for (int dq = -r; dq <= r; dq++) {
            int a = p + dp;
            int b = q + dq;
            Chunk *chunk = findChunkByCoord(a, b);
            if (chunk) {
                if (chunk->dirty) {
                    uploadChunkDataToVbos(chunk);
                }
            }
            else if (m_chunk_count < MAX_CHUNKS) {
                chunk = m_chunks + m_chunk_count++;
                createChunk(chunk, a, b);
                uploadChunkDataToVbos(chunk);
            }
        }
    }
}

void reuploadChunkDataToVbos(int p, int q) {
    for (int cx = -m_chunkSetting.render_radius; cx < m_chunkSetting.render_radius; cx++) {
        for (int cz = -m_chunkSetting.render_radius; cz < m_chunkSetting.render_radius; cz++) {
            Chunk* chunk = findChunkByCoord(p + cx, q + cz);
            if (chunk != NULL) {
                if (chunk->dirty) {
                    uploadChunkDataToVbos(chunk);
                }
            }
        }
    }
}

void setBlock(int x, int y, int z, int type) {
    int cx = getRelativeChunk(x, m_chunkSetting.size);
    int cz = getRelativeChunk(z, m_chunkSetting.size);

    Chunk *chunk = findChunkByCoord(cx, cz);
    if (chunk != NULL) {
        ChunkDataList *list = &chunk->list;
        if (setChunkListItem(list, x, y, z, type)) {
            markChunkDirty(chunk);
        }
    }

    if (type == 0 
        && getRelativeChunk(x, m_chunkSetting.size) == cx 
        && getRelativeChunk(z, m_chunkSetting.size) == cz) {

        ChunkDataList *light_mmap = &chunk->lights;
        if (setChunkListItem(light_mmap, x, y, z, 0)) {
            markChunkDirty(chunk);
        }
    }

    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            if ((dx == 0 && dz == 0)
                || (dx != 0 && getRelativeChunk(x + dx, m_chunkSetting.size) == cx)
                || (dz != 0 && getRelativeChunk(z + dz, m_chunkSetting.size) == cz)) {
                continue;
            }
            
            int dcx = cx + dx;
            int dcz = cz + dz;

            chunk = findChunkByCoord(dcx, dcz);
            if (chunk != NULL) {
                ChunkDataList *list = &chunk->list;
                if (setChunkListItem(list, x, y, z, type)) {
                    markChunkDirty(chunk);            
                }
            }

            if (type == 0 
                && getRelativeChunk(x, m_chunkSetting.size) == dcx 
                && getRelativeChunk(z, m_chunkSetting.size) == dcz) {

                ChunkDataList *light_mmap = &chunk->lights;
                if (setChunkListItem(light_mmap, x, y, z, 0)) {
                    markChunkDirty(chunk);
                }
            }
        }
    }
}

void renderChunks(ShaderAttributes *attrib, Player *player) {
    int p = getRelativeChunk(player->x, m_chunkSetting.size);
    int q = getRelativeChunk(player->z, m_chunkSetting.size);
    reuploadChunkDataToVbos(p, q);

    float light = time_of_day();
    float aspect = ((float)m_width) / m_height;
    
    glm::mat4 m_perpsective = glm::perspective(degreesToRadians(m_fov), aspect, 0.125f, m_chunkSetting.render_radius * 32.0f + 64.0f);

    glm::mat4 matrix;
    matrix = glm::rotate(matrix, player->rx, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, -player->ry, glm::normalize(glm::vec3(cosf(player->rx), 0, sinf(player->rx))));
    matrix = glm::translate(matrix, glm::vec3(-player->x, -player->y, -player->z));   

    matrix = m_perpsective * matrix;

    glUseProgram(attrib->program);
    glUniformMatrix4fv(attrib->matrix, 1, GL_FALSE, value_ptr(matrix));
    glUniform3f(attrib->camera, player->x, player->y, player->z);
    glUniform1i(attrib->sampler, 0);
    glUniform1i(attrib->extra1, is_textured);
    glUniform1f(attrib->extra2, light);
    glUniform1f(attrib->extra3, is_transparent);
    glUniform1f(attrib->timer, time_of_day());
    for (int i = 0; i < m_chunk_count; i++) {
        Chunk *chunk = m_chunks + i;
        if (getDistanceFromChunk(chunk, p, q) > m_chunkSetting.render_radius) {
            continue;
        }

        draw_triangles_3d_ao(attrib, chunk->buffer, chunk->faces * VERT_PER_FACE);
    }
}
