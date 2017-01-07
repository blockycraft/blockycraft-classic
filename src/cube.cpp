#include "cube.h"
#include "block.h"
#include "shaderutils.h"

#include <math.h>
#include <vector>

using namespace glm;

#define SPRITESHEET_WIDTH 2048.0
#define SPRITESHEET_HEIGHT 2048.0
#define ROW 16
#define COLUMN 16.0

static const glm::vec3 positions[NUM_FACES][4] = {
    {{-1, -1, -1}, {-1, -1, +1}, {-1, +1, -1}, {-1, +1, +1}},
    {{+1, -1, -1}, {+1, -1, +1}, {+1, +1, -1}, {+1, +1, +1}},
    {{-1, +1, -1}, {-1, +1, +1}, {+1, +1, -1}, {+1, +1, +1}},
    {{-1, -1, -1}, {-1, -1, +1}, {+1, -1, -1}, {+1, -1, +1}},
    {{-1, -1, -1}, {-1, +1, -1}, {+1, -1, -1}, {+1, +1, -1}},
    {{-1, -1, +1}, {-1, +1, +1}, {+1, -1, +1}, {+1, +1, +1}}
};

static const glm::vec3 normals[NUM_FACES] = {
    {-1, 0, 0},
    {+1, 0, 0},
    {0, +1, 0},
    {0, -1, 0},
    {0, 0, -1},
    {0, 0, +1}
};

static const glm::vec2 uvs[NUM_FACES][4] = {
    {{0, 0}, {1, 0}, {0, 1}, {1, 1}},
    {{1, 0}, {0, 0}, {1, 1}, {0, 1}},
    {{0, 1}, {0, 0}, {1, 1}, {1, 0}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
    {{1, 0}, {1, 1}, {0, 0}, {0, 1}}
};

static const float indices[NUM_FACES][6] = {
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3},
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3},
    {0, 3, 2, 0, 1, 3},
    {0, 3, 1, 0, 2, 3}
};

static const float flipped[NUM_FACES][6] = {
    {0, 1, 2, 1, 3, 2},
    {0, 2, 1, 2, 3, 1},
    {0, 1, 2, 1, 3, 2},
    {0, 2, 1, 2, 3, 1},
    {0, 1, 2, 1, 3, 2},
    {0, 2, 1, 2, 3, 1}
};

void constructCube(
        std::vector<Vertex>& vertices,
        glm::vec4 occlusion[NUM_FACES], 
        glm::vec4 lighting[NUM_FACES],
        BlockVisibility visibility,
        BlockDefinition wblock,
        vec3 position, float scale) {    
    bool visible[NUM_FACES] = { visibility.left, visibility.right, visibility.top, visibility.bottom, visibility.front, visibility.back };
    int tiles[NUM_FACES] = { wblock.left, wblock.right, wblock.top, wblock.bottom, wblock.front, wblock.back };
    for (int i = 0; i < NUM_FACES; i++) {
        if (!visible[i]) {
            continue;
        }
        
        // compare vert occlusion factors
        glm::vec4 occ = occlusion[i];
        int flip = (occ.x + occ.w) > (occ.y + occ.z);
        for (int v = 0; v < VERT_PER_FACE; v++) {
            int j = flip ? flipped[i][v] : indices[i][v];

            Vertex vert;
            vert.x = position.x + scale * positions[i][j][0];
            vert.y = position.y + scale * positions[i][j][1];
            vert.z = position.z + scale * positions[i][j][2];

            vert.nx = normals[i][0];
            vert.ny = normals[i][1];
            vert.nz = normals[i][2];

            float su = (uvs[i][j][0] != 0 ? 1 / COLUMN - 1 / SPRITESHEET_WIDTH : 1 / SPRITESHEET_WIDTH);
            float sv = (uvs[i][j][1] != 0 ? 1 / COLUMN - 1 / SPRITESHEET_HEIGHT : 1 / SPRITESHEET_HEIGHT);

            vert.u = (tiles[i] % ROW) * (1.0 / COLUMN) + su;
            vert.v = (tiles[i] / ROW) * (1.0 / COLUMN) + sv;

            vert.occlusion = occlusion[i][j];
            vert.lighting = lighting[i][j];

            vertices.push_back(vert);
        }
    }
}