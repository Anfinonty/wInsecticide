#include <windows.h>

//#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) | ((Color << 16) & 0xff0000)
/*HBITMAP ReplaceColor(HBITMAP hBmp,COLORREF cOldColor,COLORREF cNewColor,HDC hBmpDC)
{ //https://www.codeproject.com/Articles/2841/How-to-replace-a-color-in-a-HBITMAP
    HBITMAP RetBmp=NULL;
    if (hBmp)
    {
        HDC BufferDC=CreateCompatibleDC(NULL);    // DC for Source Bitmap
        if (BufferDC)
        {
            HBITMAP hTmpBitmap = (HBITMAP) NULL;
            if (hBmpDC)
                if (hBmp == (HBITMAP)GetCurrentObject(hBmpDC, OBJ_BITMAP))
            {
                hTmpBitmap = CreateBitmap(1, 1, 1, 1, NULL);
                SelectObject(hBmpDC, hTmpBitmap);
            }

            HGDIOBJ PreviousBufferObject=SelectObject(BufferDC,hBmp);
            // here BufferDC contains the bitmap
            
            HDC DirectDC=CreateCompatibleDC(NULL); // DC for working
            if (DirectDC)
            {
                // Get bitmap size
                BITMAP bm;
                GetObject(hBmp, sizeof(bm), &bm);

                // create a BITMAPINFO with minimal initilisation 
                // for the CreateDIBSection
                BITMAPINFO RGB32BitsBITMAPINFO; 
                ZeroMemory(&RGB32BitsBITMAPINFO,sizeof(BITMAPINFO));
                RGB32BitsBITMAPINFO.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
                RGB32BitsBITMAPINFO.bmiHeader.biWidth=bm.bmWidth;
                RGB32BitsBITMAPINFO.bmiHeader.biHeight=bm.bmHeight;
                RGB32BitsBITMAPINFO.bmiHeader.biPlanes=1;
                RGB32BitsBITMAPINFO.bmiHeader.biBitCount=32;

                // pointer used for direct Bitmap pixels access
                UINT * ptPixels;    

                HBITMAP DirectBitmap = CreateDIBSection(DirectDC, 
                                       (BITMAPINFO *)&RGB32BitsBITMAPINFO, 
                                       DIB_RGB_COLORS,
                                       (void **)&ptPixels, 
                                       NULL, 0);
                if (DirectBitmap)
                {
                    // here DirectBitmap!=NULL so ptPixels!=NULL no need to test
                    HGDIOBJ PreviousObject=SelectObject(DirectDC, DirectBitmap);
                    BitBlt(DirectDC,0,0,
                                   bm.bmWidth,bm.bmHeight,
                                   BufferDC,0,0,SRCCOPY);

                       // here the DirectDC contains the bitmap

                    // Convert COLORREF to RGB (Invert RED and BLUE)
                    cOldColor=COLORREF2RGB(cOldColor);
                    cNewColor=COLORREF2RGB(cNewColor);

                    // After all the inits we can do the job : Replace Color
                    for (int i=((bm.bmWidth*bm.bmHeight)-1);i>=0;i--)
                    {
                        if (ptPixels[i]==cOldColor) ptPixels[i]=cNewColor;
                    }
                    // little clean up
                    // Don't delete the result of SelectObject because it's 
                    // our modified bitmap (DirectBitmap)
                       SelectObject(DirectDC,PreviousObject);

                    // finish
                    RetBmp=DirectBitmap;
                }
                // clean up
                DeleteDC(DirectDC);
            }            
            if (hTmpBitmap)
            {
                SelectObject(hBmpDC, hBmp);
                DeleteObject(hTmpBitmap);
            }
            SelectObject(BufferDC,PreviousBufferObject);
            // BufferDC is now useless
            DeleteDC(BufferDC);
        }
    }
    return RetBmp;
}*/


//Graphics
void GrRect(HWND hwnd, HDC hdc, PAINTSTRUCT ps,double x,double y,int l, int h,int COLOR)
{
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



HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.  

    GetObject(hbmColour, sizeof(bm), &bm);
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
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, NOTSRCCOPY);

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PWSTR lpCmdLine, int nCmdShow) {

    MSG  msg;
    WNDCLASSW wc = {0};

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpszClassName = L"Draw Bitmap";
    wc.hInstance     = hInstance;
    wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);

    RegisterClassW(&wc);
    CreateWindowW(wc.lpszClassName, L"Draw Bitmap",
          WS_OVERLAPPEDWINDOW | WS_VISIBLE,
          100, 100, 300, 300, NULL, NULL, hInstance, NULL);

    while (GetMessage(&msg, NULL, 0, 0)) {
        
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg,
    WPARAM wParam, LPARAM lParam) {

    static HBITMAP hBitmap;
    static HBITMAP hBitmapMask;
    HDC hdc;
    HBITMAP oldBitmap;

    switch(msg) {

        case WM_CREATE:     
             hBitmap = (HBITMAP) LoadImageW(NULL, L"player-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE); //White background
             break; 



        case WM_PAINT:
           {
             PAINTSTRUCT ps;

             hdc = BeginPaint(hwnd, &ps);
             HBITMAP screen = CreateCompatibleBitmap(hdc,300,300);


             HDC hdcMem = CreateCompatibleDC(hdc);
             SelectObject(hdcMem,screen); //begin drawing on screen

            //Draw Rectangle
              HBRUSH hBrush,holdBrush;
              HPEN hPen,holdPen;

              hBrush = CreateSolidBrush(RGB(0,255,0));
              hPen=CreatePen(PS_NULL,1,RGB(0,255,0));
              holdPen=SelectObject(hdcMem,hPen);
              holdBrush=(HBRUSH) SelectObject(hdcMem,hBrush);

              Rectangle(hdcMem,0,0,150,200);
              //BitBlt(hdc, 0,0, 300,300,hdcMem, 0 ,0 ,SRCCOPY); //draw from hdcMem to HDC
              SelectObject(hdcMem, holdBrush);
              SelectObject(hdcMem, holdPen);
              DeleteObject(hPen);
              DeleteObject(hBrush);
           
             //BitBlt(hdc, 0,0, 300, 300, hdcMem, 0, 0, SRCCOPY);
             //DeleteDC(hdcMem);

            //Draw Bitmap
              //Create Bitmap Mask
              HBITMAP holdBitmap = CopyImage(hBitmap, IMAGE_BITMAP, 0,0, LR_DEFAULTSIZE);
              hBitmapMask = CreateBitmapMask(holdBitmap, RGB(0,0,0));


              HDC hdcMem2 = CreateCompatibleDC(hdcMem); //bitmap mem
              BITMAP bitmap;
              GetObject(hBitmapMask, sizeof(bitmap), &bitmap);
              //Paint Mask
              SelectObject(hdcMem2, hBitmapMask); 
              BitBlt(hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem2, 0,0, SRCAND); //draw to hdcMem

              //Paint colors
              SelectObject(hdcMem2, hBitmap);
              BitBlt(hdcMem, 0, bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem2, 0,0, SRCPAINT); //draw to hdcMem 

              //Cleanup drawn bitmap
              //DeleteObject(SelectObject(hdcMem2, oldBitmap));
              DeleteDC(hdcMem2);

              //DrawScreen
              BitBlt(hdc, 0,0, 300, 300, hdcMem, 0, 0, SRCCOPY);

             DeleteDC(hdcMem);
             DeleteObject(screen);
             EndPaint(hwnd, &ps);
             return 0;
           }
           break;

        case WM_DESTROY:

            DeleteObject(hBitmap);
            PostQuitMessage(0);
            
            return 0;
    }

    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

