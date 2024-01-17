

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent, HDC hdc)
{//http://www.winprog.org/tutorial/transparency.html
    HDC hdcMem, hdcMem2;
    HBITMAP hbmMask;
    BITMAP bm;

    // Create monochrome (1 bit) mask bitmap.  

    GetObject(hbmColour, sizeof(BITMAP), &bm);
    hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

    // Get some HDCs that are compatible with the display driver

    hdcMem = CreateCompatibleDC(hdc);
    hdcMem2 = CreateCompatibleDC(hdc);

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

//Graphics
void GrRect(HDC hdc, double x,double y,int l, int h,int COLOR) {
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


void GrLine(HDC hdc, double x1,double y1,double x2,double y2,int COLOR) {
  HPEN hPen = CreatePen(PS_SOLID, 1, COLOR);
  HPEN hOldPen = SelectObject(hdc, hPen);
  MoveToEx(hdc,x1,y1,NULL);
  LineTo(hdc,x2,y2);
  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}


void GrCircle(HDC hdc, double x, double y, int size, int COLOR) {
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


void GrPrint(HDC hdc, double x1, double y1, char *_txt, int color) {
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

void GrSprite(HDC hDC,double _x1,double _y1,double radians,HBITMAP hSourceBitmap,bool is_left,int rTransparent, int sprite_color) {
  if (hSourceBitmap != NULL) { ////https://ftp.zx.net.nz/pub/Patches/ftp.microsoft.com/MISC/KB/en-us/77/127.HTM
    HBITMAP hOldSourceBitmap, hOldDestBitmap, hDestBitmap; ////https://www.codeguru.com/multimedia/rotate-a-bitmap-image/
    HDC hMemSrc,hMemDest;
    BITMAP iSrcBitmap;

   // Step 1: Create a memory DC for the source and destination bitmaps
   //         compatible with the device used.
    hMemSrc = CreateCompatibleDC(hDC);
    hMemDest= CreateCompatibleDC(hDC);


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
    hDestBitmap = CreateBitmap(height, width, iSrcBitmap.bmPlanes,
                    iSrcBitmap.bmBitsPixel, NULL);


    hOldSourceBitmap = SelectObject(hMemSrc, hSourceBitmap);
    hOldDestBitmap = SelectObject(hMemDest, hDestBitmap);

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
    for (int y=miny;y<maxy;y++) { //0 to max height of bitmap
	  for(int x=minx;x<maxx;x++) { //0 to max width of bitmap
	    int sourcex = (int)(x*cosine+y*sine); //get pixel from sprite, x-axis
	    int sourcey = (int)(y*cosine-x*sine); //get pixel from sprite, y-axis
	    if(sourcex>=0 && sourcex<iSrcBitmap.bmWidth && sourcey>=0
	   	   && sourcey<iSrcBitmap.bmHeight ) {
           current_pixel=GetPixel(hMemSrc,sourcex,sourcey); //get current pixel color
          if (current_pixel==rTransparent) { //Set Target Transparent color (i.e. LTGREEN) to BLACK
	        SetPixel(hMemDest, x, y, BLACK);
          } else if (current_pixel==BLACK){
            if (sprite_color!=BLACK) { //Set BLACK to Custom Color
	          SetPixel(hMemDest, x, y, sprite_color);
            } else { //change BLACK to DKBLACK 
	          SetPixel(hMemDest, x, y, DKBLACK);
            }
          } else { //Set pixel, no change to color
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
      StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight/2, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMemA, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND); //Create Mask for
    } else { //Regular
      StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight/2, bitmap.bmWidth, bitmap.bmHeight, hdcMemA, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND); //Create Mask for
    }


    oldbitmap2 = SelectObject(hdcMemB,hDestBitmap);
    if (is_left) { //Flip Horizontally (X)
      StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight/2, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMemB, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCPAINT); //Create Mask for
    } else { //Regular
      StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight/2, bitmap.bmWidth, bitmap.bmHeight, hdcMemB, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCPAINT); //Create Mask for
    }
    //BitBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCAND); //This works (Demo, no flip transparent background)

    // DeleteObject(SelectObject(hdcMem, hBitmapMask));
     //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
    DeleteObject(SelectObject(hdcMemB, oldbitmap2));
    DeleteObject(SelectObject(hdcMemA, oldbitmap)); //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
    DeleteDC(hdcMemA);
    DeleteDC(hdcMemB);
  } else {
    GrPrint(hDC,_x1,_y1,"(No Sprite)",WHITE); //Print Message if sprite cannot be loaded
  }
}



void DrawTriFill(HDC hdc, int tri_color,double x1,double y1,double x2,double y2,double x3,double y3,bool IsHatch,int hatch_type)
{//https://stackoverflow.com/questions/33447305/c-windows32-gdi-fill-triangle
  HPEN hPen = CreatePen(PS_SOLID, 2, tri_color);
  HPEN hOldPen = SelectObject(hdc, hPen);

  HBRUSH hBrush;

  if (!IsHatch) {
    hBrush=CreateSolidBrush(tri_color);
  } else {
    hBrush=CreateHatchBrush(hatch_type,tri_color);
  }  
  HBRUSH hOldBrush = SelectObject(hdc, hBrush);

  POINT vertices[] = { {x1, y1}, {x2, y2}, {x3, y3} };
  Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));

  SelectObject(hdc, hOldBrush);
  DeleteObject(hBrush);

  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}
