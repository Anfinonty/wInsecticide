
<<<<<<< HEAD

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent, HDC hdc)
{//http://www.winprog.org/tutorial/transparency.html
=======
HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.  

<<<<<<< HEAD
    GetObject(hbmColour, sizeof(BITMAP), &bm);
=======
    GetObject(hbmColour, sizeof(bm), &bm);
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

<<<<<<< HEAD
    hdcMem = CreateCompatibleDC(hdc);
    hdcMem2 = CreateCompatibleDC(hdc);
=======
    hdcMem = CreateCompatibleDC(0);
    hdcMem2 = CreateCompatibleDC(0);
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c

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
<<<<<<< HEAD
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);
=======
    BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, NOTSRCCOPY);
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c

    // Clean up.

    DeleteDC(hdcMem);
    DeleteDC(hdcMem2);

    return hbmMask;
}

<<<<<<< HEAD
=======

>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
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


void GrLine(HWND hwnd, HDC hdc, PAINTSTRUCT ps,double x1,double y1,double x2,double y2,int COLOR)
{
  HPEN hPen = CreatePen(PS_SOLID, 1, COLOR);
  HPEN hOldPen = SelectObject(hdc, hPen);
  MoveToEx(hdc,x1,y1,NULL);
  LineTo(hdc,x2,y2);
  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}


void GrCircle(HWND hwnd, HDC hdc, PAINTSTRUCT ps, double x, double y, int size, int COLOR)
{
//Shape Coordinates
  double x1=x-size;
  double y1=y-size;
  double x2=x+size;
  double y2=y+size;

  int left = min(x1, x2);
  int top = min(y1, y2);
  int right = max(x1, x2);
  int bottom = max(y1, y2);

  int length = min(abs(x2-x1), abs(y2-y1));
  if (x2 < x1)
    left = right - length;
  else
    right = left + length;

  if (y2 < y1)
    top = bottom - length;
  else
    bottom = top + length;

  HBRUSH hBrush,holdBrush;
  HPEN hPen,holdPen;

  hBrush = CreateSolidBrush(COLOR);
  hPen=CreatePen(PS_NULL,1,COLOR);
  holdPen=SelectObject(hdc,hPen);
  holdBrush=(HBRUSH) SelectObject(hdc,hBrush);

  Ellipse(hdc, left, top, right, bottom);

  SelectObject(hdc, holdBrush);
  SelectObject(hdc, holdPen);
  DeleteObject(hPen);
  DeleteObject(hBrush);
}


void GrPrint(HWND hwnd, HDC hdc, PAINTSTRUCT ps, double x1, double y1, char *_txt, int color)
{
  //DWORD color;
  //HFONT hFont, holdFont;
  //color=GetSysColor(COLOR_BTNFACE);
  //SetBkColor(hdc,color);
  //hFont=CreateFontW(15,0,0,0,FW_MEDIUM,0,0,0,0,0,0,0,0,L"Times New Roman");
  //holdFont=SelectObject(hdc,hFont);
  LPCSTR txt = _txt; //convert text to lpcstr
  SetTextColor(hdc, color); //set color of the text to be drawn
  SetBkMode(hdc, TRANSPARENT); //makes background of txt transparent  //https://stackoverflow.com/questions/10571966/draw-print-text-with-transparent-background-in-c-win32
  TextOutA(hdc, x1, y1, txt, strlen(txt)); //draw text to screen
}

