#pragma once

#include <glm/glm.hpp>

#define PI 3.14159265359
#define DEGREES(radians) ((radians) * 180 / PI)
#define RADIANS(degrees) ((degrees) * PI / 180)
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct
{
    float x, y, z;
    float nx, ny, nz;
    float u, v;
    float occlusion;
    float lighting;
} Vertex;

typedef struct
{
    int x, y, z;
} Point3D;

glm::vec3 getSightVector(float rx, float ry);

bool isBlockCollision(int height, glm::vec3 pos, int hx, int hy, int hz);

glm::vec3 getMotionVector(bool flying, int sz, int sx, float rx, float ry);

glm::mat4 glOrtho(float left, float right, float bottom, float top, float near, float far);

float lerp(float value, float a, float b);
