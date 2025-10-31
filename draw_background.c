


//=================Moon============================
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
//================================

//=======Draw Stars===============
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
//=============================

//=======Clouds ==============
void CloudAct()
{
  bool flipped;
  int x,y,type,max_speed_timer;
  for (int i=0;i<CLOUD_NUM;i++) {
    if (GameCloud[i].speed_timer>0) {
      GameCloud[i].speed_timer--;
    } else {
      GameCloud[i].x--;
      GameCloud[i].speed_timer=GameCloud[i].max_speed_timer;
    }
    if (GameCloud[i].type>=0 && GameCloud[i].type<LOADED_CLOUD_NUM) {
    if (GameCloud[i].x<-DrawGameCloud[GameCloud[i].type].l) {
      y=RandNum(0,GR_HEIGHT/2,&cloud_rng_i,-1);
      type=RandNum(0,LOADED_CLOUD_NUM,&cloud_rng_i,-1);
      max_speed_timer=5+RandNum(1,8,&cloud_rng_i,-1);
      x=GR_WIDTH+DrawGameCloud[type].l+2+RandNum(50,DrawGameCloud[type].l*2,&cloud_rng_i,-1);
      flipped=(bool)RandNum(0,1,&cloud_rng_i,-1);
      SetCloud(i,x,y,type,max_speed_timer,flipped);
    }
    }
  }
}


void SetCloud(int i, int x, int y, int type,int max_speed_timer,bool flipped)
{
  GameCloud[i].x=x;
  GameCloud[i].y=y;
  GameCloud[i].type=type;
  GameCloud[i].max_speed_timer=max_speed_timer;
  GameCloud[i].speed_timer=max_speed_timer;
  GameCloud[i].flipped=flipped;
}


void InitClouds()
{
  bool flipped;
  int x,y,type,max_speed_timer;
  cloud_rng_i=0;
  for (int i=0;i<CLOUD_NUM;i++) {
    //x=RandNum((GR_WIDTH*2/CLOUD_NUM *(i)),GR_WIDTH*2,&cloud_rng_i,-1);
    x=GR_WIDTH*2/CLOUD_NUM *(i);
    y=RandNum(0,GR_HEIGHT/2,&cloud_rng_i,-1);
    type=RandNum(0,LOADED_CLOUD_NUM,&cloud_rng_i,-1);
    max_speed_timer=5+RandNum(1,8,&cloud_rng_i,-1);
    flipped=(bool)RandNum(0,1,&cloud_rng_i,-1);
    SetCloud(i,x,y,type,max_speed_timer,flipped);//max_speed_timer);
  }
}


void DrawCloud(HDC hdc,HDC hdc2,int i)
{
  DrawSprite(hdc, hdc2,GameCloud[i].x,GameCloud[i].y,&DrawGameCloud[GameCloud[i].type].draw_sprite,GameCloud[i].flipped);
}


void DrawClouds(HDC hdc, HDC hdc2)
{
  for (int i=0;i<CLOUD_NUM;i++) {
    DrawCloud(hdc,hdc2, i);
  }
}
//============================================================================


//==============Sun==============================
void InitSun()
{
  Sun.overcast_timer=0;
  Sun.overcast_lvl=0;
  sun_rng_i=0;
  Sun.current_sun_i=0;
  Sun.rays_visible_num=0;
  Sun.flag_move_cycle=0;

  Sun.x=GR_WIDTH/2+GR_WIDTH/4;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
  Sun.y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;

  //Sun.x=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
  //Sun.y=GR_HEIGHT/4+GR_HEIGHT/12;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;

  //Sun.x=GR_WIDTH/2;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
  //Sun.y=GR_HEIGHT/4;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;

  Sun.flag_overcast=TRUE;

  int l=max(GR_WIDTH,GR_HEIGHT);
  for (int i=0;i<SUN_RAY_NUM;i++) {
    //Sun.ray_unblocked_timer[i]=0;
    Sun.ray_is_blocked[i]=TRUE;
    Sun.ray_angle[i]=(2*M_PI)/SUN_RAY_NUM*i;
    //Sun.ray_spin_angle[i]=(2*M_PI)/SUN_RAY_NUM*i;
    Sun.ray_l[i]=l;
    Sun.draw_ray_x[i]=Sun.x+l*cos(Sun.ray_angle[i]);
    Sun.draw_ray_y[i]=Sun.y+l*sin(Sun.ray_angle[i]);
    //Sun.draw_ray_ox[i]=Sun.x+63*cos(Sun.ray_angle[i]);
    //Sun.draw_ray_oy[i]=Sun.y+63*sin(Sun.ray_angle[i]);
    Sun.ray_x[i]=Sun.x+62*cos(Sun.ray_angle[i]);
    Sun.ray_y[i]=Sun.y+62*sin(Sun.ray_angle[i]);
  }
}



// Assumes 32-bit BGRA format, top-down DIB
bool IsPixelColorAt(BYTE* pixels, int width, int height, int x, int y, COLORREF targetColor) {
    if (!pixels || x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }

    // Calculate the offset: 4 bytes per pixel (BGRA)
    int offset = (y * width + x) * 4;

    BYTE blue  = pixels[offset + 0];
    BYTE green = pixels[offset + 1];
    BYTE red   = pixels[offset + 2];

    COLORREF pixelColor = RGB(red, green, blue);


    //printf("::actual:%d,target:%d::\n",pixelColor,targetColor);
    return pixelColor == targetColor;
}

