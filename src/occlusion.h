#pragma once

#define NEIGHBOURS 27
#define CUBE_CELL 6
#define CELL_VERTICES 4
#define LIGHT_EMITTING_VALUE 600
#define OCC_LIGHTING_FACTOR 15
#define CENTER_OF_NEIGHBOURS 13


#define OCC_RANGE 3
#define OCC_PLANE 9
#define OCC_TRIANGLE_XTL(corner) {corner, corner+1, corner+OCC_RANGE}
#define OCC_TRIANGLE_XTR(corner) {corner, corner-1, corner+OCC_RANGE}
#define OCC_TRIANGLE_XBL(corner) {corner, corner-OCC_RANGE, corner+1}
#define OCC_TRIANGLE_XBR(corner) {corner, corner-OCC_RANGE, corner-1}

#define OCC_TRIANGLE_YTL(corner) {corner, corner+1, corner+OCC_PLANE}
#define OCC_TRIANGLE_YTR(corner) {corner, corner-1, corner+OCC_PLANE}
#define OCC_TRIANGLE_YBL(corner) {corner, corner-OCC_PLANE, corner+1}
#define OCC_TRIANGLE_YBR(corner) {corner, corner-OCC_PLANE, corner-1}

#define OCC_TRIANGLE_ZTL(corner) {corner, corner+OCC_RANGE, corner+OCC_PLANE}
#define OCC_TRIANGLE_ZTR(corner) {corner, corner-OCC_RANGE, corner+OCC_PLANE}
#define OCC_TRIANGLE_ZBL(corner) {corner, corner-OCC_PLANE, corner+OCC_RANGE}
#define OCC_TRIANGLE_ZBR(corner) {corner, corner-OCC_PLANE, corner-OCC_RANGE}

static const int neighbour_lookup_table[6][4][3] = {
    {
        OCC_TRIANGLE_XTL(0), OCC_TRIANGLE_XTR(2), OCC_TRIANGLE_XBL(6), OCC_TRIANGLE_XBR(8)
    },
    {
        OCC_TRIANGLE_XTL(18), OCC_TRIANGLE_XTR(20), OCC_TRIANGLE_XBL(24), OCC_TRIANGLE_XBR(26)
    },
    {
        OCC_TRIANGLE_YTL(6), OCC_TRIANGLE_YTR(8), OCC_TRIANGLE_YBL(24), OCC_TRIANGLE_YBR(26)
    },
    {
        OCC_TRIANGLE_YTL(6), OCC_TRIANGLE_YTR(2), OCC_TRIANGLE_YBL(18), OCC_TRIANGLE_YBR(20)
    },
    {
        OCC_TRIANGLE_YTL(0), OCC_TRIANGLE_YTR(6), OCC_TRIANGLE_YBL(18), OCC_TRIANGLE_YBR(24)
    },
    {
        OCC_TRIANGLE_YTL(2), OCC_TRIANGLE_YTR(8), OCC_TRIANGLE_YBL(20), OCC_TRIANGLE_YBR(26)        
    }
};

#define OCC_LINEAR(base) {base-2, base-1, base+1, base+2}
#define OCC_STEP_PAIR(c1, c2) {c1, c2, c1+OCC_PLANE, c2+OCC_PLANE}

static const int lighting_lookup_table[6][4][4] = {
    {
        OCC_LINEAR(2), OCC_LINEAR(3), OCC_LINEAR(5), OCC_LINEAR(6)
    },
    {
        OCC_LINEAR(20), OCC_LINEAR(21), OCC_LINEAR(23), OCC_LINEAR(24)
    },
    {
        OCC_STEP_PAIR(6,7), OCC_STEP_PAIR(7,8), OCC_STEP_PAIR(15,16), OCC_STEP_PAIR(16,17),
    },
    {
        OCC_STEP_PAIR(0, 1), OCC_STEP_PAIR(1, 2), OCC_STEP_PAIR(9, 10), OCC_STEP_PAIR(10, 11),
    },
    {
        OCC_STEP_PAIR(0, 3), OCC_STEP_PAIR(3, 6), OCC_STEP_PAIR(9, 12), OCC_STEP_PAIR(12, 15),
    },
    {
        OCC_STEP_PAIR(2, 5), OCC_STEP_PAIR(5, 8), OCC_STEP_PAIR(11, 14), OCC_STEP_PAIR(14, 17),
    }
};

static float curveFactor(int value) {
    return value * 0.25;
}

static int lookup_factor(bool neighbors[NEIGHBOURS], int i, int j) {
    bool corner = neighbors[neighbour_lookup_table[i][j][0]];
    bool side1 = neighbors[neighbour_lookup_table[i][j][1]];
    bool side2 = neighbors[neighbour_lookup_table[i][j][2]];

    return (side1 != 0 && side2 != 0) ? 3 : corner + side1 + side2;  
}

static void occlusion(
    bool neighbors[NEIGHBOURS], 
    char lights[NEIGHBOURS], 
    float shades[NEIGHBOURS], 
    glm::vec4 occlusn[6], 
    glm::vec4 light[6]
) {
    for (int i = 0; i < CUBE_CELL; i++) {
        for (int j = 0; j < CELL_VERTICES; j++) {
            int value = lookup_factor(neighbors, i, j);
            
            float shade_sum = 0;
            float light_sum = 0;
            int is_light = (lights[CENTER_OF_NEIGHBOURS] == OCC_LIGHTING_FACTOR);
            for (int k = 0; k < CELL_VERTICES; k++) {
                shade_sum += shades[lighting_lookup_table[i][j][k]];
                light_sum += lights[lighting_lookup_table[i][j][k]];
            }

            if (is_light) {
                light_sum = LIGHT_EMITTING_VALUE;
            }

            float total = curveFactor(value) + shade_sum / 4.0;
            occlusn[i][j] = MIN(total, 1.0);
            light[i][j] = light_sum / 60.0;
        }
    }
}
