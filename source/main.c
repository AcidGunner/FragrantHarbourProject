#include <tiny3d.h>

#include "soundlib/spu_soundlib.h"
#include "soundlib/audioplayer.h"

#include "drawing.h"
#include "random.h"

bool old_control_left = false;
bool old_control_right = false;
bool old_control_up = false;
bool old_control_down = false;
bool old_control_any = false;
bool old_control_shoot = false;

int frame = 0;

int main(int argc, const char* argv[])
{
	padInfo padinfo;
	int ip;
	static padData oldpad;
	padData paddata;
	
	srand(time(NULL));
	tiny3d_Init(6*1024*1024);
	tiny3d_UserViewport(1, 0, 0, (float)(Video_Resolution.width / 256.0f), (float)(Video_Resolution.height / 224.0f),
		(float)(Video_Resolution.width / 1920.0f), (float)(Video_Resolution.height / 1080.0f));
	
	sysModuleLoad(SYSMODULE_PNGDEC);
	
	Initialize();
	
	ioPadInit(7);
    LoadTexture();
    
    lang = 2;
    
	player_x = 256/2;
	player_y = 244/2;
	
	usleep(150000);
    
    PlayBGM();

    while(1)
    {
		sysUtilCheckCallback();
		UpdateFPS();
		
		UpdateFade(1.0f / 60.0f);
		UpdateEntities(1.0f / 60.0f);
		
        tiny3d_Clear(0xff000000, TINY3D_CLEAR_ALL);
		tiny3d_AlphaTest(1, 0x10, TINY3D_ALPHA_FUNC_GEQUAL);
		tiny3d_BlendFunc(1, TINY3D_BLEND_FUNC_SRC_RGB_SRC_ALPHA | TINY3D_BLEND_FUNC_SRC_ALPHA_SRC_ALPHA,
            TINY3D_BLEND_FUNC_DST_RGB_ONE_MINUS_SRC_ALPHA | TINY3D_BLEND_FUNC_DST_ALPHA_ZERO,
            TINY3D_BLEND_RGB_FUNC_ADD | TINY3D_BLEND_ALPHA_FUNC_ADD);
		
		ioPadGetInfo(&padinfo);
		for(ip = 0; ip < MAX_PADS; ip++)
		{
			if(padinfo.status[ip])
			{
				ioPadGetData(ip, &paddata);
				
				bool control_left = paddata.BTN_LEFT || paddata.ANA_L_H < 80;
				bool control_right = paddata.BTN_RIGHT || paddata.ANA_L_H > 176;

				bool control_up = paddata.BTN_UP || paddata.ANA_L_V < 80;
				bool control_down = paddata.BTN_DOWN || paddata.ANA_L_V > 176;
				bool control_any = paddata.BTN_CIRCLE || paddata.BTN_CROSS || paddata.BTN_TRIANGLE || paddata.BTN_SQUARE || paddata.BTN_START;
				bool control_shoot = paddata.BTN_CIRCLE || paddata.BTN_CROSS;
				
				switch(game_state)
				{
					case 0:
					{
						if(control_up && !old_control_up && can_click && lang<2) lang++;
						if(control_down && !old_control_down && can_click && lang>0) lang--;
						
						if(control_shoot && !old_control_shoot && can_click)
						{
							bg_temp = 0;
							FadeInOut(&game_state, 1);
						}
						break;
					}
					case 1:
					{
						if(control_any && !old_control_any && can_click)
						{
							if(bg_temp>=2) FadeInOut(&game_state, 2);
							else FadeInOut(&bg_temp, bg_temp+1);
						}
						break;
					}
					case 2:
					{
						if(control_any && !old_control_any && can_click)
						{
							bg_temp = 0;
							FadeInOut(&game_state, 3);
						}
						break;
					}
					case 3:
					{
						if(control_any && !old_control_any && can_click)
						{
							if(bg_temp>=3)
							{
								main_bg = RandRange(0, 5);
								FadeInOut(&game_state, 4);
							}
							else FadeInOut(&bg_temp, bg_temp+1);
						}
						break;
					}
					case 4:
					{
						if(control_up) player_y-=player_speed;
						if(control_down) player_y+=player_speed;
						
						if(control_left) player_x-=player_speed;
						if(control_right) player_x+=player_speed;
						
						if(control_left || control_right)
						{
							if(control_right) player_xscale = -1;
							else player_xscale = 1;
							
							frame++;
							int test = (float)frame/5;
							player_anim = 2+(test % 5);
						}
						else
						{
							player_xscale = 1;
							frame = 0;
							if(control_up) player_anim = 1; else player_anim = 0;
						}
						
						if(control_shoot && !old_control_shoot)
						{
							SpawnEntity(player_x, player_y, 0, -150, 0);
						}
						
						break;
					}
					default: break;
				}
				
				if(paddata.BTN_SELECT && !oldpad.BTN_SELECT) sysProcessExit(0);
				
				old_control_left = control_left;
				old_control_right = control_right;
				old_control_up = control_up;
				old_control_down = control_down;
				old_control_any = control_any;
				old_control_shoot = control_shoot;
				
				oldpad = paddata;
			}
		}
		
		drawScene();
		
		tiny3d_Flip();
    }

    return 0;
}