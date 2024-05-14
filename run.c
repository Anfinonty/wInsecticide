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
//i686-w64-mingw32-gcc-win32 run.c -o run.exe  -lgdi32 -municode -lwinmm -lshlwapi
//-lopengl32 -lglu32 is not used for now Jan-06-2024 -credit: sothea.dev
#include <windows.h>
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
#include <errno.h>
//#include <clocale>
//#include <curses.h>
#include <shlwapi.h>
//#include <wiavideo.h>
//#include <GL/glu.h>
//#include "<resources.h>"


#define COLORS_NUM  16
#define BLACK       RGB(0,0,0)
#define BLUE        RGB(0,0,170)
#define GREEN    	RGB(0,170,0)
#define CYAN        RGB(0,170,170)
#define RED         RGB(170,0,0)
#define PURPLE      RGB(170,0,170)
#define BROWN       RGB(170,85,0)
#define LTGRAY      RGB(170,170,170)
#define DKGRAY      RGB(85,85,85)
#define LTBLUE      RGB(0,0,255)
#define LTGREEN     RGB(0,255,0)
#define LTCYAN      RGB(0,255,255)
#define LTRED       RGB(255,0,0)
#define LTPURPLE    RGB(255,0,255)
#define YELLOW      RGB(255,255,0)
#define WHITE       RGB(255,255,255)


#define DKBLACK     RGB(24,24,24) //For drawing
#define LLTGREEN    RGB(0,254,0)
#define MYCOLOR1    RGB(123,123,123)

//#define IDI_MYICON  1000

int color_arr[COLORS_NUM]={
BLACK, //0
BLUE, //1
GREEN, //2
CYAN, //3
RED, //4
PURPLE, //5
BROWN, //6
LTGRAY, //7
DKGRAY, //8
LTBLUE, //9
LTGREEN, //10
LTCYAN, //11
LTRED, //12
LTPURPLE, //13
YELLOW, //14
WHITE //15
};

int draw_color_arr[COLORS_NUM]={
DKBLACK, //0
BLUE, //1
GREEN, //2
CYAN, //3
RED, //4
PURPLE, //5
BROWN, //6
LTGRAY, //7
DKGRAY, //8
LTBLUE, //9
LLTGREEN, //10
LTCYAN, //11
LTRED, //12
LTPURPLE, //13
YELLOW, //14
WHITE //15
};

#define SONG_NUM 10
#define SONG_FOLDER_NUM 23

#define SCREEN_WIDTH    (GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT   (GetSystemMetrics(SM_CYSCREEN))


//Global Variables
int GR_WIDTH,GR_HEIGHT,OLD_GR_WIDTH,OLD_GR_HEIGHT;
int solar_sec=0,solar_min=0,solar_hour=0,solar_day=0,solar_month=0,solar_year=0,solar_day_of_week=0;
double solar_angle_day=0;

int lunar_sec=0,lunar_min=0,lunar_hour=0,lunar_day=0,lunar_month=0,lunar_year=0,lunar_day_of_week=0;
double moon_angle_shift=0;

int frame_tick=-10;
int int_best_score=0;
int player_color=0;
int player_load_color=0;
int player_bullet_color=0;
double double_best_score=0;
wchar_t save_level[128];
double time_begin=0;
bool yes_unifont=FALSE;

//HBITMAP canny;
//HBITMAP uncanny;



#define DEFAULT_PLAYER_SPEED			1

#define ENEMY_TYPE_NUM                         10


#define PLAYER_WIDTH 	32
#define PLAYER_HEIGHT 	32

#define GRID_SIZE	160
#define VGRID_SIZE	160
#define NODE_SIZE  	 10
#define MAX_GROUNDS_WITHIN_GRID	(VGRID_SIZE/NODE_SIZE)*(VGRID_SIZE/NODE_SIZE)/2 //128 grounds

#define ENEMY_I64_ATTRIBUTES_NUM 20
#define ENEMY_F64_ATTRIBUTES_NUM 4
//#define ENEMY_BOOL_ATTRIBUTES_NUM 1

