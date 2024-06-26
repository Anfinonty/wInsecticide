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
#include <mmsystem.h>
#include <audioclient.h>
//#include <libsndfile.h> // Make sure to install libsndfile and link it during compilation
//#include <wiavideo.h>
//#include <GL/glu.h>
//#include "<resources.h>"


#define SCREEN_WIDTH    (GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT   (GetSystemMetrics(SM_CYSCREEN))


//Global Variables Game state variables
bool yes_unifont=FALSE;
bool clean_up_sound=FALSE;
bool level_loaded=FALSE;
bool hide_taskbar=FALSE;
bool flag_restart=FALSE;
bool flag_restart_audio=FALSE;
bool back_to_menu=FALSE;
bool in_main_menu=TRUE;
bool game_over=FALSE;
bool game_cam_shake=TRUE;
bool game_audio=TRUE;
bool alloc_enemy_once=TRUE;
bool load_sound=FALSE;

wchar_t save_level[128];

int level_chosen=0;
int windowx=0;
int windowy=0;

int enemy_kills=0;
int FPS = 60;
int main_menu_chosen=0; //options for main menu
int option_choose=0;

int GR_WIDTH,GR_HEIGHT,OLD_GR_WIDTH,OLD_GR_HEIGHT;

int frame_tick=-10;
int int_best_score=0; //to store to write
int player_color=0;
int player_load_color=0;
int player_bullet_color=0;




wchar_t global_wchar[3]={' ',L'H',L'I'};


long long game_timer=0;


double double_best_score=0;
double time_begin=0;
double game_volume=1.0;


//Solar Hijri Time for Drawing
int solar_sec=0,solar_min=0,solar_hour=0,solar_day=0,solar_month=0,solar_year=0,solar_day_of_week=0;
double solar_angle_day=0;

//Lunar Hijri Time for Drawing
int lunar_sec=0,lunar_min=0,lunar_hour=0,lunar_day=0,lunar_month=0,lunar_year=0,lunar_day_of_week=0;
double moon_angle_shift=0;

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

//#define VRENDER_DIST      9//20
//#define VRDGRID_NUM       VRENDER_DIST*VRENDER_DIST


#define BULLET_NUM	5000
#define ENEMY_BULLET_NUM                   1000
#define MAX_BULLET_PER_FIRE 10




#define MAX_MAP_NODE_NUM (640*20)/NODE_SIZE * (480*20)/NODE_SIZE //MAX_WIDTH/NODE_SIZE * MAX_HEIGHT/NODE_SIZE
#define MAX_VGRID_NUM   4800 //(640/160)*20 * (480/160)*20
#define MAX_GRID_NUM    4800
#define MAX_GROUND_NUM  1000
#define MAX_ENEMY_NUM   50



#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			30

#define DEFAULT_PLAYER_HEALTH			20
#define DEFAULT_PLAYER_BLOCK_HEALTH_MAX 20
#define DEFAULT_PLAYER_JUMP_HEIGHT 		85//100
#define DEFAULT_PLAYER_ATTACK_STRENGTH  	1
#define DEFAULT_PLAYER_KNOCKBACK_STRENGTH	50

#define DEFAULT_PLAYER_BUILD_RANGE		80//100//12
#define MAX_WEB_LENGTH		200//100//12
//#define DEFAULT_PLAYER_SHORT_BUILD_RANGE	10
#define DEFAULT_PLAYER_WEB_HEALTH		150

#define DEFAULT_PLAYER_WEB_NUM      20
#define MAX_WEB_NUM     100

#define DEFAULT_PLAYER_SPEED			1

#define DEFAULT_PLAYER_TIME_BREAKER_MAX	20 //10 seconds to charge
#define DEFAULT_PLAYER_TIME_BREAKER_COOLDOWN_MAX   700 //5 seconds after usage
#define DEFAULT_PLAYER_TIME_BREAKER_RECHARGE_MAX	200 //1 seconds
#define DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX	22 //45

#define PLAYER_LOW_HEALTH   3
//#define PLAYER_BULLET_NUM 32
#define PLAYER_BULLET_NUM 24//16


#include "struct_classes.c"

long clang_audio_filesize;
long tb_start_audio_filesize;
long tb_stop_audio_filesize;

long mkey_down_up_audio_filesize;
long mkey_true_audio_filesize;
long mkey_false_audio_filesize;
long mkey_paint_audio_filesize;
long mkey_esc_audio_filesize;


static int16_t* clang_audio;
static int16_t* tb_start_audio;
static int16_t* tb_stop_audio;

