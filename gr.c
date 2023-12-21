
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
      StretchBlt(hdcMem, x1+bitmap.bmWidth/2, y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdc, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
      StretchBlt(hdc, x1+bitmap.bmWidth/2, y1-bitmap.bmHeight, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
    } else { //Regular
      StretchBlt(hdcMem, x1-bitmap.bmWidth/2, y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdc, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
      StretchBlt(hdc, x1-bitmap.bmWidth/2, y1-bitmap.bmHeight, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND);
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
