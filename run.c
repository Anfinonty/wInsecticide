//wInsecticide
//Jan-25-2023 Flicker Fix (be on fullscreen)
//Jan-26-2023 Full Flicker Fix n added GR
//Dec-09-2023 Added GrSprite
//Dec-10-2023 Added Sprite Flipping
//Dec-14-2023 Added Sprite Win32 Compatibility && fixed sluggishness for win32
//Dec-21-2023 Sleep() for while loop, Fixed memleak caused by flipping sprite

//Command
//i686-w64-mingw32-gcc-win32 run.c -o run.exe -lopengl32 -lglu32 -lgdi32 -municode

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <synchapi.h>

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



//Background
void DrawBackground(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
}


void DrawTexts(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  //GrPrint(hwnd,hdc,ps,0,0,L"Hello Game!");
}


//Init
void Init() {
  InitGrid();
  InitNodeGrid();
  InitGround();
  //BulletInit;
  //EnemyInit;
  InitNodeGridAttributes();
  //BackgroundInit;
  InitPlayer();
  //GroundInit2;
}






DWORD WINAPI AnimateTask01(LPVOID lpArg) {
  bool b=true;
  while (b) {
    if (player.y>GR_HEIGHT+8) {//restart if fell out of the world
      Init();
    }

    PlayerAct();
    /*for (int j=0;j<player.grav;j++) {
      for (int i=0;i<GROUND_NUM;i++) { //gravity ground
        GroundAct(i,j);
      }
      PlayerAct();
    }*/
    Sleep(4); //Returned from sharoyveduchi's and sledixyz's feedback'
  }
}




DWORD WINAPI SongTask(LPVOID lpArg) {
  //PlaySoundA("Linkin_Park_08-In_The_End.wav", NULL, SND_FILENAME);
  PlaySoundA("music/bfmv.wav",NULL,SND_FILENAME);
}






LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  HBITMAP bitmap;
  PAINTSTRUCT ps;
  Sleep(4);
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
      InvalidateRect(hwnd,NULL,1);
      return 1;
      break;
    case WM_PAINT: //https://cplusplus.com/forum/beginner/269434/
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

      DeleteDC(hdcBackbuff);
      DeleteObject(bitmap);
      EndPaint(hwnd, &ps);
      return 1;
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
  int i=0;
  int *lpArgPtr;
  HANDLE hHandles[1];
  DWORD ThreadId;
  for (i=0;i<2;i++) {
    lpArgPtr=(int *)malloc(sizeof(int));
    *lpArgPtr=i;
    switch (i) {
      case 0: hHandles[i]=CreateThread(NULL,0,AnimateTask01,lpArgPtr,0,&ThreadId);break;
      case 1: hHandles[i]=CreateThread(NULL,0,SongTask,lpArgPtr,0,&ThreadId);break;
    }
  }

  MSG msg;
  while (GetMessage(&msg,NULL,0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
//    Sleep(4);
  }
  return (int) msg.wParam;
}