<<<<<<< HEAD
void GrSprite(HWND hwnd, HDC hDC, PAINTSTRUCT ps,  double _x1, double _y1, double radians,HBITMAP hSourceBitmap,bool is_left,int rTransparent, int sprite_color) {
  if (hSourceBitmap != NULL) { ////https://ftp.zx.net.nz/pub/Patches/ftp.microsoft.com/MISC/KB/en-us/77/127.HTM
    HBITMAP hOldSourceBitmap, hOldDestBitmap, hDestBitmap; ////https://www.codeguru.com/multimedia/rotate-a-bitmap-image/
    HDC hMemSrc,hMemDest;
=======


void GrSprite(HWND hwnd, HDC hDC, PAINTSTRUCT ps,  double _x1, double _y1, double radians,HBITMAP hSourceBitmap,bool is_left)
{////https://ftp.zx.net.nz/pub/Patches/ftp.microsoft.com/MISC/KB/en-us/77/127.HTM
  if (hSourceBitmap != NULL) { 
    HGDIOBJ hOldSourceBitmap, hOldDestBitmap;
    static HBITMAP hDestBitmap; ////https://www.codeguru.com/multimedia/rotate-a-bitmap-image/
    HDC hMemSrc, hMemDest;
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
    BITMAP iSrcBitmap;

   // Step 1: Create a memory DC for the source and destination bitmaps
   //         compatible with the device used.
    hMemSrc = CreateCompatibleDC(hDC);
    hMemDest= CreateCompatibleDC(hDC);
<<<<<<< HEAD

=======
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c

  // Step 2: Get the height and width of the source bitmap.
    GetObject(hSourceBitmap, sizeof(iSrcBitmap), (LPSTR)&iSrcBitmap);

  // Get logical coordinates
    double cosine = (double)cos(radians);
    double sine = (double)sin(radians);

	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
    int x1 = (int)(-iSrcBitmap.bmHeight * sine);
    int y1 = (int)(iSrcBitmap.bmHeight * cosine);
    int x2 = (int)(iSrcBitmap.bmWidth * cosine - iSrcBitmap.bmHeight * sine);
    int y2 = (int)(iSrcBitmap.bmHeight * cosine + iSrcBitmap.bmWidth * sine);
    int x3 = (int)(iSrcBitmap.bmWidth * cosine);
    int y3 = (int)(iSrcBitmap.bmWidth * sine);

    int minx = min(0,min(x1, min(x2,x3)));
    int miny = min(0,min(y1, min(y2,y3)));
    int maxx = max(0,max(x1, max(x2,x3)));
    int maxy = max(0, max(y1, max(y2,y3)));

    int width = maxx - minx;
    int height = maxy - miny;

   // Step 3: Select the source bitmap into the source DC. Create a
   //         destination bitmap, and select it into the destination DC.

   //hDestBitmap = NULL;//CreateCompatibleBitmap(hMemDest, width, height);
<<<<<<< HEAD
    hDestBitmap = CreateBitmap(height, width, iSrcBitmap.bmPlanes,
                    iSrcBitmap.bmBitsPixel, NULL);


=======
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
    hOldSourceBitmap = SelectObject(hMemSrc, hSourceBitmap);

    hDestBitmap = CreateBitmap(width, height, iSrcBitmap.bmPlanes, iSrcBitmap.bmBitsPixel, NULL);
    hOldDestBitmap = SelectObject(hMemDest, hDestBitmap);

    //01-14-2024 Deprecated, off to a flight now bye!!
	// Draw the background color before we change mapping mode
<<<<<<< HEAD
     /*COLORREF clrBack = rTransparent; //For transparent background
=======
     /*COLORREF clrBack = RGB(255,255,255); //For transparent background
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
	 HBRUSH hbrBack = CreateSolidBrush( clrBack );
	 HBRUSH hbrOld = (HBRUSH) SelectObject( hMemDest, hbrBack );
	 PatBlt(hMemDest, 0, 0, width, height, PATCOPY );
	 DeleteObject( SelectObject(hMemDest, hbrOld ) );*/

	// Set mapping mode so that +ve y axis is upwords
	 SetMapMode(hMemSrc, MM_ISOTROPIC);
	 SetWindowExtEx(hMemSrc, 1,1,NULL);
	 SetViewportExtEx(hMemSrc, 1,-1,NULL);
	 SetViewportOrgEx(hMemSrc, 0, iSrcBitmap.bmHeight-1,NULL);

	 SetMapMode(hMemDest, MM_ISOTROPIC);
	 SetWindowExtEx(hMemDest, 1,1,NULL);
	 SetViewportExtEx(hMemDest, 1,-1,NULL);
	 SetWindowOrgEx(hMemDest, minx, maxy-1,NULL);


   // Step 4: Copy the pixels from the source to the destination.
    int current_pixel=0;
    for (int y=miny;y<maxy;y++) {
	  for(int x=minx;x<maxx;x++) {
	    int sourcex = (int)(x*cosine+y*sine);
	    int sourcey = (int)(y*cosine-x*sine);
	    if(sourcex>=0 && sourcex<iSrcBitmap.bmWidth && sourcey>=0
	   	   && sourcey<iSrcBitmap.bmHeight ) {
           current_pixel=GetPixel(hMemSrc,sourcex,sourcey);
          if (current_pixel==rTransparent) {
	        SetPixel(hMemDest, x, y, BLACK);
          } else if (current_pixel==BLACK){
            if (sprite_color!=BLACK) {
	          SetPixel(hMemDest, x, y, sprite_color);
            } else {
	          SetPixel(hMemDest, x, y, DKBLACK);
            }
          } else {
	        SetPixel(hMemDest, x, y, current_pixel);
          }
        }
      }
    }

   // Step 5: Destroy the DCs.
    SelectObject(hMemSrc, hOldSourceBitmap);
    SelectObject(hMemDest, hOldDestBitmap);
    DeleteDC(hMemDest);
    DeleteDC(hMemSrc);

    //Draw
    HDC hdcMem = CreateCompatibleDC(hDC);

<<<<<<< HEAD
=======

    //Transparency: http://www.winprog.org/tutorial/transparency.html
    BITMAP bitmap;
    static HBITMAP hBitmapMask;
    HBITMAP hDestBitmapCopy = CopyImage(hDestBitmap, IMAGE_BITMAP, 0,0, LR_DEFAULTSIZE);
    hBitmapMask = CreateBitmapMask(hDestBitmapCopy,BLACK);
    GetObject(hBitmapMask, sizeof(BITMAP), &bitmap); //handle bitmap
    HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmapMask);
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c


    //====================Begin Bitmapmask Creation
    HDC hdcMem1, hdcMem2;
    static HBITMAP hBitmapMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.  

    GetObject(hDestBitmap, sizeof(bm), &bm);
    hBitmapMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

    hdcMem1 = CreateCompatibleDC(hDC);
    hdcMem2 = CreateCompatibleDC(hDC);

    SelectObject(hdcMem1, hDestBitmap);
    SelectObject(hdcMem2, hBitmapMask);

    // Set the background colour of the colour image to the colour
    // you want to be transparent.
    SetBkColor(hdcMem1, BLACK);

    // Copy the bits from the colour image to the B+W mask... everything
    // with the background colour ends up white while everythig else ends up
    // black...Just what we wanted.

    BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem1, 0, 0, SRCCOPY);

    // Take our new mask and use it to turn the transparent colour in our
    // original colour image to black so the transparency effect will
    // work right.
    BitBlt(hdcMem1, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

    // Clean up.

    DeleteDC(hdcMem1);
    DeleteDC(hdcMem2);
    //==================End of Bitmap Map Creation


    /* Causes Memleak
    HBITMAP hDestBitmapCopy = CopyImage(hDestBitmap, IMAGE_BITMAP, 0,0, LR_DEFAULTSIZE);
    HDC hdcMem2 = CreateCompatibleDC(hdcMem);
    static HBITMAP hBitmapMask;
    hBitmapMask = CreateBitmapMask(hDestBitmapCopy, BLACK, hdcMem);*/


    HBITMAP oldbitmap,oldbitmap2;
    BITMAP bitmap;
    HDC hdcMemA = CreateCompatibleDC(hDC);
    HDC hdcMemB = CreateCompatibleDC(hDC);
    GetObject(hBitmapMask, sizeof(BITMAP), &bitmap);

    oldbitmap = SelectObject(hdcMemA, hBitmapMask);
    if (is_left) { //Flip Horizontally (X)
<<<<<<< HEAD
      StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMemA, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND); //Create Mask for
    } else { //Regular
      StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMemA, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND); //Create Mask for
    }


    oldbitmap2 = SelectObject(hdcMemB,hDestBitmap);
    if (is_left) { //Flip Horizontally (X)
      StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMemB, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCPAINT); //Create Mask for
    } else { //Regular
      StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMemB, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCPAINT); //Create Mask for
