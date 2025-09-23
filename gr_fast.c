

void FastFlipLargeBitmapVertically(BYTE* pDst, const BYTE* pSrc, int pSrcWidth, int height)
{
    const int rowSize = ((32 * pSrcWidth + 31) / 32) * 2;
    for (int y = 0; y < height; ++y) { //only height effects performance
        memcpy(
            pDst + y * rowSize,
            pSrc + (height - 1 - y) * rowSize,
            rowSize
        );
    }
}





typedef struct {
    uint8_t r, g, b, a;
} Color;


static inline void blendPixel32Bit(BYTE* pixel, Color color) {
    // Precompute alpha and inverse alpha
    unsigned int alpha = color.a;
    unsigned int invAlpha = 255 - alpha;

    // Blend each channel using integer math and bit shifting
    pixel[0] = (BYTE)((pixel[0] * invAlpha + color.b * alpha) >> 8); // Blue
    pixel[1] = (BYTE)((pixel[1] * invAlpha + color.g * alpha) >> 8); // Green
    pixel[2] = (BYTE)((pixel[2] * invAlpha + color.r * alpha) >> 8); // Red
    pixel[3] = 255; // Fully opaque
}


/*static inline void blendPixel(BYTE* pixel, Color color) {
    // Extract RGB components from 16-bit pixel
    uint16_t p = *pixel;
    uint8_t r5 = (p >> 11) & 0x1F;
    uint8_t g6 = (p >> 5) & 0x3F;
    uint8_t b5 = p & 0x1F;

    // Convert to 8-bit
    uint8_t r = (r5 * 255) / 31;
    uint8_t g = (g6 * 255) / 63;
    uint8_t b = (b5 * 255) / 31;

    // Blend with input color
    unsigned int alpha = color.a;
    unsigned int invAlpha = 255 - alpha;

    uint8_t r_blend = (r * invAlpha + color.r * alpha) >> 8;
    uint8_t g_blend = (g * invAlpha + color.g * alpha) >> 8;
    uint8_t b_blend = (b * invAlpha + color.b * alpha) >> 8;

    // Convert back to 5/6-bit
    r5 = (r_blend * 31) / 255;
    g6 = (g_blend * 63) / 255;
    b5 = (b_blend * 31) / 255;

    // Repack into 16-bit RGB565
    *pixel = (r5 << 11) | (g6 << 5) | b5;
}*/

static inline void blendPixel(BYTE* pixel, Color color) {
    BYTE p = *pixel;

    // Extract RGB components from RGB565
    uint8_t r5 = (p >> 11) & 0x1F;
    uint8_t g6 = (p >> 5) & 0x3F;
    uint8_t b5 = p & 0x1F;

    // Convert input color to 5/6-bit
    uint8_t r5_src = (color.r * 31 + 127) / 255;
    uint8_t g6_src = (color.g * 63 + 127) / 255;
    uint8_t b5_src = (color.b * 31 + 127) / 255;

    // Blend directly in 5/6-bit space using fixed-point math
    uint8_t alpha = color.a;
    uint8_t invAlpha = 255 - alpha;

    r5 = (r5 * invAlpha + r5_src * alpha + 127) / 255;
    g6 = (g6 * invAlpha + g6_src * alpha + 127) / 255;
    b5 = (b5 * invAlpha + b5_src * alpha + 127) / 255;

    // Repack into RGB565
    *pixel = (r5 << 11) | (g6 << 5) | b5;
}


/*
static inline void blendPixel(BYTE* pixel, Color color) {
    BYTE p = *pixel;

    // Extract 4-bit channels
    unsigned int a = (p >> 12) & 0xF;
    unsigned int r = (p >> 8) & 0xF;
    unsigned int g = (p >> 4) & 0xF;
    unsigned int b = p & 0xF;

    // Convert to 8-bit
    r = (r << 4) | r;
    g = (g << 4) | g;
    b = (b << 4) | b;

    // Blend with incoming color
    unsigned int alpha = color.a;
    unsigned int invAlpha = 255 - alpha;

    r = (r * invAlpha + color.r * alpha) >> 8;
    g = (g * invAlpha + color.g * alpha) >> 8;
    b = (b * invAlpha + color.b * alpha) >> 8;

    // Convert back to 4-bit
    r >>= 4;
    g >>= 4;
    b >>= 4;
    a = 0xF; // Fully opaque

    *pixel = (WORD)((a << 12) | (r << 8) | (g << 4) | b);
}*/

