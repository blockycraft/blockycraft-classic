#include "settings.h"
#include "world.h"
#include "perlin.h"
#include "block.h"

#include <time.h>
#include <math.h>

static PerlinNoise noise(time(NULL));

void reseedWorld() {
    noise.reseed(time(NULL));
}

void createWorld(int wx, int wz, int chunk_size, float ratio, ChunkDataList* list) {
    int padding = 1;
    for (int dx = -padding; dx < chunk_size + padding; dx++) {
        for (int dz = -padding; dz < chunk_size + padding; dz++) {           
            // Get x and z
            int x = wx * chunk_size + dx;
            int z = wz * chunk_size + dz;

            // compute based on perlin noise object
            float f = noise.noise(x * ratio, z * ratio, 0);
            float g = noise.noise(-x * ratio, -z * ratio, 0);

            int computed_height = f * (g * 32 + 16);

            int wtype = BlockType::GRASS;

            int min_height = 10;
            if (computed_height < min_height) {
                computed_height = min_height;
            }

            for (int y = 0; y < computed_height; y++) {
                if (y > 25) {
                    wtype = BlockType::WHITE; // proper snow
                } else if (y > 18) {
                    wtype = BlockType::SNOW;
                } else if (y > 12) {
                    wtype = BlockType::GRASS;
                } else if (y > 6) {
                    wtype = BlockType::SAND;
                } else if (y > 4) {
                    wtype = BlockType::ROCK;
                } else {
                    wtype = BlockType::BEDROCK;
                }

                setChunkListItem(list, x, y, z, wtype);
            }
        }
    }
}