=======
      StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND); //Create Mask for
    } else { //Regular
      StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
    }

<<<<<<< HEAD
    // DeleteObject(SelectObject(hdcMem, hBitmapMask));
     //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
    DeleteObject(SelectObject(hdcMemB, oldbitmap2));
    DeleteObject(SelectObject(hdcMemA, oldbitmap)); //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
    DeleteDC(hdcMemA);
    DeleteDC(hdcMemB);
=======
    SelectObject(hdcMem,hDestBitmap);
    if (is_left) { //Flip Horizontally (X)
      StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCPAINT); //Create Mask for
    } else { //Regular
      StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCPAINT);
    }

    //BitBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, COPY); //This works (Demo, background and color)//Works with color
    
    DeleteObject(SelectObject(hdcMem, hDestBitmapCopy));
    DeleteObject(SelectObject(hdcMem, oldBitmap)); //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
    DeleteDC(hdcMem);
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
  } else {
    GrPrint(hwnd,hDC,ps,_x1,_y1,"(No Sprite)",RGB(255,255,255)); //Print Message if sprite cannot be loaded
  }
}



<<<<<<< HEAD
void DrawTriFill(HWND hwnd, HDC hdc, PAINTSTRUCT ps,int tri_color,double x1,double y1,double x2,double y2,double x3,double y3)
=======
/*void DrawTriFill(HWND hwnd, HDC hdc, PAINTSTRUCT ps,int tri_color,double x1,double y1,double x2,double y2,double x3,double y3)
{
  double x_1,x_2,
      gradient_middle1,gradient_middle2,gradient_largest,
      c_middle1,c_middle2,c_largest,
      smallest=9999,largest=0,
      x_arr[3],y_arr[3];
  x_arr[0]=x1;
  x_arr[1]=x2;
  x_arr[2]=x3;
  y_arr[0]=y1;
  y_arr[1]=y2;
  y_arr[2]=y3;
  int i,saved_largest=0,saved_smallest=0,saved_middle=0;
  for (i=0;i<3;i++) {
    if (y_arr[i]<smallest) {
      smallest=y_arr[i];
      saved_smallest=i;
    }
  }
  for (i=0;i<3;i++) {
    if (y_arr[i]>largest) {
      largest=y_arr[i];
      saved_largest=i;
    }
  }
  for (i=0;i<3;i++) {
    if (i!=saved_smallest && i!=saved_largest) {
      saved_middle=i;
    }
  }
  gradient_middle1=GetGradient(x_arr[saved_smallest],y_arr[saved_smallest],x_arr[saved_middle],y_arr[saved_middle]); //Gradient of main line
  c_middle1=GetGroundC(x_arr[saved_smallest],y_arr[saved_smallest],gradient_middle1);

  gradient_middle2=GetGradient(x_arr[saved_largest],y_arr[saved_largest],x_arr[saved_middle],y_arr[saved_middle]);
  c_middle2=GetGroundC(x_arr[saved_largest],y_arr[saved_largest],gradient_middle2);

  gradient_largest=GetGradient(x_arr[saved_largest],y_arr[saved_largest],x_arr[saved_smallest],y_arr[saved_smallest]);
  c_largest=GetGroundC(x_arr[saved_smallest],y_arr[saved_smallest],gradient_largest);
  for (i=smallest;i<y_arr[saved_middle];i+=1) {//small to middle
    x_1=GetX(i,gradient_middle1,c_middle1);
    x_2=GetX(i,gradient_largest,c_largest);
    GrLine(hwnd,hdc,ps,x_1,i,x_2,i,tri_color);
  }
  for (i=y_arr[saved_middle];i<largest;i+=1) {//middle to largest
    x_1=GetX(i,gradient_middle2,c_middle2);
    x_2=GetX(i,gradient_largest,c_largest);
    GrLine(hwnd,hdc,ps,x_1,i,x_2,i,tri_color);
  }
}*/




