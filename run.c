//wInsecticide
//Jan-25-2023 Flicker Fix (be on fullscreen)
//Jan-26-2023 Full Flicker Fix n added GR
//Dec-09-2023 Added GrSprite
//Dec-10-2023 Added Sprite Flipping
//Dec-14-2023 Added Sprite Win32 Compatibility && fixed sluggishness for win32
//Dec-21-2023 Sleep() for while loop, Fixed memleak caused by flipping sprite
//Jan-06-2023 Added FPS Sleep. Code Credit: geissomatik

//Command
//i686-w64-mingw32-gcc-win32 run.c -o run.exe  -lgdi32 -municode -lwinmm
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
#define LTBLUE      RGB(8,39,245)
#define LTGREEN     RGB(0,255,0)
#define LTCYAN      RGB(0,191,255)
#define LTRED       RGB(255,71,76)
#define LTPURPLE    RGB(255,0,255)
#define YELLOW      RGB(255,255,0)
#define WHITE       RGB(255,255,255)

#define DKBLACK     RGB(5,5,5) //For drawing
#define MYCOLOR1    RGB(123,123,123)

//int play_a_sound;

int color_arr[COLORS_NUM]={
BLACK,
BLUE,
GREEN,
CYAN,
RED,
PURPLE,
BROWN,
LTGRAY,
DKGRAY,
LTBLUE,
LTGREEN,
LTCYAN,
LTRED,
LTPURPLE,
YELLOW,
WHITE
};


/*int palette_dark_arr[COLORS_NUM]={
BLACK,
BLUE,
GREEN,
CYAN,
RED,
PURPLE,
BROWN,
LTGRAY,
DKGRAY,
LTBLUE,
LTGREEN,
LTCYAN,
LTRED,
LTPURPLE,
YELLOW,
WHITE
};*/


#define SONG_NUM 10
#define SONG_FOLDER_NUM 15

#define SCREEN_WIDTH    (GetSystemMetrics(SM_CXSCREEN))
#define SCREEN_HEIGHT   (GetSystemMetrics(SM_CYSCREEN))

int GR_WIDTH,GR_HEIGHT,OLD_GR_WIDTH,OLD_GR_HEIGHT;
int dyn_vrenderdist=0,dyn_vrenderdist_num=0;
int frame_tick=0;

#define DEFAULT_PLAYER_SPEED			1

#define ENEMY_TYPE_NUM                         10


#define PLAYER_WIDTH 	32
#define PLAYER_HEIGHT 	32

#define GRID_SIZE	160
#define VGRID_SIZE	160
#define GRID_NUM	(MAP_WIDTH/GRID_SIZE) * (MAP_HEIGHT/GRID_SIZE)
#define VGRID_NUM	(MAP_WIDTH/VGRID_SIZE) * (MAP_HEIGHT/VGRID_SIZE)

#define NODE_SIZE  	 10
#define MAX_FOLLOW_RANGE 100
#define MAX_NODE_NUM	 MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE

#define MAP_NODE_NUM     (MAP_WIDTH/NODE_SIZE) * (MAP_HEIGHT/NODE_SIZE)

#define MAX_GROUNDS_WITHIN_GRID	(VGRID_SIZE/NODE_SIZE)*(VGRID_SIZE/NODE_SIZE)/2


#define VRENDER_DIST      20
#define VRDGRID_NUM       VRENDER_DIST*VRENDER_DIST

#define RENDER_DIST	 9
#define RDGRID_NUM	 RENDER_DIST*RENDER_DIST

#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			30

#define BULLET_NUM	5000
#define MAX_BULLET_PER_FIRE 10


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

#define MAX_WEB_NUM      100







//#include "saves/Level001.c"
#include "saves/Level002.c"
//#include "saves/Level003.c"
//#include "saves/Level004.c"
//#include "saves/Level005.c"
//#include "saves/Level006.c"
//#include "saves/Level007.c"
//#include "saves/Level008.c"

