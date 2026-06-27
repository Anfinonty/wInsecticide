


//=================Moon============================

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
  //Twinkling Stars
  for (int i=0;i<STAR_NUM;i++) {
    Star.timer[i]--;
    if (Star.timer[i]<1) {
      Star.size[i]=RandNum(0,4,&star_rng_i,-1);
      //Star.timer[i]=RandNum(50,350,&star_rng_i,-1)*10;
      Star.timer[i]=RandNum(1,35,&star_rng_i,-1);
    } 

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
  if (lvl_map_background.is_stars) {
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
    switch (Sun.eclipse_type) { //stars if total eclipse
      case 2:
      case 4:
        scnt=1;
        break;
    }
    for (int i=0;i<STAR_NUM;i+=scnt) {
      int _x = Star.x[i];//(Star.x[i] * GR_WIDTH)/MAX_STAR_X;
      int _y = Star.y[i];//(Star.y[i] * GR_HEIGHT)/MAX_STAR_Y;
      if (Star.size[i]>0 && Star.is_draw[i]) {
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
        GrRect(hdc,0,0,SCREEN_WIDTH+1,SCREEN_HEIGHT/2+SCREEN_HEIGHT/4+1,YELLOW);
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
      type=RandNum(0,LOADED_CLOUD_NUM-1,&cloud_rng_i,-1);
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
    } else { // normal cloud amount
      if (SCREEN_WIDTH<960) { //large clouds are rarer on smaller width screens
        if (prelude_clouds_i<2
            || (DRAW_CLOUDS_NUM-3<prelude_clouds_i && prelude_clouds_i<DRAW_CLOUDS_NUM)) {
          //spawn big cloud
          type=RandNum(0,4,&cloud_rng_i,-1);
        } else { //spawn small cloud
          if (prelude_clouds_i!=12) {
            type=RandNum(5,LOADED_CLOUD_NUM-1,&cloud_rng_i,-1);
          } else {
            type=RandNum(8,8,&cloud_rng_i,-1);
          }
        }
      } else {
        type=RandNum(0,LOADED_CLOUD_NUM-1,&cloud_rng_i,-1);
      }
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
  if (GameCloudsBackground.timer>120) { //slower speed boosts fps
//  if (GameCloudsBackground.timer>15) { //normal speed
//  if (GameCloudsBackground.timer>0) {
    GameCloudsBackground.timer=0;
    flag_draw_game_background_sprite=TRUE;
    GameCloudsBackground.cam_x+=1;//10;//10;
    if (GameCloudsBackground.cam_x>SCREEN_WIDTH*2)
      GameCloudsBackground.cam_x=0;
  }

  if (lvl_map_background.background_id==4) {
    bg_windmillhand_timer++;
    if (bg_windmillhand_timer>15) {
  //  if (bg_windmillhand_timer>120) {
      bg_windmillhand_id++;
      if (bg_windmillhand_id>3) {
        bg_windmillhand_id=0;
      }
      bg_windmillhand_timer=0;
    }
  }
}


void InitWindmillHand(int i,int j,int x1,int y1, int x2, int y2)
{
  bg_windmillhand[i].ox1[j]=x1;
  bg_windmillhand[i].oy1[j]=y1;
  bg_windmillhand[i].ox2[j]=x2;
  bg_windmillhand[i].oy2[j]=y2;
}

void InitWindmillHandScale(int i,int j)
{
  bg_windmillhand[i].x1[j]=(float)(bg_windmillhand[i].ox1[j])*GR_WIDTH/800;
  bg_windmillhand[i].y1[j]=(float)(bg_windmillhand[i].oy1[j])*GR_HEIGHT/480;
  bg_windmillhand[i].x2[j]=(float)(bg_windmillhand[i].ox2[j])*GR_WIDTH/800;
  bg_windmillhand[i].y2[j]=(float)(bg_windmillhand[i].oy2[j])*GR_HEIGHT/480;
}


void InitWindmillHands()
{
  InitWindmillHand(0,0,239,275,257,299);
  InitWindmillHand(0,1,236,326,252,308);
  InitWindmillHand(0,2,262,297,279,277);
  //InitWindmillHand(0,3,268,317,276,328);
  InitWindmillHand(0,3,261,305,276,328);

  InitWindmillHand(0,4,346,304+3,352+1,322+1);
  InitWindmillHand(0,5,340,335,351,325);
  InitWindmillHand(0,6,357,321,366,312);
  InitWindmillHand(0,7,356,324,360+4,333+8+2);

  InitWindmillHand(0,8,400,306,414,318);
  InitWindmillHand(0,9,418,315,425,296);
  InitWindmillHand(0,10,405,341,412,327);
  InitWindmillHand(0,11,419,320,431,332);

//
  InitWindmillHand(1,0,245,271,257,300);
  InitWindmillHand(1,1,232,320,254,305);
  InitWindmillHand(1,2,264,300,282,287);
  InitWindmillHand(1,3,260,306,271,338);

  //InitWindmillHand(1,4,347+2,305+2,352+2,325+2);
  //InitWindmillHand(1,5,338+2,335+2,352+2,327+2);
  //InitWindmillHand(1,6,356+2,322+2,366+2,317+2);
  //InitWindmillHand(1,7,355+2,326+2,359+2,337+4+2);

  InitWindmillHand(1,4,351,305,354,323);
  InitWindmillHand(1,5,356,323,369,319);
  InitWindmillHand(1,6,336,330,352,326);
  InitWindmillHand(1,7,355,326,359,341);

  InitWindmillHand(1,8,401,305,415,319);
  InitWindmillHand(1,9,417,318,427,300);
  InitWindmillHand(1,10,402,343-5,412,326);
  InitWindmillHand(1,11,418,322,428,336);

//
  InitWindmillHand(2,0,258,268,258,299);
  InitWindmillHand(2,1,229,305,254,305);
  InitWindmillHand(2,2,265,306,284,306);
  InitWindmillHand(2,3,261,310,261,338);

  InitWindmillHand(2,4,338+1,323+1,351+1,327+1);
  InitWindmillHand(2,5,353+1,323+1,356+1,305+1+1);
  InitWindmillHand(2,6,352+1,330+1,350+1,343+1);
  InitWindmillHand(2,7,355+1,326+1,367+1,328+1); 

  InitWindmillHand(2,8,397,333,414,322);
  InitWindmillHand(2,9,409,297,415,319);
  InitWindmillHand(2,10,420,319,433,311);
  InitWindmillHand(2,11,418,323,425,340);

//
  InitWindmillHand(3,0,229,299,257,302);
  InitWindmillHand(3,1,262,301,270,269);
  InitWindmillHand(3,2,263,305,284,309);
  InitWindmillHand(3,3,254,331,259,306);

  InitWindmillHand(3,4,338+3,319-5,352,323);
  InitWindmillHand(3,5,355,323,360+1,309-2);
  InitWindmillHand(3,6,355,326,366,335);
  InitWindmillHand(3,7,346,344,353,327);

  //InitWindmillHand(3,8,396+5,328,414,322);
  //InitWindmillHand(3,9,414,297,417,317+5);
  //InitWindmillHand(3,10,421,320-5,435,315);
  //InitWindmillHand(3,11,418,324,418,343);

  InitWindmillHand(3,8,417,296,418,319);
  InitWindmillHand(3,9,421,322,434,323);
  InitWindmillHand(3,10,398,321,415,322);
  InitWindmillHand(3,11,418,324,418,341);

}

void InitWindmillHandsScale()
{
  bg_windmillhand_id=0;
  bg_windmillhand_timer=0;
  for (int i=0;i<4;i++) {
    for (int j=0;j<12;j++) {
      InitWindmillHandScale(i,j);
    }
  }
}


void InitClouds()
{
  GameCloudsBackground.cam_x=0;
  GameCloudsBackground.timer=0;

  InitWindmillHands();
  InitWindmillHandsScale();
}


void DrawClouds(HDC hdc, HDC hdc2)
{
  //  Sun.y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;
  int _h=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3+300;/*-GR_HEIGHT/12;*/
        //GR_HEIGHT/6+GR_HEIGHT/3;//GR_HEIGHT/20+GR_HEIGHT/4
  int h=SCREEN_HEIGHT/2+SCREEN_HEIGHT/4-_h;
  if (h<=0)
    h=0;

  int x1=GameCloudsBackground.cam_x;
  int x2=x1+GR_WIDTH;

  int clouds_type=0;

  if (lvl_map_background.is_real_time) {
    if (lvl_map_background.weather_type>0)
      clouds_type=3;
    else if (!(Sun.horizon_lvl>=-1 && Sun.horizon_lvl<=0))
      clouds_type=1;
  } else {
    clouds_type=lvl_map_background.clouds_type;
  }

  if (clouds_type>=0 && clouds_type<=3) { //only draw valid clouds
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


  //draw background infront of skies (foreground portion) <---===FOREGROUND OF CLOUD==
/*  int _sh=GR_HEIGHT/3;
  if (_sh<350) _sh=350;
  int _y=_h-_sh;

  if (_y<0) _y=0;

  SelectObject(hdc2,draw_background_sprite_stretched_mask);
  BitBlt(hdc,0,_y,GR_WIDTH,_sh,hdc2,0,_y,SRCAND);

  SelectObject(hdc2,draw_background_sprite_stretched_paint);
  BitBlt(hdc,0,_y,GR_WIDTH,_sh,hdc2,0,_y,SRCPAINT);
*/

  //Static foreground infront of clouds
  if (lvl_map_background.background_id>0 && lvl_map_background.background_id<=BACKGROUND_FOREGROUND_SPRITE_NUM) {
    SelectObject(hdc2,draw_background_sprite_stretched_mask);
    BitBlt(hdc,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,SRCAND);

    SelectObject(hdc2,draw_background_sprite_stretched_paint);
    BitBlt(hdc,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,SRCPAINT);
  }
}
//============================================================================


//==============Sun==============================
void InitSun()
{
  //Eclipse Type
  if (!lvl_map_background.is_real_time) {
    if (lvl_map_background.eclipse_type<5) {
      Sun.eclipse_type=lvl_map_background.eclipse_type;
    } else {
      is_blood_moon=TRUE;
      Sun.eclipse_type=0;
    }
  }

  Sun.overcast_timer=0;
  Sun.overcast_lvl=0;
  sun_rng_i=0;
  Sun.current_sun_i=0;
  Sun.rays_visible_num=0;
  Sun.flag_move_cycle=0;

  //Position Stars Properly
  Sun.angle=-999999;

  //Sun.x=GR_WIDTH/2+GR_WIDTH/4;//-GR_WIDTH/16*_ppx;//-mouse_x/50;
  //Sun.y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3;//-GR_HEIGHT/16*_ppy;//-mouse_y/50;

  Sun.x=-10;
  Sun.y=GR_HEIGHT;

  Sun.pivot_x=GR_WIDTH/2;
  Sun.pivot_y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/5;

  //Sun.dist_l=GetDistance(Sun.x,Sun.y,Sun.pivot_x,Sun.pivot_y);
  Sun.dist_l=(float)(GR_WIDTH/2+10);
  if (map_sunlight_seconds<=12*60*60) {
    Sun.dist_l*=(float)(map_sunlight_seconds/(19.0*60*60));   //Exaggerate distance from center
  }
  if (Sun.dist_l>GR_WIDTH/2+10)
    Sun.dist_l=GR_WIDTH/2+10;

  //Sun.angle=GetCosAngle(Sun.x-Sun.pivot_x,Sun.dist_l);
  if (seconds_since_midnight>=map_sunrise_time && seconds_since_midnight<=map_sunset_time) {
    //Sun angle at sunrise + Sun angle since sunrise
    Sun.solar_angle=M_PI + M_PI*( (float)(seconds_since_midnight-map_sunrise_time)/map_sunlight_seconds);
  } else {
    //Sun angle at sunset + Sun angle since sunset, instead of map_sunlight_seconds its map_darkness seconds
    if (seconds_since_midnight>=0 && seconds_since_midnight<map_sunrise_time) { //before sunrise, midnight -> sunrise time
      //sometimes the lunar hijri moves a day ahead during midnight causing a shift scratch-back
      Sun.solar_angle=2*M_PI + M_PI*( (float)((60*60*24-map_sunset_time)+seconds_since_midnight)/(map_darkness_seconds));
    } else { //after sunset, before midnight
      Sun.solar_angle=2*M_PI + M_PI*( (float)(seconds_since_midnight-map_sunset_time)/(map_darkness_seconds));
    }
  }

  //x-axis if inverted
  if (lvl_map_background.latitude<0)
    Sun.x=Sun.pivot_x-Sun.dist_l*cos(Sun.solar_angle);
  else
    Sun.x=Sun.pivot_x+Sun.dist_l*cos(Sun.solar_angle);

  //Sun.angle=Sun.solar_angle; //Done by SunAct();
  Sun.y=Sun.pivot_y+Sun.dist_l*sin(Sun.solar_angle);



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
          Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], lvl_map_background.day_sky_color);
          break;
        case 1:
        case 3:
          Sun.ray_is_blocked[Sun.current_sun_ray_i]=!IsPixelColorAt(publicScreenPixels, SCREEN_WIDTH,SCREEN_HEIGHT, Sun.ray_x[Sun.current_sun_ray_i], Sun.ray_y[Sun.current_sun_ray_i], lvl_map_background.day_sky_dkcolor);
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
      uint16_t _color=byte_16_color_arr[rgbPaint_i[lvl_map_background.day_sky_color_i]]/2; //the returned value from the function below is half the true color, potential bug that needs to be resolved
      switch (Sun.eclipse_type) {
        case 1:
        case 3:
          _color=byte_16_color_arr[rgbPaint_i[lvl_map_background.night_sky_color_i]]/2;
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
        if (Sun.overcast_lvl!=4 && Sun.rays_visible_num<=0 && Sun.horizon_lvl>=2) { //nighttime
          Sun.overcast_lvl=4;
          Sun.flag_overcast=TRUE;
        } else if (Sun.overcast_lvl!=3 && ((Sun.rays_visible_num<=3 && Sun.horizon_lvl==-1) || (Sun.horizon_lvl==1))) { //darkest
          Sun.overcast_lvl=3;
          Sun.flag_overcast=TRUE;
        } else if (Sun.overcast_lvl!=2 && 
            ((Sun.rays_visible_num>=8 && Sun.rays_visible_num<=12 && Sun.horizon_lvl==-1) || (Sun.horizon_lvl==0))
        ) {//darker
          Sun.overcast_lvl=2;
          Sun.flag_overcast=TRUE;
        } else if (Sun.overcast_lvl!=1 && Sun.rays_visible_num>=20 && Sun.rays_visible_num<=SUN_RAY_NUM*2-6 && Sun.horizon_lvl==-1) { //dark
          Sun.overcast_lvl=1;
          Sun.flag_overcast=TRUE;    
        } else if (Sun.overcast_lvl!=0 && Sun.rays_visible_num>SUN_RAY_NUM*2-2 && Sun.horizon_lvl==-1) { //brightest
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
        if (lvl_map_background.is_sun || lvl_map_background.brightness_type==0) { //dynamic lightning
          if (Sun.overcast_lvl>=0 && Sun.overcast_lvl<=4) {
            for (int i=0;i<PLATFORM_GRID_NUM;i++) {
              BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsBrightness[Sun.overcast_lvl]);
            }
            //BitmapPalette(hdc,hdc2,draw_background_sprite_stretched_paint,rgbColorsBrightness[Sun.overcast_lvl]);
          } else {
            for (int i=0;i<PLATFORM_GRID_NUM;i++) {
              BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDefault);
            }
            //BitmapPalette(hdc,hdc2,draw_background_sprite_stretched_paint,rgbColorsDefault);
          }
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
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE-4,lvl_map_background.day_sky_dkcolor,lvl_map_background.day_sky_dkcolor);
      break;
    case 2: //total
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE+2,WHITE,WHITE);
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE-1,BLACK,BLACK);
      break;
    case 3: //partial-annular
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE,LTRYELLOW,LTRYELLOW);
      GrCircle(hdc,Sun.x-4,Sun.y-4,SUN_SIZE-5,lvl_map_background.day_sky_dkcolor,lvl_map_background.day_sky_dkcolor);
      break;
    case 4: //partial-total
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE+2,WHITE,WHITE);
      GrCircle(hdc,Sun.x-4,Sun.y-4,SUN_SIZE-1,BLACK,BLACK);
      break;

    case 0:
    default:
      GrCircle(hdc,Sun.x,Sun.y,SUN_SIZE,LTRYELLOW,LTRYELLOW);
      break;
  }
}


