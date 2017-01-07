#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#include "chunklist.h"

typedef struct {
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 spawn;
    double time;
} Particle;

typedef struct {
    float x, y, z;
    int index;
    float time;
    std::vector<glm::vec3> positions;
    std::vector<float> orientations;
} Cloud;

typedef struct {
    GLuint buffer;
    ChunkDataList list;
    ChunkDataList lights;
    int x, z;
    int faces;
    int dirty;
    int miny, maxy;
} Chunk;

typedef struct {
    int x, z;
    ChunkDataList *block_maps[3][3];
    ChunkDataList *light_maps[3][3];
    int miny, maxy;
    int faces;
    void *data;
} ChunkGenParams;

void reseedWorld();

void createWorld(int wx, int wz, int chunk_size, float ratio, ChunkDataList* list);
