
#define COLORS_NUM  16

//REGULAR COLORS
#define BLACK       RGB(0,0,0)
#define BLUE        RGB(0,0,170)
#define GREEN    	RGB(0,170,0)
#define CYAN        RGB(0,170,170)
#define RED         RGB(170,0,0)
#define PURPLE      RGB(170,0,170)
#define BROWN       RGB(170,85,0)
#define LTGRAY      RGB(170,170,170)
#define DKGRAY      RGB(85,85,85)
#define LTBLUE      RGB(0,0,255)
#define LTGREEN     RGB(0,255,0)
#define LTCYAN      RGB(0,255,255)
#define LTRED       RGB(255,0,0)
#define LTPURPLE    RGB(255,0,255)
#define YELLOW      RGB(255,255,0)
#define WHITE       RGB(255,255,255)



#define DKBLACK     RGB(2,2,2) //For drawing
#define LPURPLE     RGB(170,43,170)
#define LLTGREEN    RGB(0,254,0)
#define MYCOLOR1    RGB(22,22,22)//RGB(1,1,1)
#define MYCOLOR32   RGB(12,254,123)

//DARK COLORS
#define DKRBLACK       RGB(0,0,0)
#define DKRBLUE        RGB(0,0,32)
#define DKRGREEN       RGB(0,32,0)
#define DKRCYAN        RGB(0,32,32)
#define DKRRED         RGB(32,0,0)
#define DKRPURPLE      RGB(32,0,32)
#define DKRBROWN       RGB(32,16,0)
#define DKRLTGRAY      RGB(48,48,48)
#define DKRDKGRAY      RGB(32,32,32)
#define DKRLTBLUE      RGB(0,0,48)
#define DKRLTGREEN     RGB(0,48,0)
#define DKRLTCYAN      RGB(0,48,48)
#define DKRLTRED       RGB(48,0,0)
#define DKRLTPURPLE    RGB(48,0,48)
#define DKRYELLOW      RGB(48,48,0)
#define DKRWHITE       RGB(255,255,255)


//LIGHT COLORS
#define LTRBLACK       RGB(0,0,0)
#define LTRBLUE        RGB(85,85,170)
#define LTRGREEN       RGB(85,170,85)
#define LTRCYAN        RGB(85,170,170)
#define LTRRED         RGB(170,85,85)
#define LTRPURPLE      RGB(170,85,170)
#define LTRBROWN       RGB(170,170,85)
#define LTRLTGRAY      RGB(170,170,170)
#define LTRDKGRAY      RGB(85,85,85)
#define LTRLTBLUE      RGB(170,170,255)
#define LTRLTGREEN     RGB(170,255,170)
#define LTRLTCYAN      RGB(170,255,255)
#define LTRLTRED       RGB(255,170,170)
#define LTRLTPURPLE    RGB(255,170,255)
#define LTRYELLOW      RGB(255,255,170)
#define LTRWHITE       RGB(255,255,255)


//LIGHTER COLORS
#define LTR2BLACK       RGB(0,0,0)
#define LTR2BLUE        RGB(149,149,170)
#define LTR2GREEN       RGB(149,170,149)
#define LTR2CYAN        RGB(149,170,170)
#define LTR2RED         RGB(170,149,149)
#define LTR2PURPLE      RGB(170,149,170)
#define LTR2BROWN       RGB(170,160,149)

#define LTR2LTGRAY      RGB(224,224,224)
#define LTR2DKGRAY      RGB(170,170,170)

#define LTR2LTBLUE      RGB(224,224,255)
#define LTR2LTGREEN     RGB(224,255,224)
#define LTR2LTCYAN      RGB(224,255,255)
#define LTR2LTRED       RGB(255,224,224)
#define LTR2LTPURPLE    RGB(255,224,255)
#define LTR2YELLOW      RGB(255,255,224)
#define LTR2WHITE       RGB(255,255,255)




int Highlight(bool predicate,int color1, int color2)
{
  if (predicate)
    return color2;
  return color1;
}

RGBQUAD rgbColorsInvert[256];
RGBQUAD rgbColorsRedToBlue[256];

void Init8BitRGBColorsInvert(RGBQUAD *rgbColors,RGBQUAD *rgbColorsSrc)
{
  int calc;
  int index_range;
  for (int i = 0; i < 256; i++) {
    if (i%15!=0 && i%16!=0) {
      rgbColors[i].rgbRed = rgbColorsSrc[255-i].rgbRed;
      rgbColors[i].rgbGreen = rgbColorsSrc[255-i].rgbGreen;
      rgbColors[i].rgbBlue = rgbColorsSrc[255-i].rgbBlue;
    } else {
      rgbColors[i].rgbRed = rgbColorsSrc[i].rgbRed;
      rgbColors[i].rgbGreen = rgbColorsSrc[i].rgbGreen;
      rgbColors[i].rgbBlue = rgbColorsSrc[i].rgbBlue;
    }
  }
}


void CopyReplaceColorPalette(RGBQUAD *paletteDest, RGBQUAD *paletteSrc,int old_color_id,int new_color)
{
  for (int i=0;i<256;i++) {
    if (i==old_color_id) {
      paletteDest[i].rgbRed = GetRValue(new_color);
      paletteDest[i].rgbGreen = GetGValue(new_color);
      paletteDest[i].rgbBlue = GetBValue(new_color);
    } else {
      paletteDest[i].rgbRed = paletteSrc[i].rgbRed;
      paletteDest[i].rgbGreen = paletteSrc[i].rgbGreen;
      paletteDest[i].rgbBlue = paletteSrc[i].rgbBlue;
    }
  }
}



void CopyReplaceColorPaletteNoir(RGBQUAD *paletteDest, RGBQUAD *paletteSrc,int old_color_id,int new_color)
{
  int calc;
  for (int i = 0; i < 256; i++) {
    if (i==old_color_id) {
      calc = new_color/16;
      if (calc==0 || calc==15) {
        paletteDest[i].rgbRed = new_color;
        paletteDest[i].rgbGreen =new_color;
        paletteDest[i].rgbBlue =new_color;
      } else {
        paletteDest[i].rgbRed = 255-new_color;
        paletteDest[i].rgbGreen = 255-new_color;
        paletteDest[i].rgbBlue = 255-new_color;
      }
    } else {
      calc = i/16;
      if (calc==0 || calc==15) {
        paletteDest[i].rgbRed = i;
        paletteDest[i].rgbGreen = i;
        paletteDest[i].rgbBlue = i;
      } else {
        paletteDest[i].rgbRed = 255-i;
        paletteDest[i].rgbGreen = 255-i;
        paletteDest[i].rgbBlue = 255-i;
      }
    }
  }
}


RGBQUAD rgbColorsNoir[256];
void Init8BitRGBColorsNoir(RGBQUAD *rgbColors)
{
  //int calc;
  for (int i = 0; i < 256; i++) {
    //calc = i/16;
    //green->ltpurple
    /*if (calc==0 || calc==15) {
      rgbColors[i].rgbRed = i;
      rgbColors[i].rgbGreen = i;
      rgbColors[i].rgbBlue = i;
    } else {
      rgbColors[i].rgbRed = i;//255-i;
      rgbColors[i].rgbGreen = i;//255-i;
      rgbColors[i].rgbBlue = i;//255-i;
    }*/
    rgbColors[i].rgbRed = i;
    rgbColors[i].rgbGreen = i;
    rgbColors[i].rgbBlue = i;
  }
}


//https://gamedev.net/forums/topic/267754-win32-replacing-color-in-a-bitmap/267754/