void SunAct()
{
  if (Sun.angle!=Sun.solar_angle) {
    Sun.angle=Sun.solar_angle;

    //x-axis if inverted
    if (lvl_map_background.latitude<0)
      Sun.x=Sun.pivot_x-Sun.dist_l*cos(Sun.angle);
    else
      Sun.x=Sun.pivot_x+Sun.dist_l*cos(Sun.angle);

    Sun.y=Sun.pivot_y+Sun.dist_l*sin(Sun.angle);

    //Modify Stars angles and position
    for (int i=0;i<STAR_NUM;i++) {
      Star.angle[i]=Sun.angle+Star.oangle[i];

      if (lvl_map_background.latitude<0)
        Star.x[i]=Star.pivot_x-Star.dist_l[i]*cos(Star.angle[i]);
      else
        Star.x[i]=Star.pivot_x+Star.dist_l[i]*cos(Star.angle[i]);

      Star.y[i]=Star.pivot_y+Star.dist_l[i]*sin(Star.angle[i]);

      //Dont Draw Stars if close to moon
      if (GetDistance(Star.x[i],Star.y[i],DrawGameMoon.day_moon_x,DrawGameMoon.day_moon_y)<56)
        Star.is_draw[i]=FALSE;
      else 
        Star.is_draw[i]=TRUE;
    }

    //horizon lvl
    if (Sun.angle>=M_PI+TWILIGHT_ANGLE_ASTRONOMICAL && Sun.angle<=2*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL) { //none,lightest
      Sun.horizon_lvl=-1;
    //below horizon
    } else if (Sun.angle<=2*M_PI+TWILIGHT_ANGLE_CIVIL || Sun.angle>=3*M_PI-TWILIGHT_ANGLE_CIVIL) {  //Civil, light
      Sun.horizon_lvl=0;
    } else if (Sun.angle<=2*M_PI+TWILIGHT_ANGLE_NAUTICAL || Sun.angle>=3*M_PI-TWILIGHT_ANGLE_NAUTICAL) { //nautical
      Sun.horizon_lvl=1;
    } else if (Sun.angle<=2*M_PI+TWILIGHT_ANGLE_ASTRONOMICAL || Sun.angle>=3*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL) { //Astronomical
      Sun.horizon_lvl=2;
    } else { //Post Astronomical , darkest
      Sun.horizon_lvl=3;
    }
    flag_draw_game_background_spriteII=TRUE;
  }
  if (Sun.y<128) {
    Sun.y=128;
  }  
}


