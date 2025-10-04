void DrawStar(HDC hdc,int x, int y, int size, float angl,int color)
{
/*
                X1,Y1

                  ^^
                 //\\
    X5,Y5   ==============   X2,Y2
              ==========
               // \ / \\
              // /   \ \\

          X4,Y4         X3,Y3

*/

  int x1=x-(size)*cos(angl);
  int y1=y-(size)*sin(angl);
  angl-=2*M_PI/5;
  int x2=x-(size)*cos(angl);
  int y2=y-(size)*sin(angl);
  angl-=2*M_PI/5;
  int x3=x-(size)*cos(angl);
  int y3=y-(size)*sin(angl);
  angl-=2*M_PI/5;
  int x4=x-(size)*cos(angl);
  int y4=y-(size)*sin(angl);
  angl-=2*M_PI/5;
  int x5=x-(size)*cos(angl);
  int y5=y-(size)*sin(angl);

  GrLine(hdc,x4,y4,x1,y1,color);
  GrLine(hdc,x1,y1,x3,y3,color); 
  GrLine(hdc,x3,y3,x5,y5,color);
  GrLine(hdc,x5,y5,x2,y2,color);
  GrLine(hdc,x2,y2,x4,y4,color);
}

void PlaceDayMoon()
{
  if (!is_moon) { //cancel operation if is_moon not enabled
    return;
  }

  int dmx=-300,dmy=-300;
  if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
    dmx=GR_WIDTH/8-GR_WIDTH/16;
    dmy=GR_HEIGHT-GR_HEIGHT/3-GR_HEIGHT/16;//GR_HEIGHT-GR_HEIGHT/6;
  } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
    dmx=GR_WIDTH/4-GR_WIDTH/16;
    dmy=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3-GR_HEIGHT/16;
  } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
    dmx=GR_WIDTH/4+GR_WIDTH/8-GR_WIDTH/16;
    dmy=GR_HEIGHT/4+GR_HEIGHT/12-GR_HEIGHT/16;
  } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
    //dmx=GR_WIDTH/2-GR_WIDTH/16;
    //dmy=GR_HEIGHT/4-GR_HEIGHT/16;
    return;
  } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
    dmx=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8-GR_WIDTH/16;
    dmy=GR_HEIGHT/4+GR_HEIGHT/12-GR_HEIGHT/16;
  } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
    dmx=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/16;
    dmy=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3-GR_HEIGHT/16;
  } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
    dmx=GR_WIDTH-GR_WIDTH/8-GR_WIDTH/16;
    dmy=GR_HEIGHT-GR_HEIGHT/3-GR_HEIGHT/16;//GR_HEIGHT-GR_HEIGHT/6;
  }

  //bitmap stuff
  BITMAP backgroundbitmap;
  GetObject(map_background_sprite,sizeof(BITMAP),&backgroundbitmap);

  BITMAP moonbitmap;
  GetObject(draw_mirror_moon_sprite[current_moon_phase_id].sprite_paint,sizeof(BITMAP),&moonbitmap);

  HDC hdc=GetDC(NULL);
  HDC hdcSrc=CreateCompatibleDC(hdc);
  HDC hdcDest=CreateCompatibleDC(hdc);

  HBITMAP tmp_bitmap=CreateLargeBitmap(backgroundbitmap.bmWidth,backgroundbitmap.bmHeight,backgroundbitmap.bmBitsPixel);
  SelectObject(hdcDest,tmp_bitmap);
  SelectObject(hdcSrc,draw_mirror_moon_sprite[current_moon_phase_id].sprite_paint);


  int background_width=backgroundbitmap.bmWidth;
  int background_height=backgroundbitmap.bmHeight;

  int moon_width=moonbitmap.bmWidth;
  int moon_height=moonbitmap.bmHeight;

  //draw moon to back background
  BitBlt(hdcDest,dmx,dmy,background_width,background_height,hdcSrc,0,0,SRCCOPY);
  //BitBlt(hdcDest,dmx,dmy, GR_WIDTH+GR_WIDTH/8,GR_HEIGHT+GR_HEIGHT/8,hdcSrc,0,0,SRCCOPY);

  SelectObject(hdcSrc,map_background_sprite);

  BLENDFUNCTION blendFunction;
  blendFunction.BlendOp = AC_SRC_OVER;
  blendFunction.BlendFlags = 0;
  blendFunction.SourceConstantAlpha = 240;// Transparency level (0-255)
  blendFunction.AlphaFormat = 0;

  //draw background over black background + moon
  AlphaBlend(hdcDest, 0, 0, background_width, background_height,
               hdcSrc, 0, 0, background_width, background_height,
               blendFunction);

  DeleteDC(hdcSrc);
  DeleteDC(hdcDest);
  ReleaseDC(NULL,hdc);

  DeleteObject(map_background_sprite);
  map_background_sprite=CopyBitmap(tmp_bitmap,SRCCOPY);
  DeleteObject(tmp_bitmap);
}