RGBQUAD rgbColorsDefault[256];
void Init8BitRGBColorsDefault(RGBQUAD *rgbColors)
{
  int calc;
  for(int i=0; i<256; i++) {
    calc=i/16;
    float index_range = i-16*calc+1; //0 to 15 //0,1, 2,3, 4,5, 6,7      8,9, 10,11, 12,13, 14,15
    switch (calc) {
      case 0: //BLACK
        rgbColors[i].rgbRed = i/2;
        rgbColors[i].rgbGreen = i/2;
        rgbColors[i].rgbBlue = i/2;
        rgbColors[i].rgbReserved = 0;
        break;
      case 1: //BLUE
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = 0;
          rgbColors[i].rgbGreen = 0;
          rgbColors[i].rgbBlue = ceil(index_range*170/8);
        } else { //9->16 Lighter values
          if (index_range==16) { //1->7 Darker values, 8 is true value
            rgbColors[i].rgbRed = 160;
            rgbColors[i].rgbGreen = 160;
          } else {
            rgbColors[i].rgbRed = ceil((index_range-8)*170/8);
            rgbColors[i].rgbGreen = ceil((index_range-8)*170/8);
          }
          rgbColors[i].rgbBlue = 170;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 2: //GREEN
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = 0;
          rgbColors[i].rgbGreen = ceil(index_range*170/8);
          rgbColors[i].rgbBlue = 0;
        } else { //9->16 Lighter values
          rgbColors[i].rgbGreen = 170;
          if (index_range==16) {
            rgbColors[i].rgbRed = 160;
            rgbColors[i].rgbBlue = 160;
          } else {
            rgbColors[i].rgbRed = ceil((index_range-8)*170/8);
            rgbColors[i].rgbBlue = ceil((index_range-8)*170/8);
          }
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 3: //CYAN
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = 0;
          rgbColors[i].rgbGreen = ceil(index_range*170/8);
          rgbColors[i].rgbBlue = ceil(index_range*170/8);
        } else { //9->16 Lighter values
          if (index_range==16) {
            rgbColors[i].rgbRed = 160;
          } else {
            rgbColors[i].rgbRed = ceil((index_range-8)*170/8);
          }
          rgbColors[i].rgbGreen = 170;
          rgbColors[i].rgbBlue = 170;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 4: //RED
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*170/8);
          rgbColors[i].rgbGreen = 0;
          rgbColors[i].rgbBlue = 0;
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 170;
          if (index_range==16) {
            rgbColors[i].rgbGreen = 160;
            rgbColors[i].rgbBlue = 160;
          } else {
            rgbColors[i].rgbGreen = ceil((index_range-8)*170/8);
            rgbColors[i].rgbBlue = ceil((index_range-8)*170/8);
          }
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 5: //PURPLE
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil((index_range-1)*170/8);
          rgbColors[i].rgbGreen = 0;
          rgbColors[i].rgbBlue = ceil((index_range-1)*170/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 170;
          if (index_range==16) {
            rgbColors[i].rgbGreen = 160;
          } else {         
            rgbColors[i].rgbGreen = ceil((index_range-8)*170/8);
          }
          rgbColors[i].rgbBlue = 170;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 6: //BROWN
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*170/8);
          rgbColors[i].rgbGreen = ceil(index_range*85/8);
          rgbColors[i].rgbBlue = 0;
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 170;
          rgbColors[i].rgbGreen = 85+ceil((index_range-8)*85/8);
          rgbColors[i].rgbBlue = ceil((index_range-8)*170/8);
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 7: //DKGRAY
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*86/8);
          rgbColors[i].rgbGreen = ceil(index_range*85/8);
          rgbColors[i].rgbBlue = ceil(index_range*85/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 86+ceil((index_range-8)*86/8);
          rgbColors[i].rgbGreen = 85+ceil((index_range-8)*85/8);
          rgbColors[i].rgbBlue = 85+ceil((index_range-8)*85/8);
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 8: //LTGRAY
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*171/8);
          rgbColors[i].rgbGreen = ceil(index_range*170/8);
          rgbColors[i].rgbBlue = ceil(index_range*170/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 171+ceil((index_range-8)*10);
          rgbColors[i].rgbGreen = 170+ceil((index_range-8)*10);
          rgbColors[i].rgbBlue = 170+ceil((index_range-8)*10);
        }

        rgbColors[i].rgbReserved = 0;
        break;


//Lighter values
      case 9: //LTBLUE
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = 0;
          rgbColors[i].rgbGreen = 0;
          rgbColors[i].rgbBlue = ceil(index_range*255/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = ceil((index_range-8)*224/8);
          rgbColors[i].rgbGreen = ceil((index_range-8)*224/8);
          rgbColors[i].rgbBlue = 255;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 10: //LTGREEN 160->175 , 167
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = 0;
          rgbColors[i].rgbBlue = 0;
          //if (index_range==7)
            //rgbColors[i].rgbGreen = 254; //<-------- green at 254
          //else
            rgbColors[i].rgbGreen = ceil(index_range*255/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = ceil((index_range-8)*224/8);
          rgbColors[i].rgbBlue = ceil((index_range-8)*224/8);
          rgbColors[i].rgbGreen = 255;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 11: //LTCYAN
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = 0;
          rgbColors[i].rgbGreen = ceil(index_range*255/8);
          rgbColors[i].rgbBlue = ceil(index_range*255/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = ceil((index_range-8)*224/8);
          rgbColors[i].rgbGreen = 255;
          rgbColors[i].rgbBlue = 255;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 12: //LTRED 16*12 =192   192 -> ,,,, 192+7=199
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*255/8);
          rgbColors[i].rgbGreen = 0;
          rgbColors[i].rgbBlue = 0;
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 255;
          rgbColors[i].rgbGreen = ceil((index_range-8)*224/8);
          rgbColors[i].rgbBlue = ceil((index_range-8)*224/8);
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 13: //LTPURPLE
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*255/8);
          rgbColors[i].rgbGreen = 0;
          rgbColors[i].rgbBlue = ceil(index_range*255/8);
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 255;
          rgbColors[i].rgbGreen = ceil((index_range-8)*224/8);
          rgbColors[i].rgbBlue = 255;
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 14: //YELLOW
        if (index_range<=8) { //1->7 Darker values, 8 is true value
          rgbColors[i].rgbRed = ceil(index_range*255/8);
          rgbColors[i].rgbGreen = ceil(index_range*255/8);
          rgbColors[i].rgbBlue = 0;
          
        } else { //9->16 Lighter values
          rgbColors[i].rgbRed = 255;
          rgbColors[i].rgbGreen = 255;
          rgbColors[i].rgbBlue = ceil((index_range-8)*224/8);
        }
        rgbColors[i].rgbReserved = 0;
        break;
      case 15: //WHITE
        rgbColors[i].rgbRed =  255;//ceil((float)(i-16*calc+1)*255/16);
        rgbColors[i].rgbGreen = 255;//ceil((float)(i-16*calc+1)*255/16);
        rgbColors[i].rgbBlue = 255;//ceil((float)(i-16*calc+1)*255/16);
        rgbColors[i].rgbReserved = 0;
        break;
    }
  }
}

int rgbPaint[256];
int rgbPaint_i[256];
void Init8BitRGBPaintDefault(int *rgbPaint_dest,int *rgbPaint_i_dest,RGBQUAD *rgbColors_src,bool is_ascending,int start_paint_index)
{
  int index=0;
  //int start_paint_index=8;
  int paint_index=0;


  paint_index=start_paint_index;
  if (is_ascending) {
    start_paint_index++;
  } else {
    start_paint_index--;
  }
  for (int x=0;x<16;x++) { //left to right
    for (int y=0;y<16;y++) { //up to down 
      rgbPaint_dest[index]=RGB(rgbColors_src[paint_index].rgbRed,rgbColors_src[paint_index].rgbGreen,rgbColors_src[paint_index].rgbBlue);
      rgbPaint_i_dest[index]=paint_index;
      index++;
      paint_index+=16;
    }
    paint_index=start_paint_index;
    if (is_ascending) {//shift darker/lighter
      start_paint_index++; 
    } else {
      start_paint_index--; 
    }
    if(start_paint_index>15) {
      start_paint_index=0;
    }
    if (start_paint_index<0) {
      start_paint_index=15;
    }
  }
}


void BitmapPalette(HDC hdc, HDC hdc2,HBITMAP hBitmap,RGBQUAD *bitmapPalette) {
  SelectObject(hdc2, hBitmap);
  SetDIBColorTable(hdc2, 0, 256, bitmapPalette);
}





//https://learn.microsoft.com/en-us/windows/win32/gdi/drawing-a-shaded-triangle
//https://stackoverflow.com/questions/33447305/c-windows32-gdi-fill-triangl//https://stackoverflow.com/questions/3142349/drawing-on-8bpp-grayscale-bitmap-unmanaged-c



HBITMAP CreateGreyscaleBitmap(int cx, int cy)
{
  BITMAPINFO* pbmi = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[256]));
  pbmi->bmiHeader.biSize = sizeof (pbmi->bmiHeader);
  pbmi->bmiHeader.biWidth = cx;
  pbmi->bmiHeader.biHeight = cy;
  pbmi->bmiHeader.biPlanes = 1;
  pbmi->bmiHeader.biBitCount = 8;
  pbmi->bmiHeader.biCompression = BI_RGB;
  pbmi->bmiHeader.biSizeImage = 0;
  pbmi->bmiHeader.biXPelsPerMeter = 14173;
  pbmi->bmiHeader.biYPelsPerMeter = 14173;
  pbmi->bmiHeader.biClrUsed = 0;
  pbmi->bmiHeader.biClrImportant = 0;

  for(int i=0; i<256; i++)
  {
    pbmi->bmiColors[i].rgbRed = i;
    pbmi->bmiColors[i].rgbGreen = i;
    pbmi->bmiColors[i].rgbBlue = i;
    pbmi->bmiColors[i].rgbReserved = 0;
  }

  PVOID pv;
  return CreateDIBSection(NULL,pbmi,DIB_RGB_COLORS,&pv,NULL,0);
}





HBITMAP Create1BitBitmap(int cx, int cy)
{
  BITMAPINFO* pbmi = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[2]));
  pbmi->bmiHeader.biSize = sizeof (pbmi->bmiHeader);
  pbmi->bmiHeader.biWidth = cx;
  pbmi->bmiHeader.biHeight = cy;
  pbmi->bmiHeader.biPlanes = 1;
  pbmi->bmiHeader.biBitCount = 1;
  pbmi->bmiHeader.biCompression = BI_RGB;
  pbmi->bmiHeader.biSizeImage = 0;

  pbmi->bmiColors[0].rgbRed = 0;
  pbmi->bmiColors[0].rgbGreen = 0;
  pbmi->bmiColors[0].rgbBlue = 0;
  pbmi->bmiColors[0].rgbReserved = 0;

  pbmi->bmiColors[1].rgbRed = 255;
  pbmi->bmiColors[1].rgbGreen = 255;
  pbmi->bmiColors[1].rgbBlue = 255;
  pbmi->bmiColors[1].rgbReserved = 0;

  pbmi->bmiHeader.biClrUsed = 0;
  pbmi->bmiHeader.biClrImportant = 0;
  PVOID pv;
  return CreateDIBSection(NULL,pbmi,DIB_RGB_COLORS,&pv,NULL,0);
}



HBITMAP CreateCrunchyBitmap(int cx, int cy)
{
  BITMAPINFO* pbmi = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[256]));
  pbmi->bmiHeader.biSize = sizeof (pbmi->bmiHeader);
  pbmi->bmiHeader.biWidth = cx;
  pbmi->bmiHeader.biHeight = cy;
  pbmi->bmiHeader.biPlanes = 1;
  pbmi->bmiHeader.biBitCount = 8;
  pbmi->bmiHeader.biCompression = BI_RGB;
  pbmi->bmiHeader.biSizeImage = 0;

  pbmi->bmiHeader.biXPelsPerMeter = 14173;
  pbmi->bmiHeader.biYPelsPerMeter = 14173;
  Init8BitRGBColorsDefault(pbmi->bmiColors);

  pbmi->bmiHeader.biClrUsed = 0;
  pbmi->bmiHeader.biClrImportant = 0;
  PVOID pv;
  return CreateDIBSection(NULL,pbmi,DIB_RGB_COLORS,&pv,NULL,0);
}



