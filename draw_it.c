


//Background
void DrawBackground(HDC hdc) 
{
  switch (map_background) {
    case 0:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      break;
    case 1:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      break;
    default:
      if (map_background_sprite==NULL) {
        GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,custom_map_background_color);
      } else {
        DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      }
      break;
  }

  if (is_moon) {
    DrawSprite(hdc, GR_WIDTH-128,128,&draw_moon_sprite,FALSE);
  }
}



void DrawWaterPlatforms(HDC hdc) 
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
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cy2;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid!=-1 && _y>0 && _x>0 && _x<MAP_WIDTH && _y<MAP_HEIGHT) {
      tmf_id=VGrid[gid]->draw_foreground_seg_id;
      if (tmf_id!=-1) {
        x=GR_WIDTH/2+TileMapForeground[tmf_id]->x-px+cx1+cx2;
        y=GR_HEIGHT/2+TileMapForeground[tmf_id]->y-py+cy1+cy2;
        DrawBitmap(hdc,x, y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapForeground[tmf_id]->sprite_mask,SRCAND,FALSE,FALSE);
        DrawBitmap(hdc,x,y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapForeground[tmf_id]->sprite_paint,SRCPAINT,FALSE,FALSE);
        //DrawSprite(hdc,x,y,&TileMapForeground[tmf_id]->draw_tile,FALSE);
      } 
    } 
  }
}


void DrawPlatforms(HDC hdc)
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
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cy2;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid!=-1 && _y>0 && _x>0 && _x<MAP_WIDTH && _y<MAP_HEIGHT) {
      tmp_id=VGrid[gid]->draw_platform_seg_id;
      if (tmp_id!=-1) {
        x=GR_WIDTH/2+TileMapPlatform[tmp_id]->x-px+cx1+cx2;
        y=GR_HEIGHT/2+TileMapPlatform[tmp_id]->y-py+cy1+cy2;
        DrawBitmap(hdc,x, y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapPlatform[tmp_id]->sprite_mask,SRCAND,FALSE,FALSE);
        DrawBitmap(hdc,x,y,0, 0,VGRID_SIZE,VGRID_SIZE,TileMapPlatform[tmp_id]->sprite_paint,SRCPAINT,FALSE,FALSE);
        //DrawSprite(hdc,x,y,&TileMapPlatform[tmp_id]->draw_tile,FALSE);
      } 
    } 
  }
}


void DrawShadows(HDC hdc)
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
  //  GrCircle(hdc,GR_WIDTH/2-cx1+cx2,GR_HEIGHT/2-cy1+cy2,10,BLUE,BLUE);

//  for (int i=0;i<SHADOW_GRID_NUM;i++) {
//  for (int i=0;i<RDGRID_NUM;i++) {
  for (int i=0;i<RDGRID_DYN_NUM;i++) {
    //tms_id=i;
    _x=RDGrid[i].x+px-GR_WIDTH/2-cx1-cy2;//+cx1+cx2;//GR_WIDTH/2+RDGrid[i].x-px+cx1+cx2;
    _y=RDGrid[i].y+py-GR_HEIGHT/2-cy1-cy2;//+cy1+cy2;//GR_HEIGHT/2+RDGrid[i].y-py+cy1+cy2;
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    //GrCircle(hdc,_x,_y,15,RED,RED);
    if (gid!=-1 && _y>0 && _x>0 && _x<MAP_WIDTH && _y<MAP_HEIGHT) {
      tms_id=VGrid[gid]->draw_shadow_seg_id;
      if (tms_id!=-1) {
        x=GR_WIDTH/2+TileMapShadow[tms_id]->x-px+cx1+cx2;
        y=GR_HEIGHT/2+TileMapShadow[tms_id]->y-py+cy1+cy2;
        DrawGlassBitmap(hdc, 
          TileMapShadow[tms_id]->sprite_paint, 
          x,y,
          32//128
        );
      } else {
        x=GR_WIDTH/2+VGrid[gid]->x1-px+cx1+cx2;
        y=GR_HEIGHT/2+VGrid[gid]->y1-py+cy1+cy2;
        if (map_background==1 || (map_background==2 && custom_map_background_color_i>127)) {
          GrGlassRect(hdc,x,y,VGRID_SIZE,VGRID_SIZE,DKRDKGRAY,32);
        } else {
          GrGlassRect(hdc,x,y,VGRID_SIZE,VGRID_SIZE,LTGRAY,32);
        }
      }
    } /*else {
      x=_x;
      y=_y;
      if (map_background==1 || (map_background==2 && custom_map_background_color_i>127)) {
        GrGlassRect(hdc,x,y,VGRID_SIZE,VGRID_SIZE,DKRDKGRAY,32);
      } else {
        GrGlassRect(hdc,x,y,VGRID_SIZE,VGRID_SIZE,LTGRAY,32);
      }
    }*/
  }
}


