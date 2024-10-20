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

//for .avi
#include <vfw.h>

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

//#include <mmsystem.h>
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
bool load_sound=FALSE;
bool back_to_menu=FALSE;
bool clean_up_sound=FALSE;
bool run_after_once=FALSE;

//game options
bool yes_unifont=FALSE;
bool game_cam_shake=TRUE;
bool game_audio=TRUE;



//game state
bool hide_taskbar=FALSE;
bool in_main_menu=TRUE;
bool level_loaded=FALSE;
bool game_over=FALSE;
bool show_fps=FALSE;
bool in_map_editor=FALSE;

//bool alloc_enemy_once=TRUE;

//to be used to load a level
wchar_t save_level[128];
//lvl name type in or editing


//Create Level Menu
//Typing level name attributes
wchar_t typing_lvl_name[16];//=//{' ',L'H',L'I'};
int typing_lvl_name_pos=0;
int set_ground_amount=10;
int set_enemy_amount=1;
int set_map_width_amount=640;
int set_map_height_amount=480;
int create_lvl_option_choose=0;




//Game System Values
int windowx=0;
int windowy=0;

int GR_WIDTH;
int GR_HEIGHT;
int OLD_GR_WIDTH;
int OLD_GR_HEIGHT;
int RESOLUTION_X[3]={640,800,0};
int RESOLUTION_Y[3]={480,600,0};
int level_chosen=0;
int main_menu_chosen=-1; //options for main menu
int select_main_menu=0;
int option_choose=0;
int resolution_choose=0; //640,480, 800,600, SCREEN_WIDTH,SCREEN_HEIGHT

int call_help_timer=0;



bool lvl_has_song=FALSE;
wchar_t src_music_dir[64];


int enemy_kills=0;
//int FPS = 60;
int int_best_score=0; //to store to write
int frame_tick=-10;
int FPS = 35; //minimum FPS, otherwise run according to screen refresh rate


//Player visuals Options Values
int player_color=0;
int old_player_color=0;
int player_load_color=0;

int player_iris_color=4;
int old_player_iris_color=4;
int player_load_iris_color=4;

int player_pupil_color=12;
int old_player_pupil_color=12;
int player_load_pupil_color=12;

int player_bullet_color=0;


int showoff=0;
int saved_showoff=0;

//double, game state system values
long long game_timer=0;
double double_best_score=0;



//double, game options
double time_begin=0;
double game_volume=1.0;
double old_game_volume=1.0;


bool shadows=FALSE;
bool raining=FALSE;
int rain_duration=0;
double rain_grad_rise=1,rain_grad_run=1;



bool is_khmer=TRUE;

#define DEFAULT_PLAYER_SPEED			1

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

#define RENDER_DIST      10//20
#define RDGRID_NUM       RENDER_DIST*RENDER_DIST


#define RAIN_NUM    50
#define SHOOT_BULLET_NUM    5000
#define BULLET_NUM	SHOOT_BULLET_NUM+RAIN_NUM

#define ENEMY_BULLET_NUM            1000
#define MAX_BULLET_PER_FIRE         10




#define MAX_MAP_NODE_NUM (640*20)/NODE_SIZE * (480*20)/NODE_SIZE //MAX_WIDTH/NODE_SIZE * MAX_HEIGHT/NODE_SIZE
#define MAX_VGRID_NUM   4800 //(640/160)*20 * (480/160)*20
//#define MAX_GRID_NUM    4800
#define MAX_GROUND_NUM  2000
#define MAX_ENEMY_NUM   200//50



#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			30

#define DEFAULT_PLAYER_HEALTH			20
#define DEFAULT_PLAYER_BLOCK_HEALTH_MAX 100//20
#define DEFAULT_PLAYER_JUMP_HEIGHT 		85//100
#define DEFAULT_PLAYER_ATTACK_STRENGTH  	1
#define DEFAULT_PLAYER_KNOCKBACK_STRENGTH	50

