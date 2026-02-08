


//=================Moon============================
/*void PlaceDayMoon()
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
  GetObject(Moon[current_moon_phase_id].draw_moon_sprite[0].sprite_paint,sizeof(BITMAP),&moonbitmap);

  HDC hdc=GetDC(NULL);
  HDC hdcSrc=CreateCompatibleDC(hdc);
  HDC hdcDest=CreateCompatibleDC(hdc);

  HBITMAP tmp_bitmap=CreateLargeBitmap(backgroundbitmap.bmWidth,backgroundbitmap.bmHeight,backgroundbitmap.bmBitsPixel);
  SelectObject(hdcDest,tmp_bitmap);
  SelectObject(hdcSrc,Moon[current_moon_phase_id].draw_moon_sprite[0].sprite_paint);


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
}*/
//================================


#define SUN_SIZE    32

//=======Draw Stars===============
void InitShootingStars()
{
  sstar_rng_i=0;
  for (int i=0;i<SSTAR_NUM;i++) {
    SStar[i].lifetime=0;
    SStar[i].cooldown=0;
    SStar[i].speed=10.0;
  }
}


void ShootingStarAct()
{
  for (int i=0;i<SSTAR_NUM;i++) {
    //movement
    if (SStar[i].lifetime<1) { //how long it appears
      if (SStar[i].cooldown<1) { //how long it doesnt appear
        SStar[i].x=RandNum(0,GR_WIDTH,&sstar_rng_i,-1);
        SStar[i].y=RandNum(0,GR_HEIGHT,&sstar_rng_i,-1);
        SStar[i].angle=RandAngle(0,180,&sstar_rng_i,-1);
        SStar[i].lifetime=5+RandNum(0,(GR_WIDTH+GR_HEIGHT)/60,&sstar_rng_i,-1);
        SStar[i].cooldown=1000+RandNum(0,(GR_WIDTH+GR_HEIGHT)*2,&sstar_rng_i,-1);
      } else {
        SStar[i].cooldown--;
      }
    } else {
      SStar[i].x+=SStar[i].speed*cos(SStar[i].angle);
      SStar[i].y+=SStar[i].speed*sin(SStar[i].angle);
      SStar[i].lifetime--;
    }
  }
}


void StarAct()
{
  for (int i=0;i<STAR_NUM;i++) {
    Star.timer[i]--;
    if (Star.timer[i]<1) {
      Star.size[i]=RandNum(0,4,&star_rng_i,-1);
      Star.timer[i]=RandNum(50,350,&star_rng_i,-1);
    } 

    Star.x[i]=Star.pivot_x+Star.dist_l[i]*cos(Star.angle[i]);
    Star.y[i]=Star.pivot_y+Star.dist_l[i]*sin(Star.angle[i]);
    //Star.angle[i]+=0.0025;
    //if (Star.angle[i]<=0) {
      //Star.angle[i]+=2*M_PI;
    //}
    //if (Star.angle[i]>=2*M_PI) {
      //Star.angle[i]-=2*M_PI;
    //}
  }
}


