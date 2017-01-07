#pragma once

#include <vector>
#include <map>

#include "mathhelper.h"

typedef int CubeFaceTexture;

typedef struct {
    CubeFaceTexture left, right, top, bottom, front, back;
} BlockDefinition;

#define BLOCK_FULL_VISIBLE { true, true, true, true, true, true };

typedef struct {
    bool left, right, top, bottom, front, back;
} BlockVisibility;

typedef struct {
    int x;
    int y;
    int z;
    int w;
} Block;

enum BlockType {
    EMPTY = 0,
    GRASS = 1,
    SAND = 2,
    STONE = 3,
    BRICK = 4,
    WOOD = 5,
    CEMENT = 6,
    DIRT = 7,
    PLANK = 8,
    SNOW = 9,
    GLASS = 10,
    COBBLE = 11,
    LIGHT_STONE = 12,
    DARK_STONE = 13,
    CHEST = 14,
    LEAVES = 15,
    CLOUD = 16,
    COLOR_00 = 17,
    COLOR_01 = 18,
    COLOR_02 = 19,
    COLOR_03 = 20,
    COLOR_04 = 21,
    COLOR_05 = 22,
    COLOR_06 = 23,
    COLOR_07 = 24,
    ROCK = 25,
    BEDROCK = 26,
    COLOR_10 = 27,
    COLOR_11 = 28,
    COLOR_12 = 29,
    COLOR_13 = 30,
    COLOR_14 = 31,
    COLOR_15 = 32,
    COLOR_16 = 33,
    COLOR_17 = 34,
    COLOR_18 = 35,
    COLOR_19 = 36,
    COLOR_20 = 37,
    COLOR_21 = 38,
    COLOR_22 = 39,
    COLOR_23 = 40,
    COLOR_24 = 41,
    COLOR_25 = 42,
    COLOR_26 = 43,
    COLOR_27 = 44,
    COLOR_28 = 45,
    WHITE = 46,
    COLOR_30 = 47,
    COLOR_31 = 48,
    DUST
};

bool isObstacle(BlockType type);
bool isTransparent(BlockType type);
bool isDestructable(BlockType type);

std::vector<BlockType> getBlockTypes();
std::vector<BlockType> getInteractiveTypes();
std::map<BlockType, BlockDefinition> getBlockDefinitions();
BlockDefinition getBlockDefinition(BlockType type);

extern std::map<BlockType, BlockDefinition> m_definitions;