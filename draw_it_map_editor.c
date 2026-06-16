
char *enemy_type_int_attr_names[ENEMY_TYPE_INT_ATTR_NUM]=
{
"Species",
"Follow Range",
"Unchase Range",
"Chase Range",
"Color", //4
"Speed *",
"Health",
"Shoot at Player Range",
"Aim Randomness",
"Bullet Cooldown",
"Bullet Fire Cooldown",
"Bullet Fired At Once",
"Bullet Length",
"Bullet Damage",
"Bullet Speed *",
"Bullet Range",
"Bullet Color", //16
"Bullet Type",
"Timebreaker Rare",
"Timebreaker Length"
};

char *enemy_type_float_attr_names[ENEMY_TYPE_FLOAT_ATTR_NUM]=
{
"Speed",
"Bullet Speed"
};

char *enemy_type_bool_attr_names[ENEMY_TYPE_BOOL_ATTR_NUM]=
{
"Timebreaker Immune"
};


char *melvlattrtxt_arr[S_LVL_ATTR_NUM]=
{
  "Background ID",
  "Real Time",
  "Unix Time",
  "Latitude",
  "Longitude",
  "UTC Offset",
  "Sun",
  "Stars",
  "Moon",
  "Eclipse",
  "Clouds",
  "Background Day Color",
  "Background Night Color",
  "Weather",
  "Weather dy",
  "Weather dx",
  "Brightness Type",
  "Brightness Level"
};

char *melvlattrtxt_eclipse_arr[6]=
{
  "None",
  "Annular",
  "Total",
  "Partial Annular",
  "Partial Total",
  "Lunar"
};

char *melvlattrtxt_cloud_arr[5]=
{
  "Light Some",
  "Dark Some",
  "Light Cloudy",
  "Dark Cloudy",
  "None",
};


char *melvlattrtxt_weather_arr[4]=
{
  "None",
  "Raining",
  "Snowing",
  "Hailstorm"
};

char *melvlattrtxt_brightness_type_arr[2]=
{
  "Static",
  "Dynamic"
};

char *melvlattrtxt_darkness_lvl_arr[5]=
{
  "Normal",
  "Dark",
  "Darker",
  "Darkest",
  "Night"
};


void DrawMapEditorWaterTexturePlatforms(HDC hdc, HDC hdc2)
{
  //Draw type 1 == Water
  int i;
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
              DrawTexturedTriangle(hdc,hdc2,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,texture_water[0]);
        }
      }
    }
  }
}

void DrawMapEditorWaterPlatforms(HDC hdc)
{
  //Draw type 1 == Water
  int i,c;
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        //c=rgbPaint[Ground[i]->color_id];
        c=rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[i]->color_id];
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,TRUE,HS_HORIZONTAL);
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,TRUE,HS_BDIAGONAL);

        }
      }
    }
  }
}


//Graphics
void DrawMapEditorFallingGround(HDC hdc,HDC hdc2)
{
  int c;
  int fgcx1;
  int fgcy1;
  int fgcx2;
  int fgcy2;
  int fgcx3;
  int fgcy3;
  //NOTE: STARTS AT CENTER
  for (int j=0;j<FGROUND_NUM;j++) {
    //Draw Start and End Line
    GrLine(hdc,
      F_GROUND[j].x_start+player.cam_x+GR_WIDTH/2,
      F_GROUND[j].y_start+player.cam_y+GR_HEIGHT/2,
      F_GROUND[j].x_end+player.cam_x+GR_WIDTH/2,
      F_GROUND[j].y_end+player.cam_y+GR_HEIGHT/2,
      BLACK
    );

    GrCircle(hdc,
      F_GROUND[j].x_end+player.cam_x+GR_WIDTH/2,
      F_GROUND[j].y_end+player.cam_y+GR_HEIGHT/2,
      10,
      BLACK,-1
    );

    //Draw Falling Ground
    for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {

      fgcx1=F_GROUND[j].x1[i]+player.cam_x+GR_WIDTH/2;
      fgcy1=F_GROUND[j].y1[i]+player.cam_y+GR_HEIGHT/2;
      fgcx2=F_GROUND[j].x2[i]+player.cam_x+GR_WIDTH/2;
      fgcy2=F_GROUND[j].y2[i]+player.cam_y+GR_HEIGHT/2;
      fgcx3=F_GROUND[j].x3[i]+player.cam_x+GR_WIDTH/2;
      fgcy3=F_GROUND[j].y3[i]+player.cam_y+GR_HEIGHT/2;

      c=rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][F_GROUND[j].f2ground[i].color_id];
      switch (F_GROUND[j].f2ground[i].type) {

        case 0: //line
        case 1: //line but floor not ceiling
          GrLine(hdc,fgcx1,fgcy1,fgcx2,fgcy2,c);
          break;
        case 2: //trifill
	        DrawTriFill(hdc,c,
               fgcx1,fgcy1,fgcx2,fgcy2,fgcx3,fgcy3,FALSE,0);
          break;
        case 3: //texture trifill
          {
          int texture_type=F_GROUND[j].f2ground[i].texture_type;
          if (texture_type>=0 && texture_type<PLATFORM_TEXTURES_NUM) {
            DrawTexturedTriangle(hdc,hdc2,
              fgcx1,fgcy1,fgcx2,fgcy2,fgcx3,fgcy3,
              GamePlatformTextures[texture_type].palette_sprite);
          }
          }
          break;
      }
    }  
  }
}