HBITMAP CreateLargeBitmap(int cx, int cy)
{
//https://forums.codeguru.com/showthread.php?526563-Accessing-Pixels-with-CreateDIBSection
 unsigned char* lpBitmapBits; 

  BITMAPINFO bi; 
  ZeroMemory(&bi, sizeof(BITMAPINFO));
  bi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
  bi.bmiHeader.biWidth=cx;
  bi.bmiHeader.biHeight=-cy;
  bi.bmiHeader.biPlanes=1;
  bi.bmiHeader.biBitCount=16;
  return CreateDIBSection(NULL, &bi,DIB_RGB_COLORS, (VOID**)&lpBitmapBits,NULL,0);
}




HBITMAP CreateLargeBitmapWithBuffer(int width, int height, BYTE** ppPixels) {
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 16;
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hDC = GetDC(NULL);
    HBITMAP hBitmap = CreateDIBSection(hDC, &bmi, DIB_RGB_COLORS, (void**)ppPixels, NULL, 0);
    ReleaseDC(NULL, hDC);
    return hBitmap;
}



HBITMAP CopyBitmapWithBuffer(HBITMAP srcBitmap, BYTE **ppPixels, int SRCOPERATION)
{
  BITMAP bm;
  GetObject(srcBitmap, sizeof(bm), &bm);
  HBITMAP destBitmap=CreateLargeBitmapWithBuffer(bm.bmWidth,bm.bmHeight,ppPixels);

  HDC hdcMem = CreateCompatibleDC(NULL);
  HDC hdcMem2 = CreateCompatibleDC(NULL);

  SelectObject(hdcMem2, srcBitmap);
  SelectObject(hdcMem, destBitmap);

  BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCOPERATION);

  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return destBitmap;
}


void CopyPartialGreyscaleBitmap(HBITMAP dBitmap, HBITMAP sBitmap,int x,int SRCOPERATION)
{

  BITMAP bm;
  GetObject(sBitmap, sizeof(bm), &bm);

  HDC hdc1=CreateCompatibleDC(NULL);
  HDC hdc2=CreateCompatibleDC(NULL);
  SelectObject(hdc2,sBitmap);
  SelectObject(hdc1,dBitmap);
  BitBlt(hdc1, 0, 0, x, bm.bmHeight, hdc2, 0, 0, SRCOPERATION);
  DeleteDC(hdc2);
  DeleteDC(hdc1);
}


HBITMAP CopyGreyscaleBitmap(HBITMAP sBitmap, int SRCOPERATION)
{
  BITMAP bm;
  GetObject(sBitmap, sizeof(bm), &bm);

  BITMAPINFO* pbmi = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[256]));
  pbmi->bmiHeader.biSize = sizeof (pbmi->bmiHeader);
  pbmi->bmiHeader.biWidth = bm.bmWidth;
  pbmi->bmiHeader.biHeight = bm.bmHeight;
  pbmi->bmiHeader.biPlanes = 1;
  pbmi->bmiHeader.biBitCount = 8;
  pbmi->bmiHeader.biCompression = BI_RGB;
  pbmi->bmiHeader.biSizeImage = 0;
  pbmi->bmiHeader.biXPelsPerMeter = 14173;
  pbmi->bmiHeader.biYPelsPerMeter = 14173;
  pbmi->bmiHeader.biClrUsed = 0;
  pbmi->bmiHeader.biClrImportant = 0;

  for(int i=0; i<256; i++)
  {
    pbmi->bmiColors[i].rgbRed = i;
    pbmi->bmiColors[i].rgbGreen = i;
    pbmi->bmiColors[i].rgbBlue = i;
    pbmi->bmiColors[i].rgbReserved = 0;
  }

  PVOID pv;
  HDC hdc=CreateCompatibleDC(NULL);
  HDC hdc2=CreateCompatibleDC(NULL);
  HBITMAP dBitmap=CreateDIBSection(NULL,pbmi,DIB_RGB_COLORS,&pv,NULL,0);
  SelectObject(hdc2,sBitmap);
  SelectObject(hdc,dBitmap);
  BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hdc2, 0, 0, SRCOPERATION);
  DeleteDC(hdc2);
  DeleteDC(hdc);
  return dBitmap;
}


HBITMAP CopyBitmap(HBITMAP srcBitmap,int SRCOPERATION)
{
  BITMAP bm;
  GetObject(srcBitmap, sizeof(bm), &bm);
  HBITMAP destBitmap=CreateLargeBitmap(bm.bmWidth,bm.bmHeight);

  HDC hdcMem = CreateCompatibleDC(NULL);
  HDC hdcMem2 = CreateCompatibleDC(NULL);

  SelectObject(hdcMem2, srcBitmap);
  SelectObject(hdcMem, destBitmap);

  BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCOPERATION);

  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return destBitmap;
}



HBITMAP CopyCrunchyBitmap(HBITMAP srcBitmap,int SRCOPERATION)
{
  BITMAP bm;
  GetObject(srcBitmap, sizeof(bm), &bm);
  HBITMAP destBitmap=CreateCrunchyBitmap(bm.bmWidth,bm.bmHeight);

  HDC hdcMem = CreateCompatibleDC(NULL);
  HDC hdcMem2 = CreateCompatibleDC(NULL);

  SelectObject(hdcMem2, srcBitmap);
  SelectObject(hdcMem, destBitmap);

  BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCOPERATION);

  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return destBitmap;
}




HBITMAP CopyStretchBitmap(HBITMAP srcBitmap,int SRCOPERATION, int nWidth, int nHeight)
{
  BITMAP bm;
  GetObject(srcBitmap, sizeof(bm), &bm);

  HBITMAP destBitmap=CreateLargeBitmap(nWidth,nHeight);

  HDC hdcMem = CreateCompatibleDC(NULL);
  HDC hdcMem2 = CreateCompatibleDC(NULL);

  SelectObject(hdcMem2, srcBitmap);
  SelectObject(hdcMem, destBitmap);

  StretchBlt(hdcMem, 0, 0, nWidth, nHeight, hdcMem2, 0,0, bm.bmWidth, bm.bmHeight, SRCOPERATION); //draw to 

  DeleteDC(hdcMem);
  DeleteDC(hdcMem2);

  return destBitmap;
}



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
void GrRect(HDC hdc, float x,float y,int l, int h,int COLOR) {
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


//https://stackoverflow.com/questions/10966008/draw-slightly-transparent-blue-rectangle-in-native-win32-gdi
//https://www.codeproject.com/Articles/286/Using-the-AlphaBlend-function
void GrGlassRect(HDC hdc, HDC hdcMem, int x, int y, int width, int height, int COLOR, BYTE alpha) {
    BLENDFUNCTION blendFunction;
    blendFunction.BlendOp = AC_SRC_OVER;
    blendFunction.BlendFlags = 0;
    blendFunction.SourceConstantAlpha = alpha; // Transparency level (0-255)
    blendFunction.AlphaFormat = 0;

    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    SelectObject(hdcMem, hBitmap);

    // Fill the rectangle with the desired color
    HBRUSH hBrush = CreateSolidBrush(COLOR); // Blue color
    RECT rect = {0, 0, width, height};
    FillRect(hdcMem, &rect, hBrush);

    // Use AlphaBlend to draw the transparent rectangle
    AlphaBlend(hdc, x, y, width, height, hdcMem, 0, 0, width, height, blendFunction);

    // Clean up
    DeleteObject(hBrush);
    DeleteObject(hBitmap);
}


// Function to decompress BI_RLE8-compressed data while retaining color mapping
int DecompressRLE8(const BYTE *compressedData, int width, int height, BYTE *decompressedData) {
    int x = 0, y = 0;
    const BYTE *src = compressedData;
    BYTE *dest = decompressedData;

    while (y < height) {
        BYTE command = *src++;
        if (command == 0) { // Escape code
            BYTE escape = *src++;
            if (escape == 0) { // End of line
                x = 0;
                y++;
            } else if (escape == 1) { // End of bitmap
                break;
            } else if (escape == 2) { // Delta
                BYTE dx = *src++;
                BYTE dy = *src++;
                x += dx;
                y += dy;
            } else { // Absolute mode
                for (int i = 0; i < escape; i++) {
                    if (x >= width || y >= height) {
                        return -1; // Error: Out of bounds
                    }
                    dest[y * width + x] = *src++;
                    x++;
                }
                if (escape & 1) {
                    src++; // Skip padding byte
                }
            }
        } else { // Encoded mode
            BYTE colorIndex = *src++;
            for (int i = 0; i < command; i++) {
                if (x >= width || y >= height) {
                    return -1; // Error: Out of bounds
                }
                dest[y * width + x] = colorIndex;
                x++;
            }
        }
    }
    return 0; // Success
}

// Function to load a BI_RLE8-compressed bitmap as a DIBSection with retained colors
HBITMAP LoadRLE8CompressedBitmap(const wchar_t *filePath) {
    FILE *file = _wfopen(filePath, L"rb");
    if (file == NULL) {
        wprintf(L"Failed to open file: %ls\n", filePath);
        return NULL;
    }

    // Read the BITMAPFILEHEADER
    BITMAPFILEHEADER fileHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);

    if (fileHeader.bfType != 0x4D42) { // 'BM'
        wprintf(L"Invalid bitmap file: %ls\n", filePath);
        fclose(file);
        return NULL;
    }

    // Read the BITMAPINFOHEADER
    BITMAPINFOHEADER infoHeader;
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);

    if (infoHeader.biCompression != BI_RLE8) {
        wprintf(L"Bitmap is not BI_RLE8 compressed: %ls\n", filePath);
        fclose(file);
        return NULL;
    }

    // Allocate memory for the color table (palette)
    int colorTableSize = infoHeader.biClrUsed ? infoHeader.biClrUsed : 256;
    RGBQUAD *colorTable = malloc(colorTableSize * sizeof(RGBQUAD));
    fread(colorTable, sizeof(RGBQUAD), colorTableSize, file);

    // Calculate dimensions and allocate memory for decompressed data
    int width = infoHeader.biWidth;
    int height = abs(infoHeader.biHeight);
    int imageSize = width * height;
    BYTE *compressedData = malloc(infoHeader.biSizeImage);
    BYTE *decompressedData = malloc(imageSize);

    if (!compressedData || !decompressedData || !colorTable) {
        wprintf(L"Failed to allocate memory.\n");
        fclose(file);
        free(compressedData);
        free(decompressedData);
        free(colorTable);
        return NULL;
    }

    // Read the compressed pixel data
    fseek(file, fileHeader.bfOffBits, SEEK_SET);
    fread(compressedData, 1, infoHeader.biSizeImage, file);
    fclose(file);


    // Decompress the RLE8 data
    if (DecompressRLE8(compressedData, width, height, decompressedData) != 0) {
        wprintf(L"Failed to decompress RLE8 data.\n");
        free(compressedData);
        free(decompressedData);
        free(colorTable);
        return NULL;
    }


    // Set up the BITMAPINFO structure
    BITMAPINFO* bmInfo = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[256]));
    bmInfo->bmiHeader.biSize = sizeof (bmInfo->bmiHeader);
    bmInfo->bmiHeader.biWidth = width;
    bmInfo->bmiHeader.biHeight = height; // Top-down DIB
    bmInfo->bmiHeader.biPlanes = 1;
    bmInfo->bmiHeader.biBitCount = 8;
    bmInfo->bmiHeader.biCompression = BI_RGB;
    bmInfo->bmiHeader.biSizeImage = infoHeader.biSizeImage;
    bmInfo->bmiHeader.biXPelsPerMeter = infoHeader.biXPelsPerMeter;
    bmInfo->bmiHeader.biYPelsPerMeter = infoHeader.biYPelsPerMeter;

    Init8BitRGBColorsDefault(bmInfo->bmiColors);
    
    bmInfo->bmiHeader.biClrUsed = infoHeader.biClrUsed;
    bmInfo->bmiHeader.biClrImportant = infoHeader.biClrImportant;
    bmInfo->bmiHeader.biSizeImage = imageSize;

    // Create a compatible device context (DC)
    HDC hdc = GetDC(NULL);

    // Create the DIBSection
    void *dibPixels = NULL;
    HBITMAP hBitmap = //CreateDIBSection(hdc,bmInfo,DIB_RGB_COLORS,&dibPixels,NULL,0);
            CreateDIBitmap(
                hdc,
                &bmInfo->bmiHeader,       // Pointer to BITMAPINFOHEADER
                CBM_INIT,                 // Initialization flag
                decompressedData,         // Pointer to the decompressed pixel data
                bmInfo,                   // Pointer to BITMAPINFO structure with color information
                DIB_RGB_COLORS            // Color usage
            );
    // Release the DC
    ReleaseDC(NULL, hdc);

    if (hBitmap == NULL) {
        wprintf(L"Failed to create DIBSection.\n");
        free(bmInfo);
        //free(bitmapInfo);
        free(compressedData);
        free(decompressedData);
        free(colorTable);
        return NULL;
    }

    // Copy the decompressed data into the DIBSection
    free(bmInfo);
    free(compressedData);
    free(decompressedData);
    free(colorTable);

    return hBitmap;
}