static int16_t* mkey_down_up_audio;
static int16_t* mkey_true_audio;
static int16_t* mkey_false_audio;
static int16_t* mkey_paint_audio;
static int16_t* mkey_esc_audio;


static int16_t* clang_audio_cache;
static int16_t* tb_start_audio_cache;
static int16_t* tb_stop_audio_cache;


static int16_t* mkey_down_up_audio_cache;
static int16_t* mkey_true_audio_cache;
static int16_t* mkey_false_audio_cache;
static int16_t* mkey_paint_audio_cache;
static int16_t* mkey_esc_audio_cache;




static int16_t* fast_mem_audio;
static int16_t* fast_mem_audio_cache;
long fast_mem_audio_filesize;
int fast_mem_audio_duration;


static int16_t* cdeath_mem_audio;
static int16_t* cdeath_mem_audio_cache;
long cdeath_mem_audio_filesize;
int cdeath_mem_audio_duration;


/*
long tmp_clang_audio_filesize;
int16_t* tmp_clang_audio;//=LoadWav("snd/fast.wav",&tmp_clang_audio_filesize);
int tmp_duration;// = (double)tmp_clang_audio_filesize / (11025L * 1 * 16/8) *1000;



long tmp_clang_audio_filesize2;
int16_t* tmp_clang_audio2;//=LoadWav("snd/fast.wav",&tmp_clang_audio_filesize);
int tmp_duration2;// = (double)tmp_clang_audio_filesize / (11025L * 1 * 16/8) *1000;
*/




//SpamSnd* clang_audio_cache;
//SpamSnd* tb_start_audio_cache;
//SpamSnd* tb_stop_audio_cache;


#include "math.c"
#include "gr.c"
#include "sound.c"


#include "load_save.c"

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