void DrawStars(HDC hdc)
{
  GrRect(hdc,0,0,GR_WIDTH+24,GR_HEIGHT+24,BLACK);  
  for (int i=0;i<STAR_NUM;i++) {
      int _x = (Star[i].x * GR_WIDTH)/MAX_STAR_X;
      int _y = (Star[i].y * GR_HEIGHT)/MAX_STAR_Y;
      if (Star[i].size>0) {
        //SetPixel(hdc,_x,_y,WHITE);
        //SetPixel(hdc,_x+1,_y,WHITE);
        //SetPixel(hdc,_x+1,_y+1,WHITE);
        //SetPixel(hdc,_x+1,_y-1,WHITE);
        if (GR_WIDTH>1000)
          GrRect(hdc,_x,_y,3,3,WHITE);
        else
          GrRect(hdc,_x,_y,2,2,WHITE);
      }
      //GrCircle(hdc,_x,_y,Star[i].size,WHITE,WHITE);
  }
}



//Background
void DrawBackground(HDC hdc,HDC hdc2) 
{
  int 
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;

  float parralax_x=0;
  float parralax_y=0;
  float _ppx=((player.x-cx1-cx2-cx3)/MAP_WIDTH);
  float _ppy=((player.y-cy1-cy2-cy3)/MAP_HEIGHT);
  float ppx=(MAP_WIDTH-(player.x-cx1-cx2-cx3))/MAP_WIDTH;
  float ppy=(MAP_HEIGHT-(player.y-cy1-cy2-cy3))/MAP_HEIGHT;
  parralax_x=(-GR_WIDTH/8)+GR_WIDTH/16*ppx; 
  parralax_y=(-GR_HEIGHT/8)+GR_HEIGHT/16*ppy;
  int draw_p_px=(int)parralax_x;
  int draw_p_py=(int)parralax_y;


  GrRect(hdc,0,0,GR_WIDTH+24,GR_HEIGHT+24,custom_map_background_color);
  switch (map_background) {
    case 1:
      DrawStars(hdc);
      break;
    case 0:
    //case 1:
    case 2:
    case 3: //default backgrounds
      if (map_background_sprite!=NULL) {
        DrawBitmap(hdc,hdc2,draw_p_px,draw_p_py,0,0,GR_WIDTH*2,GR_HEIGHT*2,map_background_sprite,SRCCOPY,FALSE,FALSE);
      }
      break;
    default: //custom map background
      if (map_background_sprite!=NULL) {
        DrawBitmap(hdc,hdc2,draw_p_px,draw_p_py,0,0,GR_WIDTH*2,GR_HEIGHT*2,map_background_sprite,SRCCOPY,FALSE,FALSE);
      }
      break;
  }


  //Draw shooting stars
  if (map_background==1) {
    for (int i=0;i<SSTAR_NUM;i++) {
      if (SStar[i].lifetime>0) {
        GrCircle(hdc,SStar[i].x-GR_WIDTH/16*_ppx,SStar[i].y-GR_HEIGHT/16*_ppy,1,WHITE,WHITE);
      }
    }
  }


  //draw night moon
    if (
        ((!in_main_menu || (in_main_menu && map_weather==0 && !(solar_hour>6 && solar_hour<18))) &&
         ((
           !(map_background>=2 && map_background<=3) && 
           is_moon && 
           custom_map_background_color_i>127
          ) || map_background==1
        ))        
    ) {
      int dmx=-1000;
      int dmy=-1000;
      if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
        dmx=GR_WIDTH-GR_WIDTH/8;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;//GR_HEIGHT-GR_HEIGHT/6;
        //dmy=160+160-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
        dmx=GR_WIDTH/2+GR_WIDTH/4;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
        //dmy=160+110-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
        dmx=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT/4+GR_HEIGHT/12;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
        //dmy=160+50-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
        dmx=GR_WIDTH/2;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT/4;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
        //dmy=160-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
        dmx=GR_WIDTH/4+GR_WIDTH/8;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT/4+GR_HEIGHT/12;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
        //dmy=160+50-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
        dmx=GR_WIDTH/4;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
        //dmy=160+110-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
        dmx=GR_WIDTH/8;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
        dmy=GR_HEIGHT-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;//GR_HEIGHT-GR_HEIGHT/6;
        //dmy=160+160-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);
      }// else {
//      }
//      DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite[current_moon_phase_id],FALSE);

  }
}




