#pragma once

#include "block.h"
#include "mathhelper.h"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

#define NUM_FACES 6
#define VERT_PER_FACE 6

void constructCube(
        std::vector<Vertex>& vertices,
        glm::vec4 occlusion[NUM_FACES], 
        glm::vec4 lighting[NUM_FACES],
        BlockVisibility visibility,
        BlockDefinition wblock,
        glm::vec3 position, float scale);