void DrawTriFill(HWND hwnd, HDC hdc, PAINTSTRUCT ps,int tri_color,double x1,double y1,double x2,double y2,double x3,double y3,bool isHatch)
>>>>>>> 6d6a5189a3d5e5e97d37d7f1d166c042c8addb6c
{//https://stackoverflow.com/questions/33447305/c-windows32-gdi-fill-triangle
  HPEN hPen = CreatePen(PS_SOLID, 2, tri_color);
  HPEN hOldPen = SelectObject(hdc, hPen);

  HBRUSH hBrush;
  if (!isHatch) {
    hBrush = CreateSolidBrush(tri_color);
  } else {
    hBrush = CreateHatchBrush(HS_BDIAGONAL,tri_color); //more cpu, but i may use for night levels
  }
  HBRUSH hOldBrush = SelectObject(hdc, hBrush);

  POINT vertices[] = { {x1, y1}, {x2, y2}, {x3, y3} };
  Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));

  SelectObject(hdc, hOldBrush);
  DeleteObject(hBrush);

  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}






#define COLORREF2RGB(Color) (Color & 0xff00) | ((Color >> 16) & 0xff) \
                                 | ((Color << 16) & 0xff0000)

//-------------------------------------------------------------------------------
// ReplaceColor
//
// Author    : Dimitri Rochette drochette@coldcat.fr
// Specials Thanks to Joe Woodbury for his comments and code corrections
//
// Includes  : Only <windows.h>

//
// hBmp         : Source Bitmap
// cOldColor : Color to replace in hBmp
// cNewColor : Color used for replacement
// hBmpDC    : DC of hBmp ( default NULL ) could be NULL if hBmp is not selected
//
// Retcode   : HBITMAP of the modified bitmap or NULL for errors
//
//-------------------------------------------------------------------------------
HBITMAP ReplaceColor(HBITMAP hBmp,COLORREF cOldColor,COLORREF cNewColor,HDC hBmpDC)
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
}
