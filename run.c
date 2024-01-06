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
//#include <profileapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
//#include <synchapi.h>

#define GR_WIDTH    800
#define GR_HEIGHT   800

#define MAP_WIDTH GR_WIDTH
#define MAP_HEIGHT GR_HEIGHT



#define DEFAULT_PLAYER_JUMP_HEIGHT 		85//100
#define DEFAULT_PLAYER_SPEED			1


#define GROUND_NUM    2
#define PLAYER_WIDTH 	25
#define PLAYER_HEIGHT 	15

#define GRID_SIZE	160
#define GRID_NUM	(MAP_WIDTH/GRID_SIZE) * (MAP_HEIGHT/GRID_SIZE)

#define NODE_SIZE  	 10
#define MAX_FOLLOW_RANGE 100
#define MAX_NODE_NUM	 MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE
#define MAP_NODE_NUM     (MAP_WIDTH/NODE_SIZE) * (MAP_HEIGHT/NODE_SIZE)

#define MAX_GROUNDS_WITHIN_GRID	(GRID_SIZE/NODE_SIZE)*(GRID_SIZE/NODE_SIZE)/2

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
#define GRID_SIZE	160
#define GRID_NUM	(MAP_WIDTH/GRID_SIZE) * (MAP_HEIGHT/GRID_SIZE)

#define NODE_SIZE  	 10
#define MAX_FOLLOW_RANGE 100
#define MAX_NODE_NUM	 MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE
#define MAP_NODE_NUM     (MAP_WIDTH/NODE_SIZE) * (MAP_HEIGHT/NODE_SIZE)

#define MAX_GROUNDS_WITHIN_GRID	(GRID_SIZE/NODE_SIZE)*(GRID_SIZE/NODE_SIZE)/2

#define MAX_WEB_NUM      100

#define RENDER_DIST	 9
#define RDGRID_NUM	 RENDER_DIST*RENDER_DIST

#define BULLET_NUM	5000
#define MAX_BULLET_PER_FIRE 10
#define DEFAULT_PLAYER_HEALTH			20
#define DEFAULT_PLAYER_JUMP_HEIGHT 		85//100
#define DEFAULT_PLAYER_ATTACK_STRENGTH  	1
#define DEFAULT_PLAYER_KNOCKBACK_STRENGTH	50

#define DEFAULT_SLEEP_TIMER			6
#define SLOWDOWN_SLEEP_TIMER			30

#define DEFAULT_SLEEP_TIMER_ZEAL		3
#define SLOWDOWN_SLEEP_TIMER_ZEAL		20

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

#define DEFAULT_MUSIC_TEMPO	5.100
*/

#include "math.c"
#include "gr.c"

#include "struct_classes.c"

//#include "classvalues.c" //Set Class default amount
//#include "class.c" //Call all classes

#include "ground.c"
#include "player.c"
#include "grid.c"

//#include <commctrl.h>assign bitmap variable C win32

//int machine_speed;

//Background
void DrawBackground(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
}


void DrawTexts(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  //GrPrint(hwnd,hdc,ps,0,0,L"Hello Game!");
}



//int machine_speed=999999;

LARGE_INTEGER m_high_perf_timer_freq;
LARGE_INTEGER m_prev_end_of_frame;  
//Init
void Init() {
//  machine_speed=134217728*2/machinespeed();

  InitGrid();
  InitNodeGrid();
  InitGround();
  //BulletInit;
  //EnemyInit;
  InitNodeGridAttributes();
  //BackgroundInit;
  InitPlayer();

  //GroundInit2;

  if (!QueryPerformanceFrequency(&m_high_perf_timer_freq))
      m_high_perf_timer_freq.QuadPart = 0;
  m_prev_end_of_frame.QuadPart = 0;

}