void DrawStars(HDC hdc)
{
  //GrRect(hdc,0,0,GR_WIDTH+24,GR_HEIGHT+24,BLACK);  
  int scnt=1;
  switch (Sun.horizon_lvl) {
    case -1:
    default:
      scnt=0;
      break;
    case 0: //emerging
      scnt=8;
      break;
    case 1:
      scnt=4;
      break;
    case 2:
      scnt=2;
      break;
    case 3: //final stage
      scnt=1;
      break;
  }

  for (int i=0;i<STAR_NUM;i+=scnt) {
      int _x = Star.x[i];//(Star.x[i] * GR_WIDTH)/MAX_STAR_X;
      int _y = Star.y[i];//(Star.y[i] * GR_HEIGHT)/MAX_STAR_Y;
      if (Star.size[i]>0) {
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
void LoadClouds(HDC hdc,HDC hdc2) //runs once
{
  HDC thdcDst=CreateCompatibleDC(hdc);
  HDC thdcSrc=CreateCompatibleDC(hdc);

  for (int k=0;k<2;k++) {
    if (k==0)
      SelectObject(thdcSrc,cloudwhite8bit_sprite_2);
    else
      SelectObject(thdcSrc,cloudgrey8bit_sprite_2);
    for (int i=0;i<2;i++) {
      DrawGameClouds[k][i].sprite_cache=CreateCrunchyBitmap(cloud_l[i],cloud_w[i]); //size of bitmap
      SelectObject(thdcDst,DrawGameClouds[k][i].sprite_cache);
      BitBlt(thdcDst, 0, 0, cloud_l[i], cloud_w[i], thdcSrc, cloud_src_x[i], cloud_src_y[i],SRCCOPY); //axis from large src
    }
    //

    if (k==0)
      SelectObject(thdcSrc,cloudwhite8bit_sprite_1);
    else
      SelectObject(thdcSrc,cloudgrey8bit_sprite_1);
    for (int i=2;i<LOADED_CLOUD_NUM;i++) {
      DrawGameClouds[k][i].sprite_cache=CreateCrunchyBitmap(cloud_l[i],cloud_w[i]); //size of bitmap
      SelectObject(thdcDst,DrawGameClouds[k][i].sprite_cache);
      BitBlt(thdcDst, 0, 0, cloud_l[i], cloud_w[i], thdcSrc, cloud_src_x[i], cloud_src_y[i],SRCCOPY); //axis from large src
    }

    //Generate Draw Sprites
    for (int i=0;i<LOADED_CLOUD_NUM;i++) {
      DrawGameClouds[k][i].l=cloud_l[i];                
      ReplaceBitmapColor(DrawGameClouds[k][i].sprite_cache,LTGREEN,BLACK);
      //GenerateDrawSprite(&DrawGameClouds[k][i].draw_sprite,DrawGameClouds[k][i].sprite_cache);
    }
  }

  DeleteDC(thdcDst);
  DeleteDC(thdcSrc);
}



bool prelude_clouds_loaded=FALSE;
int prelude_clouds_i=0;
int prelude_cloudy_clouds_i=0;
int prelude_clouds_paint=0;

void PreludeLoadCloudBackgroundSprite(HDC hdc,HDC hdc2)
{
  //Create Bitmap and Begin placing clouds on bitmap
  if (prelude_clouds_i==0) {
    if (prelude_clouds_paint==0) { //Create sprite paint 1
      for (int k=0;k<4;k++) {
        DrawGameCloudsBackground[k].sprite_paint1=CreateCrunchyBitmap(SCREEN_WIDTH,(SCREEN_HEIGHT/2+SCREEN_HEIGHT/4)); //max size
        SelectObject(hdc,DrawGameCloudsBackground[k].sprite_paint1);
        GrRect(hdc,0,0,SCREEN_WIDTH+1,SCREEN_HEIGHT/2+SCREEN_HEIGHT/4+1,YELLOW);
      }
    } else if (prelude_clouds_paint==1) { //Create sprite paint 2
      for (int k=0;k<4;k++) {
        DrawGameCloudsBackground[k].sprite_paint2=CreateCrunchyBitmap(SCREEN_WIDTH,(SCREEN_HEIGHT/2+SCREEN_HEIGHT/4));
        SelectObject(hdc,DrawGameCloudsBackground[k].sprite_paint2);
        GrRect(hdc,0,0,SCREEN_WIDTH+1,SCREEN_HEIGHT/2+SCREEN_HEIGHT/4+1,YELLOW/*BROWN*/);
      }
    } 
    prelude_clouds_paint++;
  }
  
  //Draw Clouds
  if (prelude_clouds_paint==2) {
    int x;
    int y;
    int type;
    int flipped;

    if (prelude_clouds_i>=DRAW_CLOUDS_NUM) { //after normal clouds are drawn, draw cloudy clouds
      type=RandNum(0,LOADED_CLOUD_NUM,&cloud_rng_i,-1);
      y=-cloud_w[type]+RandNum(0,SCREEN_HEIGHT/2+SCREEN_HEIGHT/8,&cloud_rng_i,-1);
      x=RandNum(0,SCREEN_WIDTH*2,&cloud_rng_i,-1);
      flipped=(bool)RandNum(0,1,&cloud_rng_i,-1);

      if (x<SCREEN_WIDTH) { //sprite1 cross over to sprite 2, draw on sprite 2 as well (left to right)  S1 [] S2
        if (x+cloud_l[type]>=SCREEN_WIDTH) {
          BitBlt8BitTransparent(DrawGameCloudsBackground[2].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[3].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
          x-=(SCREEN_WIDTH);
          BitBlt8BitTransparent(DrawGameCloudsBackground[2].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[3].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
        } else {
          BitBlt8BitTransparent(DrawGameCloudsBackground[2].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[3].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
        }
       } else {  //sprite2 cross over to sprite , draw on sprite 1 as well (left to right)   ] S1 S2 [
         if (x+cloud_l[type]>SCREEN_WIDTH*2) {
           x-=SCREEN_WIDTH;
           BitBlt8BitTransparent(DrawGameCloudsBackground[2].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
           BitBlt8BitTransparent(DrawGameCloudsBackground[3].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
           x-=SCREEN_WIDTH;
           BitBlt8BitTransparent(DrawGameCloudsBackground[2].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
           BitBlt8BitTransparent(DrawGameCloudsBackground[3].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
         } else {
           x-=SCREEN_WIDTH;
           BitBlt8BitTransparent(DrawGameCloudsBackground[2].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
           BitBlt8BitTransparent(DrawGameCloudsBackground[3].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
         }
       }

      prelude_cloudy_clouds_i++;
      loading_numerator++;
      if (prelude_cloudy_clouds_i>=DRAW_CLOUDY_CLOUDS_NUM) {
        prelude_clouds_loaded=TRUE; //completed
      }
    } else {
      type=RandNum(0,LOADED_CLOUD_NUM,&cloud_rng_i,-1);
      y=-cloud_w[type]+RandNum(0,SCREEN_HEIGHT/2+SCREEN_HEIGHT/8,&cloud_rng_i,-1);
      x=RandNum(0,SCREEN_WIDTH*2,&cloud_rng_i,-1);
      flipped=(bool)RandNum(0,1,&cloud_rng_i,-1);

      if (x<SCREEN_WIDTH) { //sprite1 cross over to sprite 2, draw on sprite 2 as well (left to right)  S1 [] S2
        if (x+cloud_l[type]>=SCREEN_WIDTH) {
          BitBlt8BitTransparent(DrawGameCloudsBackground[0].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[1].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
          x-=(SCREEN_WIDTH);
          BitBlt8BitTransparent(DrawGameCloudsBackground[0].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[1].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
        } else {
          BitBlt8BitTransparent(DrawGameCloudsBackground[0].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[1].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
        }
      } else {  //sprite2 cross over to sprite , draw on sprite 1 as well (left to right)   ] S1 S2 [
        if (x+cloud_l[type]>SCREEN_WIDTH*2) {
          x-=SCREEN_WIDTH;
          BitBlt8BitTransparent(DrawGameCloudsBackground[0].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[1].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
          x-=SCREEN_WIDTH;
          BitBlt8BitTransparent(DrawGameCloudsBackground[0].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[1].sprite_paint1, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
        } else {
          x-=SCREEN_WIDTH;
          BitBlt8BitTransparent(DrawGameCloudsBackground[0].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[0][type].sprite_cache, 0, 0, BLACK,flipped);
          BitBlt8BitTransparent(DrawGameCloudsBackground[1].sprite_paint2, x,y, cloud_l[type], cloud_w[type], DrawGameClouds[1][type].sprite_cache, 0, 0, BLACK,flipped);
        }
      }
      prelude_clouds_i++;
      loading_numerator++;
    }
 
 }
  //BitmapPalette(hdc,hdc2,GameCloudsBackground.sprite_paint,rgbColorsNoir);
}


void CloudAct()
{
  GameCloudsBackground.timer++;
  if (GameCloudsBackground.timer>15) {
//  if (GameCloudsBackground.timer>0) {
    GameCloudsBackground.timer=0;
    flag_draw_game_background_sprite=TRUE;
    GameCloudsBackground.cam_x+=1;//10;//10;
    if (GameCloudsBackground.cam_x>SCREEN_WIDTH*2)
      GameCloudsBackground.cam_x=0;
  }
}


void InitClouds()
{
  GameCloudsBackground.cam_x=0;
  GameCloudsBackground.timer=0;
}


void DrawClouds(HDC hdc, HDC hdc2)
{
  //  Sun.y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
  int _h=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3+300;
        //GR_HEIGHT/6+GR_HEIGHT/3;//GR_HEIGHT/20+GR_HEIGHT/4
  int h=SCREEN_HEIGHT/2+SCREEN_HEIGHT/4-_h;
  if (h<=0)
    h=0;

  int x1=GameCloudsBackground.cam_x;
  int x2=x1+GR_WIDTH;

  int clouds_type=0;
  if (map_weather>0)
    clouds_type=3;
  else if (map_background==0 && Sun.horizon_lvl!=-1)
    clouds_type=1;
  else if (map_background==1)
    clouds_type=1;
  
  if (x1>SCREEN_WIDTH) { //paint 2 only
    if (x2<SCREEN_WIDTH*2) {//paint 2 only
      SelectObject(hdc2,DrawGameCloudsBackground[clouds_type].sprite_paint2);
      BitBlt(hdc, 0, 0, GR_WIDTH,_h, hdc2, x1-SCREEN_WIDTH, h,  SRCPAINT);

    } else { //paint 2 loopback to paint 1
      int x_remaining=SCREEN_WIDTH*2-x1;
      SelectObject(hdc2,DrawGameCloudsBackground[clouds_type].sprite_paint2);
      BitBlt(hdc, 0, 0, x_remaining,_h, hdc2, x1-SCREEN_WIDTH, h,  SRCPAINT);

      int x_loop_back=x2-SCREEN_WIDTH*2;
      SelectObject(hdc2,DrawGameCloudsBackground[clouds_type].sprite_paint1);
      BitBlt(hdc, x_remaining, 0, x_loop_back,_h, hdc2, 0, h,  SRCPAINT);

      //GrRect(hdc,x_remaining,0,5,GR_HEIGHT,BLACK);
    }
  } else { //paint 1 only
    if (x2<SCREEN_WIDTH) { //paint 1 only
      SelectObject(hdc2,DrawGameCloudsBackground[clouds_type].sprite_paint1);
      BitBlt(hdc, 0, 0, GR_WIDTH,_h, hdc2, x1, h,  SRCPAINT);

    } else { //paint 1 crossing into paint 2
      int x_remaining=SCREEN_WIDTH-x1;
      SelectObject(hdc2,DrawGameCloudsBackground[clouds_type].sprite_paint1);
      BitBlt(hdc, 0, 0, x_remaining,_h, hdc2, x1, h,  SRCPAINT);

      int x_loop_back=x2-SCREEN_WIDTH;
      SelectObject(hdc2,DrawGameCloudsBackground[clouds_type].sprite_paint2);
      BitBlt(hdc, x_remaining, 0, x_loop_back,_h, hdc2, 0, h,  SRCPAINT);

      //GrRect(hdc,x_remaining,0,2,GR_HEIGHT,YELLOW);
    }
  }
}
//============================================================================


//==============Sun==============================
void InitSun()
{
  Sun.eclipse_type=0;
  Sun.overcast_timer=0;
  Sun.overcast_lvl=0;
  sun_rng_i=0;
  Sun.current_sun_i=0;
  Sun.rays_visible_num=0;
  Sun.flag_move_cycle=0;

  //Sun.x=GR_WIDTH/2+GR_WIDTH/4;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
  //Sun.y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;

  Sun.x=-10;
  Sun.y=GR_HEIGHT;

  Sun.pivot_x=GR_WIDTH/2;
  Sun.pivot_y=GR_HEIGHT;

  Sun.dist_l=GetDistance(Sun.x,Sun.y,Sun.pivot_x,Sun.pivot_y);

  //Sun.angle=GetCosAngle(Sun.x-Sun.pivot_x,Sun.dist_l);

  if (seconds_since_midnight>=map_sunrise_time && seconds_since_midnight<=map_sunset_time) {
    //Sun angle at sunrise + Sun angle since sunrise
    Sun.solar_angle=M_PI + M_PI*( (float)(seconds_since_midnight-map_sunrise_time)/map_sunlight_seconds);
  } else {
    //Sun angle at sunset + Sun angle since sunset, instead of map_sunlight_seconds its map_darkness seconds
    if (seconds_since_midnight>=0 && seconds_since_midnight<map_sunrise_time) { //before sunrise, 00 00 -> sunrise time
      Sun.solar_angle=2*M_PI + M_PI_2*( (float)(60*60*24-map_sunset_time)/(map_darkness_seconds/2)) + M_PI_2*( (float)(seconds_since_midnight)/(map_darkness_seconds/2));
    } else { //after sunset
      Sun.solar_angle=2*M_PI + M_PI_2*( (float)(seconds_since_midnight-map_sunset_time)/(map_darkness_seconds/2));
    }
  }

  //Sun.angle=Sun.solar_angle; //Done by SunAct();
  Sun.x=Sun.pivot_x+Sun.dist_l*cos(Sun.angle);
  Sun.y=Sun.pivot_y+Sun.dist_l*sin(Sun.angle);

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
    Sun.ray_x[i]=Sun.x+(SUN_SIZE+2)*cos(Sun.ray_angle[i]);
    Sun.ray_y[i]=Sun.y+(SUN_SIZE+2)*sin(Sun.ray_angle[i]);
    Sun.ray_is_blocked[i]=FALSE;
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



bool IsPixelColorAt16(BYTE* pixels, int width, int height, int x, int y, WORD targetColor) {
    if (!pixels || x < 0 || y < 0 || x >= width || y >= height) {
        return false;
    }
    // Calculate the offset: 2 bytes per pixel
    int offset = (y * width + x) * 2;

    // Read the 16-bit pixel value
    WORD pixelColor = *(WORD*)(pixels + offset);

    //printf("color:%d,target:%d\n",pixelColor,targetColor);
    return pixelColor == targetColor;
}

//bool IsPixelColorAt(HDC hdc, HBITMAP hBitmap, int x, int y, COLORREF targetColor) {
   // if (!hdc || !hBitmap || x < 0 || y < 0) {
     //   return false;
    //}

    // Create a compatible DC and select the bitmap into it
    //HDC memDC = CreateCompatibleDC(hdc);
    //if (!memDC) {
      //  return false;
    //}

    //HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    // Get the pixel color
    //SelectObject(hdc,hBitmap);
    //COLORREF pixelColor = GetPixel(hdc, x, y);

    // Restore the original bitmap and clean up
    //SelectObject(memDC, oldBitmap);
    //DeleteDC(memDC);

    // Compare the pixel color
    //return pixelColor == targetColor;
//}
#define TWILIGHT_ANGLE_CIVIL     deg2rad(6)
#define TWILIGHT_ANGLE_NAUTICAL     deg2rad(12)
#define TWILIGHT_ANGLE_ASTRONOMICAL     deg2rad(18)



void DrawSunRays(HDC hdc,HDC hdc2)
{

  for (int k=0;k<8;k++) {
    if (global_screen_bits==32) {
      switch (Sun.eclipse_type) {
        case 0:
          Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], custom_map_background_color);
          break;
        case 1:
        case 3:
          Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], custom_map_background_dkcolor);
          break;
        case 2:
        case 4:
          Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], BLACK);
          break;
      }
      if (!(Sun.ray_x[Sun.current_sun_ray_i]<=GR_WIDTH && Sun.ray_x[Sun.current_sun_ray_i]>=0 && Sun.ray_y[Sun.current_sun_ray_i]<GR_HEIGHT && Sun.ray_y[Sun.current_sun_ray_i]>=0)) { //not within, block rays
        Sun.ray_is_blocked[Sun.current_sun_ray_i]=TRUE;
      }
    } else if (global_screen_bits==16) {
      uint16_t _color=byte_16_color_arr[rgbPaint_i[custom_map_background_color_i]]/2; //the returned value from the function below is half the true color, potential bug that needs to be resolved
      switch (Sun.eclipse_type) {
        case 1:
        case 3:
          _color=byte_16_color_arr[rgbPaint_i[custom_map_background_dkcolor_i]]/2;
          break;
        case 2:
        case 4:
          _color=byte_16_color_arr[rgbPaint_i[0]]/2;
          break;
      }
      Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt16(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], _color);
      if (!(Sun.ray_x[Sun.current_sun_ray_i]<=GR_WIDTH && Sun.ray_x[Sun.current_sun_ray_i]>=0 && Sun.ray_y[Sun.current_sun_ray_i]<GR_HEIGHT && Sun.ray_y[Sun.current_sun_ray_i]>=0)) {
        Sun.ray_is_blocked[Sun.current_sun_ray_i]=TRUE;
      }
    }


    Sun.current_sun_ray_i++;
    if (Sun.current_sun_ray_i>SUN_RAY_NUM-1) {
      Sun.current_sun_ray_i=0;
    }
  }

  for (int i=0;i<SUN_RAY_NUM;i++) {
    if (!Sun.ray_is_blocked[i]) {
        //GrLineThick(hdc,Sun.ray_x[i],Sun.ray_y[i],Sun.draw_ray_x[i],Sun.draw_ray_y[i],3,LTRYELLOW);
      if (Sun.eclipse_type<=1 || Sun.eclipse_type==3) {
        GrLine(hdc,Sun.draw_ray_x[i],Sun.draw_ray_y[i],Sun.ray_x[i],Sun.ray_y[i],LTRYELLOW);
      }  else {
        GrLine(hdc,Sun.draw_ray_x[i],Sun.draw_ray_y[i],Sun.ray_x[i],Sun.ray_y[i],WHITE);
      }
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
      if (Sun.eclipse_type==0) {
        if (Sun.overcast_lvl!=4 && Sun.rays_visible_num<=0 && Sun.y+32>=GR_HEIGHT-GR_HEIGHT/3) {
          Sun.overcast_lvl=4;
          Sun.flag_overcast=TRUE;
        } else if (Sun.overcast_lvl!=3 && Sun.rays_visible_num<=3 && Sun.y+32<GR_HEIGHT-GR_HEIGHT/3) { //darkest
          Sun.overcast_lvl=3;
          Sun.flag_overcast=TRUE;
        } else if (Sun.overcast_lvl!=2 && Sun.rays_visible_num>=8 && Sun.rays_visible_num<=12) {//darker
          Sun.overcast_lvl=2;
          Sun.flag_overcast=TRUE;
        } else if (Sun.overcast_lvl!=1 && Sun.rays_visible_num>=20 && Sun.rays_visible_num<=SUN_RAY_NUM*2-6) { //dark
          Sun.overcast_lvl=1;
          Sun.flag_overcast=TRUE;    
        } else if (Sun.overcast_lvl!=0 && Sun.rays_visible_num>SUN_RAY_NUM*2-2) { //brightest
          Sun.overcast_lvl=0;
          Sun.flag_overcast=TRUE;
        }
      } else { //Set Darker palette once during eclipse
        switch (Sun.eclipse_type) {
          case 1:
          case 3:
            if (Sun.overcast_lvl!=2) {
              Sun.overcast_lvl=2;
              Sun.flag_overcast=TRUE;
            }
            break;
          case 2:
          case 4:
            if (Sun.overcast_lvl!=4) {
              Sun.overcast_lvl=4;
              Sun.flag_overcast=TRUE;
            }
            break;
        }  
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
          case 4: //night
            for (int i=0;i<PLATFORM_GRID_NUM;i++) {
              BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsNight);
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


void DrawSun(HDC hdc,HDC hdc2)
{
  //Sun.eclipse_type=<demo>;
  switch (Sun.eclipse_type) {
    case 1: //annular
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE,LTRYELLOW,LTRYELLOW);
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE-4,custom_map_background_dkcolor,custom_map_background_dkcolor);
      break;
    case 2: //total
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE+2,WHITE,WHITE);
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE-1,BLACK,BLACK);
      break;
    case 3: //partial-annular
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE,LTRYELLOW,LTRYELLOW);
      GrCircle(hdc,Sun.x-4,Sun.y-4,SUN_SIZE-5,custom_map_background_dkcolor,custom_map_background_dkcolor);
      break;
    case 4: //partial-total
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE+2,WHITE,WHITE);
      GrCircle(hdc,Sun.x-4,Sun.y-4,SUN_SIZE-1,BLACK,BLACK);
      break;

    default:
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE,LTRYELLOW,LTRYELLOW);
      break;
  }
}


void SunAct()
{
  if (Sun.angle!=Sun.solar_angle) {
    Sun.angle=Sun.solar_angle;
    Sun.x=Sun.pivot_x+Sun.dist_l*cos(Sun.angle);
    Sun.y=Sun.pivot_y+Sun.dist_l*sin(Sun.angle);
    for (int i=0;i<STAR_NUM;i++) {
      Star.angle[i]=Sun.angle+Star.oangle[i];
    }

    //horizon lvl
    if (Sun.angle>=M_PI+TWILIGHT_ANGLE_ASTRONOMICAL && Sun.angle<=2*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL) { //none
      Sun.horizon_lvl=-1;
    //below horizon
    } else if (Sun.angle<=2*M_PI+TWILIGHT_ANGLE_CIVIL || Sun.angle>=3*M_PI-TWILIGHT_ANGLE_CIVIL) {  //Civil
      Sun.horizon_lvl=0;
    } else if (Sun.angle<=2*M_PI+TWILIGHT_ANGLE_NAUTICAL || Sun.angle>=3*M_PI-TWILIGHT_ANGLE_NAUTICAL) { //nautical
      Sun.horizon_lvl=1;
    } else if (Sun.angle<=2*M_PI+TWILIGHT_ANGLE_ASTRONOMICAL || Sun.angle>=3*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL) { //Astronomical
      Sun.horizon_lvl=2;
    } else { //Post Astronomical
      Sun.horizon_lvl=3;
    }
  }
  if (Sun.y<128) {
    Sun.y=128;
  }  
}


void SunRayAct()
{
  if (Sun.overcast_lvl==0 || Sun.eclipse_type>0) { 
    Sun.ray_l[Sun.current_sun_i]=SUN_SIZE+sun_ray_l[sun_rng_i];
  } else if (Sun.overcast_lvl==1){
    Sun.ray_l[Sun.current_sun_i]=SUN_SIZE+sun_ray_l[sun_rng_i]*3;
  } else {
    Sun.ray_l[Sun.current_sun_i]=SUN_SIZE+sun_ray_l[sun_rng_i]*5;
  }
  //RandNum(0,max(GR_WIDTH/2,GR_HEIGHT/2),&sun_rng_i,-1);
  sun_rng_i++;
  if (sun_rng_i>=9)
    sun_rng_i=0;

  Sun.draw_ray_x[Sun.current_sun_i]=Sun.x+Sun.ray_l[Sun.current_sun_i]*cos(Sun.ray_angle[Sun.current_sun_i]);
  Sun.draw_ray_y[Sun.current_sun_i]=Sun.y+Sun.ray_l[Sun.current_sun_i]*sin(Sun.ray_angle[Sun.current_sun_i]);
  Sun.ray_x[Sun.current_sun_i]=Sun.x+(SUN_SIZE+2)*cos(Sun.ray_angle[Sun.current_sun_i]);
  Sun.ray_y[Sun.current_sun_i]=Sun.y+(SUN_SIZE+2)*sin(Sun.ray_angle[Sun.current_sun_i]);

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




//==============Draw Moon====================
void InitMoon()
{
  DrawGameMoon.phase_range_x[0]=GR_WIDTH-GR_WIDTH/8;
  DrawGameMoon.phase_range_y[0]=GR_HEIGHT-GR_HEIGHT/3;

  DrawGameMoon.phase_range_x[1]=GR_WIDTH/2+GR_WIDTH/4;
  DrawGameMoon.phase_range_y[1]=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;

  DrawGameMoon.phase_range_x[2]=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8;
  DrawGameMoon.phase_range_y[2]=GR_HEIGHT/4+GR_HEIGHT/12;

  DrawGameMoon.phase_range_x[3]=GR_WIDTH/2;
  DrawGameMoon.phase_range_y[3]=GR_HEIGHT/4;

  DrawGameMoon.phase_range_x[4]=GR_WIDTH/4+GR_WIDTH/8;
  DrawGameMoon.phase_range_y[4]=GR_HEIGHT/4+GR_HEIGHT/12;

  DrawGameMoon.phase_range_x[5]=GR_WIDTH/4+GR_WIDTH/8;
  DrawGameMoon.phase_range_y[5]=GR_HEIGHT/4+GR_HEIGHT/12;

  DrawGameMoon.phase_range_x[6]=GR_WIDTH/4;
  DrawGameMoon.phase_range_y[6]=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;

  DrawGameMoon.phase_range_x[7]=GR_WIDTH/8;
  DrawGameMoon.phase_range_y[7]=GR_HEIGHT-GR_HEIGHT/3;


  for (int k=0;k<8;k++) {
    MoonAngle[k].lunar_angle=2*M_PI - M_PI/8*(k+1);
  }

  DrawGameMoon.x=-10;
  DrawGameMoon.y=GR_HEIGHT;

  DrawGameMoon.pivot_x=GR_WIDTH/2;
  DrawGameMoon.pivot_y=GR_HEIGHT;

  DrawGameMoon.dist_l=GetDistance(DrawGameMoon.x,DrawGameMoon.y,DrawGameMoon.pivot_x,DrawGameMoon.pivot_y);

  DrawGameMoon.angle=GetCosAngle(DrawGameMoon.x-DrawGameMoon.pivot_x,DrawGameMoon.dist_l);
  DrawGameMoon.flag_nightshade=TRUE;
}



void MoonAct()
{
  //DrawGameMoon.x++;
  //DrawGameMoon.y=MoonF(DrawGameMoon.x);
  //Action after moon leaves visibility
  /*if (DrawGameMoon.x>GR_WIDTH) { //reset moon position after out of view (width)
      DrawGameMoon.x=0;
      DrawGameMoon.y=GR_HEIGHT;

      DrawGameMoon.pivot_x=GR_WIDTH/2;
      DrawGameMoon.pivot_y=GR_HEIGHT;

      DrawGameMoon.angle=GetCosAngle(DrawGameMoon.x-DrawGameMoon.pivot_x,DrawGameMoon.dist_l);    
      for (int i=0;i<STAR_NUM;i++) {
        Star.angle[i]=Star.oangle[i];
      }

      current_moon_phase_id++;
      if (current_moon_phase_id>=8)
        current_moon_phase_id=0;
  }*/
  int cmp_id=current_moon_phase_id;
  if (cmp_id==7)
    cmp_id=3;

  if (cmp_id!=-1) {
    if (DrawGameMoon.angle<MoonAngle[cmp_id].lunar_angle && DrawGameMoon.x+64<GR_WIDTH-48) {
      DrawGameMoon.angle+=0.025;//0.005;
      for (int i=0;i<STAR_NUM;i++) {
        Star.angle[i]+=0.0125;//0.0025;
      }
    } /*else { //continuously draw moon and stars moving slowly
      DrawGameMoon.angle+=0.025/1000;
      for (int i=0;i<STAR_NUM;i++) {
        Star.angle[i]+=0.0125/1000;
      }
    }*/
  } else if (DrawGameMoon.angle<M_PI+M_PI_2) {
    DrawGameMoon.angle+=0.025;//0.005;    
    for (int i=0;i<STAR_NUM;i++) {
      Star.angle[i]+=0.0125;//0.0025;
    }
  }

  //if (DrawGameMoon.x+64>GR_WIDTH-48)
    //DrawGameMoon.x=GR_WIDTH-48;
  DrawGameMoon.x=DrawGameMoon.pivot_x+DrawGameMoon.dist_l*cos(DrawGameMoon.angle);
  DrawGameMoon.y=DrawGameMoon.pivot_y+DrawGameMoon.dist_l*sin(DrawGameMoon.angle);
  if (DrawGameMoon.y<128) {
    DrawGameMoon.y=128;
  }

  if (DrawGameMoon.x>DrawGameMoon.phase_range_x[0]) { //new-cresent moon, rightside
    DrawGameMoon.current_angle_id=0;
  } else if (DrawGameMoon.x<DrawGameMoon.phase_range_x[7]) { //old-cresent moon, leftside
    DrawGameMoon.current_angle_id=6;    
  } else {
    if (DrawGameMoon.x>=GR_WIDTH/4+GR_WIDTH/8&& DrawGameMoon.x<=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8) {
      DrawGameMoon.current_angle_id=3;
    } else {
      for (int k=1;k<6;k++) { //Decending
        if (DrawGameMoon.x<=DrawGameMoon.phase_range_x[k] && DrawGameMoon.x>=DrawGameMoon.phase_range_x[k+1]) {
          DrawGameMoon.current_angle_id=k;
          break;
        }
      }
    }
  }
}


/*
  Draws -Background-background, runs once on start unless explicitly called.
        -it is a large bitmap
  https://www.astronomy.com/observing/twilights-glow-is-calling/

  https://earthsky.org/earth/twilight-2/

  (O)    > +18deg ->  no gradient



          (O)   +18deg -> +12deg   Low gradient



                  (O)   +12deg -> +6deg   Low gradient



                          (O)   +6deg -> 0deg     Middle gradient



                                 (O)   0 deg       Middle Gradient           CROSSING THE HORIZON
_________________________________



                          0deg -> -6deg     High gradient               CIVIL TWILIGHT



                  -6deg -> -12deg     middle gradient           NAUTICAL TWILIGHT



          -12deg -> -18deg   low gradient               ASTRONOMICAL TWILIGHT



  <-18deg -> no gradient, night sky color

*/



//Sun at runrise = PI,
//Sun at sunset = 2*PI


void DrawGameBackgroundSpriteII(HDC hdc1,HDC hdc2)
{ //runs on level start or screen res change, more infrequent.
  HDC hdcBG=CreateCompatibleDC(hdc1);
  //Black Layer
  SelectObject(hdcBG,bg_black_layer);
  GrRect(hdcBG,0,0,GR_WIDTH+4,GR_HEIGHT+4,BLACK);

  //sky with gradient
  //draw gradient to glass layer
  SelectObject(hdcBG,bg_glass_layer);
  if (!(Sun.solar_angle>=M_PI+TWILIGHT_ANGLE_ASTRONOMICAL && Sun.solar_angle<=2*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL)) { //Sky dawn/dusk/twilight
    if ((Sun.solar_angle>=2*M_PI-TWILIGHT_ANGLE_CIVIL && Sun.solar_angle<=2*M_PI+TWILIGHT_ANGLE_ASTRONOMICAL) || 
        (Sun.solar_angle>=3*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL) || 
        (Sun.solar_angle<=M_PI+TWILIGHT_ANGLE_CIVIL)) //Below +civil
      SelectObject(hdc2,red_black_gradient_bg_sprite);
    else
      SelectObject(hdc2,yellow_black_gradient_bg_sprite);
  } else { //Sky Day
    SelectObject(hdc2,white_gradient_bg_sprite);
  }
  StretchBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,800,600,SRCCOPY); //draw gradient to glass layer

  //draw clear sky
  SelectObject(hdc2,game_background_deco_sprite);
  if (!(Sun.solar_angle>2*M_PI+TWILIGHT_ANGLE_NAUTICAL && Sun.solar_angle<3*M_PI-TWILIGHT_ANGLE_NAUTICAL)) { //above horizon and between sunlight hours
    switch (Sun.eclipse_type) {
    case 0:
    default:
      GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,custom_map_background_color);
      break;
    case 1: //Annular Eclipse
    case 3:
      GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,custom_map_background_dkcolor);
      break;
    case 2: //Total/Full Eclipse
    case 4:
      GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,BLACK);
      break;
    }
  } else { //Sunrise/Sunsset
    if (Sun.x>GR_WIDTH/2) //rising
      GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,RGB(0,0,24));
    else //setting
      GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,BLACK);
  }

  //overlay gradient on glass layer over clear sky
  if (Sun.eclipse_type==0) {
    if (
      !(Sun.solar_angle>=M_PI+TWILIGHT_ANGLE_ASTRONOMICAL && Sun.solar_angle<=2*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL)
    ) { //dawn/dusk gradient
      int alpha=0;
      switch (Sun.horizon_lvl) {
        case -1:
        default:
          alpha=-1;
          break;
        case 0:
          alpha=136;
          break;
        case 1:
          alpha=136;
          break;
        case 2:
          alpha=136;
          break;
        case 3:
          alpha=136;
          break;
      }
      if (alpha>0) {
        int the=-GR_HEIGHT+Sun.y+SUN_SIZE*8;

        GrGlassTexture(hdc2, game_background_deco_sprite,0,0,GR_WIDTH,the-SUN_SIZE*2,hdcBG, bg_black_layer,0,0,GR_WIDTH,the, alpha/2);
        GrGlassTexture(hdc2, game_background_deco_sprite,0,the,GR_WIDTH,GR_HEIGHT,hdcBG, bg_glass_layer,0,0,GR_WIDTH,GR_HEIGHT, alpha/2);

        GrGlassTexture(hdc2, game_background_deco_sprite,0,0,GR_WIDTH,the,hdcBG, bg_black_layer,0,0,GR_WIDTH,the, alpha/2);
        GrGlassTexture(hdc2, game_background_deco_sprite,0,the-SUN_SIZE*2,GR_WIDTH,GR_HEIGHT,hdcBG, bg_glass_layer,0,0,GR_WIDTH,GR_HEIGHT, alpha/2);
      }
    } else { //day gradient
      int _h=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;
      if (Sun.y+64<_h) { //day gradient moving downwards
        GrGlassTexture(hdc2, game_background_deco_sprite,0,_h,GR_WIDTH,GR_HEIGHT-_h,
                      hdcBG, bg_glass_layer,             0,0,        GR_WIDTH,GR_HEIGHT, 64);
      } else {
        GrGlassTexture(hdc2, game_background_deco_sprite,0,Sun.y+64,GR_WIDTH,GR_HEIGHT-_h,
                      hdcBG, bg_glass_layer,             0,0,        GR_WIDTH,GR_HEIGHT, 64);
      }
    }
  }
  DeleteDC(hdcBG);
}

void DrawGameBackgroundSprite(HDC hdcMain,HDC hdc2)
{ //runs on call
  HDC hdcBG=CreateCompatibleDC(hdcMain);

  //Draw decorated background (like a rect) to official game background, 16/32-bits
  SelectObject(hdcBG,game_background_sprite);
  SelectObject(hdc2,game_background_deco_sprite);
  BitBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,SRCCOPY);

  //Draw Stars
  if (Sun.solar_angle>=M_PI+TWILIGHT_ANGLE_CIVIL && Sun.solar_angle<=2*M_PI-TWILIGHT_ANGLE_CIVIL) {
    switch (Sun.eclipse_type) {
      /*case 0:
      default:
        if (Sun.y>GR_HEIGHT-GR_HEIGHT/3)
          DrawStars(hdcBG);
        break;*/
      case 1: //Annular Eclipse
      case 3:
          DrawStars(hdcBG);
        break;
      case 2: //Total/Full Eclipse
      case 4:
        DrawStars(hdcBG);
        break;
      }
  } else { //dawn/dusk
    DrawStars(hdcBG);
  }

  //Draw Day Moon
  int day_moon_phase_id=0;
  int day_moon_angle_id=-1;
  if (!(lunar_day>=13 && lunar_day<=15)) {
    is_blood_moon=FALSE;
  }
  if (lunar_day>=1 && lunar_day<=9) {
    if (lunar_month<12) {
      if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
        day_moon_phase_id=0;
      } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
        day_moon_phase_id=1;
      }
    } else { //month 12 lunar
      if (lunar_day>=1 && lunar_day<=4) { //1, 2, 3, 4
        day_moon_phase_id=0;
      } else {// 5, 6, 7, --> 8 ,9
        day_moon_phase_id=1;
      }
    }
  }else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
    day_moon_phase_id=2;
  } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
    day_moon_phase_id=3;
    if (is_blood_moon) {
      day_moon_phase_id=7;
    }
  } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
    day_moon_phase_id=4;
  } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
    day_moon_phase_id=5;
  } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
    day_moon_phase_id=6;
  } else { // New Moon, no stars
    day_moon_phase_id-1;
  }


  if (lunar_day>=1 && lunar_day<=26 && day_moon_phase_id>=0 && day_moon_phase_id<=7) {
    DrawGameMoon.phase_range_x[0]=GR_WIDTH-GR_WIDTH/8;
    DrawGameMoon.phase_range_y[0]=GR_HEIGHT-GR_HEIGHT/3;

    DrawGameMoon.phase_range_x[1]=GR_WIDTH/2+GR_WIDTH/4;
    DrawGameMoon.phase_range_y[1]=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;

    DrawGameMoon.phase_range_x[2]=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8;
    DrawGameMoon.phase_range_y[2]=GR_HEIGHT/4+GR_HEIGHT/12;

    DrawGameMoon.phase_range_x[3]=GR_WIDTH/2;
    DrawGameMoon.phase_range_y[3]=GR_HEIGHT/4;

    DrawGameMoon.phase_range_x[4]=GR_WIDTH/4+GR_WIDTH/8;
    DrawGameMoon.phase_range_y[4]=GR_HEIGHT/4+GR_HEIGHT/12;

    DrawGameMoon.phase_range_x[5]=GR_WIDTH/4+GR_WIDTH/8;
    DrawGameMoon.phase_range_y[5]=GR_HEIGHT/4+GR_HEIGHT/12;

    DrawGameMoon.phase_range_x[6]=GR_WIDTH/4;
    DrawGameMoon.phase_range_y[6]=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;

    DrawGameMoon.phase_range_x[7]=GR_WIDTH/8;
    DrawGameMoon.phase_range_y[7]=GR_HEIGHT-GR_HEIGHT/3;

    float day_moon_angle=Sun.angle-(2*M_PI*lunar_day/27);
    if (lunar_day<=3) {
      day_moon_angle=Sun.angle-(2*M_PI*3/27);
    } else if (lunar_day>=24)
      day_moon_angle=Sun.angle-(2*M_PI*24/27);
    float day_moon_x=Sun.pivot_x+Sun.dist_l*cos(day_moon_angle);
    float day_moon_y=Sun.pivot_y+Sun.dist_l*sin(day_moon_angle);

    if (day_moon_x>DrawGameMoon.phase_range_x[0]) { //new-cresent moon, rightside
      day_moon_angle_id=0;
    } else if (day_moon_x<DrawGameMoon.phase_range_x[7]) { //old-cresent moon, leftside
      day_moon_angle_id=6;    
    } else {
      if (day_moon_x>=GR_WIDTH/4+GR_WIDTH/8 && day_moon_x<=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8) {
        day_moon_angle_id=3;
      } else {
        for (int k=1;k<6;k++) { //Decending
          if (day_moon_x<=DrawGameMoon.phase_range_x[k] && day_moon_x>=DrawGameMoon.phase_range_x[k+1]) {
            day_moon_angle_id=k;
            break;
          }
        }
        if (day_moon_angle_id==-1) {
          if (day_moon_x<GR_WIDTH/2) {
            day_moon_angle_id=5;
          } else {
            day_moon_angle_id=1;
          }
        }
      }
    }

    //Back of moon during sunset
    /*if (Sun.y>=GR_HEIGHT+GR_HEIGHT/7) {
      if (Sun.x>GR_WIDTH/2) { //sun-set
        GrCircle(hdcBG,day_moon_x,day_moon_y,28,RGB(0,0,24),RGB(0,0,24));
      } else {
        GrCircle(hdcBG,day_moon_x,day_moon_y,28,custom_map_background_color,custom_map_background_color);
      }
    }*/

    if (day_moon_angle_id!=-1) {
      BITMAP bm;
      GetObject(Moon[day_moon_phase_id].draw_moon_sprite[day_moon_angle_id].sprite_paint,sizeof(BITMAP),&bm);
      int size=bm.bmWidth;
      SelectObject(hdc2,Moon[day_moon_phase_id].draw_moon_sprite[day_moon_angle_id].sprite_paint);
      BitBlt(hdcBG,day_moon_x-size/2,day_moon_y-size/2,size,size,hdc2,0,0,SRCPAINT);
    }

  }
  //End of Draw Day Moon


  //Draw sun and clouds  
  if (map_weather==0 && map_background==0)
    DrawSun(hdcBG,hdc2);

  //Draw Clouds
  DrawClouds(hdcBG,hdc2);

  DeleteDC(hdcBG);
}