#define DEFAULT_PLAYER_BUILD_RANGE		80//100//12
#define MAX_WEB_LENGTH		400//200//100//12
//#define DEFAULT_PLAYER_SHORT_BUILD_RANGE	10
#define DEFAULT_PLAYER_WEB_HEALTH		150

#define DEFAULT_PLAYER_WEB_NUM      20
#define MAX_WEB_NUM     200//100

#define DEFAULT_PLAYER_SPEED			1

#define DEFAULT_PLAYER_TIME_BREAKER_MAX	20 //10 seconds to charge
#define DEFAULT_PLAYER_TIME_BREAKER_COOLDOWN_MAX   700 //5 seconds after usage
#define DEFAULT_PLAYER_TIME_BREAKER_RECHARGE_MAX	200 //1 seconds
#define DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX	22 //45

#define HP_SHOW_TIMER_NUM   450

#define PLAYER_LOW_HEALTH   3
//#define PLAYER_BULLET_NUM 32
#define PLAYER_BULLET_NUM 24//16
#define PLAYER_FLING_WEB_NUM    32

#define GAME_OPTIONS_NUM    12
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


void RemoveFolderRecursive(const wchar_t* dirname)
{
  _WDIR *d;
  struct _wdirent *dir;
  d = _wopendir(dirname);
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for 
        RemoveFolderRecursive(indir);
      } else {
        _wremove(indir);
      }
    }
    _wrmdir(dirname);
  }
}


DWORD WINAPI AnimateTask01(LPVOID lpArg) {
  while (TRUE) {
    if (!in_main_menu) { //In Game
      if (level_loaded) {
        PlayerAct();
        for (int i=0;i<ENEMY_NUM;i++) {
          EnemyAct(i);
        }
        if (raining) {
          RainAct();
          if (!player.time_breaker) {
            ScreenRainDropAct();
          }
        }
        Sleep(player.sleep_timer);
      } else {
        Sleep(1000);
      }
    } else if (in_map_editor) {
      MapEditorAct();
      Sleep(6);
    } else {
      Sleep(1000);
    }
  }
}


