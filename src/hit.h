#pragma once

#include "globals.h"
#include "graphics.h"
#include "chunks.h"

BlockType getBlockHitVector(int previous, glm::vec3 pos, float rx, float ry, Point3D& hit) {
    BlockType type = EMPTY;
    float best = 0;
    int p = getRelativeChunk(pos.x, m_chunkSetting.size);
    int q = getRelativeChunk(pos.z, m_chunkSetting.size);

    glm::vec3 sight = getSightVector(rx, ry);
    for (int i = 0; i < m_chunk_count; i++) {
        Chunk *chunk = m_chunks + i;
        if (getDistanceFromChunk(chunk, p, q) > 1) {
            continue;
        }

        float x = pos.x, y = pos.y, z = pos.z;
        int hx, hy, hz;
        int px = 0, py = 0, pz = 0;
        int result = 0;
        for (int i = 0; i < MAX_DIST; i++) {
            int nx = roundf(x);
            int ny = roundf(y);
            int nz = roundf(z);
            if (nx != px || ny != py || nz != pz) {
                int btype = getChunkListItem(&chunk->list, nx, ny, nz);
                if (btype > 0) {
                    if (previous) {
                        hx = px;
                        hy = py;
                        hz = pz;
                    }
                    else {
                        hx = nx;
                        hy = ny;
                        hz = nz;
                    }
                    result = btype;
                    break;
                }
                px = nx;
                py = ny;
                pz = nz;
            }
            x += sight.x / 32;
            y += sight.y / 32;
            z += sight.z / 32;
        } 

        if (result > 0) {
            float dist = sqrtf(powf(hx - pos.x, 2) + powf(hy - pos.y, 2) + powf(hz - pos.z, 2));
            if (best == 0 || dist < best) {
                best = dist;
                hit.x = hx;
                hit.y = hy;
                hit.z = hz;
                type = (BlockType)result;
            }
        }
    }
    return type;
}
