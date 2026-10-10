#include "random.h"
#include "assets.h"
#include "libraries.h"
#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"

#include "spu_soundmodule_bin.h"

float lerp(int x, int y, float amount)
{
	return x + amount * (y - x);
}

int fps = 0;
static int frames = 0;
static u64 last_time = 0;

int main_bg = 0;
int bg_temp = 0;
int lang = 2;

bool can_click = true;

int player_x = 0;
int player_y = 0;
int player_speed = 1;

void UpdateFPS()
{
    u64 now = sysGetSystemTime();
    frames++;

    if(now - last_time >= 1000000ULL)
    {
        fps = frames;
        frames = 0;
        last_time = now;
    }
}

int RandRange(int min, int max)
{
	int rndom;
	rndom = rand() % (max-min+1) + min;
	return rndom;
}

int max(int min, int max)
{
	int biggest;
	biggest = (min > max) ? min : max;
	
	return biggest;
}

void PlayBGM(int id, int inf)
{
	FILE *fp = (FILE *)mem_open((char *)loop_bin, loop_bin_size);
    PlayAudiofd(fp, 0, AUDIO_INFINITE_TIME);
}

void Load_PNG()
{
	pngLoadFromBuffer(select_bin, select_bin_size, &spritesheet[0]);
	pngLoadFromBuffer(pre_intro_bin, pre_intro_bin_size, &spritesheet[1]);
	pngLoadFromBuffer(title_bin, title_bin_size, &spritesheet[2]);
	pngLoadFromBuffer(intro_bin, intro_bin_size, &spritesheet[3]);
	pngLoadFromBuffer(back_bin, back_bin_size, &spritesheet[4]);
	pngLoadFromBuffer(projectile_bin, projectile_bin_size, &spritesheet[5]);
}

void Initialize()
{
	printf("Initialization...\n");
	u32 spu;
	sysSpuImage spu_image;
	sysSpuInitialize(1, 5);
	sysSpuRawCreate(&spu, NULL);
	sysSpuImageImport(&spu_image, spu_soundmodule_bin, 0);
	sysSpuRawImageLoad(spu, &spu_image);
	SND_Init(spu);
}