void DrawWaterPlatformsReflection(HDC hdc, HDC hdc2,HBITMAP mirror_screen)
{
  if (WATER_GROUND_NUM>0) {
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int x1,y1,x2,y2,x3,y3;
  int i;
  if (mirror_screen!=NULL) {
  for (int k=0;k<WATER_GROUND_NUM;k++) {
    i = rendered_water_ground[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
            x1=GR_WIDTH/2+(int)Ground[i]->x1-px+cx1+cx2+cx3;
            y1=GR_HEIGHT/2+(int)Ground[i]->y1-py+cy1+cy2+cy3;
            x2=GR_WIDTH/2+(int)Ground[i]->x2-px+cx1+cx2+cx3;
            y2=GR_HEIGHT/2+(int)Ground[i]->y2-py+cy1+cy2+cy3;
            x3=GR_WIDTH/2+(int)Ground[i]->x3-px+cx1+cx2+cx3;
            y3=GR_HEIGHT/2+(int)Ground[i]->y3-py+cy1+cy2+cy3;
            DrawTexturedTriangle(hdc,hdc2,x1,y1,x2,y2,x3,y3,mirror_screen);
        }
      }
    }
  }
  }
  }
}




/*
void DrawWaterPlatformsTexture(HDC hdc,HDC hdc2)
{
  if (WATER_GROUND_NUM>0) {
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int x1,y1,x2,y2,x3,y3;
  int i,c;

  if (global_water_texture_timer>1) {
    global_water_texture_timer=0;
    global_water_texture_id++;
    if (global_water_texture_id>=9) {
      global_water_texture_id=0;
    }
  } else {
    global_water_texture_timer++;
  }


  for (int k=0;k<WATER_GROUND_NUM;k++) {
    i = rendered_water_ground[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        //c=rgbPaint[Ground[i]->color_id];
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
            x1=GR_WIDTH/2+(int)Ground[i]->x1-px+cx1+cx2+cx3;
            y1=GR_HEIGHT/2+(int)Ground[i]->y1-py+cy1+cy2+cy3;
            x2=GR_WIDTH/2+(int)Ground[i]->x2-px+cx1+cx2+cx3;
            y2=GR_HEIGHT/2+(int)Ground[i]->y2-py+cy1+cy2+cy3;
            x3=GR_WIDTH/2+(int)Ground[i]->x3-px+cx1+cx2+cx3;
            y3=GR_HEIGHT/2+(int)Ground[i]->y3-py+cy1+cy2+cy3;
            //DrawTexturedTriangle(hdc,hdc2,x1,y1,x2,y2,x3,y3,texture_water[global_water_texture_id]);
            //FastDrawTexturedTriangle(publicScreenPixels,x1,y1,x2,y2,x3,y3,SCREEN_WIDTH,ptexture_water[global_water_texture_id],160,160);
            //DrawGlassTriangle(publicScreenPixels, SCREEN_WIDTH, x1,y1,x2,y2,x3,y3,rgbColorsDefault,rgbPaint_i[Ground[i]->color_id],200);
        }
      }
    }
  }
  }
}*/


