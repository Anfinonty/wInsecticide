//wInsecticide
//Jan-25-2023 Flicker Fix (be on fullscreen)
//Jan-26-2023 Full Flicker Fix n added GR
//Dec-09-2023 Added GrSprite
//Dec-10-2023 Added Sprite Flipping
//Dec-14-2023 Added Sprite Win32 Compatibility && fixed sluggishness for win32
//Dec-21-2023 Sleep() for while loop, Fixed memleak caused by flipping sprite
//Jan-06-2023 Added FPS Sleep. Code Credit: geissomatik
//I've lost track of the logs XD'

//Command
//i686-w64-mingw32-gcc-win32 run.c -o "WINSECTICIDE.EXE" my.o -lgdi32 -lgdiplus -lmsimg32 -municode -lwinmm -lshlwapi 
//-lopengl32 -lglu32 is not used for now Jan-06-2024 -credit: sothea.dev

//#include <winsock2.h> //for multiplayer
#include <windows.h>
//#include <gdiplus.h> //for gif
//#include <wingdi.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <dirent.h>
#include <direct.h>
#include <errno.h>
#include <shlwapi.h>
#include <float.h>
//#include <mmsystem.h>
//#include <commctrl.h>
//#include <omp.h>

//for .avi
#include <vfw.h>

//for music
//#include <mmsystem.h>
//#include <dsound.h>

//#define KHMER_CHARSET   0x6B //107


//for complex scripts
//#include <usp10.h>
//#include <tchar.h>

//#pragma comment(lib, "usp10.lib") // This line ensures the linker includes usp10.lib

//https://learn.microsoft.com/en-us/windows/win32/api/usp10/nf-usp10-scriptlayout?source=recommendations
//https://learn.microsoft.com/en-us/windows/win32/intl/about-complex-scripts?redirectedfrom=MSDN
//https://learn.microsoft.com/en-us/windows/win32/intl/complex-script-processing
//https://learn.microsoft.com/en-us/windows/win32/gdi/complex-scripts
//https://learn.microsoft.com/en-us/globalization/fonts-layout/font-support
//https://learn.microsoft.com/en-us/windows/win32/intl/complex-script-processing

//#include <hidsdi.h>
//#include <hidusage.h>


//#include <clocale>
//#include <curses.h>
//#include <audioclient.h>
//#include <locale.h>
//#include <libsndfile.h> // Make sure to install libsndfile and link it during compilation
//#include <wiavideo.h>
//#include <GL/glu.h>
//#include "<resources.h>"


#define SCREEN_WIDTH    (GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT   (GetSystemMetrics(SM_CYSCREEN))


//Global Variables Game state variables
//flags
bool flag_restart=FALSE;
bool flag_restart_audio=FALSE;
bool flag_adjust_audio=FALSE;
bool flag_adjust_wav_out_audio=FALSE;
bool flag_load_level=FALSE;
bool flag_load_melevel=FALSE;
bool load_sound=FALSE;
bool back_to_menu=TRUE;
bool run_after_once=FALSE;
bool run_once_only=FALSE;
bool flag_update_background=FALSE;
bool flag_resolution_change=FALSE;
bool flag_fullscreen=FALSE;
bool flag_hide_taskbar=FALSE;
bool flag_taskbar_change_act=FALSE;
bool flag_borderless_resolution_change=FALSE;
bool flag_difficulty_change=TRUE;
bool flag_begin_drawing_tiles=FALSE;
bool flag_display_long_loading=FALSE;
bool hide_cursor=FALSE;
bool hide_mm=FALSE;
bool flag_load_player_sprite=TRUE;

//game options
bool yes_unifont=TRUE;//FALSE;
bool game_cam_shake=TRUE;//FALSE;
bool game_audio=TRUE;
bool game_shadow=FALSE;
bool free_will=FALSE;
bool hide_taskbar=FALSE;
bool show_hijiri=FALSE;
bool game_hard=FALSE;

//game state
bool blank_level=FALSE;
bool in_main_menu=TRUE;
bool level_loaded=FALSE;
bool level_loading=FALSE;
bool game_over=FALSE;
bool show_fps=FALSE;
bool in_map_editor=FALSE;
bool prelude=TRUE;
//bool flag_prelude=TRUE;
//bool alloc_enemy_once=TRUE;

//to be used to load a level
wchar_t save_level[128];
//lvl name type in or editing


//Create Level Menu
//Typing level name attributes
wchar_t typing_lvl_name[16];//=//{' ',L'H',L'I'};
int wav_mode=0;
int typing_lvl_name_pos=0;
int set_ground_amount=10;
int set_enemy_amount=1;
int set_map_width_amount=640;
int set_map_height_amount=480;
int create_lvl_option_choose=0;
//int once_timer=0;




//Game System Values

int windowx=0;
int windowy=0;

int GR_WIDTH=640;
int GR_HEIGHT=480;
int OLD_GR_WIDTH=640;
int OLD_GR_HEIGHT=480;
int MAX_RESOLUTION_I=1;

//bool rst_mbutton=FALSE;
int mouse_wheel_timer=0;
bool rst_shift=FALSE;
//HBITMAP myTmpEnemyDrawSprite;


#define SCREEN_RESOLUTION_NUM   127
int RESOLUTION_X[SCREEN_RESOLUTION_NUM];
int RESOLUTION_Y[SCREEN_RESOLUTION_NUM];
char* RESOLUTION_NAME[SCREEN_RESOLUTION_NUM];
wchar_t* WRESOLUTION_NAME[SCREEN_RESOLUTION_NUM];

int level_chosen=0;
int main_menu_chosen=-1; //options for main menu
int select_main_menu=0;
int option_choose=0;
int resolution_choose=0; //640,480, 800,600, SCREEN_WIDTH,SCREEN_HEIGHT

int call_help_timer=0;

double loading_numerator=0;
double loading_denominator=1;
double loading_percentage=0;

bool lvl_has_song=FALSE;
wchar_t src_music_dir[64];
//wchar_t src_music_dir2[64];


int enemy_kills=0;
int int_best_score=0; //to store to write
int frame_tick=-10;
//int FPS = 60;
int FPS = 35; //minimum FPS, otherwise run according to screen refresh rate

//Player visuals Options Values
int player_color=0;
int old_player_color=-1;
int player_load_color=0;

int player_iris_color=4;
int old_player_iris_color=-1;
int player_load_iris_color=4;

int player_pupil_color=12;
int old_player_pupil_color=-1;
int player_load_pupil_color=12;

int player_bullet_color=0;

int showoff=0;
int saved_showoff=0;

//double, game state system values
long long game_timer=0;
double double_best_score=0;



//double, game options
double time_begin=0;
double game_volume=0.2;
double old_game_volume=1.2;



bool is_khmer=TRUE;

#define DEFAULT_PLAYER_SPEED			4//1

#define ENEMY_TYPE_NUM                         10


#define PLAYER_WIDTH 	32
#define PLAYER_HEIGHT 	32

#define GRID_SIZE	160
#define VGRID_SIZE	160
#define NODE_SIZE  	 10
#define MAX_GROUNDS_WITHIN_GRID	(VGRID_SIZE/NODE_SIZE)*(VGRID_SIZE/NODE_SIZE)/2 //128 grounds

//#define ENEMY_I64_ATTRIBUTES_NUM 20
//#define ENEMY_F64_ATTRIBUTES_NUM 4
//#define ENEMY_BOOL_ATTRIBUTES_NUM 1

#define GROUND_F64_ATTRIBUTES_NUM 6
#define GROUND_BOOL_ATTRIBUTES_NUM 1
#define GROUND_I64_ATTRIBUTES_NUM 2
//#define GROUND_U8_ATTRIBUTES_NUM 1


#define MAX_FOLLOW_RANGE 100
#define MAX_NODE_NUM	 MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE

//#define RENDER_DIST      12//10//20
#define RENDER_WIDTH_MAX    104//13   -->160 *    16384/160
#define RENDER_HEIGHT_MAX   54//8    -->160 *   8640/160
#define RDGRID_NUM       RENDER_WIDTH_MAX*RENDER_HEIGHT_MAX


#define RAIN_NUM    80//20//50
#define SHOOT_BULLET_NUM    25000//100000// More bullets, otherwise memleak, idk why haha 2024-12-21 //5000
#define BULLET_NUM	SHOOT_BULLET_NUM+RAIN_NUM

#define ENEMY_BULLET_NUM            1000
#define MAX_BULLET_PER_FIRE         10

#define MAX_EXP_NUM_NORMAL                 10
#define MAX_EXP_NUM_HARD                   5


#define MAX_MAP_NODE_NUM (640*20)/NODE_SIZE * (480*20)/NODE_SIZE //MAX_WIDTH/NODE_SIZE * MAX_HEIGHT/NODE_SIZE
#define MAX_VGRID_NUM   4800 //(640/160)*20 * (480/160)*20
//#define MAX_GRID_NUM    4800
#define MAX_GROUND_NUM  2000//10000
#define MAX_ENEMY_NUM   200
//500 works but interferes with max bullet num
//200
//50



#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			35//30

#define DEFAULT_PLAYER_HEALTH			20
#define DEFAULT_PLAYER_BLOCK_HEALTH_MAX 50//20//100//20
#define DEFAULT_PLAYER_JUMP_HEIGHT 		150//85//100
#define DEFAULT_PLAYER_ATTACK_STRENGTH  	1
#define DEFAULT_PLAYER_KNOCKBACK_STRENGTH	50

#define DEFAULT_PLAYER_BUILD_RANGE		100//12
#define MAX_WEB_LENGTH		200//100//12
//#define DEFAULT_PLAYER_SHORT_BUILD_RANGE	10
#define DEFAULT_PLAYER_WEB_HEALTH		1500

#define DEFAULT_PLAYER_WEB_NUM      20//20
#define MAX_WEB_NUM     200//100

#define DEFAULT_PLAYER_TIME_BREAKER_MAX	20 //10 seconds to charge
#define DEFAULT_PLAYER_TIME_BREAKER_COOLDOWN_MAX   700 //5 seconds after usage
#define DEFAULT_PLAYER_TIME_BREAKER_RECHARGE_MAX	200 //1 seconds
#define DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX	22 //45

#define HP_SHOW_TIMER_NUM   450

#define PLAYER_LOW_HEALTH   3
#define PLAYER_BULLET_NUM 16//36//24//16
#define PLAYER_FLING_WEB_NUM    64//32

#define GAME_OPTIONS_NUM    16
#define PLAYER_BLUR_NUM     2

#include "gr.c"
#include "math.c"
#include "sound.c"


#include "struct_classes.c"



#include "load_save.c"

#include "grid.c"
#include "ground.c"
#include "bullet.c"
#include "player.c"
#include "enemy.c"

#include "song.c"
#include "draw_it.c"
#include "draw_gui.c"

#include "load_level.c"
#include "cleanup.c"


#include "map_editor.c"
#include "draw_it_map_editor.c"

#include "save_level.c"


#include "keybinds.c"
#include "keybinds_controller.c"
#include "keybinds_map_editor.c"

//Init
LARGE_INTEGER m_high_perf_timer_freq;
LARGE_INTEGER m_prev_end_of_frame;  
void InitTickFrequency() {
  if (!QueryPerformanceFrequency(&m_high_perf_timer_freq))
      m_high_perf_timer_freq.QuadPart = 0;
  m_prev_end_of_frame.QuadPart = 0;
}



void InitFPS() { //https://cboard.cprogramming.com/windows-programming/30730-finding-monitor-refresh-rate.html
  int index=0, currentfps=0;
  DEVMODE screen; 
  memset(&screen, 0, sizeof(DEVMODE));
  while(EnumDisplaySettings(NULL, index++, &screen)){
    //printf("The current refresh rate is %i\n", screen.dmDisplayFrequency); //debug
    currentfps=(int)screen.dmDisplayFrequency;
    //MessageBox(NULL, message, "Refresh Rate:", MB_OK);
    memset(&screen, 0, sizeof(DEVMODE));
    if (currentfps>FPS) FPS=currentfps;
  }
}


void FrameRateSleep(int max_fps)
{//http://www.geisswerks.com/ryan/FAQS/timing.html https://github.com/geissomatik
    LARGE_INTEGER t;
    QueryPerformanceCounter(&t);

    if (m_prev_end_of_frame.QuadPart != 0) 
    {

      int ticks_to_wait = (int) (m_high_perf_timer_freq.QuadPart / max_fps);
      bool done = FALSE;
      do
      {
        QueryPerformanceCounter(&t);
        
        int ticks_passed = (int)(t.QuadPart - m_prev_end_of_frame.QuadPart);
        int ticks_left = ticks_to_wait - ticks_passed;

        if (t.QuadPart < m_prev_end_of_frame.QuadPart)    // time wrap
          done = TRUE;
        if (ticks_passed >= ticks_to_wait)
          done = TRUE;
       
        if (!done)
        {
            // if > 0.002s left, do Sleep(1), which will actually sleep some 
            //   steady amount, probably 1-2 ms,
            //   and do so in a nice way (cpu meter drops; laptop battery spared).
            // otherwise, do a few Sleep(0)'s, which just give up the timeslice,
            //   but don't really save cpu or battery, but do pass a tiny
            //   amount of time.
          if (ticks_left > (int) (m_high_perf_timer_freq.QuadPart*2/1000))
            Sleep(1);
          else                        
            for (int i=0; i<10; i++) 
              Sleep(0);  // causes thread to give up its timeslice
        }
      } while (!done);            
    }
    m_prev_end_of_frame = t;
}