void GrLine(HDC hdc, float x1,float y1,float x2,float y2,int COLOR) {
  HPEN hPen = CreatePen(PS_SOLID, 1, COLOR);
  HPEN hOldPen = SelectObject(hdc, hPen);
  MoveToEx(hdc,x1,y1,NULL);
  LineTo(hdc,x2,y2);
  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}

void GrLineThick(HDC hdc, float x1,float y1,float x2,float y2,int size,int COLOR) {
  HPEN hPen = CreatePen(PS_SOLID, size, COLOR);
  HPEN hOldPen = SelectObject(hdc, hPen);
  MoveToEx(hdc,x1,y1,NULL);
  LineTo(hdc,x2,y2);
  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}


void GrDottedLine(HDC hdc, float x1,float y1,float x2,float y2,int COLOR) {
  DWORD dashPattern[2] = { 10, 5 };
  //10 pixels solid, 5 pixels gap 
  LOGBRUSH lb = { BS_SOLID, COLOR, 0 }; // Black color // Create a custom pen with the dash pattern 
  HPEN hPen = ExtCreatePen(PS_GEOMETRIC | PS_USERSTYLE, 1, &lb, 2, dashPattern); 
  HPEN hOldPen = (HPEN)SelectObject(hdc, hPen); // Draw the line 
  MoveToEx(hdc, x1, y1, NULL); 
  LineTo(hdc, x2, y2); // Restore the original pen 
  SelectObject(hdc, hOldPen); 
  DeleteObject(hPen);
}

void GrCircle(HDC hdc, float x, float y, int size, int COLOR, int COLOR_2) {
//Shape Coordinates
  float x1=x-size;
  float y1=y-size;
  float x2=x+size;
  float y2=y+size;

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

  //Draw circle
  SelectObject(hdc, GetStockObject(DC_PEN));
  if (COLOR_2==-1) {
    SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); //Draw circle with no fill
    SetDCPenColor(hdc, COLOR); //BORDER
  } else {
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, COLOR); //SOLID FILL
    SetDCPenColor(hdc, COLOR_2); //BORDER
  }

  Ellipse(hdc, left, top, right, bottom);
}


/*void GrDashCircle(HDC hdc, float x, float y, int size, int COLOR, int COLOR_2) {
    // Shape Coordinates
    float x1 = x - size;
    float y1 = y - size;
    float x2 = x + size;
    float y2 = y + size;

    int left = (int)min(x1, x2);
    int top = (int)min(y1, y2);
    int right = (int)max(x1, x2);
    int bottom = (int)max(y1, y2);

    int length = min(abs(right - left), abs(bottom - top));
    if (x2 < x1)
        left = right - length;
    else
        right = left + length;

    if (y2 < y1)
        top = bottom - length;
    else
        bottom = top + length;

    // Create dashed pen
    HPEN hPen = CreatePen(PS_DASH, 1, COLOR_2 == -1 ? COLOR : COLOR_2);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    // Set brush
    HBRUSH hOldBrush;
    if (COLOR_2 == -1) {
        hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(HOLLOW_BRUSH)); // No fill
    } else {
        HBRUSH hBrush = CreateSolidBrush(COLOR);
        hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    }

    // Draw the circle
    Ellipse(hdc, left, top, right, bottom);

    // Restore old objects
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);

    // Clean up
    DeleteObject(hPen);
    if (COLOR_2 != -1) {
        DeleteObject((HBRUSH)hOldBrush); // Only delete if custom brush was used
    }
}*/




void GrPrintW(HDC hdc, int x1, int y1, wchar_t *_txt, char *_atxt, int color, int _height, bool A,bool is_unifont) 
{
  LPCWSTR txt=_txt;
  LPCSTR atxt=_atxt;
  HFONT hf;

  if (is_unifont) {
    hf=CreateFontW(_height, //Height
                0, //cWidth
                0, //cescapement
                0, //corientation
                FW_MEDIUM, //cweight
                FALSE, //bitalic
                FALSE, //bunderline
                0, //bstrikeout
                0, //icharset
                0, //ioutprecision
                0,//iclip precision
                0, //iqyaluty
                0, //ipitchandfamily
                L"Unifont");  //pszfacename
  } else {
    hf=CreateFontW(_height, //Height
                0, //cWidth
                0, //cescapement
                0, //corientation
                FW_MEDIUM, //cweight
                FALSE, //bitalic
                FALSE, //bunderline
                0, //bstrikeout
                0, //icharset
                0, //ioutprecision
                0,//iclip precision
                0, //iqyaluty
                0, //ipitchandfamily
                L"Arial");  //pszfacename
  }

  HFONT hfOld = SelectObject(hdc,hf);
  SelectObject(hdc,hf);

  SetTextColor(hdc, color); //set color of the text to be drawn
  SetBkMode(hdc, TRANSPARENT); //makes background of txt transparent  //https://stackoverflow.com/questions/10571966/  
  RECT rect;
  //GetClientRect(hwnd, &rect);
  rect.left=x1;
  rect.top=y1;
  
  //TextOutA(hdc, x1, y1, txt_reverse, txt_len); //draw text to screen
  if (!A) {
    //TextOut(hdc, x1, y1, txt, wcslen(txt)); //draw text to screen
    DrawText(hdc, txt, -1, &rect, DT_NOCLIP);
  } else {
    //TextOutA(hdc, x1, y1, atxt, strlen(atxt)); //draw text to screen
    DrawTextA(hdc, atxt, -1, &rect, DT_NOCLIP);
  }
  SelectObject(hdc,hfOld);
  DeleteObject(hfOld);
  DeleteObject(hf);
  SetTextColor(hdc, TRANSPARENT);
}

void GrPrintWThick(HDC hdc, int x, int y, wchar_t *_txt, char *_atxt, int color1, int color2,int _height, bool A,bool is_unifont) 
{
  GrPrintW(hdc,x   ,y  ,_txt,"",color2,_height,A,is_unifont);
  GrPrintW(hdc,x+2 ,y  ,_txt,"",color2,_height,A,is_unifont);
  GrPrintW(hdc,x   ,y+2,_txt,"",color2,_height,A,is_unifont);
  GrPrintW(hdc,x+2 ,y+2,_txt,"",color2,_height,A,is_unifont);
  GrPrintW(hdc,x+1 ,y+1,_txt,"",color1,_height,A,is_unifont);
}



void GrPrint(HDC hdc, int x1, int y1, char *_txt, int color) 
{//https://forums.codeguru.com/showthread.php?329037-Drawtext-with-japanese-character
  //DWORD color;
  //HFONT hFont, holdFont;
  //color=GetSysColor(COLOR_BTNFACE);
  //SetBkColor(hdc,color);
  //holdFont=SelectObject(hdc,hFont);

  LPCSTR txt = _txt; //convert text to lpcstr
  SetTextColor(hdc, color); //set color of the text to be drawn
  SetBkMode(hdc, TRANSPARENT); //makes background of txt transparent  //https://stackoverflow.com/questions/10571966/draw-print-text-with-transparent-background-in-c-win32
  TextOutA(hdc, x1, y1, txt, strlen(txt)); //draw text to screen
  SetTextColor(hdc, TRANSPARENT);
}


