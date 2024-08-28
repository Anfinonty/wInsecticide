


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
	    c=color_arr[Ground[i]->color_id];
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
       // if (!IsInvertedBackground()) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,FALSE,0);
        /*} else {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,TRUE,HS_BDIAGONAL);*/
        }
      } else if (Ground[i]->type==1) {
        if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
            !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,BLUE,
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
                color_arr[Ground[i]->color_id]);

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
            color_arr[Ground[i]->color_id],
            16,
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



void DrawMapEditorEnemy(HDC hdc)
{
  int c;
  char txt_i[4];
  for (int i=0;i<ENEMY_NUM;i++) {
    int type=MEEnemy[i]->type;
    sprintf(txt_i,"%d",i);
    c=Highlight((i==MapEditor.selected_enemy_id && MapEditor.selected_option==2),color_arr[set_enemy_type_color[type]],LTPURPLE);
    if (set_enemy_type_species[type]==0)
      GrPrint(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-32+GR_HEIGHT/2,txt_i,c);
    else
      GrPrint(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-64+GR_HEIGHT/2,txt_i,c);


    //GrSprite(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,MEEnemySprite[type]->sprite_1,FALSE);
    DrawSprite(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,&MEEnemySprite[type]->draw_sprite_1,FALSE);
  }
}



void DrawMapEditorPlayer(HDC hdc)
{
  //GrSprite(hdc,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,player.sprite_1_cache,FALSE);
  DrawSprite(hdc,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,&player.draw_sprite_1,FALSE);
}



void DrawMapEditorUI(HDC hdc)
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
      case 0:
        c = Highlight((MapEditor.selected_ground_option==0),BLACK,LTPURPLE);
        GrPrint(hdc,8,16,"GROUNDS:",c);
        char print_ground_id[8];
        sprintf(print_ground_id,"<%d>",MapEditor.selected_ground_id);
        GrPrint(hdc,8*11,16,print_ground_id,c);


        //type
        c = Highlight((MapEditor.selected_ground_option==1),BLACK,LTPURPLE);
        GrPrint(hdc,8,34,"Type:",c);
        char print_ground_type[8];
        sprintf(print_ground_id,"<%d>",Ground[MapEditor.selected_ground_id]->type);
        GrPrint(hdc,8*11,34,print_ground_id,c);

        //color
        c = Highlight((MapEditor.selected_ground_option==2),BLACK,LTPURPLE);
        GrPrint(hdc,8,52,"Color:",c);
        GrPrint(hdc,8*11,52,"<    >",c);

        if (Ground[MapEditor.selected_ground_id]->color_id!=0) {
          GrRect(hdc,8*12+1,52,16,16,BLACK);
        } else {
          GrRect(hdc,8*12+1,52,16,16,WHITE);
        }

        GrRect(hdc,8*12+2+1,52+2,12,12,draw_color_arr[Ground[MapEditor.selected_ground_id]->color_id]);

        //is_ghost
        c = Highlight((MapEditor.selected_ground_option==3),BLACK,LTPURPLE);
        GrPrint(hdc,8,70,"IsGhost:",c);
        if (Ground[MapEditor.selected_ground_id]->is_ghost) {
          GrPrint(hdc,8*11,70,"<TRUE>",c);
        } else {
          GrPrint(hdc,8*11,70,"<FALSE>",c);
        }

        wchar_t duplicate_txt_visual[513];
        if (Ground[MapEditor.selected_ground_id]->type==2 && !MapEditor.is_typing_search) { //Ground text, show gui
          if (!MapEditor.is_ground_txt_typing) {
            GrPrint(hdc,8,86,"[Enter]: Begin Typing",GREEN);
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
        char print_enemy_id[8];
        sprintf(print_enemy_id,"<%d>",MapEditor.selected_enemy_id);
        GrPrint(hdc,8*11,16,print_enemy_id,c);



        c = Highlight((MapEditor.selected_enemy_option==1),BLACK,LTPURPLE);
        GrPrint(hdc,8,34,"Type:",c);
        char print_enemy_type[8];
        sprintf(print_enemy_type,"<%d>",MEEnemy[MapEditor.selected_enemy_id]->type);
        GrPrint(hdc,8*11,34,print_enemy_type,c);
        break;

      case 3: //set enemy type
        GrPrint(hdc,8,16,"ENEMY TYPE:",BLACK);
/*
    set_enemy_type_speed[i]=saved_enemy_type_speed[i];
    set_enemy_type_bullet_speed[i]=saved_enemy_type_bullet_speed[i];
    set_enemy_type_species[i]=saved_enemy_type_species[i];
    set_enemy_type_follow_range[i]=saved_enemy_type_follow_range[i];
    set_enemy_type_unchase_range[i]=saved_enemy_type_unchase_range[i];
    set_enemy_type_chase_range[i]=saved_enemy_type_chase_range[i];
    set_enemy_type_color[i]=saved_enemy_type_color[i];
    set_enemy_type_speed_multiplier[i]=saved_enemy_type_speed_multiplier[i];
    set_enemy_type_health[i]=saved_enemy_type_health[i];
    set_enemy_type_shoot_at_player_range[i]=saved_enemy_type_shoot_at_player_range[i];
    set_enemy_type_aim_rand[i]=saved_enemy_type_aim_rand[i];
    set_enemy_type_bullet_cooldown[i]=saved_enemy_type_bullet_cooldown[i];
    set_enemy_type_bullet_fire_cooldown[i]=saved_enemy_type_bullet_fire_cooldown[i];
    set_enemy_type_bullet_fire_at_once[i]=saved_enemy_type_bullet_fire_at_once[i];
    set_enemy_type_bullet_length[i]=saved_enemy_type_bullet_length[i];
    set_enemy_type_bullet_damage[i]=saved_enemy_type_bullet_damage[i];
    set_enemy_type_bullet_speed_multiplier[i]=saved_enemy_type_bullet_speed_multiplier[i];
    set_enemy_type_bullet_range[i]=saved_enemy_type_bullet_range[i];
    set_enemy_type_bullet_color[i]=saved_enemy_type_bullet_color[i];
    set_enemy_type_bullet_graphics_type[i]=saved_enemy_type_bullet_graphics_type[i];
    set_enemy_type_time_breaker_rare[i]=saved_enemy_type_time_breaker_rare[i];
    set_enemy_type_time_breaker_length[i]=saved_enemy_type_time_breaker_length[i];
    set_enemy_type_time_breaker_immune[i]=saved_enemy_type_time_breaker_immune[i];

*/

        break;

      case 4: //set map backgroudn and palette
        GrPrint(hdc,8,16,"LEVEL",BLACK);
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
