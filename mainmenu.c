//wInsecticide Main Menu 02-06-2024

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


#define GR_WIDTH    640
#define GR_HEIGHT   480
int mouse_x,mouse_y;
int level_chosen=0;

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

#include "gr.c"

//#include "saves/Level001.c"
//#include "saves/Level002.c"
//#include "saves/Level003.c"
//#include "saves/Level004.c"
//#include "saves/Level005.c"
//#include "saves/Level006.c"
//#include "saves/Level007.c"
//#include "saves/Level008.c"


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
  mouse_x=0;
  mouse_y=0;
  InitTickFrequency();
  InitFPS();
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



DWORD WINAPI AnimateTask01(LPVOID lpArg)
{
  bool b=TRUE;
  while (b) { //every 1 second
    Sleep(1000);
  }
}

void DrawBackground(HDC hdc)
{
  GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,BLUE);
}


void DrawTexts(HDC hdc)
{
  GrPrint(hdc,10,10,"Welcome to the wInsecticide Menu! (02-06-2024)",WHITE);

  GrPrint(hdc,10,10+32,"-  Level 0",WHITE);
  GrPrint(hdc,10,10+32+16,"-  Level 1",WHITE);
  GrPrint(hdc,10,10+32+16*2,"-  Level 2",WHITE);
  GrPrint(hdc,10,10+32+16*3,"-  Level 3",WHITE);
  GrPrint(hdc,10,10+32+16*4,"-  Level 4",WHITE);
  GrPrint(hdc,10,10+32+16*5,"-  Level 5",WHITE);
  GrPrint(hdc,10,10+32+16*6,"-  Level 6",WHITE);
  GrPrint(hdc,10,10+32+16*7,"-  Level 7",WHITE);

  GrPrint(hdc,10,10+32+16*9,"Press 'Enter' to Compile and Run",WHITE);
  GrPrint(hdc,10,10+32+16*10,"Use Up or Down Keys to Select a Level",WHITE);

  GrPrint(hdc,10,10+32+16*level_chosen,">",WHITE);
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  HDC hdc, hdcBackbuff;
  //FrameRateSleep(FPS); // (Uncapped)
  switch(msg) {
    /*case WM_LBUTTONDOWN:
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
      break;*/
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
      /*switch (wParam) {
        case 'S':case VK_DOWN:player.rst_down=TRUE;break;
        case 'D':case VK_RIGHT:player.rst_right=TRUE;break;
        case 'A':case VK_LEFT:player.rst_left=TRUE;break;
        case 'W':case VK_UP:player.rst_up=TRUE;break;
        case VK_RETURN:Init();break;
        case ' ':player.rst_key_sprint=TRUE;break;
	    case 'E':
	      player.uppercut=TRUE;
	      break;
      }*/
      break;
    case WM_KEYUP:
      switch (wParam) {
        case VK_DOWN:
          level_chosen++;
          if (level_chosen>7) {
            level_chosen=0;
          }
          break;

        case VK_UP:
          level_chosen--;
          if (level_chosen<0) {
            level_chosen=7;
          }
          break;

        case VK_RETURN:
          //Create a New file called load_level.c
          if (level_chosen>-1 && level_chosen<8){
            FILE *fptr;
            fptr = fopen("load_level.c","w");
            fprintf(fptr,"#include \"saves/Level00%d.c\"",level_chosen);
            fclose(fptr);
            /*system("cd mingw32/bin && i686-w64-mingw32-gcc.exe ../../run.c -o ../../tmp.exe -lgdi32 -municode -lwinmm && cd ../../ && tmp.exe");*/
            
            system("rungame.exe");
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
      FrameRateSleep(FPS); // (Uncapped)

      PAINTSTRUCT ps;
      hdc=BeginPaint(hwnd, &ps);
      hdcBackbuff=CreateCompatibleDC(hdc);
      HBITMAP screen=CreateCompatibleBitmap(hdc,GR_WIDTH,GR_HEIGHT);
      SelectObject(hdcBackbuff,screen);
      
      DrawBackground(hdcBackbuff);
      DrawTexts(hdcBackbuff);

      BitBlt(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcBackbuff, 0, 0,  SRCCOPY);

      DeleteDC(hdcBackbuff);
      DeleteObject(screen);
      EndPaint(hwnd, &ps);
      return 0;
    }
      break;
    case WM_CREATE:
      srand(time(NULL));
      ShowCursor(TRUE);
      InitOnce();
      return 0;
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
      break;
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
                L"wInsecticide Main Menu (02-06-2024)",
                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
                0,
                0,
                GR_WIDTH,
                GR_HEIGHT,
                NULL,
                NULL,
                hInstance, 
                NULL);


  //threads
  int *lpArgPtr;
  HANDLE hHandles[1];
  DWORD ThreadId;
  for (int i=0;i<1;i++) {
    lpArgPtr=(int *)malloc(sizeof(int));
    *lpArgPtr=i;
    switch (i) {
      case 0: hHandles[i]=CreateThread(NULL,0,AnimateTask01,lpArgPtr,0,&ThreadId);break;
    }
  }


  MSG msg;
  while (GetMessage(&msg,NULL,0,0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return (int) msg.wParam;
}




