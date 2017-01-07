#pragma once

#define MAX_CHUNKS 4092
#define MAX_DIST 256

#define FLY_SPEED 20
#define WALK_SPEED 3.5

#define NUM_OF_STEPS 4

std::vector<BlockType> blockTypes = getInteractiveTypes();
std::vector<Cloud> clouds;
std::vector<Particle> m_particles;

static GLFWwindow *m_window;
static Chunk m_chunks[MAX_CHUNKS];
static int m_chunk_count;
static Player* m_player;
static int m_width;
static int m_height;
static int m_flying;
static int m_item_index;
static int m_scale;
static float m_fov;
static bool m_local_running;

static int is_transparent, is_textured;
static bool no_particle_move, no_orientation;

static GLuint texture;
static GLuint cloud_buffer;
static GLuint dust_buffer;

static KeybindingSettings m_keys;
static GraphicSettings m_graphics;
static ChunkSettings m_chunkSetting;