void DrawMoon(HDC hdc,HDC hdc2)
{
  if (current_moon_phase_id>=0 && current_moon_phase_id<=7) {
    DrawSprite(hdc, hdc2,DrawGameMoon.x,DrawGameMoon.y,&Moon[current_moon_phase_id].draw_moon_sprite[DrawGameMoon.current_angle_id],FALSE);
  }

  //init palette nighttime
  if (DrawGameMoon.flag_nightshade && level_loaded) {
    for (int i=0;i<PLATFORM_GRID_NUM;i++) {
      BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsNight);
    }
    DrawGameMoon.flag_nightshade=FALSE;
  }
}
//===========================================
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

  int dbg_height=GR_HEIGHT;
  if (GR_HEIGHT>SCREEN_HEIGHT-1);
    dbg_height=SCREEN_HEIGHT-1;
  switch (map_background) {
    case 0: //day and night
      if (flag_draw_game_background_spriteII) {
        DrawGameBackgroundSpriteII(hdc,hdc2);
        flag_draw_game_background_spriteII=FALSE;
      }
      if (flag_draw_game_background_sprite) { //Refresh background, runs when timer hits 15, delayed for speed        
        //DrawGameBackgroundSpriteII(hdc,hdc2); //debug only
        DrawGameBackgroundSprite(hdc,hdc2);
        flag_draw_game_background_sprite=FALSE;
      }
      FastDrawTexturedRect(publicScreenPixels,SCREEN_WIDTH,publicBackgroundPixels,GR_WIDTH,dbg_height,global_screen_bits);

      break;
    case 1: //night
     // GrRect(hdc,0,0,GR_WIDTH+24,GR_HEIGHT+24,custom_map_background_color);
      if (flag_draw_game_background_spriteII) {
        DrawGameBackgroundSpriteII(hdc,hdc2);
        flag_draw_game_background_spriteII=FALSE;
      }
      if (flag_draw_game_background_sprite) { //Refresh background, runs when timer hits 15, delayed for speed
        DrawGameBackgroundSprite(hdc,hdc2);
        flag_draw_game_background_sprite=FALSE;
      }
      FastDrawTexturedRect(publicScreenPixels,SCREEN_WIDTH,publicBackgroundPixels,GR_WIDTH,dbg_height,global_screen_bits);

      //DrawStars(hdc);
      /*char printme[32];
      sprintf(printme,"lunar_angle:%5.4f",MoonAngle[current_moon_phase_id].lunar_angle);
      GrPrint(hdc,200,200,printme,WHITE);
      sprintf(printme,"MoonAngle:%5.4f",DrawGameMoon.angle);
      GrPrint(hdc,200,232,printme,WHITE);*/
      break;

    case 2:
    case 3: //default backgrounds
      GrRect(hdc,0,0,GR_WIDTH+24,GR_HEIGHT+24,custom_map_background_color);
      //if (map_background_sprite!=NULL) {
        //DrawBitmap(hdc,hdc2,draw_p_px,draw_p_py,0,0,GR_WIDTH*2,GR_HEIGHT*2,map_background_sprite,SRCCOPY,FALSE,FALSE);
      //}
      break;
    default: //custom map background
      GrRect(hdc,0,0,GR_WIDTH+24,GR_HEIGHT+24,custom_map_background_color);
      //if (map_background_sprite!=NULL) {
        //DrawBitmap(hdc,hdc2,draw_p_px,draw_p_py,0,0,GR_WIDTH*2,GR_HEIGHT*2,map_background_sprite,SRCCOPY,FALSE,FALSE);
      //}
      break;
  }


  //Draw shooting stars
  if ((map_background==0 && Sun.y>=GR_HEIGHT+GR_HEIGHT/7) || map_background==1) {
    for (int i=0;i<SSTAR_NUM;i++) {
      if (SStar[i].lifetime>0) {
        GrCircle(hdc,SStar[i].x-GR_WIDTH/16*_ppx,SStar[i].y-GR_HEIGHT/16*_ppy,1,WHITE,WHITE);
      }
    }
  }


  //draw night moon
   if (
        ((!in_main_menu || (in_main_menu && map_weather==0 && !(map_sunrise_time<=seconds_since_midnight && seconds_since_midnight<=map_sunset_time))) &&
         ((
           !(map_background>=2 && map_background<=3) && 
           is_moon && 
           custom_map_background_color_i>127
          ) || map_background==1
        ))        
    ) {
      DrawMoon(hdc,hdc2);
  }
}
//==============================================================================