void DrawMapEditorFallingGroundEditor(HDC hdc, HDC hdc2)
{
  //Draw Draw Map Editor Grid at Center
  int cx1=GR_WIDTH/2-(FGROUND_SIZE+2)/2;
  int cy1=GR_HEIGHT/2-(FGROUND_SIZE)/2;
  int cx2=GR_WIDTH/2+(FGROUND_SIZE)/2;
  int cy2=GR_HEIGHT/2+(FGROUND_SIZE)/2;
  GrLineThick(hdc,cx1,cy1,cx2,cy1,3,BLACK);
  GrLineThick(hdc,cx1,cy1,cx1,cy2,3,BLACK);
  GrLineThick(hdc,cx2,cy1,cx2,cy2,3,BLACK);
  GrLineThick(hdc,cx1,cy2,cx2,cy2,3,BLACK);
  GrLine(hdc,cx1,cy1,cx2,cy1,LTGRAY);
  GrLine(hdc,cx1,cy1,cx1,cy2,LTGRAY);
  GrLine(hdc,cx2,cy1,cx2,cy2,LTGRAY);
  GrLine(hdc,cx1,cy2,cx2,cy2,LTGRAY);
  GrCircle(hdc,GR_WIDTH/2,GR_HEIGHT/2,2,BLACK,LTGRAY);

  int c;
  int fgcx1;
  int fgcy1;
  int fgcx2;
  int fgcy2;
  int fgcx3;
  int fgcy3;

  //Draw Selected Falling Ground at Center
  for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {

    fgcx1=F_GROUND[MapEditor.selected_fground_id].f2ground[i].ox1+cx1;
    fgcy1=F_GROUND[MapEditor.selected_fground_id].f2ground[i].oy1+cy1;
    fgcx2=F_GROUND[MapEditor.selected_fground_id].f2ground[i].ox2+cx1;
    fgcy2=F_GROUND[MapEditor.selected_fground_id].f2ground[i].oy2+cy1;
    fgcx3=F_GROUND[MapEditor.selected_fground_id].f2ground[i].ox3+cx1;
    fgcy3=F_GROUND[MapEditor.selected_fground_id].f2ground[i].oy3+cy1;

    c=rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][F_GROUND[MapEditor.selected_fground_id].f2ground[i].color_id];
    switch (F_GROUND[MapEditor.selected_fground_id].f2ground[i].type) {

      case 0: //line
      case 1: //line but floor not ceiling
        GrLine(hdc,fgcx1,fgcy1,fgcx2,fgcy2,c);
        break;
      case 2: //trifill
	      DrawTriFill(hdc,c,
             fgcx1,fgcy1,fgcx2,fgcy2,fgcx3,fgcy3,FALSE,0);
        break;
      case 3: //texture trifill
        {
        int texture_type=F_GROUND[MapEditor.selected_fground_id].f2ground[i].texture_type;
        if (texture_type>=0 && texture_type<PLATFORM_TEXTURES_NUM) {
        DrawTexturedTriangle(hdc,hdc2,
          fgcx1,fgcy1,fgcx2,fgcy2,fgcx3,fgcy3,
          GamePlatformTextures[texture_type].palette_sprite);
        }
        }
        break;
    }
  }

  char print_ground_id[4];
  for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {
    fgcx1=F_GROUND[MapEditor.selected_fground_id].f2ground[i].ox1+cx1;
    fgcy1=F_GROUND[MapEditor.selected_fground_id].f2ground[i].oy1+cy1;
    fgcx2=F_GROUND[MapEditor.selected_fground_id].f2ground[i].ox2+cx1;
    fgcy2=F_GROUND[MapEditor.selected_fground_id].f2ground[i].oy2+cy1;
    fgcx3=F_GROUND[MapEditor.selected_fground_id].f2ground[i].ox3+cx1;
    fgcy3=F_GROUND[MapEditor.selected_fground_id].f2ground[i].oy3+cy1;


      c=Highlight((i==MapEditor.selected_fground_ground_id),BLACK,LTPURPLE);
      sprintf(print_ground_id,"%d",i);
      GrPrintThick(hdc,fgcx1,fgcy1,print_ground_id,c,BLACK);

      c=Highlight((MapEditor.selected_ground_pivot==0 && i==MapEditor.selected_fground_id),BLACK,LTPURPLE);
      GrCircle(hdc,fgcx1,fgcy1,6,c,-1);
      c=Highlight((MapEditor.selected_ground_pivot==1 && i==MapEditor.selected_fground_id),BLACK,LTPURPLE);
      GrCircle(hdc,fgcx2,fgcy2,6,c,-1);
      if (F_GROUND[MapEditor.selected_fground_id].f2ground[i].type==3 || F_GROUND[MapEditor.selected_fground_id].f2ground[i].type==2) {
        c=Highlight((MapEditor.selected_ground_pivot==2 && i==MapEditor.selected_fground_id),BLACK,LTPURPLE);
        GrCircle(hdc,fgcx3,fgcy3,6,c,-1);
      }

      if (i==MapEditor.selected_fground_ground_id) {
        switch (MapEditor.selected_ground_pivot) {
          case 0:
            c=Highlight((MapEditor.selected_ground_pivot==0),BLACK,LTPURPLE);
            GrCircle(hdc,fgcx1,fgcy1,8,c,-1);
            break;
          case 1:
            c=Highlight((MapEditor.selected_ground_pivot==1),BLACK,LTPURPLE);
            GrCircle(hdc,fgcx2,fgcy2,8,c,-1);
            break;
          case 2:
            c=Highlight((MapEditor.selected_ground_pivot==2),BLACK,LTPURPLE);
            GrCircle(hdc,fgcx3,fgcy3,8,c,-1);
            break;
        }
      }
  }
}



