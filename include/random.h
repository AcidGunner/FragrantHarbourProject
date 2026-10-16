#ifndef RANDOM_H
#define RANDOM_H

#include <pngdec/pngdec.h>

#define TOTAL_SHEETS 10
pngData spritesheet[TOTAL_SHEETS];
u32 sprite_off[TOTAL_SHEETS];

extern int bg_temp;
extern int main_bg;
extern int lang;

extern int player_x;
extern int player_y;
extern int player_speed;
extern int player_anim;
extern int player_xscale;
extern int score;

extern bool can_click;
extern bool show_fps;

const char *bgm;
void PlayBGM();

int RandRange(int min, int max);
int max(int min, int max);

void Load_PNG();

void UpdateFPS();
extern int fps;
void Initialize();

float lerp(int x, int y, float amount);

typedef struct
{
    bool active;
    int type;

    float x;
    float y;

    float vx;
    float vy;
} Entity;

#define MAX_ENTITIES 256

#define E_PLAYER_PROJECTILE 0

Entity entities[MAX_ENTITIES];

void UpdateEntities(float dt);
void SpawnEntity(float x, float y, float vx, float vy, int type);

#endif