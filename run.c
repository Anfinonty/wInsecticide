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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <synchapi.h>
#include <dirent.h>
#include <errno.h>

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



int color_arr[16]={
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


#define SONG_NUM 10
#define SONG_FOLDER_NUM 13

#define GR_WIDTH    800
#define GR_HEIGHT   800

#define DEFAULT_PLAYER_JUMP_HEIGHT 		85//100
#define DEFAULT_PLAYER_SPEED			1

#define ENEMY_TYPE_NUM                         10


#define PLAYER_WIDTH 	32
#define PLAYER_HEIGHT 	32

#define GRID_SIZE	160
#define GRID_NUM	(MAP_WIDTH/GRID_SIZE) * (MAP_HEIGHT/GRID_SIZE)

#define NODE_SIZE  	 10
#define MAX_FOLLOW_RANGE 100
#define MAX_NODE_NUM	 MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE
#define MAP_NODE_NUM     (MAP_WIDTH/NODE_SIZE) * (MAP_HEIGHT/NODE_SIZE)

#define MAX_GROUNDS_WITHIN_GRID	(GRID_SIZE/NODE_SIZE)*(GRID_SIZE/NODE_SIZE)/2

#define RENDER_DIST	 9
#define RDGRID_NUM	 RENDER_DIST*RENDER_DIST

#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			30

#include "saves/Level001.c"

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


#define MAX_WEB_NUM      100

#define RDGRID_NUM	 RENDER_DIST*RENDER_DIST

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
#define DEFAULT_PLAYER_SPEED_BREAKER_MAX	275
#define DEFAULT_PLAYER_SPEED_BREAKER_COOLDOWN   100
#define DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN	100
#define DEFAULT_PLAYER_SND_DURATION	10
#define DEFAULT_PLAYER_SND_PITCH	90
#define DEFAULT_PLAYER_SND_RAND		10
#define DEFAULT_PLAYER_BLOCK_HEALTH_MAX 20

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
#include "player.c"
//#include "enemy.c"
//#include "bullet.c"
#include "song.c"

//Background
void DrawBackground(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255))); //RAVE
  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,custom_map_background_color);
}


LARGE_INTEGER m_high_perf_timer_freq;
LARGE_INTEGER m_prev_end_of_frame;  
void InitTickFrequency() {
  if (!QueryPerformanceFrequency(&m_high_perf_timer_freq))
      m_high_perf_timer_freq.QuadPart = 0;
  m_prev_end_of_frame.QuadPart = 0;
}
//Init

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
  InitTickFrequency();
  InitFPS();
}

void Init() {
  InitSongBank();
  InitGrid();
  InitNodeGrid();
  InitGround();
  //InitBullet();
  //InitEnemy();
  InitNodeGridAttributes();
  //InitBackground;
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
    /*for (int i=0;i<player.rendered_enemy_num;i++) {
      EnemyAct(player.render_enemies[i]);
    }*/
    GroundAct();
    SongAct();
    Sleep(DEFAULT_SLEEP_TIMER);
  }
}


void DrawTexts(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  char txt[64];
  char *song_name=song_names[rand_song1][rand_song2];
  int sec = (song_time_end-time_now)%60;
  if (sec>9)
    sprintf(txt,"%s [%d:%d]",song_name,(song_time_end-time_now)/60,sec);
  else 
    sprintf(txt,"%s [%d:0%d]",song_name,(song_time_end-time_now)/60,sec);

  GrPrint(hwnd,hdc,ps,0,0,txt,RGB(255,255,255));

//  GrPrint(hwnd,hdc,ps,0,0,_txt,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255)));

  char txt2[64];
  char *album_name=album_names[rand_song1][rand_song2];
  sprintf(txt2,"%s",album_name);
  GrPrint(hwnd,hdc,ps,0,16,txt2,RGB(255,255,255));
  //GrPrint(hwnd,hdc,ps,0,16,_txt2,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255)));
}




LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  HBITMAP bitmap;
  switch(msg) {
    case  WM_MOUSEMOVE: //https://stackoverflow.com/questions/22039413/moving-the-mouse-blocks-wm-timer-and-wm-paint
      UpdateWindow(hwnd);
      break;
    case WM_KEYDOWN:
      switch (wParam) {
        case 'S':case VK_DOWN:player.rst_down=TRUE;break;
        case 'D':case VK_RIGHT:player.rst_right=TRUE;break;
        case 'A':case VK_LEFT:player.rst_left=TRUE;break;
        case 'W':case VK_UP:player.rst_up=TRUE;break;
        case VK_RETURN:Init();break;
        case ' ':player.rst_key_sprint=TRUE;break;
      }
      break;
    case WM_KEYUP:
      switch (wParam) {
        case 'S':case VK_DOWN:if(player.rst_down)player.rst_down=FALSE;break;
        case 'D':case VK_RIGHT:if(player.rst_right)player.rst_right=FALSE;break;
        case 'A':case VK_LEFT:if(player.rst_left)player.rst_left=FALSE;break;
        case 'W':case VK_UP:if(player.rst_up)player.rst_up=FALSE;break;
        case 'M':song_time_end=0;play_new_song=FALSE;break;//end current song
        case ' ':if(player.rst_key_sprint)player.rst_key_sprint=FALSE;break;
      }
      break;
    case WM_ERASEBKGND:
      InvalidateRect(hwnd,NULL,TRUE);
      return TRUE;
      break;
    case WM_PAINT: //https://cplusplus.com/forum/beginner/269434/
    {
      //FrameRateSleep(35); //35 or 60 fps Credit: ayevdood/sharoyveduchi && y4my4m - move it here
      FrameRateSleep(FPS); // (Uncapped)
      PAINTSTRUCT ps;
      hdc=BeginPaint(hwnd, &ps);
      hdcBackbuff=CreateCompatibleDC(hdc);
      bitmap=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
      SelectObject(hdcBackbuff,bitmap);

      DrawBackground(hwnd,hdcBackbuff,ps);
      //DrawGrid(hwnd,hdcBackbuff,ps);
      DrawGroundTriFill(hwnd,hdcBackbuff,ps);
      DrawGround(hwnd,hdcBackbuff,ps); //limited
      DrawGroundText(hwnd,hdcBackbuff,ps);

      DrawPlayer(hwnd,hdcBackbuff,ps);
      DrawTexts(hwnd,hdcBackbuff,ps);

      BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0, SRCCOPY);
//      StretchBlt(hdc, GR_WIDTH/2, -GR_HEIGHT, -GR_WIDTH-1, GR_HEIGHT, hdcBackbuff, 0, 0, GR_WIDTH, GR_HEIGHT, SRCCOPY);
      DeleteDC(hdcBackbuff);
      DeleteObject(bitmap);
      EndPaint(hwnd, &ps);
      return 0;
    }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProcW(hwnd, msg, wParam, lParam);
}





//




int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow) {
  //Window Class
  timeBeginPeriod(1);

  WNDCLASSW wc = {0};
  wc.style = 0;//CS_HREDRAW | CS_VREDRAW;
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
                GR_WIDTH+7,
                GR_HEIGHT+27,
                NULL,
                NULL,
                hInstance,
                NULL);

  //Init
  srand(time(NULL));

  InitOnce();//cannot be repeatedly run
  Init();

  //threads
  int *lpArgPtr;
  HANDLE hHandles[1];
  DWORD ThreadId;
  for (int i=0;i<1;i++) {
    lpArgPtr=(int *)malloc(sizeof(int));
    *lpArgPtr=i;
    switch (i) {
//      case 0: hHandles[i]=CreateThread(NULL,0,SongTask,lpArgPtr,0,&ThreadId);break;
      case 0: hHandles[i]=CreateThread(NULL,0,AnimateTask01,lpArgPtr,0,&ThreadId);break;
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
