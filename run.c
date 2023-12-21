//wInsecticide
//Jan-25-2023 Flicker Fix (be on fullscreen)
//Jan-26-2023 Full Flicker Fix n added GR
//Dec-09-2023 Added GrSprite
//Dec-10-2023 Added Sprite Flipping
//Dec-14-2023 Added Sprite Win32 Compatibility && fixed sluggishness for win32
//Dec-21-2023 Delay works across different windows machines

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
#define GR_HEIGHT   600


#define PLATFORM_NUM    100
#define PLAYER_WIDTH    16
#define PLAYER_HEIGHT   16


#include "math.c"
#include "gr.c"
#include "player.c"
#include "platform.c"

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
  InitPlayer();
  InitPlatform();
}







DWORD WINAPI AnimateTask01(LPVOID lpArg) {
  bool b=true;
  while (b) {
    if (player.y>GR_HEIGHT+8) {//restart if fell out of the world
      Init();
    }

    for (int j=0;j<player.grav;j++) {
      for (int i=0;i<PLATFORM_NUM;i++) { //gravity platform
        PlatformAct(i,j);
      }
      PlayerAct();
    }
    Sleep(1); //Returned from sharoyveduchi's and sledixyz's feedback'
  }
}










LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  HBITMAP bitmap;
  PAINTSTRUCT ps;
  switch(msg) {
    case WM_KEYDOWN:
      switch (wParam) {
        case 'D':case VK_RIGHT:player.rst_right=true;break;
        case 'A':case VK_LEFT:player.rst_left=true;break;
        case 'W':case VK_UP:player.rst_up=true;break;
        case VK_RETURN:Init();break;
      }
      break;
    case WM_KEYUP:
      switch (wParam) {
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
      bitmap=CreateCompatibleBitmap(hdc,1000,1000);
      SelectObject(hdcBackbuff,bitmap);

      DrawBackground(hwnd,hdcBackbuff,ps);
      DrawPlatforms(hwnd,hdcBackbuff,ps);
      DrawPlayer(hwnd,hdcBackbuff,ps);
      DrawTexts(hwnd,hdcBackbuff,ps);

      BitBlt(hdc, 0, 0, 1000, 1000, hdcBackbuff, 0, 0, SRCCOPY);
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
  lpArgPtr=(int *)malloc(sizeof(int));
  *lpArgPtr=i;
  hHandles[i]=CreateThread(NULL,0,AnimateTask01,lpArgPtr,0,&ThreadId);

  MSG  msg;
  while (GetMessage(&msg,NULL,0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int) msg.wParam;
}