void setPixel(BYTE* pixel, COLORREF color) {
    // Extract RGB components from COLORREF (assumes 0x00BBGGRR format)
    BYTE r = GetRValue(color); // Red component (0–255)
    BYTE g = GetGValue(color); // Green component (0–255)
    BYTE b = GetBValue(color); // Blue component (0–255)

    // Convert to RGB565
    WORD rgb565 = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);

    // Write to memory (little endian)
    pixel[0] = rgb565 & 0xFF;        // Low byte
    pixel[1] = (rgb565 >> 8) & 0xFF; // High byte
}


void DrawGlassTriangle(BYTE* pDst, int width, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, RGBQUAD *rgbPalette, int color_id,int t) {
    /*for (int x=0;x<GR_WIDTH;x++) {
      for (int y=0;y<GR_HEIGHT;y++) {
        BYTE* pixel = pDst + (y * width + x) * 4;
        blendPixel(pixel, color);
      }
    }*/
    //to a point where below function is faster than filling a square

   if (t>255) t=255;
   Color color={rgbPalette[color_id].rgbRed, 
                rgbPalette[color_id].rgbGreen, 
                rgbPalette[color_id].rgbBlue, t};

 
//=============
  float
    gradient_middle1,gradient_middle2,gradient_largest,
    c_middle1,c_middle2,c_largest,
    smallest=INT_MAX,largest=0,
    x_arr[3],y_arr[3];

  x_arr[0]=_x1;
  x_arr[1]=_x2;
  x_arr[2]=_x3;
  y_arr[0]=_y1;
  y_arr[1]=_y2;
  y_arr[2]=_y3;

  int x_1,x_2,i,x,y,k,saved_largest=0,saved_smallest=0,saved_middle=0;

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



  int smallest_y=smallest;
  int middle_y=y_arr[saved_middle];
  int largest_y=largest;


  int x1,x2;
  if (smallest_y>GR_HEIGHT)
    smallest_y=GR_HEIGHT;
  else if (smallest_y<0)
    smallest_y=0;
  if (middle_y>GR_HEIGHT)
    middle_y=GR_HEIGHT;
  else if (middle_y<0)
    middle_y=0;
  if (largest_y>GR_HEIGHT)
    largest_y=GR_HEIGHT;
  else if (largest_y<0)
    largest_y=0;


  for (int r=0;r<2;r++) {
    int miny,maxy;
    float gradient1,gradient2,c1,c2;
    switch (r) {
      case 0: //smallest to middle
        miny=smallest_y;
        maxy=middle_y;
        gradient1=gradient_middle1;
        gradient2=gradient_largest;
        c1=c_middle1;
        c2=c_largest;
        break;
      case 1: //middlle to largest
        miny=middle_y;
        maxy=largest_y;
        gradient1=gradient_middle2;
        gradient2=gradient_largest;
        c1=c_middle2;
        c2=c_largest;
        break;
    }

    for (y=miny;y<maxy;y+=1) {//small to middle
      x_1=GetX(y,gradient1,c1);//left point to right point
      x_2=GetX(y,gradient2,c2);
      x1=min(x_1,x_2);
      x2=max(x_1,x_2);
      if (x1>GR_WIDTH)
        x1=GR_WIDTH;
      else if (x1<0)
        x1=0;
      if (x2>GR_WIDTH)
        x2=GR_WIDTH;
      else if (x2<0)
        x2=0;

      if (y%2==0) {
        for (x=x1;x<x2;x+=2) {
          BYTE* pixel = pDst + (y * width + x) *2;
          if (x%8==0)
            blendPixel(pixel, color);
          else
            setPixel(pixel, RGB(rgbPalette[color_id].rgbRed, rgbPalette[color_id].rgbGreen, rgbPalette[color_id].rgbBlue));
        }
      } else {
        for (x=x1;x<x2;x+=3) {
          BYTE* pixel = pDst + (y * width + x) *2;
          if (x%12==0)
            blendPixel(pixel, color);
          else
            setPixel(pixel, RGB(rgbPalette[color_id].rgbRed, rgbPalette[color_id].rgbGreen, rgbPalette[color_id].rgbBlue));
        }
      }
    }
  }
}