void FastDrawWaterPlatformsTexture()
{
  if (WATER_GROUND_NUM>0) {
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int x1,y1,x2,y2,x3,y3;
  int i,c;

  if (global_water_texture_timer>1) {
    global_water_texture_timer=0;
    global_water_texture_id++;
    if (global_water_texture_id>=9) {
      global_water_texture_id=0;
    }
  } else {
    global_water_texture_timer++;
  }


  for (int k=0;k<WATER_GROUND_NUM;k++) {
    i = rendered_water_ground[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        //c=rgbPaint[Ground[i]->color_id];
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
            x1=GR_WIDTH/2+(int)Ground[i]->x1-px+cx1+cx2+cx3;
            y1=GR_HEIGHT/2+(int)Ground[i]->y1-py+cy1+cy2+cy3;
            x2=GR_WIDTH/2+(int)Ground[i]->x2-px+cx1+cx2+cx3;
            y2=GR_HEIGHT/2+(int)Ground[i]->y2-py+cy1+cy2+cy3;
            x3=GR_WIDTH/2+(int)Ground[i]->x3-px+cx1+cx2+cx3;
            y3=GR_HEIGHT/2+(int)Ground[i]->y3-py+cy1+cy2+cy3;
            FastDrawTexturedTriangle(publicScreenPixels,x1,y1,x2,y2,x3,y3,SCREEN_WIDTH,ptexture_water[global_water_texture_id],160,160,global_screen_bits);
            //DrawGlassTriangle(publicScreenPixels, SCREEN_WIDTH, x1,y1,x2,y2,x3,y3,rgbColorsDefault,rgbPaint_i[Ground[i]->color_id],200);
        }
      }
    }
  }
  }
}

void FastDrawWaterPlatformsReflection()
{
  if (WATER_GROUND_NUM>0) {
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int x1,y1,x2,y2,x3,y3;
  int i;
  for (int k=0;k<WATER_GROUND_NUM;k++) {
    i = rendered_water_ground[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
            x1=GR_WIDTH/2+(int)Ground[i]->x1-px+cx1+cx2+cx3;
            y1=GR_HEIGHT/2+(int)Ground[i]->y1-py+cy1+cy2+cy3;
            x2=GR_WIDTH/2+(int)Ground[i]->x2-px+cx1+cx2+cx3;
            y2=GR_HEIGHT/2+(int)Ground[i]->y2-py+cy1+cy2+cy3;
            x3=GR_WIDTH/2+(int)Ground[i]->x3-px+cx1+cx2+cx3;
            y3=GR_HEIGHT/2+(int)Ground[i]->y3-py+cy1+cy2+cy3;
            FastDrawTexturedTriangle(publicScreenPixels,x1,y1,x2,y2,x3,y3,SCREEN_WIDTH,publicScreenMirrorPixels,SCREEN_WIDTH,GR_HEIGHT,global_screen_bits);
            //DrawGlassTriangle(publicScreenPixels, SCREEN_WIDTH, x1,y1,x2,y2,x3,y3,rgbColorsDefault,rgbPaint_i[Ground[i]->color_id],200);
        }
      }
    }
  }
  }
}


