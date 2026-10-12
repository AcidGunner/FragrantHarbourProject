#include "drawing.h"
#include "text.h"
#include "random.h"
#include "asimov_ttf_bin.h"

#define FADE_NONE 0
#define FADE_IN   1
#define FADE_OUT  2

static int fade_mode = FADE_NONE;
static float fade_alpha = 0.0f;

static int *fade_variable = NULL;
static int fade_value;

u32 c_white = 0xffffffff;
u32 c_black = 0x000000ff;
u32 c_green = 0x00ff00ff;
u32 c_red = 0xff0000ff;
u32 c_blue = 0x0000ffff;
u32 c_yellow = 0xffff00ff;
u32 c_pink = 0xff00ffff;
u32 c_cyan = 0x00ffffff;

int game_state = 0;

void DrawBackground2D(u32 rgba, float l)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(0, 0, l);
    tiny3d_VertexColor(rgba);
    tiny3d_VertexPos(1000, 0, l);
    tiny3d_VertexPos(1000, 1000, l);
    tiny3d_VertexPos(0, 1000, l);
    tiny3d_End();
}

void DrawRectangle2D(float x, float y, float w, float h, u32 rgba, float l)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, l);
    tiny3d_VertexColor(rgba);
    tiny3d_VertexPos(x+w, y, l);
    tiny3d_VertexPos(x+w, y+h, l);
    tiny3d_VertexPos(x, y+h, l);
    tiny3d_End();
}

void DrawSprite2D(int id, float x, float y, int wid2, int hei2, int wid1, int hei1, int wid0, int hei0, float layer, u32 color)
{
	int cols = wid0 / wid1;
    int col = id % cols;
    int row = id / cols;
    float u0 = (float)(col * wid1) / wid0;
    float v0 = (float)(row * hei1) / hei0;
    float u1 = (float)((col + 1) * wid1) / wid0;
    float v1 = (float)((row + 1) * hei1) / hei0;
	
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(x+wid2, y, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(x+wid2, y+hei2, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(x, y+hei2, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void DrawFull(float x, float y, int wid, int hei, float layer, u32 color)
{
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(0, 0);
    tiny3d_VertexPos(x+wid, y, layer);
    tiny3d_VertexTexture(0.99f, 0);
    tiny3d_VertexPos(x+wid, y+hei, layer);
    tiny3d_VertexTexture(0.99f, 0.99f);
    tiny3d_VertexPos(x, y+hei, layer);
    tiny3d_VertexTexture(0, 0.99f);

    tiny3d_End();
}

void DrawSpriteUI(float x, float y, int wid1, int hei1, int wid2, int hei2, int wid0, int hei0, int wid3, int hei3, float layer, u32 color)
{
    float u0 = (float)wid1 / (float)wid0;
    float v0 = (float)hei1 / (float)hei0;
    float u1 = (float)wid2 / (float)wid0;
    float v1 = (float)hei2 / (float)hei0;
	
    tiny3d_SetPolygon(TINY3D_QUADS);

    tiny3d_VertexPos(x, y, layer);
    tiny3d_VertexColor(color);
    tiny3d_VertexTexture(u0, v0);
    tiny3d_VertexPos(x+wid3, y, layer);
    tiny3d_VertexTexture(u1, v0);
    tiny3d_VertexPos(x+wid3, y+hei3, layer);
    tiny3d_VertexTexture(u1, v1);
    tiny3d_VertexPos(x, y+hei3, layer);
    tiny3d_VertexTexture(u0, v1);

    tiny3d_End();
}

void SetTexture(int id)
{
	tiny3d_SetTexture(0, sprite_off[id], spritesheet[id].width, spritesheet[id].height,
		spritesheet[id].pitch, TINY3D_TEX_FORMAT_A8R8G8B8, TEXTURE_NEAREST);
}

void DrawFade()
{
    if(fade_alpha <= 0.0f) return;

    u32 alpha = (u32)fade_alpha;
    u32 color = alpha;
    color = 0x00000000 | alpha;
	
	DrawBackground2D(color, -2);
}

void DrawEntities(int frames)
{
    int i;

    for(i = 0; i < MAX_ENTITIES; i++)
    {
        Entity *e = &entities[i];
        if(!e->active) continue;
		
		switch(e->type)
		{
			case 0:
			{
				SetTexture(7);
				DrawFull(e->x - 2, e->y - 3.5f, 4, 7, 0, c_white);
				break;
			}
			default: break;
		}
	}
}

void drawScene()
{
	static int frames = 0;
    tiny3d_Project2D();
    
    frames++;
	
	switch(game_state)
	{
		case 0:
		{
			SetTexture(0);
			DrawFull(0, 0, 256, 224, 1, c_white);
			SetTexture(5);
			DrawSprite2D((float)frames/5, 56, 138+((2-lang)*16), 13, 16, 13, 16, 65, 16, 0, c_white);
			break;
		}
		case 1:
		{
			SetTexture(1);
			DrawSprite2D((bg_temp*3)+lang, 0, 0, 256, 224, 256, 224, 256*3, 224*3, 1, c_white);
			break;
		}
		case 2:
		{
			SetTexture(2);
			DrawFull(0, 0, 256, 224, 1, c_white);
			break;
		}
		case 3:
		{
			SetTexture(3);
			DrawSprite2D((bg_temp*3)+lang, 0, 0, 256, 224, 256, 224, 256*3, 224*4, 1, c_white);
			break;
		}
		case 4:
		{
			SetTexture(4);
			DrawSprite2D(main_bg*4, 0, 0, 256, 224, 256, 224, 256*4, 224*6, 1, c_white);
			SetTexture(6);
			DrawSprite2D(player_anim, player_x-(16*player_xscale), player_y-20, 32*player_xscale, 40, 32, 40, 224, 40, -1, c_white);
			break;
		}
		default: break;
	}
	
	DrawEntities(frames);
	
	DrawFade();
}

u32 *texture_pointer = NULL;

void LoadTexture()
{
	int g;
	
    u32 * texture_mem = tiny3d_AllocTexture(64*1024*1024);
    if(!texture_mem) return;
    texture_pointer = texture_mem;
	
    Load_PNG();
    
    for(g = 0; g < TOTAL_SHEETS; g++)
	{
		memcpy(texture_pointer, spritesheet[g].bmp_out, spritesheet[g].pitch * spritesheet[g].height);
		sprite_off[g] = tiny3d_TextureOffset(texture_pointer);
		free(spritesheet[g].bmp_out);
		spritesheet[g].bmp_out = NULL;
		
		texture_pointer += ((spritesheet[g].pitch * spritesheet[g].height + 15) & ~15) / 4;
    }
}

void FadeInOut(int *variable, int value)
{
    fade_variable = variable;
    fade_value = value;
	
	if(*fade_variable == value) return;

    fade_alpha = 0.0f;
    fade_mode = FADE_OUT;
}

void UpdateFade(float dt)
{
    if(fade_mode == FADE_OUT)
    {
    	can_click = false;
        fade_alpha += 255.0f * dt / 0.5f;

        if(fade_alpha >= 255.0f)
        {
            fade_alpha = 255.0f;
            fade_mode = FADE_IN;

            if(fade_variable)
            {
                *fade_variable = fade_value;
                fade_variable = NULL;
            }
        }
    }
    else if(fade_mode == FADE_IN)
    {
        fade_alpha -= 255.0f * dt / 0.5f;

        if(fade_alpha <= 0.0f)
        {
            fade_alpha = 0.0f;
            fade_mode = FADE_NONE;
	    	can_click = true;
        }
    }
}