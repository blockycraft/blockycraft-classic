#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "chunklist.h"

int hash(int x) {
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x;
}

int hash3d(int x, int y, int z) {
    x = hash(x);
    y = hash(y);
    z = hash(z);
    return x ^ y ^ z;
}

void createChunkList(ChunkDataList *list, int dx, int dy, int dz, int length) {
    list->dx = dx;
    list->dy = dy;
    list->dz = dz;
    list->length = length;
    list->size = 0;
    list->entries.resize(length + 1);
}

ChunkDataListEntry* _find(unsigned int index, int x, int y, int z, ChunkDataList *list, int* match) {
    ChunkDataListEntry *entry = &list->entries[index];
    while (!LIST_EMPTY(entry)) {
        if (entry->x == x && entry->y == y && entry->z == z) {
            *match = 1;
            return entry;
        }
        index = (index + 1) & list->length;
        entry = &list->entries[index];
    }

    return entry;
}

int setChunkListItem(ChunkDataList *list, int px, int py, int pz, int w) {
    unsigned int index = hash3d(px, py, pz) & list->length;
    int x = px - list->dx, y = py - list->dy, z = pz - list->dz;
    int overwrite = 0;
    ChunkDataListEntry *entry = _find(index, x, y, z, list, &overwrite);
    
    if (overwrite) {
        if (entry->value != w) {
            entry->value = w;
            return 1;
        }
    }
    else if (w) {
        entry->x = x;
        entry->y = y;
        entry->z = z;
        entry->value = w;
        list->size++;
        return 1;
    }
    return 0;
}

int getChunkListItem(ChunkDataList *list, int px, int py, int pz) {
    unsigned int index = hash3d(px, py, pz) & list->length;
    int x = px - list->dx, y = py - list->dy, z = pz - list->dz;
    int overwrite = 0;
    ChunkDataListEntry *entry = _find(index, x, y, z, list, &overwrite);
    return entry->value;
}