void DrawCursor(HDC hDC)
{
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite,SRCAND,FALSE);
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite_mask,SRCPAINT,FALSE);

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


  //right side eyes
  switch (player.knives_per_throw) {
    case 1: //right->closed
    case 3:
      DrawSprite(hDC,mouse_x+rand_shake_x+46,mouse_y+rand_shake_y-12,&draw_player_cursor_body[13],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+46,mouse_y+rand_shake_y-12,&draw_player_cursor_iris[13],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x+50,mouse_y+rand_shake_y+10,&draw_player_cursor_body[14],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+50,mouse_y+rand_shake_y+10,&draw_player_cursor_iris[14],FALSE);


      DrawSprite(hDC,mouse_x+rand_shake_x+60,mouse_y+rand_shake_y+6,&draw_player_cursor_body[15],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+60,mouse_y+rand_shake_y+6,&draw_player_cursor_iris[15],FALSE);
      break;

    case 0:
    case 5: //right->opened
    case 15:
      DrawSprite(hDC,mouse_x+rand_shake_x+46,mouse_y+rand_shake_y-12,&draw_player_cursor_body[5],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+46,mouse_y+rand_shake_y-12,&draw_player_cursor_iris[5],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x+50,mouse_y+rand_shake_y+10,&draw_player_cursor_body[6],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+50,mouse_y+rand_shake_y+10,&draw_player_cursor_iris[6],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x+60,mouse_y+rand_shake_y+6,&draw_player_cursor_body[7],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+60,mouse_y+rand_shake_y+6,&draw_player_cursor_iris[7],FALSE);
      break;
  }



  //left side eyes
  switch (player.knives_per_throw) {
    case 1:
    case 5:
      DrawSprite(hDC,mouse_x+rand_shake_x+16,mouse_y+rand_shake_y-24,&draw_player_cursor_body[9],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+16,mouse_y+rand_shake_y-24,&draw_player_cursor_iris[9],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x-22,mouse_y+rand_shake_y+20,&draw_player_cursor_body[10],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x-22,mouse_y+rand_shake_y+20,&draw_player_cursor_iris[10],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x+24,mouse_y+rand_shake_y+20,&draw_player_cursor_body[11],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+24,mouse_y+rand_shake_y+20,&draw_player_cursor_iris[11],FALSE);
      break;
    case 0:
    case 3:
    case 15:
      DrawSprite(hDC,mouse_x+rand_shake_x+16,mouse_y+rand_shake_y-24,&draw_player_cursor_body[1],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+16,mouse_y+rand_shake_y-24,&draw_player_cursor_iris[1],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x-22,mouse_y+rand_shake_y+20,&draw_player_cursor_body[2],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x-22,mouse_y+rand_shake_y+20,&draw_player_cursor_iris[2],FALSE);

      DrawSprite(hDC,mouse_x+rand_shake_x+24,mouse_y+rand_shake_y+20,&draw_player_cursor_body[3],FALSE);
      DrawSprite(hDC,mouse_x+rand_shake_x+24,mouse_y+rand_shake_y+20,&draw_player_cursor_iris[3],FALSE);
      break;
  }


  if (player.health>PLAYER_LOW_HEALTH) {
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_body[0],FALSE); //left ete open
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_iris[0],FALSE);
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_pupil[0],FALSE);

    DrawSprite(hDC,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-4,&draw_player_cursor_body[4],FALSE); //right eye open
    DrawSprite(hDC,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-4,&draw_player_cursor_iris[4],FALSE);
  } else {
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_body[8],FALSE); //left eye low hp
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_iris[8],FALSE);
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_pupil[1],FALSE);

    DrawSprite(hDC,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-4,&draw_player_cursor_body[12],FALSE); //right eye low hp
    DrawSprite(hDC,mouse_x+rand_shake_x+52,mouse_y+rand_shake_y-4,&draw_player_cursor_iris[12],FALSE);
  }
  }
}


