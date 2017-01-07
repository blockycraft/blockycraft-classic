#pragma once

// Visibility settings
typedef struct {
    bool lights;
    bool items;
    bool crosshairs;
    bool clouds;
    bool audio;
    bool particles;

    int width;
    int height;
    float timeOfDay;
} GraphicSettings;

// Keybindings settings
typedef struct {
    int forward;
    int backward;
    int left;
    int right;

    int look_up;
    int look_down;
    int look_left;
    int look_right;

    int chunk_inc;
    int chunk_dec;

    int jump;
    int fly;
    int light_it;

    int item_next;
    int item_prev;

    int reset_world;
} KeybindingSettings;

// Chunk settings
typedef struct {
    int size;
    float ratio;

    int create_radius;
    int render_radius;
} ChunkSettings;