DWORD WINAPI AnimateTask02(LPVOID lpArg) { //FPS counter
  while (TRUE) {
    saved_showoff=showoff;
    showoff=0;
    Sleep(1000);
  }
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;

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


    //Mouse Movement
    case  WM_MOUSEMOVE: //https://stackoverflow.com/questions/22039413/moving-the-mouse-blocks-wm-timer-and-wm-paint
      if (!IsIconic(hwnd)) //no action when minimized
      {
        POINT point;
        if (GetCursorPos(&point)) {
          mouse_x=point.x-windowx;
          mouse_y=point.y-windowy;
        }
        UpdateWindow(hwnd);
      }
      break;

    //Various Keypress down
    case WM_KEYDOWN:
    {
      //Global keydown press
      if (main_menu_chosen!=2) {
        GlobalKeypressDown(wParam);
      }

      //Key Down Presses depending on game state
      if (!in_main_menu) {
        GameKeypressDown(wParam);
      } else { //Main menu
        switch (main_menu_chosen) {
           case -1:
             MinusOneMenuKeypressDown(wParam);
             break;

           case 0:
             ZeroMenuKeypressDown(hwnd,hdc,wParam);
             break;

           //Player options in main menu
           case 1:
             OptionsKeypressDown(hwnd, wParam);
             break;

           case 2:
             TwoMenuKeypressDown(wParam);
             break;

           case 3:
             ThreeMenuKeypressDown(wParam);
             break;


           case 4:
             MapEditorKeypressDown(wParam);
             break;

        } //end of switch statement for menu chosen
      } //end of menu chosen if else
      break; //Break WM_KEYDOWN;
    } 


    //Key Release
    case WM_KEYUP:
    {
  //GLOBAL wParam Release Key
  //    printf("%d",wParam);
      if (main_menu_chosen!=2) {
        GlobalKeypressUp(hwnd,wParam);
      }

      if (!in_main_menu) { //Gaming
        GameKeypressUp(wParam);
      } else { 
        switch (main_menu_chosen) {
          case -1:
            MinusOneMenuKeypressUp(wParam);
            break;

          case 0: //Level select keyup
            ZeroMenuKeypressUp(hwnd,hdc,wParam);
            break;

          case 1:
            OptionsKeypressUp(wParam);
            break;

          case 2:            
            TwoMenuKeypressUp(wParam);
            break;

           case 3:
             ThreeMenuKeypressUp(wParam);
             break;

           case 4:
             MapEditorKeypressUp(wParam);
             break;
          }
          break; //end of main menu chosen switch

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
                swprintf(remove_folder_name,48,L"saves/%s",level_names[level_chosen]);
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
        //UpdateFrame(hwnd);
        HBITMAP screen;//,screen2;
        PAINTSTRUCT ps;
        hdc=BeginPaint(hwnd, &ps);
        RECT rect;
        if(GetWindowRect(hwnd, &rect))
        {
          GR_WIDTH = rect.right - rect.left;
          GR_HEIGHT = rect.bottom - rect.top;        
          windowx = rect.left;
          windowy = rect.top;
        }

        if (flag_adjust_audio) {
          freeSoundEffectCache(&keySoundEffectCache[2]);
          adjustSFXVolume(&keySoundEffectCache[2],&keySoundEffect[2],game_volume,FALSE);
          flag_adjust_audio=FALSE;
        }


        if (GR_WIDTH!=OLD_GR_WIDTH || GR_HEIGHT!=OLD_GR_HEIGHT) {
          if (!in_map_editor) {
            InitPlayerCamera();
            player.cam_x=0;
            player.cam_y=0;
            CameraInit(player.x,player.y); //idk scaling is weird for sprite
          }
          OLD_GR_WIDTH = GR_WIDTH;
          OLD_GR_HEIGHT = GR_HEIGHT;
          //Load Map Background sprites
          if (!in_main_menu || in_map_editor) {
            if (map_background>=0 && map_background<=2) {
              DeleteObject(map_background_sprite);
              HBITMAP tmp_map_background_sprite;

              wchar_t lvl_background_bmp[64];
              swprintf(lvl_background_bmp,64,L"saves/%s/images/background.bmp",level_names[level_chosen]);
              tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, lvl_background_bmp, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

              if (tmp_map_background_sprite==NULL) { //not found :/
                switch (map_background) {
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
                }
              }
              if (tmp_map_background_sprite!=NULL) {
                map_background_sprite=CopyStretchBitmap(tmp_map_background_sprite,SRCCOPY,GR_WIDTH,GR_HEIGHT); //note runs once only
              } else {
                map_background_sprite=NULL;
              }
              DeleteObject(tmp_map_background_sprite);
            }
          } else {            
            //LoadMainMenuBackground();
          }
        }

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
                if (game_timer<int_best_score) { //New high score
                /*DIR* dir;
                dir=opendir("score_saves");
                if (ENOENT==errno) {
                  mkdir("score_saves");
                }*/
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


            if (!(player.time_breaker || player.is_swinging || player.is_rebounding)) {
              if (game_cam_shake) {
                PlayerCameraShake();
              }
            } else {
              player.cam_move_x=0;
              player.cam_move_y=0;
            }

            player.seed=rand();

            if (level_loaded && (player.health<=0 || flag_restart)) { // restart level when player health hits 0 or VK_RETURN
              flag_restart_audio=TRUE;
              Init(hdc);
              flag_restart=FALSE;
            }

            hdcBackbuff=CreateCompatibleDC(hdc);
            screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
            SelectObject(hdcBackbuff,screen);
            DrawBackground(hdcBackbuff);
            DrawPlatforms(hdcBackbuff);
            DrawWebs(hdcBackbuff);
            DrawEnemy(hdcBackbuff);
            DrawPlayer(hdcBackbuff);


            if (shadows) {
              DrawShadows(hdcBackbuff);
            }

            DrawUI(hdcBackbuff);
            //GrGlassRect(hdcBackbuff,0,0,GR_WIDTH,GR_HEIGHT,YELLOW,128);
            DrawCursor(hdcBackbuff);
            //DrawGrids(hdcBackbuff); //debugging
            DrawWaterShader(hdcBackbuff);           
            if (raining) {
              DrawRain(hdcBackbuff);
              DrawRainShader2(hdcBackbuff);
            }


            //void DrawGlassBitmap(HDC hdc, HBITMAP hBitmap, int x, int y, int level)
            //DrawGlassBitmap(hdcBackbuff,map_platforms_shadow_shaders,MAP_WIDTH/2,MAP_HEIGHT/2,128);
            //DrawNodeGrids(hdcBackbuff);
            //DrawTBitmap(hdcBackbuff,&draw_moon_sprite,GR_WIDTH/2,GR_HEIGHT/2,50);

            if (!IsInvertedBackground()){ //Inverted palette level
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
            } else { //non inverted palette level
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
            }

            
            DeleteDC(hdcBackbuff);
            DeleteObject(screen);


            //Trigger go back to main menu
            if (back_to_menu) {
              CleanupAll();
            }
          }
        } else if (in_map_editor) {
          if (level_loaded) {
            frame_tick++;
            showoff++;
            if (frame_tick>FPS) {
              frame_tick=0;
            }

            hdcBackbuff=CreateCompatibleDC(hdc);
            screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
            SelectObject(hdcBackbuff,screen);
            DrawBackground(hdcBackbuff);
            DrawMapEditorPlatforms(hdcBackbuff);
            DrawMapEditorEnemy(hdcBackbuff);
            DrawMapEditorPlayer(hdcBackbuff);
            DrawGrids(hdcBackbuff,player.cam_x+GR_WIDTH/2,player.cam_y+GR_HEIGHT/2);
            DrawMapEditorUI(hdcBackbuff);
            DrawCursor(hdcBackbuff);

            player.seed=rand();

            if (!IsInvertedBackground()){ //Inverted palette level
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
            } else { //non inverted palette level
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
            }
            DeleteDC(hdcBackbuff);
            DeleteObject(screen);

              //Trigger go back to main menu
            if (back_to_menu) {
              CleanupMapEditorAll();
            }
          }
        } else { //In Main Menu
          showoff++;
          PAINTSTRUCT ps;
          hdc=BeginPaint(hwnd, &ps);
          hdcBackbuff=CreateCompatibleDC(hdc);
          HBITMAP screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
          SelectObject(hdcBackbuff,screen);
      
          DrawMainMenu(hdcBackbuff);
          DrawCursor(hdcBackbuff);

          BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);

          DeleteDC(hdcBackbuff);
          DeleteObject(screen);
        
        }
      //}
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
      //MessageBox(NULL, TEXT("ភាសាខ្មែរ"), TEXT("ភាសាខ្មែរ") ,MB_OK); //khmer text box
      //printf("boolsize:%d",sizeof(bool));      
      AddFontResource(L"fonts/unifont-8.0.01.ttf");
      AddFontResource(L"fonts/KhmerOS.ttf");
      //AddFontResource(L"fonts/KhmerUI.ttf");
      //AddFontResource(L"fonts/KhmerOSsys.ttf");
      Init8BitRGBColorsNoir(rgbColorsNoir);
      Init8BitRGBColorsDefault(rgbColorsDefault);
      wav_out_original_volume=VolumeValue(50,1); //set volume
      //waveOutGetVolume(hWaveOut[2],&wav_out_original_volume);
      RESOLUTION_X[2]=SCREEN_WIDTH; //set resolution x to be of screen width
      RESOLUTION_Y[2]=SCREEN_HEIGHT; //set reolution y to be of screen height

      swprintf(src_music_dir,64,L"music"); //set dir of music



      //Delete tmp in music
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp

      MessageBox(NULL, TEXT("ចងចាំអ្នកខ្មែរដែលបាត់បង់ជីវិតក្នុងសង្គ្រាមដែលអ្នកអាគាំងនិងអ្នកជនជាតិជ្វីហ្វចង់ដណ្ដើមយកទន្លេមេគង្គពីសម្តេចឪនរោត្តមសីហនុចាប់ផ្តើមពីឆ្នាំ ១៩៦៣ ដល់ ១៩៩៧ កម្ពុជាក្រោមព្រៃនគរពីឆ្នាំ ១៨៥៨ ដល់ ១៩៤៩ និងកម្ពុជាខាងជើង។\n\nខ្មែរធ្វើបាន! ជយោកម្ពុជា!\n\nIn memory of the Innocent Cambodian Lives lost caused by wars and destabilization efforts (1963-1997).\n\n\nCode is in my Github: https://github.com/Anfinonty/wInsecticide/releases\n\nwInsecticide Version: v1446-04-16"), TEXT("អ្នករាបចង្រៃ") ,MB_OK);
//TEXT("អាពីងស៊ីរុយ") ,MB_OK); //ឈ្មោះចាស់

      //load levels in save
      GetSavesInDir(L"saves");

      InitTickFrequency(); //get ticks frequency 
      InitFPS(); //get screen refresh rate

      player.health=20; //init player heath (for cursor)



    //hijri related (*
      int64_t timenow=int64_current_timestamp(); //local timestamp is returned

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
      back_to_menu=FALSE;
      in_main_menu=TRUE;
      level_chosen=0;
      stop_playing_song=FALSE;


      //Init avi playing
      if (solar_hour>6 && solar_hour<18) { //day
        InitExtractAVIFrames(L"avi/mainmenu_gameplay_day.avi",0);
      } else {
        InitExtractAVIFrames(L"avi/mainmenu_gameplay_night.avi",0);
      }

     //Load Song
      InitSongBank();
     //

      ShowCursor(FALSE);

      //Load Best Scores :D
      //,,,

      //Load Player Sprites
      player.osprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.osprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.osprite_jump = (HBITMAP) LoadImageW(NULL, L"sprites/player3-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.oattack_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.oattack_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.oattack_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.oattack_sprite_4 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.oblock_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player-block-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.oblock_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player-block-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.oblock_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/player-block-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.ospin_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/player-spin.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      //Load Enemy Sprites
      enemy1_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy1-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy1_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy1-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      enemy2_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy2_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy2_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);



      //Load mouse cursor sprite
      mouse_cursor_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/player_cursor2_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      mouse_cursor_sprite2 = (HBITMAP) LoadImageW(NULL, L"sprites/player_cursor2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);
      mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);

      mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,RED);
      mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,RED);

      mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,LTRED);
      mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,LTRED);


      GenerateDrawSprite(&draw_mouse_cursor_sprite,mouse_cursor_sprite_cache);
      GenerateDrawSprite(&draw_mouse_cursor_sprite_iris,mouse_cursor_sprite_iris_cache);
      GenerateDrawSprite(&draw_mouse_cursor_sprite_pupil,mouse_cursor_sprite_pupil_cache);

      GenerateDrawSprite(&draw_mouse_cursor_sprite2,mouse_cursor_sprite_cache2);
      GenerateDrawSprite(&draw_mouse_cursor_sprite_iris2,mouse_cursor_sprite_iris_cache2);
      GenerateDrawSprite(&draw_mouse_cursor_sprite_pupil2,mouse_cursor_sprite_pupil_cache2);


      



      //Load moon sprite based on lunar day
      if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-8.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-11.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-14.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-16.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-21.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-26.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-28.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      }
      //moon_sprite_cache=CreteLargeBitmap(NULL, 128, 128);
      moon_sprite_cache=RotateSprite(NULL, moon_sprite,0,LTGREEN,BLACK,BLACK,-1);

      
      GenerateDrawSprite(&draw_moon_sprite,moon_sprite_cache);

      }


      title_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/title.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      title_small_sprite = CopyStretchBitmap(title_sprite,SRCCOPY,356*3/5,256*3/5);
      title_sprite_mask=CreateBitmapMask(title_sprite,LTGREEN,NULL);
      title_small_sprite_mask=CreateBitmapMask(title_small_sprite,LTGREEN,NULL);



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


      for (int i=0;i<7;i++) { //options
        wchar_t mm2khtxt[32];
        swprintf(mm2khtxt,32,L"sprites/khmai/mm2kh_%d.bmp",i);
        mm2_kh[i]=(HBITMAP) LoadImageW(NULL, mm2khtxt, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        mm2_kh_white[i]=RotateSprite(NULL, mm2_kh[i],0,LTGREEN,BLACK,WHITE,-1);
        mm2_kh_green[i]=RotateSprite(NULL, mm2_kh[i],0,LTGREEN,BLACK,LLTGREEN,-1);
        mm2_kh_mask[i]= CreateBitmapMask(mm2_kh[i],LTGREEN,NULL);
      }



      //fullscreen
      //ShowWindow(hwnd,SW_SHOWMAXIMIZED);

      //=====Load Audio=====
    //https://stackoverflow.com/questions/32320825/load-wavs-into-memory-then-play-sounds-asynchronously-using-win32-api
    //https://stackoverflow.com/questions/65851460/playing-sound-from-a-memory-buffer-in-pure-winapi
    //https://stackoverflow.com/questions/2457482/processing-an-audio-wav-file-with-c
    //https://stackoverflow.com/questions/8754111/how-to-read-the-data-in-a-wav-file-to-an-array
      //if (load_sound && level_loaded) {

       loadSoundEffect(&spamSoundEffect[0],L"snd/timebreaker__start.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[1],L"snd/timebreaker__stop.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[2],L"snd/clang.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[3],L"snd/player_block.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[4],L"snd/player_block_perfect.wav",FALSE);
       loadSoundEffect(&spamSoundEffect[5],L"snd/player_hurt.wav",FALSE);


       loadSoundEffect(&keySoundEffect[0],L"snd/play_level.wav",FALSE); //Enter Sound Effect (Sometimes) [0]
       loadSoundEffect(&keySoundEffect[1],L"snd/FE_COMMON_MB_02.wav",FALSE); //Key Up Down Sound Effect [1]
       loadSoundEffect(&keySoundEffect[2],L"snd/FE_COMMON_MB_03.wav",FALSE); //False Sound Effect --> [2]
       loadSoundEffect(&keySoundEffect[3],L"snd/FE_COMMON_MB_04.wav",FALSE); //True Sound Effect --> [3]
       loadSoundEffect(&keySoundEffect[4],L"snd/FE_COMMON_MB_05.wav",FALSE); //ESC Sound Effect --> [4]
       loadSoundEffect(&keySoundEffect[5],L"snd/FE_MB_18.wav",FALSE); //Paint Sound Effect --> [5]


       for (int i=0;i<KEY_SFX_NUM;i++) {
         adjustSFXVolume(&keySoundEffectCache[i],&keySoundEffect[i],game_volume,FALSE);
       }

       loadSoundEffect(&channelSoundEffect[0],L"snd/fast.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[1],L"snd/clang_death.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[2],L"snd/S_DIO_00033.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[3],L"snd/rain2classic.wav",TRUE);
       loadSoundEffect(&channelSoundEffect[4],L"snd/rain2classic.wav",TRUE);


       //for wav sound effects
       waveOutOpen(&hWaveOut[0], WAVE_MAPPER, &wfx_wav_sfx, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[0], &whdr[0], sizeof(WAVEHDR));

       waveOutOpen(&hWaveOut[1], WAVE_MAPPER, &wfx_wav_sfx, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[1], &whdr[1], sizeof(WAVEHDR));


       //Get details of sfx2, 2nd type of audio, its smaller (8000hz)
       wfx_wav_sfx2.wFormatTag = WAVE_FORMAT_PCM;
       wfx_wav_sfx2.nChannels = channelSoundEffect[2].wav_header->NumOfChan;
       wfx_wav_sfx2.nSamplesPerSec = channelSoundEffect[2].wav_header->SamplesPerSec;
       wfx_wav_sfx2.nAvgBytesPerSec = channelSoundEffect[2].wav_header->bytesPerSec;
       wfx_wav_sfx2.nBlockAlign = channelSoundEffect[2].wav_header->blockAlign;
       wfx_wav_sfx2.wBitsPerSample = channelSoundEffect[2].wav_header->bitsPerSample;
       wfx_wav_sfx2.cbSize = 0;


       wfx_wav_sfx3.wFormatTag = WAVE_FORMAT_PCM;
       wfx_wav_sfx3.nChannels = channelSoundEffect[3].wav_header->NumOfChan;
       wfx_wav_sfx3.nSamplesPerSec = channelSoundEffect[3].wav_header->SamplesPerSec;
       wfx_wav_sfx3.nAvgBytesPerSec = channelSoundEffect[3].wav_header->bytesPerSec;
       wfx_wav_sfx3.nBlockAlign = channelSoundEffect[3].wav_header->blockAlign;
       wfx_wav_sfx3.wBitsPerSample = channelSoundEffect[3].wav_header->bitsPerSample;
       wfx_wav_sfx3.cbSize = 0;

       /*printf("%d\n", wfx_wav_sfx2.nChannels);
       printf("%d\n", wfx_wav_sfx2.nSamplesPerSec);
       printf("%d\n", wfx_wav_sfx2.nAvgBytesPerSec);
       printf("%d\n", wfx_wav_sfx2.nBlockAlign);
       printf("%d\n", wfx_wav_sfx2.wBitsPerSample);*/
       waveOutOpen(&hWaveOut[3], WAVE_MAPPER, &wfx_wav_sfx2, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[3], &whdr[3], sizeof(WAVEHDR));

       waveOutOpen(&hWaveOut[4], WAVE_MAPPER, &wfx_wav_sfx3, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[4], &whdr[4], sizeof(WAVEHDR));

       waveOutOpen(&hWaveOut[5], WAVE_MAPPER, &wfx_wav_sfx3, 0, 0, CALLBACK_NULL);
       waveOutPrepareHeader(hWaveOut[5], &whdr[5], sizeof(WAVEHDR));
       return 0;
       break;
    //Tasks to perform on exit
    case WM_DESTROY:

      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp
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
  int small_screen_x=640;
  int small_screen_y=480;
  resolution_choose=0;
  if (SCREEN_WIDTH>640) {
    small_screen_x=800;
    small_screen_y=600;
    resolution_choose=1;
  }
  HWND hwnd = CreateWindowW(wc.lpszClassName,
//                L"អាពីងស៊ីរុយ - wInsecticide",
                L"អ្នករាបចង្រៃ - wInsecticide",
                WS_POPUP | WS_BORDER | WS_OVERLAPPEDWINDOW | WS_VISIBLE | CW_USEDEFAULT| CW_USEDEFAULT /*| WS_MINIMIZE*/,
                SCREEN_WIDTH/2-small_screen_x/2,
                SCREEN_HEIGHT/2-small_screen_y/2,
                //800,//SCREEN_WIDTH,//GR_WIDTH+7,
                //600-8*4,//SCREEN_HEIGHT,//GR_HEIGHT+27, //4:3 aspect ratio
                small_screen_x,
                small_screen_y,
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

    Sleep(100);
  }

  SetForegroundWindow(hwnd);
  HANDLE thread1=CreateThread(NULL,0,AnimateTask01,NULL,0,NULL); //Spawm Game Logic Thread
  HANDLE thread2=CreateThread(NULL,0,AnimateTask02,NULL,0,NULL); //Spawm Game Logic Thread
  HANDLE thread3=CreateThread(NULL,0,SongTask,NULL,0,NULL); //Spawn Song Player Thread
  HANDLE thread4=CreateThread(NULL,0,AnimateAVI,NULL,0,NULL); //Spawm Game Logic Thread

  //SetTimer(hwnd, 1, frameDelays[currentFrame] , NULL);

  MSG msg;
  while (true) {
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
      if (msg.message == WM_QUIT) break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  /*MSG msg;
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  timeEndPeriod(1);*/

  //In case WM_DESTROY doesnt work
  //HWND hShellWnd = FindWindowA("Shell_TrayWnd", NULL);
  //ShowWindow(hShellWnd, SW_SHOW);
  waveOutSetVolume(hWaveOut[2],wav_out_original_volume);
  return (int) msg.wParam;
}