void DrawMapEditorPlatforms(HDC hdc,HDC hdc2)
{
  //Draw type 3
  int c;
  int i;
  int player_cam_move_x=+player.cam_x+GR_WIDTH/2;
  int player_cam_move_y=+player.cam_y+GR_HEIGHT/2;
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
      if (Ground[i]->type==3 || Ground[i]->type==4 || Ground[i]->type==8) { 
	    //c=rgbPaint[Ground[i]->color_id];
        c=rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[i]->color_id];
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,FALSE,0);
        }
      } else if (Ground[i]->type==5 || Ground[i]->type==6) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
              int texture_type=Ground[i]->font_size;
              if (texture_type>=0 && texture_type<PLATFORM_TEXTURES_NUM) {
              DrawTexturedTriangle(hdc,hdc2,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,
                GamePlatformTextures[texture_type].palette_sprite);
              }
        }
      } else if (Ground[i]->type==7) {
	    //c=rgbPaint[Ground[i]->color_id];
        c=rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[i]->color_id];
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,TRUE,HS_CROSS);
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,TRUE,HS_DIAGCROSS);
        }
      }
    } 
  }


  //Draw type 0
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
    if (Ground[i]->type==0) { //draw ground type 1
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          
          GrLine(hdc,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
                Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
                Ground[i]->x2+player.cam_x+GR_WIDTH/2,
                Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
                //rgbPaint[Ground[i]->color_id]);
                rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[i]->color_id]);
      }
    }
    }
  }




  //Draw type 2 == Text
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
    if (Ground[i]->type==2) { 
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	    GrPrintW(hdc,
            Ground[i]->x1+player.cam_x+GR_WIDTH/2,
            Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
            Ground[i]->text,
            "",
//            rgbPaint[Ground[i]->color_id],
            rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[i]->color_id],
            //16,
            Ground[i]->font_size,
            FALSE,
            yes_unifont);
      }
    }
    }
  }

  //Print ground details
  char print_ground_id[4];
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];

    if (i!=-1) {

      c=Highlight((i==MapEditor.selected_ground_id),BLACK,LTPURPLE);
      sprintf(print_ground_id,"%d",i);
      GrPrintThick(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y-16,print_ground_id,c,BLACK);

      c=Highlight((MapEditor.selected_ground_pivot==0 && i==MapEditor.selected_ground_id),BLACK,LTPURPLE);
      GrCircle(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y,6,c,-1);
      c=Highlight((MapEditor.selected_ground_pivot==1 && i==MapEditor.selected_ground_id),BLACK,LTPURPLE);
      GrCircle(hdc,Ground[i]->x2+player_cam_move_x,Ground[i]->y2+player_cam_move_y,6,c,-1);
      if (Ground[i]->type==3 || Ground[i]->type==1) {
        c=Highlight((MapEditor.selected_ground_pivot==2 && i==MapEditor.selected_ground_id),BLACK,LTPURPLE);
        GrCircle(hdc,Ground[i]->x3+player_cam_move_x,Ground[i]->y3+player_cam_move_y,6,c,-1);
      }

      if (i==MapEditor.selected_ground_id) {
        switch (MapEditor.selected_ground_pivot) {
          case 0:
            c=Highlight((MapEditor.selected_ground_pivot==0),BLACK,LTPURPLE);
            GrCircle(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y,8,c,-1);
            break;
          case 1:
            c=Highlight((MapEditor.selected_ground_pivot==1),BLACK,LTPURPLE);
            GrCircle(hdc,Ground[i]->x2+player_cam_move_x,Ground[i]->y2+player_cam_move_y,8,c,-1);
            break;
          case 2:
            c=Highlight((MapEditor.selected_ground_pivot==2),BLACK,LTPURPLE);
            GrCircle(hdc,Ground[i]->x3+player_cam_move_x,Ground[i]->y3+player_cam_move_y,8,c,-1);
            break;
        }
      }
    }
  }

}


void DrawMapEditorBackground(HDC hdc,HDC hdc2)
{
  DrawBackground(hdc,hdc2);
}

void DrawEnemyAntannae(HDC hdc, int le_x, int le_y,int ac,bool flip_bool)
{
     int al=44;
     const int al2=40;
     float ang_offset=0.09;
     const float ang_offset2=0.1;
     float moving_ang_offset=0.00;

     float l_angle=-enemy_rotated_angle_arr[8];


      if (flip_bool) {
        l_angle=-l_angle+ang_offset+M_PI;
      } else {
        l_angle-=ang_offset;
      }

    moving_ang_offset=30*0.005;
    int ax1=le_x+cos(l_angle)*(al);
    int ay1=le_y+sin(l_angle)*(al);
    int ax2_1=ax1+cos(l_angle+moving_ang_offset)*115;
    int ay2_1=ay1+sin(l_angle+moving_ang_offset)*115;
    int ax2_2=ax1+cos(l_angle+0.05-moving_ang_offset)*115;
    int ay2_2=ay1+sin(l_angle+0.05-moving_ang_offset)*115;
    GrLine(hdc,ax1,ay1,ax2_1,ay2_1,ac);
    GrLine(hdc,ax1,ay1,ax2_2,ay2_2,ac); 

}

void DrawMapEditorEnemy(HDC hdc,HDC hdc2)
{
  int c;
  char txt_i[4];
  for (int i=0;i<ENEMY_NUM;i++) {
    int type=MEEnemy[i]->type;
    sprintf(txt_i,"%d",i);
    c=Highlight((i==MapEditor.selected_enemy_id && MapEditor.selected_option==2),rgbPaint[set_enemy_type_color[type]],LTPURPLE);
    if (set_enemy_type_species[type]==1 || set_enemy_type_species[type]==3)
      GrPrintThick(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-64+GR_HEIGHT/2,txt_i,c,BLACK);
    else
      GrPrintThick(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-32+GR_HEIGHT/2,txt_i,c,BLACK);


    //GrSprite(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,MEEnemySprite[type]->sprite_1,FALSE);
    if (set_enemy_type_species[type]!=6) {
      DrawSprite(hdc,hdc2,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,&MEEnemySprite[type]->draw_sprite_1,FALSE);
    } else {
      DrawSprite(hdc,hdc2,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,&MEEnemySprite[type]->draw_sprite_1,TRUE);
    }

    if (set_enemy_type_species[type]==1) { //cockroach draw antannae
      DrawEnemyAntannae(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,rgbPaint[set_enemy_type_color[type]],FALSE);
    }
  }
}



void DrawMapEditorPlayer(HDC hdc,HDC hdc2)
{
  //GrSprite(hdc,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,player.sprite_1_cache,FALSE);
  DrawSprite(hdc,hdc2,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,&PlayerSprite[0].sprite_1[16],FALSE);

  if (MapEditor.flag_enemy_palette_i!=-1) {
    BitmapPalette(hdc,hdc2,MEEnemySprite[MapEditor.flag_enemy_palette_i]->draw_sprite_1.sprite_paint,MEEnemySprite[MapEditor.flag_enemy_palette_i]->enemyPalette);
    MapEditor.flag_enemy_palette_i=-1;
  }

  if (!MapEditor.init_palettes) {
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      BitmapPalette(hdc,hdc2,MEEnemySprite[i]->draw_sprite_1.sprite_paint,MEEnemySprite[i]->enemyPalette);
    }
    MapEditor.init_palettes=TRUE;
  }
}