void DrawAlphaWaterColour(BYTE *pDst)
{
  if (WATER_GROUND_NUM>0) {
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int x1,y1,x2,y2,x3,y3;
  int i,c;
  //const float scalex=1.0023; //idk why it just works
  //const float scaley=1.023;
  for (int k=0;k<WATER_GROUND_NUM;k++) {
    i = rendered_water_ground[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
            /*float cx=(Ground[i]->x1+Ground[i]->x2+Ground[i]->x3)/3.0f;
            float cy=(Ground[i]->y1+Ground[i]->y2+Ground[i]->y3)/3.0f;
            float sx1 = cx + (Ground[i]->x1 - cx) * scalex;
            float sy1 = cy + (Ground[i]->y1 - cy) * scaley;
            float sx2 = cx + (Ground[i]->x2 - cx) * scalex;
            float sy2 = cy + (Ground[i]->y2 - cy) * scaley;
            float sx3 = cx + (Ground[i]->x3 - cx) * scalex;
            float sy3 = cy + (Ground[i]->y3 - cy) * scaley;

            x1=GR_WIDTH/2-px+cx1+cx2+cx3+(int)sx1;
            y1=GR_HEIGHT/2-py+cy1+cy2+cy3+(int)sy1;
            x2=GR_WIDTH/2-px+cx1+cx2+cx3+(int)sx2;
            y2=GR_HEIGHT/2-py+cy1+cy2+cy3+(int)sy2;
            x3=GR_WIDTH/2-px+cx1+cx2+cx3+(int)sx3;
            y3=GR_HEIGHT/2-py+cy1+cy2+cy3+(int)sy3;*/
            x1=GR_WIDTH/2+(int)Ground[i]->x1-px+cx1+cx2+cx3;
            y1=GR_HEIGHT/2+(int)Ground[i]->y1-py+cy1+cy2+cy3;
            x2=GR_WIDTH/2+(int)Ground[i]->x2-px+cx1+cx2+cx3;
            y2=GR_HEIGHT/2+(int)Ground[i]->y2-py+cy1+cy2+cy3;
            x3=GR_WIDTH/2+(int)Ground[i]->x3-px+cx1+cx2+cx3;
            y3=GR_HEIGHT/2+(int)Ground[i]->y3-py+cy1+cy2+cy3;
            if (!player.time_breaker) {
              DrawGlassTriangle(pDst, SCREEN_WIDTH, x1,y1,x2,y2,x3,y3,rgbColorsDefault,rgbPaint_i[Ground[i]->color_id],200,global_screen_bits);
            } else {
              DrawGlassTriangle(pDst, SCREEN_WIDTH, x1,y1,x2,y2,x3,y3,rgbColorsNoir,rgbPaint_i[Ground[i]->color_id],200,global_screen_bits);
            }

        }
      }
    }
  }
  }
}


void DrawFirePlatforms(HDC hdc)
{
  if (FIRE_GROUND_NUM>0 && level_loaded) {
  int x,y,x3,y3,
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int i,c;
  x=-(GR_WIDTH/2-px+cx1+cx2+cx3);
  y=-(GR_HEIGHT/2-py+cy1+cy2+cy3);
  for (int k=0;k<FIRE_GROUND_NUM;k++) {
    i = GroundFire[k]->ground_id;
    if (i!=-1) {
      if (Ground[i]->type>=7 && Ground[i]->type<=8) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
            if (Ground[i]->type==7) {
              GrLineThick(hdc,-x+Ground[i]->x1,-y+Ground[i]->y1,-x+GroundFire[k]->old_x3,-y+GroundFire[k]->old_y3,2,Ground[i]->color);
              GrLineThick(hdc,-x+Ground[i]->x2,-y+Ground[i]->y2,-x+GroundFire[k]->old_x3,-y+GroundFire[k]->old_y3,2,Ground[i]->color);
            }
            Draw1FireTrifill(hdc,i,x,y,x+GroundFire[k]->current_tx,y+GroundFire[k]->current_ty);

        }
        for (int j=0;j<GROUND_FIRE_SMOKE_NUM;j++) { //smoke coming out of fire
          GrCircle(hdc,-x+GroundFire[k]->smoke_x[j],-y+GroundFire[k]->smoke_y[j],7,DKGRAY,DKGRAY);
        }
      }
    }
  }
  }
}
/*
void DrawWaterPlatforms(HDC hdc,HDC hdc2) 
{
/*  int extra_h=0;
  if (hide_taskbar) {
    extra_h=8*4;
  }
  DrawBitmap(hDC,
                 0,
                 0,
                 player.x-player.cam_mouse_move_x-player.cam_move_x-GR_WIDTH/2,
                 player.y-player.cam_mouse_move_y-player.cam_move_y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+extra_h,
                 map_water_platforms_sprite_mask,SRCAND,FALSE,FALSE);
  //Draw water platforms paint
  DrawBitmap(hDC,
                 0,
                 0,                    
                 player.x-player.cam_mouse_move_x-player.cam_move_x-GR_WIDTH/2,
                 player.y-player.cam_mouse_move_y-player.cam_move_y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+extra_h,
                 map_water_platforms_sprite,SRCPAINT,FALSE,FALSE);*/