//  alloc_enemy_once=TRUE;
  player_load_color=player_color;
  player_bullet_color=WHITE;
  if (IsInvertedBackground()) { //invert player color if background is inverted
    player_load_color=COLORS_NUM-player_color-1;
    player_bullet_color=BLACK;
  }


  if (tb_start_audio_cache!=NULL)
    free(tb_start_audio_cache);
  if(tb_stop_audio_cache!=NULL)
    free(tb_stop_audio_cache);
  if (clang_audio_cache!=NULL)
    free(clang_audio_cache);
  if (fast_mem_audio_cache!=NULL)
    free(fast_mem_audio_cache);
  if (cdeath_mem_audio_cache!=NULL)
    free(cdeath_mem_audio_cache);

 
  tb_stop_audio_cache=adjustVolumeA(tb_start_audio,tb_start_audio_filesize,game_volume);
  tb_start_audio_cache=adjustVolumeA(tb_stop_audio,tb_stop_audio_filesize,game_volume);
  clang_audio_cache=adjustVolumeA(clang_audio,clang_audio_filesize,game_volume);

  cdeath_mem_audio_cache=adjustVolume(cdeath_mem_audio,cdeath_mem_audio_filesize,game_volume);
  fast_mem_audio_cache=adjustVolume(fast_mem_audio,fast_mem_audio_filesize,game_volume/5);
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
  InitBullet();
  InitGrid();
  InitNodeGrid();
  InitGround();
  InitNodeGridAttributes();
  InitEnemy();
  InitPlayer();
  BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
  for (int i=0;i<SND_THREAD_NUM;i++) {
    mem_snd_interrupt[i]=TRUE;
    waveOutReset(hWaveOut[i]);
  }
  PlaySound(NULL, NULL, SND_ASYNC);
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

  DeleteObject(mouse_cursor_sprite_cache);
  //if (!IsInvertedBackground()) {
    mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,draw_color_arr[player_load_color],-1);
  /*} else {
    HBITMAP tmp_mouse_cursor_sprite=CopyCrunchyBitmap(mouse_cursor_sprite,NOTSRCCOPY);
    mouse_cursor_sprite_cache=RotateSprite(NULL, tmp_mouse_cursor_sprite,0,PURPLE,draw_color_arr[player_load_color],-1);
    DeleteObject(tmp_mouse_cursor_sprite);
  }*/


  DeleteObject(mouse_cursor_sprite_cache2);
  //if (!IsInvertedBackground()) {
    mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,draw_color_arr[player_load_color],-1);
  /*} else {
    HBITMAP tmp_mouse_cursor_sprite2=CopyCrunchyBitmap(mouse_cursor_sprite2,NOTSRCCOPY);
    mouse_cursor_sprite_cache2=RotateSprite(NULL, tmp_mouse_cursor_sprite2,0,PURPLE,draw_color_arr[player_load_color],-1);
    DeleteObject(tmp_mouse_cursor_sprite2);
  }*/
  //Load Enemy cache spritesF
  InitEnemySprites();

  InitPlatformsSprite(hwnd,hdc);

  /*printf("clang_audio:%lu\n",clang_audio_filesize);
  printf("tb_start_audio:%lu\n",tb_start_audio_filesize);
  printf("tb_death_audio:%lu\n",tb_stop_audio_filesize);
  printf("clang_death_audio:%lu\n",clang_death_audio_filesize);*/


  //allocate smallest to biggest




  //load_sound=TRUE;
  level_loaded=TRUE;
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
      if (level_loaded) {
        PlayerAct();
        for (int i=0;i<ENEMY_NUM;i++) {
          EnemyAct(i);
        }
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
  //HWND hShellWnd = FindWindowA("Shell_TrayWnd", NULL);
  //LONG originalStyle = GetWindowLong(hwnd, GWL_STYLE);

  //wchar_t le_msg[32];
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
        switch (wParam) {
        //Holding down '9' Key
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

        //Holding down '0' Key
          case '0'://skip song, downwards (next)
            if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
              if (song_mode<=2) {
                song_rand_num=LimitValue(song_rand_num+1,0,song_num);
                skip_song=TRUE;
                play_new_song=TRUE;
                loading_flac=FALSE;
              }
            }
            break;
        }
      }


      //Key Down Presses depending on game state
      if (!in_main_menu) {

        switch (wParam) {
        //Holding Down Shift && Escape
          case VK_ESCAPE:
            if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
              if (level_loaded) { //allow back to menu only if level is fully loaded
                back_to_menu=TRUE;
              }
            }
            break;

        //Holding Down Down Arrow or 'S'
          case 'S':
          case VK_DOWN:
            player.rst_down=TRUE;
            break;


        //Holding Down Right Arrow or 'D'
          case 'D':
          case VK_RIGHT:
            player.rst_right=TRUE;
            break;


        //Holding Down Left Arrow or 'A'
          case 'A':
          case VK_LEFT:
            player.rst_left=TRUE;
            break;


        //Holding Down Up Arrow or 'W''
          case 'W':
          case VK_UP:
            player.rst_up=TRUE;
            break;


        //Holding down ENTER key
          case VK_RETURN:
            flag_restart=TRUE;
            break;

        //Holding down SPACE key
          case ' ':
            player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
            break;


        //Holding down 'E' key
	      case 'E':
            player.uppercut=TRUE;
	        break;

        //Holding down 'C' Key
          case 'C':
            if (player.sleep_timer==DEFAULT_SLEEP_TIMER) {
              player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
            } else {
              player.sleep_timer=DEFAULT_SLEEP_TIMER;
            }
            break;


        //Holding down 'Z' Key
          case 'Z':
            if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
              player.time_breaker=TRUE;
              if (game_audio) {
                //PlaySound(L"snd/timebreaker__start.wav", NULL, SND_FILENAME | SND_ASYNC);
                PlaySound(tb_start_audio_cache, NULL, SND_MEMORY | SND_ASYNC);
              }
              player.time_breaker_cooldown=player.time_breaker_cooldown_max;
              player.speed+=player.time_breaker_units_max/2-1;
            }
            break;
        } //End of Keys switch statement


      } else { //Main menu
        switch (main_menu_chosen) {
           case 0:
             switch (wParam) {
              //Holding Down Shift && Escape
               case VK_ESCAPE:
                 if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
                   PostQuitMessage(0);
                   return 0;
                 }
                 break;

              //Holding Down Down Arrow or 'S'
               case 'S':
               case VK_DOWN:
                 level_chosen=LimitValue(level_chosen+1,0,level_num);
                 if (game_audio)
                   PlaySound(mkey_down_up_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //up down
                 break;

              //Holding Down Up Arrow or 'W''
               case 'W':
               case VK_UP:
                 level_chosen=LimitValue(level_chosen-1,0,level_num);
                 if (game_audio)
                   PlaySound(mkey_down_up_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //up down
                 break;


              //Holding down ENTER key
               case VK_RETURN:
                 if (player_color>-1 && player_color<COLORS_NUM) {               
                   if (level_chosen>=0 && level_chosen<level_num && main_menu_chosen==0)
                     InitLevel(hwnd, hdc);
                 }
                 break;

             } //End of switch statement
             break;



           //Player options in main menu
           case 1:
             switch (wParam) {
            //Holding Down Down Arrow or 'S'
               case 'S':
               case VK_DOWN:
                 option_choose=LimitValue(option_choose+1,0,4);
                 if (game_audio)
                   PlaySound(mkey_down_up_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //up down
                 break;


            //Holding Down Right Arrow or 'D'
               case 'D':
               case VK_RIGHT:
                 switch (option_choose) {
                   case 0: //Change color of player ++
                     player_color=LimitValue(player_color+1,0,COLORS_NUM);
                     if (game_audio)
                       PlaySound(mkey_paint_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //paint
                     break;
                   case 1: //Enable/Disable sound effects
                     if (game_audio)
                       PlaySound(mkey_false_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //false
                     else
                       PlaySound(mkey_true_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //true
                     game_audio=!game_audio;
                     break;
                   case 2: //Enable/Disable camera shaking
                     if (game_cam_shake)
                       if (game_audio)
                         PlaySound(mkey_false_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //false
                     else
                       if (game_audio)
                         PlaySound(mkey_true_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //true
                     game_cam_shake=!game_cam_shake;                
                     break;
                   case 3:
                     if (game_volume>=2.0) {
                       game_volume+=1.0;
                     } else {
                       game_volume+=0.1;
                     }
                     if (game_volume>20.0) {
                        game_volume=0.0;
                     }
                     if (game_audio)
                       PlaySound(mkey_false_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //false
                     break;
                 }
                 break;


            //Holding Down Left Arrow or 'A'
              case 'A':
              case VK_LEFT:
                switch (option_choose) {
                  case 0: //Change color of player --
                    player_color=LimitValue(player_color-1,0,COLORS_NUM);
                    if (game_audio)
                      PlaySound(mkey_paint_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //paint
                    break;
                  case 1: //Enable/Disable sound effects
                    if (game_audio)
                      PlaySound(mkey_false_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //false
                    else
                      PlaySound(mkey_true_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //true
                    game_audio=!game_audio;
                    break;
                  case 2:  //Enable/Disable camera shaking 
                    if (game_cam_shake)
                      if (game_audio)
                        PlaySound(mkey_false_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //false
                    else
                      if (game_audio)
                        PlaySound(mkey_true_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //true
                    game_cam_shake=!game_cam_shake;                
                    break;
                   case 3:
                     if (game_volume>2.0) {
                       game_volume-=1.0;
                     } else {
                       game_volume-=0.1;
                     }
                     if (game_volume<0.0) {
                        game_volume=20.0;
                     }
                     //PlaySound(L"snd/FE_COMMON_MB_03.wav", NULL, SND_FILENAME | SND_ASYNC); //false
                     if (game_audio)
                       PlaySound(mkey_false_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //false
                     break;
                }
                break;


            //Holding Down Up Arrow or 'W''
              case 'W':
              case VK_UP:
                option_choose=LimitValue(option_choose-1,0,4);
                //PlaySound(L"snd/FE_COMMON_MB_02.wav", NULL, SND_FILENAME | SND_ASYNC); //up down
                 if (game_audio)
                  PlaySound(mkey_down_up_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //up down
                break;
           } //End of switch statement for keys
           break;


          /*case 2:
              //https://learn.microsoft.com/en-us/windows/win32/learnwin32/keyboard-input
              //https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/getch-getwch?view=msvc-170
              //swprintf_s(le_msg,32,L"Key: 0x%x\n",wParam); 
              //wprintf(le_msg);
          {
            //int a_wchar=_getwch();
            //global_wchar[0]=a_wchar;
            global_wchar[0]=wParam;
          }*/
          break;
        } //end of switch statement for menu chosen
      } //end of menu chosen if else
      break; //Break WM_KEYDOWN;
    }  


    //Key Release
    case WM_KEYUP:
    {
  //GLOBAL wParam Release Key
      if (main_menu_chosen!=2) {
      switch (wParam) {
        //Release 'T' key holding SHIFT
        case 'T': //Hide or Show Taskbar
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (!hide_taskbar) {
              //ShowWindow(hShellWnd, SW_HIDE); //hide taskbar

              //https://stackoverflow.com/questions/2398746/removing-window-border
              LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
              lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
              SetWindowLong(hwnd, GWL_STYLE, lStyle);


              SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);


              SetForegroundWindow(hwnd); //return back focus

              hide_taskbar=TRUE;
            } else {
              //ShowWindow(hShellWnd, SW_SHOW); //show taskbar again


              LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
              lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
              SetWindowLong(hwnd, GWL_STYLE, lStyle);

              SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);

              SetForegroundWindow(hwnd);
              hide_taskbar=FALSE;
            }
          }
          break;

        //release U key
        /*case 'U':
          PlayMemSnd(fast_mem_audio_cache,fast_mem_audio_filesize,fast_mem_audio_duration);
          break;

        case 'I':
          PlayMemSnd(cdeath_mem_audio_cache,cdeath_mem_audio_filesize,cdeath_mem_audio_duration);
          break;*/

        //Release N key while holding SHIFT or not
        case 'N':
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (song_mode==0 || song_mode==3) {
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
            song_mode=LimitValue(song_mode-1,0,4);
          } else {
            if (!stop_playing_song) {
              play_new_song=TRUE;
              loading_flac=FALSE;
            }
          }
          break;//end current song


        //Release M key while holding SHIFT or not
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

        //Release 'L' Key
        case 'L':
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (yes_unifont) {
              yes_unifont=FALSE;
            } else {
              yes_unifont=TRUE;
            }
          }
          break;
      } //end of global key release
      }

      if (!in_main_menu) { //Gaming
        switch (wParam) {

          //Release '8' key holding SHIFT
          case '8':
            if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
              if (!display_controls) {
                display_controls=TRUE;
              } else {
                display_controls=FALSE;
              }
            }
            break;


          //Release Q key
          case 'Q':
            player.destroy_ground=TRUE;
            break;

        //Release S or Down key
          case 'S':
          case VK_DOWN:
            player.rst_down=FALSE;
            break;

        //Release D or Right key
          case 'D':
          case VK_RIGHT:
            player.rst_right=FALSE;
            break;


        //Release A or Left key
          case 'A':
          case VK_LEFT:
            player.rst_left=FALSE;
            break;

        //Release W or Up key
          case 'W':
          case VK_UP:
            player.rst_up=FALSE;
            break;

        //Release Space key
          case ' ': 
            player.sleep_timer=DEFAULT_SLEEP_TIMER;
            break;


        //Release '1' Key
	      case '1':
            player.attack_rst=TRUE;
	        break;


        //Release '2' Key
          case '2':
            if (player.max_web_num-player.placed_web_num>=3 && player.knives_per_throw==5) {
              player.knives_per_throw=13;
            }
            /*if (player.max_web_num-player.placed_web_num>=6 && player.knives_per_throw==15) {
              player.knives_per_throw=28;
            }*/
            /*if (player.max_web_num-player.placed_web_num>5) {
              player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,30+1); //limit to 1,3,5,15
          } else*/
            if (player.max_web_num-player.placed_web_num>2) {          
              player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,15+1); //limit to 1,3,5,15
            } else if (player.max_web_num-player.placed_web_num>0){ //limit to 1,3,5
              player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,5+1);
            } else { //limit to 1,3
              player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,3+1);
            }
            break;

        //Release '3' Key'
          case '3':
            player.low_jump=!player.low_jump;
            break;

        //Release 'E' Key
  	      case 'E':
            if (player.uppercut) {
              player.uppercut=FALSE;
            }
	        break;
        }

      } else { 
        switch (main_menu_chosen) {
          //Release '0' Key
          case 0:
            switch (wParam) {
              case '1':
                main_menu_chosen=1;
                if (game_audio)
                  PlaySound(mkey_esc_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //esc
                break;
              case '2':
                main_menu_chosen=2;
                if (game_audio)
                  PlaySound(mkey_esc_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //esc
                break;
            }
            break;



          //Release '1' Key
          case 1:
            switch (wParam) {
               case '1':
               case VK_ESCAPE:
                 if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT) || wParam=='1') { //ESC + L/RSHIFT = QUIT
                   main_menu_chosen=0;

                   DeleteObject(mouse_cursor_sprite_cache);
                   mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,draw_color_arr[player_color],-1);
   
                   DeleteObject(mouse_cursor_sprite_cache2);
                   mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,draw_color_arr[player_color],-1);
                   //PlaySound(L"snd/FE_COMMON_MB_05.wav", NULL, SND_FILENAME | SND_ASYNC); //esc

                   //adjust volume
                   if (mkey_down_up_audio_cache!=NULL)
                     free(mkey_down_up_audio_cache);
                   if (mkey_false_audio_cache!=NULL)
                     free(mkey_false_audio_cache);
                   if (mkey_true_audio_cache!=NULL)
                     free(mkey_true_audio_cache);
                   if (mkey_paint_audio_cache!=NULL)
                     free(mkey_paint_audio_cache);
                   if (mkey_esc_audio_cache!=NULL)
                     free(mkey_esc_audio_cache);

                    mkey_down_up_audio_cache=adjustVolumeA(mkey_down_up_audio,mkey_down_up_audio_filesize,game_volume);
                    mkey_true_audio_cache=adjustVolumeA(mkey_true_audio,mkey_true_audio_filesize,game_volume);
                    mkey_false_audio_cache=adjustVolumeA(mkey_false_audio,mkey_false_audio_filesize,game_volume);
                    mkey_paint_audio_cache=adjustVolumeA(mkey_paint_audio,mkey_paint_audio_filesize,game_volume);
                    mkey_esc_audio_cache=adjustVolumeA(mkey_esc_audio,mkey_esc_audio_filesize,game_volume);

                    if (game_audio)
                      PlaySound(mkey_esc_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //esc
                 }
                break;
            }
            break;



          //Release '2' Key
          case 2:

            
            switch (wParam) {
               case VK_ESCAPE:
                 if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
                   main_menu_chosen=0;
                   //PlaySound(L"snd/FE_COMMON_MB_05.wav", NULL, SND_FILENAME | SND_ASYNC); //esc
                   if (game_audio)
                     PlaySound(mkey_esc_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //esc
                 }
                 break;
            }
            break;

          }
          break; //end of main menu chosen switch

      } //end of !main_menu_chosen
      break; // end of release key WM_KEYUP
    }


    case WM_CHAR:
      if (in_main_menu)
        if (main_menu_chosen==2)
           global_wchar[0]=wParam;
      break;

    //Constantly Update Screen
    case WM_ERASEBKGND:
      if (!IsIconic(hwnd)) //no action when minimized
        InvalidateRect(hwnd,NULL,TRUE);
      return TRUE;
      break;



    //Graphics
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

          //HBITMAP screen = CreateGreyscaleBitmap(GR_WIDTH,GR_HEIGHT);
          screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
          SelectObject(hdcBackbuff,screen);
          DrawBackground(hdcBackbuff);
          DrawPlatforms(hdcBackbuff);
          DrawWebs(hdcBackbuff);
          DrawEnemy(hdcBackbuff);
          DrawPlayer(hdcBackbuff);
          DrawUI(hdcBackbuff);
          DrawCursor(hdcBackbuff);
          //DrawGrids(hdcBackbuff);

          if (!IsInvertedBackground()){ //Inverted palette level
            BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
          } else { //non inverted palette level
            BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
          }
          DeleteDC(hdcBackbuff);
          DeleteObject(screen);


         //Manage Clean Snd Task
          /*for (int i=0;i<SND_THREAD_NUM;i++) {
            if (mem_snd_stopped[i]) {
              //waveOutUnprepareHeader(hWaveOut[i], &whdr[i], sizeof(WAVEHDR));
              //waveOutClose(hWaveOut[i]);
              CloseHandle(hMemSndArray[i]);
              mem_snd_playing[i]=FALSE;
              mem_snd_stopped[i]=FALSE;
            }
          }*/

          //Trigger go back to main menu
          if (back_to_menu) {
            level_loaded=FALSE;
            InitBullet();
            CleanUpPlayer(); //clean up all sprites
            CleanUpEnemySprites();
            //CleanUpGrid();
            //CleanUpNodeGrid();
            //CleanUpEnemy();
            //CleanUpGround();
            CleanupPlayerAttributes();
            save_level[0]='\0';



            //free saved grounds pointer & Ground
            free(saved_ground_is_ghost);
            free(saved_ground_color);
            free(saved_ground_type);
            free(saved_ground_x1);
            free(saved_ground_y1);
            free(saved_ground_x2);
            free(saved_ground_y2);
            free(saved_ground_x3);
            free(saved_ground_y3);
            for (int i=0;i<GROUND_NUM;i++) {
              free(saved_ground_text[i]);
            }
            free(saved_ground_text);



            for (int i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {//free objects
              freeGround(Ground[i]);
            }


            for (int i=0;i<MAP_NODE_NUM;i++) {
              freeNode(NodeGrid[i]); //free actual obj
            }


            for (int i=0;i<VGRID_NUM;i++) {
              freeVGrid(VGrid[i]); //free actual obj
            }


            for (int i=0;i<ENEMY_NUM;i++) {
              freeEnemy(Enemy[i]);
            }
            //printf("===All objects freed\n");



            free(Ground); //free pointer to pointers
            free(NodeGrid); //free pointer to pointers
            free(VGrid); //free pointer to pointers
            free(Enemy);
            //printf("===All pointers freed\n");


            DeleteObject(map_platforms_sprite); //delete sprites
            DeleteObject(map_platforms_sprite_mask);
            LoadMainMenuBackground();
            back_to_menu=FALSE;
            clean_up_sound=TRUE;
            in_main_menu=TRUE;

            DeleteObject(moon_sprite_cache);
            moon_sprite_cache=RotateSprite(NULL, moon_sprite,0,LTGREEN,BLACK,-1);

            DeleteObject(mouse_cursor_sprite_cache);
            mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,draw_color_arr[player_color],-1);

            DeleteObject(mouse_cursor_sprite_cache2);
            mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,draw_color_arr[player_color],-1);


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


    //Tasks to perform on start
    case WM_CREATE:
    {
      //MessageBox(NULL, TEXT("ភាសាខ្មែរ"), TEXT("ភាសាខ្មែរ") ,MB_OK); //khmer text box
      Init8BitRGBColorsNoir(rgbColorsNoir);
      Init8BitRGBColorsDefault(rgbColorsDefault);
      //Delete tmp in music
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp

      MessageBox(NULL, TEXT("ចងចាំអ្នកខ្មែរដែលបាត់បង់ជីវិតក្នុងសង្គ្រាមដែលអ្នកអាគាំងនិងអ្នកជនជាតិជ្វីហ្វចង់ដណ្ដើមយកទន្លេមេគង្គពីសម្តេចឪនរោត្តមសីហនុចាប់ផ្តើមពីឆ្នាំ ១៩៦៩ ដល់ ១៩៩៧ កម្ពុជាក្រោមព្រៃនគរពីឆ្នាំ ១៨៥៨ ដល់ ១៩៤៩ និងកម្ពុជាខាងជើង។\n\nខ្មែរធ្វើបាន! ជយោកម្ពុជា!\n\nIn memory of the Innocent Cambodian Lives lost caused by wars and destabilization efforts (1969-1997).\n\n\nCode is in my Github: https://github.com/Anfinonty/wInsecticide/releases\n\nwInsecticide Version: v1445-12-20"), TEXT("អាពីងស៊ីរុយ") ,MB_OK);

      //load levels in save
      GetSavesInDir(L"saves");

      InitTickFrequency();
      InitFPS();
      //unsigned long long timenow=current_timestamp();
      //printf("\nSeconds Passed Since Jan-1-1970: %llu",timenow);

      player.health=20;
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
      mouse_cursor_sprite2 = (HBITMAP) LoadImageW(NULL, L"sprites/player_cursor2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,-1);
      mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,-1);

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

      //=====Load Audio=====
    //https://stackoverflow.com/questions/32320825/load-wavs-into-memory-then-play-sounds-asynchronously-using-win32-api
    //https://stackoverflow.com/questions/65851460/playing-sound-from-a-memory-buffer-in-pure-winapi
    //https://stackoverflow.com/questions/2457482/processing-an-audio-wav-file-with-c
    //https://stackoverflow.com/questions/8754111/how-to-read-the-data-in-a-wav-file-to-an-array
      //if (load_sound && level_loaded) {
        tb_start_audio=LoadWavA("snd/timebreaker__start.wav",&tb_start_audio_filesize);
        tb_stop_audio=LoadWavA("snd/timebreaker__stop.wav",&tb_stop_audio_filesize);
        clang_audio=LoadWavA("snd/clang.wav",&clang_audio_filesize);


        mkey_down_up_audio=LoadWavA("snd/FE_COMMON_MB_02.wav",&mkey_down_up_audio_filesize);
        mkey_false_audio=LoadWavA("snd/FE_COMMON_MB_03.wav",&mkey_false_audio_filesize);
        mkey_true_audio=LoadWavA("snd/FE_COMMON_MB_04.wav",&mkey_true_audio_filesize);
        mkey_esc_audio=LoadWavA("snd/FE_COMMON_MB_05.wav",&mkey_esc_audio_filesize);
        mkey_paint_audio=LoadWavA("snd/FE_MB_18.wav",&mkey_paint_audio_filesize);

        mkey_down_up_audio_cache=adjustVolumeA(mkey_down_up_audio,mkey_down_up_audio_filesize,game_volume);
        mkey_true_audio_cache=adjustVolumeA(mkey_true_audio,mkey_true_audio_filesize,game_volume);
        mkey_false_audio_cache=adjustVolumeA(mkey_false_audio,mkey_false_audio_filesize,game_volume);
        mkey_paint_audio_cache=adjustVolumeA(mkey_paint_audio,mkey_paint_audio_filesize,game_volume);
        mkey_esc_audio_cache=adjustVolumeA(mkey_esc_audio,mkey_esc_audio_filesize,game_volume);


        fast_mem_audio=LoadWav("snd/fast.wav",&fast_mem_audio_filesize, &fast_mem_audio_duration);
        //fast_mem_audio_duration=(double)fast_mem_audio_filesize / (11025L * 1 * 16/8) *1000;

        cdeath_mem_audio=LoadWav("snd/clang_death.wav",&cdeath_mem_audio_filesize, &cdeath_mem_audio_duration);


         for (int i=0;i<SND_THREAD_NUM;i++) {
           waveOutOpen(&hWaveOut[i], WAVE_MAPPER, &wfx1, 0, 0, CALLBACK_NULL);
           waveOutPrepareHeader(hWaveOut[i], &whdr[i], sizeof(WAVEHDR));
         }
      //}

        //DWORD tmp_clang_audio_filesize;
        //char* tmp_clang_audio=LoadAudioData("snd/clang.wav",&tmp_clang_audio_filesize);


        //long tmp_clang_audio_filesize2;
        //int16_t* tmp_clang_audio2=LoadWav("snd/clang.wav",&tmp_clang_audio_filesize2);


        //PlaySound(clang_audio,NULL,SND_MEMORY|SND_ASYNC);
        //PlayStereoAudio(tmp_clang_audio,tmp_clang_audio_filesize,duration);//,tmp_clang_audio2,tmp_clang_audio_filesize2);

        //SpamSoundAudio mySpamSound = {tmp_clang_audio,tmp_clang_audio_filesize,duration};

        
        //HANDLE thread_audio=CreateThread(NULL,0,PlaySpamSound,NULL,0,NULL); //Spawm Game Logic Thread
        //HANDLE thread_audio2=CreateThread(NULL,0,PlaySpamSound2,NULL,0,NULL); //Spawm Game Logic Thread


        //AMemSnd *myMemSnd= createMemSnd(tmp_clang_audio,tmp_clang_audio_filesize,tmp_duration);
        //PlayStereoAudio(tmp_clang_audio,tmp_clang_audio_filesize);
        //PlayStereoAudio(tmp_clang_audio,tmp_clang_audio_filesize);
        //PlayStereoAudio(tmp_clang_audio,tmp_clang_audio_filesize);
        //PlayStereoAudio(tmp_clang_audio,tmp_clang_audio_filesize);
       


        //PlayStereoAudio(clang_audio,clang_audio_filesize);
        //PlayStereoAudio(clang_audio,clang_audio_filesize);
        //PlayStereoAudio(clang_audio,clang_audio_filesize);
        //PlaySound(clang_audio,NULL,SND_MEMORY);
        //PlaySound((LPCWSTR)tmp_clang_audio,NULL,SND_MEMORY);

        //free(tmp_clang_audio);
        //free(tmp_clang_audio2);

        //free(tmp_clang_audio2);
      return 0;
      break;



    //Tasks to perform on exit
    case WM_DESTROY:
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp
      //HWND hShellWnd = FindWindowA("Shell_TrayWnd", NULL);
      //ShowWindow(hShellWnd, SW_SHOW);
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
                WS_POPUP | WS_BORDER | WS_OVERLAPPEDWINDOW | WS_VISIBLE | CW_USEDEFAULT| CW_USEDEFAULT /*| WS_MINIMIZE*/,
                SCREEN_WIDTH/2-400,
                SCREEN_HEIGHT/2-300,
                //800,//SCREEN_WIDTH,//GR_WIDTH+7,
                //600-8*4,//SCREEN_HEIGHT,//GR_HEIGHT+27, //4:3 aspect ratio
                800,
                600,
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

  //HANDLE sound_thread1=CreateThread(NULL,0,SoundTask1,NULL,0,NULL); //Spawn Song Player Thread
  //HANDLE sound_thread2=CreateThread(NULL,0,SoundTask2,NULL,0,NULL); //Spawn Song Player Thread
  //HANDLE sound_thread3=CreateThread(NULL,0,SoundTask3,NULL,0,NULL); //Spawn Song Player Thread

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

  //In case WM_DESTROY doesnt work
  //HWND hShellWnd = FindWindowA("Shell_TrayWnd", NULL);
  //ShowWindow(hShellWnd, SW_SHOW);
  return (int) msg.wParam;
}