/*
#define ENEMY_I64_ATTRIBUTES_NUM 26
#define ENEMY_F64_ATTRIBUTES_NUM 4
#define ENEMY_BOOL_ATTRIBUTES_NUM 1

#define GROUND_F64_ATTRIBUTES_NUM 6
#define GROUND_BOOL_ATTRIBUTES_NUM 1
#define GROUND_I64_ATTRIBUTES_NUM 2
#define GROUND_U8_ATTRIBUTES_NUM 1

#define PLAYER_COMBO_TIME_LIMIT 351
#define BUILD_RANGE   	60

#define DEFAULT_PLAYER_SND_DURATION	10
#define DEFAULT_PLAYER_SND_PITCH	90
#define DEFAULT_PLAYER_SND_RAND		10

#define PERFORMANCE_TXT_TIMER_MAX	201

#define GAMEMODE_NUM	2
#define OPTION_NUM	4
#define COMBO_NUM	3
*/

#include "math.c"
#include "gr.c"

#include "struct_classes.c"
#include "grid.c"
#include "ground.c"
#include "bullet.c"
#include "player.c"
#include "enemy.c"
#include "song.c"

//Background
void DrawBackground(HDC hdc) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255))); //RAVE
  switch (map_background) {
    case 0:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,TRUE);
      break;
    case 1:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,NOTSRCCOPY,TRUE);
      break;
    default:
      break;
  }
}

void DrawPlatforms(HDC hDC)
{ //Dynamically scale with window size 
  //Draw platforms bitmap mask
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

int FPS = 60;
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

  player.cam_move_x=0,
  player.cam_move_y=0,

  dyn_vrenderdist=ceil(GR_WIDTH/100)+1;
  dyn_vrenderdist_num=dyn_vrenderdist*dyn_vrenderdist;

  InitTickFrequency();
  InitFPS();
}

void Init() {
  OLD_GR_WIDTH=GR_WIDTH;
  OLD_GR_HEIGHT=GR_HEIGHT;
  InitSongBank();
  InitGrid();
  InitNodeGrid();
  InitGround();
  InitBullet();
  InitEnemy();
  InitNodeGridAttributes();
  InitPlayer();
  InitGround2();
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
  bool b=TRUE;
  while (b) {
    PlayerAct();
    for (int i=0;i<player.rendered_enemy_num;i++) {
      EnemyAct(player.render_enemies[i]);
    }
    //GroundAct();
    //SongAct();
    if (player.health<1) {
      Init();
    }
    Sleep(player.sleep_timer);
    //Sleep(6);
  }
}

