
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
    DeleteObject(oldBitmap);
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
