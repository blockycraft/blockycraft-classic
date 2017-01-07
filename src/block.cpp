#include "block.h"
#include "shaderutils.h"

using namespace std;

#define SINGLE_TEXTURE_CUBE(x) { x, x, x, x, x, x }

std::map<BlockType, BlockDefinition> m_definitions = getBlockDefinitions();

BlockDefinition getBlockDefinition(BlockType type) {
    return m_definitions.at(type);
}

std::map<BlockType, BlockDefinition> getBlockDefinitions() {
    map<BlockType, BlockDefinition> definition;

    definition[EMPTY] = { 0, 0, 0, 0, 0, 0 };
    definition[GRASS] = { 16, 16, 32, 0, 16, 16 };
    definition[SAND] = SINGLE_TEXTURE_CUBE(1);
    definition[STONE] = SINGLE_TEXTURE_CUBE(2);
    definition[BRICK] = SINGLE_TEXTURE_CUBE(3);
    definition[WOOD] = { 20, 20, 36, 4, 20, 20 };
    definition[CEMENT] = SINGLE_TEXTURE_CUBE(5);
    definition[DIRT] = SINGLE_TEXTURE_CUBE(6);
    definition[PLANK] = SINGLE_TEXTURE_CUBE(7);
    definition[SNOW] = { 24, 24, 40, 8, 24, 24 };
    definition[GLASS] = SINGLE_TEXTURE_CUBE(9);
    definition[COBBLE] = SINGLE_TEXTURE_CUBE(10);
    definition[LIGHT_STONE] = SINGLE_TEXTURE_CUBE(11);
    definition[DARK_STONE] = SINGLE_TEXTURE_CUBE(12);
    definition[CHEST] = SINGLE_TEXTURE_CUBE(13);
    definition[LEAVES] = SINGLE_TEXTURE_CUBE(14);
    definition[CLOUD] = SINGLE_TEXTURE_CUBE(15);
    definition[DUST] = SINGLE_TEXTURE_CUBE(80);
    definition[COLOR_00] = SINGLE_TEXTURE_CUBE(48);
    definition[COLOR_01] = SINGLE_TEXTURE_CUBE(49);
    definition[COLOR_02] = SINGLE_TEXTURE_CUBE(50);
    definition[COLOR_03] = SINGLE_TEXTURE_CUBE(51);
    definition[COLOR_04] = SINGLE_TEXTURE_CUBE(52);
    definition[COLOR_05] = SINGLE_TEXTURE_CUBE(53);
    definition[COLOR_06] = SINGLE_TEXTURE_CUBE(54);
    definition[COLOR_07] = SINGLE_TEXTURE_CUBE(55);
    definition[ROCK] = SINGLE_TEXTURE_CUBE(56);
    definition[BEDROCK] = SINGLE_TEXTURE_CUBE(57);
    definition[COLOR_10] = SINGLE_TEXTURE_CUBE(58);
    definition[COLOR_11] = SINGLE_TEXTURE_CUBE(59);
    definition[COLOR_12] = SINGLE_TEXTURE_CUBE(60);
    definition[COLOR_13] = SINGLE_TEXTURE_CUBE(61);
    definition[COLOR_14] = SINGLE_TEXTURE_CUBE(62);
    definition[COLOR_15] = SINGLE_TEXTURE_CUBE(63);
    definition[COLOR_16] = SINGLE_TEXTURE_CUBE(64);
    definition[COLOR_17] = SINGLE_TEXTURE_CUBE(65);
    definition[COLOR_18] = SINGLE_TEXTURE_CUBE(66);
    definition[COLOR_19] = SINGLE_TEXTURE_CUBE(67);
    definition[COLOR_20] = SINGLE_TEXTURE_CUBE(68);
    definition[COLOR_21] = SINGLE_TEXTURE_CUBE(69);
    definition[COLOR_22] = SINGLE_TEXTURE_CUBE(70);
    definition[COLOR_23] = SINGLE_TEXTURE_CUBE(71);
    definition[COLOR_24] = SINGLE_TEXTURE_CUBE(72);
    definition[COLOR_25] = SINGLE_TEXTURE_CUBE(73);
    definition[COLOR_26] = SINGLE_TEXTURE_CUBE(74);
    definition[COLOR_27] = SINGLE_TEXTURE_CUBE(75);
    definition[COLOR_28] = SINGLE_TEXTURE_CUBE(76);
    definition[WHITE] = SINGLE_TEXTURE_CUBE(77);
    definition[COLOR_30] = SINGLE_TEXTURE_CUBE(78);
    definition[COLOR_31] = SINGLE_TEXTURE_CUBE(79);

    return definition;
}

std::vector<BlockType> getInteractiveTypes() {
    vector<BlockType> blocks;
    for (int i = GRASS; i < COLOR_31 + 1; i++) {
        if (i == CLOUD) continue;
        blocks.push_back((BlockType)i);
    }
    return blocks;
}

std::vector<BlockType> getBlockTypes() {
    vector<BlockType> blocks;
    for (int i = EMPTY; i < COLOR_31 + 1; i++) {        
        blocks.push_back((BlockType)i);
    }
    return blocks;
}

bool isObstacle(BlockType type) {
    switch (type) {
        case EMPTY:
        case CLOUD:
            return false;
        default:
            return true;
    }
}

bool isTransparent(BlockType type) {
    switch (type) {
        case EMPTY:
        case GLASS:
        case LEAVES:
            return true;
        default:
            return false;
    }
}

bool isDestructable(BlockType type) {
    switch (type) {
        case EMPTY:
        case CLOUD:
            return false;
        default:
            return true;
    }
}