void GrPrintThick(HDC hdc,int x, int y, char *_txt,int color1,int color2)
{
  GrPrint(hdc,x,  y,  _txt,color2);
  GrPrint(hdc,x+2,y,  _txt,color2);
  GrPrint(hdc,x,  y+2,_txt,color2);
  GrPrint(hdc,x+2,y+2,_txt,color2);
  GrPrint(hdc,x+1,y+1,_txt,color1); //main topmost
}


void GrPrintA(HDC hdc, float x1, float y1, wchar_t *_txt, int color) 
{//https://forums.codeguru.com/showthread.php?329037-Drawtext-with-japanese-character
//https://stackoverflow.com/questions/1974015/how-to-use-drawtext-to-write-text-in-a-given-window-whose-handle-is-known

  LPCWSTR txt = _txt; //convert text to lpcstr
  SetTextColor(hdc, color); //set color of the text to be drawn
  SetBkMode(hdc, TRANSPARENT); //makes background of txt transparent
  RECT rect;
  //GetClientRect(hwnd, &rect);
  rect.left=x1;
  rect.top=y1;
  DrawText(hdc, txt, -1, &rect, DT_NOCLIP);

  SetTextColor(hdc, TRANSPARENT);
}

/*HPALETTE rgbPaletteColorsNoir;
void CreateNoirPalette()
{
  LOGPALETTE *pLogPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE)+255*sizeof(PALETTEENTRY));
  pLogPal->palVersion = 0x300;
  pLogPal->palNumEntries=256;
  for (int i=0;i<256;i++) {
    pLogPal->palPalEntry[i].peRed=i;
    pLogPal->palPalEntry[i].peGreen=i;
    pLogPal->palPalEntry[i].peBlue=i;
    pLogPal->palPalEntry[i].peFlags=0;
  }
  rgbPaletteColorsNoir=CreatePalette(pLogPal);
  free(pLogPal);
}*/



void DrawBitmap(HDC hDC, HDC hdcMem,float _x1,float _y1, float _x2, float _y2, int width, int height, HBITMAP hSourceBitmap,int _SRCTYPE,bool stretch,bool is_left)

{
  if (hSourceBitmap!=NULL) {
    BITMAP bitmap;
    GetObject(hSourceBitmap,sizeof(bitmap),&bitmap);
    SelectObject(hdcMem,hSourceBitmap);
    int b_width=width;
    if (is_left) {
      b_width=-bitmap.bmWidth-1;
      _x1+=width;
    }

    //SelectPalette(hdcMem,rgbPaletteColorsNoir,FALSE);
    //RealizePalette(hdcMem);
    if (stretch || is_left)
      StretchBlt(hDC, _x1, _y1, b_width, height, hdcMem, 0,0, bitmap.bmWidth, bitmap.bmHeight, _SRCTYPE); //draw to 
     //StretchBlt(hDC, _x1+bitmap.bmWidth/2, _y1-bitmap.bmHeight/2, -bitmap.bmWidth-1, bitmap.bmHeight, hdcMemA, 0,0, bitmap.bmWidth, bitmap.bmHeight, SRCAND); //Create Mask for
    else
      BitBlt(hDC, _x1, _y1, width, height, hdcMem, _x2, _y2, _SRCTYPE);
    //DeleteDC(hdcMem);
  }
}



////https://ftp.zx.net.nz/pub/Patches/ftp.microsoft.com/MISC/KB/en-us/77/127.HTM
////https://www.codeguru.com/multimedia/rotate-a-bitmap-image/
////https://github.com/StephanBusch/EcoZip/blob/35c215fbc3efc16951182f6127885d22d11a018e/EcoZipFM/ImageUtils.cpp

HBITMAP RotateSprite(HDC hDC, HBITMAP hSourceBitmap, float radians,int rTransparent, int old_color, int sprite_color, int sprite_color_2) 
{ //if (hSourceBitmap != NULL) { ////https://ftp.zx.net.nz/pub/Patches/ftp.microsoft.com/MISC/KB/en-us/77/127.HTM
 //https://github.com/StephanBusch/EcoZip/blob/35c215fbc3efc16951182f6127885d22d11a018e/EcoZipFM/ImageUtils.cpp
  HBITMAP hOldSourceBitmap, hOldDestBitmap, hDestBitmap; ////https://www.codeguru.com/multimedia/rotate-a-bitmap-image/
  HDC hMemSrc,hMemDest;
  BITMAP iSrcBitmap;

  // Step 1: Create a memory DC for the source and destination bitmaps
  //         compatible with the device used.
  hMemSrc = CreateCompatibleDC(hDC);
  hMemDest= CreateCompatibleDC(hDC);


  // Step 2: Get the height and width of the source bitmap.
  GetObject(hSourceBitmap, sizeof(BITMAP), (LPSTR)&iSrcBitmap);


  //printf("bits:%d",iSrcBitmap.bmBitsPixel)
  // Get logical coordinates
  float cosine = (float)cos(radians);
  float sine = (float)sin(radians);

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

  hDestBitmap = CreateCrunchyBitmap(width,height);//CreateLargeBitmap(height, width);
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
      /*if (iSrcBitmap.bmHeight!=iSrcBitmap.bmWidth) {
        sourcex=x;
        sourcey=y;
      }*/
	  if(sourcex>=0 && sourcex<iSrcBitmap.bmWidth && sourcey>=0
	   	 && sourcey<iSrcBitmap.bmHeight ) {
         current_pixel=GetPixel(hMemSrc,sourcex,sourcey); //get current pixel color
        if (current_pixel==rTransparent) { //Set Target Transparent color (i.e. LTGREEN) to BLACK
	      SetPixel(hMemDest, x, y, BLACK);
        } else if (current_pixel==old_color){
          if (sprite_color_2==-1) { //custom flag to disallow dithreing
            if (sprite_color!=BLACK) { //Set BLACK to Custom Color
	          SetPixel(hMemDest, x, y, sprite_color);
            } else { //change BLACK to DKBLACK 
	          SetPixel(hMemDest, x, y, DKBLACK);
            }
          } else { //dither == old_color does not matter (for now) (im lazy)
            if (sprite_color!=BLACK) { //Set BLACK to Custom Color
              if (y%2==0) {
                if (x%2==0) {
	              SetPixel(hMemDest, x, y, sprite_color_2);
                } else {
	              SetPixel(hMemDest, x, y, sprite_color);
                }
              } else {
                if (x%2!=0) {
	              SetPixel(hMemDest, x, y, sprite_color_2);
                } else {
	              SetPixel(hMemDest, x, y, sprite_color);
                }
              }
            } else { //change BLACK to DKBLACK 
	          SetPixel(hMemDest, x, y, DKBLACK);
            }
          }
        } else {
          SetPixel(hMemDest, x, y, current_pixel);
        }
      }
    }
  }

 // Step 5: Destroy the DCs.
  //DeleteObject(SelectObject(hMemSrc, hOldSourceBitmap));
  //DeleteObject(SelectObject(hMemDest, hOldDestBitmap));
  SelectObject(hMemSrc, hOldSourceBitmap);
  SelectObject(hMemDest, hOldDestBitmap);
  DeleteObject(hOldSourceBitmap);
  DeleteObject(hOldDestBitmap);
  DeleteDC(hMemDest);
  DeleteDC(hMemSrc);
  return (hDestBitmap);
}

HBITMAP GetRotated8BitBitmap(HBITMAP hBitmap,float radians,COLORREF clrBack)
{
//***https://forums.codeguru.com/showthread.php?111988-rotating-a-HBITMAP***
//###https://www.codeguru.com/multimedia/rotate-a-bitmap-at-any-angle-without-getpixel-setpixel/###
//https://web.archive.org/web/20150519150259/http://www.codeguru.com/cpp/g-m/bitmap/specialeffects/article.php/c1743/Rotate-a-bitmap-image.htm

  //Convert to 8Bit, play it safe
  HBITMAP tmp_h8BitBitmap = CopyCrunchyBitmap(hBitmap,SRCCOPY);

  //Retrieve Bitmap Info
  BITMAP bmp;
  if (!GetObject(hBitmap,sizeof(BITMAP),&bmp)) //assign 8bit bitmap to bmp
    return NULL;


  int width = bmp.bmWidth;
  int height = bmp.bmHeight;
  int bpp = 1;//8/8;
  int stride = ((width*bpp+3) & ~3); //row size aligned to 4 bytes


  //Create DIB Section for Source Bitmap
  BITMAPINFO* bmInfo = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[256]));
  bmInfo->bmiHeader.biSize = sizeof (bmInfo->bmiHeader);
  bmInfo->bmiHeader.biWidth = width;
  bmInfo->bmiHeader.biHeight = height; // Top-down DIB
  bmInfo->bmiHeader.biPlanes = 1;
  bmInfo->bmiHeader.biBitCount = 8;
  bmInfo->bmiHeader.biCompression = BI_RGB;
  bmInfo->bmiHeader.biSizeImage = 0;
  bmInfo->bmiHeader.biXPelsPerMeter = 14173;
  bmInfo->bmiHeader.biYPelsPerMeter = 14173;

  Init8BitRGBColorsDefault(bmInfo->bmiColors);
  bmInfo->bmiHeader.biClrUsed = 0;
  bmInfo->bmiHeader.biClrImportant = 0;

  // Find palette index for clrBack
  BYTE backgroundIndex = 0xFF; // Invalid index initially
  for (int i = 0; i < 256; i++) {
     if (RGB(bmInfo->bmiColors[i].rgbRed, bmInfo->bmiColors[i].rgbGreen, bmInfo->bmiColors[i].rgbBlue) == clrBack) {
         backgroundIndex = (BYTE)i;
         break;
     }
  }

  HDC hdc = GetDC(NULL);
  void *sourcePixels=NULL;
  HBITMAP hSourceDIB = CreateDIBSection(hdc,bmInfo,DIB_RGB_COLORS,&sourcePixels,NULL,0);
  if (!hSourceDIB) {
    free(bmInfo);
    DeleteObject(hSourceDIB);
    DeleteObject(tmp_h8BitBitmap);
    ReleaseDC(NULL,hdc);
    return NULL;
  }

  //Copy pixels from bitmap to hSourceDIB
  HDC hMemDC = CreateCompatibleDC(hdc);
  SelectObject(hMemDC,hSourceDIB);
  HDC hSrcDC = CreateCompatibleDC(hdc);
  SelectObject(hSrcDC,tmp_h8BitBitmap);
  BitBlt(hMemDC,0,0,width,height,hSrcDC,0,0,SRCCOPY);
  DeleteDC(hSrcDC);



  //Get Dimensions of rotated bitmap
  float cosine = cos(radians);
  float sine = sin(radians);
  int newWidth = (int)(abs(width*cosine) + abs(height*sine));
  int newHeight = (int)(abs(width*sine) + abs(height*cosine));

  //Create DIB section for rotated bitmap 8bit
  bmInfo->bmiHeader.biWidth = newWidth;
  bmInfo->bmiHeader.biHeight = newHeight; //top-down db


  void *rotatedPixels=NULL;

  HBITMAP hRotatedDIB = CreateDIBSection(hdc,bmInfo,DIB_RGB_COLORS,&rotatedPixels,NULL,0);

  if (!hRotatedDIB) {
    free(bmInfo);
    DeleteObject(hSourceDIB);
    DeleteObject(tmp_h8BitBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(NULL,hdc);
    return(NULL);
  }

  //Initialize rotated bitmap with background color
  memset(rotatedPixels, backgroundIndex,newHeight*((newWidth *bpp+3) & ~3));


  //Perform Rotation
  for (int y=0;y<newHeight;y++) {
    for (int x=0;x<newWidth;x++) {
      //Reverse-transform destination pixel to source pixel
      int srcX=(int)((x-newWidth/2)*cosine+(y-newHeight/2)*sine+width/2);
      int srcY=(int)((y-newHeight/2)*cosine-(x-newWidth/2)*sine+height/2);
      if (srcX >=0 && srcX<width && srcY>=0 && srcY<height) {
        //copy pixel from src to dest
         int srcIndex = srcY * stride + srcX; // Source pixel index
         int destIndex = y * ((newWidth * bpp + 3) & ~3) + x; // Destination pixel index

         // Copy pixel from source to destination
         ((BYTE*)rotatedPixels)[destIndex] = ((BYTE*)sourcePixels)[srcIndex];
      }
    }
  }

  //Cleanup
  free(bmInfo);
  DeleteObject(hSourceDIB);
  DeleteObject(tmp_h8BitBitmap);
  DeleteDC(hMemDC);
  ReleaseDC(NULL,hdc);
  return hRotatedDIB;
}


