//wInsecticide
//Jan-25-2023 Flicker Fix (be on fullscreen)
//Jan-26-2023 Full Flicker Fix n added GR
//Dec-09-2023 Added GrSprite
//Dec-10-2023 Added Sprite Flipping

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
#include <commctrl.h>

#define GR_WIDTH    640
#define GR_HEIGHT   480

#define PLATFORM_NUM    100
#define PLAYER_WIDTH    16
#define PLAYER_HEIGHT   16










//Maths
int RandNum(int min, int max) {return rand()%(max-min + 1) + min;}










//Graphics
void GrRect(HWND hwnd, HDC hdc, PAINTSTRUCT ps,double x,double y,int l, int h,int COLOR) {
  HBRUSH hBrush,holdBrush;
  HPEN hPen,holdPen;

  hBrush = CreateSolidBrush(COLOR);
  hPen=CreatePen(PS_NULL,1,COLOR);
  holdPen=SelectObject(hdc,hPen);
  holdBrush=(HBRUSH) SelectObject(hdc,hBrush);

  Rectangle(hdc,x,y,x+l,y+h);

  SelectObject(hdc, holdBrush);
  SelectObject(hdc, holdPen);
  DeleteObject(hPen);
  DeleteObject(hBrush);
}

void GrLine(HWND hwnd, HDC hdc, PAINTSTRUCT ps,double x1,double y1,double x2,double y2,int COLOR) {
  HPEN hPen = CreatePen(PS_SOLID, 1, COLOR);
  HPEN hOldPen = SelectObject(hdc, hPen);
  MoveToEx(hdc,x1,y1,NULL);
  LineTo(hdc,x2,y2);
  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}

//http://www.winprog.org/tutorial/transparency.html
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
  HDC hdcMem, hdcMem2;
  HBITMAP hbmMask;
  BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.
  GetObject(hbmColour, sizeof(BITMAP), &bm);
  hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver
  hdcMem = CreateCompatibleDC(0);
  hdcMem2 = CreateCompatibleDC(0);

  SelectObject(hdcMem, hbmColour);
  SelectObject(hdcMem2, hbmMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
  SetBkColor(hdcMem, crTransparent);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.
  BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
  BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.
  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return hbmMask;
}