void DrawMapEditorUI(HDC hdc,HDC hdc2)
{
  //Print cursor Axis
  char axis_x[16];
  char axis_y[16];
  sprintf(axis_x,"cam_x:%1.0f",player.cam_move_x);
  sprintf(axis_y,"cam_y:%1.0f",player.cam_move_y);
  GrPrintThick(hdc,mouse_x,mouse_y+32+8,axis_x,WHITE,BLACK);
  //GrPrintThick(hdc,mouse_x+1,mouse_y+32+9,axis_x,WHITE);

  GrPrintThick(hdc,mouse_x,mouse_y+50+8,axis_y,WHITE,BLACK);
  //GrPrintThick(hdc,mouse_x+1,mouse_y+50+9,axis_y,WHITE);

  sprintf(axis_x,"cursor_x:%d",MapEditor.cursor_x);
  sprintf(axis_y,"cursor_y:%d",MapEditor.cursor_y);
  GrPrintThick(hdc,mouse_x,mouse_y+68+8,axis_x,WHITE,BLACK);
  //GrPrintThick(hdc,mouse_x+1,mouse_y+68+9,axis_x,WHITE);
  GrPrintThick(hdc,mouse_x,mouse_y+82+8,axis_y,WHITE,BLACK);
  //GrPrintThick(hdc,mouse_x+1,mouse_y+82+9,axis_y,WHITE);


  sprintf(axis_y,"sticky:%d",MapEditor.sticky_level);
  GrPrintThick(hdc,mouse_x,mouse_y+100+8,axis_y,GREEN,BLACK);

  sprintf(axis_x,"fg_x:%d",mouse_x-GR_WIDTH/2+FGROUND_SIZE/2);
  sprintf(axis_y,"fg_y:%d",mouse_y-GR_HEIGHT/2+FGROUND_SIZE/2);
  GrPrintThick(hdc,mouse_x,mouse_y+116+8,axis_x,WHITE,BLACK);
  GrPrintThick(hdc,mouse_x,mouse_y+132+8,axis_y,WHITE,BLACK);

  int c;
  if (level_loaded) {



    switch (MapEditor.selected_option) {
      case 0: //move ground
        c = Highlight((MapEditor.selected_ground_option==0),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,16,"GROUNDS:",c,BLACK);
        char print_ground_id[16];
        sprintf(print_ground_id,"<%d>  {%d}",MapEditor.selected_ground_id,MapEditor.clipboard_ground_id);
        GrPrintThick(hdc,8*11,16,print_ground_id,c,BLACK);


        //type
        c = Highlight((MapEditor.selected_ground_option==1),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,34,"Type:",c,BLACK);
        char print_ground_type[16];
        sprintf(print_ground_id,"<%d>  {%d}",Ground[MapEditor.selected_ground_id]->type,Ground[MapEditor.clipboard_ground_id]->type);
        GrPrintThick(hdc,8*11,34,print_ground_id,c,BLACK);

        //color
        c = Highlight((MapEditor.selected_ground_option==2),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,52,"Color:",c,BLACK);
        GrPrintThick(hdc,8*11,52,"[      ]",c,BLACK);
        GrPrintThick(hdc,8*15,52,"{     }",c,BLACK);
        GrPrintThick(hdc,8*19,52,"(      )",c,BLACK);

        //if (Ground[MapEditor.selected_ground_id]->color_id!=0) {
          //GrRect(hdc,8*12+1,52,16,16,BLACK);
        //} else {
          //GrRect(hdc,8*12+1,52,16,16,WHITE);
        //}

        GrRect(hdc,8*12+1,52,16,16,WHITE);
        GrRect(hdc,8*12+2+1,52+2,12,12,rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[MapEditor.selected_ground_id]->color_id]);//rgbPaint[Ground[MapEditor.selected_ground_id]->color_id]);

        GrRect(hdc,8*16,52,16,16,WHITE);
        GrRect(hdc,8*16+2,52+2,12,12,rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][Ground[MapEditor.clipboard_ground_id]->color_id]);//rgbPaint[Ground[MapEditor.clipboard_ground_id]->color_id]);

        GrRect(hdc,8*20,52,16,16,WHITE);
        GrRect(hdc,8*20+2,52+2,12,12,rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][MapEditor.clipboard_ground_color_id]);//rgbPaint[MapEditor.clipboard_ground_color_id]);

        if (color_chooser.is_choosing_color) {
          DrawPaintSquare(hdc,8*12+2+1+64,52+2,color_chooser.color_id,color_chooser.color_id_choosing);
        }


        //is_ghost
        c = Highlight((MapEditor.selected_ground_option==3),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,70,"IsGhost:",c,BLACK);
        if (Ground[MapEditor.selected_ground_id]->is_ghost) {
          GrPrintThick(hdc,8*11,70,"<TRUE>",c,BLACK);
        } else {
          GrPrintThick(hdc,8*11,70,"<FALSE>",c,BLACK);
        }
        if (Ground[MapEditor.clipboard_ground_id]->is_ghost) {
          GrPrintThick(hdc,8*19,70,"{TRUE}",c,BLACK);
        } else {
          GrPrintThick(hdc,8*19,70,"{FALSE}",c,BLACK);
        }

        //font size
        c = Highlight((MapEditor.selected_ground_option==4),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,86,"Font Size:",c,BLACK);
        char print_ground_font_size[16];
        sprintf(print_ground_font_size,"<%d>  {%d}",Ground[MapEditor.selected_ground_id]->font_size,Ground[MapEditor.clipboard_ground_id]->font_size);
        GrPrintThick(hdc,8*11,86,print_ground_font_size,c,BLACK);
        

        //ground text
        wchar_t duplicate_txt_visual[513];
        if (Ground[MapEditor.selected_ground_id]->type==2 && !MapEditor.is_typing_search) { //Ground text, show gui
          if (!MapEditor.is_ground_txt_typing) {
            GrPrintThick(hdc,8,86+16*2,"[Enter]: Begin Typing",GREEN,BLACK);
          } else {
            GrRect(hdc,0,86+16*2,GR_WIDTH,GR_HEIGHT-86-16*2,BLACK);
            GrPrintThick(hdc,8,86+16*2,"[ESC]: Exit and Save.  [SHIFT_ESC]: Abort.  [BACKSPACE]: Backspace",GREEN,BLACK);
          }
          
          if (frame_tick%FPS>10) {
            //swprintf(duplicate_txt_visual,MapEditor.typing_ground_txt_pos+2,L"%ls*",MapEditor.typing_ground_txt);
            for (int i=0;i<MapEditor.typing_ground_txt_pos;i++) {
              duplicate_txt_visual[i]=MapEditor.typing_ground_txt[i];
            }
            duplicate_txt_visual[MapEditor.typing_ground_txt_pos]='_';
            for (int i=MapEditor.typing_ground_txt_pos+1;i<513;i++) {
              duplicate_txt_visual[i]=0;
            }
            GrPrintW(hdc,8,94+16*3,duplicate_txt_visual,"",GREEN,16,FALSE,yes_unifont);
          }
          GrPrintW(hdc,8,94+16*3,MapEditor.typing_ground_txt,"",WHITE,16,FALSE,yes_unifont);
        }
        break;


      case 1: //move player
        //c = Highlight((MapEditor.selected_ground_option==0),BLACK,LTPURPLE);
        GrPrintThick(hdc,8,16,"PLAYER",YELLOW,BLACK);
        break;


      case 2: //move enemy
        c = Highlight((MapEditor.selected_enemy_option==0),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,16,"ENEMY:",c,BLACK);
        char print_enemy_id[16];
        sprintf(print_enemy_id,"<%d>  {%d}",MapEditor.selected_enemy_id,MapEditor.clipboard_enemy_id);
        GrPrintThick(hdc,8*11,16,print_enemy_id,c,BLACK);



        c = Highlight((MapEditor.selected_enemy_option==1),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,34,"Type:",c,BLACK);
        char print_enemy_type[16];
        sprintf(print_enemy_type,"<%d>  {%d}",MEEnemy[MapEditor.selected_enemy_id]->type,MEEnemy[MapEditor.clipboard_enemy_id]->type);
        GrPrintThick(hdc,8*11,34,print_enemy_type,c,BLACK);
        break;

      case 3: //set enemy type