void DitherBitmapColor(HBITMAP hBitmap, COLORREF oldColor, COLORREF newColor)
{
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // Ensure it's an 8-bit bitmap
    if (bitmap.bmBitsPixel != 8) {
        return; // Not an 8-bit bitmap
    }

    // Access the pixel data
    BYTE *pixels = (BYTE *)bitmap.bmBits;

    // Retrieve the palette
    HDC hdc = CreateCompatibleDC(NULL);
    SelectObject(hdc, hBitmap);
    RGBQUAD palette[256];
    GetDIBColorTable(hdc, 0, 256, palette); // Get the palette
    DeleteDC(hdc);

    // Find indices for oldColor and newColor
    BYTE oldColorIndex = 0xFF, newColorIndex = 0xFF; // Invalid initially
    for (int i = 0; i < 256; i++) {
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == oldColor) {
            oldColorIndex = (BYTE)i;
        }
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == newColor) {
            newColorIndex = (BYTE)i;
        }
    }

    for (int cf=0;cf<16;cf++) {
      if (oldColor==cf) {
        oldColorIndex=cf;
        break;
      }
    }
    for (int cf=0;cf<16;cf++) {
      if (newColor==cf) {
        newColorIndex=cf;
        break;
      }
    }

    // Check if both colors are in the palette
    if ((oldColorIndex == 0xFF || newColorIndex == 0xFF) && oldColor!=-1) {
        return; // Color not found
    }

    // Traverse and replace pixel data
    for (int y = 0; y < bitmap.bmHeight; y++) {
        BYTE *row = pixels + (y * bitmap.bmWidthBytes); // Calculate row pointer

        for (int x = 0; x < bitmap.bmWidth; x++) {
            /*if (y%2==0 && x%2==0 ||
               (y%2!=0 && x%2!=0)) {
              if (row[x] == oldColorIndex || oldColor==-1) {
                row[x] = newColorIndex; // Replace the color index
              }
            }*/
            if (y%2==0 && x%2==0 ||
               (y%2!=0 && x%2!=0)) {
              if (row[x] == oldColorIndex || oldColor==-1) {
                row[x] = newColorIndex; // Replace the color index
              }
            }
        }
    }

}



void ReplaceBitmapColor(HBITMAP hBitmap, COLORREF oldColor, COLORREF newColor)//, RGBQUAD *palette) 
{
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // Ensure it's an 8-bit bitmap
    if (bitmap.bmBitsPixel != 8) {
        return; // Not an 8-bit bitmap
    }

    // Access the pixel data
    BYTE *pixels = (BYTE *)bitmap.bmBits;

    // Retrieve the palette
    HDC hdc = CreateCompatibleDC(NULL);
    SelectObject(hdc, hBitmap);
    RGBQUAD palette[256];
    GetDIBColorTable(hdc, 0, 256, palette); // Get the palette
    DeleteDC(hdc);

    // Find indices for oldColor and newColor
    BYTE oldColorIndex = 0xFF, newColorIndex = 0xFF; // Invalid initially
    for (int i = 0; i < 256; i++) {
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == oldColor) {
            oldColorIndex = (BYTE)i;
        }
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == newColor) {
            newColorIndex = (BYTE)i;
        }
    }

    for (int cf=0;cf<16;cf++) {
      if (oldColor==cf) {
        oldColorIndex=cf;
        break;
      }
    }
    for (int cf=0;cf<16;cf++) {
      if (newColor==cf) {
        newColorIndex=cf;
        break;
      }
    }

    // Check if both colors are in the palette
    if (oldColorIndex == 0xFF || newColorIndex == 0xFF) {
        return; // Color not found
    }

    // Traverse and replace pixel data
    for (int y = 0; y < bitmap.bmHeight; y++) {
        BYTE *row = pixels + (y * bitmap.bmWidthBytes); // Calculate row pointer

        for (int x = 0; x < bitmap.bmWidth; x++) {
            //printf("x:%d,y:%d: cindex:%d\n",x,y,row[x]);
            if (row[x] == oldColorIndex) {
                row[x] = newColorIndex; // Replace the color index
            }
        }
    }
}




void ReplaceBitmapColor2(HBITMAP hBitmap, 
    COLORREF oldColor1, COLORREF newColor1, //R
    COLORREF oldColor2, COLORREF newColor2)
{
    BITMAP bitmap;
    GetObject(hBitmap, sizeof(BITMAP), &bitmap);

    // Ensure it's an 8-bit bitmap
    if (bitmap.bmBitsPixel != 8) {
        return; // Not an 8-bit bitmap
    }

    // Access the pixel data
    BYTE *pixels = (BYTE *)bitmap.bmBits;

    // Retrieve the palette
    HDC hdc = CreateCompatibleDC(NULL);
    SelectObject(hdc, hBitmap);
    RGBQUAD palette[256];
    GetDIBColorTable(hdc, 0, 256, palette); // Get the palette
    DeleteDC(hdc);

    // Find indices for oldColor and newColor
    BYTE oldColorIndex1 = 0xFF, newColorIndex1 = 0xFF; // Invalid initially
    BYTE oldColorIndex2 = 0xFF, newColorIndex2 = 0xFF; // Invalid initially
    for (int i = 0; i < 256; i++) {
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == oldColor1) {
            oldColorIndex1 = (BYTE)i;
        }
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == newColor1) {
            newColorIndex1 = (BYTE)i;
        }
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == oldColor2) {
            oldColorIndex2 = (BYTE)i;
        }
        if (RGB(palette[i].rgbRed, palette[i].rgbGreen, palette[i].rgbBlue) == newColor2) {
            newColorIndex2 = (BYTE)i;
        }
    }

    for (int cf=0;cf<16;cf++) {
      if (oldColor1==cf) {
        oldColorIndex1=cf;
        break;
      }
    }
    for (int cf=0;cf<16;cf++) {
      if (newColor1==cf) {
        newColorIndex1=cf;
        break;
      }
    }
    for (int cf=0;cf<16;cf++) {
      if (oldColor2==cf) {
        oldColorIndex2=cf;
        break;
      }
    }
    for (int cf=0;cf<16;cf++) {
      if (newColor2==cf) {
        newColorIndex2=cf;
        break;
      }
    }

    // Check if both colors are in the palette
    if (oldColorIndex1 == 0xFF || newColorIndex1 == 0xFF || oldColorIndex2 == 0xFF || newColorIndex2 == 0xFF) {
        return; // Color not found
    }

    // Traverse and replace pixel data
    for (int y = 0; y < bitmap.bmHeight; y++) {
        BYTE *row = pixels + (y * bitmap.bmWidthBytes); // Calculate row pointer

        for (int x = 0; x < bitmap.bmWidth; x++) {
            //printf("cindex:%d",row[x]);
            if (row[x] == oldColorIndex1)
                row[x] = newColorIndex1; // Replace the color index
            else if (row[x] == oldColorIndex2)
                row[x] = newColorIndex2; // Replace the color index

        }
    }
}



