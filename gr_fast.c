

uint16_t byte_16_color_arr[256];

void FastFlipLargeBitmapVertically(BYTE* pDst, int dstWidth, const BYTE* pSrc, int srcWidth, int srcHeight, int bytesPerPixel)
{ //both same true size
    int bspace=bytesPerPixel/8;
    const int dstRowSize = ((32 * dstWidth + 31) / 32) * bspace;
    const int srcRowSize = ((32 * srcWidth + 31) / 32) * bspace;
    for (int y = 0; y < srcHeight; ++y) { //only height effects performance
        memcpy(
            pDst + y * dstRowSize,
            pSrc + (srcHeight - 1 - y) * dstRowSize,
            srcRowSize
        );
    }
}



typedef struct {
    uint8_t r, g, b, a;
} Color;


static inline void blendPixel32Bits(BYTE* pixel, Color color) {
    // Precompute alpha and inverse alpha
    unsigned int alpha = color.a;
    unsigned int invAlpha = 255 - alpha;

    // Blend each channel using integer math and bit shifting
    pixel[0] = (BYTE)((pixel[0] * invAlpha + color.b * alpha) >> 8); // Blue
    pixel[1] = (BYTE)((pixel[1] * invAlpha + color.g * alpha) >> 8); // Green
    pixel[2] = (BYTE)((pixel[2] * invAlpha + color.r * alpha) >> 8); // Red
    pixel[3] = 255; // Fully opaque
}

static inline void setPixel32Bits(BYTE* pixel, Color color) {
    pixel[0] = color.b; // Blue
    pixel[1] = color.g; // Green
    pixel[2] = color.r; // Red
    pixel[3] = color.a; // Alpha
}


void setPixel16Bits(uint16_t* pixel, uint16_t color) {
    *pixel=color;
}


void DrawGlassTriangle(BYTE* pDst, int width, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, RGBQUAD *rgbPalette, int color_id,int t, int bytesPerPixel) {
    /*for (int x=0;x<GR_WIDTH;x++) {
      for (int y=0;y<GR_HEIGHT;y++) {
        BYTE* pixel = pDst + (y * width + x) * 4;
        blendPixel(pixel, color);
      }
    }*/
    //to a point where below function is faster than filling a square
   int bspace=bytesPerPixel/8;

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

        if (bytesPerPixel==32) {
          for (x=x1;x<x2;x+=1) {
            BYTE* pixel = pDst + (y * width + x) *bspace;
            blendPixel32Bits(pixel, color);
            //setPixel32Bits(pixel, color);
          }
        } else if (bytesPerPixel==16) {
          //for (x=x1;x<x2;x+=1) {
            //BYTE* pixel = pDst + (y * width + x) *bspace;
            //setPixel16Bits(pixel, low_byte, high_byte);
          //}

          //draws line of colored pixels in 1 straight line, //red=0xF800;
          //BYTE* dest = pDst + (y * width + x1) *bspace;
          //asm volatile (
          //      "rep stosw"
          //      : /* no outputs */
          //      : "c"(count), "D"(dest), "a"(_color)
          //      : "memory"
          //);          

          uint16_t _color=byte_16_color_arr[color_id];
          if (y%2==0) {
            for (int x=x1;x<x2;x++) {
              uint16_t* pixel = (uint16_t*)(pDst + (y * width + x) * bspace);
              setPixel16Bits(pixel,_color);
              if (x%2==0) {
                x+=1;
              }
            }
          } else {
            for (int x=x1+1;x<x2;x++) {
              uint16_t* pixel = (uint16_t*)(pDst + (y * width + x) * bspace);
              setPixel16Bits(pixel,_color);
              if (x%3==0) {  
                x+=1;
              }
            }
          }
        }
      }
    } 
}





void FastDrawTexturedTriangle(BYTE *pDst, int _x1, int _y1, int _x2, int _y2, int _x3, int _y3, int width, BYTE *pTexture, int twidth, int theight, int bytesPerPixel) 
{
  int bspace=bytesPerPixel/8;
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
        memcpy(pDst + (y * width + x) *bspace,
               pTexture + (ty *twidth +tx) *bspace,
               (x2-x1)*bspace);
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
                memcpy(pDst + (y * width + x) *bspace,
                   pTexture + (ty *twidth +tx) *bspace,
                   halfdist1*bspace);
              } else {
                memcpy(pDst + (y * width + x) *bspace,
                   pTexture + (ty *twidth +tx) *bspace,
                   _l*bspace);
              }
            }
            //Draw Right Side nearest 160 factor to
            int halfdist2=x2-x_mid;
            if (halfdist2<twidth && halfdist2>0) {
              x=x_mid;
              tx=x%twidth;
              ty=y%theight;
              if (x_mid<x2) {
                memcpy(pDst + (y * width + x) *bspace,
                   pTexture + (ty *twidth +tx) *bspace,
                   halfdist2*bspace);
              }
            }
          } else {
            x=x1;
            if (start_l>0) {
              tx=x%twidth;
              ty=y%theight;
              memcpy(pDst + (y * width + x) *bspace,
                 pTexture + (ty *twidth +tx) *bspace,
                 start_l*bspace);
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
              memcpy(pDst + (y * width + x) *bspace,
                 pTexture + (ty *twidth +tx) *bspace,
                 end_l*bspace);
            }
          }
      }


      //inbetween
      for (x=x1-start_x+twidth;x<x2-end_x;x+=twidth) {
        tx=x%twidth;
        ty=y%theight;
        memcpy(pDst + (y * width + x) *bspace,
               pTexture + (ty *twidth +tx) *bspace,
               twidth*bspace);
      }
    }
  }


  }
}