void SunRayAct()
{
  if (lvl_map_background.is_sun) {
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
  DrawGameMoon.pivot_y=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/5;

  DrawGameMoon.dist_l=GetDistance(DrawGameMoon.x,DrawGameMoon.y,DrawGameMoon.pivot_x,DrawGameMoon.pivot_y);

  DrawGameMoon.angle=GetCosAngle(DrawGameMoon.x-DrawGameMoon.pivot_x,DrawGameMoon.dist_l);
  DrawGameMoon.flag_nightshade=TRUE;




  //Set moon attributes based on *date*
  DrawGameMoon.day_moon_phase_id=0;
  DrawGameMoon.day_moon_angle_id=-1;
  if (!(lunar_day>=13 && lunar_day<=15)) {
    is_blood_moon=FALSE;
  }
  if (lunar_day>=1 && lunar_day<=9) {
    if (lunar_day>=1 && lunar_day<=4) { //1, 2, 3, 4, 5
      DrawGameMoon.day_moon_phase_id=0;
    } else if (lunar_day>=5 && lunar_day<=7) {// 6, 7, 8, 9
      DrawGameMoon.day_moon_phase_id=1;
    } else {
      DrawGameMoon.day_moon_phase_id=2;
    }
  } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
    DrawGameMoon.day_moon_phase_id=3;
  } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
    DrawGameMoon.day_moon_phase_id=4;
    if (is_blood_moon) {
      DrawGameMoon.day_moon_phase_id=8;
    }
  } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
    DrawGameMoon.day_moon_phase_id=5;
  } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
    DrawGameMoon.day_moon_phase_id=6;
  } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
    DrawGameMoon.day_moon_phase_id=7;
  } else { // New Moon, no stars
    DrawGameMoon.day_moon_phase_id=-1;
  }
  //printf("day_moon_phase_id:%d\n",day_moon_phase_id);

  if (lunar_day>=1 && lunar_day<=26 && DrawGameMoon.day_moon_phase_id>=0 && DrawGameMoon.day_moon_phase_id<=8) {
    //Pass public timenow here
    int64_t lhd0=global_lhd0;
    int64_t lhd_n=global_timenow;
          
    float day_moon_angle=Sun.solar_angle - 2*M_PI*(lhd_n-lhd0)/(27*24*60*60); //seconds passed since the first of today's lunar month
    float min_day_moon_angle=  2*M_PI*(2*24*60*60)/(27*24*60*60);
    if (2*M_PI*(lhd_n-lhd0)/(27*24*60*60) <  min_day_moon_angle)
      day_moon_angle=Sun.solar_angle - min_day_moon_angle;
    //day_moon_angle=M_PI+M_PI_2;//debug only

    //Moon's distance
    //https://www.almanac.com/its-spring-see-how-sun-getting-higher-every-day
    float max_day_moon_dist_l=GR_WIDTH/2+10;
    if (map_darkness_seconds<=12*60*60)
      max_day_moon_dist_l*=(float)(map_darkness_seconds/(19.0*60*60)); //Exaggerate
    if (max_day_moon_dist_l>=GR_WIDTH/2+10)
      max_day_moon_dist_l=GR_WIDTH/2+10;

   // printf("dls *:%5.4f ds:%5.4f/24\n",map_sunlight_seconds/(24.0*60*60),(map_darkness_seconds/(24.0*60*60)));

    float day_moon_dist_l=Sun.dist_l; //cresent moon generally same dist_l as Sun
    //Start from Sun_dist_l, move towards max_day_moon_dist_l
    int64_t lhd_4=lhd0+4*24*60*60;
    int64_t lhd_15=lhd0+15*24*60*60;
    if (lunar_day>=4 && lunar_day<=24) { //21-5=16 days  /2 --> 8 days rise, 8 days set
      int64_t day_moon_dist_l_delta=(max_day_moon_dist_l-Sun.dist_l);
      //printf("delta: %lld\n",day_moon_dist_l_delta);
      //delta<0 (-) moon is lower than sun in the summer; dist goes: sun_higher_height-> moon_low_height -> sun_higher_height
      //delta>0 (+) moon is higher than sun in the winter; dist goes: sun_lower_height-> moon_higher_height -> sun_lower_height 
      if (lhd_n<lhd_15) { //4 -> 14
        day_moon_dist_l=  Sun.dist_l + (float)day_moon_dist_l_delta*(float)(lhd_n-lhd_4)/(19*24*60*60.0);
        //printf("delta: %lld, lhd<15: %5.4f\n",day_moon_dist_l_delta,(float)day_moon_dist_l_delta*(float)(lhd_n-lhd_4)/(10*24*60*60.0));
        //printf("delta: %lld, lhd_n: %lld, lhd_4: %lld, lhd<15: %5.4f\n",day_moon_dist_l_delta, lhd_n, lhd_4 ,(float)day_moon_dist_l_delta*(float)(lhd_n-lhd_4)/(10*24*60*60.0));
      } else { // 15 -> 25
        day_moon_dist_l = max_day_moon_dist_l - (float)day_moon_dist_l_delta * (float)(lhd_n-lhd_15)/(19*24*60*60.0);
        //printf("delta: %lld, lhd>=15: %5.4f\n",day_moon_dist_l_delta,(float)day_moon_dist_l_delta * (float)(lhd_n-lhd_15)/(10*24*60*60.0));
      }
      //day_moon_dist_l = Sun.dist_l + (float)day_moon_dist_l_delta * (float)abs(lhd_n-lhd_15)/(10*24*60*60.0);
    }
    if (day_moon_dist_l>=GR_WIDTH/2+10)
      day_moon_dist_l=GR_WIDTH/2+10;
    if (day_moon_dist_l<=10)
      day_moon_dist_l=10;

    //x axis if latitude<0
    if (lvl_map_background.latitude<0)
      DrawGameMoon.day_moon_x=Sun.pivot_x-day_moon_dist_l*cos(day_moon_angle);
    else
      DrawGameMoon.day_moon_x=Sun.pivot_x+day_moon_dist_l*cos(day_moon_angle);

    DrawGameMoon.day_moon_y=Sun.pivot_y+day_moon_dist_l*sin(day_moon_angle);
    if (DrawGameMoon.day_moon_y<128) {
      DrawGameMoon.day_moon_y=128;
    }  


    if (DrawGameMoon.day_moon_x>DrawGameMoon.phase_range_x[0]) { //new-cresent moon, rightside
      DrawGameMoon.day_moon_angle_id=0;
    } else if (DrawGameMoon.day_moon_x<DrawGameMoon.phase_range_x[7]) { //old-cresent moon, leftside
      DrawGameMoon.day_moon_angle_id=6;    
    } else {
      if (DrawGameMoon.day_moon_x>=GR_WIDTH/4+GR_WIDTH/8 && DrawGameMoon.day_moon_x<=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8) {
        DrawGameMoon.day_moon_angle_id=3;
      } else {
        for (int k=1;k<6;k++) { //Decending
          if (DrawGameMoon.day_moon_x<=DrawGameMoon.phase_range_x[k] && DrawGameMoon.day_moon_x>=DrawGameMoon.phase_range_x[k+1]) {
            DrawGameMoon.day_moon_angle_id=k;
            break;
          }
        }
        if (DrawGameMoon.day_moon_angle_id==-1) {
          if (DrawGameMoon.day_moon_x<GR_WIDTH/2) {
            DrawGameMoon.day_moon_angle_id=5;
          } else {
            DrawGameMoon.day_moon_angle_id=1;
          }
        }
      }
    }

  }
}