void FastDrawTexturedTriangle(BYTE *pDst, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int width, BYTE *pTexture, int twidth, int theight) 
{
//=============
  float
    gradient_middle1,gradient_middle2,gradient_largest,
    c_middle1,c_middle2,c_largest,
    smallest=INT_MAX,largest=0,
    x_arr[3],y_arr[3];

  x_arr[0]=_x1;
  x_arr[1]=_x2;
  x_arr[2]=_x3;
  y_arr[0]=_y1;
  y_arr[1]=_y2;
  y_arr[2]=_y3;

  int x_1,x_2,i,x,y,k,saved_largest=0,saved_smallest=0,saved_middle=0;

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



  int smallest_y=smallest;
  int middle_y=y_arr[saved_middle];
  int largest_y=largest;


  int x1,x2;
  int tx,ty;
  if (smallest_y>GR_HEIGHT)
    smallest_y=GR_HEIGHT;
  else if (smallest_y<0)
    smallest_y=0;
  if (middle_y>GR_HEIGHT)
    middle_y=GR_HEIGHT;
  else if (middle_y<0)
    middle_y=0;
  if (largest_y>GR_HEIGHT)
    largest_y=GR_HEIGHT;
  else if (largest_y<0)
    largest_y=0;



  for (int r=0;r<2;r++) {
    int miny,maxy;
    float gradient1,gradient2,c1,c2;
    switch (r) {
      case 0: //smallest to middle
        miny=smallest_y;
        maxy=middle_y;
        gradient1=gradient_middle1;
        gradient2=gradient_largest;
        c1=c_middle1;
        c2=c_largest;
        break;
      case 1: //middlle to largest
        miny=middle_y;
        maxy=largest_y;
        gradient1=gradient_middle2;
        gradient2=gradient_largest;
        c1=c_middle2;
        c2=c_largest;
        break;
    }


  for (y=miny;y<maxy;y+=1) {//small to middle
    x_1=GetX(y,gradient1,c1);//left point to right point
    x_2=GetX(y,gradient2,c2);
    x1=min(x_1,x_2);
    x2=max(x_1,x_2);
    if (x1>GR_WIDTH)
      x1=GR_WIDTH;
    else if (x1<0)
      x1=0;
    if (x2>GR_WIDTH)
      x2=GR_WIDTH;
    else if (x2<0)
      x2=0;
    if (twidth==width) { //same as size as texture
      for (x=x1;x<x2;x+=twidth) {
        tx=x%twidth;
        ty=y%theight;
        memcpy(pDst + (y * width + x) *2,
               pTexture + (ty *twidth +tx) *2,
               (x2-x1)*2);
      }
    } else {
      int _l=x2-x1;
      int start_x=x1%twidth;
      int end_x=x2%twidth;
      //(x1 to x1-start_x+twidth)
      if (x1>=0 && x1<GR_WIDTH) { 
          int start_l=(twidth-start_x);
          if (_l<twidth) {
            /***
                           160 factor
                x1=============|--------------------x2
            ****/
            int x_mid=((x1+(twidth-1))/twidth)*twidth; //find smallest multiple
            int halfdist1=x_mid-x1;
            //Draw Left Side, x1 to nearest 160 factor
            if (halfdist1<twidth && halfdist1>0) {
              x=x1;
              tx=x%twidth;
              ty=y%theight;
              if (x_mid<x2) {
                memcpy(pDst + (y * width + x) *2,
                   pTexture + (ty *twidth +tx) *2,
                   halfdist1*2);
              } else {
                memcpy(pDst + (y * width + x) *2,
                   pTexture + (ty *twidth +tx) *2,
                   _l*2);
              }
            }
            //Draw Right Side nearest 160 factor to
            int halfdist2=x2-x_mid;
            if (halfdist2<twidth && halfdist2>0) {
              x=x_mid;
              tx=x%twidth;
              ty=y%theight;
              if (x_mid<x2) {
                memcpy(pDst + (y * width + x) *2,
                   pTexture + (ty *twidth +tx) *2,
                   halfdist2*2);
              }
            }
          } else {
            x=x1;
            if (start_l>0) {
              tx=x%twidth;
              ty=y%theight;
              memcpy(pDst + (y * width + x) *2,
                 pTexture + (ty *twidth +tx) *2,
                 start_l*2);
            }
          }
      }


      //x2-end_x to x2
      if (x2>=0 && x2<GR_WIDTH) { 
          int end_l=end_x;  
          if (_l>twidth) {
            if (end_l>0) {
              x=x2-end_x;
              tx=x%twidth; 
              ty=y%theight;
              memcpy(pDst + (y * width + x) *2,
                 pTexture + (ty *twidth +tx) *2,
                 end_l*2);
            }
          }
      }


      //inbetween
      for (x=x1-start_x+twidth;x<x2-end_x;x+=twidth) {
        tx=x%twidth;
        ty=y%theight;
        memcpy(pDst + (y * width + x) *2,
               pTexture + (ty *twidth +tx) *2,
               twidth*2);
      }
    }
  }


  }
}