/*  int gid,tmf_id;
  int x,y,_x,_y;
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2-cx3;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2-cy3;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid>-1 && gid<VGRID_NUM && _x>=0 && _y>=0 && _x<=MAP_WIDTH && _y<=MAP_HEIGHT) {
      tmf_id=VGrid[gid]->draw_foreground_seg_id;
      if (tmf_id!=-1) {
        x=GR_WIDTH/2+TileMapForeground[tmf_id]->x-px+cx1+cx2+cx3;
        y=GR_HEIGHT/2+TileMapForeground[tmf_id]->y-py+cy1+cy2+cy3;
//        DrawBitmap(hdc,x, y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapForeground[tmf_id]->sprite_mask,SRCAND,FALSE,FALSE);
//        DrawBitmap(hdc,x,y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapForeground[tmf_id]->sprite_paint,SRCPAINT,FALSE,FALSE);
        //DrawSprite(hdc,x,y,&TileMapForeground[tmf_id]->draw_tile,FALSE);

        SelectObject(hdc2,TileMapForeground[tmf_id]->sprite_mask);
        BitBlt(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, SRCAND);

        SelectObject(hdc2,TileMapForeground[tmf_id]->sprite_paint);
        BitBlt(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, SRCPAINT);

      } 
    } 
  }
}
*/

void DrawPlatforms(HDC hdc,HDC hdc2)
{ //Dynamically scale with window size 
  //Draw platforms bitmap mask
  /*int extra_h=0;
  if (hide_taskbar)
    extra_h=8*4;
  DrawBitmap(hDC,
                 0,
                 0,
                 player.x-player.cam_mouse_move_x-player.cam_move_x-GR_WIDTH/2,
                 player.y-player.cam_mouse_move_y-player.cam_move_y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+extra_h,
                 map_platforms_sprite_mask,SRCAND,FALSE,FALSE);
  //Draw platforms paint
  DrawBitmap(hDC,
                 0,
                 0,                    
                 player.x-player.cam_mouse_move_x-player.cam_move_x-GR_WIDTH/2,
                 player.y-player.cam_mouse_move_y-player.cam_move_y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+extra_h,
                 map_platforms_sprite,SRCPAINT,FALSE,FALSE);*/

  int gid,tmp_id;
  int x,y,_x,_y;
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2-cx3;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2-cy3;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid>-1 && gid<VGRID_NUM && _x>=0 && _y>=0 && _x<=MAP_WIDTH && _y<=MAP_HEIGHT) {
      tmp_id=VGrid[gid]->draw_platform_seg_id;
      if (tmp_id!=-1) {
        x=GR_WIDTH/2+TileMapPlatform[tmp_id]->x-px+cx1+cx2+cx3;
        y=GR_HEIGHT/2+TileMapPlatform[tmp_id]->y-py+cy1+cy2+cy3;


        SelectObject(hdc2,TileMapPlatform[tmp_id]->sprite_mask);
        BitBlt(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, SRCAND);

        SelectObject(hdc2,TileMapPlatform[tmp_id]->sprite_paint);
        BitBlt(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, SRCPAINT);


//        DrawBitmap(hdc,x, y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapPlatform[tmp_id]->sprite_mask,SRCAND,FALSE,FALSE);
//        DrawBitmap(hdc,x,y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapPlatform[tmp_id]->sprite_paint,SRCPAINT,FALSE,FALSE);
        //DrawSprite(hdc,x,y,&TileMapPlatform[tmp_id]->draw_tile,FALSE);
      } 
    } 
  }
}


