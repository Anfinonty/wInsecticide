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
#include <shlwapi.h>
//#include <wiavideo.h>
//#include <GL/glu.h>

#define COLORS_NUM  16
#define BLACK       RGB(0,0,0)
#define BLUE        RGB(0,0,255)
#define GREEN    	RGB(0,128,0)
#define CYAN        RGB(0,128,128)
#define RED         RGB(255,0,0)
#define PURPLE      RGB(128,0,128)
#define BROWN       RGB(150,75,0)
#define LTGRAY      RGB(211,211,211)
#define DKGRAY      RGB(169,169,169)
#define LTBLUE      RGB(0,85,255)
#define LTGREEN     RGB(0,255,0)
#define LTCYAN      RGB(0,255,255)
#define LTRED       RGB(255,71,76)
#define LTPURPLE    RGB(255,0,255)
#define YELLOW      RGB(255,255,0)
#define WHITE       RGB(255,255,255)


#define DKBLACK     RGB(5,5,5) //For drawing
#define LLTGREEN    RGB(0,254,0)
#define MYCOLOR1    RGB(123,123,123)

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



int GR_WIDTH,GR_HEIGHT,OLD_GR_WIDTH,OLD_GR_HEIGHT;
int solar_sec=0,solar_min=0,solar_hour=0,solar_day=0,solar_month=0,solar_year=0,solar_day_of_week=0;
double solar_angle_day=0;

int lunar_sec=0,lunar_min=0,lunar_hour=0,lunar_day=0,lunar_month=0,lunar_year=0,lunar_day_of_week=0;
double moon_angle_shift=0;



int frame_tick=-10;
int int_best_score=0;
double double_best_score=0;
char save_level[40];
int player_color=0;
double time_begin=0;
HBITMAP map_platforms_timebreaker_sprite;


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
#define DEFAULT_PLAYER_WEB_HEALTH		10
#define DEFAULT_PLAYER_WEB_NUM			20
#define DEFAULT_PLAYER_SPEED			1

#define DEFAULT_PLAYER_TIME_BREAKER_MAX	20 //10 seconds to charge
#define DEFAULT_PLAYER_TIME_BREAKER_COOLDOWN_MAX   700 //5 seconds after usage
#define DEFAULT_PLAYER_TIME_BREAKER_RECHARGE_MAX	200 //1 seconds
#define DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX	22 //45

#define DEFAULT_PLAYER_BLOCK_HEALTH_MAX 20


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
#include "cleanup.c"


//Background
void DrawBackground(HDC hdc) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255))); //RAVE

  if (!player.time_breaker) {
    switch (map_background) {
      case 0:
        DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE);
        break;
      case 1:
        DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,NOTSRCCOPY,FALSE);
        GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);
        break;
      default:
        GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,custom_map_background_color);
        break;
    }
  }
}


void DrawPlatforms(HDC hDC)
{ //Dynamically scale with window size 
  //Draw platforms bitmap mask
  if (!player.time_breaker) {
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2,
                map_platforms_sprite_mask,SRCAND,FALSE);
  //Draw platforms paint
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2,
                map_platforms_sprite,SRCPAINT,FALSE);

  } else {
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2,
                map_platforms_timebreaker_sprite,NOTSRCCOPY,FALSE);
  }
}


void DrawCursor(HDC hDC)
{
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite,SRCAND,FALSE);
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite_mask,SRCPAINT,FALSE);
  GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_cache,FALSE);
  GrCircle(hDC,mouse_x,mouse_y,1,WHITE,-1);
}

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

  InitSongBank();
  //InitCustard()
  //printf("\n===Init Once Done===\n");
}