bool display_controls=FALSE;
void DrawTexts(HDC hdc) {
  int c;
  if (!IsInvertedBackground()) {
    c=WHITE;
  } else {
    c=BLACK;
  }



  if (song_folder_num>0) {
    char txt[64];
    char *song_name=song_names[rand_song1][rand_song2];
    //int sec = (song_time_end-time_now)%60;
    int sec = (song_seconds_run_max-song_seconds_run)%60;
    if (sec>-1) {
      if (sec>9)
        //sprintf(txt,"%s [%d:%d]",song_name,(song_time_end-time_now)/60,sec);
        sprintf(txt,"%s [%d:%d]",song_name,(song_seconds_run_max-song_seconds_run)/60,sec);
      else 
        //sprintf(txt,"%s [%d:0%d]",song_name,(song_time_end-time_now)/60,sec);
        sprintf(txt,"%s [%d:0%d]",song_name,(song_seconds_run_max-song_seconds_run)/60,sec);

      char txt2[128];
      char *album_name=/*album_name_arr[*/album_names[rand_song1][rand_song2]/*]*/;
      sprintf(txt2,"%s",album_name);  

      GrPrint(hdc,4,0,txt,c);
      GrPrint(hdc,4,16,txt2,c);
    } else {
      GrPrint(hdc,4,0,"Choosing Song...",c);
    }
  }
  //GrPrint(hwnd,hdc,ps,0,0,_txt,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255)));


//Print player details

  //print player web amount
  /*char txt[2];
  int print_web_num=player.max_web_num-player.placed_web_num;
  sprintf(txt,"%d",print_web_num);
  if (!IsInvertedBackground()) {
    GrPrint(hDC,mouse_x+32,mouse_y+32,txt,WHITE);
  } else {
    GrPrint(hDC,mouse_x+32,mouse_y+32,txt,BLACK);
  }*/

  //print player health
  //int print_player_health=player.health;
  //sprintf(txt,"%d",print_player_health);
  //GrPrint(hDC,mouse_x-32,mouse_y+32,txt,LTRED);
  int i=0,j=0;
  int c2;


  //draw player block health
  for (i=0;i<player.block_health;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,6,DKGRAY,-1);
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,4,DKGRAY,-1);
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,2,DKGRAY,-1);
  }

  //draw player health
  for (i=0;i<player.health;i++) {
    j=i/10; //new row of hearts
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,2,RED,RED);
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
        if (i<5)
         c2=GREEN;
        else if (i<10)
          c2=LTGREEN;
        else 
          c2=RED;
      } else {
        if (i<5)
          c2=DKGRAY;
        else if (i<10)
          c2=LTPURPLE;
        else
          c2=LTCYAN;
      }
    }
    //GrCircle(hdc,player.sprite_x-64,player.sprite_y-8*i+(10*8)/2,3,c2,c2);
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



  //draw player time breaker
  for (i=0;i<player.time_breaker_units;i++) {
    double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*i*2;
    GrCircle(hdc,
      player.sprite_x-32*cos(tb_angle),
      player.sprite_y-32*sin(tb_angle),
      2,PURPLE,PURPLE);
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    int frame_t=frame_tick%20;
    for (i=0;i<3;i++) {
      double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*(frame_t-i)*2;
      GrCircle(hdc,
        player.sprite_x-32*cos(tb_angle),
        player.sprite_y-32*sin(tb_angle),
        3,LTPURPLE,-1);
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
  GrPrint(hdc,4,GR_HEIGHT-80-16*22,"Controls:",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*21,"'W' - Jump from Surface",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*20,"'A' - Move Left (Clockwise)",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*19,"'S' - Block or Spin",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*18,"'D' - Move Right (Anti-Clockwise)",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*17,"'Q' - Pick Up Web",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*16,"'E' - Hold with Attack for Uppercut",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*15,"'Z' - Time Breaker Ability",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*14,"'C' - Increase Reaction Time",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*13,"'M' - New Random Music",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*12,"[Space] - Sprint",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*11,"[Left Click] or '1' - Attack and Stop Web Shooting",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*10,"[Right Click] - Shoot web",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*9,"[Enter] - Restart Level",c);

  GrPrint(hdc,4,GR_HEIGHT-80-16*7,"Controls While Swinging:",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*6,"'W' - Decrease Web Length",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*5,"'A' - Swing Clockwise",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*4,"'S' - Increase Web Length",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*3,"'D' - Swing Anti-Clockwise",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16*2,"[Left Click] or '1' - Swing without Web Placement",c);
  GrPrint(hdc,4,GR_HEIGHT-80-16,"[Right Click] - Swing with Web Placement",c);
  }

  GrPrint(hdc,4,GR_HEIGHT-80+16,"Press '0' for Controls Help",c);



  /*char txt3[10];
  int print_1=frame_tick;
  sprintf(txt3,"%d",print_1);  
  GrPrint(hdc,0,32,txt3,c);*/

  /*char txt3[10];
  int print_1=player.time_breaker_recharge_timer;
  sprintf(txt3,"%d",print_1);  
  GrPrint(hdc,0,32,txt3,c);


  char txt4[10];
  int print_2= player.time_breaker_cooldown;
  sprintf(txt4,"%d",print_2);  
  GrPrint(hdc,0,48,txt4,c);


  char txt5[10];
  int print_3=player.time_breaker_units;
  sprintf(txt5,"%d",print_3);  
  GrPrint(hdc,0,64,txt5,c);*/







  /*for (int i=player.block_health;i>0;i--) {
    GrCircle(hdc,player.sprite_x+8*i,player.sprite_y+32,2,LTGRAY,LTGRAY);
  }*/

  //Print Player Block Health
  //int print_block_health=player.block_health;
  //sprintf(txt,"%d",print_block_health);
  //GrPrint(hDC,mouse_x,mouse_y+32,txt, BLACK);


/*  char txt3[19];
  int print_dynrenderdist=dyn_vrenderdist;
  sprintf(txt3,"Render Distance: %d",print_dynrenderdist);  
  GrPrint(hdc,0,32,txt3,c);


  char txt4[10];
  int print_left = player.previous_web_placed;
  int print_left2= player.on_ground_id;
  sprintf(txt4,"%d webplaced: %d",print_left2, print_left);  
  GrPrint(hdc,0,48,txt4,c);*/


  /*char txt5[10];
  int print_speed=player.speed;
  int print_grav_speed=player.grav;
  sprintf(txt5,"Speed:%d  %d",print_grav_speed, print_speed);  
  GrPrint(hdc,0,64,txt5,c);*/
  //GrPrint(hwnd,hdc,ps,0,16,_txt2,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255)));


  /*char txt6[16];
  int print_jump=player.in_air_timer;
  sprintf(txt6,"inairtimer: %d",print_jump);  
  GrPrint(hdc,0,80,txt6,c);*/
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  //FrameRateSleep(FPS); // (Uncapped)
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
          mouse_x=point.x;
          mouse_y=point.y;
        }
        UpdateWindow(hwnd);
      }
      break;
    case WM_KEYDOWN:
      switch (wParam) {
        case 'S':case VK_DOWN:player.rst_down=TRUE;break;
        case 'D':case VK_RIGHT:player.rst_right=TRUE;break;
        case 'A':case VK_LEFT:player.rst_left=TRUE;break;
        case 'W':case VK_UP:player.rst_up=TRUE;break;
        case VK_RETURN:Init();break;
        case ' ':player.rst_key_sprint=TRUE;break;
	    case 'E':
	      player.uppercut=TRUE;
	      break;
      }
      break;
    case WM_KEYUP:
      switch (wParam) {
        case 'Q':player.destroy_ground=TRUE;break;
        case 'S':case VK_DOWN:if(player.rst_down)player.rst_down=FALSE;break;
        case 'D':case VK_RIGHT:if(player.rst_right)player.rst_right=FALSE;break;
        case 'A':case VK_LEFT:if(player.rst_left)player.rst_left=FALSE;break;
        case 'W':case VK_UP:if(player.rst_up)player.rst_up=FALSE;break;
        case 'M':song_seconds_run_max=-1;play_new_song=FALSE;break;//end current song
        case '0':
          if (!display_controls) {
            display_controls=TRUE;
          } else {
            display_controls=FALSE;
          }
          break;
        case ' ':if(player.rst_key_sprint)player.rst_key_sprint=FALSE;break;
        case 'C':
          if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
            player.time_breaker=TRUE;
            player.time_breaker_cooldown=player.time_breaker_cooldown_max;
            player.speed+=player.time_breaker_units_max/2-1;
          }
          if (player.sleep_timer==DEFAULT_SLEEP_TIMER) {
            player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
          } else {
            player.sleep_timer=DEFAULT_SLEEP_TIMER;
          }
          break;
        case 'Z':
          if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
            player.time_breaker=TRUE;
            player.time_breaker_cooldown=player.time_breaker_cooldown_max;
            player.speed+=player.time_breaker_units_max/2-1;
          }
          break;
	    case '1':
	      player.attack_rst=TRUE;
	      break;
	    case 'E':
          if (player.uppercut) {
	        player.uppercut=FALSE;
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
    if (!IsIconic(hwnd)) //no action when minimized, prevents crash https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-isiconic?redirectedfrom=MSDN
    { //https://stackoverflow.com/questions/752593/win32-app-suspends-on-minimize-window-animation
      //FrameRateSleep(35); //35 or 60 fps Credit: ayevdood/sharoyveduchi && y4my4m - move it here
      frame_tick++;
      if (frame_tick>9000) {
        frame_tick=0;
      }
      FrameRateSleep(FPS); // (Uncapped)
      PlayerCameraShake();
      for (int i=0;i<player.rendered_enemy_num;i++) {
        Enemy[player.render_enemies[i]].seed=rand();
      }
      RECT rect;
      if(GetWindowRect(hwnd, &rect))
      {
        GR_WIDTH = rect.right - rect.left;
        GR_HEIGHT = rect.bottom - rect.top;        
      }

      if (GR_WIDTH!=OLD_GR_WIDTH || GR_HEIGHT!=OLD_GR_HEIGHT) {
        InitPlayerCamera();
        player.cam_x=0;
        player.cam_y=0;
        dyn_vrenderdist=ceil(GR_WIDTH/100)+1;
        dyn_vrenderdist_num=dyn_vrenderdist*dyn_vrenderdist;
        //bg_cam_fall_cooldown=0;
        //background_cam_move_x=0;
        //background_cam_move_y=0;
        CameraInit(player.x,player.y+PLAYER_HEIGHT/2); //idk scaling is weird for sprite
        OLD_GR_WIDTH = GR_WIDTH;
        OLD_GR_HEIGHT = GR_HEIGHT;
      }

      PAINTSTRUCT ps;
      hdc=BeginPaint(hwnd, &ps);
      hdcBackbuff=CreateCompatibleDC(hdc);
      HBITMAP screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
      SelectObject(hdcBackbuff,screen);
      
      DrawBackground(hdcBackbuff);
      DrawPlatforms(hdcBackbuff);
      DrawWebs(hdcBackbuff);
      DrawEnemy(hdcBackbuff);
      DrawPlayer(hdcBackbuff);
      DrawCursor(hdcBackbuff);
      DrawTexts(hdcBackbuff);

      if (!IsInvertedBackground()){
        if (!player.time_breaker) {
          BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
        } else {
          BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
        }
      } else {
        if (!player.time_breaker) {
          BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  NOTSRCCOPY);
        } else {
          BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);
        }
      }
  //      StretchBlt(hdc, GR_WIDTH/2, -GR_HEIGHT, -GR_WIDTH-1, GR_HEIGHT, hdcBackbuff, 0, 0, GR_WIDTH, GR_HEIGHT,     SRCCOPY);
      DeleteDC(hdcBackbuff);
      DeleteObject(screen);
      EndPaint(hwnd, &ps);
      return 0;
    }
      break;
    case WM_CREATE:
      srand(time(NULL));
      timeBeginPeriod(1);
      ShowCursor(FALSE);

      InitOnce();//cannot be repeatedly run
      Init();


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


      //Create Platforms Sprite
      //BITMAP bm;
      PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
      hdc=BeginPaint(hwnd, &ps);
      HDC hdc2=CreateCompatibleDC(hdc);
      //HBITMAP tmp_map_platforms_sprite=CreateCompatibleBitmap(hdc,MAP_WIDTH,MAP_HEIGHT);
      //https://forums.codeguru.com/showthread.php?526563-Accessing-Pixels-with-CreateDIBSection
      unsigned char* lpBitmapBits; 

      BITMAPINFO bi; 
      ZeroMemory(&bi, sizeof(BITMAPINFO));
      bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
      bi.bmiHeader.biWidth=MAP_WIDTH;
      bi.bmiHeader.biHeight=-MAP_HEIGHT;
      bi.bmiHeader.biPlanes=1;
      bi.bmiHeader.biBitCount=32;
      HBITMAP tmp_map_platforms_sprite=CreateDIBSection(hdc2,&bi,DIB_RGB_COLORS, (VOID**)&lpBitmapBits,NULL,0);

      SelectObject(hdc2,tmp_map_platforms_sprite);
      if (map_background==2)
        GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,custom_map_background_color);
      else 
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



      //Load Player cache Sprites
      player.sprite_jump_cache = RotateSprite(NULL, player.sprite_jump,player.sprite_angle,LTGREEN,BLACK,-1);
      player.sprite_1_cache = RotateSprite(NULL, player.sprite_1,player.sprite_angle,LTGREEN,BLACK,-1);
      player.sprite_2_cache = RotateSprite(NULL, player.sprite_2,player.sprite_angle,LTGREEN,BLACK,-1);

      player.attack_sprite_1_cache = RotateSprite(NULL, player.attack_sprite_1,player.sprite_angle,LTGREEN,BLACK,-1);
      player.attack_sprite_2_cache = RotateSprite(NULL, player.attack_sprite_2,player.sprite_angle,LTGREEN,BLACK,-1);
      player.attack_sprite_3_cache = RotateSprite(NULL, player.attack_sprite_3,player.sprite_angle,LTGREEN,BLACK,-1);
      player.attack_sprite_4_cache = RotateSprite(NULL, player.attack_sprite_4,player.sprite_angle,LTGREEN,BLACK,-1);

      player.block_sprite_1_cache = RotateSprite(NULL, player.block_sprite_1,player.sprite_angle,LTGREEN,BLACK,-1);
      player.block_sprite_2_cache = RotateSprite(NULL, player.block_sprite_2,player.sprite_angle,LTGREEN,BLACK,-1);
      player.block_sprite_3_cache = RotateSprite(NULL, player.block_sprite_3,player.sprite_angle,LTGREEN,BLACK,-1);

      player.spin_sprite_1_cache = RotateSprite(NULL, player.spin_sprite,0.1,LTGREEN,BLACK,-1);
      player.spin_sprite_2_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI_2,LTGREEN,BLACK,-1);
      player.spin_sprite_3_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI,LTGREEN,BLACK,-1);
      player.spin_sprite_4_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI+M_PI_2,LTGREEN,BLACK,-1);

      mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,-1);

      //Load Enemy cache sprites
      InitEnemySprites();



      //Load Map Background sprites
      switch (map_background) {
        case 0:
          map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
          break;
        case 1:
          map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
          break;
        default:
          map_background_sprite=NULL;
          break;
      } 
      return 0;
      break;
    case WM_DESTROY:
      CleanUpPlayer();
      CleanUpEnemySprites();
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
  wc.style = 0;//CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpszClassName = L"DrawIt";
  wc.hInstance     = hInstance;
  wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
  wc.lpfnWndProc   = WndProc;
  wc.hCursor       = LoadCursor(0, IDC_ARROW);
  RegisterClassW(&wc);

  //create window
  CreateWindowW(wc.lpszClassName,
                L"wInsecticide (Press [Enter] to Restart)",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0,
                0,
                SCREEN_WIDTH,//GR_WIDTH+7,
                SCREEN_HEIGHT,//GR_HEIGHT+27,
                NULL,
                NULL,
                hInstance, 
                NULL);


  //threads
  int *lpArgPtr;
  HANDLE hHandles[2];
  DWORD ThreadId;
  for (int i=0;i<2;i++) {
    lpArgPtr=(int *)malloc(sizeof(int));
    *lpArgPtr=i;
    switch (i) {
      case 0: hHandles[i]=CreateThread(NULL,0,AnimateTask01,lpArgPtr,0,&ThreadId);break;
      case 1: hHandles[i]=CreateThread(NULL,0,SongTask,lpArgPtr,0,&ThreadId);break;
      //case 2: hHandles[i]=CreateThread(NULL,0,SoundTask,lpArgPtr,0,&ThreadId);break;
    }
  }


  MSG msg;
  while (GetMessage(&msg,NULL,0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  timeEndPeriod(1);
  return (int) msg.wParam;
}