int prelude_sprite_jid=0;
int prelude_sprite_id=0;

void Prelude()
{
  //Load Enemy Rotated Sprite
  //note: tmp_sprite1, may cause issues with memory
  double angle_rn;
  int j=prelude_sprite_jid;
  int i=prelude_sprite_id;
  //for (int j=0;j<4;j++) {
    //for (int i=0;i<ROTATED_SPRITE_NUM;i++) {
  if (i==0 && j==0) {
    InitPlayerSpritesAll();
  }
  if (j<4) {
    angle_rn=M_PI_2-M_PI_16*i;
    switch (j) {
      case 0: //cockroach
        LoadEnemyRotatedSprite[j].prelude_tmp_sprite1[i]=
          GetRotated8BitBitmap(enemy2_sprite_1,angle_rn,LTGREEN);
        loading_numerator++;

        LoadEnemyRotatedSprite[j].prelude_tmp_sprite2[i]=
          GetRotated8BitBitmap(enemy2_sprite_2,angle_rn,LTGREEN);
        loading_numerator++;
        break;
      case 1: //toe biter part 1
        LoadEnemyRotatedSprite[j].prelude_tmp_sprite1[i]=
          GetRotated8BitBitmap(enemy4_sprite_1,angle_rn,LTGREEN);
         loading_numerator++;

        LoadEnemyRotatedSprite[j].prelude_tmp_sprite2[i]=
          GetRotated8BitBitmap(enemy4_sprite_2,angle_rn,LTGREEN);
        loading_numerator++;
        break;
      case 2: //ants
        LoadEnemyRotatedSprite[j].prelude_tmp_sprite1[i]=
          GetRotated8BitBitmap(enemy6_sprite_1,angle_rn,LTGREEN);
        loading_numerator++;

        LoadEnemyRotatedSprite[j].prelude_tmp_sprite2[i]=
          GetRotated8BitBitmap(enemy6_sprite_2,angle_rn,LTGREEN);
        loading_numerator++;
        break;
      case 3: //toe biter part 2
        XLoadEnemyRotatedSprite[0].prelude_tmp_sprite[i]=
          GetRotated8BitBitmap(enemy4_sprite_1_0,angle_rn,LTGREEN);
        loading_numerator++;
        break;
    }


    if (j<3) {
      ReplaceBitmapColor(LoadEnemyRotatedSprite[j].prelude_tmp_sprite1[i],LTBLUE,LTGREEN);
      ReplaceBitmapColor(LoadEnemyRotatedSprite[j].prelude_tmp_sprite2[i],LTBLUE,LTGREEN);


      ReplaceBitmapColor2(LoadEnemyRotatedSprite[j].prelude_tmp_sprite1[i],LTGREEN,BLACK,8,LTGREEN); //8 due to pureblack reserved for mask
      ReplaceBitmapColor2(LoadEnemyRotatedSprite[j].prelude_tmp_sprite2[i],LTGREEN,BLACK,8,LTGREEN);
        //if (j==0 && i==11) {
          //myTmpEnemyDrawSprite=CopyCrunchyBitmap(tmp_sprite1,SRCCOPY);
        //}
      GenerateDrawSprite(&LoadEnemyRotatedSprite[j].draw_rotated_sprite1[i],LoadEnemyRotatedSprite[j].prelude_tmp_sprite1[i]);
      GenerateDrawSprite(&LoadEnemyRotatedSprite[j].draw_rotated_sprite2[i],LoadEnemyRotatedSprite[j].prelude_tmp_sprite2[i]);
      //DeleteObject(tmp_sprite2);
      //DeleteObject(tmp_sprite1);
      //printf("%d/%d %1.0f/%1.0f\n",i,j,loading_numerator,loading_denominator);
    } else { //j==3, xtra sprites
      ReplaceBitmapColor(XLoadEnemyRotatedSprite[0].prelude_tmp_sprite[i],LTBLUE,LTGREEN);
      ReplaceBitmapColor2(XLoadEnemyRotatedSprite[0].prelude_tmp_sprite[i],LTGREEN,BLACK,8,LTGREEN);
      GenerateDrawSprite(&XLoadEnemyRotatedSprite[0].draw_rotated_sprite[i],XLoadEnemyRotatedSprite[0].prelude_tmp_sprite[i]);
      //DeleteObject(tmp_sprite1);
      //printf("%1.0f/%1.0f xsprite:%d\n",loading_numerator,loading_denominator,i);
    }
    //}
  //} 
    prelude_sprite_id++;
    if (i>=ROTATED_SPRITE_NUM-1) {
      prelude_sprite_id=0;
      prelude_sprite_jid++;
    }
  }
  //printf("%1.0f/%1.0f\n",loading_numerator,loading_denominator);
}



DWORD WINAPI AnimateTask01(LPVOID lpArg) {
  while (TRUE) {
    if (prelude) {
      Sleep(1000);
    } else if (level_loading) {
      Sleep(1000);
    } else if (!in_main_menu) { //In Game
      if (flag_restart) {
        //Sleep(100);
      } else {
        if (level_loaded) {
          PlayerAct();
          //PlayerAct(player_type);

          for (int i=0;i<ENEMY_NUM;i++) {
            EnemyAct(i);
          }
          if (map_weather>0) {
            RainAct();
          //if (!player.time_breaker) {
            //ScreenRainDropAct();
          //}
          }
          if (FIRE_GROUND_NUM>0 && !player.time_breaker) {
            GroundFireAct();
          }
          Sleep(player.sleep_timer);
        } else {
          Sleep(1000);
        }
      }
    } else if (in_map_editor) {
      MapEditorAct();
      Sleep(6);
    } else {
      if (main_menu_chosen==3 || blank_level) {
        Sleep(1000);
      } else {
        if (wav_mode!=0) { //non mode 0, dj
          Sleep(1000);
        } else { //mode 0, map demo
          if (flag_restart) {
            //Sleep(100);
          }
          if (level_loaded && !flag_restart) {
            PlayerAct();
      
            for (int i=0;i<ENEMY_NUM;i++) {
              EnemyAct(i);
            }
            if (map_weather>0) {
              RainAct();
              /*if (!player.time_breaker) {
                ScreenRainDropAct();
              }*/
            }
            if (FIRE_GROUND_NUM>0 && !player.time_breaker) {
              GroundFireAct();
            }
          }

          //demo main menu sound act
          if (game_audio && !flag_load_melevel) {
              for (int i=0;i<player.bullet_shot_num;i++) {
                BulletSndAct(player.bullet[i]);
              }
              if (player.bullet_shot!=-1) { //player sounds made by sniper player bullets
                BulletSndAct(player.bullet_shot);
              }
              /*for (int i=0;i<ENEMY_NUM;i++) {
                EnemySndAct(i);
              }*/
              PlayerSndAct();       
          }
          Sleep(6);
          //Sleep(1000);
        } //end of wav_mode==0

        if (flag_load_level) {
          flag_load_level=FALSE;
          if (main_menu_chosen==0) {
            InitLevel(TRUE);
          }
        } else if (flag_load_melevel) {
          InterruptAllSnd();
          flag_load_melevel=FALSE;
          InitLevelMapEditor();
        }
      }
    }
  }
}


DWORD WINAPI AnimateTask02(LPVOID lpArg) { //FPS counter
  while (TRUE) {
    if (!prelude) {
        saved_showoff=showoff;
        showoff=0;

        //music name scrolling
        for (int z=0;z<2;z++) {
          if (audioData[z].playing_wav && !stop_playing_song[z]) {
             //scrolling effect
             int padding_length=5;
             int le_song_len=wcslen(song_names[song_rand_num[z]]);
             int total_length = le_song_len+padding_length;
             if (le_song_len>47) { //more than song length
               if (wspc_start[z]<le_song_len) {
                 wspc_start[z]+=2;           
               } else {
                 wspc_start[z]=0;
               }
              for (int w = 0; w < 48; w++) {
                int src_index = (wspc_start[z] + w) % total_length; // Source index with wrap-around 
                if (src_index < le_song_len) { 
                  print_song_playing[z][w] = song_names[song_rand_num[z]][src_index]; 
                } else {
                  print_song_playing[z][w] = L' '; // Add padding with blank spaces
                } 
              }
             } else {
               wspc_start[z]=0;
             }
           }
        }
        Sleep(1000);


    } else { //intro prelude
        if (loading_numerator>=loading_denominator) {
            BYTE* pDIB = (BYTE*) AVIStreamGetFrame(pFrame, global_frames);
            if (global_frames%2!=0) {
              DeleteObject(global_avi_bitmap2);
              global_avi_bitmap2=CreateFromPackedDIBPointer2(pDIB); //set up and draw even
            } else {
              DeleteObject(global_avi_bitmap1);
              global_avi_bitmap1=CreateFromPackedDIBPointer2(pDIB); //set up and draw odd
            }

            global_frames++;
            if (global_frames>=iNumFrames)
              global_frames=0;


            Sleep(70);
        } else {
          Sleep(1000);
        }
    }
  }
}



void InitSetRes(int i,int w,int h,char *txt,wchar_t* wtxt)
{
  RESOLUTION_X[i]=w;
  RESOLUTION_Y[i]=h;
  RESOLUTION_NAME[i]=txt;
  WRESOLUTION_NAME[i]=wtxt;
}







LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff, hdcBackbuff2;
  switch(msg) {

    //Left Click Hold
    case WM_LBUTTONDOWN:
      player.rst_left_click=TRUE;
      break;

    //Left Click Release
    case WM_LBUTTONUP:
      player.rst_left_click=FALSE;
      player.attack_rst=TRUE;
      break;


    //Right Click Hold
    case WM_RBUTTONDOWN:
      player.rst_right_click=TRUE;
      break;

    //Right Click Release
    case WM_RBUTTONUP:
      player.rst_right_click=FALSE;
      break;


    /*case WM_MBUTTONUP:
      rst_mbutton=FALSE;
      break;


    case WM_MBUTTONDOWN:
      rst_mbutton=TRUE;
      break;*/

    //Mouse Scroller movement like disc scratching
    case WM_MOUSEWHEEL: {
        int e=1;
        if (keydown(VK_LSHIFT)) {
          e=3;
        } else if (keydown(VK_RSHIFT)) {
          e=5;
        }
        for (int f=0;f<e;f++) {
        mouse_wheel_timer=6;
        int wheelData = GET_WHEEL_DELTA_WPARAM(wParam);
        if (!stop_playing_song[gct] && audioData[gct].playing_wav && wheelData!=0) {
          audioData[gct].song_rewind=TRUE;

          if (audioData[gct].current_filesize>audioData[gct].read_size*2 && audioData[gct].current_filesize<audioData[gct].filesize-audioData[gct].read_size*2) {


              if (wheelData>0) { //scroll up
                if (audioData[gct].queue_read_buffer>0) {audioData[gct].queue_read_buffer--;} else {audioData[gct].queue_read_buffer=READ_BUFFER_NUM-1;}
                if (audioData[gct].queue_play_buffer>0) {audioData[gct].queue_play_buffer--;} else {audioData[gct].queue_play_buffer=READ_BUFFER_NUM-1;} 
                audioData[gct].current_filesize-=audioData[gct].read_size;
              } else if (wheelData<0){ //scroll down
                if (audioData[gct].queue_read_buffer<=18) {audioData[gct].queue_read_buffer++;} else {audioData[gct].queue_read_buffer=0;}
                if (audioData[gct].queue_play_buffer<=18) {audioData[gct].queue_play_buffer++;} else {audioData[gct].queue_play_buffer=0;} 
                audioData[gct].current_filesize+=audioData[gct].read_size;
              }


              adjustBufferVol(audioData[gct].buffer1,audioData[gct].read_buffer[audioData[gct].queue_play_buffer],audioData[gct].read_size,audioData[gct].volume);
              adjustBufferVol(audioData[gct].buffer2,audioData[gct].read_buffer[audioData[gct].queue_play_buffer],audioData[gct].read_size,audioData[gct].volume);

              audioData[gct].read_filesize=audioData[gct].current_filesize-(audioData[gct].read_size*READ_BUFFER_NUM/2);

              fseek(audioData[gct].music_file, audioData[gct].current_filesize, SEEK_SET);
              fread(audioData[gct].read_buffer[audioData[gct].queue_read_buffer], sizeof(BYTE), audioData[gct].read_size, audioData[gct].music_file);  //copy then go backwards/forwards
              if (audioData[gct].queue_read_buffer>0) {
                PassFilter(audioData[gct].read_buffer[audioData[gct].queue_read_buffer],audioData[gct].read_buffer[audioData[gct].queue_read_buffer-1],&audioData[gct],chosen_buffer_length);
              } else {
                PassFilter(audioData[gct].read_buffer[0],audioData[gct].read_buffer[19],&audioData[gct],chosen_buffer_length);
              }

              fseek(audioData[gct].music_file, audioData[gct].current_filesize, SEEK_SET);

              waveOutWrite(audioData[gct].hWaveOut, &audioData[gct].waveHdr1, sizeof(WAVEHDR));
              waveOutWrite(audioData[gct].hWaveOut, &audioData[gct].waveHdr2, sizeof(WAVEHDR));

          }
        }
        }
      }    
      break;
        

    //Mouse Movement
    case  WM_MOUSEMOVE: //https://stackoverflow.com/questions/22039413/moving-the-mouse-blocks-wm-timer-and-wm-paint
      if (!IsIconic(hwnd)) //no action when minimized
      {
        POINT point;
        int true_mouse_x;
        int true_mouse_y;
        if (GetCursorPos(&point)) {
          if (hide_taskbar) {
            true_mouse_x=mouse_x=point.x-(SCREEN_WIDTH/2-RESOLUTION_X[resolution_choose]/2);
            true_mouse_y=mouse_y=point.y-(SCREEN_HEIGHT/2-RESOLUTION_Y[resolution_choose]/2);
          } else {
            true_mouse_x=mouse_x=point.x-windowx;
            true_mouse_y=mouse_y=point.y-windowy;
          }
        }
        if (true_mouse_x>5 && true_mouse_x<GR_WIDTH-5 && true_mouse_y>5 && true_mouse_y<GR_HEIGHT-5) { //within game
          if (!hide_cursor) {
            hide_cursor=TRUE;
            ShowCursor(FALSE);
          }
        } else {
          if (hide_cursor) { //outside of game
            hide_cursor=FALSE;
            ShowCursor(TRUE);
          }
        }
        if (mouse_x>GR_WIDTH)
          mouse_x=GR_WIDTH;
        else if (mouse_x<0)
          mouse_x=0;
        if (mouse_y>GR_HEIGHT)
          mouse_y=GR_HEIGHT;
        else if (mouse_y<0)
          mouse_y=0;
        UpdateWindow(hwnd);
      }
      break;

    //Various Keypress down
    case WM_KEYDOWN:
    {
      //Global keydown press
      if (prelude) {
        if (loading_numerator>=loading_denominator) {
          prelude=FALSE;
          LoadOptions();
          InitSongBank(); //load song
          play_new_song[0]=TRUE;

          //Load options after prelude
          if (hide_taskbar) { //task bar hidden yes fullscreen
            flag_fullscreen=TRUE;
          } else {
            flag_borderless_resolution_change=TRUE;
          }

          for (int i=0;i<KEY_SFX_NUM;i++) {
            adjustSFXVolume(&keySoundEffectCache[i],&keySoundEffect[i],game_volume,FALSE);
          }
          AdjustGameAudio();

          //flag_adjust_wav_out_audio=TRUE;
          flag_adjust_audio=TRUE;
          flag_update_background=TRUE;
          InitLevel(FALSE);
          frame_tick=-FPS;
          player.flag_revert_palette=TRUE;
          player.time_breaker_tick=-1;
          KeyChangePlayerColor();
        }
      }


      if (!level_loading && !prelude) {
      if (main_menu_chosen!=2) {
        GlobalKeypressDown(wParam);
      }

      //Key Down Presses depending on game state
      if (!in_main_menu) {
        GameKeypressDown(wParam);
      } else { //Main menu
        if (!in_map_editor && main_menu_chosen<2 && !color_chooser.is_choosing_color) {
          MMKeypressDown(wParam);
        }
        switch (main_menu_chosen) {
           case -1:
             MinusOneMenuKeypressDown(wParam);
             break;

           case 0:
             ZeroMenuKeypressDown(hwnd,hdc,wParam);
             break;

           //Player options in main menu
           case 1:
             if (color_chooser.is_choosing_color) {
              switch (option_choose) {
                case 0:
                  ColorKeypressDown(wParam,&player_color);
                  break;
                case 1:
                  ColorKeypressDown(wParam,&player_iris_color);
                  break;
                case 2:
                  ColorKeypressDown(wParam,&player_pupil_color);
                  break;
              }
             } else {
               OptionsKeypressDown(hwnd, wParam);
             }
             break;

           case 2:
             TwoMenuKeypressDown(wParam);
             break;

           case 3:
             ThreeMenuKeypressDown(wParam);
             break;


           case 4:
            if (color_chooser.is_choosing_color) {
             MEColorPickKeypressDown(wParam);
            } else {
             MapEditorKeypressDown(wParam);
            }
            break;

        } //end of switch statement for menu chosen
      } //end of menu chosen if else
      }
      break; //Break WM_KEYDOWN;
    } 



    //Key Release
    case WM_KEYUP:
    {
  //GLOBAL wParam Release Key
  //    printf("%d",wParam);
      if (!level_loading && !prelude) {
      if (main_menu_chosen!=2) {
        GlobalKeypressUp(hwnd,wParam);
      }

      if (!in_main_menu) { //Gaming
        GameKeypressUp(wParam);
      } else { 
        if (!in_map_editor && main_menu_chosen<2 && !color_chooser.is_choosing_color) {
          MMKeypressUp(wParam);
        }
        switch (main_menu_chosen) {
          case -1:
            MinusOneMenuKeypressUp(wParam);
            break;

          case 0: //Level select keyup
            ZeroMenuKeypressUp(hwnd,hdc,wParam);
            break;

          case 1:
            if (color_chooser.is_choosing_color) {
              switch (option_choose) {
                case 0:
                  ColorKeypressUp(wParam,&player_color);
                  break;
                case 1:
                  ColorKeypressUp(wParam,&player_iris_color);
                  break;
                case 2:
                  ColorKeypressUp(wParam,&player_pupil_color);
                  break;
              }
            } else {
              OptionsKeypressUp(wParam);
            }
            break;

          case 2:            
            TwoMenuKeypressUp(wParam);
            break;

           case 3:
             ThreeMenuKeypressUp(wParam,hwnd,hdc);
             break;

           case 4:
             if (color_chooser.is_choosing_color) {
               MEColorPickKeypressUp(wParam);
             } else {
               MapEditorKeypressUp(wParam, hwnd, hdc);
             }
             break;
          }
          break; //end of main menu chosen switch
      }

      } //end of !main_menu_chosen
      break; // end of release key WM_KEYUP
    }


    case WM_CHAR:
      if (MapEditor.is_ground_txt_typing) {
          if (MapEditor.typing_ground_txt_pos<512) {
            if (wParam==0x08) { //Backspace
              if (MapEditor.typing_ground_txt_pos>0) {
                MapEditor.typing_ground_txt_pos--;
                MapEditor.typing_ground_txt[MapEditor.typing_ground_txt_pos]=0;
              }
            } else {
              if (MapEditor.typing_ground_txt_pos<511) {
                if (!(wParam>=0 && wParam<13) &&
                    !(wParam>13 && wParam<=31) //13 == '\n'
                ) { //Disallow illegal characters
                  if (MapEditor.is_ground_txt_typing_loaded) {
                    MapEditor.typing_ground_txt[MapEditor.typing_ground_txt_pos]=wParam;
                    MapEditor.typing_ground_txt_pos++;
                  } else {
                    MapEditor.is_ground_txt_typing_loaded=TRUE;
                  }
                }
              }
            }
          }        
      } else if (MapEditor.is_typing_search) {
        if (MapEditor.typing_search_txt_pos<5) {
          if (wParam==0x08) { //Backspace
            if (MapEditor.typing_search_txt_pos>0) {
              MapEditor.typing_search_txt_pos--;
              MapEditor.typing_search_id/=10;
            }
          } else {
            if (MapEditor.typing_search_txt_pos<4) {
              if (wParam>='0' && wParam<='9') { //keys '0' to '9' only
                MapEditor.typing_search_id*=10;
                MapEditor.typing_search_id+=wParam-'0';
                MapEditor.typing_search_txt_pos++;
              }
            }
          }
        }
      }
      //https://learn.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input
      //https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch?view=msvc-170
      if (main_menu_chosen==-1 || main_menu_chosen==0 || main_menu_chosen==1 || !in_main_menu) {
        //extended keys for dj
        DJKeys(wParam);
      }
      if (in_main_menu) {
        if (main_menu_chosen==2 && create_lvl_option_choose==0) {
          if (typing_lvl_name_pos<17) {
            if (wParam==0x08) { //Backspace
              if (typing_lvl_name_pos>0) {
                typing_lvl_name_pos--;
                typing_lvl_name[typing_lvl_name_pos]=0;
              }
            } else {
              if (typing_lvl_name_pos<16) {
                if (wParam!='\\' && 
                    wParam!='/' &&
                    wParam!='<' &&
                    wParam!='>' &&
                    wParam!=':' &&
                    wParam!='"' &&
                    wParam!='|' &&
                    wParam!='?' &&
                    wParam!='*' &&
                    wParam!='.' &&
                    wParam!=' ' &&
                    wParam!='\n' &&
                    !(wParam>=0 && wParam<=31) 
                ) { //Disallaw illegal characters
                  typing_lvl_name[typing_lvl_name_pos]=wParam;
                  typing_lvl_name_pos++;
                }
              }
            }
          }
        } else if (main_menu_chosen==0) {
          if (wParam==0x08) { //backspace = delete level
            if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
              if (level_chosen>=0 && level_chosen<level_num) {
                wchar_t remove_folder_name[48];
                swprintf(remove_folder_name,48,L"saves/%ls",level_names[level_chosen]);
                RemoveFolderRecursive(remove_folder_name);
                GetSavesInDir(L"saves");
              }
              if (game_audio)
                PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
            }
          }
        }
      }
      break;

    //Constantly Update Screen
    case WM_ERASEBKGND:
      if (!IsIconic(hwnd)) //no action when minimized
        InvalidateRect(hwnd,NULL,TRUE);
      return TRUE;
      break;



    //Graphics DrawIt()
    case WM_PAINT: //https://cplusplus.com/forum/beginner/269434/
      FrameRateSleep(FPS); // (Uncapped) //35 or 60 fps Credit: ayevdood/sharoyveduchi && y4my4m - move it here
      if (!IsIconic(hwnd)) //no action when minimized, prevents crash https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isiconic?redirectedfrom=MSDN
      {
        //RandIndexIncrement();
        if (mouse_wheel_timer>0) {
          mouse_wheel_timer--;
          if (mouse_wheel_timer==1) {
            mouse_wheel_timer=0;
            audioData[gct].song_rewind=FALSE;
            if (!stop_playing_song[gct] && audioData[gct].playing_wav) {
              InitAudioBuffer(gct);
            }
          }
        }

        if (flag_hide_taskbar) {
          flag_hide_taskbar=FALSE;
          hide_taskbar=FALSE;
          LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
          lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
          SetWindowLong(hwnd, GWL_STYLE, lStyle);
          SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
        }
        if (flag_adjust_audio) {
          freeSoundEffectCache(&keySoundEffectCache[2]);
          adjustSFXVolume(&keySoundEffectCache[2],&keySoundEffect[2],game_volume,FALSE);
          flag_adjust_audio=FALSE;
        }
        if (flag_fullscreen) {
          flag_fullscreen=FALSE;
          hide_taskbar=TRUE;
          LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
          lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU); //borderless mode
          SetWindowLong(hwnd, GWL_STYLE, lStyle);
          SetWindowPos(hwnd,HWND_TOPMOST,0,0,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
        }
        if (hide_taskbar) {
          GR_WIDTH=RESOLUTION_X[resolution_choose];
          GR_HEIGHT=RESOLUTION_Y[resolution_choose];
        } else {
          RECT rect;
          if(GetWindowRect(hwnd, &rect)) {
            GR_WIDTH = rect.right - rect.left;
            GR_HEIGHT = rect.bottom - rect.top;        
            windowx = rect.left;
            windowy = rect.top;
          }
        }

        if (GR_WIDTH!=OLD_GR_WIDTH || GR_HEIGHT!=OLD_GR_HEIGHT || flag_update_background) { //change in background res
          if (!in_map_editor) {
            InitPlayerCamera(player.saved_x,player.saved_y);
            player.cam_x=0;
            player.cam_y=0;
            CameraInit(player.x,player.y);
          }
          InitRDGrid();
          ResetBulletRain();
          if (map_weather>0) {
            InitBulletRain();
          }
          if (in_map_editor) {
            InitMERDGrid();
          }
          if (flag_update_background) {
            flag_update_background=FALSE;
          }
          OLD_GR_WIDTH = GR_WIDTH;
          OLD_GR_HEIGHT = GR_HEIGHT;

          //In main menu
          //Load Map Background sprites
             int mb_val=-1;
             if (in_map_editor) {
               mb_val=MapEditor.set_lvl_ambient_val[0];
             } else {
               mb_val=map_background;
             }
            if (mb_val>=0 && mb_val<=4) {
              if (map_background_sprite!=NULL) { //renew and scale upwards
                DeleteObject(map_background_sprite);
              }
              HBITMAP tmp_map_background_sprite;

              wchar_t lvl_background_bmp[64];
              swprintf(lvl_background_bmp,64,L"saves/%ls/images/background.bmp",level_names[level_chosen]);
              tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, lvl_background_bmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

              if ((in_main_menu && !level_loading && !in_map_editor) || back_to_menu) {
                if (tmp_map_background_sprite==NULL) {  //custom overwrites regular
                  if (solar_hour>6 && solar_hour<18) { //day
                    if (map_weather!=0) {mb_val=2;} else {mb_val=0;}
                  } else {       //night
                    if (map_weather!=0) {mb_val=3;} else {mb_val=1;}
                  }
                }
              }

              if (tmp_map_background_sprite==NULL) { //custom background doesnt exist
                switch (mb_val) {
                  case 0:
                    if (GR_WIDTH<800 && GR_HEIGHT<600) {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                    } else {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky_hd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                    }
                    break;
                  case 1:
                    if (GR_WIDTH<800 && GR_HEIGHT<600) {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                    } else {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars_hd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);                    
                    }
                    break;
                  case 2:
                    if (GR_WIDTH<800 && GR_HEIGHT<600) {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/skdark0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                    } else {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/skdark0_hd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);                    
                    }
                    break;
                  case 3:
                    if (GR_WIDTH<800 && GR_HEIGHT<600) {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/skdark1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                    } else {
                      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/skdark1_hd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);                    
                    }
                    break;
                }
              }
              if (tmp_map_background_sprite!=NULL) {
                map_background_sprite=CopyStretchBitmap(tmp_map_background_sprite,SRCCOPY,GR_WIDTH+GR_WIDTH/8,GR_HEIGHT+GR_HEIGHT/8); //note runs once only
                if (mb_val!=1 && mb_val!=2 && mb_val!=3) {
                  PlaceDayMoon();
                }
              } else {
                map_background_sprite=NULL;
              }
              DeleteObject(tmp_map_background_sprite);
            }
        } //end of screen actions


        HBITMAP screen;
        PAINTSTRUCT ps;
        
        if (prelude) {
          hdc=BeginPaint(hwnd, &ps);
          hdcBackbuff=CreateCompatibleDC(hdc);
          hdcBackbuff2=CreateCompatibleDC(hdcBackbuff);

          screen=CreateCompatibleBitmap(hdc,640,440);
          SelectObject(hdcBackbuff,screen);

          DrawMovingAVI(hdcBackbuff,hdcBackbuff2);

          DrawBitmap(hdcBackbuff,hdcBackbuff2,GR_WIDTH/2-370/2,GR_HEIGHT/2-370/2-48,0,0,370,370,intro_msg_mask,SRCAND,FALSE,FALSE);
          DrawBitmap(hdcBackbuff,hdcBackbuff2,GR_WIDTH/2-370/2,GR_HEIGHT/2-370/2-48,0,0,370,370,intro_msg,SRCPAINT,FALSE,FALSE);

          if (loading_numerator<loading_denominator) {
            DrawLoading(hdcBackbuff);
            Prelude();
          } else {
            DrawBitmap(hdcBackbuff,hdcBackbuff2,GR_WIDTH/2-324/2,GR_HEIGHT-96-8,0,0,324,47,kh_pressanykey_mask,SRCAND,FALSE,FALSE);
            DrawBitmap(hdcBackbuff,hdcBackbuff2,GR_WIDTH/2-324/2,GR_HEIGHT-96-8,0,0,324,47,kh_pressanykey,SRCPAINT,FALSE,FALSE);
          }

          BitBlt(hdc, 0, 0, 640,440, hdcBackbuff, 0, 0,  SRCCOPY);
          DeleteDC(hdcBackbuff2);
          DeleteDC(hdcBackbuff);
          DeleteObject(screen);
        } else if (level_loading) {
          hdc=BeginPaint(hwnd, &ps);
          hdcBackbuff=CreateCompatibleDC(hdc);
          hdcBackbuff2=CreateCompatibleDC(hdcBackbuff);

          if (flag_begin_drawing_tiles) {
            DrawCreateTiles(hdcBackbuff,hdcBackbuff2);
          }

          screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
          SelectObject(hdcBackbuff,screen);
          //GrRect(hdcBackbuff,0,0,GR_WIDTH+2,GR_HEIGHT+2,BLACK);    
          DrawBackground(hdcBackbuff,hdcBackbuff2);
          DrawLoading(hdcBackbuff/*,16*/);
          DrawCursor(hdcBackbuff,hdcBackbuff2);
          if (hide_taskbar) {
            BitBlt(hdc, SCREEN_WIDTH/2-RESOLUTION_X[resolution_choose]/2, 
                        SCREEN_HEIGHT/2-RESOLUTION_Y[resolution_choose]/2, 
                        RESOLUTION_X[resolution_choose],
                        RESOLUTION_Y[resolution_choose],
                        hdcBackbuff, 0, 0,  SRCCOPY);
          } else {
            BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
          }
          DeleteDC(hdcBackbuff2);
          DeleteDC(hdcBackbuff);
          DeleteObject(screen);
        } else {



        if (!in_main_menu) //### In game
        { //https://stackoverflow.com/questions/752593/win32-app-suspends-on-minimize-window-animation
          frame_tick++;
          showoff++;
          if (frame_tick>FPS) {
            frame_tick=0;
          }

          if (level_loaded) {
            if (enemy_kills<ENEMY_NUM) {
              game_timer= current_timestamp() - time_begin;
            } else {
              if (!game_over) {
                if (game_timer<int_best_score && player.health>0) { //New high score
                  FILE *fptr;
                  fptr = _wfopen(save_level,L"w");
                  char txt[12];
                  int tmp_game_timer=game_timer;
                  sprintf(txt,"%d\n",tmp_game_timer);
                  fprintf(fptr,txt);
                  fclose(fptr);
                }
                game_over=TRUE;
              }
            }

            if (game_cam_shake && player.health>0) {
              PlayerCameraShake();
            }

            if (level_loaded && flag_restart) { // restart level when player health hits 0 or VK_RETURN
              flag_restart_audio=TRUE;
              Init();
              flag_restart=FALSE;
            }

            hdc=BeginPaint(hwnd, &ps);
            hdcBackbuff=CreateCompatibleDC(hdc);
            hdcBackbuff2=CreateCompatibleDC(hdcBackbuff);
            screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
            SelectObject(hdcBackbuff,screen);
            DrawBackground(hdcBackbuff,hdcBackbuff2);
            DrawWaterPlatformsTexture(hdcBackbuff,hdcBackbuff2);
            DrawPlatforms(hdcBackbuff,hdcBackbuff2);
            DrawFirePlatforms(hdcBackbuff);
            DrawWebs(hdcBackbuff);
            DrawEnemy(hdcBackbuff,hdcBackbuff2);
            //DrawPlayer(hdcBackbuff,hdcBackbuff2);
            DrawPlayer(hdcBackbuff,hdcBackbuff2,player.type);

            DrawWaterPlatforms(hdcBackbuff,hdcBackbuff2);
            //DrawNodeGrids(hdcBackbuff); //debugging


            if (is_shadows && game_shadow && SHADOW_GRID_NUM>0) {
              DrawShadows(hdcBackbuff,hdcBackbuff2);
            }

            //DrawGrids(hdcBackbuff); //debugging
            //DrawWaterShader(hdcBackbuff,hdcBackbuff2); //deprecated
            if (map_weather>0) {
              DrawRain(hdcBackbuff,hdcBackbuff2);
              /*if (!player.in_water) {
                DrawRainShader(hdcBackbuff,hdcBackbuff2);  //deprecated
              }*/
            }
            DrawBlackBorders(hdcBackbuff);
            DrawUI(hdcBackbuff,hdcBackbuff2);
            if (player.health>0) {
              DrawCursor(hdcBackbuff,hdcBackbuff2);
            } else if (player.death_timer>150) { //dead cursor
              int pc=rgbPaint[player_color];
              GrCircle(hdcBackbuff,mouse_x,mouse_y,10,pc,pc);
              pc=rgbPaint[player_pupil_color];
              GrCircle(hdcBackbuff,mouse_x,mouse_y,5,pc,pc);
              pc=rgbPaint[player_iris_color];
              if (player.bullet_shot_num>0) {
                if (!player.rst_left_click) {
                  GrCircle(hdcBackbuff,mouse_x,mouse_y,RandNum(1,5,&misc_rng_i,-1),pc,pc);
                } else {
                  GrCircle(hdcBackbuff,mouse_x,mouse_y,5,pc,pc);
                }
              } else {
                call_help_timer=0;
              }
            }



            int c=BLACK;
            if (IsSpeedBreaking()) { //cinema mode when speedbreaking
              GrRect(hdcBackbuff,0,0,GR_WIDTH+4,32,c);
              if (hide_taskbar) {
                GrRect(hdcBackbuff,0,GR_HEIGHT-48,GR_WIDTH+4,100,c);
              } else {
                GrRect(hdcBackbuff,0,GR_HEIGHT-32-32,GR_WIDTH+4,32+32,c);
              }
            }


            if (hide_taskbar) {
              BitBlt(hdc, SCREEN_WIDTH/2-RESOLUTION_X[resolution_choose]/2, 
                            SCREEN_HEIGHT/2-RESOLUTION_Y[resolution_choose]/2, 
                            RESOLUTION_X[resolution_choose],
                            RESOLUTION_Y[resolution_choose],
                            hdcBackbuff, 0, 0,  SRCCOPY);
            } else {
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
            }
            
            DeleteDC(hdcBackbuff2);
            DeleteDC(hdcBackbuff);
            DeleteObject(screen);


            //Trigger go back to main menu
            if (back_to_menu) {
              CleanupAll(TRUE);
              flag_update_background=TRUE;
              InitLevel(FALSE);
            }
          }
        } else if (in_map_editor) {
          if (level_loaded) {
            frame_tick++;
            showoff++;
            if (frame_tick>FPS) {
              frame_tick=0;
            }

            hdc=BeginPaint(hwnd, &ps);
            hdcBackbuff=CreateCompatibleDC(hdc);
            hdcBackbuff2=CreateCompatibleDC(hdcBackbuff);


            if (MapEditor.load_ptextures) {
              for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
                InitColorPlatformTextures(hdcBackbuff,hdcBackbuff2,i);
              }
              MapEditor.load_ptextures=FALSE;
            }

            if (MapEditor.alter_ptexture_color) {
              InitColorPlatformTextures(hdcBackbuff,hdcBackbuff2,MapEditor.selected_ptexture_id);
              MapEditor.alter_ptexture_color=FALSE;
            }


            screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
            SelectObject(hdcBackbuff,screen);
            DrawMapEditorBackground(hdcBackbuff,hdcBackbuff2);
            DrawMapEditorWaterTexturePlatforms(hdcBackbuff,hdcBackbuff2);
            DrawMapEditorPlatforms(hdcBackbuff,hdcBackbuff2);
            DrawMapEditorEnemy(hdcBackbuff,hdcBackbuff2);
            DrawMapEditorPlayer(hdcBackbuff,hdcBackbuff2);
            DrawGrids(hdcBackbuff,player.cam_x+GR_WIDTH/2,player.cam_y+GR_HEIGHT/2);
            DrawMapEditorWaterPlatforms(hdcBackbuff);
            DrawMapEditorUI(hdcBackbuff,hdcBackbuff2);
            DrawCursor(hdcBackbuff,hdcBackbuff2);

            if (hide_taskbar) {
              BitBlt(hdc, SCREEN_WIDTH/2-RESOLUTION_X[resolution_choose]/2, 
                            SCREEN_HEIGHT/2-RESOLUTION_Y[resolution_choose]/2, 
                            RESOLUTION_X[resolution_choose],
                            RESOLUTION_Y[resolution_choose],
                            hdcBackbuff, 0, 0,  SRCCOPY);
            } else {
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
            }
            DeleteDC(hdcBackbuff2);
            DeleteDC(hdcBackbuff);
            DeleteObject(screen);

              //Map Editor, Trigger go back to main menu
            if (back_to_menu) {
              CleanupMapEditorAll();
              flag_update_background=TRUE;
              InitLevel(FALSE);
            }
          }
        } else { //In Main Menu
          showoff++;
          frame_tick++;
          if (frame_tick>FPS) {
            frame_tick=0;
          }
          hdc=BeginPaint(hwnd, &ps);
          hdcBackbuff=CreateCompatibleDC(hdc);
          hdcBackbuff2=CreateCompatibleDC(hdcBackbuff);

          if (flag_load_player_sprite) {
            flag_load_player_sprite=FALSE;
            InitPlayerSpritesObjColor(hdc,hdcBackbuff);
            InitPlayerCursorColor(hdc,hdcBackbuff2);
          }

          if (flag_difficulty_change) {
            flag_difficulty_change=FALSE;
            if (game_hard) {
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_sprite[0],rgbColorsRedToBlue);
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_small_sprite[0],rgbColorsRedToBlue);
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_sprite[1],rgbColorsRedToBlue);
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_small_sprite[1],rgbColorsRedToBlue);
            } else {
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_sprite[0],rgbColorsNoir);
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_small_sprite[0],rgbColorsNoir);
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_sprite[1],rgbColorsNoir);
              BitmapPalette(hdcBackbuff,hdcBackbuff2,title_small_sprite[1],rgbColorsNoir);
            }
          }

          if (!flag_taskbar_change_act && !flag_resolution_change && !flag_borderless_resolution_change) {
            screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
            SelectObject(hdcBackbuff,screen);
      
            DrawMainMenu(hdcBackbuff,hdcBackbuff2);
          //DrawNodeGrids(hdcBackbuff); //debugging

            DrawCursor(hdcBackbuff,hdcBackbuff2);
            DrawMMExtraKeys(hdcBackbuff);
            if (hide_taskbar) {
              BitBlt(hdc, SCREEN_WIDTH/2-RESOLUTION_X[resolution_choose]/2, 
                            SCREEN_HEIGHT/2-RESOLUTION_Y[resolution_choose]/2, 
                            RESOLUTION_X[resolution_choose],
                            RESOLUTION_Y[resolution_choose],
                            hdcBackbuff, 0, 0,  SRCCOPY);
            } else {
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
            }
          } else {
            if (flag_resolution_change) { //blackout clear screen
              screen=CreateCompatibleBitmap(hdc,SCREEN_WIDTH,SCREEN_HEIGHT);
              SelectObject(hdcBackbuff,screen);
              GrRect(hdcBackbuff,0,0,SCREEN_WIDTH,SCREEN_HEIGHT,BLACK);
              BitBlt(hdc, 0,0, SCREEN_WIDTH,SCREEN_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
              flag_resolution_change=FALSE;
            } else if (flag_taskbar_change_act) {
              TaskbarChangeAct(hwnd);
              flag_taskbar_change_act=FALSE;
            } else if (flag_borderless_resolution_change) {
              SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
              ShowWindow(hwnd, SW_RESTORE);
              SetForegroundWindow(hwnd); //return back focus
              GR_WIDTH=RESOLUTION_X[resolution_choose];
              GR_HEIGHT=RESOLUTION_Y[resolution_choose];
              flag_borderless_resolution_change=FALSE;
            }
          }

          DeleteDC(hdcBackbuff2);
          DeleteDC(hdcBackbuff);
          DeleteObject(screen);
        }
        }
        EndPaint(hwnd, &ps);
      }
      return 0;
      break;


    case WM_INPUT:
        {
        //https://www.codeproject.com/Articles/185522/Using-the-Raw-Input-API-to-Process-Joystick-Input
            UINT dwSize;
            GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &dwSize, sizeof(RAWINPUTHEADER));
            LPBYTE lpb = (LPBYTE)malloc(dwSize);

            if (lpb == NULL) {
                return 0;
            }

            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER)) != dwSize) {
                printf("GetRawInputData does not return correct size!\n");
            }

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEHID) {
                // Process joystick input [debug print]
                /*printf("\n Joystick input data: ");
                for (UINT i = 0; i < raw->data.hid.dwSizeHid; ++i) {
                    printf("%02X ", raw->data.hid.bRawData[i]);
                }*/


                // Add code here to interpret the joystick data
                ccontroller.connected=TRUE;
                GlobalJoystickMove(raw->data.hid.bRawData);

                if (!in_main_menu) {
                  GameCKeyPress(raw->data.hid.bRawData);
                } else { //Main menu
                  switch (main_menu_chosen) {
                     case -1:
                       MinusOneMenuCKeyPress(raw->data.hid.bRawData);
                       break;
                     case 0:
                       ZeroMenuCKeyPress(hwnd,hdc,raw->data.hid.bRawData);
                       break;
                     case 1:
                       OptionsCKeyPress(hwnd, raw->data.hid.bRawData);
                       break;
                     //case 2:
                       //TwoMenuCKeyPress(raw->data.hid.bRawData);
                       //break;
                     //case 3:
                       //ThreeMenuCKeyPress(raw->data.hid.bRawData);
                       //break;
                     //case 4:
                       //MapEditorCKeyPress(raw->data.hid.bRawData);
                       //break;
                  } //end of switch statement for menu chosen
                } //end of menu chosen if else
            } else {
              ccontroller.connected=FALSE;
            }

            free(lpb);
            break;
        }
        return 0;
        break;


                //Actions based on Joystick input
                // Process joystick input

        //}
        //return 0;

    //Tasks to perform on start
    case WM_CREATE:
    {
      //LoadBufferSFX(L"music/helicopter.wav");
      //MessageBox(NULL, TEXT("ភាសាខ្មែរ"), TEXT("ភាសាខ្មែរ") ,MB_OK); //khmer text box
      //printf("boolsize:%d",sizeof(bool));      
      loading_numerator=0;
      loading_denominator=ROTATED_SPRITE_NUM*7; //(2roach,2toebiter,2ant,extratoebiter


      AddFontResource(L"fonts/unifont-8.0.01.ttf");
      AddFontResource(L"fonts/KhmerOS.ttf");
      if (!DirExists(L"music_tmp")) {
        system("mkdir \"music_tmp/\""); //make new music tmp
      }


      //AddFontResource(L"fonts/KhmerUI.ttf");
      //AddFontResource(L"fonts/KhmerOSsys.ttf");

      //Init screen resolution
        //https://en.wikipedia.org/wiki/List_of_common_display_resolutions
        //https://en.wikipedia.org/wiki/Display_resolution_standards
        //https://en.wikipedia.org/wiki/Ultrawide_formats 
        InitSetRes(0,SCREEN_WIDTH,SCREEN_HEIGHT," * ",L" * ");
        InitSetRes(1,640,480,"VGA",L"VGA");
        InitSetRes(2,640,512,"0.33M3",L"0.33M3");
        InitSetRes(3,768,480,"WVGA",L"WVGA");
        InitSetRes(4,800,480,"WGA",L"WGA");
        InitSetRes(5,800,600,"SVGA",L"SVGA");
        InitSetRes(6,832,624,"0.52M3",L"0.52M3");
        InitSetRes(7,848,480,"W-PAL",L"W-PAL");
        InitSetRes(8,854,480,"FWVGA",L"FWVGA");

        InitSetRes(9,960,540,"qHD",L"qHD");
        InitSetRes(10,960,544,"0.52M9",L"0.52M9");
        InitSetRes(11,960,640,"DVGA",L"DVGA");

        InitSetRes(12,1024,576,"0.59M9",L"0.59M9");
        InitSetRes(13,1024,600,"WSVGA",L"WSVGA");
        InitSetRes(14,1024,640,"0.66MA",L"0.66MA");
        InitSetRes(15,1024,768,"XGA",L"XGA");
        InitSetRes(16,1024,800,"0.82M3",L"0.82M3");
        InitSetRes(17,1024,1024,"1.05M1:1",L"1.05M1:1");

        InitSetRes(18,1080,1200,"1.30M0.9",L"1.30M0.9");

        InitSetRes(19,1120,832,"0.93M3",L"0.93M3");
        InitSetRes(20,1136,640,"0.73M9",L"0.73M9");
        InitSetRes(21,1138,640,"0.73M9",L"0.73M9");
        InitSetRes(22,1152,768,"0.88M2",L"0.88M2");
        InitSetRes(23,1152,864,"XGA+",L"XGA+");
        InitSetRes(24,1152,900,"1.04M32:25",L"1.04M32:25");

        InitSetRes(25,1280,720,"WXGA:M",L"WXGA:M");
        InitSetRes(26,1280,768,"WXGA:A",L"WXGA:A");
        InitSetRes(27,1280,800,"WXGA",L"WXGA");
        InitSetRes(28,1280,854,"1.09M2",L"1.09M2");
        InitSetRes(29,1280,960,"SXGA-",L"SXGA-");
        InitSetRes(30,1280,1024,"SXGA",L"SXGA");

        InitSetRes(31,1334,750,"1.00M9",L"1.00M9");
        InitSetRes(32,1366,768,"WXGA HD",L"WXGA HD");

        InitSetRes(33,1440,900,"WSXGA/WXGA+",L"WSXGA/WXGA+");
        InitSetRes(34,1440,960,"1.38M2",L"1.38M2");
        InitSetRes(35,1400,1050,"SXGA+",L"SXGA+");
        InitSetRes(36,1440,1024,"1.47M5",L"1.47M5");
        InitSetRes(37,1440,1080,"1.56M3",L"1.56M3");
        InitSetRes(38,1440,1440,"2.07M1",L"2.07M1");

        InitSetRes(39,1600,768,"1.23M2.083",L"1.23M2.083");
        InitSetRes(40,1600,900,"HD+",L"HD+");
        InitSetRes(41,1600,1024,"1.64M0",L"1.64M0");
        InitSetRes(42,1680,1050,"WSXGA+",L"WSXGA+");
        InitSetRes(43,1600,1200,"UXGA",L"UXGA");
        InitSetRes(44,1600,1280,"2.05M4",L"2.05M4");

        InitSetRes(45,1776,1000,"1.78M9",L"1.78M9");
        InitSetRes(46,1792,1344,"QWXGA",L"QWXGA");

        InitSetRes(47,1800,1440,"2.59M4",L"2.59M4");
        InitSetRes(48,1856,1392,"2.58M3",L"2.58M3");

        InitSetRes(49,1920,1080,"FHD",L"FHD");
        InitSetRes(50,1920,1280,"2.41M3",L"2.41M3");
        InitSetRes(51,1920,1400,"TXGA",L"TXGA");
        InitSetRes(52,1920,1440,"2.76M3",L"2.76M3");

        InitSetRes(53,2048,1080,"DCI2K",L"DCI2K");
        InitSetRes(54,2048,1152,"WUXGA",L"WUXGA");
        InitSetRes(55,2048,1280,"2.62MA",L"2.62MA");
        InitSetRes(56,2048,1536,"QXGA",L"QXGA");

        InitSetRes(57,2160,1200,"2.59M9",L"2.59M9");
        InitSetRes(58,2160,1440,"3.11M2",L"3.11M2");
        InitSetRes(59,2256,1504,"3.39MA",L"3.39MA");
        InitSetRes(60,2280,1080,"FHD+/2.46M19:9",L"FHD+/2.46M19:9");

        InitSetRes(61,2304,1440,"3.32MA",L"3.32MA");
        InitSetRes(62,2304,1728,"3.98M3",L"3.98M3");
        InitSetRes(63,2340,1080,"2.53M19.5∶9",L"2.53M19.5∶9");
        InitSetRes(64,2400,1080,"2.59M09",L"2.59M09");
        InitSetRes(65,2436,1125,"2.74M2.165",L"2.74M2.165");
        InitSetRes(66,2520,1080,"2.72M1B",L"2.72M1B");
        InitSetRes(67,2538,1080,"2.74M1AD",L"2.74M1AD");
        InitSetRes(68,2560,1080,"UW-FHD",L"UW-FHD");
        InitSetRes(69,2560,1440,"WQHD",L"WQHD");
        InitSetRes(70,2560,1600,"WQXGA",L"WQXGA");
        InitSetRes(71,2560,1664,"4.26M1.538",L"4.26M1.538");
        InitSetRes(72,2560,1700,"4.35M2",L"4.35M2");
        InitSetRes(73,2560,1800,"4.61M1.422",L"4.61M1.422");
        InitSetRes(74,2560,1920,"4.92M3",L"4.92M3");
        InitSetRes(75,2560,2048,"QSXGA",L"QSXGA");

        InitSetRes(76,2576,1450,"3.74M9",L"3.74M9");

        InitSetRes(77,2732,2048,"5.60M3",L"5.60M3");
        InitSetRes(78,2736,1824,"4.99M2",L"4.99M2");

        InitSetRes(79,2880,900,"CWSXGA",L"CWSXGA");
        InitSetRes(80,2880,1200,"WFHD+",L"WFHD+");
        InitSetRes(81,2880,1440,"4.15M2∶1",L"4.15M2∶1");
        InitSetRes(82,2880,1620,"4.67M9",L"4.67M9");
        InitSetRes(83,2880,1800,"5.18MA",L"5.18MA");
        InitSetRes(84,2880,1864,"5.37M17∶11",L"5.37M17");
        InitSetRes(85,2880,1920,"5.53M2",L"5.53M2");
        InitSetRes(86,2800,2100,"QSXGA+",L"QSXGA+");

        InitSetRes(87,2960,1440,"Infinity Display",L"Infinity Display");

        InitSetRes(88,3000,2000,"3K",L"3K");
        InitSetRes(89,3000,2120,"3K",L"3K");
        InitSetRes(90,3024,1964,"5.94M17∶11",L"5.94M17∶11");
        InitSetRes(91,3072,1920,"5.90MA",L"5.90MA");
        InitSetRes(92,3200,2048,"WQSXGA",L"WQSXGA");
        InitSetRes(93,3200,2400,"QUXGA",L"QUXGA");
        InitSetRes(94,3200,1800,"WQXGA+",L"WQXGA+");
        InitSetRes(95,3240,2160,"7.00M2",L"7.00M2");
        InitSetRes(96,3440,1440,"Ultra-Wide QHD",L"Ultra-Wide QHD");
        InitSetRes(97,3456,2234,"7.72M1.547",L"7.72M1.547");
        InitSetRes(98,3840,1600,"UW4K",L"UW4K");
        InitSetRes(99,3840,2160,"4K UHD-1",L"4K UHD-1");
        InitSetRes(100,3840,2400,"WQUXGA",L"WQUXGA");

        InitSetRes(101,4096,2160,"DCI 4K",L"DCI 4K");
        InitSetRes(102,4096,2304,"9.44M9",L"9.44M9");
        InitSetRes(103,4096,3072,"HXGA",L"HXGA");
        InitSetRes(104,4320,1800,"UW4K",L"UW4K");
        InitSetRes(105,4480,2520,"11.29M9",L"11.29M9");
        InitSetRes(106,4500,3000,"13.50M2",L"13.50M2");

        InitSetRes(107,5120,1440,"DQHD",L"DQHD");
        InitSetRes(108,5120,2400,"UW5K+",L"UW5K+");
        InitSetRes(109,5120,2160,"UW5K (WUHD)",L"UW5K (WUHD)");
        InitSetRes(110,5120,2880,"5K",L"5K");
        InitSetRes(111,5120,3200,"WHXGA",L"WHXGA");
        InitSetRes(112,5120,4096,"HSXGA",L"HSXGA");

        InitSetRes(113,6016,3384,"6K",L"6K");
        InitSetRes(114,6400,4096,"WHSXGA",L"WHSXGA");
        InitSetRes(115,6400,4800,"HUXGA",L"HUXGA");
        InitSetRes(116,6480,3240,"-",L"-");
        InitSetRes(117,6880,2880,"UW6K",L"UW6K");

        InitSetRes(118,7680,3200,"UW7K",L"UW7K");
        InitSetRes(119,7680,4320,"8K UHD-2",L"8K UHD-2");
        InitSetRes(120,7680,4800,"WHUXGA",L"WHUXGA");

        InitSetRes(121,8192,4320,"8K Full Format",L"8K Full Format");
        InitSetRes(122,8192,8192,"8K Fulldome",L"8K Fulldome");

        InitSetRes(123,8640,3600,"UW8K",L"UW8K");

        InitSetRes(124,10240,4320,"UW10K",L"UW10K");

        InitSetRes(125,15360,8640,"16K",L"16K");
        InitSetRes(126,16384,8640,"16K Full Format",L"16K Full Format");
      //...
        gblendFunction.BlendOp = AC_SRC_OVER;
        gblendFunction.BlendFlags = 0;
        gblendFunction.SourceConstantAlpha = 32; // Transparency level (0-255)
        gblendFunction.AlphaFormat = 0;



       casbs_i=0;
       chosen_buffer_length_o=buffer_length_arr[casbs_i];
       chosen_buffer_size_o=buffer_size_arr[casbs_i];
       chosen_buffer_length=buffer_length_arr[casbs_i];
       chosen_buffer_size=buffer_size_arr[casbs_i];


      for (int i=1;i<SCREEN_RESOLUTION_NUM;i++) {
        if (SCREEN_WIDTH==RESOLUTION_X[i] && SCREEN_HEIGHT==RESOLUTION_Y[i]) {
          MAX_RESOLUTION_I=i+1;
          break;
        }
      }


      /*WATER_GROUND_NUM=0;
      for (int i=0;i<MAX_GROUND_NUM;i++) {
        rendered_water_ground[i]=-1;
      }*/


      resolution_choose=0;//MAX_RESOLUTION_I-1;
      GR_WIDTH=RESOLUTION_X[resolution_choose];
      GR_HEIGHT=RESOLUTION_Y[resolution_choose];
      flag_resolution_change=TRUE;

      LoadRngTable(L"saves/rngtable.txt");

      Init8BitRGBColorsNoir(rgbColorsNoir);
      Init8BitRGBColorsDefault(rgbColorsDefault);
      Init8BitRGBColorsInvert(rgbColorsInvert,rgbColorsDefault);
      Init8BitRGBPaintDefault(rgbPaint,rgbPaint_i,rgbColorsDefault,TRUE,8);

      CopyReplaceColorPalette(rgbColorsRedToBlue,rgbColorsDefault,199,LTBLUE);

      wav_out_original_volume=VolumeValue(50,1); //set volume
      //waveOutGetVolume(hWaveOut[2],&wav_out_original_volume);
      //waveOutSetVolume(hWaveOut[6],wav_out_original_volume);
     // waveOutOpen(&audioData[0].hWaveOut, WAVE_MAPPER, &audioData[0].awfx_music, (DWORD_PTR)waveOutProc1, (DWORD_PTR)&audioData[0], CALLBACK_FUNCTION);
     // waveOutOpen(&audioData[1].hWaveOut, WAVE_MAPPER, &audioData[1].awfx_music, (DWORD_PTR)waveOutProc1, (DWORD_PTR)&audioData[1], CALLBACK_FUNCTION);

      InitExtractAVIFrames(L"avi/intro_.avi",0);


      //waveOutGetVolume(audioData[0].hWaveOut,&wav_out_original_volume);
      //waveOutSetVolume(audioData[0].hWaveOut,wav_out_original_volume);
      audioData[0].tempo=1.0;
      audioData[0].sps_o=0;
      audioData[0].sps_offset=0;
      audioData[0].volume=0.2;

      //waveOutSetVolume(audioData[1].hWaveOut,wav_out_original_volume);
      audioData[1].tempo=1.0;
      audioData[1].sps_o=0;
      audioData[1].sps_offset=0;
      audioData[1].volume=0.2;

      audioData[0].playing_wav=FALSE;
      audioData[1].playing_wav=FALSE;

      audioData[0].LOW_CUTOFF_FREQUENCY=240;
      audioData[1].LOW_CUTOFF_FREQUENCY=240;

      audioData[0].HIGH_CUTOFF_FREQUENCY=1240;//120;
      audioData[1].HIGH_CUTOFF_FREQUENCY=1240;//120;

      audioData[0].lpf_on=FALSE;
      audioData[1].hpf_on=FALSE;

      audioData[0].high_gain_db=0;
      //audioData[0].mid_gain_db=0;
      audioData[0].low_gain_db=0;

      audioData[1].high_gain_db=0;
      //audioData[1].mid_gain_db=0;
      audioData[1].low_gain_db=0;

      audioData[0].low_eq_on=FALSE;
      audioData[0].high_eq_on=FALSE;

      audioData[1].low_eq_on=FALSE;
      audioData[1].high_eq_on=FALSE;

      BelieveWaveReOpen(0);

      color_chooser.is_choosing_color=FALSE;
      color_chooser.color_id=0;
      color_chooser.color_id_choosing=0;

      //InitMarbles(16);

      //Delete tmp in music
      remove("music_tmp/tmp/tmp.wav");
      rmdir("music_tmp/tmp"); //remove tmp
      remove("music_tmp/tmp2/tmp.wav");
      rmdir("music_tmp/tmp2"); //remove tmp

      /*MessageBox(NULL, TEXT(
"\
ចងចាំអ្នកខ្មែរដែលបាត់បង់ជីវិតក្នុងសង្គ្រាមដែល\n\
អ្នកអាគាំងនិងអ្នកជនជាតិជ្វីហ្វចង់ដណ្ដើមយក\n\
ទន្លេមេគង្គពីសម្តេចឪនរោត្តមសីហនុចាប់ផ្តើមពី\n\
ឆ្នាំ ១៩៥៩, ១៩៦៣ ដល់ ១៩៩៧ \n\
កម្ពុជាក្រោមពីឆ្នាំ ១៨៥៨ ដល់ ១៩៤៩ \n\
និងកម្ពុជាខាងជើង ឆ្នាំ ១៩៤១។\n\n\
\
\
ខ្មែរធ្វើបាន! ជយោកម្ពុជា!\n\n\
\
\
In memory of the Innocent Cambodian Lives lost caused by wars and destabilization efforts by the CIA (1959, 1963-1997).\n\n\nCode is in my Github: https://github.com/Anfinonty/wInsecticide/releases\n\nwInsecticide Version: v-Prel-02-01"), TEXT("អ្នកសម្លាប់សត្វចង្រៃ") ,MB_OK);
//TEXT("អាពីងស៊ីរុយ") ,MB_OK);*/ //ឈ្មោះចាស់

      //load levels in save
      GetSavesInDir(L"saves");

      InitTickFrequency(); //get ticks frequency 
      InitFPS(); //get screen refresh rate

      player.health=20; //init player heath (for cursor)



    //hijri related (*
      int64_t timenow= int64_current_timestamp(); //local timestamp is returned

      printf("\nSeconds Passed Since Jan-1-1970: %d",timenow);
      PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day);
      PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift);

      int num_char='*'; //hijri
      if (solar_month==1 && solar_day>=12 && solar_day<=19) {
        num_char='+';
      }

      printf("\n~:: Solar Hijri ::~ %c\n~:: %02d.%s(%02d).%02d // %s // [%02d:%02d:%02d] ::~\n",
        num_char,
        solar_year,
        solar_months_en_txt[solar_month-1],
        solar_month,
        solar_day,
        solar_days_en_txt[solar_day_of_week],
        solar_hour,
        solar_min,
        solar_sec
      );


      //) |> (|> 0 <|) <| ( @
      printf("\n~:: Lunar Hijri ::~ ");
      if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
        printf("  )");
      } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
        printf(" |)");
      } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
        printf("<|)");
      } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15
        printf("(O)");
      } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
        printf("(|>");
      } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
        printf("(|");
      } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
        printf("(");
      } else { //27, 28, 29, 30
        printf(" @");
      }



      printf("\n~:: %d.%s(%d).%d // %s // [%02d:%02d:%02d] ::~\n",
        lunar_year,
        lunar_months_en_txt[lunar_month-1],
        lunar_month,
        lunar_day,
        lunar_days_en_txt[lunar_day_of_week],
        lunar_hour,
        lunar_min,
        lunar_sec
      );


      //init for sound
      back_to_menu=TRUE;
      in_main_menu=TRUE;
      level_chosen=0;
      stop_playing_song[0]=FALSE;
      stop_playing_song[1]=TRUE;

      //Load Best Scores :D
      //,,,

      //Load Player Sprites
      LoadPlayerSprite.sprite_1 =  LoadRLE8CompressedBitmap(L"sprites/player1.bmp");
      LoadPlayerSprite.sprite_2 = LoadRLE8CompressedBitmap(L"sprites/player2.bmp");
      LoadPlayerSprite.sprite_jump = LoadRLE8CompressedBitmap(L"sprites/player3-1.bmp");
      LoadPlayerSprite.attack_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/player-attack-1.bmp");
      LoadPlayerSprite.attack_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/player-attack-2.bmp");
      LoadPlayerSprite.attack_sprite_3 = LoadRLE8CompressedBitmap(L"sprites/player-attack-3.bmp");
      LoadPlayerSprite.attack_sprite_4 = LoadRLE8CompressedBitmap(L"sprites/player-attack-4.bmp");
      LoadPlayerSprite.block_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/player-block-1.bmp");
      LoadPlayerSprite.block_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/player-block-2.bmp");
      LoadPlayerSprite.block_sprite_3 = LoadRLE8CompressedBitmap(L"sprites/player-block-3.bmp");
      LoadPlayerSprite.spin_sprite = LoadRLE8CompressedBitmap(L"sprites/player-spin.bmp");


      //LoadPlayerSprite.sprite_bee_1=LoadRLE8CompressedBitmap(L"sprites/bee-1.bmp");
      //LoadPlayerSprite.sprite_bee_2=LoadRLE8CompressedBitmap(L"sprites/bee-2.bmp");
      //LoadPlayerSprite.sprite_bee_aero_1=LoadRLE8CompressedBitmap(L"sprites/bee-aero-1.bmp");
      //LoadPlayerSprite.sprite_bee_aero_2=LoadRLE8CompressedBitmap(L"sprites/bee-aero-2.bmp");


      //Load Enemy Sprites
      enemy1_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/enemy1-1.bmp");
      enemy1_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/enemy1-2.bmp");

      enemy2_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/enemy2-1.bmp");
      enemy2_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/enemy2-2.bmp");
      enemy2_sprite_3 = LoadRLE8CompressedBitmap(L"sprites/enemy2-3.bmp");
      enemy2_sprite_4 = LoadRLE8CompressedBitmap(L"sprites/enemy2-4.bmp");

      enemy3_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/enemy3-1.bmp");
      enemy3_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/enemy3-2.bmp");

      enemy4_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/enemy4-1.bmp");
      enemy4_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/enemy4-2.bmp");
      enemy4_sprite_1_0 = LoadRLE8CompressedBitmap(L"sprites/enemy4-1-0.bmp");
      enemy4_sprite_3 = LoadRLE8CompressedBitmap(L"sprites/enemy4-3.bmp");
      enemy4_sprite_4 = LoadRLE8CompressedBitmap(L"sprites/enemy4-4.bmp");

      enemy5_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/enemy5-1.bmp");
      enemy5_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/enemy5-2.bmp");

      enemy6_sprite_1 = LoadRLE8CompressedBitmap(L"sprites/enemy6-1.bmp");
      enemy6_sprite_2 = LoadRLE8CompressedBitmap(L"sprites/enemy6-2.bmp");

      //water textures 0 to 7
      //SetTexturePalette(24,waterPalette); //24 is dkblue
      //SetTexturePalette(166,waterPalette); //lt green water
      //SetTexturePalette(16*2+4,waterPalette); //dkgreen water
      //SetTexturePalette(166,waterPalette);
      SetTexturePalette(16*9+8,waterPalette); //ltblue water (default perfered)

      for (int i=0;i<9;i++) {
        wchar_t fname[48];
        swprintf(fname,48,L"sprites/textures/water_texture%d.bmp",i);        
        //HBITMAP tmp_bitmap = (HBITMAP) LoadImageW(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);//LoadRLE8CompressedBitmap(fname);
        //texture_water[i] =  CopyCrunchyBitmap(tmp_bitmap,SRCCOPY);
        //DeleteObject(tmp_bitmap);

        //tricky working with 32-bit like sprites, expection'
        texture_water[i] =(HBITMAP) LoadImageW(NULL, fname, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
      }


      dkrdkgray_shadow_tile = LoadRLE8CompressedBitmap(L"sprites/shadow_dkrdkgray.bmp");//(HBITMAP) LoadImageW(NULL, L"sprites/shadow_dkrdkgray.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      ltgray_shadow_tile = LoadRLE8CompressedBitmap(L"sprites/shadow_ltgray.bmp");//(HBITMAP) LoadImageW(NULL, L"sprites/shadow_ltgray.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      //Load mouse cursor sprite
      //player cursor
      for (int i=0;i<16;i++) { //open: 0,,3; 4..7;   closed:  8..11; 12..15
        wchar_t fname[32];
        swprintf(fname,32,L"sprites/player_cursor%d.bmp",i);
        player_cursor[i]=LoadRLE8CompressedBitmap(fname);
            //LoadImageW(NULL,fname,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        player_cursor_cache[i]=CopyCrunchyBitmap(player_cursor[i],SRCCOPY);
        ReplaceBitmapColor2(player_cursor_cache[i],LTGREEN,BLACK,8,LTGREEN);
        GenerateDrawSprite(&draw_player_cursor[i],player_cursor_cache[i]);
      }


      /*for (int i=0;i<5;i++) {
        wchar_t fname[32];
        swprintf(fname,32,L"sprites/player_cursorbee%d.bmp",i);
        player_cursorbee[i]=LoadRLE8CompressedBitmap(fname);
            //LoadImageW(NULL,fname,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
        player_cursorbee_cache[i]=CopyCrunchyBitmap(player_cursorbee[i],SRCCOPY);
        ReplaceBitmapColor2(player_cursorbee_cache[i],LTGREEN,BLACK,8,LTGREEN);
        GenerateDrawSprite(&draw_player_cursorbee[i],player_cursorbee_cache[i]);
      }*/

      //Load snowflake
      snowflake_sprite=LoadRLE8CompressedBitmap(L"sprites/snowflake0.bmp");
      snowflake_sprite_cache=CopyCrunchyBitmap(snowflake_sprite,SRCCOPY);
      ReplaceBitmapColor(snowflake_sprite_cache,LTGREEN,BLACK);
      GenerateDrawSprite(&draw_snowflake_sprite,snowflake_sprite_cache);


      //Load mooon sprites
      wchar_t moon_sprite_name[48];
      wchar_t moon_cartoony_sprite_name[48];

      //Load moon sprite based on lunar day
      //lunar_day=1; //moon debug
      double lunar_angle=0;
      double mirror_lunar_angle=0;
      if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-1.bmp");
        lunar_angle=-M_PI_4-M_PI_4/2;
        mirror_lunar_angle=M_PI_4+M_PI_4/2;
        current_moon_phase_id=0;
      } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-8.bmp");
        lunar_angle=-M_PI_4;
        mirror_lunar_angle=M_PI_4;
        current_moon_phase_id=1;
      } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-11.bmp");
        lunar_angle=-M_PI_4+M_PI_4/2;
        mirror_lunar_angle=M_PI_4-M_PI_4/2;
        current_moon_phase_id=2;
      } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-14.bmp");
        lunar_angle=0;
        mirror_lunar_angle=0;
        current_moon_phase_id=3;
      } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-16.bmp");
        lunar_angle=M_PI_4-M_PI_4/2;
        mirror_lunar_angle=-M_PI_4+M_PI_4/2;
        current_moon_phase_id=4;
      } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-21.bmp");
        lunar_angle=M_PI_4;
        mirror_lunar_angle=-M_PI_4;
        current_moon_phase_id=5;
      } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-26.bmp");
        lunar_angle=M_PI_4+M_PI_4/2;
        mirror_lunar_angle=-M_PI_4-M_PI_4/2;
        current_moon_phase_id=6;
      } else { // new moon
        swprintf(moon_cartoony_sprite_name,48,L"sprites/moon-cartoon-28.bmp");
        current_moon_phase_id=7;
      }

      //generate cartoon moon in calendar
      moon_cartoon_sprite=LoadRLE8CompressedBitmap(moon_cartoony_sprite_name);
      moon_cartoon_sprite_cache=GetRotated8BitBitmap(moon_cartoon_sprite,lunar_angle,LTGREEN);
      ReplaceBitmapColor(moon_cartoon_sprite_cache,LTGREEN,BLACK);
      GenerateDrawSprite(&draw_moon_cartoon_sprite,moon_cartoon_sprite_cache);

      //generate backgorund moons
      for (int i=0;i<8;i++) {
        switch (i) {
          case 0:
            swprintf(moon_sprite_name,48,L"sprites/moon-1.bmp");
            lunar_angle=-M_PI_4-M_PI_4/2;
            mirror_lunar_angle=M_PI_4+M_PI_4/2;
            break;
          case 1:
            swprintf(moon_sprite_name,48,L"sprites/moon-8.bmp");
            lunar_angle=-M_PI_4;
            mirror_lunar_angle=M_PI_4;
            break;
          case 2:
            swprintf(moon_sprite_name,48,L"sprites/moon-11.bmp");
            lunar_angle=-M_PI_4+M_PI_4/2;
            mirror_lunar_angle=M_PI_4-M_PI_4/2;
            break;
          case 3:
            swprintf(moon_sprite_name,48,L"sprites/moon-14.bmp");
            lunar_angle=mirror_lunar_angle=0;
            break;
          case 4:
            swprintf(moon_sprite_name,48,L"sprites/moon-16.bmp");
            lunar_angle=M_PI_4-M_PI_4/2;
            mirror_lunar_angle=-M_PI_4+M_PI_4/2;
            break;
          case 5:
            swprintf(moon_sprite_name,48,L"sprites/moon-21.bmp");
            lunar_angle=M_PI_4;
            mirror_lunar_angle=-M_PI_4;
            break;
          case 6:
            swprintf(moon_sprite_name,48,L"sprites/moon-26.bmp");
            lunar_angle=M_PI_4+M_PI_4/2;
            mirror_lunar_angle=-M_PI_4-M_PI_4/2;
            break;
          case 7:
            swprintf(moon_sprite_name,48,L"sprites/moon-28.bmp");
            lunar_angle=mirror_lunar_angle=0;
            break;
        }
        moon_sprite[i]=LoadRLE8CompressedBitmap(moon_sprite_name);
        moon_sprite_cache[i]=GetRotated8BitBitmap(moon_sprite[i],lunar_angle,LTGREEN);
        mirror_moon_sprite_cache[i]=GetRotated8BitBitmap(moon_sprite[i],mirror_lunar_angle,LTGREEN);
        ReplaceBitmapColor(moon_sprite_cache[i],LTGREEN,BLACK);
        ReplaceBitmapColor(mirror_moon_sprite_cache[i],LTGREEN,BLACK);
      
        GenerateDrawSprite(&draw_moon_sprite[i],moon_sprite_cache[i]);
        GenerateDrawSprite(&draw_mirror_moon_sprite[i],mirror_moon_sprite_cache[i]);
      }

      }
      intro_msg = LoadRLE8CompressedBitmap(L"sprites/intro_msg.bmp");
      intro_msg_mask = CreateBitmapMask(intro_msg,BLACK,NULL);


      for (int k=0;k<PLATFORM_TEXTURES_NUM;k++) {
        wchar_t texture_name[48];
        swprintf(texture_name,48,L"sprites/textures/texture%d.bmp",k);
        LoadedPlatformTextures[k]=LoadRLE8CompressedBitmap(texture_name);
      }
    
      kh_pressanykey = (HBITMAP) LoadImageW(NULL, L"sprites/kh_pressanykey.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      kh_pressanykey_mask = CreateBitmapMask(kh_pressanykey,LTGREEN,NULL);

      HBITMAP tmp_title_sprite= (HBITMAP) LoadImageW(NULL, L"sprites/title.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      HBITMAP tmp_small_title_sprite= (HBITMAP) LoadImageW(NULL, L"sprites/title_small.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      title_sprite[0] = CopyCrunchyBitmap(tmp_title_sprite,SRCCOPY);
      title_small_sprite[0] = CopyCrunchyBitmap(tmp_small_title_sprite,SRCCOPY);
      title_sprite_mask[0]= CreateBitmapMask(title_sprite[0],LTGREEN,NULL);
      title_small_sprite_mask[0]=CreateBitmapMask(title_small_sprite[0],LTGREEN,NULL);
      DeleteObject(tmp_title_sprite);
      DeleteObject(tmp_small_title_sprite);

      tmp_title_sprite= (HBITMAP) LoadImageW(NULL, L"sprites/title_english.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      tmp_small_title_sprite= (HBITMAP) LoadImageW(NULL, L"sprites/title_english_small.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      title_sprite[1] = CopyCrunchyBitmap(tmp_title_sprite,SRCCOPY);
      title_small_sprite[1] = CopyCrunchyBitmap(tmp_small_title_sprite,SRCCOPY);
      title_sprite_mask[1]=CreateBitmapMask(title_sprite[1],LTGREEN,NULL);
      title_small_sprite_mask[1]=CreateBitmapMask(title_small_sprite[1],LTGREEN,NULL);
      DeleteObject(tmp_title_sprite);
      DeleteObject(tmp_small_title_sprite);


      //KHMER
      for (int i=0;i<4;i++) { //main menu
        wchar_t mm0khtxt[32];
        swprintf(mm0khtxt,32,L"sprites/khmai/mm0kh_%d.bmp",i);
        mm0_kh[i]=(HBITMAP) LoadImageW(NULL, mm0khtxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        mm0_kh_white[i]=RotateSprite(NULL, mm0_kh[i],0,LTGREEN,BLACK,WHITE,-1);
        mm0_kh_green[i]=RotateSprite(NULL, mm0_kh[i],0,LTGREEN,BLACK,LLTGREEN,-1);
        mm0_kh_mask[i]= CreateBitmapMask(mm0_kh[i],LTGREEN,NULL);
      }

      for (int i=0;i<2;i++) { //hijiri
        wchar_t mm0khtxth[32];
        swprintf(mm0khtxth,32,L"sprites/khmai/mm0kh_4_%d.bmp",i);
        mm0_kh_hijri[i]=(HBITMAP) LoadImageW(NULL, mm0khtxth, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        mm0_kh_hijri_mask[i]= CreateBitmapMask(mm0_kh_hijri[i],LTGREEN,NULL);
      }


      for (int i=0;i<16;i++) { //options
        wchar_t mm2khtxt[32];
        swprintf(mm2khtxt,32,L"sprites/khmai/mm2kh_%d.bmp",i);
        mm2_kh[i]=(HBITMAP) LoadImageW(NULL, mm2khtxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        mm2_kh_white[i]=RotateSprite(NULL, mm2_kh[i],0,LTGREEN,BLACK,WHITE,-1);
        mm2_kh_green[i]=RotateSprite(NULL, mm2_kh[i],0,LTGREEN,BLACK,LLTGREEN,-1);
        mm2_kh_mask[i]= CreateBitmapMask(mm2_kh[i],LTGREEN,NULL);
      }


      for (int i=0;i<4;i++) { //ingame
        wchar_t ga0khhardtxt[32];
        swprintf(ga0khhardtxt,32,L"sprites/khmai/kmaigametxth%d.bmp",i);
        ga0_khhard[i]=(HBITMAP) LoadImageW(NULL, ga0khhardtxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        ga0_khhard_mask[i]= CreateBitmapMask(ga0_khhard[i],BLACK,NULL);
      }

      for (int i=0;i<5;i++) { //ingame
        wchar_t ga0khtxt[32];
        swprintf(ga0khtxt,32,L"sprites/khmai/kmaigametxt%d.bmp",i);
        ga0_kh[i]=(HBITMAP) LoadImageW(NULL, ga0khtxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        ga0_kh_mask[i]= CreateBitmapMask(ga0_kh[i],/*LTBLUE*/BLACK,NULL);
      }

      for (int i=0;i<2;i++) {//khmer bool
        wchar_t khbooltxt[32];
        swprintf(khbooltxt,32,L"sprites/khmai/kh_bool%d.bmp",i);
        kh_bool[i]=(HBITMAP) LoadImageW(NULL, khbooltxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        kh_bool_white[i]=RotateSprite(NULL, kh_bool[i],0,LTGREEN,BLACK,WHITE,-1);
        kh_bool_green[i]=RotateSprite(NULL, kh_bool[i],0,LTGREEN,BLACK,LLTGREEN,-1);
        kh_bool_mask[i]= CreateBitmapMask(kh_bool[i],LTGREEN,NULL);
      }


      for (int i=0;i<2;i++) {//khmer difficulty
        wchar_t khdiffictxt[32];
        swprintf(khdiffictxt,32,L"sprites/khmai/kh_difficult%d.bmp",i);
        kh_difficulty[i]=(HBITMAP) LoadImageW(NULL, khdiffictxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        kh_difficulty_white[i]=RotateSprite(NULL, kh_difficulty[i],0,LTGREEN,BLACK,WHITE,-1);
        kh_difficulty_green[i]=RotateSprite(NULL, kh_difficulty[i],0,LTGREEN,BLACK,LLTGREEN,-1);
        kh_difficulty_mask[i]= CreateBitmapMask(kh_difficulty[i],LTGREEN,NULL);
      }

      kh_cornmid= LoadImageW(NULL, L"sprites/khmai/kh_cornmid.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      kh_cornmid_white=RotateSprite(NULL, kh_cornmid,0,LTGREEN,BLACK,WHITE,-1);
      kh_cornmid_green=RotateSprite(NULL, kh_cornmid,0,LTGREEN,BLACK,LLTGREEN,-1);
      kh_cornmid_mask=CreateBitmapMask(kh_cornmid,LTGREEN,NULL);


      kh_goback= LoadImageW(NULL, L"sprites/khmai/kh_back.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      kh_goback_mask=CreateBitmapMask(kh_goback,LTGREEN,NULL);

      prelude=TRUE;
      //flag_prelude=TRUE;

      //fullscreen
      //ShowWindow(hwnd,SW_SHOWMAXIMIZED);

      //=====Load Audio=====
    //https://stackoverflow.com/questions/32320825/load-wavs-into-memory-then-play-sounds-asynchronously-using-win32-api
    //https://stackoverflow.com/questions/65851460/playing-sound-from-a-memory-buffer-in-pure-winapi
    //https://stackoverflow.com/questions/2457482/processing-an-audio-wav-file-with-c
    //https://stackoverflow.com/questions/8754111/how-to-read-the-data-in-a-wav-file-to-an-array
      //if (load_sound && level_loaded) {

       loadSoundEffect(&spamSoundEffect[0],L"snd/_timebreaker_start.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[1],L"snd/_timebreaker_stop.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[2],L"snd/_enemy_hurt.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[3],L"snd/_player_block.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[4],L"snd/_player_block_perfect.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[5],L"snd/_player_hurt.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[6],L"snd/_enemy_block.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[7],L"snd/_clang.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[8],L"snd/_powerup.wav",FALSE);


       loadSoundEffect(&keySoundEffect[0],L"snd/_mm_playlevel.wav",FALSE); //Enter Sound Effect (Sometimes) [0]
       loadSoundEffect(&keySoundEffect[1],L"snd/_mm_keyupdown.wav",FALSE); //Key Up Down Sound Effect [1]
       loadSoundEffect(&keySoundEffect[2],L"snd/_mm_false.wav",FALSE); //False Sound Effect --> [2]
       loadSoundEffect(&keySoundEffect[3],L"snd/_mm_true.wav",FALSE); //True Sound Effect --> [3]
       loadSoundEffect(&keySoundEffect[4],L"snd/_mm_esc.wav",FALSE); //ESC Sound Effect --> [4]
       loadSoundEffect(&keySoundEffect[5],L"snd/_mm_paintscroll.wav",FALSE); //Paint Sound Effect --> [5]
//       loadSoundEffect(&keySoundEffect[6],L"snd/FE_MB_16.wav",FALSE); //Paint Confirm Sound Effect --> [6]
       loadSoundEffect(&keySoundEffect[6],L"snd/_mm_paintconfirm.wav",FALSE); //Paint Confirm Sound Effect --> [6]

       loadSoundEffect(&channelSoundEffect[0],L"snd/_player_death.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[1],L"snd/_enemy_death.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[2],L"snd/_knife_throw.wav",TRUE);
       //loadSoundEffect(&channelSoundEffect[3],L"snd/_rain.wav",TRUE); //deprecated
       //loadSoundEffect(&channelSoundEffect[4],L"snd/_rain.wav",TRUE); //deprecated
       loadSoundEffect(&channelSoundEffect[5],L"snd/_shotgun_fire.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[6],L"snd/_sniper.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[7],L"snd/_shotgun_reload.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[8],L"snd/_pistol.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[9],L"snd/_load_knife.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[10],L"snd/_load_3_knife.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[11],L"snd/_load_pistol.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[12],L"snd/_shotgun_empty.wav",TRUE);

       //for wav sound effects
       /*wfx_wav_sfx1.wFormatTag = WAVE_FORMAT_PCM;
       wfx_wav_sfx1.nChannels = channelSoundEffect[1].wav_header->NumOfChan;
       wfx_wav_sfx1.nSamplesPerSec = channelSoundEffect[1].wav_header->SamplesPerSec;
       wfx_wav_sfx1.nAvgBytesPerSec = channelSoundEffect[1].wav_header->bytesPerSec;
       wfx_wav_sfx1.nBlockAlign = channelSoundEffect[1].wav_header->blockAlign;
       wfx_wav_sfx1.wBitsPerSample = channelSoundEffect[1].wav_header->bitsPerSample;
       wfx_wav_sfx1.cbSize = 0;*/

       /*waveOutOpen(&hWaveOut[0], WAVE_MAPPER, &wfx_wav_sfx1, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[0], &whdr[0], sizeof(WAVEHDR));*/
       /*printf("%d\n", wfx_wav_sfx2.nChannels);
       printf("%d\n", wfx_wav_sfx2.nSamplesPerSec);
       printf("%d\n", wfx_wav_sfx2.nAvgBytesPerSec);
       printf("%d\n", wfx_wav_sfx2.nBlockAlign);
       printf("%d\n", wfx_wav_sfx2.wBitsPerSample);*/



       //Enemy death sound
       waveOutOpen(&hWaveOut[0], WAVE_MAPPER, &wfx_wav_sfx, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[0], &whdr[0], sizeof(WAVEHDR));

       //player death audio
       waveOutOpen(&hWaveOut[1], WAVE_MAPPER, &wfx_wav_sfx, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[1], &whdr[1], sizeof(WAVEHDR));

       //knife throw ,shotgun , sniper 11052hz
       waveOutOpen(&hWaveOut[3], WAVE_MAPPER, &wfx_wav_sfx, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[3], &whdr[3], sizeof(WAVEHDR));


       //For Raining -> unique sfx
       /*wfx_wav_sfx_rain.wFormatTag = WAVE_FORMAT_PCM;
       wfx_wav_sfx_rain.nChannels = channelSoundEffect[3].wav_header->NumOfChan;
       wfx_wav_sfx_rain.nSamplesPerSec = channelSoundEffect[3].wav_header->SamplesPerSec;
       wfx_wav_sfx_rain.nAvgBytesPerSec = channelSoundEffect[3].wav_header->bytesPerSec;
       wfx_wav_sfx_rain.nBlockAlign = channelSoundEffect[3].wav_header->blockAlign;
       wfx_wav_sfx_rain.wBitsPerSample = channelSoundEffect[3].wav_header->bitsPerSample;
       wfx_wav_sfx_rain.cbSize = 0;*/


       //waveOutOpen(&hWaveOut[4], WAVE_MAPPER, &wfx_wav_sfx_rain, 0, 0, CALLBACK_NULL);
       //waveOutPrepareHeader(hWaveOut[4], &whdr[4], sizeof(WAVEHDR));


       //reload
       waveOutOpen(&hWaveOut[5], WAVE_MAPPER, &wfx_wav_sfx, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[5], &whdr[5], sizeof(WAVEHDR));

       return 0;
       break;
    //Tasks to perform on exit
    case WM_DESTROY:
      if (audioData[0].music_file) {
        fclose(audioData[0].music_file);
      }
      if (audioData[1].music_file) {
        fclose(audioData[1].music_file);
      }
      remove("music_tmp/tmp/tmp.wav");
      rmdir("music_tmp/tmp"); //remove tmp
      remove("music_tmp/tmp2/tmp.wav");
      rmdir("music_tmp/tmp2"); //remove tmp
      PostQuitMessage(0);
      return 0;
      break;


    //default:
     //break;
  }
  return DefWindowProcW(hwnd, msg, wParam, lParam);
}






int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
  //Window Class
  WNDCLASSW wc = {0};
  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS; 
  wc.lpszClassName = L"DrawIt";
  wc.hInstance     = hInstance;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  wc.hIcon = LoadIcon(hInstance, L"id");

  RegisterClassW(&wc);


  //https://cplusplus.com/forum/beginner/9908/
  //create window
  HWND hwnd = CreateWindowW(wc.lpszClassName,
//                L"អាពីងស៊ីរុយ - wInsecticide",
//                L"អ្នករាបចង្រៃ - wInsecticide",
                L"អ្នកសម្លាប់សត្វចង្រៃ - wInsecticide",
                WS_POPUP | WS_BORDER | WS_OVERLAPPEDWINDOW | WS_VISIBLE | CW_USEDEFAULT| CW_USEDEFAULT /*| WS_MINIMIZE*/,
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                0,
                0,
                NULL,
                NULL,
                hInstance, 
                NULL);
  //HMONITOR hmon = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
  //SetWindowLong(hwnd, GWL_STYLE, 0);
  //ShowWindow(hwnd, SW_SHOW);
  //https://www.codeproject.com/Questions/441008/Hide-TaskBar-in-C-through-Win32
  //Make game un fast when level is run (simulates focusing tab)
  //https://batchloaf.wordpress.com/2012/10/18/simulating-a-ctrl-v-keystroke-in-win32-c-or-c-using-sendinput/





   //Register Joystick on Start :^)
   RAWINPUTDEVICE rid;
   rid.usUsagePage = 0x01; // Generic desktop controls
   rid.usUsage = 0x04;     // Joystick
   rid.dwFlags = 0;
   rid.hwndTarget = hwnd;

   if (!RegisterRawInputDevices(&rid, 1, sizeof(rid))) {
      // Handle registration error
      printf("Failed to register raw input device\n");
      //return -1;
   }
   InitCController();


  //this looks unneccessary but for some reason its required for high fps :/
  Sleep(100);
  INPUT ip;
  ip.type = INPUT_KEYBOARD;
  ip.ki.wScan = 0;
  ip.ki.time = 0;
  ip.ki.dwExtraInfo = 0;
 
  for (int i=0;i<2;i++) {
  // Press the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Press the ESCAPE key
    ip.ki.wVk = VK_ESCAPE;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    // Release the escape key
    ip.ki.wVk = VK_ESCAPE;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    // Release the "Ctrl" key
    ip.ki.wVk = VK_CONTROL;
    ip.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));

    Sleep(200);
  }

  SetForegroundWindow(hwnd);
  HANDLE thread1=CreateThread(NULL,0,AnimateTask01,NULL,0,NULL); //Spawm Game Logic Thread
  HANDLE thread2=CreateThread(NULL,0,AnimateTask02,NULL,0,NULL); //Spawm Game Logic Thread
  HANDLE thread3=CreateThread(NULL,0,SoundTask,NULL,0,NULL); //Spawn Song Player Thread
  HANDLE thread4=CreateThread(NULL,0,SpamSoundTask,NULL,0,NULL);


  //HANDLE thread4=CreateThread(NULL,0,AnimateAVI,NULL,0,NULL); //Spawm Game Logic Thread
  //HANDLE thread5=CreateThread(NULL,0,PlayMemSnd3,NULL,0,NULL); //Spawn music buffering1
  //HANDLE thread6=CreateThread(NULL,0,PlayMemSnd7,NULL,0,NULL); //Spawn music buffering2

  //SetTimer(hwnd, 1, frameDelays[currentFrame] , NULL);
  /*LONG lStyle0 = GetWindowLong(hwnd, GWL_STYLE);
  lStyle0 |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
  SetWindowLong(hwnd, GWL_STYLE, lStyle0);
  SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,RESOLUTION_X[0],RESOLUTION_Y[0], SWP_FRAMECHANGED);*/

  //Sleep(1000);

  //LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
  //lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU); //borderless mode
  //SetWindowLong(hwnd, GWL_STYLE, lStyle);
  //SetWindowPos(hwnd,HWND_TOPMOST,0,0,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
  SetWindowPos(hwnd,HWND_TOPMOST,SCREEN_WIDTH/2-640/2,SCREEN_HEIGHT/2-440/2,640,440, SWP_FRAMECHANGED);

  MSG msg;
  while (true) {
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
      if (msg.message == WM_QUIT) break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  //In case WM_DESTROY doesnt work
  //HWND hShellWnd = FindWindowA("Shell_TrayWnd", NULL);
  //ShowWindow(hShellWnd, SW_SHOW);

  remove("music_tmp/tmp/tmp.wav");
  remove("music_tmp/tmp2/tmp.wav");
  rmdir("music_tmp/tmp"); //remove tmp, manually because C is like that
  waveOutSetVolume(hWaveOut[2],wav_out_original_volume);
  waveOutSetVolume(hWaveOut[6],wav_out_original_volume);
  if (level_loaded) {
    SaveOptions();
  }
  return (int) msg.wParam;
}




