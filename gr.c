

double le_angle=0;

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


void GrSprite(HWND hwnd, HDC hDC, PAINTSTRUCT ps,  double _x1, double _y1, double radians,HBITMAP hSourceBitmap,bool is_left) {
   HBITMAP hOldSourceBitmap, hOldDestBitmap, hDestBitmap;
   HDC hMemSrc,hMemDest,hdcMem;
   BITMAP iSrcBitmap;

   // Step 1: Create a memory DC for the source and destination bitmaps
   //         compatible with the device used.
   hMemSrc = CreateCompatibleDC(hDC);
   hMemDest= CreateCompatibleDC(hDC);
   hdcMem = CreateCompatibleDC(hDC);


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
   hDestBitmap = CreateCompatibleBitmap(hdcMem, width, height);


   hOldSourceBitmap = SelectObject(hMemSrc, hSourceBitmap);
   hOldDestBitmap = SelectObject(hMemDest, hDestBitmap);


	// Draw the background color before we change mapping mode
    COLORREF clrBack = RGB(255,255,255); //For transparent background
	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH) SelectObject( hMemDest, hbrBack );
	PatBlt(hMemDest, 0, 0, width, height, PATCOPY );
	DeleteObject( SelectObject(hMemDest, hbrOld ) );

	// Set mapping mode so that +ve y axis is upwords
	SetMapMode(hMemSrc, MM_ISOTROPIC);
	SetWindowExtEx(hMemSrc, 1,1,NULL);
	SetViewportExtEx(hMemSrc, 1,-1,NULL);
	SetViewportOrgEx(hMemSrc, 0, iSrcBitmap.bmHeight-1,NULL);

	SetMapMode(hMemDest, MM_ISOTROPIC);
	SetWindowExtEx(hMemDest, 1,1,NULL);
	SetViewportExtEx(hMemDest, 1,-1,NULL);
	SetWindowOrgEx(hMemDest, minx, maxy,NULL);


   // Step 4: Copy the pixels from the source to the destination.
   for (int y=miny;y<maxy;y++) {
	 for(int x=minx;x<maxx;x++) {
	   int sourcex = (int)(x*cosine+y*sine);
	   int sourcey = (int)(y*cosine-x*sine);
	   if(sourcex>=0 && sourcex<iSrcBitmap.bmWidth && sourcey>=0
	   	   && sourcey<iSrcBitmap.bmHeight )
	     SetPixel(hMemDest, x, y, GetPixel(hMemSrc,sourcex,sourcey));
	  }
	}

   // Step 5: Destroy the DCs.
   SelectObject(hMemSrc, hOldSourceBitmap);
   SelectObject(hMemDest, hOldDestBitmap);
   DeleteDC(hMemDest);
   DeleteDC(hMemSrc);


/*    src_Bitmap=Rotate(hdc, src_Bitmap, le_angle);*/
   HBITMAP oldBitmap;
   BITMAP bitmap;
   oldBitmap = SelectObject(hdcMem, hDestBitmap);
   GetObject(hDestBitmap, sizeof(BITMAP), &bitmap); //hBitmap point to bitmap var

   if (is_left) { //Flip Horizontally (X)
     StretchBlt(hdcMem, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hDC, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
     StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
   } else { //Regular
     StretchBlt(hdcMem, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hDC, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
     StretchBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
   }
   //BitBlt(hDC, _x1-bitmap.bmWidth/2, _y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCAND); //This works

   DeleteObject(SelectObject(hdcMem, oldBitmap)); //https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-createcompatiblebitmap https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-deleteobject
   DeleteDC(hdcMem);
}


void GrPrint(HWND hwnd, HDC hdc, PAINTSTRUCT ps, double x1, double y1, LPCWSTR txt) {
 // DWORD color;
 // HFONT hFont, holdFont;
 // color=GetSysColor(COLOR_BTNFACE);
//  SetBkColor(hdc,TRANSPARENT);
//  hFont=CreateFontW(15,0,0,0,FW_MEDIUM,0,0,0,0,0,0,0,0,L"Calbri");
//  holdFont=SelectObject(hdc,hFont);
  TextOutW(hdc, x1, y1, txt, lstrlenW(txt));
}