/*
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
/*  int cmp_id=current_moon_phase_id;
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
/*  } else if (DrawGameMoon.angle<M_PI+M_PI_2) {
    DrawGameMoon.angle+=0.025;//0.005;    
    for (int i=0;i<STAR_NUM;i++) {
      Star.angle[i]+=0.0125;//0.0025;
    }
  }

  //if (DrawGameMoon.x+64>GR_WIDTH-48)
    //DrawGameMoon.x=GR_WIDTH-48;
  if (lvl_map_background.latitude<0)
    DrawGameMoon.x=DrawGameMoon.pivot_x-DrawGameMoon.dist_l*cos(DrawGameMoon.angle);
  else
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
}*/


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
  GrRect(hdcBG,0,0,GR_WIDTH+4,GR_HEIGHT/2,BLACK);
  //GrRect(hdcBG,0,GR_HEIGHT/2,GR_WIDTH+4,GR_HEIGHT/2+4,YELLOW);

  //=================================
  //sky with gradient
  //draw gradient to glass layer
  SelectObject(hdcBG,bg_glass_layer);
  if (lvl_map_background.is_sun) {
    if (!(Sun.solar_angle>=M_PI+TWILIGHT_ANGLE_ASTRONOMICAL && Sun.solar_angle<=2*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL)) { //Sky dawn/dusk/twilight
      /*if ((Sun.solar_angle>=2*M_PI-TWILIGHT_ANGLE_CIVIL && Sun.solar_angle<=2*M_PI+TWILIGHT_ANGLE_ASTRONOMICAL) || 
          (Sun.solar_angle>=3*M_PI-TWILIGHT_ANGLE_ASTRONOMICAL) || 
          (Sun.solar_angle<=M_PI+TWILIGHT_ANGLE_CIVIL))*/ //Below +civil
      if (Sun.horizon_lvl>=1)
        SelectObject(hdc2,red_black_gradient_bg_sprite);
      else
        SelectObject(hdc2,yellow_black_gradient_bg_sprite);
    } else { //Sky Day
      SelectObject(hdc2,white_gradient_bg_sprite);
    }
    StretchBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,800,600,SRCCOPY); //draw gradient to glass layer
  }
  //=================================


  //=================================
  //draw clear sky (no gradient)
  SelectObject(hdc2,game_background_deco_sprite);
  //if (!(Sun.solar_angle>2*M_PI+TWILIGHT_ANGLE_NAUTICAL && Sun.solar_angle<3*M_PI-TWILIGHT_ANGLE_NAUTICAL)) { //above horizon and between sunlight hours
  //draw sky gradient
  if (Sun.horizon_lvl!=3) {
    switch (Sun.eclipse_type) {
      case 0:
      default:
        GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,lvl_map_background.day_sky_color);
        break;
      case 1: //Annular Eclipse
      case 3:
        GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,lvl_map_background.day_sky_dkcolor);
        break;
      case 2: //Total/Full Eclipse
      case 4:
        GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,BLACK);
        break;
    }
  } else { //Night
    //if (Sun.x>GR_WIDTH/2) //rising
    //if (...); //moon is visible
      //GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,RGB(0,0,24));
    GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,lvl_map_background.night_sky_color);
    //else //setting
      //GrRect(hdc2,0,0,GR_WIDTH+24,GR_HEIGHT+24,BLACK);
  }
  //=================================



  //=================================
  //overlay gradient on glass layer over clear sky
  if (lvl_map_background.is_sun) { //Draw Gradient if sun exists
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
            //alpha=136/3; //nice yellow clouds
            if (global_screen_bits==32)
              //alpha=136/3+16; //nice yellow clouds
              alpha=136/2-14; //nice yellow clouds**
              //alpha=136/2-4; //nice yellow clouds
            else
              alpha=136/2-13; //nice yellow clouds
            break;
          case 1:
            alpha=136;
            break;
          case 2:
            alpha=136;
            break;
          case 3:
            alpha=-1;
            break;
        }
        if (alpha>0) {
        //int the=-GR_HEIGHT+Sun.y+SUN_SIZE*8;
        /*if (Sun.horizon_lvl>=1) {
          the=Sun.y-GR_HEIGHT;
        } else {
          if (the<0)
            the=0;
        }*/
        /*GrGlassTexture(hdc2, game_background_deco_sprite,0,0,GR_WIDTH,the-SUN_SIZE*2,hdcBG, bg_black_layer,0,0,GR_WIDTH,the, alpha/2);
        GrGlassTexture(hdc2, game_background_deco_sprite,0,the,GR_WIDTH,GR_HEIGHT,hdcBG, bg_glass_layer,0,0,GR_WIDTH,GR_HEIGHT, alpha/2);

        GrGlassTexture(hdc2, game_background_deco_sprite,0,0,GR_WIDTH,the,hdcBG, bg_black_layer,0,0,GR_WIDTH,the, alpha/2);
        GrGlassTexture(hdc2, game_background_deco_sprite,0,the-SUN_SIZE*2,GR_WIDTH,GR_HEIGHT,hdcBG, bg_glass_layer,0,0,GR_WIDTH,GR_HEIGHT, alpha/2);*/

        //GrGlassTexture(hdc2, game_background_deco_sprite,0,0,GR_WIDTH,the,hdcBG, bg_black_layer,0,0,GR_WIDTH,the, alpha);

        //GR_HEIGHT/6+GR_HEIGHT/3;//GR_HEIGHT/20+GR_HEIGHT/4
        //

          //Consistent 2026-02-08
          GrGlassTexture(hdc2, game_background_deco_sprite,0,0,GR_WIDTH,Sun.y+SUN_SIZE*4,hdcBG, bg_glass_layer,0,0,GR_WIDTH,GR_HEIGHT, alpha); //abovesun to sun
          GrGlassTexture(hdc2, game_background_deco_sprite,0,Sun.y+SUN_SIZE*4,GR_WIDTH,  GR_HEIGHT ,hdcBG, bg_glass_layer,0,GR_HEIGHT-16,GR_WIDTH,16, alpha); //yellow below sun
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
  }
  //=================================




  if (lvl_map_background.background_id>BACKGROUND_FOREGROUND_SPRITE_NUM) {
  //stretched full background not foreground, overwrites previous 
    SelectObject(hdcBG,load_background_full_sprite[lvl_map_background.background_id-BACKGROUND_FOREGROUND_SPRITE_NUM-1]);
    StretchBlt(hdc2,0,0,GR_WIDTH,GR_HEIGHT,hdcBG,0,0,800,480,SRCCOPY);
  } else {


  //Draw Sun
  if (lvl_map_background.is_sun && Sun.horizon_lvl<3)
    DrawSun(hdc2,hdcBG);

  //========Draw Moon (flipped or not)=============
  //Stretch (upsidedown + flipped) .sprite_paint to target .sprite_paint only
  //Extra condition: flip moon upside down and face it leftwards if latitude <= 0degs is south
  if (DrawGameMoon.day_moon_phase_id!=-1 && DrawGameMoon.day_moon_angle_id!=-1 && lvl_map_background.is_moon) {
    SelectObject(hdc2,Moon[DrawGameMoon.day_moon_phase_id].draw_moon_sprite[DrawGameMoon.day_moon_angle_id].sprite_paint); //select moon
    SelectObject(hdcBG,DrawGameMoon.draw_moon_sprite_lvl); //select 

    BITMAP bm;
    GetObject(Moon[DrawGameMoon.day_moon_phase_id].draw_moon_sprite[DrawGameMoon.day_moon_angle_id].sprite_paint,sizeof(BITMAP),&bm);
    int size=bm.bmWidth;
    GrRect(hdcBG,0,0,257,257,BLACK);

    if (lvl_map_background.latitude<0) {
      StretchBlt(hdcBG,size,size,-size,-size,hdc2,0,0,size,size,SRCCOPY);
    } else {
      BitBlt(hdcBG,0,0,size,size,hdc2,0,0,SRCCOPY);
    }
  }

  //===============================================
  //Draw Background type
  if (lvl_map_background.background_id>0 && lvl_map_background.background_id<=BACKGROUND_FOREGROUND_SPRITE_NUM) {
    SelectObject(hdcBG,draw_background_sprite_stretched_mask);
    SelectObject(hdc2,draw_background_sprite[lvl_map_background.background_id-1].sprite_mask);
    GrRect(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,BLACK);
    StretchBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,800,480,SRCCOPY);
  
    SelectObject(hdcBG,draw_background_sprite_stretched_paint);
    SelectObject(hdc2,draw_background_sprite[lvl_map_background.background_id-1].sprite_paint);
    GrRect(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,BLACK);
    StretchBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,800,480,SRCCOPY);
  }
  //===============================================

  SelectObject(hdcBG,game_background_deco_sprite);

  //Draw Moon **
  if (DrawGameMoon.day_moon_phase_id!=-1 && DrawGameMoon.day_moon_angle_id!=-1 && lvl_map_background.is_moon) {
    BITMAP bm;
    GetObject(Moon[DrawGameMoon.day_moon_phase_id].draw_moon_sprite[DrawGameMoon.day_moon_angle_id].sprite_paint,sizeof(BITMAP),&bm);
    int size=bm.bmWidth;
    SelectObject(hdc2,DrawGameMoon.draw_moon_sprite_lvl);
    BitBlt(hdcBG,DrawGameMoon.day_moon_x-size/2,DrawGameMoon.day_moon_y-size/2,size,size,hdc2,0,0,SRCPAINT); //draw to background
  }

  //}
  //End of Draw Moon
  //===================)===========================


  if (lvl_map_background.background_id>0 && lvl_map_background.background_id<BACKGROUND_FOREGROUND_SPRITE_NUM){
  //Draw Background infront of clouds and skies
    SelectObject(hdc2,draw_background_sprite_stretched_mask);
    BitBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,SRCAND);

    SelectObject(hdc2,draw_background_sprite_stretched_paint);
    BitBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,SRCPAINT);
  }
  }
  DeleteDC(hdcBG);
}

