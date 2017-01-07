#pragma once

#include <vector>

#define INDEX3D(x, y, z) ((y) * ARRAY_WIDTH * ARRAY_WIDTH + (x) * ARRAY_WIDTH + (z))
#define INDEX2D(x, z) ((x) * ARRAY_WIDTH + (z))
#define INDEX3DP(v) ((v.y) * ARRAY_WIDTH * ARRAY_WIDTH + (v.x) * ARRAY_WIDTH + (v.z))
#define EMPTY_ENTRY(entry) ((entry).x == 0 && (entry).y == 0 && (entry).z == 0 && (entry).value == 0)
#define LIST_EMPTY(entry) ((entry)->x == 0 && (entry)->y == 0 && (entry)->z == 0 && (entry)->value == 0)

typedef struct {
    int x;
    int y;
    int z;
    int value;
} ChunkDataListEntry;

typedef struct {
    int dx;
    int dy;
    int dz;
    unsigned int length;
    unsigned int size;
    std::vector<ChunkDataListEntry> entries;
} ChunkDataList;

void createChunkList(ChunkDataList *list, int dx, int dy, int dz, int length);
int setChunkListItem(ChunkDataList *list, int x, int y, int z, int w);
int getChunkListItem(ChunkDataList *list, int x, int y, int z);