void DrawTriFill(HDC hdc, int tri_color, float x1,float y1,float x2,float y2,float x3,float y3,bool IsHatch,int hatch_type)
{//https://stackoverflow.com/questions/33447305/c-windows32-gdi-fill-triangle
  HPEN hPen = CreatePen(PS_SOLID, 2, tri_color);
  HPEN hOldPen = SelectObject(hdc, hPen);

  HBRUSH hBrush, hOldBrush;
  POINT vertices[] = { {x1, y1}, {x2, y2}, {x3, y3} };

  if (!IsHatch) {
    hBrush=CreateSolidBrush(tri_color);
    hOldBrush = SelectObject(hdc, hBrush);
  } else {
 // Set background mode to transparent 
    SetBkMode(hdc, TRANSPARENT);

    hBrush=CreateHatchBrush(hatch_type,tri_color);
    hOldBrush = SelectObject(hdc, hBrush);
  }  

  Polygon(hdc, vertices, sizeof(vertices) / sizeof(vertices[0]));

  SelectObject(hdc, hOldBrush);
  DeleteObject(hBrush);

  SelectObject(hdc, hOldPen);
  DeleteObject(hPen);
}

void DrawPaletteSquare(HDC hdc,int move_x,int move_y)
{
  int size=8;
  int index=0;
  for (int y=0;y<16;y++) { //up to down 
    for (int x=0;x<16;x++) { //left to right
      GrRect(hdc,move_x+x*size,move_y+y*size,size,size,RGB(rgbColorsDefault[index].rgbRed,rgbColorsDefault[index].rgbGreen,rgbColorsDefault[index].rgbBlue));
      index++;
    }
  }
}


void DrawPaintSquare(HDC hdc,int move_x,int move_y,int original_index, int target_index)
{
  const int size=8;
  const int size2=12;
  int index=0;
  for (int y=0;y<16;y++) { //up to down 
    for (int x=0;x<16;x++) { //left to right
      if (index==target_index) {
        GrRect(hdc,move_x+x*size-2,move_y+y*size-2,size2,size2,WHITE);        
        GrRect(hdc,move_x+x*size,move_y+y*size,size,size,rgbPaint[index]);
      } else {
        GrCircle(hdc,move_x+x*size+4,move_y+y*size+4,4,rgbPaint[index],rgbPaint[index]);
      }
      //GrRect(hdc,move_x+x*size,move_y+y*size,size,size,rgbPaint[index]);
      if (index==original_index) {
        GrCircle(hdc,move_x+x*size+4,move_y+y*size+4,2,WHITE,WHITE);
       }
      index++;
    }
  }
}




typedef struct DRAWSPRITE
{
  HBITMAP sprite_mask;
  HBITMAP sprite_paint;
} DRAWSPRITE;



void GenerateDrawSprite(DRAWSPRITE* myDrawSprite,HBITMAP srcBitmap)
{
    myDrawSprite->sprite_paint=CopyCrunchyBitmap(srcBitmap,SRCCOPY);
    myDrawSprite->sprite_mask=CreateBitmapMask(myDrawSprite->sprite_paint,BLACK,NULL);
}

void GenerateLargeDrawSprite(DRAWSPRITE* myDrawSprite,HBITMAP srcBitmap)
{
    myDrawSprite->sprite_paint=CopyBitmap(srcBitmap,SRCCOPY);
    myDrawSprite->sprite_mask=CreateBitmapMask(myDrawSprite->sprite_paint,BLACK,NULL);
}


void FreeDrawSprite(DRAWSPRITE* myDrawSprite)
{
  if (myDrawSprite->sprite_mask!=NULL)
    DeleteObject(myDrawSprite->sprite_mask);
  if (myDrawSprite->sprite_paint!=NULL)
    DeleteObject(myDrawSprite->sprite_paint);
}



void DrawSprite(HDC hdc,HDC hdc2,int _x1, int _y1, DRAWSPRITE* myDrawSprite,bool is_left)
{
  BITMAP bm;
  GetObject(myDrawSprite->sprite_mask, sizeof(bm), &bm);

  DrawBitmap(hdc,hdc2,
                 _x1-bm.bmWidth/2,
                 _y1-bm.bmHeight/2,
                 0,
                 0,
                 bm.bmWidth,
                 bm.bmHeight,
                 myDrawSprite->sprite_mask,SRCAND,FALSE,is_left);
  DrawBitmap(hdc,hdc2,
                 _x1-bm.bmWidth/2,
                 _y1-bm.bmHeight/2,
                 0,
                 0,
                 bm.bmWidth,
                 bm.bmHeight,
                 myDrawSprite->sprite_paint,SRCPAINT,FALSE,is_left);

}



//https://github.com/jroop6/SimpleKeylogger/blob/2a8fd44237a2ba85bcdb9b6bd05bec26ac2cd4f5/SimpleKeylogger/MainWindow.cpp
//https://github.com/MarySoroka/OSASP-2/blob/70c997b0f58a3a2886654fc97feaf82572038c95/Paint/Paint/paint.cpp


bool SaveLargeBitmapToFile(HBITMAP hBitmap, LPCWSTR lpszFileName) {
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;
    HANDLE hf;                  // file handle
    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD dwTotal;              // total count of bytes
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;

    // Retrieve the bitmap color format, width, and height.
    if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp)) {
        return FALSE;
    }

    // Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1) {
        cClrBits = 1;
    } else if (cClrBits <= 4) {
        cClrBits = 4;
    } else if (cClrBits <= 8) {
        cClrBits = 8;
    } else if (cClrBits <= 16) {
        cClrBits = 16;
    } else if (cClrBits <= 24) {
        cClrBits = 24;
    } else {
        cClrBits = 32;
    }

    // Allocate memory for the BITMAPINFO structure.
    if (cClrBits != 24) {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));
    } else {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));
    }

    // Initialize the fields in the BITMAPINFO structure.
    pbih = (PBITMAPINFOHEADER) pbmi;
    pbih->biSize = sizeof(BITMAPINFOHEADER);
    pbih->biWidth = bmp.bmWidth;
    pbih->biHeight = bmp.bmHeight;
    pbih->biPlanes = bmp.bmPlanes;
    pbih->biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24) {
        pbih->biClrUsed = (1 << cClrBits);
    }

    // If the bitmap is not compressed, set the BI_RGB flag.
    pbih->biCompression = BI_RGB;

    // Compute the number of bytes in the array of color indices and store the result in biSizeImage.
    pbih->biSizeImage = ((pbih->biWidth * cClrBits + 31) & ~31) / 8 * pbih->biHeight;
    pbih->biClrImportant = 0;

    // Allocate memory for the array of color indices.
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    // Retrieve the color table (RGBQUAD array) and the bits (array of palette indices).
    if (!GetDIBits(GetDC(0), hBitmap, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) {
        return FALSE;
    }

    // Create the .BMP file.
    hf = CreateFile(lpszFileName, GENERIC_READ | GENERIC_WRITE, (DWORD) 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    hdr.bfType = 0x4D42;  // "BM"
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD);

    // Write the BITMAPFILEHEADER to the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp, NULL)) {
        return FALSE;
    }

    // Write the BITMAPINFOHEADER and RGBQUAD array to the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, NULL)) {
        return FALSE;
    }

    // Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp, NULL)) {
        return FALSE;
    }

    // Close the .BMP file.
    if (!CloseHandle(hf)) {
        return FALSE;
    }

    // Free memory.
    GlobalFree((HGLOBAL)lpBits);
    LocalFree((HLOCAL)pbmi);

    return TRUE;
}


//DONT DELETE
void SaveBitmapToFile(HBITMAP hBitmap, RGBQUAD* palette, const wchar_t* filename) {
    BITMAP bmp;
    BITMAPINFOHEADER bi;
    BITMAPFILEHEADER bf;
    FILE* file;
    HDC hdc;

    // Get the bitmap information
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    // Initialize the bitmap info header
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 8;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = bmp.bmWidth * bmp.bmHeight;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 256;
    bi.biClrImportant = 256;

    // Initialize the bitmap file header
    bf.bfType = 0x4D42; // 'BM'
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
    bf.bfSize = bf.bfOffBits + bi.biSizeImage;
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;

    // Open the file for writing
    file = _wfopen(filename, L"wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write the file headers
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);

    // Write the palette to the file
    fwrite(palette, sizeof(RGBQUAD), 256, file);

    // Write the bitmap bits to the file
    fwrite(bmp.bmBits, bi.biSizeImage, 1, file);

    // Close the file
    fclose(file);
    //printf("drawn\n");
}






void SaveBitmapToFile2(HBITMAP hBitmap, RGBQUAD* palette, const wchar_t* filename) {
    BITMAP bmp;
    BITMAPINFOHEADER bi;
    BITMAPFILEHEADER bf;
    FILE* file;
    BYTE* rleData;
    DWORD rleSize = 0;

    // Get the bitmap information
    GetObject(hBitmap, sizeof(BITMAP), &bmp);

    // Initialize the bitmap info header
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = bmp.bmWidth;
    bi.biHeight = bmp.bmHeight;
    bi.biPlanes = 1;
    bi.biBitCount = 8;
    bi.biCompression = BI_RLE8; // Use RLE compression
    bi.biSizeImage = 0; // Set to 0 for RLE compression
    bi.biXPelsPerMeter = 14173;
    bi.biYPelsPerMeter = 14173;
    bi.biClrUsed = 256;
    bi.biClrImportant = 256;

    // Initialize the bitmap file header
    bf.bfType = 0x4D42; // 'BM'
    bf.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256;
    bf.bfSize = bf.bfOffBits; // We'll add the RLE size later
    bf.bfReserved1 = 0;
    bf.bfReserved2 = 0;

    // Open the file for writing
    file = _wfopen(filename, L"wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    // Write the file headers
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);

    // Write the palette to the file
    fwrite(palette, sizeof(RGBQUAD), 256, file);

    // Allocate memory for RLE data
    rleData = (BYTE*)malloc(bmp.bmWidth * bmp.bmHeight * 2); // Allocate enough memory for worst-case scenario

    // Encode the bitmap bits to RLE format
    BYTE* src = (BYTE*)bmp.bmBits;
    BYTE* dst = rleData;
    for (int y = 0; y < bmp.bmHeight; y++) {
        int x = 0;
        while (x < bmp.bmWidth) {
            BYTE count = 1;
            BYTE value = src[y * bmp.bmWidth + x];
            while (x + count < bmp.bmWidth && src[y * bmp.bmWidth + x + count] == value && count < 255) {
                count++;
            }
            *dst++ = count;
            *dst++ = value;
            x += count;
        }
        *dst++ = 0; // End of line
        *dst++ = 0;
    }
    rleSize = dst - rleData;

    // Update the size image field
    bi.biSizeImage = rleSize;

    // Write the RLE data to the file
    fwrite(rleData, rleSize, 1, file);

    // Update the file size in the file header
    bf.bfSize += rleSize;
    fseek(file, 0, SEEK_SET);
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, file);
    fseek(file, sizeof(BITMAPFILEHEADER), SEEK_SET);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);

    // Close the file
    fclose(file);

    // Free the allocated memory
    free(rleData);
    //printf("drawn\n");
}