void DrawBlackBorders(HDC hdc)
{
  //Draw Black Borders
  //RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2;
  int w,l;
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  int x=GR_WIDTH/2+0-px+cx1+cx2+cx3;
  GrRect(hdc,0,0,x+14,GR_HEIGHT+14,BLACK); //left rect


  x=GR_WIDTH/2+MAP_WIDTH-px+cx1+cx2+cx3-14;
  w=GR_WIDTH-(MAP_WIDTH-player.x);
  if (w>0) {
    GrRect(hdc,x,0,w,GR_HEIGHT+14,BLACK); //right rect
  }

  int //y=GR_HEIGHT/2+0-py+cy1+cy2;
  //GrRect(hdc,0,0,GR_WIDTH+14,y+8,BLACK); //upper rect
   

  y=GR_HEIGHT/2+MAP_HEIGHT-py+cy1+cy2+cy3-14;
  l=GR_HEIGHT-(MAP_HEIGHT-player.y);
  if (l>0) {
    GrRect(hdc,-14,y,GR_WIDTH+18,l,BLACK); //lowerrect
  } 
}


void DrawShadows(HDC hdc,HDC hdc2)
{
  /*DrawGlassBitmap(hdc, 
        map_platforms_shadow_shader, 
        GR_WIDTH/2-player.x+player.cam_mouse_move_x+player.cam_move_x,
        GR_HEIGHT/2-player.y+player.cam_mouse_move_y+player.cam_move_y,
        32//128
  );*/
  int gid,tms_id;
  int x,y,_x,_y;
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  //2024-12-21 IMPORTANT: floating point in drawing sprite will cause grids to form and cause grid infighting = lag
  //turn it to int to make it appear as one singular bitmap
  //  GrCircle(hdc,GR_WIDTH/2-cx1+cx2,GR_HEIGHT/2-cy1+cy2,10,BLUE,BLUE); //debug

  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2-cx3;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2-cy3;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid>-1 && gid<VGRID_NUM && _x>=0 && _y>=0 && _x<=MAP_WIDTH && _y<=MAP_HEIGHT) {
      tms_id=VGrid[gid]->draw_shadow_seg_id;
      if (tms_id!=-1) {
        x=GR_WIDTH/2+TileMapShadow[tms_id]->x-px+cx1+cx2+cx3;
        y=GR_HEIGHT/2+TileMapShadow[tms_id]->y-py+cy1+cy2+cy3;
        SelectObject(hdc2,TileMapShadow[tms_id]->sprite_paint);
        AlphaBlend(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, gblendFunction);
      } else {
        x=GR_WIDTH/2+VGrid[gid]->x1-px+cx1+cx2+cx3;
        y=GR_HEIGHT/2+VGrid[gid]->y1-py+cy1+cy2+cy3;
        if (map_background==1 || (map_background==2 && custom_map_background_color_i>127)) {
          //GrGlassRect(hdc,x,y,VGRID_SIZE,VGRID_SIZE,DKRDKGRAY,32);
          SelectObject(hdc2,dkrdkgray_shadow_tile);
          AlphaBlend(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, gblendFunction);
        } else {
          //GrGlassRect(hdc,x,y,VGRID_SIZE,VGRID_SIZE,LTGRAY,32);
          SelectObject(hdc2,ltgray_shadow_tile);
          AlphaBlend(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, gblendFunction);
        }
      }
    }
  }
}