#define GROUND_F64_ATTRIBUTES_NUM 6
#define GROUND_BOOL_ATTRIBUTES_NUM 1
#define GROUND_I64_ATTRIBUTES_NUM 2
//#define GROUND_U8_ATTRIBUTES_NUM 1


#define MAX_FOLLOW_RANGE 100
#define MAX_NODE_NUM	 MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE

#define VRENDER_DIST      9//20
#define VRDGRID_NUM       VRENDER_DIST*VRENDER_DIST


#define BULLET_NUM	5000
#define PLAYER_BULLET_NUM          16
#define ENEMY_BULLET_NUM                   1000
#define MAX_BULLET_PER_FIRE 10


#define MAX_WEB_NUM      100


#define MAX_MAP_NODE_NUM (640*20)/NODE_SIZE * (480*20)/NODE_SIZE //MAX_WIDTH/NODE_SIZE * MAX_HEIGHT/NODE_SIZE
#define MAX_VGRID_NUM   4800 //(640/160)*20 * (480/160)*20
#define MAX_GRID_NUM    4800
#define MAX_GROUND_NUM  1000
#define MAX_ENEMY_NUM   50

#include "struct_classes.c"
#include "load_save.c"


#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			30

#define DEFAULT_PLAYER_HEALTH			20
#define DEFAULT_PLAYER_JUMP_HEIGHT 		85//100
#define DEFAULT_PLAYER_ATTACK_STRENGTH  	1
#define DEFAULT_PLAYER_KNOCKBACK_STRENGTH	50

#define DEFAULT_PLAYER_BUILD_RANGE		100//12
#define DEFAULT_PLAYER_SHORT_BUILD_RANGE	10
#define DEFAULT_PLAYER_WEB_HEALTH		150
#define DEFAULT_PLAYER_WEB_NUM			20
#define DEFAULT_PLAYER_SPEED			1

#define DEFAULT_PLAYER_TIME_BREAKER_MAX	20 //10 seconds to charge
#define DEFAULT_PLAYER_TIME_BREAKER_COOLDOWN_MAX   700 //5 seconds after usage
#define DEFAULT_PLAYER_TIME_BREAKER_RECHARGE_MAX	200 //1 seconds
#define DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX	22 //45

#define DEFAULT_PLAYER_BLOCK_HEALTH_MAX 20



bool flag_restart=FALSE;
bool back_to_menu=FALSE;
bool in_main_menu=TRUE;
int level_chosen=0;
int windowx=0;
int windowy=0;
long long game_timer=0;
bool game_over=FALSE;
int enemy_kills=0;
int FPS = 60;



#include "math.c"
#include "gr.c"

#include "grid.c"
#include "ground.c"
#include "bullet.c"
#include "player.c"
#include "enemy.c"

#include "song.c"
#include "draw_gui.c"
#include "cleanup.c"


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



void InitOnce() {
  GR_WIDTH=SCREEN_WIDTH;
  GR_HEIGHT=SCREEN_HEIGHT;

  player.cam_move_x=0;
  player.cam_move_y=0;


  player_load_color=player_color;
  player_bullet_color=WHITE;
  if (IsInvertedBackground()) { //invert player color if background is inverted
    player_load_color=COLORS_NUM-player_color-1;
    player_bullet_color=BLACK;
  }
}



void Init(HDC hdc) {
  time_begin=current_timestamp();

  //Load Best Score
  //Folder & file creation
  FILE *fptr;
  if (_waccess(save_level, F_OK)==0) { //if file exists
  //do nothing
  } else if (ENOENT==errno) { //if file doesnt exist
    fptr = _wfopen(save_level,L"a");
    fprintf(fptr,"2147483646\n");
    fclose(fptr);
  }


  int_best_score=0;
  FILE *fr; //Begin setting best score
  int c;
  int current_int;
  fr = _wfopen(save_level,L"r"); //check if scoresave data of levelname
  while ((c=fgetc(fr))!=EOF) {
    if (c>='0' && c<='9') {
      current_int=c-'0';
      int_best_score*=10; //shift digit by left,  //Read from file while !=EOF score*=10
      int_best_score+=current_int; //add to right digit //after reading, convert into (double)/1000
    }
  }
  fclose(fr);
  double_best_score=(double)int_best_score/1000;


  //Start level
  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;
  game_timer=0;
  enemy_kills=0;
  game_over=FALSE;
  frame_tick=-10;


  //Initialize Level
  InitGrid();
  InitNodeGrid();
  InitGround();
  InitBullet();
  InitNodeGridAttributes();
  InitEnemy();
  InitPlayer();
  BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
}