wchar_t *khnumtxt[10]={L"០",L"១",L"២",L"៣",L"៤",L"៥",L"៦",L"៧",L"៨",L"៩"};
wchar_t* ReplaceToKhmerNum(wchar_t* mytxt) 
{
  int len=wcslen(mytxt);
  for (int i=0;i<len;i++) {
    int c = mytxt[i];
    if ('0'<=c && c<='9') {
      mytxt[i]=khnumtxt[c-'0'][0];
    }
  }
  return mytxt;
}













int global_frames=0;
int iNumFrames=0;
HBITMAP global_avi_bitmap1;
HBITMAP global_avi_bitmap2;


PAVIFILE avi;
AVIFILEINFO avi_info;
PAVISTREAM pStream;
int res;
BITMAPINFOHEADER bih;
int iNumFrames;
int iFirstFrame;
PGETFRAME pFrame;


void DrawMovingAVI(HDC hdc,HDC hdc2) 
{
    if (global_frames%2==0) {
      DrawBitmap(hdc,hdc2,0,0,0,0,GR_WIDTH,GR_HEIGHT,global_avi_bitmap2,SRCCOPY,TRUE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,0,0,0,0,GR_WIDTH,GR_HEIGHT,global_avi_bitmap1,SRCCOPY,TRUE,FALSE);
    }
}

HBITMAP CreateFromPackedDIBPointer2(LPBYTE pDIB)
{
    /*if (pDIB == NULL)
    {
        return NULL;
    }*/

    // The BITMAPINFOHEADER is at the start of the DIB
    BITMAPINFOHEADER* _bih = (BITMAPINFOHEADER*)pDIB;

    // The pixel data starts immediately after the BITMAPINFOHEADER
    BYTE* _pPixels = pDIB + sizeof(BITMAPINFOHEADER);

    // Create a device context
    HDC hdc = GetDC(NULL);

    // Create a bitmap from the DIB
    HBITMAP hBitmap = CreateDIBitmap(hdc, _bih, CBM_INIT, _pPixels, (BITMAPINFO*)_bih, DIB_RGB_COLORS);

    // Release the device context
    ReleaseDC(NULL, hdc);
    return hBitmap;
}


bool InitExtractAVIFrames(const wchar_t* szFileName,int index)
{
    AVIFileInit();
    res=AVIFileOpen(&avi, szFileName, OF_READ, NULL);
    AVIFileInfo(avi, &avi_info, sizeof(AVIFILEINFO));
    res=AVIFileGetStream(avi, &pStream, streamtypeVIDEO /*video stream*/, 
                                               0 /*first stream*/);

    //do some task with the stream

    //iFirstFrame=AVIStreamStart(pStream);
    iNumFrames=AVIStreamLength(pStream);

    //getting bitmap from frame
    ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));


    bih.biBitCount=24;    //24 bit per pixel

    bih.biClrImportant=0;
    bih.biClrUsed = 0;
    bih.biCompression = BI_RGB;
    bih.biPlanes = 1;
    bih.biSize = 40;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    //calculate total size of RGBQUAD scanlines (DWORD aligned)
    bih.biSizeImage = (((bih.biWidth * 3) + 3) & 0xFFFC) * bih.biHeight ;


    pFrame=AVIStreamGetFrameOpen(pStream, NULL);
    //https://www.codeproject.com/Questions/238467/AVIStreamGetFrameOpen-returns-NULL
    //https://www.codeproject.com/Articles/8739/Extracting-AVI-Frames
    //https://www.vbforums.com/showthread.php?293534-Memory-leak-problem-resolved
    //https://www.codeproject.com/Articles/8739/Extracting-AVI-Frames
    //https://stackoverflow.com/questions/39059959/vfw-avistreamgetframeopen-returns-null


    return TRUE;
}


/*DWORD WINAPI AnimateAVI(LPVOID lpArg) {
  while (TRUE) {
    //https://www.vbforums.com/showthread.php?604246-AVIStreamGetFrameOpen-AVIStreamGetFrameClose-memory-leak
    //https://forum.doom9.org/showthread.php?t=100297
    //https://forums.qhimm.com/index.php?topic=2402.25
    //https://www.codeproject.com/Articles/8739/Extracting-AVI-Frames
    //https://stackoverflow.com/questions/39059959/vfw-avistreamgetframeopen-returns-null
    //ffmpeg -i gameplay_day_crop.mp4 -vcodec cinepak -vf scale=320:240 -r 15 gameplay_day_crop_crt5.avi
    //ffmpeg -i gameplay_day_crop.mp4 -vcodec cinepak -r 15 gameplay_day_crop_crt5_1.avi


    //15 frames = 1000ms
    //1frame = 1000/15 = 66.666666667


    //30 frames = 1000ms
    //1frame = 1000/30 = 33.33333333333
      if (prelude) {
      } else {
        Sleep(1000); //standby
      }
  }
}*/


void DrawTexturedTriangle(HDC hdc, HDC hdc2, int x1, int y1, int x2, int y2, int x3, int y3, HBITMAP hBitmap) {
    float cx = (x1 + x2 + x3) / 3.0f;
    float cy = (y1 + y2 + y3) / 3.0f;

    // Apply scaling (1.5 pixels outward)
    float offset = 1.5f; // Offset amount
    float factor = offset / sqrt(pow(x1 - cx, 2) + pow(y1 - cy, 2)); // Normalize

    int sx1 = x1 + (x1 - cx) * factor;
    int sy1 = y1 + (y1 - cy) * factor;
    int sx2 = x2 + (x2 - cx) * factor;
    int sy2 = y2 + (y2 - cy) * factor;
    int sx3 = x3 + (x3 - cx) * factor;
    int sy3 = y3 + (y3 - cy) * factor;


    // Select the BMP into a memory DC
    HBITMAP oldBitmap = (HBITMAP)SelectObject(hdc2, hBitmap);

    // Define the triangle vertices
    POINT triangle[3] = {{sx1, sy1}, {sx2, sy2}, {sx3, sy3}};

    // Create a brush from the texture
    HBRUSH brush = CreatePatternBrush(hBitmap);

    // Select the brush and set the pen to NULL (removes borders)
    HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);
    HPEN oldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));

    // Draw the triangle
    Polygon(hdc, triangle, 3);

    // Cleanup
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    SelectObject(hdc2, oldBitmap);
    DeleteObject(brush);
}




RGBQUAD waterPalette[256];
void SetTexturePalette(int target_color_id,RGBQUAD *myTexturePalette) {
  float grey_val_r;
  float grey_val_g;
  float grey_val_b;
  target_color_id=rgbPaint_i[target_color_id];
  for (int i=0;i<256;i++) {
    grey_val_r=((float)(rgbColorsNoir[i/*255-i*/].rgbRed))/255*rgbColorsDefault[target_color_id].rgbRed;
    grey_val_g=((float)(rgbColorsNoir[i/*255-i*/].rgbGreen))/255*rgbColorsDefault[target_color_id].rgbGreen;
    grey_val_b=((float)(rgbColorsNoir[i/*255-i*/].rgbBlue))/255*rgbColorsDefault[target_color_id].rgbBlue;
    myTexturePalette[i].rgbRed=grey_val_r;
    myTexturePalette[i].rgbGreen=grey_val_g;
    myTexturePalette[i].rgbBlue=grey_val_b;
  }
}


HBITMAP FlipLargeBitmapVertically(HDC hdc, HBITMAP hBitmap)
{
//ACHTUNG! only works with 32 bit bitmap
    if (!hBitmap) return NULL;

    BITMAP bmp;
    if (!GetObject(hBitmap, sizeof(BITMAP), &bmp)) return NULL;

    const int rowSize = ((bmp.bmBitsPixel * bmp.bmWidth + 31) / 32) * 4;
    const int dataSize = rowSize * bmp.bmHeight;

    BYTE* pPixels = (BYTE*)malloc(dataSize);
    BYTE* pFlipped = (BYTE*)malloc(dataSize);
    if (!pPixels || !pFlipped) {
        free(pPixels);
        free(pFlipped);
        return NULL;
    }

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bmp.bmWidth;
    bmi.bmiHeader.biHeight = bmp.bmHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = bmp.bmBitsPixel;
    bmi.bmiHeader.biCompression = BI_RGB;

    BOOL success = GetDIBits(hdc, hBitmap, 0, bmp.bmHeight, pPixels, &bmi, DIB_RGB_COLORS);
    if (!success) {
        free(pPixels);
        free(pFlipped);
        ReleaseDC(NULL, hdc);
        return NULL;
    }

    for (int y = 0; y < bmp.bmHeight; ++y) {
        memcpy(
            pFlipped + y * rowSize,
            pPixels + (bmp.bmHeight - 1 - y) * rowSize,
            rowSize
        );
    }

    HBITMAP hFlipped;
    //if (ppPixels==NULL) {
      hFlipped=CreateBitmap(bmp.bmWidth, bmp.bmHeight, 1, bmp.bmBitsPixel, NULL);
    //} else {
      //hFlipped=CreateLargeBitmapWithBuffer(bmp.bmWidth,bmp.bmHeight,ppPixels);
    //}
    if (hFlipped) {
        SetDIBits(hdc, hFlipped, 0, bmp.bmHeight, pFlipped, &bmi, DIB_RGB_COLORS);
    }

    free(pPixels);
    free(pFlipped);
    return hFlipped;
}


