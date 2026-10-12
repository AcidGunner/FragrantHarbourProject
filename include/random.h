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

extern bool can_click;

const char *bgm;
void PlayBGM();

int RandRange(int min, int max);
int max(int min, int max);

void Load_PNG();

void UpdateFPS();
extern int fps;
void Initialize();

float lerp(int x, int y, float amount);

#endif