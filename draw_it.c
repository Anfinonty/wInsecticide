


//Background
void DrawBackground(HDC hdc,HDC hdc2) 
{
  double parralax_x=0;
  double parralax_y=0;
  double _ppx=(player.x/MAP_WIDTH);
  double _ppy=(player.y/MAP_HEIGHT);
  double ppx=(MAP_WIDTH-player.x)/MAP_WIDTH;
  double ppy=(MAP_HEIGHT-player.y)/MAP_HEIGHT;
  parralax_x=(-GR_WIDTH/16*2)+GR_WIDTH/16*ppx;
  parralax_y=(-GR_HEIGHT/16*2)+GR_HEIGHT/16*ppy;
  int draw_p_px=(int)parralax_x;
  int draw_p_py=(int)parralax_y;


  switch (map_background) {
    case 0:
      DrawBitmap(hdc,hdc2,draw_p_px,draw_p_py,0,0,GR_WIDTH*2,GR_HEIGHT*2,map_background_sprite,SRCCOPY,FALSE,FALSE);
      break;
    default:
      if (map_background_sprite==NULL) {
        GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,custom_map_background_color);
      } else {
        DrawBitmap(hdc,hdc2,draw_p_px,draw_p_py,0,0,GR_WIDTH*2,GR_HEIGHT*2,map_background_sprite,SRCCOPY,FALSE,FALSE);
      }
      break;
  }

  if (is_moon) {
    /*if (lunar_day<=15) {
      DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
    } else {
    }*/

      int dmx=-1000;
      int dmy=-1000;
      if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
        dmx=GR_WIDTH-GR_WIDTH/8-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT-GR_HEIGHT/3-GR_HEIGHT/16*_ppy;//GR_HEIGHT-GR_HEIGHT/6;
        //dmy=160+160-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
        dmx=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3-GR_HEIGHT/16*_ppy;
        //dmy=160+110-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
        dmx=GR_WIDTH/2+GR_WIDTH/4-GR_WIDTH/8-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT/4+GR_HEIGHT/12-GR_HEIGHT/16*_ppy;
        //dmy=160+50-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15 //fullmoon
        dmx=GR_WIDTH/2-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT/4-GR_HEIGHT/16*_ppy;
        //dmy=160-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
        dmx=GR_WIDTH/4+GR_WIDTH/8-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT/4+GR_HEIGHT/12-GR_HEIGHT/16*_ppy;
        //dmy=160+50-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
        dmx=GR_WIDTH/4-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT-GR_HEIGHT/6-GR_HEIGHT/3-GR_HEIGHT/16*_ppy;
        //dmy=160+110-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
        dmx=GR_WIDTH/8-GR_WIDTH/16*_ppx;
        dmy=GR_HEIGHT-GR_HEIGHT/3-GR_HEIGHT/16*_ppy;//GR_HEIGHT-GR_HEIGHT/6;
        //dmy=160+160-GR_HEIGHT/16*_ppy;
        DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);
      }// else {
//      }
//      DrawSprite(hdc, hdc2,dmx,dmy,&draw_moon_sprite,FALSE);

  }
}