//https://stackoverflow.com/questions/60922844/hbitmap-stretchblt-caused-image-saturation
HBITMAP ResizeBitmap(HDC hdc, HBITMAP source)
{
    HDC hMemDC1,hMemDC2;

    hMemDC2 = CreateCompatibleDC(hdc);
    HGDIOBJ hOld2 = SelectObject(hMemDC2, source);

    BITMAP bitmap = { 0 };
    GetObject(source, sizeof(BITMAP), &bitmap);

    hMemDC1 = CreateCompatibleDC(hdc);
    HBITMAP hBitmap1 = CreateCompatibleBitmap(hdc, bitmap.bmWidth, bitmap.bmHeight);
    HGDIOBJ hOld1 = SelectObject(hMemDC1, hBitmap1);

   //Flips horizontally (x)
    StretchBlt(hMemDC1, bitmap.bmWidth, 0, -bitmap.bmWidth-1, bitmap.bmHeight, hMemDC2, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

    SelectObject(hMemDC1, hOld1);
    SelectObject(hMemDC2, hOld2);

    DeleteDC(hMemDC1);
    DeleteDC(hMemDC2);

    return hBitmap1;
}


void GrSprite(HWND hwnd, HDC hdc, PAINTSTRUCT ps,  double x1, double y1, LPCWSTR filename, BOOL is_left) {
  static HBITMAP hBitmap;
  HBITMAP hBitmapMask;
  BITMAP bitmap;
  HDC hdcMem;
  HGDIOBJ oldBitmap;

  if (!is_left) {
    hBitmap=(HBITMAP) LoadImageW(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  } else {
    hBitmap=ResizeBitmap(hdc,(HBITMAP) LoadImageW(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE));
  }

  hBitmapMask = CreateBitmapMask(hBitmap, RGB(0,0,0));

  if (hBitmap!=NULL) {
    hdcMem = CreateCompatibleDC(hdc);

    oldBitmap = SelectObject(hdcMem, hBitmap);
    GetObject(hBitmap, sizeof(bitmap), &bitmap);

    BitBlt(hdc, x1-bitmap.bmWidth/2, y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCAND);
    SelectObject(hdcMem, oldBitmap);

    SelectObject(hdcMem, hBitmapMask);
    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCAND);

    SelectObject(hdcMem, hBitmap);
    BitBlt(hdc, 0, bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCPAINT);

    DeleteDC(hdcMem);
    DeleteObject(hBitmap);
    DeleteObject(hBitmapMask);
  }
}

void GrPrint(HWND hwnd, HDC hdc, PAINTSTRUCT ps, double x1, double y1, LPCWSTR txt) {
 // DWORD color;
 // HFONT hFont, holdFont;
 // color=GetSysColor(COLOR_BTNFACE);
//  SetBkColor(hdc,TRANSPARENT);
//  hFont=CreateFontW(15,0,0,0,FW_MEDIUM,0,0,0,0,0,0,0,0,L"Calbri");
//  holdFont=SelectObject(hdc,hFont);
  TextOutW(hdc, x1, y1, txt, lstrlenW(txt));
//  SelectObject(hdc, holdFont);
//  DeleteObject(hFont);
}







//Player
struct player {
  bool rst_left,rst_right,rst_up,is_left;
  int grav, jump_height, sprite_timer;
  double x,y;
} player;

void InitPlayer() {
  player.grav=1;
  player.sprite_timer=0;
  player.rst_left=player.rst_right=player.rst_up=false;
  player.is_left=false;
  player.jump_height=0;
  player.x=GR_WIDTH/2;
  player.y=GR_HEIGHT/2;
}

void PlayerAct() {
  //gravity
  if (player.jump_height<-10 && player.grav<2) {
    player.grav++;
  }
  player.y+=1;
  player.jump_height--;
  //movement
  if (player.jump_height==62) {
    player.jump_height=0;
  }
  if (player.rst_up && player.jump_height==63) {
    player.jump_height=50;
  }
  if (0<player.jump_height && player.jump_height<51) {
    player.y-=2;
  }
  if (player.rst_left && player.x>0) {
    player.x--;
    player.is_left=true;
    if (player.sprite_timer==0)
      player.sprite_timer=20;
  } else if (player.rst_right && player.x<GR_WIDTH) {
    player.x++;
    player.is_left=false;
    if (player.sprite_timer==0)
      player.sprite_timer=20;
  }
  if (player.sprite_timer>0) {
    player.sprite_timer--;
  }
}


void DrawPlayer(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  if (player.sprite_timer<10) {
    GrSprite(hwnd,hdc,ps,player.x,player.y,L"player1.bmp",player.is_left);
  } else {
    GrSprite(hwnd,hdc,ps,player.x,player.y,L"player2.bmp",player.is_left);
  }
  //GrRect(hwnd,hdc,ps,player.x,player.y,PLAYER_WIDTH,PLAYER_HEIGHT,RGB(0, 76, 255));
}










//Platform
struct Platform {
  double x;
  double y;
  int length;
} platform[PLATFORM_NUM];


void SetPlatform(int i,int y) {
  platform[i].x=8*RandNum(0,GR_WIDTH/8);
  platform[i].y=y;
  platform[i].length=8*RandNum(1,8);
}


void InitPlatform() {
  platform[0].x=player.x-20;
  platform[0].y=player.y+PLAYER_HEIGHT;
  platform[0].length=40;
  for (int i=1;i<PLATFORM_NUM;i++) {
    SetPlatform(i,8*RandNum(0,GR_HEIGHT/8));
  }
}


void PlatformAct(int pid,int g) {
  //gravity
  if (g==0) {
    platform[pid].y+=0.5;
  }
  if (platform[pid].y>GR_HEIGHT) {
    SetPlatform(pid,8*RandNum(0,4));
  }
  //collision
  if ((platform[pid].y-2<=player.y && player.y<=platform[pid].y) &&
       ((platform[pid].x-16<=player.x && player.x<=platform[pid].x+platform[pid].length+16) ||
        (platform[pid].x-16<=player.x && player.x<=platform[pid].x+platform[pid].length+16)
      )) {
    player.grav=1;
    player.jump_height=64;
    player.y-=2;
  }
}


void DrawPlatforms(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  for (int i=0;i<PLATFORM_NUM;i++) {
    GrLine(hwnd,hdc,ps,platform[i].x,platform[i].y,platform[i].x+platform[i].length,platform[i].y,RGB(255,255,255));
  }
}










//Background
void DrawBackground(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
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
  bool b=1;
  char x;
  while (b) {
    if (player.y>GR_HEIGHT+8) {
      Init();
    }
    for (int j=0;j<player.grav;j++) {
      for (int i=0;i<PLATFORM_NUM;i++) {
        PlatformAct(i,j);
      }
      PlayerAct();
    }
    usleep(6000);
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