void Init() {
  time_begin=current_timestamp();

  //Load Best Score
  //Folder & file creation
  DIR* dir = opendir("score_saves");
  if (dir) {//Check for scoresaves folder
    FILE *fptr;
    if (access(save_level, F_OK)==0) { //if file exists
      //do nothing
    } else if (ENOENT==errno) { //if file doesnt exist
      fptr = fopen(save_level,"a");
      fprintf(fptr,"2147483646\n");
      fclose(fptr);
    }
  } else if (ENOENT==errno) {//if it doesn't exist create one
    mkdir("score_saves");
    //Create bestscore=9999
    FILE *fptr;
    fptr = fopen(save_level,"a");
    fprintf(fptr,"2147483646\n");
    fclose(fptr);
  }


  int_best_score=0;
  FILE *fr; //Begin setting best score
  int c;
  int current_int;
  fr = fopen(save_level,"r"); //check if scoresave data of levelname
  while ((c=fgetc(fr))!=EOF) {
    if (c>='0' && c<='9') {
      current_int=c-'0';
      int_best_score*=10; //shift digit by left,  //Read from file while !=EOF score*=10
      int_best_score+=current_int; //add to right digit //after reading, convert into (double)/1000
    }
  }
  fclose(fr);
  double_best_score=(double)int_best_score/1000;




  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;
  game_timer=0;
  enemy_kills=0;
  game_over=FALSE;
  frame_tick=-10;

  InitGrid();
  //printf("\n===Grid Initialized\n");

  InitNodeGrid();
  //printf("\n===Node Grid Initialized\n");

  InitGround();
  //printf("\n===Ground Initialized\n");

  /*for (int i=0;i<GROUND_NUM;i++)
      printf("%d x1: %1.0f, y1:%1.0f/x2: %1.0f, y2:%1.0f/x3: %1.0f, y3:%1.0f\n",i,Ground[i].x1,Ground[i].y1,Ground[i].x2,Ground[i].y2,Ground[i].x3,Ground[i].y3);

      printf("MAP WIDTH %d,\n",MAP_WIDTH);
      printf("MAP HEIGHT %d,\n",MAP_HEIGHT);   
      printf("GRID NUM %d,\n",GRID_NUM);*/
  

  InitBullet();
  //printf("\n===Bullet Initialized\n");

  InitNodeGridAttributes();
  //printf("\n===Node Grid Attributes Initialized\n");

  InitEnemy();
  //printf("\n===Enemy Initialized\n");

  InitPlayer();
  //printf("\n===Player Attributes Initialized\n");
  //InitGround2();
}


void InitPlatformsTBSprite(HWND hwnd, HDC hdc)
{
  PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
  hdc=BeginPaint(hwnd, &ps);
  HDC hdc2=CreateCompatibleDC(hdc);
  //HBITMAP tmp_map_platforms_sprite=CreateCompatibleBitmap(hdc,MAP_WIDTH,MAP_HEIGHT);
  HBITMAP tmp_map_platforms_sprite;
  tmp_map_platforms_sprite=CreateLargeBitmap(MAP_WIDTH,MAP_HEIGHT);
  SelectObject(hdc2,tmp_map_platforms_sprite);

  GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,WHITE);

  DrawGround(hdc2);
  DrawGroundText(hdc2);

  DeleteDC(hdc2);
  EndPaint(hwnd, &ps);

  map_platforms_timebreaker_sprite=CopyBitmap(tmp_map_platforms_sprite,NOTSRCCOPY);//ReplaceColor(tmp_map_platforms_sprite,MYCOLOR1,BLACK,NULL);
  DeleteObject(tmp_map_platforms_sprite);
  //end of platform sprite creation
}


void InitPlatformsSprite(HWND hwnd, HDC hdc)
{
  PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
  hdc=BeginPaint(hwnd, &ps);
  HDC hdc2=CreateCompatibleDC(hdc);
  //HBITMAP tmp_map_platforms_sprite=CreateCompatibleBitmap(hdc,MAP_WIDTH,MAP_HEIGHT);
  HBITMAP tmp_map_platforms_sprite;
  tmp_map_platforms_sprite=CreateLargeBitmap(MAP_WIDTH,MAP_HEIGHT);
  SelectObject(hdc2,tmp_map_platforms_sprite);

  GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,MYCOLOR1);

  DrawGroundTriFill(hdc2);
  DrawGround(hdc2);
  DrawGroundText(hdc2);

  DeleteDC(hdc2);
  EndPaint(hwnd, &ps);

  map_platforms_sprite=ReplaceColor(tmp_map_platforms_sprite,MYCOLOR1,BLACK,NULL);
  map_platforms_sprite_mask=CreateBitmapMask(map_platforms_sprite,BLACK,NULL);
  DeleteObject(tmp_map_platforms_sprite);
  //end of platform sprite creation
}