void InitPlatformsSprite(HWND hwnd, HDC hdc)
{
  PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
  hdc=BeginPaint(hwnd, &ps);
  HDC hdc2=CreateCompatibleDC(hdc);


  map_platforms_sprite=CreateCrunchyBitmap(MAP_WIDTH,MAP_HEIGHT);
  //map_platforms_sprite=CreateLargeBitmap(MAP_WIDTH,MAP_HEIGHT);

  SelectObject(hdc2,map_platforms_sprite);

  GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,MYCOLOR1); //Create Background with random color over platforms

  DrawGroundTriFill(hdc2);
  DrawGround(hdc2);
  DrawGroundText(hdc2);

  DeleteDC(hdc2);
  EndPaint(hwnd, &ps);

  //map_platforms_sprite=ReplaceColor(tmp_map_platforms_sprite,MYCOLOR1,BLACK,NULL);
  map_platforms_sprite_mask=CreateBitmapMask(map_platforms_sprite,MYCOLOR1,NULL); //create mask where black becomes   //end of platform sprite creation
}



void InitLevel(HWND hwnd, HDC hdc)
{
  wchar_t txt[128];
  swprintf(txt,128,L"saves/%s/level.txt",level_names[level_chosen]);
  swprintf(save_level,128,L"saves/%s/scores.txt",level_names[level_chosen]);

  LoadSave(txt);

  srand(time(NULL));
  timeBeginPeriod(1);


  InitOnce();//cannot be repeatedly run
  Init(hdc);


  //Load Player Sprites
  player.sprite_jump_cache = RotateSprite(NULL, player.sprite_jump,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.sprite_1_cache = RotateSprite(NULL, player.sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.sprite_2_cache = RotateSprite(NULL, player.sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);

  player.attack_sprite_1_cache = RotateSprite(NULL, player.attack_sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.attack_sprite_2_cache = RotateSprite(NULL, player.attack_sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.attack_sprite_3_cache = RotateSprite(NULL, player.attack_sprite_3,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.attack_sprite_4_cache = RotateSprite(NULL, player.attack_sprite_4,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);

  player.block_sprite_1_cache = RotateSprite(NULL, player.block_sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.block_sprite_2_cache = RotateSprite(NULL, player.block_sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);
  player.block_sprite_3_cache = RotateSprite(NULL, player.block_sprite_3,player.sprite_angle,LTGREEN,draw_color_arr[player_load_color],-1);

  player.spin_sprite_1_cache = RotateSprite(NULL, player.spin_sprite,0.1,LTGREEN,draw_color_arr[player_load_color],-1);
  player.spin_sprite_2_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI_2,LTGREEN,draw_color_arr[player_load_color],-1);
  player.spin_sprite_3_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI,LTGREEN,draw_color_arr[player_load_color],-1);
  player.spin_sprite_4_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI+M_PI_2,LTGREEN,draw_color_arr[player_load_color],-1);

  //moon sprite
  DeleteObject(moon_sprite_cache);
  HBITMAP tmp_moon_sprite=CopyCrunchyBitmap(moon_sprite,NOTSRCCOPY);
  moon_sprite_cache=RotateSprite(NULL, tmp_moon_sprite,0,PURPLE,BLACK,-1);
  DeleteObject(tmp_moon_sprite);



  //Load Enemy cache spritesF
  InitEnemySprites();

  InitPlatformsSprite(hwnd,hdc);

  in_main_menu=FALSE;
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



DWORD WINAPI AnimateTask01(LPVOID lpArg) {
  while (TRUE) {
    if (!in_main_menu) { //In Game
      PlayerAct();
      for (int i=0;i<player.rendered_enemy_num;i++) {
        EnemyAct(player_render_enemies[i]);
      }
      Sleep(player.sleep_timer);
    } else {
      Sleep(1000);
    }
  }
}



void LoadMainMenuBackground()
{
  DeleteObject(map_background_sprite);
  HBITMAP tmp_map_background_sprite;
  if (solar_hour>6 && solar_hour<18) { //day
    tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  } else { //night
    tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  }
  map_background_sprite=CopyStretchBitmap(tmp_map_background_sprite,SRCCOPY,GR_WIDTH,GR_HEIGHT); //note runs once only
  DeleteObject(tmp_map_background_sprite);
}



bool keydown(int key) //https://stackoverflow.com/questions/47667367/is-there-a-way-to-check-for-shift-held-down-then-control-held-down-for-each-inpu
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}



bool keydownalt()
{
    return (GetAsyncKeyState(VK_RMENU) & 0x8000)!= 0;
}



LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  switch(msg) {
    case WM_LBUTTONDOWN:
      player.rst_left_click=TRUE;
      break;
    case WM_LBUTTONUP:
      player.rst_left_click=FALSE;
      break;
    case WM_RBUTTONDOWN:
      player.rst_right_click=TRUE;
      break;
    case WM_RBUTTONUP:
      player.rst_right_click=FALSE;
      break;
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
    case WM_KEYDOWN:
      switch (wParam) {
        case VK_ESCAPE:
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
            if (!in_main_menu) {
              back_to_menu=TRUE;
            } else {
              PostQuitMessage(0);
              return 0;
            }
          }
          break;
        case 'S':
        case VK_DOWN:
            if (!in_main_menu) {
              player.rst_down=TRUE;
            } else {
              level_chosen++;
              if (level_chosen>level_num-1) {
                level_chosen=0;
              }
            }
            break;

        case 'D':
        case VK_RIGHT:
          player.rst_right=TRUE;
          if (in_main_menu) {
            player_color=LimitValue(player_color+1,0,COLORS_NUM);
          }
          break;


        case 'A':
        case VK_LEFT:
          player.rst_left=TRUE;
          if (in_main_menu) {
            player_color=LimitValue(player_color-1,0,COLORS_NUM);
          }
          break;


        case 'W':
        case VK_UP:
          if (!in_main_menu) {
            player.rst_up=TRUE;
          } else {
            level_chosen--;
            if (level_chosen<0) {
              level_chosen=level_num-1;
            }
          }
          break;


        case VK_RETURN:
          if (!in_main_menu) {
            flag_restart=TRUE;
          } else {//Run Level
            if (player_color>-1 && player_color<COLORS_NUM) {
              if (level_chosen>=0 && level_chosen<level_num)
                InitLevel(hwnd, hdc);
            }
          }
          break;

        case ' ':
          if (!in_main_menu) {
            player.rst_key_sprint=TRUE;
          }
          break;

	    case 'E':
          if (!in_main_menu) {
	        player.uppercut=TRUE;
          }
	      break;


        case '9'://skip song, upwnwards (previous)
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (song_mode<=2) {
              if  (!keydownalt()) {
                song_rand_num=LimitValue(song_rand_num-1,0,song_num);
                skip_song=TRUE;
                play_new_song=TRUE;
                loading_flac=FALSE;
              }
            }
          }
          break;

        case '0'://skip song, downwards (next)
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (song_mode<=2) {
              song_rand_num=LimitValue(song_rand_num+1,0,song_num);
              skip_song=TRUE;
              play_new_song=TRUE;
              loading_flac=FALSE;
            }
          }


        case 'C':
          if (!in_main_menu) {
            if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
              player.time_breaker=TRUE;
              PlaySound(L"snd/timebreaker__start.wav", NULL, SND_FILENAME | SND_ASYNC);
              player.time_breaker_cooldown=player.time_breaker_cooldown_max;
              player.speed+=player.time_breaker_units_max/2-1;
            }
            if (player.sleep_timer==DEFAULT_SLEEP_TIMER) {
              player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
            } else {
              player.sleep_timer=DEFAULT_SLEEP_TIMER;
            }
          }
          break;


        case 'Z':
          if (!in_main_menu) {
            if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
              player.time_breaker=TRUE;
              PlaySound(L"snd/timebreaker__start.wav", NULL, SND_FILENAME | SND_ASYNC);
              player.time_breaker_cooldown=player.time_breaker_cooldown_max;
              player.speed+=player.time_breaker_units_max/2-1;
            }
          }
          break;

      }
      break;



    case WM_KEYUP:
      switch (wParam) {
        case '8':
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
            if (!in_main_menu) {
              if (!display_controls) {
                display_controls=TRUE;
              } else {
                display_controls=FALSE;
              }
            }
          }
          break;

        case 'Q':
          player.destroy_ground=TRUE;
          break;


        case 'S':
        case VK_DOWN:
          if(player.rst_down)
            player.rst_down=FALSE;
          break;


        case 'D':
        case VK_RIGHT:
          if (!in_main_menu) {
            if(player.rst_right) {
              player.rst_right=FALSE;
            }
          }
          break;


        case 'A':
        case VK_LEFT:
          if (!in_main_menu) {
            if(player.rst_left) {
              player.rst_left=FALSE;
            } 
          }
          break;

        case 'W':
        case VK_UP:
          if(player.rst_up)
            player.rst_up=FALSE;
          break;
          break;

        case 'M':
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (song_mode>=2) {
              if (!stop_playing_song) {
                stop_playing_song=TRUE;
                toggle_stop_playing_song=TRUE;
              } else { //renable song
                remove("music/tmp/tmp.wav");
                rmdir("music/tmp"); //remove tmp
                InitSongBank();
                song_rand_num=LimitValue(-1,0,song_num);
                stop_playing_song=FALSE;
              }
            }
            song_mode=LimitValue(song_mode+1,0,4);
          } else {
            if (!stop_playing_song) {
              play_new_song=TRUE;
              loading_flac=FALSE;
            }
          }
          break;//end current song


        case ' ':
          if (!in_main_menu) {
            if (!in_main_menu) {
              if(player.rst_key_sprint)
                player.rst_key_sprint=FALSE;
            }
          }
          break;

	    case '1':
          if (!in_main_menu) {
	        player.attack_rst=TRUE;
          }
	      break;


	    case 'E':
          if (!in_main_menu) {
            if (player.uppercut) {
	          player.uppercut=FALSE;
            }
          }
	      break;

        case 'L':
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (yes_unifont) {
              yes_unifont=FALSE;
            } else {
              yes_unifont=TRUE;
            }
          }
          break;
      }
      break;
    case WM_ERASEBKGND:
      if (!IsIconic(hwnd)) //no action when minimized
        InvalidateRect(hwnd,NULL,TRUE);
      return TRUE;
      break;


    case WM_PAINT: //https://cplusplus.com/forum/beginner/269434/



      FrameRateSleep(FPS); // (Uncapped) //35 or 60 fps Credit: ayevdood/sharoyveduchi && y4my4m - move it here
      if (!IsIconic(hwnd)) //no action when minimized, prevents crash https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isiconic?redirectedfrom=MSDN
      {
        HBITMAP screen;
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

        if (GR_WIDTH!=OLD_GR_WIDTH || GR_HEIGHT!=OLD_GR_HEIGHT) {
          InitPlayerCamera();
          player.cam_x=0;
          player.cam_y=0;
          CameraInit(player.x,player.y+PLAYER_HEIGHT/2); //idk scaling is weird for sprite
          OLD_GR_WIDTH = GR_WIDTH;
          OLD_GR_HEIGHT = GR_HEIGHT;


          //Load Map Background sprites
          if (!in_main_menu) {
            if (map_background>=0 && map_background<=1) {
              DeleteObject(map_background_sprite);
              HBITMAP tmp_map_background_sprite;
              switch (map_background) {
                case 0:
                  tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                  break;
                case 1:
                  tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
                  break;
              }
              map_background_sprite=CopyStretchBitmap(tmp_map_background_sprite,SRCCOPY,GR_WIDTH,GR_HEIGHT); //note runs once only
              DeleteObject(tmp_map_background_sprite);
            }
          } else {            
            LoadMainMenuBackground();
          }
        }

        if (!in_main_menu) //### LevelLoaded
        { //https://stackoverflow.com/questions/752593/win32-app-suspends-on-minimize-window-animation

          frame_tick++;
          if (frame_tick>FPS) {
            frame_tick=0;
          }

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

          if (!player.time_breaker) { //camera shake by default
            PlayerCameraShake();
          } else { //don't move camera when time breaking'
            player.cam_move_x=0;
            player.cam_move_y=0;
          }

          for (int i=0;i<player.rendered_enemy_num;i++) { //Enemy has random seed value
            Enemy[player_render_enemies[i]].seed=rand();
          }
          player.seed=rand();


          if (player.health<=0 || flag_restart) { // restart level when player health hits 0 or VK_RETURN
            Init(hdc);
            flag_restart=FALSE;
          }

          hdcBackbuff=CreateCompatibleDC(hdc);

          //HBITMAP screen = CreateGreyscaleBitmap(GR_WIDTH,GR_HEIGHT);
          screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
          SelectObject(hdcBackbuff,screen);
          DrawBackground(hdcBackbuff);
          DrawPlatforms(hdcBackbuff);
          DrawWebs(hdcBackbuff);
          DrawEnemy(hdcBackbuff);
          DrawPlayer(hdcBackbuff);
          DrawCursor(hdcBackbuff);
          DrawUI(hdcBackbuff);

          if (!IsInvertedBackground()){ //Inverted palette level
            BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
          } else { //non inverted palette level
            BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
          }
          DeleteDC(hdcBackbuff);
          DeleteObject(screen);

          //Trigger go back to main menu
          if (back_to_menu) {
            CleanUpPlayer(); //clean up all sprites
            CleanUpEnemySprites();
            CleanUpGrid();
            CleanUpNodeGrid();
            CleanUpEnemy();
            CleanUpGround();
            save_level[0]='\0';

            DeleteObject(map_platforms_sprite); //delete sprites
            DeleteObject(map_platforms_sprite_mask);
            LoadMainMenuBackground();
            back_to_menu=FALSE;
            in_main_menu=TRUE;

            DeleteObject(moon_sprite_cache);
            moon_sprite_cache=RotateSprite(NULL, moon_sprite,0,LTGREEN,BLACK,-1);
          }
        } else { //In Main Menu
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
    case WM_CREATE:
    {
      //MessageBox(NULL, TEXT("ភាសាខ្មែរ"), TEXT("ភាសាខ្មែរ") ,MB_OK); //khmer text box
      Init8BitRGBColorsNoir(rgbColorsNoir);
      Init8BitRGBColorsDefault(rgbColorsDefault);

      //Delete tmp in music
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp

      MessageBox(NULL, TEXT("ចងចាំអ្នកខ្មែរដែលបាត់បង់ជីវិតក្នុងសង្គ្រាមដែលអ្នកអាគាំងនិងអ្នកជនជាតិជ្វីហ្វចង់ដណ្ដើមយកទន្លេមេគង្គពីសម្តេចឪនរោត្តមសីហនុចាប់ផ្តើមពីឆ្នាំ ១៩៦៩ ដល់ ១៩៩៧ កម្ពុជាក្រោមព្រៃនគរពីឆ្នាំ ១៨៥៨ ដល់ ១៩៤៩ និងកម្ពុជាខាងជើង។\n\nខ្មែរធ្វើបាន! ជយោកម្ពុជា!\n\nIn memory of the Innocent Cambodian Lives lost caused by wars and destabilization efforts by the United States of America under Richard Nixxon's and Henry Kissinger's Administration with the CIA and Mossad to take over the Mekong River Trade Route (1969-1997) from our Late King Father Norodom Sihanouk (1922-2012), Kampuchea-Krom (Saigon; originally Prei Nokor) (1858-1949) and Kampuchea-Kang-Cheng (North).\n\nCode is in my Github: https://github.com/Anfinonty/wInsecticide/releases\n\nwInsecticide Version: v1445_10_23"), TEXT("អាពីងស៊ីរុយ") ,MB_OK);

      //load levels in save
      GetSavesInDir(L"saves");

      InitTickFrequency();
      InitFPS();
      //unsigned long long timenow=current_timestamp();
      //printf("\nSeconds Passed Since Jan-1-1970: %llu",timenow);

      int64_t timenow=int64_current_timestamp(); //local timestamp is returned

      printf("\nSeconds Passed Since Jan-1-1970: %d",timenow);
      PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day);
      PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift);

      int num_char='*';
      if (solar_month==1 && solar_day>=12 && solar_day<=19) {
        num_char='+';
      }

      printf("\n~:: Solar Hijri ::~ %c\n~:: %d.%s(%d).%d // %s // [%d:%d:%d] ::~\n",
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



      printf("\n~:: %d.%s(%d).%d // %s // [%d:%d:%d] ::~\n",
        lunar_year,
        lunar_months_en_txt[lunar_month-1],
        lunar_month,
        lunar_day,
        lunar_days_en_txt[lunar_day_of_week],
        lunar_hour,
        lunar_min,
        lunar_sec
      );

      back_to_menu=FALSE;
      in_main_menu=TRUE;
      level_chosen=0;
      stop_playing_song=FALSE;

     //Load Song
      InitSongBank();
     //

      ShowCursor(FALSE);

      //Load Best Scores :D
      //,,,

      //Load Player Sprites
      //canny =  (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      //uncanny =  (HBITMAP) LoadImageW(NULL, L"sprites/uncanny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


      player.sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.sprite_jump = (HBITMAP) LoadImageW(NULL, L"sprites/player3-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.attack_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.attack_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.attack_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.attack_sprite_4 = (HBITMAP) LoadImageW(NULL, L"sprites/player-attack-4.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.block_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player-block-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.block_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player-block-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.block_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/player-block-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.spin_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/player-spin.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


      /*player.sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.sprite_jump = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.attack_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.attack_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.attack_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.attack_sprite_4 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.block_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.block_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      player.block_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      player.spin_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/canny.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);*/

      //Load Enemy Sprites
      enemy1_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy1-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy1_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy1-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      enemy2_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy2_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy2_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);



      //Load mouse cursor sprite
      mouse_cursor_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/player_cursor1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,-1);


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
      moon_sprite_cache=RotateSprite(NULL, moon_sprite,0,LTGREEN,BLACK,-1);
      }

      //fullscreen
      //ShowWindow(hwnd,SW_SHOWMAXIMIZED);

      return 0;
      break;
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