void DrawWaterPlatformsTexture(HDC hdc,HDC hdc2)
{
  if (WATER_GROUND_NUM>0) {
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y;
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
            x1=GR_WIDTH/2+(int)Ground[i]->x1-px+cx1+cx2;
            y1=GR_HEIGHT/2+(int)Ground[i]->y1-py+cy1+cy2;
            x2=GR_WIDTH/2+(int)Ground[i]->x2-px+cx1+cx2;
            y2=GR_HEIGHT/2+(int)Ground[i]->y2-py+cy1+cy2;
            x3=GR_WIDTH/2+(int)Ground[i]->x3-px+cx1+cx2;
            y3=GR_HEIGHT/2+(int)Ground[i]->y3-py+cy1+cy2;
            DrawTexturedTriangle(hdc,hdc2,x1,y1,x2,y2,x3,y3,texture_water[global_water_texture_id]);
        }
      }
    }
  }
  }
}


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
  int gid,tmf_id;
  int x,y,_x,_y;
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y;
  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid>-1 && gid<VGRID_NUM && _x>=0 && _y>=0 && _x<=MAP_WIDTH && _y<=MAP_HEIGHT) {
      tmf_id=VGrid[gid]->draw_foreground_seg_id;
      if (tmf_id!=-1) {
        x=GR_WIDTH/2+TileMapForeground[tmf_id]->x-px+cx1+cx2;
        y=GR_HEIGHT/2+TileMapForeground[tmf_id]->y-py+cy1+cy2;
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
    cy2=player.cam_move_y;
  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid>-1 && gid<VGRID_NUM && _x>=0 && _y>=0 && _x<=MAP_WIDTH && _y<=MAP_HEIGHT) {
      tmp_id=VGrid[gid]->draw_platform_seg_id;
      if (tmp_id!=-1) {
        x=GR_WIDTH/2+TileMapPlatform[tmp_id]->x-px+cx1+cx2;
        y=GR_HEIGHT/2+TileMapPlatform[tmp_id]->y-py+cy1+cy2;


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
    cy2=player.cam_move_y;
  int x=GR_WIDTH/2+0-px+cx1+cx2;

  GrRect(hdc,0,0,x+4,GR_HEIGHT+4,BLACK);
  x=GR_WIDTH/2+MAP_WIDTH-px+cx1+cx2-4;
  w=GR_WIDTH-(MAP_WIDTH-player.x);
  if (w>0) {
    GrRect(hdc,x,0,w,GR_HEIGHT+4,BLACK);
  }

  int y=GR_HEIGHT/2+MAP_HEIGHT-py+cy1+cy2-4;
  l=GR_HEIGHT-(MAP_HEIGHT-player.y);
  if (l>0) {
    GrRect(hdc,-4,y,GR_WIDTH+8,l,BLACK);
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
    cy2=player.cam_move_y;
  //2024-12-21 IMPORTANT: floating point in drawing sprite will cause grids to form and cause grid infighting = lag
  //turn it to int to make it appear as one singular bitmap
  //  GrCircle(hdc,GR_WIDTH/2-cx1+cx2,GR_HEIGHT/2-cy1+cy2,10,BLUE,BLUE); //debug

  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cx2;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid>-1 && gid<VGRID_NUM && _x>=0 && _y>=0 && _x<=MAP_WIDTH && _y<=MAP_HEIGHT) {
      tms_id=VGrid[gid]->draw_shadow_seg_id;
      if (tms_id!=-1) {
        x=GR_WIDTH/2+TileMapShadow[tms_id]->x-px+cx1+cx2;
        y=GR_HEIGHT/2+TileMapShadow[tms_id]->y-py+cy1+cy2;
        SelectObject(hdc2,TileMapShadow[tms_id]->sprite_paint);
        AlphaBlend(hdc, x, y, VGRID_SIZE, VGRID_SIZE, hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, gblendFunction);
      } else {
        x=GR_WIDTH/2+VGrid[gid]->x1-px+cx1+cx2;
        y=GR_HEIGHT/2+VGrid[gid]->y1-py+cy1+cy2;
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
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite,SRCAND,FALSE);
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite_mask,SRCPAINT,FALSE);
  /*if (show_fps && saved_showoff>0) {
    char fpstxt[32];
    sprintf(fpstxt,"FPS: %d / %d",saved_showoff,FPS);
    int FPS_x=mouse_x-(strlen(fpstxt)*8-14)/2;
    GrPrintThick(hDC,FPS_x,mouse_y+64,fpstxt,WHITE,BLACK);
  }*/


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
      rand_shake_x=RandNum(-10,10,player.seed);
      rand_shake_y=RandNum(-10,10,player.seed);
    } else  {
      rand_shake_x=RandNum(-5,5,player.seed);
      rand_shake_y=RandNum(-5,5,player.seed);
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
*/

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
  }
}


