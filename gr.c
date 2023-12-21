
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
HBITMAP CreateBitmapMask(HDC hdc, HBITMAP hbmColour, COLORREF crTransparent)
{
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

/*
//https://www.codeguru.com/multimedia/rotate-a-bitmap-image/
HBITMAP GetRotatedBitmap(HBITMAP hBitmap, float radians, COLORREF clrBack )
{
	// Create a memory DC compatible with the display
	HDC sourceDC, destDC;
	sourceDC->CreateCompatibleDC( NULL );
	destDC->CreateCompatibleDC( NULL );

	// Get logical coordinates
	BITMAP bm;
	::GetObject( hBitmap, sizeof( bm ), &bm );

	float cosine = (float)cos(radians);
	float sine = (float)sin(radians);

	// Compute dimensions of the resulting bitmap
	// First get the coordinates of the 3 corners other than origin
	int x1 = (int)(-bm.bmHeight * sine);
	int y1 = (int)(bm.bmHeight * cosine);
	int x2 = (int)(bm.bmWidth * cosine - bm.bmHeight * sine);
	int y2 = (int)(bm.bmHeight * cosine + bm.bmWidth * sine);
	int x3 = (int)(bm.bmWidth * cosine);
	int y3 = (int)(bm.bmWidth * sine);

	int minx = min(0,min(x1, min(x2,x3)));
	int miny = min(0,min(y1, min(y2,y3)));
	int maxx = max(x1, max(x2,x3));
	int maxy = max(y1, max(y2,y3));

	int w = maxx - minx;
	int h = maxy - miny;


	// Create a bitmap to hold the result
	HBITMAP hbmResult = ::CreateCompatibleBitmap(CClientDC(NULL), w, h);

	HBITMAP hbmOldSource = (HBITMAP)::SelectObject( sourceDC.m_hDC, hBitmap );
	HBITMAP hbmOldDest = (HBITMAP)::SelectObject( destDC.m_hDC, hbmResult );

	// Draw the background color before we change mapping mode
	HBRUSH hbrBack = CreateSolidBrush( clrBack );
	HBRUSH hbrOld = (HBRUSH)::SelectObject( destDC.m_hDC, hbrBack );
	destDC.PatBlt( 0, 0, w, h, PATCOPY );
	::DeleteObject( ::SelectObject( destDC.m_hDC, hbrOld ) );

	// Set mapping mode so that +ve y axis is upwords
	sourceDC->SetMapMode(MM_ISOTROPIC);
	sourceDC->SetWindowExt(1,1);
	sourceDC->SetViewportExt(1,-1);
	sourceDC->SetViewportOrg(0, bm.bmHeight-1);

	destDC->SetMapMode(MM_ISOTROPIC);
	destDC->SetWindowExt(1,1);
	destDC->SetViewportExt(1,-1);
	destDC->SetWindowOrg(minx, maxy);

	// Now do the actual rotating - a pixel at a time
	// Computing the destination point for each source point
	// will leave a few pixels that do not get covered
	// So we use a reverse transform - e.i. compute the source point
	// for each destination point

	for( int y = miny; y < maxy; y++ )
	{
		for( int x = minx; x < maxx; x++ )
		{
			int sourcex = (int)(x*cosine + y*sine);
			int sourcey = (int)(y*cosine - x*sine);
			if( sourcex >= 0 && sourcex < bm.bmWidth && sourcey >= 0
					&& sourcey < bm.bmHeight )
				destDC.SetPixel(x,y,sourceDC.GetPixel(sourcex,sourcey));
		}
	}

	// Restore DCs
	::SelectObject( sourceDC.m_hDC, hbmOldSource );
	::SelectObject( destDC.m_hDC, hbmOldDest );

	return hbmResult;
}*/


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


HBITMAP FlipBitmapHorz(HDC hWC, HBITMAP hBitmap )
{
//https://forums.codeguru.com/showthread.php?118153-Flipping-graphics-using-StretchBlt
  BITMAP bm;
  GetObject(hBitmap, sizeof(bm), &bm);
  HWND hWnd = GetDesktopWindow();
  HDC hSC = CreateCompatibleDC(hWC);
  HDC hDC = CreateCompatibleDC(hWC);
  HBITMAP hRetBitmap = CreateCompatibleBitmap(hWC, bm.bmWidth, bm.bmHeight);
  HGDIOBJ hSCBMOld = SelectObject(hSC, hBitmap);
  HGDIOBJ hDCBMOld = SelectObject(hDC, hRetBitmap);
  SetMapMode(hDC, MM_ANISOTROPIC);
  ScaleViewportExtEx(hDC, -1, 1, 1, 1, NULL);
  SetViewportOrgEx(hDC, bm.bmWidth - 1, 0, NULL);
  BitBlt(hDC, 0, 0, bm.bmWidth, bm.bmHeight, hSC, 0, 0, SRCCOPY);
  SelectObject(hDC, hDCBMOld);
  SelectObject(hSC, hSCBMOld);
  ReleaseDC(hWnd, hWC);
  DeleteDC(hDC);
  DeleteDC(hSC);
  return hRetBitmap;
}


void GrSprite(HWND hwnd, HDC hdc, PAINTSTRUCT ps,  double x1, double y1, HBITMAP src_Bitmap, bool is_left) {
  BITMAP bitmap;
  HBITMAP oldBitmap;
  HDC hdcMem;

  if (src_Bitmap!=NULL) {
    hdcMem=CreateCompatibleDC(hdc);
    oldBitmap = SelectObject(hdcMem, src_Bitmap);
    GetObject(src_Bitmap, sizeof(bitmap), &bitmap); //hBitmap point to bitmap var

    //Draws Bitmap on Axis -- OLDBITMAP
    //BitBlt(hdc, x1-bitmap.bmWidth/2, y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCAND);
    if (is_left) { //Flip Horizontally (X)
      StretchBlt(hdc, x1+bitmap.bmWidth/2, y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
    } else { //Regular
      StretchBlt(hdc, x1-bitmap.bmWidth/2, y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
    }

    SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
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
}
