
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

char *melvlambienttxt_arr[9]=
{
"Background Type",
"Background Color",
"Has Moon",
"Has Rain",
"Rain Rise",
"Rain Run",
"Has Shadow",
"Shadow Rise",
"Shadow Run"
};

char *enemy_type_double_attr_names[ENEMY_TYPE_DOUBLE_ATTR_NUM]=
{
"Speed",
"Bullet Speed"
};

char *enemy_type_bool_attr_names[ENEMY_TYPE_BOOL_ATTR_NUM]=
{
"Timebreaker Immune"
};


void DrawMapEditorWaterPlatforms(HDC hdc)
{
  //Draw type 1 == Water
  int i,c;
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
      if (Ground[i]->type==1) {
        c=rgbPaint[Ground[i]->color_id];
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
void DrawMapEditorPlatforms(HDC hdc)
{
  //Draw type 3
  int c;
  int i;
  int player_cam_move_x=+player.cam_x+GR_WIDTH/2;
  int player_cam_move_y=+player.cam_y+GR_HEIGHT/2;
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
      if (Ground[i]->type==3) { 
	    c=rgbPaint[Ground[i]->color_id];
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
                rgbPaint[Ground[i]->color_id]);

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
            rgbPaint[Ground[i]->color_id],
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
      GrPrint(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y-16,print_ground_id,c);

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
  switch (MapEditor.set_lvl_ambient_val[0]) {
    case 0:
      DrawBitmap(hdc,hdc2,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      break;
    case 1:
      DrawBitmap(hdc,hdc2,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      break;
    default:
      if (map_background_sprite==NULL) {
        GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,rgbPaint[MapEditor.set_lvl_ambient_val[1]]);
      } else {
        DrawBitmap(hdc,hdc2,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      }
      break;
  }

  if (MapEditor.set_lvl_ambient_val[2]==1) {
    DrawSprite(hdc, hdc2,GR_WIDTH-128,128,&draw_moon_sprite,FALSE);
  }
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
      GrPrint(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-64+GR_HEIGHT/2,txt_i,c);
    else
      GrPrint(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-32+GR_HEIGHT/2,txt_i,c);


    //GrSprite(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,MEEnemySprite[type]->sprite_1,FALSE);
    if (set_enemy_type_species[type]!=6) {
      DrawSprite(hdc,hdc2,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,&MEEnemySprite[type]->draw_sprite_1,FALSE);
    } else {
      DrawSprite(hdc,hdc2,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,&MEEnemySprite[type]->draw_sprite_1,TRUE);
    }
  }
}



void DrawMapEditorPlayer(HDC hdc,HDC hdc2)
{
  //GrSprite(hdc,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,player.sprite_1_cache,FALSE);
  DrawSprite(hdc,hdc2,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,&player.draw_sprite_1,FALSE);
}



void DrawMapEditorUI(HDC hdc,HDC hdc2)
{
  //Print cursor Axis
  char axis_x[16];
  char axis_y[16];
  sprintf(axis_x,"cam_x:%1.0f",player.cam_move_x);
  sprintf(axis_y,"cam_y:%1.0f",player.cam_move_y);
  GrPrint(hdc,mouse_x,mouse_y+32+8,axis_x,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+32+9,axis_x,WHITE);

  GrPrint(hdc,mouse_x,mouse_y+50+8,axis_y,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+50+9,axis_y,WHITE);

  sprintf(axis_x,"cursor_x:%d",MapEditor.cursor_x);
  sprintf(axis_y,"cursor_y:%d",MapEditor.cursor_y);
  GrPrint(hdc,mouse_x,mouse_y+68+8,axis_x,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+68+9,axis_x,WHITE);
  GrPrint(hdc,mouse_x,mouse_y+82+8,axis_y,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+82+9,axis_y,WHITE);


  sprintf(axis_y,"sticky:%d",MapEditor.sticky_level);
  GrPrint(hdc,mouse_x,mouse_y+100+8,axis_y,GREEN);


  int c;
  if (level_loaded) {



    switch (MapEditor.selected_option) {
      case 0: //move ground
        c = Highlight((MapEditor.selected_ground_option==0),BLACK,LTPURPLE);
        GrPrint(hdc,8,16,"GROUNDS:",c);
        char print_ground_id[16];
        sprintf(print_ground_id,"<%d>  {%d}",MapEditor.selected_ground_id,MapEditor.clipboard_ground_id);
        GrPrint(hdc,8*11,16,print_ground_id,c);


        //type
        c = Highlight((MapEditor.selected_ground_option==1),BLACK,LTPURPLE);
        GrPrint(hdc,8,34,"Type:",c);
        char print_ground_type[16];
        sprintf(print_ground_id,"<%d>  {%d}",Ground[MapEditor.selected_ground_id]->type,Ground[MapEditor.clipboard_ground_id]->type);
        GrPrint(hdc,8*11,34,print_ground_id,c);

        //color
        c = Highlight((MapEditor.selected_ground_option==2),BLACK,LTPURPLE);
        GrPrint(hdc,8,52,"Color:",c);
        GrPrint(hdc,8*11,52,"[      ]",c);
        GrPrint(hdc,8*15,52,"{     }",c);
        GrPrint(hdc,8*19,52,"(      )",c);

        //if (Ground[MapEditor.selected_ground_id]->color_id!=0) {
          //GrRect(hdc,8*12+1,52,16,16,BLACK);
        //} else {
          //GrRect(hdc,8*12+1,52,16,16,WHITE);
        //}

        GrRect(hdc,8*12+1,52,16,16,WHITE);
        GrRect(hdc,8*12+2+1,52+2,12,12,rgbPaint[Ground[MapEditor.selected_ground_id]->color_id]);

        GrRect(hdc,8*16,52,16,16,WHITE);
        GrRect(hdc,8*16+2,52+2,12,12,rgbPaint[Ground[MapEditor.clipboard_ground_id]->color_id]);

        GrRect(hdc,8*20,52,16,16,WHITE);
        GrRect(hdc,8*20+2,52+2,12,12,rgbPaint[MapEditor.clipboard_ground_color_id]);

        if (color_chooser.is_choosing_color) {
          DrawPaintSquare(hdc,8*12+2+1+64,52+2,color_chooser.color_id,color_chooser.color_id_choosing);
        }


        //is_ghost
        c = Highlight((MapEditor.selected_ground_option==3),BLACK,LTPURPLE);
        GrPrint(hdc,8,70,"IsGhost:",c);
        if (Ground[MapEditor.selected_ground_id]->is_ghost) {
          GrPrint(hdc,8*11,70,"<TRUE>",c);
        } else {
          GrPrint(hdc,8*11,70,"<FALSE>",c);
        }
        if (Ground[MapEditor.clipboard_ground_id]->is_ghost) {
          GrPrint(hdc,8*19,70,"{TRUE}",c);
        } else {
          GrPrint(hdc,8*19,70,"{FALSE}",c);
        }

        //font size
        c = Highlight((MapEditor.selected_ground_option==4),BLACK,LTPURPLE);
        GrPrint(hdc,8,86,"Font Size:",c);
        char print_ground_font_size[16];
        sprintf(print_ground_font_size,"<%d>  {%d}",Ground[MapEditor.selected_ground_id]->font_size,Ground[MapEditor.clipboard_ground_id]->font_size);
        GrPrint(hdc,8*11,86,print_ground_font_size,c);
        

        //ground text
        wchar_t duplicate_txt_visual[513];
        if (Ground[MapEditor.selected_ground_id]->type==2 && !MapEditor.is_typing_search) { //Ground text, show gui
          if (!MapEditor.is_ground_txt_typing) {
            GrPrint(hdc,8,86+16*2,"[Enter]: Begin Typing",GREEN);
          } else {
            GrRect(hdc,0,86+16*2,GR_WIDTH,GR_HEIGHT-86-16*2,BLACK);
            GrPrint(hdc,8,86+16*2,"[ESC]: Exit and Save.  [SHIFT_ESC]: Abort.  [BACKSPACE]: Backspace",GREEN);
          }
          
          if (frame_tick%FPS>10) {
            //swprintf(duplicate_txt_visual,MapEditor.typing_ground_txt_pos+2,L"%s*",MapEditor.typing_ground_txt);
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
        GrPrint(hdc,8,16,"PLAYER",BLACK);
        break;


      case 2: //move enemy
        c = Highlight((MapEditor.selected_enemy_option==0),BLACK,LTPURPLE);
        GrPrint(hdc,8,16,"ENEMY:",c);
        char print_enemy_id[16];
        sprintf(print_enemy_id,"<%d>  {%d}",MapEditor.selected_enemy_id,MapEditor.clipboard_enemy_id);
        GrPrint(hdc,8*11,16,print_enemy_id,c);



        c = Highlight((MapEditor.selected_enemy_option==1),BLACK,LTPURPLE);
        GrPrint(hdc,8,34,"Type:",c);
        char print_enemy_type[16];
        sprintf(print_enemy_type,"<%d>  {%d}",MEEnemy[MapEditor.selected_enemy_id]->type,MEEnemy[MapEditor.clipboard_enemy_id]->type);
        GrPrint(hdc,8*11,34,print_enemy_type,c);
        break;

      case 3: //set enemy type
//        GrPrint(hdc,8,16,"ENEMY TYPE:",BLACK);
        c = Highlight((MapEditor.selected_enemy_type_option==0),BLACK,LTPURPLE);
        GrPrint(hdc,8,16,"ENEMY TYPE:",c);
        char print_enemy_type_id[16];
        sprintf(print_enemy_type_id,"<%d>  {%d}",MapEditor.selected_enemy_type_id,MapEditor.clipboard_enemy_type_id);
        GrPrint(hdc,8*13,16,print_enemy_type_id,c);



        for (int i=0;i<ENEMY_TYPE_INT_ATTR_NUM;i++) {
          c = Highlight((MapEditor.selected_enemy_type_option==i+1),BLACK,LTPURPLE);
          char print_enemy_type_int_attr[40];
          if (i!=4 && i!=16) {
            sprintf(print_enemy_type_int_attr,"%s <%d>  {%d}",enemy_type_int_attr_names[i],set_enemy_type_int_attr[i][MapEditor.selected_enemy_type_id],set_enemy_type_int_attr[i][MapEditor.clipboard_enemy_type_id]);            
            GrPrint(hdc,8,32+16*i,print_enemy_type_int_attr,c);
          } else {

            GrRect(hdc,8*14+1,32+16*i,16,16,WHITE);
            GrRect(hdc,8*18,32+16*i,16,16,WHITE);
            int di=MapEditor.selected_enemy_type_option-1;
            if (i==4) {
              GrRect(hdc,8*14+2+1,32+16*i+2,12,12,rgbPaint[set_enemy_type_color[MapEditor.selected_enemy_type_id]]);
              GrPrint(hdc,8,32+16*i,"Enemy Color:",c);
              GrPrint(hdc,8*13,32+16*i,"[      ]",c);

              GrRect(hdc,8*18+2,32+16*i+2,12,12,rgbPaint[set_enemy_type_color[MapEditor.clipboard_enemy_type_id]]);
              GrPrint(hdc,8*17,32+16*i,"{     }",c);
            } else {
              GrRect(hdc,8*14+2+1,32+16*i+2,12,12,rgbPaint[set_enemy_type_bullet_color[MapEditor.selected_enemy_type_id]]);
              GrPrint(hdc,8,32+16*i,"Bullet Color:",c);
              GrPrint(hdc,8*13,32+16*i,"[      ]",c);

              GrRect(hdc,8*18+2,32+16*i+2,12,12,rgbPaint[set_enemy_type_bullet_color[MapEditor.clipboard_enemy_type_id]]);
              GrPrint(hdc,8*17,32+16*i,"{     }",c);
            }

            if (color_chooser.is_choosing_color) {
              DrawPaintSquare(hdc,8*24+2+1+64,32+16*di,color_chooser.color_id,color_chooser.color_id_choosing);
            }
          }
        }


        for (int i=0;i<ENEMY_TYPE_DOUBLE_ATTR_NUM;i++) {
          c = Highlight((MapEditor.selected_enemy_type_option==i+ENEMY_TYPE_INT_ATTR_NUM+1),BLACK,LTPURPLE);
          char print_enemy_type_double_attr[32];
          sprintf(print_enemy_type_double_attr,"%s <%1.1f>  {%1.1f}",enemy_type_double_attr_names[i],set_enemy_type_double_attr[i][MapEditor.selected_enemy_type_id],set_enemy_type_double_attr[i][MapEditor.clipboard_enemy_type_id]);
          GrPrint(hdc,8,32+16*ENEMY_TYPE_INT_ATTR_NUM+16*i,print_enemy_type_double_attr,c);
        }


        for (int i=0;i<ENEMY_TYPE_BOOL_ATTR_NUM;i++) {
          c = Highlight((MapEditor.selected_enemy_type_option==i+ENEMY_TYPE_DOUBLE_ATTR_NUM+ENEMY_TYPE_INT_ATTR_NUM+1),BLACK,LTPURPLE);
          char print_enemy_type_bool_attr[32];
          sprintf(print_enemy_type_bool_attr,"%s <%d>  {%d}",enemy_type_bool_attr_names[i],set_enemy_type_bool_attr[i][MapEditor.selected_enemy_type_id],set_enemy_type_bool_attr[i][MapEditor.clipboard_enemy_type_id]);
          GrPrint(hdc,8,32+16*(ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_DOUBLE_ATTR_NUM)+16*i,print_enemy_type_bool_attr,c);
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


        //draw bullet frequency
        GrLine(hdc,MapEditor.demo_enemy_spritex+NODE_SIZE*set_enemy_type_shoot_at_player_range[MapEditor.selected_enemy_type_id]/2,MapEditor.demo_enemy_spritey+24,
                   MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey+24,LTPURPLE);
        GrCircle(hdc,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey,NODE_SIZE*set_enemy_type_shoot_at_player_range[MapEditor.selected_enemy_type_id]/2,LTPURPLE,-1);
        for (int i=0;i<ENEMY_BULLET_NUM;i++) {
          if (Bullet[i].shot) {
            DrawBullet2(hdc,i,Bullet[i].x,Bullet[i].y,Bullet[i].color);
          }
        }
        break;

      case 4: //set map backgroudn and palette
        GrPrint(hdc,8,16,"LEVEL",BLACK);
        GrLine(hdc,320,16*8,320+MapEditor.set_lvl_ambient_val[5],16*8+MapEditor.set_lvl_ambient_val[4],BLUE);
        GrLine(hdc,320,16*17,320+MapEditor.set_lvl_ambient_val[8],16*17+MapEditor.set_lvl_ambient_val[7],LTGRAY);

        for (int i=0;i<9;i++) {
          c = Highlight(MapEditor.selected_lvl_ambient_option==i,BLACK,LTPURPLE);
          char melvlambienttxt[20];
          switch (i) {
            case 1:
              GrPrint(hdc,8,16*3,"Background Color: [      ]",c);
              GrRect(hdc,8*17+1+2,16*3,16,16,WHITE);
              GrRect(hdc,8*17+2+1+2,16*3+2,12,12,rgbPaint[MapEditor.set_lvl_ambient_val[1]]);
              if (color_chooser.is_choosing_color) {
                DrawPaintSquare(hdc,8*25,52+2,color_chooser.color_id,color_chooser.color_id_choosing);
              }
              break;

            case 3:
            case 6:
              if (MapEditor.set_lvl_ambient_val[i]==0) {
                sprintf(melvlambienttxt,"%s: <FALSE>",melvlambienttxt_arr[i]);
              } else {
                sprintf(melvlambienttxt,"%s: <TRUE>",melvlambienttxt_arr[i]);
              }
              GrPrint(hdc,8,16*(i+2),melvlambienttxt,c);
              break;

            default:
              sprintf(melvlambienttxt,"%s: <%d>",melvlambienttxt_arr[i],MapEditor.set_lvl_ambient_val[i]);
              GrPrint(hdc,8,16*(i+2),melvlambienttxt,c);
              break;
         }
       }
        break;

    }

    if (MapEditor.selected_option==0 || MapEditor.selected_option==2) {
      char print_search_id[8];
      if (!MapEditor.is_typing_search) {
        if (!MapEditor.is_ground_txt_typing)
          GrPrint(hdc,8,102,"[CTRL+F]: Find",GREEN);
      } else {
        GrRect(hdc,0,86,8*40,102-16*3,BLACK);
        GrPrint(hdc,8,86,"[CTRL+F]: Abort.  [Enter]: Search.",GREEN);
        sprintf(print_search_id,"Find: %d",MapEditor.typing_search_id);
        GrPrint(hdc,8,86+16,print_search_id,GREEN);          
      }
    }
  }
}