void InitLevel(HWND hwnd, HDC hdc)
{
  char txt[32];
  int chosen_level=level_chosen;
  int chosen_level2=level_chosen;
  sprintf(txt,"saves/_Level00%d.txt",chosen_level);
  sprintf(save_level,"score_saves/_Level00%d.txt",chosen_level2);
  LoadSave(txt);

  srand(time(NULL));
  timeBeginPeriod(1);


  InitOnce();//cannot be repeatedly run
  Init();


  //Load Player Sprites
  player.sprite_jump_cache = RotateSprite(NULL, player.sprite_jump,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.sprite_1_cache = RotateSprite(NULL, player.sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.sprite_2_cache = RotateSprite(NULL, player.sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);

  player.attack_sprite_1_cache = RotateSprite(NULL, player.attack_sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.attack_sprite_2_cache = RotateSprite(NULL, player.attack_sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.attack_sprite_3_cache = RotateSprite(NULL, player.attack_sprite_3,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.attack_sprite_4_cache = RotateSprite(NULL, player.attack_sprite_4,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);

  player.block_sprite_1_cache = RotateSprite(NULL, player.block_sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.block_sprite_2_cache = RotateSprite(NULL, player.block_sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);
  player.block_sprite_3_cache = RotateSprite(NULL, player.block_sprite_3,player.sprite_angle,LTGREEN,draw_color_arr[player_color],-1);

  player.spin_sprite_1_cache = RotateSprite(NULL, player.spin_sprite,0.1,LTGREEN,draw_color_arr[player_color],-1);
  player.spin_sprite_2_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI_2,LTGREEN,draw_color_arr[player_color],-1);
  player.spin_sprite_3_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI,LTGREEN,draw_color_arr[player_color],-1);
  player.spin_sprite_4_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI+M_PI_2,LTGREEN,draw_color_arr[player_color],-1);

  //moon sprite
  DeleteObject(moon_sprite_cache);
  HBITMAP tmp_moon_sprite=CopyBitmap(moon_sprite,NOTSRCCOPY);
  moon_sprite_cache=RotateSprite(NULL, tmp_moon_sprite,0,LTPURPLE,BLACK,-1);
  DeleteObject(tmp_moon_sprite);

  //Load Enemy cache sprites
  InitEnemySprites();

  InitPlatformsSprite(hwnd,hdc);
  InitPlatformsTBSprite(hwnd,hdc);

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



void DrawPlayingMusic(HDC hdc,int x,int y,int c, int c4)
{
  if (!stop_playing_song) {

    if (song_num>0) {
      wchar_t txt[32+256];
      swprintf(txt,32+256,L"%c%d/%d%c: %s",171,song_rand_num+1,song_num,187,song_names[song_rand_num]);
      //%c 187

      GrPrintArabic(hdc,x,y,txt,"",c,FALSE);
      GrPrintArabic(hdc,x+1,y+1,txt,"",c4,FALSE);

      

      char txt2[72];
      //char txt2_1[2046];
      //char txt2_2[256];
      /*for (int j=0;j<128;j++) {
        sprintf(txt2_1,"%s %d:%c ",txt2_1,'z'+j,'z'+j);
      }*/ //max 256
      //note %c 134 is a cross

      switch (song_mode) {
        case 0:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [SHIFT_M: /%c]",171,187,177,187,171);
          break;
        case 1:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [SHIFT_M: /?]",171,187,177,171);
          break;
        case 2:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /?] [SHIFT_M: X]",171,187,177);
          break;
      }
      GrPrint(hdc,x,y+16,txt2,c);   
      GrPrint(hdc,x+1,y+1+16,txt2,c4);


      //GrPrint(hdc,x,y+32,txt2_1,c);   
      //GrPrint(hdc,x+1,y+1+32,txt2_1,c4);
    }
  } else {
    GrPrint(hdc,x,y,"Press Shift + M to Enable Songs",c);
    GrPrint(hdc,x+1,y+1,"Press Shift + M to Enable Songs",c4);
  }
      

  //char epic[256];
  //for (int i=134;i<134+128;i++)
    //sprintf(epic,"%s %c",epic,i);
  //sprintf(epic,"%c",143);
  //GrPrintArabic(hdc,x,y,epic,c4);

  //GrPrintKhmer(hdc,x,y,TEXT("សួស្តី​ពិភពលោក"),c4);
  //GrPrintKhmer(hdc,x,y,L"こんにちは",c4);
}


void DrawMainMenu(HDC hdc)
{
  //if (solar_hour>6 && solar_hour<18) { //7 AM to 6PM
    //GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,BLUE);
  //} else {
    //DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE);
    //GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,BLACK);
  //}

  //draw bkgrnd
  DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE);

  //Draw Moon Phase
  GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);

  //Moon Pos
  int mcalendar_l=64;
  int mcalendar_x=GR_WIDTH-mcalendar_l*2;
  int mcalendar_y=GR_HEIGHT-mcalendar_l*2;
  double moon_angle=0; 
  //Space Clock
  //Draw blue marbel
  GrCircle(hdc,mcalendar_x,mcalendar_y,10,LTBLUE,LTBLUE);
  GrRect(hdc,mcalendar_x,mcalendar_y,3,4,LTGREEN);
  GrRect(hdc,mcalendar_x-3,mcalendar_y,9,4,LTGREEN);
  GrRect(hdc,mcalendar_x+4,mcalendar_y,5,2,LTGREEN);
  GrRect(hdc,mcalendar_x-4,mcalendar_y-4,9,9,LTGREEN);
  GrCircle(hdc,mcalendar_x-4,mcalendar_y,2,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x+2,mcalendar_y+5,3,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x-2,mcalendar_y-3,3,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x-3,mcalendar_y-5,2,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x-3,mcalendar_y+5,2,LTGREEN,LTGREEN);
  //GrRect(hdc,mcalendar_x+5,mcalendar_y-6,3,5,LTGREEN);
  //GrCircle(hdc,mcalendar_x+5,mcalendar_y-3,2,LTGREEN,LTGREEN);

  if (lunar_day<27) //0 to 26
    moon_angle=(-2*M_PI/27 * lunar_day ) - moon_angle_shift;
  else
    moon_angle=-moon_angle_shift;

  for (int i=0;i<27;i++) {
    double tmp_angle=-2*M_PI/27 * i - moon_angle_shift;
    if (i>1 && i<26) {
      GrCircle(hdc,mcalendar_x + mcalendar_l*cos(tmp_angle), mcalendar_y + mcalendar_l*sin(tmp_angle),5,BLACK,DKGRAY);
    } else {
      if (i==1 || i==26) { //Cresent Moon
        GrCircle(hdc,mcalendar_x + mcalendar_l*cos(tmp_angle), mcalendar_y + mcalendar_l*sin(tmp_angle),6,BLACK,CYAN);
      } else {
        GrCircle(hdc,mcalendar_x + mcalendar_l*cos(tmp_angle), mcalendar_y + mcalendar_l*sin(tmp_angle),7,BLACK,LTCYAN);
      }
    }
  }
  GrCircle(hdc,mcalendar_x + mcalendar_l*cos(moon_angle), mcalendar_y + mcalendar_l*sin(moon_angle),5,WHITE,WHITE);

  //Sun Pos
  GrCircle(hdc,mcalendar_x + mcalendar_l*cos(-solar_angle_day), mcalendar_y + mcalendar_l*sin(-solar_angle_day),5,YELLOW,YELLOW);
  //GrCircle(hdc,GR_WIDTH/2 + 3*100*cos(sun_angle), 128*4 + 3*100*sin(sun_angle),5,YELLOW,YELLOW);
  //GrLine(hdc,GR_WIDTH/2,128*4,GR_WIDTH/2 + 3*100*cos(sun_angle), 128*4 + 3*100*sin(sun_angle),YELLOW);
  //  GrCircle(hdc,GR_WIDTH/2, 128*4,300,YELLOW,-1);



  //Write Hijri Dates
  wchar_t time_row1[16];
  char s_hijri_row1[128];
  wchar_t s_hijri_row2[128];
  char l_hijri_row1[128];
  wchar_t l_hijri_row2[128];

  int num_char='*';
  if (solar_month==1 && (solar_day>=12 && solar_day<=19)) {
    num_char=134;
  }

      swprintf(time_row1,16,L"[%d:%02d:%02d]",current_hour,current_min,current_sec);

      sprintf(s_hijri_row1,"=:: Solar Hijri ::= %c",num_char);

      swprintf(s_hijri_row2,128,L":: %s //%d. %s(%d) .%d",
solar_days_txt[solar_day_of_week],
solar_day,
solar_months_txt[solar_month-1],
solar_month,
solar_year
);


  //( <| <|)  O  (|> |> ) @
  if (lunar_day>=1 && lunar_day<8) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","  )");
  } else if (lunar_day>=8 && lunar_day<11) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s"," |)");
  }  else if (lunar_day>=11 && lunar_day<14) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","<|)");
  } else if (lunar_day>=14 && lunar_day<16) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(O)");
  } else if (lunar_day>=16 && lunar_day<21) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(|>");
  } else if (lunar_day>=21 && lunar_day<26) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(|");
  } else if (lunar_day>=26 && lunar_day<28) {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(");
  } else {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s"," @");
  }


  swprintf(l_hijri_row2,128,L":: %s //%d. %s(%d) .%d",
lunar_days_txt[lunar_day_of_week],
lunar_day,
lunar_months_txt[lunar_month-1],
lunar_month,
lunar_year
);

  GrPrintArabic(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-64,time_row1,"",WHITE,FALSE);
  GrPrintArabic(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-32,L"",s_hijri_row1,WHITE,TRUE);
  GrPrintArabic(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-16,s_hijri_row2,"",WHITE,FALSE);
  GrPrintArabic(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y+16,L"",l_hijri_row1,WHITE,TRUE);
  GrPrintArabic(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y+32,l_hijri_row2,"",WHITE,FALSE);


  GrPrint(hdc,30,10,"Welcome to the wInsecticide Menu!",WHITE);
  char C[1];
  sprintf(C,"%c",134);
  GrPrintArabic(hdc,GR_WIDTH-8*18,8*23+10,L"",C,WHITE,TRUE);
  GrPrintArabic(hdc,GR_WIDTH-8*18-8*2,8*23+12,L"",C,WHITE,TRUE);
  GrPrintArabic(hdc,GR_WIDTH-8*18+8*2,8*23+12,L"",C,WHITE,TRUE);
  

  GrLine(hdc,GR_WIDTH-8*17-4,8*25+10,GR_WIDTH-8*17-4-8*8,8*25+12,WHITE);
  GrLine(hdc,GR_WIDTH-8*17-4,8*25+10,GR_WIDTH-8*17-4+8*8,8*25+12,WHITE);

  GrPrint(hdc,30,10+32,"-  Level 0",WHITE);
  GrPrint(hdc,30,10+32+16,"-  Level 1",WHITE);
  GrPrint(hdc,30,10+32+16*2,"-  Level 2",WHITE);
  GrPrint(hdc,30,10+32+16*3,"-  Level 3",WHITE);
  GrPrint(hdc,30,10+32+16*4,"-  Level 4",WHITE);
  GrPrint(hdc,30,10+32+16*5,"-  Level 5",WHITE);
  GrPrint(hdc,30,10+32+16*6,"-  Level 6",WHITE);
  GrPrint(hdc,30,10+32+16*7,"-  Level 7",WHITE);

  GrPrint(hdc,30,10+32+16*9,"Press 'Enter' to Play Selected Level",WHITE);
  GrPrint(hdc,30,10+32+16*10,"Use Up or Down Keys to Select a Level",WHITE);
  GrPrint(hdc,30,10+32+16*11,"Press [SHIFT_ESC] to Exit",WHITE);
  GrPrint(hdc,30,10+32+16*12,"Press [SHIFT] + 'M' to Change Music Mode",WHITE);

  GrPrint(hdc,30,10+32+16*level_chosen,">",WHITE);
  DrawPlayingMusic(hdc,30,10+32+16*14,BLACK,WHITE);


  GrPrint(hdc,30,10+32+16*17,"Player Color: <    >",WHITE);
  GrPrint(hdc,30,10+32+16*18,"Press [LEFT] or [RIGHT] to Select Player Colour",WHITE);

  if (player_color!=0) {
    GrRect(hdc,30+8*12,10+32+16*17,16,16,BLACK);
  } else {
    GrRect(hdc,30+8*12,10+32+16*17,16,16,WHITE);
  }
  if (player_color>-1 && player_color<COLORS_NUM) {
    GrRect(hdc,30+8*12+2,10+32+16*17+2,12,12,draw_color_arr[player_color]);
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


bool display_controls=FALSE;
void DrawUI(HDC hdc) {
  int c;
  int c4;
  if (!IsInvertedBackground()) {
    c=WHITE;
    c4=BLACK;
  } else {
    c=BLACK;
    c4=WHITE;
  }

//======Print Game Score Text
  //GrRect(hdc,0,8+32,8*40,16*5,c4);

  char gametimetxt[32];
  double print_time_ms=(double)game_timer/1000;
  if (!game_over) {
    sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
    GrPrint(hdc,16+4,8+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+6,8+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+4,10+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+6,10+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+5,9+48,gametimetxt,BROWN);
  } else { //game is over
    if (game_timer<int_best_score) { //New Score :D
      sprintf(gametimetxt,"New Best! :D %5.3f",print_time_ms);
      GrPrint(hdc,16+5,9+48,gametimetxt,LTPURPLE);
    } else {
      sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
      GrPrint(hdc,16+4,8+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+6,8+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+4,10+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+6,10+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+5,9+48,gametimetxt,BROWN);
    }
  }

  char gamebesttimetxt[32];
  double best_time=double_best_score;
  sprintf(gamebesttimetxt,"Best Time: %5.3f",best_time);
  GrPrint(hdc,16+4,8+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+6,8+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+4,10+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+6,10+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+5,9+64,gamebesttimetxt,BROWN);

  char enemykills[32];
  int printenemykills=ENEMY_NUM-enemy_kills;
  int printenemynum=ENEMY_NUM;
  sprintf(enemykills,"Enemies Left: %d/%d",printenemykills,printenemynum);
  GrPrint(hdc,16+4,8+80,enemykills,YELLOW);
  GrPrint(hdc,16+6,8+80,enemykills,YELLOW);
  GrPrint(hdc,16+4,10+80,enemykills,YELLOW);
  GrPrint(hdc,16+6,10+80,enemykills,YELLOW);
  GrPrint(hdc,16+5,9+80,enemykills,BROWN);
//========================


  DrawPlayingMusic(hdc,16+4,8,c,c4);



//=========Draw Player UI
  int i=0,j=0;
  int c2,c3;

  if (!IsInvertedBackground()) {
    c2=RED;
    c3=BLACK;
  } else {
    c2=LTCYAN;
    c3=CYAN;
  }      
  //draw player block health
  for (i=0;i<player.block_health;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,5,BLACK,BLACK);
  }

  //draw player health
  for (i=0;i<player.health;i++) {
    j=i/10; //new row of hearts
    if (i<20) {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,4,c2,c3);
    } else {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+50+8*j,3,c,YELLOW);
    }
  }

  //GrLine(hdc,GR_WIDTH/2,0,GR_WIDTH/2,GR_HEIGHT,BLACK);
  //draw player speed
  //hehehehe
  for (i=0;i<player.speed;i++) {
    double speed_angle=i*0.1;
    double speed_dist=64;
    double angle_limit=M_PI_4+M_PI_2;
    if (speed_angle>angle_limit) {
      int speed_times=speed_angle/angle_limit;
      speed_angle-=angle_limit*speed_times;
      speed_dist=64+8*speed_times;
      if (!IsInvertedBackground()) {
        c2=LTCYAN;
      } else {
        c2=RED;
      }      
    } else {
      if (!IsInvertedBackground()) {
        if (i<5) c2=GREEN;
        else if (i<10) c2=LTGREEN;
        else c2=RED;
      } else {
        if (i<5) c2=DKGRAY;
        else if (i<10) c2=LTPURPLE;
        else c2=LTCYAN;
      }
    }
      GrCircle(hdc,
        player.sprite_x-speed_dist*cos(speed_angle),
        player.sprite_y-speed_dist*sin(speed_angle),
        3,c2,c2
      );
  }


  //draw player web left
  for (i=0;i<player.max_web_num-player.placed_web_num;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+64+8*j,player.sprite_y+8*(i%10)-(11*8)/2,3,LTCYAN,CYAN);
  }



  int c5;
  if (!player.time_breaker) {
    c5=PURPLE;
  } else {
    c5=LTPURPLE;
  }
  //draw player time breaker
  for (i=0;i<player.time_breaker_units;i++) {
    double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*i*2;
    GrCircle(hdc,
      player.sprite_x-32*cos(tb_angle),
      player.sprite_y-32*sin(tb_angle),
      2,c5,c5);
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    int frame_t=frame_tick%20;
    for (i=0;i<3;i++) {
      double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*(frame_t-i)*2;
      GrCircle(hdc,
        player.sprite_x-32*cos(tb_angle),
        player.sprite_y-32*sin(tb_angle),
        3,LTPURPLE,LTPURPLE);
    }
  }