//int64_t funnyrun;
/*void DrawMoon(HDC hdc,HDC hdc2)
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
*/

//loads 120 cnt delay 
void DrawGameBackgroundSprite(HDC hdcMain,HDC hdc2)
{ //runs on call
  HDC hdcBG=CreateCompatibleDC(hdcMain); 

  //Draw decorated background (like a rect) to official game background, 16/32-bits (has gradient**)
  SelectObject(hdcBG,game_background_sprite);
  SelectObject(hdc2,game_background_deco_sprite);
  BitBlt(hdcBG,0,0,GR_WIDTH,GR_HEIGHT,hdc2,0,0,SRCCOPY);

  //====================*=========================
  //Draw Stars
  if (Sun.solar_angle>=M_PI+TWILIGHT_ANGLE_CIVIL && Sun.solar_angle<=2*M_PI-TWILIGHT_ANGLE_CIVIL) {
    switch (Sun.eclipse_type) {
      case 2: //Total/Full Eclipse
      case 4:
        if (lvl_map_background.is_stars)
          DrawStars(hdcBG);
        break;
    }
  } else { //dawn/dusk
    if (lvl_map_background.is_stars)
      DrawStars(hdcBG);
  }
  //==============================================


  //Draw Clouds
  DrawClouds(hdcBG,hdc2);
  DeleteDC(hdcBG);
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
  if (!hide_taskbar) {
    if (GR_HEIGHT>SCREEN_HEIGHT-1);
      dbg_height=SCREEN_HEIGHT-1;
  } else {
    if (GR_HEIGHT>SCREEN_HEIGHT);
      dbg_height=SCREEN_HEIGHT;
  }


  if (flag_draw_game_background_spriteII) { //Draw static rect background with gradient
    DrawGameBackgroundSpriteII(hdc,hdc2);
    flag_draw_game_background_spriteII=FALSE;
  }
  if (flag_draw_game_background_sprite) { //Refresh background, runs when timer hits 120, delayed for speed, dynamic rect background, no gradient   
    //DrawGameBackgroundSpriteII(hdc,hdc2); //debug only
    StarAct();
    DrawGameBackgroundSprite(hdc,hdc2);
    flag_draw_game_background_sprite=FALSE;
  }


  FastDrawTexturedRect(publicScreenPixels,SCREEN_WIDTH,publicBackgroundPixels,GR_WIDTH,dbg_height,global_screen_bits); //Draw Background giant rect infront of skies

  //Draw shooting stars, live according to tick
  if ((lvl_map_background.is_stars && Sun.y>=GR_HEIGHT+GR_HEIGHT/7)) {
    for (int i=0;i<SSTAR_NUM;i++) {
      if (SStar[i].lifetime>0) {
        GrCircle(hdc,SStar[i].x-GR_WIDTH/16*_ppx,SStar[i].y-GR_HEIGHT/16*_ppy,1,WHITE,WHITE);
      }
    }
  }

  //Draw Wind Mill Hands
  if (lvl_map_background.background_id==4) {
    int i=bg_windmillhand_id;
    if (i>=0 && i<=3) {
      for (int j=0;j<12;j++) {
        GrLineThick(hdc,bg_windmillhand[i].x1[j],bg_windmillhand[i].y1[j],bg_windmillhand[i].x2[j],bg_windmillhand[i].y2[j],3,DKGRAY);
      }
    }
  }


}
//==============================================================================