void DrawCursor(HDC hDC,HDC hDC2)
{
  if (player.invalid_shoot_timer>0) {
    if (player.invalid_shoot_timer==10) {
      if (game_audio) {
        PlayMemSnd(&channelSoundEffect[12],&channelSoundEffectCache[12],TRUE,5);
      }
    }
    player.invalid_shoot_timer--;
  }

  if (!(player.speed>24 && frame_tick%2==0)) {
  int rand_shake_x=0;
  int rand_shake_y=0;
  if (player.invalid_shoot_timer>0) {
    if (player.invalid_shoot_timer>7) {
      rand_shake_x=RandNum(-10,10,&misc_rng_i,-1);
      rand_shake_y=RandNum(-10,10,&misc_rng_i,-1);
    } else  {
      rand_shake_x=RandNum(-5,5,&misc_rng_i,-1);
      rand_shake_y=RandNum(-5,5,&misc_rng_i,-1);
    }
  }


/*
open
        1     5
    0           4
  2    3       6  7



closed 
        9     13
    8           12
 10   11       14   15


bee-open
      1 2 3
0               4


bee-closed
     6 7 8

5               9

*/
  if (player.type==0) {
  //DrawBitmap(hDC,hDC2,mouse_x,mouse_y,0,0,64,64,player_cursor[0],SRCCOPY,FALSE,FALSE);
  //right side eyes
  switch (player.knives_per_throw) {
    case 1: //right->closed
    case 3:
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+46,mouse_y+rand_shake_y-12,&draw_player_cursor[13],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+50,mouse_y+rand_shake_y+10,&draw_player_cursor[14],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+60,mouse_y+rand_shake_y+6,&draw_player_cursor[15],FALSE);
      break;

    case 0:
    case 5: //right->opened
    case 15:
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+46,mouse_y+rand_shake_y-12,&draw_player_cursor[5],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+50,mouse_y+rand_shake_y+10,&draw_player_cursor[6],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+60,mouse_y+rand_shake_y+6,&draw_player_cursor[7],FALSE);
      break;
  }



  //left side eyes
  switch (player.knives_per_throw) {
    case 1:
    case 5:
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+16,mouse_y+rand_shake_y-24,&draw_player_cursor[9],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x-22,mouse_y+rand_shake_y+20,&draw_player_cursor[10],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+24,mouse_y+rand_shake_y+20,&draw_player_cursor[11],FALSE);
      break;
    case 0:
    case 3:
    case 15:
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+16,mouse_y+rand_shake_y-24,&draw_player_cursor[1],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x-22,mouse_y+rand_shake_y+20,&draw_player_cursor[2],FALSE);
      DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+24,mouse_y+rand_shake_y+20,&draw_player_cursor[3],FALSE);
      break;
  }


  if (player.health>PLAYER_LOW_HEALTH) {
    DrawSprite(hDC,hDC2,mouse_x-2,mouse_y-2,&draw_player_cursor[0],FALSE); //left ete open
    DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-4,&draw_player_cursor[4],FALSE); //right eye open
  } else {
    DrawSprite(hDC,hDC2,mouse_x-2,mouse_y-2,&draw_player_cursor[8],FALSE); //left eye low hp
    DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-4,&draw_player_cursor[12],FALSE); //right eye low hp
  }

  } else if (player.type==1) {
  //bee eyes
    switch (player.knives_per_throw) {
      case 1: 
      case 3:
      case 0:
      case 5:
      case 15:
        /*DrawSprite(hDC,hDC2,mouse_x-1,mouse_y-8,&draw_player_cursorbee[0],FALSE); //left main eye
        DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+23+2,mouse_y+rand_shake_y-25,&draw_player_cursorbee[1],FALSE); //middle left
        DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+35+2,mouse_y+rand_shake_y-15,&draw_player_cursorbee[3],FALSE); //middle
        DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+40+1+2,mouse_y+rand_shake_y-23,&draw_player_cursorbee[2],FALSE); //middle right
        DrawSprite(hDC,hDC2,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-3,&draw_player_cursorbee[4],FALSE);*/ //right eye
        break;
      }
  }

  } //end of flicker fast
}