//        GrPrintThick(hdc,8,16,"ENEMY TYPE:",BLACK);
        {
        c = Highlight((MapEditor.selected_enemy_type_option==0),WHITE,LTPURPLE);
        GrPrintThick(hdc,8,16,"ENEMY TYPE:",c,BLACK);
        char print_enemy_type_id[16];
        sprintf(print_enemy_type_id,"<%d>  {%d}",MapEditor.selected_enemy_type_id,MapEditor.clipboard_enemy_type_id);
        GrPrintThick(hdc,8*13,16,print_enemy_type_id,c,BLACK);



        char print_enemy_type_int_attr[40];
        for (int i=0;i<ENEMY_TYPE_INT_ATTR_NUM;i++) {
          c = Highlight((MapEditor.selected_enemy_type_option==i+1),WHITE,LTPURPLE);
          if (i!=4 && i!=16) {
            sprintf(print_enemy_type_int_attr,"%s <%d>  {%d}",enemy_type_int_attr_names[i],set_enemy_type_int_attr[i][MapEditor.selected_enemy_type_id],set_enemy_type_int_attr[i][MapEditor.clipboard_enemy_type_id]);            
            GrPrintThick(hdc,8,32+16*i,print_enemy_type_int_attr,c,BLACK);
          } else {

            GrRect(hdc,8*14+1,32+16*i,16,16,WHITE);
            GrRect(hdc,8*18,32+16*i,16,16,WHITE);
            int di=MapEditor.selected_enemy_type_option-1;
            if (i==4) {
              GrRect(hdc,8*14+2+1,32+16*i+2,12,12,rgbPaint[set_enemy_type_color[MapEditor.selected_enemy_type_id]]);
              GrPrintThick(hdc,8,32+16*i,"Enemy Color:",c,BLACK);
              GrPrintThick(hdc,8*13,32+16*i,"[      ]",c,BLACK);

              GrRect(hdc,8*18+2,32+16*i+2,12,12,rgbPaint[set_enemy_type_color[MapEditor.clipboard_enemy_type_id]]);
              GrPrintThick(hdc,8*17,32+16*i,"{     }",c,BLACK);
            } else {
              GrRect(hdc,8*14+2+1,32+16*i+2,12,12,rgbPaint[set_enemy_type_bullet_color[MapEditor.selected_enemy_type_id]]);
              GrPrintThick(hdc,8,32+16*i,"Bullet Color:",c,BLACK);
              GrPrintThick(hdc,8*13,32+16*i,"[      ]",c,BLACK);

              GrRect(hdc,8*18+2,32+16*i+2,12,12,rgbPaint[set_enemy_type_bullet_color[MapEditor.clipboard_enemy_type_id]]);
              GrPrintThick(hdc,8*17,32+16*i,"{     }",c,BLACK);
            }

            if (color_chooser.is_choosing_color) {
              DrawPaintSquare(hdc,8*24+2+1+64,32+16*di,color_chooser.color_id,color_chooser.color_id_choosing);
            }
          }
        }


        for (int i=0;i<ENEMY_TYPE_FLOAT_ATTR_NUM;i++) {
          c = Highlight((MapEditor.selected_enemy_type_option==i+ENEMY_TYPE_INT_ATTR_NUM+1),WHITE,LTPURPLE);
          char print_enemy_type_float_attr[32];
          sprintf(print_enemy_type_float_attr,"%s <%1.1f>  {%1.1f}",enemy_type_float_attr_names[i],set_enemy_type_float_attr[i][MapEditor.selected_enemy_type_id],set_enemy_type_float_attr[i][MapEditor.clipboard_enemy_type_id]);
          GrPrintThick(hdc,8,32+16*ENEMY_TYPE_INT_ATTR_NUM+16*i,print_enemy_type_float_attr,c,BLACK);
        }


        for (int i=0;i<ENEMY_TYPE_BOOL_ATTR_NUM;i++) {
          c = Highlight((MapEditor.selected_enemy_type_option==i+ENEMY_TYPE_FLOAT_ATTR_NUM+ENEMY_TYPE_INT_ATTR_NUM+1),WHITE,LTPURPLE);
          char print_enemy_type_bool_attr[32];
          sprintf(print_enemy_type_bool_attr,"%s <%d>  {%d}",enemy_type_bool_attr_names[i],set_enemy_type_bool_attr[i][MapEditor.selected_enemy_type_id],set_enemy_type_bool_attr[i][MapEditor.clipboard_enemy_type_id]);
          GrPrintThick(hdc,8,32+16*(ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_FLOAT_ATTR_NUM)+16*i,print_enemy_type_bool_attr,c,BLACK);
        }
        

        //Draw Circle Follow Range
        GrCircle(hdc,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,NODE_SIZE*set_enemy_type_follow_range[MapEditor.selected_enemy_type_id]/2,LTGREEN,-1);
        GrLine(hdc,MapEditor.demo_enemy_spritex+NODE_SIZE*set_enemy_type_follow_range[MapEditor.selected_enemy_type_id]/2,MapEditor.demo_enemy_spritey-8,
                   MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey-8,LTGREEN);

        //Draw Circle Chase Range
        GrLine(hdc,MapEditor.demo_enemy_spritex+NODE_SIZE*set_enemy_type_chase_range[MapEditor.selected_enemy_type_id]/2,MapEditor.demo_enemy_spritey+8,
                   MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey+8,LTRED);
        GrCircle(hdc,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,NODE_SIZE*set_enemy_type_chase_range[MapEditor.selected_enemy_type_id]/2,LTRED,-1);

        //Draw Circle Unchase Range
        GrLine(hdc,MapEditor.demo_enemy_spritex+NODE_SIZE*set_enemy_type_unchase_range[MapEditor.selected_enemy_type_id]/2,MapEditor.demo_enemy_spritey,
                   MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,LTBLUE);
        GrCircle(hdc,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,NODE_SIZE*set_enemy_type_unchase_range[MapEditor.selected_enemy_type_id]/2,LTBLUE,-1);

        //Draw 
        DrawSprite(hdc,hdc2,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,&MEEnemySprite[MapEditor.selected_enemy_type_id]->draw_sprite_1,MapEditor.demo_enemy_spriteisleft);
        if (set_enemy_type_species[MapEditor.selected_enemy_type_id]==1) {
          DrawEnemyAntannae(hdc,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey, rgbPaint[set_enemy_type_color[MapEditor.selected_enemy_type_id]],MapEditor.demo_enemy_spriteisleft);
        }


        //draw bullet frequency
        GrLine(hdc,MapEditor.demo_enemy_spritex+NODE_SIZE*set_enemy_type_shoot_at_player_range[MapEditor.selected_enemy_type_id]/2,MapEditor.demo_enemy_spritey+24,
                   MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey+24,LTPURPLE);
        GrCircle(hdc,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,NODE_SIZE*set_enemy_type_shoot_at_player_range[MapEditor.selected_enemy_type_id]/2,LTPURPLE,-1);
        for (int i=0;i<ENEMY_BULLET_NUM;i++) {
          if (Bullet[i].shot) {
            DrawBullet2(hdc,hdc2,i,Bullet[i].x,Bullet[i].y,Bullet[i].color);
          }
        }
        }
        break;

      case 4: //set map background and palette
        {
        GrPrintThick(hdc,8,16,"LEVEL ENVIRONMENT",YELLOW,BLACK);

         //Draw Weather Gradient Lines
        //GrLine(hdc,320,16*8,320+MapEditor.set_lvl_ambient_val[5],16*8+MapEditor.set_lvl_ambient_val[4],BLUE);
        //GrLine(hdc,320,16*17,320+MapEditor.set_lvl_ambient_val[8],16*17+MapEditor.set_lvl_ambient_val[7],LTGRAY);
        char melvlambienttxt[32];
        for (int i=0;i<S_LVL_ATTR_NUM;i++) {
          c = Highlight(MapEditor.selected_bg_attr_option==i,WHITE,LTPURPLE);
          switch (MELVL_bgattr_type[i]) {
             case 0: {//int
               int int_val=*(int*)MELVL_bgattr_ptr[i];
               switch (i) {
                 case 9: //Eclipse Type
                   sprintf(melvlambienttxt,"%s: <%s>",melvlattrtxt_arr[i],melvlattrtxt_eclipse_arr[int_val]);
                   GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                   break;
                 case 10: //Cloud Type
                   sprintf(melvlambienttxt,"%s: <%s>",melvlattrtxt_arr[i],melvlattrtxt_cloud_arr[int_val]);
                   GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                   break;
                 case 13: //Weather Type
                   sprintf(melvlambienttxt,"%s: <%s>",melvlattrtxt_arr[i],melvlattrtxt_weather_arr[int_val]);
                   GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                   break;
                 case 16:
                    sprintf(melvlambienttxt,"%s: <%s>",melvlattrtxt_arr[i],melvlattrtxt_brightness_type_arr[int_val]);
                    GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                    break;
                 case 17:
                    sprintf(melvlambienttxt,"%s: <%s>",melvlattrtxt_arr[i],melvlattrtxt_darkness_lvl_arr[int_val]);
                    GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                    break;
                 default: //numerical values default              
                   sprintf(melvlambienttxt,"%s: <%d>",melvlattrtxt_arr[i],int_val);
                   GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                   break;
                 case 11: //paint
                 case 12: //paint
                   sprintf(melvlambienttxt,"%s: [      ] ",melvlattrtxt_arr[i]);
                   GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
                   if (i==11) {
                     GrRect(hdc,8*19,16*(i+2)+1,16,16,WHITE);
                     GrRect(hdc,8*19+2,16*(i+2)+3,12,12,rgbPaint[int_val]);
                   } else {
                     GrRect(hdc,8*20,16*(i+2)+1,16,16,WHITE);
                     GrRect(hdc,8*20+2,16*(i+2)+3,12,12,rgbPaint[int_val]);
                   }
                   if (color_chooser.is_choosing_color && MapEditor.selected_bg_attr_option==i) {
                     DrawPaintSquare(hdc,8*25,16*(i+2),color_chooser.color_id,color_chooser.color_id_choosing);
                   }
                   break;
                 }
               }
               break;
             case 1: { //bool
               bool bool_val=*(bool*)MELVL_bgattr_ptr[i];
               if (!bool_val) {
                 sprintf(melvlambienttxt,"%s: <FALSE>",melvlattrtxt_arr[i]);
               } else {
                 sprintf(melvlambienttxt,"%s: <TRUE>",melvlattrtxt_arr[i]);
               }
               GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
               }
               break;
             case 2: { //int64_t
               int64_t int64_val=*(int64_t*)MELVL_bgattr_ptr[i];
               sprintf(melvlambienttxt,"%s: <%lld>",melvlattrtxt_arr[i],int64_val);
               GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
               }
               break;
             case 3: {//float
               float float_val=*(float*)MELVL_bgattr_ptr[i];
               sprintf(melvlambienttxt,"%s: <%5.4f>",melvlattrtxt_arr[i],float_val);
               GrPrintThick(hdc,8,16*(i+2),melvlambienttxt,c,BLACK);
               }
               break;
          }
        }
        }
        break;


      case 5: {//set platform textures
        char txt[24];
        GrPrintThick(hdc,8,16,"PLATFORM TEXTURE",YELLOW,BLACK);
        for (int i=0;i<4;i++) {
          c = Highlight(MapEditor.selected_ptexture_option==i,WHITE,LTPURPLE);
          switch (i) {
            case 0:
              sprintf(txt,"ID: <%d>",MapEditor.selected_ptexture_id);
              GrPrintThick(hdc,8,32+16*i,txt,c,BLACK);
              break;
            case 1:
              sprintf(txt,"Type: <%d>",GamePlatformTextures[MapEditor.selected_ptexture_id].type);
              GrPrintThick(hdc,8,32+16*i,txt,c,BLACK); 
              break;
            case 2:
              sprintf(txt,"Saturation: <%d>",GamePlatformTextures[MapEditor.selected_ptexture_id].solid_value);
              GrPrintThick(hdc,8,32+16*i,txt,c,BLACK); 
              break;
            case 3: 
              GrPrintThick(hdc,8,32+16*i,"Color:",c,BLACK); 
              GrPrintThick(hdc,8*11,32+16*i,"[      ]",c,BLACK);
              GrRect(hdc,8*12+1,32+16*i+2,16,16,WHITE);
              GrRect(hdc,8*12+2+1,32+16*i+4,12,12,rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][GamePlatformTextures[MapEditor.selected_ptexture_id].color_id]);//rgbPaint[GamePlatformTextures[MapEditor.selected_ptexture_id].color_id]);
              break;
          }
        }
        if (color_chooser.is_choosing_color) {
          DrawPaintSquare(hdc,8*25,52+2,color_chooser.color_id,color_chooser.color_id_choosing);
        }
        DrawBitmap(hdc,hdc2,16,32+16*5,0,0,VGRID_SIZE,VGRID_SIZE,GamePlatformTextures[MapEditor.selected_ptexture_id].palette_sprite,SRCCOPY,FALSE,FALSE); //Draw Game Textures
        DrawBitmap(hdc,hdc2,16,32+16*5+VGRID_SIZE+16,0,0,VGRID_SIZE,VGRID_SIZE,LoadedPlatformTextures[GamePlatformTextures[MapEditor.selected_ptexture_id].type],SRCCOPY,FALSE,FALSE); //Draw Original Textures
        }
        break;


      case 6: { //Set Falling Ground >:D 2026-05-27
        DrawMapEditorFallingGroundEditor(hdc,hdc2);
        if (color_chooser.is_choosing_color) {
          DrawPaintSquare(hdc,GR_WIDTH/2+FGROUND_SIZE/2+8,19*16+32,color_chooser.color_id,color_chooser.color_id_choosing);
        }

        char txt[48];
        GrPrintThick(hdc,8,16,"Falling Ground",YELLOW,BLACK);

        for (int i=0;i<FGROUND_ATTR_NUM+1-3;i++) {
          c = Highlight(MapEditor.selected_fground_option==i,WHITE,LTPURPLE);
          switch (i) {
            case 0:
              sprintf(txt,"Falling Ground ID: <%d>",MapEditor.selected_fground_id);
              break;
            case 1:
              sprintf(txt,"Spin Angle: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ospin_angle/100.0, (float)F_GROUND_CLIPBOARD.ospin_angle/100.0);
              break;
            case 2:
              sprintf(txt,"Spin Angle Delta: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ospin_angle_delta/100.0,(float)F_GROUND_CLIPBOARD.ospin_angle_delta/100.0);
              break;
            case 3:
              sprintf(txt,"Spin Angle Min: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ospin_angle_min/100.0,(float)F_GROUND_CLIPBOARD.ospin_angle_min/100.0);
              break;
            case 4:
              sprintf(txt,"Spin Angle Max: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ospin_angle_max/100.0,(float)F_GROUND_CLIPBOARD.ospin_angle_max/100.0);
              break;
            case 5:
              sprintf(txt,"Y Oscillation Angle Delta: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].oy_oscillation_angle_delta/100.0,(float)F_GROUND_CLIPBOARD.oy_oscillation_angle_delta/100.0);
              break;
            case 6:
              sprintf(txt,"Y Oscillation Angle Max: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].oy_oscillation_angle_max/100.0,(float)F_GROUND_CLIPBOARD.oy_oscillation_angle_max/100.0);
              break;
            case 7:
              sprintf(txt,"Y Oscillation Angle: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].oy_oscillation_angle/100.0,(float)F_GROUND_CLIPBOARD.oy_oscillation_angle/100.0);
              break;
            case 8:
              sprintf(txt,"X Oscillation Angle Delta: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ox_oscillation_angle_delta/100.0,(float)F_GROUND_CLIPBOARD.ox_oscillation_angle_delta/100.0);
              break;
            case 9:
              sprintf(txt,"X Oscillation Angle Max: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ox_oscillation_angle_max/100.0,(float)F_GROUND_CLIPBOARD.ox_oscillation_angle_max/100.0);
              break;
            case 10:
              sprintf(txt,"X Oscillation Angle: <%3.2f> {%3.2f}",(float)F_GROUND[MapEditor.selected_fground_id].ox_oscillation_angle/100.0,(float)F_GROUND_CLIPBOARD.ox_oscillation_angle/100.0);
              break;
            case 11:
              sprintf(txt,"Speed Multiplier: <%d> {%d}",F_GROUND[MapEditor.selected_fground_id].speed_multiplier,F_GROUND_CLIPBOARD.speed_multiplier);
              break;
            case 12:
              sprintf(txt,"Speed: <%1.1f> {%1.1f}",(float)F_GROUND[MapEditor.selected_fground_id].ospeed/10,(float)F_GROUND_CLIPBOARD.ospeed/10);
              break;
            case 13: //Rotational Pivot X,Y (Default is FGROUND_SIZE/FGROUND_SIZE/2)
              sprintf(txt,"Pivot: %1.0f,%1.0f {%1.0f,%1.0f}",(float)F_GROUND[MapEditor.selected_fground_id].opivot_x,(float)F_GROUND_CLIPBOARD.opivot_y,(float)F_GROUND_CLIPBOARD.opivot_x,(float)F_GROUND_CLIPBOARD.opivot_y);
              
              break;
            case 14: //Action on reach end <Jump back to Start / Travel back to Start>
              sprintf(txt,"Reach End Action: <%d> {%d}",F_GROUND[MapEditor.selected_fground_id].oreach_end_type,F_GROUND_CLIPBOARD.oreach_end_type);
              break;
            case 15:
              sprintf(txt,"Dist Begin: %1.0f / %1.0f {%1.0f / %1.0f}",
                    F_GROUND[MapEditor.selected_fground_id].odist_start,F_GROUND[MapEditor.selected_fground_id].travel_dist_max,
                    F_GROUND_CLIPBOARD.odist_start,F_GROUND_CLIPBOARD.travel_dist_max
              );
              break;
            case 16:
              sprintf(txt,"X,Y Start: %1.0f,%1.0f {%1.0f,%1.0f}",
                    F_GROUND[MapEditor.selected_fground_id].x_start,F_GROUND[MapEditor.selected_fground_id].y_start,
                    F_GROUND_CLIPBOARD.x_start,F_GROUND_CLIPBOARD.y_start
              );
              break;
            case 17:
              sprintf(txt,"X,Y End: %1.0f,%1.0f {%1.0f,%1.0f}",
                    F_GROUND[MapEditor.selected_fground_id].x_end,F_GROUND[MapEditor.selected_fground_id].y_end,
                    F_GROUND_CLIPBOARD.x_end,F_GROUND_CLIPBOARD.y_end
              );
              break;
          }          
          GrPrintThick(hdc,8,32+16*i,txt,c,BLACK);
        }



        for (int i=0;i<FGROUND_GROUND_ATTR_NUM+2;i++) {
          c = Highlight(MapEditor.selected_fground_option==(FGROUND_ATTR_NUM+1-3+i),WHITE,LTPURPLE);
          switch (i) {
            case 0:
              sprintf(txt,"FGround-Ground ID: <%d>",MapEditor.selected_fground_ground_id);
              break;
            case 1:
              sprintf(txt,"Is Ghost: <%d> {%d}",F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].is_ghost,f2ground_clipboard.is_ghost);
              break;
            case 2:
              sprintf(txt,"Color:");
              GrPrintThick(hdc,8*8,64+16*17+i*16,"[      ]",c,BLACK);

              //GrRect(hdc,8*13,32+16*i+2,12,12,rgbPaint[F_GROUND[MapEditor.selected_fground_id].color_id[MapEditor.selected_fground_ground_id]]);
              //GrPrintThick(hdc,8*17,32+16*i,"{     }",c,BLACK);

              GrRect(hdc,8*9+1,64+16*17+i*16,16,16,WHITE);
              GrRect(hdc,8*9+2+1,64+16*17+i*16+2,12,12,rgbPaintBrightness[MapEditor.bg_attr_dark_lvl][F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].color_id]);

              break;
            case 3:
              sprintf(txt,"Type: <%d> {%d}",F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].type,f2ground_clipboard.type);
              break;
            case 4:
              sprintf(txt,"Texture Type: <%d> {%d}",F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].texture_type,f2ground_clipboard.texture_type);
              break;
            case 5:
              sprintf(txt,"xy1:%1.0f,%1.0f {%1.0f,%1.0f}",F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].ox1,F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].oy1,
                                                          f2ground_clipboard.ox1,f2ground_clipboard.oy1);
              break;
            case 6:
              sprintf(txt,"xy2:%1.0f,%1.0f {%1.0f,%1.0f}",F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].ox2,F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].oy2,
                                                          f2ground_clipboard.ox2,f2ground_clipboard.oy2);
              break;
            case 7:
              sprintf(txt,"xy3:%1.0f,%1.0f {%1.0f,%1.0f}",F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].ox3,F_GROUND[MapEditor.selected_fground_id].f2ground[MapEditor.selected_fground_ground_id].oy3,
                                                          f2ground_clipboard.ox3,f2ground_clipboard.oy3);
              break;

            case 8:
              //<Mirror Sprite Horizontally X>
              sprintf(txt,"<Flip Horizontally (X)>");
              break;

            case 9:
              //<Mirror Sprite Vertically Y>
              sprintf(txt,"<Flip Vertically (Y)>");
              break;

            case 10:
              //<Move sprite X>
              sprintf(txt,"<Shift Horizontally (X)>");
              break;

            case 11:
              //<Move Sprite Y>
              sprintf(txt,"<Shift Vertically (Y)>");
              break;

          }          
          GrPrintThick(hdc,8,64+16*17+i*16,txt,c,BLACK);
        }
        break;
      }
    }

    if (MapEditor.selected_option==0 || MapEditor.selected_option==2) {
      char print_search_id[8];
      if (!MapEditor.is_typing_search) {
        if (!MapEditor.is_ground_txt_typing)
          GrPrintThick(hdc,8,102,"[CTRL+F]: Find",GREEN,BLACK);
      } else {
        GrRect(hdc,0,86,8*40,102-16*3,BLACK);
        GrPrintThick(hdc,8,86,"[CTRL+F]: Abort.  [Enter]: Search.",GREEN,BLACK);
        sprintf(print_search_id,"Find: %d",MapEditor.typing_search_id);
        GrPrintThick(hdc,8,86+16,print_search_id,GREEN,BLACK);
      }
    }
  }
}
