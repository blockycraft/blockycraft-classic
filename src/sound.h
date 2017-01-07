#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

#include "globals.h"
#include "hit.h"

#define SONG_OVERWORLD 1
#define SONG_UNDERWORLD 2
#define GET_KEY(value) (glfwGetKey(m_window, value))


static Mix_Music *m_overworld;
static Mix_Music *m_underworld;
static double m_last_step_play;
static int current_song = -1;

static Mix_Chunk* m_walk_grass[NUM_OF_STEPS];
static Mix_Chunk* m_walk_dirt[NUM_OF_STEPS];
static Mix_Chunk* m_walk_sand[NUM_OF_STEPS];
static Mix_Chunk* m_walk_stone[NUM_OF_STEPS];
static Mix_Chunk* m_walk_wood[NUM_OF_STEPS];
static Mix_Chunk* m_walk_snow[NUM_OF_STEPS];
static std::default_random_engine engine(time(NULL));
static std::uniform_int_distribution<int> integer_dist(0, NUM_OF_STEPS - 1);
static std::uniform_real_distribution<float> float_dist(-1.0, 1.0f);
static int m_prev_step_sound = 0;

void initAudio() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        printf("Failed to init SDL\n");
        exit(1);
    }

    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 640) == -1) {
        printf("Failed to load Music. SDL_Mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    m_underworld = Mix_LoadMUS("../assets/underworld.wav");
    m_overworld = Mix_LoadMUS("../assets/overworld.wav");
    if (m_overworld == NULL || m_underworld == NULL) {
        printf("Failed to load Music. SDL_Mixer Error: %s\n", Mix_GetError());
        exit(1);
    }  

    for (int i = 0; i < NUM_OF_STEPS; i++) {
        m_walk_grass[i] = Mix_LoadWAV(("../assets/grass" + std::to_string(i + 1) + ".wav").c_str());
        m_walk_dirt[i] = Mix_LoadWAV(("../assets/gravel" + std::to_string(i + 1) + ".wav").c_str());
        m_walk_sand[i] = Mix_LoadWAV(("../assets/sand" + std::to_string(i + 1) + ".wav").c_str());
        m_walk_stone[i] = Mix_LoadWAV(("../assets/stone" + std::to_string(i + 1) + ".wav").c_str());
        m_walk_wood[i] = Mix_LoadWAV(("../assets/wood" + std::to_string(i + 1) + ".wav").c_str());
        m_walk_snow[i] = Mix_LoadWAV(("../assets/snow" + std::to_string(i + 1) + ".wav").c_str());
    }
}

void playBackgroundMusic(int song) {
    if (current_song != song) {
        current_song = song;
        Mix_HaltMusic();
        
        Mix_Music* music;
        if (song == SONG_OVERWORLD) {
            music = m_overworld;
        } else if (song == SONG_UNDERWORLD) {
            music = m_underworld;
        } else {
            music = m_overworld;
        }

        Mix_PlayMusic(music, 1);        
    }  
}

void handleAudio(double now, double dt) {
    Player* player = m_player;
    glm::vec3 pos(player->x, player->y, player->z);
    Point3D hit = { 0, 0, 0 };

    bool adplay = false;    
    if (m_last_step_play <= 0) {
        m_last_step_play = 0.35;
        adplay = true;
    }
    m_last_step_play -= dt;

    bool moved = GET_KEY(m_keys.forward) 
                || GET_KEY(m_keys.backward) 
                || GET_KEY(m_keys.left)
                || GET_KEY(m_keys.right);
    

    // first straight down
    BlockType type = getBlockHitVector(0, pos, 0.0f, -PI / 2, hit);
    if (hit.y > 0 && hit.y < 256 && isObstacle(type) && !m_flying && adplay && moved) {

        int current = 0;
        do {
            current = integer_dist(engine);
        } while (current == m_prev_step_sound);
        m_prev_step_sound = current;
        
        switch (type) {
            case GRASS: 
                Mix_PlayChannel (-1, m_walk_grass[current], 0);
                break;
            case SAND: 
                Mix_PlayChannel (-1, m_walk_sand[current], 0);
                break;
            case SNOW: 
                Mix_PlayChannel (-1, m_walk_snow[current], 0);
                break;
            case CEMENT:
            case BRICK:
            case STONE: 
                Mix_PlayChannel (-1, m_walk_stone[current], 0);
                break;
            case WOOD: 
                Mix_PlayChannel (-1, m_walk_wood[current], 0);
                break;
            case DIRT: 
                Mix_PlayChannel (-1, m_walk_dirt[current], 0);
                break;
            default:
                Mix_PlayChannel (-1, m_walk_dirt[current], 0);
                break;
        }
    }

    // straight up
    type = getBlockHitVector(0, pos, 0.0f, PI / 2, hit);
    if (type != EMPTY) {
        playBackgroundMusic(SONG_UNDERWORLD);
    } else {
        playBackgroundMusic(SONG_OVERWORLD);
    }
}