/*
Controls:
W - Jump from Surface
A - Move Left (Anti-Clockwise)
S - Block or Spin
D - Move Right (Clockwise)
Z - Time Breaker Ability
C - Increase Reaction Time
Space - Sprint
Left Click - Attack and Stop Web Shooting
Right Click - Shoot web
Enter - Restart Level

*While Swinging
W - Decrease Web Length
A - Swing Anti-clockwise
S - Increase Web Length
D - Swing Clockwise
Left Click - Swing without Web Placement
Right Click - Swing with Wceb Placement
*/

  if (display_controls) {
  GrRect(hdc,16,GR_HEIGHT-128-16*26,8*42,128+16*22,c4);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*25,"Controls:",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*24,"'W' - Jump from Surface",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*23,"'A' - Move Left (Clockwise)",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*22,"'S' - Block / Spin / Drag Down Attack",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*21,"'D' - Move Right (Anti-Clockwise)",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*20,"'Q' - Pick Up Web",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*19,"'E' - Hold with Attack for Uppercut",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*18,"'Z' - Time Breaker Ability",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*17,"'C' - Increase Reaction Time",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*16,"'M' - Next Music",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*15,"[SHIFT] + 'M' - Change Song Playing Mode",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*14,"[Space] - Sprint",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*13,"[Left Click] or '1' - Attack and Stop Web Shooting",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*12,"[Right Click] - Shoot web",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*11,"[Enter] - Restart Level",c);

  GrPrint(hdc,16+4,GR_HEIGHT-128-16*9,"Controls While Swinging:",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*8,"'W' - Decrease Web Length",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*7,"'A' - Swing Clockwise",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*6,"'S' - Increase Web Length",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*5,"'D' - Swing Anti-Clockwise",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*4,"[Left Click] or '1' - Swing without Web Placement",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*3,"[Right Click] - Swing with Web Placement",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16,"[SHIFT_ESC] to Quit",c);
  } 

  GrPrint(hdc,16+4,GR_HEIGHT-128+16,"Press '*' for Controls Help",c4);
  GrPrint(hdc,16+5,GR_HEIGHT-128+17,"Press '*' for Controls Help",c);
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
              if (level_chosen>7) {
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
              level_chosen=7;
            }
          }
          break;


        case VK_RETURN:
          if (!in_main_menu) {
            Init();
          } else {//Run Level
            if (player_color>-1 && player_color<COLORS_NUM) {
              if (level_chosen>=0 && level_chosen<=7)
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


        case 'C':
          if (!in_main_menu) {
            if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
              player.time_breaker=TRUE;
              //mciSendStringA("play snd/timeskip.mp3",NULL,0,NULL);
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
              //mciSendStringA("play snd/timeskip.mp3",NULL,0,NULL);
              player.time_breaker_cooldown=player.time_breaker_cooldown_max;
              player.speed+=player.time_breaker_units_max/2-1;
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
      }
      break;
    case WM_ERASEBKGND:
      if (!IsIconic(hwnd)) //no action when minimized
        InvalidateRect(hwnd,NULL,TRUE);
      return TRUE;
      break;


    case WM_PAINT: //https://cplusplus.com/forum/beginner/269434/
      FrameRateSleep(FPS); // (Uncapped)
      if (!IsIconic(hwnd)) //no action when minimized, prevents crash https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isiconic?redirectedfrom=MSDN
      {
        HBITMAP screen;
        PAINTSTRUCT ps;
        hdc=BeginPaint(hwnd, &ps);
       //FrameRateSleep(35); //35 or 60 fps Credit: ayevdood/sharoyveduchi && y4my4m - move it here


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
                DIR* dir;
                dir=opendir("score_saves");
                if (ENOENT==errno) {
                  mkdir("score_saves");
                }
                FILE *fptr;
                fptr = fopen(save_level,"w");
                char txt[12];
                int tmp_game_timer=game_timer;
                sprintf(txt,"%d\n",tmp_game_timer);
                fprintf(fptr,txt);
                fclose(fptr);
              }
              game_over=TRUE;
            }
          }

          if (!player.time_breaker) {
            PlayerCameraShake();
          } else {
            player.cam_move_x=0;
            player.cam_move_y=0;
          }
          for (int i=0;i<player.rendered_enemy_num;i++) {
            Enemy[player_render_enemies[i]].seed=rand();
          }


          if (player.health<=0) {
            Init();
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

          if (!IsInvertedBackground()){
            if (!player.time_breaker) {
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
            } else {            
              BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
            }
          } else {
            BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
          }
          DeleteDC(hdcBackbuff);
          DeleteObject(screen);

          //Trigger go back to main menu
          if (back_to_menu) {
            CleanUpPlayer();
            CleanUpEnemySprites();
            CleanUpGrid();
            CleanUpNodeGrid();
            CleanUpEnemy();
            CleanUpGround();
            save_level[0]='\0';

            DeleteObject(map_platforms_sprite);
            DeleteObject(map_platforms_sprite_mask);
            DeleteObject(map_platforms_timebreaker_sprite);
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
      //MessageBox(NULL, TEXT("ភាសាខ្មែរ"), TEXT("ភាសាខ្មែរ") ,MB_OK);
      HANDLE thread1=CreateThread(NULL,0,AnimateTask01,NULL,0,NULL); //Spawm Game Logic Thread
      HANDLE thread2=CreateThread(NULL,0,SongTask,NULL,0,NULL); //Spawn Song Player Thread

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
solar_sec);


  //) |> (|> 0 <|) <| ( @
  printf("\n~:: Lunar Hijri ::~ ");
  if (lunar_day>=1 && lunar_day<8) {
    printf("  )");
  } else if (lunar_day>=8 && lunar_day<11) {
    printf(" |)");
  }  else if (lunar_day>=11 && lunar_day<14) {
    printf("<|)");
  } else if (lunar_day>=14 && lunar_day<16) {
    printf("(O)");
  } else if (lunar_day>=16 && lunar_day<21) {
    printf("(|>");
  } else if (lunar_day>=21 && lunar_day<26) {
    printf("(|");
  } else if (lunar_day>=26 && lunar_day<28) {
    printf("(");
  } else {
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
lunar_sec);

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



      //Load Enemy Sprites
      enemy1_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy1-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy1_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy1-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

      enemy2_sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy2_sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      enemy2_sprite_3 = (HBITMAP) LoadImageW(NULL, L"sprites/enemy2-3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


      mouse_cursor_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/player_cursor1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,-1);



      //moon sprite
      if (lunar_day>=1 && lunar_day<8) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=8 && lunar_day<11) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-8.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      }  else if (lunar_day>=11 && lunar_day<14) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-11.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=14 && lunar_day<16) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-14.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=16 && lunar_day<21) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-16.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=21 && lunar_day<26) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-21.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else if (lunar_day>=26 && lunar_day<28) {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-26.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      } else {
        moon_sprite = (HBITMAP) LoadImageW(NULL, L"sprites/moon-28.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
      }
      //moon_sprite_cache=CreteLargeBitmap(NULL, 128, 128);
      moon_sprite_cache=RotateSprite(NULL, moon_sprite,0,LTGREEN,BLACK,-1);
      }
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
  RegisterClassW(&wc);

  //create window
  CreateWindowW(wc.lpszClassName,
                L"wInsecticide",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0,
                0,
                SCREEN_WIDTH,//GR_WIDTH+7,
                SCREEN_HEIGHT,//GR_HEIGHT+27,
                NULL,
                NULL,
                hInstance, 
                NULL);


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