void DrawSunRays(HDC hdc,HDC hdc2)
{

  //for (int k=0;k<2;k++) {
    Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], custom_map_background_color);
    Sun.current_sun_ray_i++;
    if (Sun.current_sun_ray_i>SUN_RAY_NUM-1) {
      Sun.current_sun_ray_i=0;
    }
  //}

  for (int i=0;i<SUN_RAY_NUM;i++) {
    if (!Sun.ray_is_blocked[i]) {
        //GrLineThick(hdc,Sun.ray_x[i],Sun.ray_y[i],Sun.draw_ray_x[i],Sun.draw_ray_y[i],3,LTRYELLOW);
      GrLine(hdc,Sun.draw_ray_x[i],Sun.draw_ray_y[i],Sun.ray_x[i],Sun.ray_y[i],LTRYELLOW);
      Sun.rays_visible_num++;
    }
  }

  if (!player.time_breaker) {
    //OverCast Actions
    if (Sun.rays_visible_num<=SUN_RAY_NUM*2-5) { //begin overcast if half is covered
      if (Sun.overcast_timer<30)
        Sun.overcast_timer+=2;
    } else { // more than half is shown, stop overcast + shine effect
      if (Sun.overcast_timer>0)
        Sun.overcast_timer-=2;
      if (Sun.overcast_timer<=0)
        Sun.overcast_timer=0;
    }

    if (Sun.flag_move_cycle==2) {
      if (Sun.overcast_lvl!=3 && Sun.rays_visible_num<=3) { //darkest
        Sun.overcast_lvl=3;
        Sun.flag_overcast=TRUE;
      } else if (Sun.overcast_lvl!=2 && Sun.rays_visible_num>=8 && Sun.rays_visible_num<=12) {//darker
        Sun.overcast_lvl=2;
        Sun.flag_overcast=TRUE;
      } else if (Sun.overcast_lvl!=1 && Sun.rays_visible_num>=20 && Sun.rays_visible_num<=SUN_RAY_NUM*2-8) { //dark
        Sun.overcast_lvl=1;
        Sun.flag_overcast=TRUE;    
      } else if (Sun.overcast_lvl!=0 && Sun.rays_visible_num>SUN_RAY_NUM*2-4) { //brightest
        Sun.overcast_lvl=0;
        Sun.flag_overcast=TRUE;
      }

      if (Sun.flag_overcast) {
        switch (Sun.overcast_lvl) {
          case 0:
            for (int i=0;i<PLATFORM_GRID_NUM;i++) {
              BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDefault);
            }
            break;
          case 1:
             for (int i=0;i<PLATFORM_GRID_NUM;i++) {
             BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDarker1);
            }
            break;
          case 2:
            for (int i=0;i<PLATFORM_GRID_NUM;i++) {
              BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDarker2);
            }
            break;
          case 3:
            for (int i=0;i<PLATFORM_GRID_NUM;i++) {
              BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDarker3);
            }
            break;
        }
        Sun.flag_overcast=FALSE;
      }
      Sun.rays_visible_num=0;
      Sun.flag_move_cycle=0;
    }
    Sun.flag_move_cycle++;
  }

}


void DrawSun(HDC hdc)
{
  GrCircle(hdc,Sun.x,Sun.y,60,LTRYELLOW,LTRYELLOW);
}


void SunRayAct()
{
  if (Sun.overcast_lvl==0) { 
    Sun.ray_l[Sun.current_sun_i]=60+sun_ray_l[sun_rng_i];
  } else if (Sun.overcast_lvl==1){
    Sun.ray_l[Sun.current_sun_i]=60+sun_ray_l[sun_rng_i]*3;
  } else {
    Sun.ray_l[Sun.current_sun_i]=60+sun_ray_l[sun_rng_i]*5;
  }
  //RandNum(0,max(GR_WIDTH/2,GR_HEIGHT/2),&sun_rng_i,-1);
  sun_rng_i++;
  if (sun_rng_i>=9)
    sun_rng_i=0;

  Sun.draw_ray_x[Sun.current_sun_i]=Sun.x+Sun.ray_l[Sun.current_sun_i]*cos(Sun.ray_angle[Sun.current_sun_i]);
  Sun.draw_ray_y[Sun.current_sun_i]=Sun.y+Sun.ray_l[Sun.current_sun_i]*sin(Sun.ray_angle[Sun.current_sun_i]);
  Sun.ray_x[Sun.current_sun_i]=Sun.x+62*cos(Sun.ray_angle[Sun.current_sun_i]);
  Sun.ray_y[Sun.current_sun_i]=Sun.y+62*sin(Sun.ray_angle[Sun.current_sun_i]);

  Sun.current_sun_i++;
  if (Sun.current_sun_i>SUN_RAY_NUM-1) {
    Sun.current_sun_i=0;
  }

  for (int i=0;i<SUN_RAY_NUM;i++) {
    Sun.ray_angle[i]+=0.01;
    if (Sun.ray_angle[i]>2*M_PI) {
      Sun.ray_angle[i]-=2*M_PI;
    }
  }
}


//===============Background===================
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
    case 0:
      DrawSun(hdc);
      DrawClouds(hdc,hdc2);
      break;
    case 1:
      DrawStars(hdc);
      break;

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
//==============================================================================

