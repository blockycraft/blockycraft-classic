#include "mathhelper.h"

#include <math.h>

glm::vec3 getSightVector(float rx, float ry) {
    float m = cosf(ry);

    return glm::vec3(cosf(rx - RADIANS(90)) * m, sinf(ry), sinf(rx - RADIANS(90)) * m);
}

bool isBlockCollision(int height, glm::vec3 pos, int hx, int hy, int hz) {
    int nx = roundf(pos.x); int ny = roundf(pos.y); int nz = roundf(pos.z);
    for (int i = 0; i < height; i++) {
        if (nx == hx && nz == hz && ny - i == hy) {
            return true;
        }
    }
    return false;
}

glm::vec3 getMotionVector(bool flying, int sz, int sx, float rx, float ry) {
    glm::vec3 vector;

    if (!sz && !sx) {
        return vector;
    }

    float strafe = atan2f(sz, sx);
    if (flying) {
        float m = cosf(ry);
        float y = sinf(ry);
        if (sx) {
            if (!sz) {
                y = 0;
            }
            m = 1;
        }
        if (sz > 0) {
            y = -y;
        }        
        vector = glm::vec3(cosf(rx + strafe) * m, y, sinf(rx + strafe) * m);
    }
    else {
        vector = glm::vec3(cosf(rx + strafe), 0, sinf(rx + strafe));
    }

    return vector;
}

glm::mat4 glOrtho(float left, float right, float bottom, float top, float near, float far)
{ 
    glm::mat4 matrix;
    matrix[0][0] = 2 / (right - left); 
    matrix[0][1] = 0; 
    matrix[0][2] = 0; 
    matrix[0][3] = 0; 
 
    matrix[1][0] = 0; 
    matrix[1][1] = 2 / (top - bottom); 
    matrix[1][2] = 0; 
    matrix[1][3] = 0; 
 
    matrix[2][0] = 0; 
    matrix[2][1] = 0; 
    matrix[2][2] = -2 / (far - near); 
    matrix[2][3] = 0; 
 
    matrix[3][0] = -(right + left) / (right - left); 
    matrix[3][1] = -(top + bottom) / (top - bottom); 
    matrix[3][2] = -(far + near) / (far - near); 
    matrix[3][3] = 1; 
    
    return matrix;
}

float lerp(float value, float a, float b) {
    return a + value * (b - a);
}