int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
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
                L"អាពីងស៊ីរុយ - wInsecticide",
                WS_BORDER | WS_OVERLAPPEDWINDOW | WS_VISIBLE | CW_USEDEFAULT| CW_USEDEFAULT /*| WS_MINIMIZE*/,
                SCREEN_WIDTH/2-400,
                SCREEN_HEIGHT/2-300,
                800,//SCREEN_WIDTH,//GR_WIDTH+7,
                600,//SCREEN_HEIGHT,//GR_HEIGHT+27,
                NULL,
                NULL,
                hInstance, 
                NULL);
  //SetWindowLong(hwnd, GWL_STYLE, 0);
  //ShowWindow(hwnd, SW_SHOW);

  //Make game un fast when level is run (simulates focusing tab)
  //https://batchloaf.wordpress.com/2012/10/18/simulating-a-ctrl-v-keystroke-in-win32-c-or-c-using-sendinput/
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
  //SetFocus(hwnd);
  //BringWindowToTop(hwnd);
  SetForegroundWindow(hwnd);
  HANDLE thread1=CreateThread(NULL,0,AnimateTask01,NULL,0,NULL); //Spawm Game Logic Thread
  HANDLE thread2=CreateThread(NULL,0,SongTask,NULL,0,NULL); //Spawn Song Player Thread
  //HANDLE thread3=CreateThread(NULL,0,SndTask,NULL,0,NULL); //Spawn Sound Task

  MSG msg;
  while (true) {
    if (PeekMessage(&msg,NULL,0,0,PM_REMOVE)) {
      if (msg.message == WM_QUIT) break;

      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  timeEndPeriod(1);
  return (int) msg.wParam;
}