DWORD WINAPI AnimateTask01(LPVOID lpArg) {
  bool b=true;
  
  LARGE_INTEGER t;
  int max_fps = 507;
  timeBeginPeriod(1);

  while (b) {
    PlayerAct();

    QueryPerformanceCounter(&t);

    if (m_prev_end_of_frame.QuadPart != 0) //http://www.geisswerks.com/ryan/FAQS/timing.html https://github.com/geissomatik
    {
      int ticks_to_wait = (int)m_high_perf_timer_freq.QuadPart / max_fps;
      int done = 0;
      do
      {
        QueryPerformanceCounter(&t);
        
        int ticks_passed = (int)((int)t.QuadPart - (int)m_prev_end_of_frame.QuadPart);
        int ticks_left = ticks_to_wait - ticks_passed;

        if (t.QuadPart < m_prev_end_of_frame.QuadPart)    // time wrap
          done = 1;
        if (ticks_passed >= ticks_to_wait)
          done = 1;
       
        if (!done)
        {
            // if > 0.002s left, do Sleep(1), which will actually sleep some 
            //   steady amount, probably 1-2 ms,
            //   and do so in a nice way (cpu meter drops; laptop battery spared).
            // otherwise, do a few Sleep(0)'s, which just give up the timeslice,
            //   but don't really save cpu or battery, but do pass a tiny
            //   amount of time.
          if (ticks_left > m_high_perf_timer_freq.QuadPart*2/1000)
            Sleep(1);
          else                        
            for (int i=0; i<10; i++) 
              Sleep(0);  // causes thread to give up its timeslice
        }
      } while (!done);            
    }
    m_prev_end_of_frame = t;
  }
  timeEndPeriod(1);
}



DWORD WINAPI SongTask(LPVOID lpArg) {
  //PlaySoundA("Linkin_Park_08-In_The_End.wav", NULL, SND_FILENAME);
  srand(time(NULL));
  char songname[11];
  int rand_song=RandNum(0,9);
  //printf("%d\n",rand_song);
  sprintf(songname,"music/%d.wav",rand_song);
  //printf("%s\n",songname);
  PlaySoundA(songname,NULL,SND_FILENAME);
}






LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  HBITMAP bitmap;
  switch(msg) {
    case WM_KEYDOWN:
      switch (wParam) {
        case 'S':case VK_DOWN:player.rst_down=true;break;
        case 'D':case VK_RIGHT:player.rst_right=true;break;
        case 'A':case VK_LEFT:player.rst_left=true;break;
        case 'W':case VK_UP:player.rst_up=true;break;
        case VK_RETURN:Init();break;
      }
      break;
    case WM_KEYUP:
      switch (wParam) {
        case 'S':case VK_DOWN:if(player.rst_down)player.rst_down=false;break;
        case 'D':case VK_RIGHT:if(player.rst_right)player.rst_right=false;break;
        case 'A':case VK_LEFT:if(player.rst_left)player.rst_left=false;break;
        case 'W':case VK_UP:if(player.rst_up)player.rst_up=false;break;
      }
      break;
    case WM_ERASEBKGND:
      InvalidateRect(hwnd,NULL,true);
      return true;
      break;
    case WM_PAINT: //https://cplusplus.com/forum/beginner/269434/
    {
      PAINTSTRUCT ps;
      hdc=BeginPaint(hwnd, &ps);
      hdcBackbuff=CreateCompatibleDC(hdc);
      bitmap=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
      SelectObject(hdcBackbuff,bitmap);

      DrawBackground(hwnd,hdcBackbuff,ps);
      DrawGrid(hwnd,hdcBackbuff,ps);
      DrawGrounds(hwnd,hdcBackbuff,ps);
      DrawPlayer(hwnd,hdcBackbuff,ps);
      DrawTexts(hwnd,hdcBackbuff,ps);

      BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0, SRCCOPY);
//      StretchBlt(hdc, GR_WIDTH/2, -GR_HEIGHT, -GR_WIDTH-1, GR_HEIGHT, hdcBackbuff, 0, 0, GR_WIDTH, GR_HEIGHT, SRCCOPY);
      SwapBuffers(hdc); //instead of Sleep();
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
  Init();

  //threads
  int *lpArgPtr;
  HANDLE hHandles[2];
  DWORD ThreadId;
  for (int i=0;i<2;i++) {
    lpArgPtr=(int *)malloc(sizeof(int));
    *lpArgPtr=i;
    switch (i) {
      case 0: hHandles[i]=CreateThread(NULL,0,SongTask,lpArgPtr,0,&ThreadId);break;
      case 1: hHandles[i]=CreateThread(NULL,0,AnimateTask01,lpArgPtr,0,&ThreadId);break;
    }
  }

  MSG msg;
  while (true) {
    if (PeekMessage(&msg,NULL,0,0, PM_REMOVE)) {
      if (msg.message==WM_QUIT) break;
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  return (int) msg.wParam;
}
