


void UpdateMEDrawSprite()
{
    int ei=MapEditor.selected_enemy_type_id;
    //creating new sprite :o
    DeleteObject(MEEnemySprite[ei]->sprite_1); //delete old sprite
    MEEnemySprite[ei]->sprite_1=NULL;
    FreeDrawSprite(&MEEnemySprite[ei]->draw_sprite_1);
    //Create New Sprite
    switch (set_enemy_type_species[ei]) {
      case 0:
        MEEnemySprite[ei]->sprite_1=CopyCrunchyBitmap(enemy1_sprite_1,SRCCOPY);
        break;
      case 1:
        MEEnemySprite[ei]->sprite_1=CopyCrunchyBitmap(enemy2_sprite_1,SRCCOPY);
        break;
      case 2:
        MEEnemySprite[ei]->sprite_1=CopyCrunchyBitmap(enemy3_sprite_1,SRCCOPY);
        break;
      case 3:
        MEEnemySprite[ei]->sprite_1=CopyCrunchyBitmap(enemy4_sprite_1,SRCCOPY);
        break;
      case 4:
        MEEnemySprite[ei]->sprite_1=CopyCrunchyBitmap(enemy5_sprite_1,SRCCOPY);
        break;
      case 5:
      case 6:
      case 7:
        MEEnemySprite[ei]->sprite_1=CopyCrunchyBitmap(enemy6_sprite_1,SRCCOPY);
        break;
    }

    ReplaceBitmapColor2(MEEnemySprite[ei]->sprite_1,LTGREEN,BLACK,8,LTGREEN);
    loading_numerator++;
    GenerateDrawSprite(&MEEnemySprite[ei]->draw_sprite_1,MEEnemySprite[ei]->sprite_1);

    //Update Palette!!
    CopyReplaceColorPalette(MEEnemySprite[ei]->enemyPalette,rgbColorsDefault,167,rgbPaint[set_enemy_type_color[ei]]); //set normal palette

    MapEditor.flag_enemy_palette_i=ei;
}


void MEColorPickKeypressDown(WPARAM wParam)
{
  switch (MapEditor.pick_color) {
    case 0: //Ground
      ColorKeypressDown(wParam,&Ground[MapEditor.selected_ground_id]->color_id);
      break;
    case 1: //Enemy Type color
      ColorKeypressDown(wParam,&set_enemy_type_color[MapEditor.selected_enemy_type_id]);
      break;
    case 2: //Enemy Type bulllet color
      ColorKeypressDown(wParam,&set_enemy_type_bullet_color[MapEditor.selected_enemy_type_id]);
      break;
    case 3: //Background
      ColorKeypressDown(wParam,&MapEditor.set_lvl_ambient_val[1]);
      break;
   }
}


void MEColorPickKeypressUp(WPARAM wParam)
{
  switch (MapEditor.pick_color) {
    case 0: //Ground
      ColorKeypressUp(wParam,&Ground[MapEditor.selected_ground_id]->color_id);
      break;
    case 1: //Enemy Type color
      ColorKeypressUp(wParam,&set_enemy_type_color[MapEditor.selected_enemy_type_id]);
      UpdateMEDrawSprite();
      break;
    case 2: //Enemy Type bulllet color
      ColorKeypressUp(wParam,&set_enemy_type_bullet_color[MapEditor.selected_enemy_type_id]);
      break;
    case 3:
      ColorKeypressUp(wParam,&MapEditor.set_lvl_ambient_val[1]);
      break;
   }
}



void MapEditorKeypressDown(WPARAM wParam)
{
  switch (wParam) {
    //Copy
      case 'c':
      case 'C':
        if (keydown(VK_CONTROL)) {//Copy
          switch (MapEditor.selected_option) {
            case 0: //Ground
              MapEditor.clipboard_ground_id=MapEditor.selected_ground_id;
              break;
            case 2: //Enemy
              MapEditor.clipboard_enemy_id=MapEditor.selected_enemy_id;
              break;
            case 3: //Enemy Type
              MapEditor.clipboard_enemy_type_id=MapEditor.selected_enemy_type_id;
              break;
          }
        }
        break;

    //Paste
      case 'v':
      case 'V':
        if (keydown(VK_CONTROL)) {//Paste
          switch (MapEditor.selected_option) {
            case 0: //Ground 
              {
                int k=MapEditor.selected_ground_id;
                int l=MapEditor.clipboard_ground_id;
                DestroyMEGround(k); //destroy current ground

                //set ground attributes
                Ground[k]->x1=Ground[l]->x1;
                Ground[k]->y1=Ground[l]->y1;
                Ground[k]->x2=Ground[l]->x2;
                Ground[k]->y2=Ground[l]->y2;
                Ground[k]->x3=Ground[l]->x3;
                Ground[k]->y3=Ground[l]->y3;
                Ground[k]->type=Ground[l]->type;
                Ground[k]->color_id=Ground[l]->color_id;
                Ground[k]->color=rgbPaint[Ground[l]->color_id];//color_arr[saved_ground_color[i]];
                wcsncpy(Ground[k]->text,Ground[l]->text,512);
                Ground[k]->font_size=Ground[l]->font_size;
                Ground[k]->is_ghost=Ground[l]->is_ghost;
                
                //place ground on map
                SetGround(k);
                SetMENodeGridAttributes(k);
                InitRDGrid();
              }
              break;
            case 2: //Enemy
              MEEnemy[MapEditor.selected_enemy_id]->x=MEEnemy[MapEditor.clipboard_enemy_id]->x;
              MEEnemy[MapEditor.selected_enemy_id]->y=MEEnemy[MapEditor.clipboard_enemy_id]->y;
              MEEnemy[MapEditor.selected_enemy_id]->type=MEEnemy[MapEditor.clipboard_enemy_id]->type;
              break;
            case 3: //Enemy Type
            {
              int k=MapEditor.selected_enemy_type_id;
              int l=MapEditor.clipboard_enemy_type_id;
              set_enemy_type_speed[k]=set_enemy_type_speed[l];
              set_enemy_type_bullet_speed[k]=set_enemy_type_bullet_speed[l];
              set_enemy_type_species[k]=set_enemy_type_species[l];
              set_enemy_type_follow_range[k]=set_enemy_type_follow_range[l];
              set_enemy_type_unchase_range[k]=set_enemy_type_unchase_range[l];
              set_enemy_type_chase_range[k]=set_enemy_type_chase_range[l];
              set_enemy_type_color[k]=set_enemy_type_color[l];
              set_enemy_type_speed_multiplier[k]=set_enemy_type_speed_multiplier[l];
              set_enemy_type_health[k]=set_enemy_type_health[l];
              set_enemy_type_shoot_at_player_range[k]=set_enemy_type_shoot_at_player_range[l];
              set_enemy_type_aim_rand[k]=set_enemy_type_aim_rand[l];
              set_enemy_type_bullet_cooldown[k]=set_enemy_type_bullet_cooldown[l];
              set_enemy_type_bullet_fire_cooldown[k]=set_enemy_type_bullet_fire_cooldown[l];
              set_enemy_type_bullet_fire_at_once[k]=set_enemy_type_bullet_fire_at_once[l];
              set_enemy_type_bullet_length[k]=set_enemy_type_bullet_length[l];
              set_enemy_type_bullet_damage[k]=set_enemy_type_bullet_damage[l];
              set_enemy_type_bullet_speed_multiplier[k]=set_enemy_type_bullet_speed_multiplier[l];
              set_enemy_type_bullet_range[k]=set_enemy_type_bullet_range[l];
              set_enemy_type_bullet_color[k]=set_enemy_type_bullet_color[l];
              set_enemy_type_bullet_graphics_type[k]=set_enemy_type_bullet_graphics_type[l];
              set_enemy_type_time_breaker_rare[k]=set_enemy_type_time_breaker_rare[l];
              set_enemy_type_time_breaker_length[k]=set_enemy_type_time_breaker_length[l];
              set_enemy_type_time_breaker_immune[k]=set_enemy_type_time_breaker_immune[l];
              UpdateMEDrawSprite();
            }
              break;
          }    
        }
        break;

    //Copy & Paste Color: ctrl p: copy, shift p: paste
      case 'p':
      case 'P':
        if (MapEditor.selected_option==0) { //Ground
          if (keydown(VK_CONTROL)) {//Copy current color
            MapEditor.clipboard_ground_color_id=Ground[MapEditor.selected_ground_id]->color_id;
          } else if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //Paste ccurrent color t target
            Ground[MapEditor.selected_ground_id]->color_id=MapEditor.clipboard_ground_color_id;
          }
        }
        break;

    //F
      case 'F':
        if (keydown(VK_CONTROL)) {//CTRL + 'F' ==> Find
          if (!MapEditor.is_ground_txt_typing &&
              (MapEditor.selected_option==0 || MapEditor.selected_option==2)) {
            if (MapEditor.is_typing_search) {
              MapEditor.is_typing_search=FALSE;
            } else {
              MapEditor.is_typing_search=TRUE;
            }

            //Clear All typing search
            MapEditor.typing_search_txt_pos=0;
            MapEditor.typing_search_id=0;
            //MapEditor.is_typing_search=FALSE;
          }
        }
        break;

    //Tab
      case 0x09:
        /*if (MapEditor.selected_option==3) {
          UpdateMEDrawSprite();
        }*/
        if (!MapEditor.is_typing_search && !MapEditor.is_ground_txt_typing) {
          MapEditor.selected_option=LimitValueInt(MapEditor.selected_option+1,0,5);
        }
        break;

    //Holding Down Down Arrow or 'S'
      case 'S': //movement down y
        player.rst_down=TRUE;
        break;

      case VK_DOWN: //change type to alter ++
        switch (MapEditor.selected_option) {
          case 0:
            if (!MapEditor.is_ground_txt_typing)
              MapEditor.selected_ground_option=LimitValueInt(MapEditor.selected_ground_option+1,0,5);
            break;
          case 2:
            MapEditor.selected_enemy_option=LimitValueInt(MapEditor.selected_enemy_option+1,0,2);
            break;
          case 3:
            MapEditor.selected_enemy_type_option=
                LimitValueInt(MapEditor.selected_enemy_type_option+1,0,ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_DOUBLE_ATTR_NUM+ENEMY_TYPE_BOOL_ATTR_NUM+1);
            break;
          case 4:
            MapEditor.selected_lvl_ambient_option=LimitValueInt(MapEditor.selected_lvl_ambient_option+1,0,9);
            break;
        }
        break;



    //Holding Down Up Arrow or 'W''
      case 'W':
        player.rst_up=TRUE;
        break;

      case VK_UP: //change type to alter --
        switch (MapEditor.selected_option) {
          case 0:
            if (!MapEditor.is_ground_txt_typing)
              MapEditor.selected_ground_option=LimitValueInt(MapEditor.selected_ground_option-1,0,5);
            break;
          case 2:
            MapEditor.selected_enemy_option=LimitValueInt(MapEditor.selected_enemy_option-1,0,2);
            break;
          case 3:
            MapEditor.selected_enemy_type_option=LimitValueInt(MapEditor.selected_enemy_type_option-1,0,ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_DOUBLE_ATTR_NUM+ENEMY_TYPE_BOOL_ATTR_NUM+1);
            break;
          case 4:
            MapEditor.selected_lvl_ambient_option=LimitValueInt(MapEditor.selected_lvl_ambient_option-1,0,9);
            break;
        }
        break;



    //Holding Down 'G' key
      case 'G':
        if (keydown(VK_CONTROL)) {//CTRL + 'G' ==> Sticky increase
          MapEditor.sticky_level=LimitValueInt(MapEditor.sticky_level+4,0,33);
        }
        break;


    //Holding Down Left Arrow or 'A'
      case 'A':
        player.rst_left=TRUE;
        break;

      case VK_LEFT:// change value --
        switch (MapEditor.selected_option) {
          case 0: //Ground
            if (!MapEditor.is_ground_txt_typing) {
            switch (MapEditor.selected_ground_option) {
              case 0: //ground_id
                MapEditor.selected_ground_id=LimitValueInt(MapEditor.selected_ground_id-1,0,GROUND_NUM);
                MapEditor.selected_ground_pivot=0;
                break;
              case 1: // type
                if (Ground[MapEditor.selected_ground_id]->type==0)
                  Ground[MapEditor.selected_ground_id]->is_ghost = TRUE;
                Ground[MapEditor.selected_ground_id]->type=LimitValueInt(Ground[MapEditor.selected_ground_id]->type-1,0,8);
                MapEditor.selected_ground_pivot=0;
                break;
              /*case 2: // color
                Ground[MapEditor.selected_ground_id]->color_id=LimitValueInt(Ground[MapEditor.selected_ground_id]->color_id-1,0,COLORS_NUM);
                break;*/
              case 3: // is_ghost
                if (Ground[MapEditor.selected_ground_id]->type==0)
                  Ground[MapEditor.selected_ground_id]->is_ghost = !Ground[MapEditor.selected_ground_id]->is_ghost;
                break;

                case 4: //font size
                  Ground[MapEditor.selected_ground_id]->font_size=LimitValueInt(Ground[MapEditor.selected_ground_id]->font_size-1,0,64);
                break;

            }
            } else {
            //if (MapEditor.typing_ground_txt_pos>0)
              //MapEditor.typing_ground_txt_pos--;
            }
            break;

          case 2: //Enemy
            switch (MapEditor.selected_enemy_option) {
              case 0:
                MapEditor.selected_enemy_id=LimitValueInt(MapEditor.selected_enemy_id-1,0,ENEMY_NUM);
                break;
              case 1:
                MEEnemy[MapEditor.selected_enemy_id]->type=LimitValueInt(MEEnemy[MapEditor.selected_enemy_id]->type-1,0,ENEMY_TYPE_NUM);
                break;                
            }
            break;

          case 3: //enemy type --
            if (MapEditor.selected_enemy_type_option>0) { //set enemy_type
              int seto = MapEditor.selected_enemy_type_option-1;
              if ( MapEditor.selected_enemy_type_option!=5 && MapEditor.selected_enemy_type_option!=17) {
              if (seto<ENEMY_TYPE_INT_ATTR_NUM) {//int values

                if (seto!=2 && seto!=3) {
                  set_enemy_type_int_attr[seto][MapEditor.selected_enemy_type_id]=
                  LimitValueInt(
                    set_enemy_type_int_attr[seto][MapEditor.selected_enemy_type_id]-enemy_int_attr_delta[seto],
                    enemy_int_attr_min[seto],
                    enemy_int_attr_max[seto]
                  );
                } else {
                  set_enemy_type_int_attr[seto][MapEditor.selected_enemy_type_id]--;
                }

                if (seto>=1 && seto<=3) {
                  set_enemy_type_int_attr[2][MapEditor.selected_enemy_type_id]=
                    LimitValueInt(set_enemy_type_int_attr[2][MapEditor.selected_enemy_type_id],1,1+set_enemy_type_int_attr[1][MapEditor.selected_enemy_type_id]);

                  set_enemy_type_int_attr[3][MapEditor.selected_enemy_type_id]=
                    LimitValueInt(set_enemy_type_int_attr[3][MapEditor.selected_enemy_type_id],1,1+set_enemy_type_int_attr[1][MapEditor.selected_enemy_type_id]);
                }

              } else if (seto>=ENEMY_TYPE_INT_ATTR_NUM && seto<ENEMY_TYPE_DOUBLE_ATTR_NUM+ENEMY_TYPE_INT_ATTR_NUM) { //double values
                seto-=ENEMY_TYPE_INT_ATTR_NUM;
                set_enemy_type_double_attr[seto][MapEditor.selected_enemy_type_id]=
                LimitValue(
                  set_enemy_type_double_attr[seto][MapEditor.selected_enemy_type_id]-enemy_double_attr_delta[seto],
                  enemy_double_attr_min[seto],
                  enemy_double_attr_max[seto]+1
                );                
              } else { //bool values
                seto-=(ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_DOUBLE_ATTR_NUM);
                set_enemy_type_bool_attr[seto][MapEditor.selected_enemy_type_id]=!set_enemy_type_bool_attr[seto][MapEditor.selected_enemy_type_id];
              }
              }
            } else { //changing tyoe 0 to 10
              MapEditor.selected_enemy_type_id = LimitValueInt(MapEditor.selected_enemy_type_id-1,0,ENEMY_TYPE_NUM);
            }
            break;
          case 4:
            if (MapEditor.selected_lvl_ambient_option!=1) {
              if (MapEditor.selected_lvl_ambient_option==0) {
                flag_update_background=TRUE;
              }
              MapEditor.set_lvl_ambient_val[MapEditor.selected_lvl_ambient_option]=
                LimitValue(MapEditor.set_lvl_ambient_val[MapEditor.selected_lvl_ambient_option]-1,
                  melvlambience_min[MapEditor.selected_lvl_ambient_option],
                  melvlambience_max[MapEditor.selected_lvl_ambient_option]);
            }
            break;

        }
        break;

    //Holding Down Right Arrow or 'D'
      case 'D': //movement up y
        player.rst_right=TRUE;
        break;

      case VK_RIGHT: //change value ++
        switch (MapEditor.selected_option) {
          case 0:
            if (!MapEditor.is_ground_txt_typing) {
              switch (MapEditor.selected_ground_option) {
                case 0: //ground_id
                  MapEditor.selected_ground_id=LimitValueInt(MapEditor.selected_ground_id+1,0,GROUND_NUM);
                  MapEditor.selected_ground_pivot=0;
                  break;
                case 1: // type
                  if (Ground[MapEditor.selected_ground_id]->type==0)
                    Ground[MapEditor.selected_ground_id]->is_ghost = TRUE;
                  Ground[MapEditor.selected_ground_id]->type=LimitValueInt(Ground[MapEditor.selected_ground_id]->type+1,0,8);
                  MapEditor.selected_ground_pivot=0;
                  break;
                /*case 2: // color
                  Ground[MapEditor.selected_ground_id]->color_id=LimitValueInt(Ground[MapEditor.selected_ground_id]->color_id+1,0,COLORS_NUM);
                  break;*/
                case 3: // is_ghost
                  if (Ground[MapEditor.selected_ground_id]->type==0)
                    Ground[MapEditor.selected_ground_id]->is_ghost = !Ground[MapEditor.selected_ground_id]->is_ghost;
                  break;
                case 4: //font size
                  Ground[MapEditor.selected_ground_id]->font_size=LimitValueInt(Ground[MapEditor.selected_ground_id]->font_size+1,0,64+1);
                  break;

              }
            } else {
              //if (MapEditor.typing_ground_txt_pos<lstrlenW(MapEditor.typing_ground_txt))
                //MapEditor.typing_ground_txt_pos++;
            }
            break;
          case 2: // Enemy
            switch (MapEditor.selected_enemy_option) {
              case 0:
                MapEditor.selected_enemy_id=LimitValueInt(MapEditor.selected_enemy_id+1,0,ENEMY_NUM);
                break;
              case 1:
                MEEnemy[MapEditor.selected_enemy_id]->type=LimitValueInt(MEEnemy[MapEditor.selected_enemy_id]->type+1,0,ENEMY_TYPE_NUM);
                break;                
            }
            break;


          case 3: //Enemy Type ++ 
            if (MapEditor.selected_enemy_type_option>0) { //set enemy_type
              int seto = MapEditor.selected_enemy_type_option-1;
              if (MapEditor.selected_enemy_type_option!=5 && MapEditor.selected_enemy_type_option!=17) {
              if (seto<ENEMY_TYPE_INT_ATTR_NUM) {//int values
                if (seto!=2 && seto!=3) {
                  set_enemy_type_int_attr[seto][MapEditor.selected_enemy_type_id]=
                  LimitValueInt(
                    set_enemy_type_int_attr[seto][MapEditor.selected_enemy_type_id]+enemy_int_attr_delta[seto],
                    enemy_int_attr_min[seto],
                    enemy_int_attr_max[seto]
                  );
                } else {
                  set_enemy_type_int_attr[seto][MapEditor.selected_enemy_type_id]++;
                }
                //Limit Other Follow Ranges
                    //seto0 follow range
                    //seto1 unchase range
                    //seto2 rechase range
                if (seto>=1 && seto<=3) {
                  set_enemy_type_int_attr[2][MapEditor.selected_enemy_type_id]=
                    LimitValueInt(set_enemy_type_int_attr[2][MapEditor.selected_enemy_type_id],1,1+set_enemy_type_int_attr[1][MapEditor.selected_enemy_type_id]);

                  set_enemy_type_int_attr[3][MapEditor.selected_enemy_type_id]=
                    LimitValueInt(set_enemy_type_int_attr[3][MapEditor.selected_enemy_type_id],1,1+set_enemy_type_int_attr[1][MapEditor.selected_enemy_type_id]);
                }

              } else if (seto>=ENEMY_TYPE_INT_ATTR_NUM && seto<ENEMY_TYPE_DOUBLE_ATTR_NUM+ENEMY_TYPE_INT_ATTR_NUM) { //double values
                seto-=ENEMY_TYPE_INT_ATTR_NUM;
                set_enemy_type_double_attr[seto][MapEditor.selected_enemy_type_id]=
                LimitValue(
                  set_enemy_type_double_attr[seto][MapEditor.selected_enemy_type_id]+enemy_double_attr_delta[seto],
                  enemy_double_attr_min[seto],
                  enemy_double_attr_max[seto]
                );                
              } else { //bool values
                seto-=(ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_DOUBLE_ATTR_NUM);
                set_enemy_type_bool_attr[seto][MapEditor.selected_enemy_type_id]=!set_enemy_type_bool_attr[seto][MapEditor.selected_enemy_type_id];
              }
              }
            } else { //changing tyoe 0 to 10
              InitBullet(ENEMY_BULLET_NUM);
              MapEditor.selected_enemy_type_id = LimitValueInt(MapEditor.selected_enemy_type_id+1,0,ENEMY_TYPE_NUM);
            }
            break;

          //Level ambience
          case 4:
            if (MapEditor.selected_lvl_ambient_option!=1) {
              if (MapEditor.selected_lvl_ambient_option==0) {
                flag_update_background=TRUE;
              }
              MapEditor.set_lvl_ambient_val[MapEditor.selected_lvl_ambient_option]=
                LimitValue(MapEditor.set_lvl_ambient_val[MapEditor.selected_lvl_ambient_option]+1,
                  melvlambience_min[MapEditor.selected_lvl_ambient_option],
                  melvlambience_max[MapEditor.selected_lvl_ambient_option]);
            }
            break;
        }
        break;


  //Holding down ENTER key
    case VK_RETURN:
      //Begin Painting
      if (!MapEditor.is_typing_search) {
      switch (MapEditor.selected_option) {
        case 0:
          if (MapEditor.selected_ground_option==2) {
            MapEditor.pick_color=0;
            color_chooser.is_choosing_color=TRUE;
            color_chooser.color_id=
            color_chooser.color_id_choosing=Ground[MapEditor.selected_ground_id]->color_id;
          }
          break;
        case 3:
          switch (MapEditor.selected_enemy_type_option) {
            case 5:
              color_chooser.is_choosing_color=TRUE;
              MapEditor.pick_color=1;
              color_chooser.color_id=
              color_chooser.color_id_choosing=set_enemy_type_color[MapEditor.selected_enemy_type_id];
              break;
            case 17:
              color_chooser.is_choosing_color=TRUE;
              MapEditor.pick_color=2;
              color_chooser.color_id=
              color_chooser.color_id_choosing=set_enemy_type_bullet_color[MapEditor.selected_enemy_type_id];
              break;
          } 
          break;
      case 4:
        if (MapEditor.selected_lvl_ambient_option==1) {
          color_chooser.is_choosing_color=TRUE;
          MapEditor.pick_color=3;
          color_chooser.color_id=
          color_chooser.color_id_choosing=MapEditor.set_lvl_ambient_val[1];
        }
        break;
      }
      }

      //Enable typing of Ground Text
      if (Ground[MapEditor.selected_ground_id]->type==2) {
        if (!MapEditor.is_ground_txt_typing && !MapEditor.is_typing_search) {
          MapEditor.is_ground_txt_typing=TRUE;
          MapEditor.is_ground_txt_typing_loaded=FALSE;
          MapEditor.typing_ground_txt_pos=lstrlenW(Ground[MapEditor.selected_ground_id]->text);
          swprintf(MapEditor.typing_ground_txt,512,L"%s",Ground[MapEditor.selected_ground_id]->text);
        }
      } 

      if (MapEditor.is_typing_search) {
        switch (MapEditor.selected_option) {
          case 0: //Searching for Ground
            MapEditor.selected_ground_id=LimitValueInt(MapEditor.typing_search_id,0,GROUND_NUM-1);
            break;
          case 2: //Searching for Enemy
            MapEditor.selected_enemy_id=LimitValueInt(MapEditor.typing_search_id,0,ENEMY_NUM-1);
            break;
        }
        //Clear All typing search
        MapEditor.typing_search_txt_pos=0;
        MapEditor.typing_search_id=0;
        MapEditor.is_typing_search=FALSE;
      }
      InitRDGrid();
      switch (MapEditor.selected_option) {
        case 0: //Goto Ground
          switch (MapEditor.selected_ground_pivot) {
            case 0:
              player.cam_x=-Ground[MapEditor.selected_ground_id]->x1;
              player.cam_y=-Ground[MapEditor.selected_ground_id]->y1;
              break;
            case 1:
              player.cam_x=-Ground[MapEditor.selected_ground_id]->x2;
              player.cam_y=-Ground[MapEditor.selected_ground_id]->y2;
              break;
            case 2:
              player.cam_x=-Ground[MapEditor.selected_ground_id]->x3;
              player.cam_y=-Ground[MapEditor.selected_ground_id]->y3;
              break;
          }
          break;
        case 1: //Goto player
          player.cam_x=-player.x;
          player.cam_y=-player.y;
          break;
        case 2: //Goto enemy
          player.cam_x=-MEEnemy[MapEditor.selected_enemy_id]->x;
          player.cam_y=-MEEnemy[MapEditor.selected_enemy_id]->y;
          break;
      }
      break;
  }
}


void MapEditorKeypressUp(WPARAM wParam, HWND hwnd, HDC hdc)
{
  switch (wParam) {
    // 0 key to lighten, shift 0 to darken
      case '0': {
        int gcolor_i;
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //darken
          for (int i=0;i<GROUND_NUM;i++) {
            gcolor_i=Ground[i]->color_id;
            gcolor_i-=16;
            if (gcolor_i<0) {
              gcolor_i+=256;
            }
            Ground[i]->color_id=gcolor_i;
          }          
        } else if (keydown(VK_CONTROL)){ //Swap White
          for (int i=0;i<GROUND_NUM;i++) { //brighten
            gcolor_i=Ground[i]->color_id;
            if ((gcolor_i+1)%16==0) {
              gcolor_i-=15;
            } else if ((gcolor_i+1)%16==1) {
              gcolor_i+=15;
            }
            if (gcolor_i<0) {
              gcolor_i+=256;
            } else if (gcolor_i>255) {
              gcolor_i-=256;
            }
            Ground[i]->color_id=gcolor_i;
          }
        } else {
          for (int i=0;i<GROUND_NUM;i++) { //brighten
            gcolor_i=Ground[i]->color_id;
            gcolor_i+=16;
            if (gcolor_i>255) {
              gcolor_i-=256;
            }
            Ground[i]->color_id=gcolor_i;
          }          
        }
        }
        break;
    //Release S or Down key
      case 'S':
      case VK_DOWN:
        if (keydown(VK_CONTROL) && wParam=='S') {//CTRL + 'S' ==> SAVE
          //printf("Level Saved!\n");
          if (game_audio)
            PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start        
          SaveMELvl(hwnd,hdc);
        }
        player.rst_down=FALSE;
        break;

    //Release D or Right key
      case 'D':
      case VK_RIGHT:
        player.rst_right=FALSE;
        switch (MapEditor.selected_option) {
          case 3: //Enemy Type
            if (MapEditor.selected_enemy_type_option==1) {
              UpdateMEDrawSprite();
            }
            break;
        }
        break;


    //Release A or Left key
      case 'A':
      case VK_LEFT:
        player.rst_left=FALSE;
        switch (MapEditor.selected_option) {
          case 3: //Enemy Type && species
            if (MapEditor.selected_enemy_type_option==1) {
              UpdateMEDrawSprite();
            }
            break;
        }
        break;

    //Release W or Up key
      case 'W':
      case VK_UP:
        player.rst_up=FALSE;
        break;    

    //Holding Down Shift && Escape
    case VK_ESCAPE:
      if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = ABORT DONT SAVE
        if (level_loaded) { //allow back to menu only if level is fully loaded
          if (MapEditor.is_ground_txt_typing) {
            if (game_audio)
              PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
            MapEditor.is_ground_txt_typing=FALSE;
            MapEditor.typing_ground_txt_pos=0;
            for (int i=0;i<512;i++)
              MapEditor.typing_ground_txt[i]=0;
          } else if (MapEditor.is_typing_search) {
          } else {
            if (game_audio)
              PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
            back_to_menu=TRUE;
          }
        }
      } else {
        if (level_loaded) { //allow back to menu only if level is fully loaded, QUIT && SAVE
          if (MapEditor.is_ground_txt_typing) {
            if (game_audio)
              PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start

            for (int i=0;i<512;i++)
              Ground[MapEditor.selected_ground_id]->text[i]=0;
            swprintf(Ground[MapEditor.selected_ground_id]->text,512,L"%s",MapEditor.typing_ground_txt);
            MapEditor.is_ground_txt_typing=FALSE;
            MapEditor.typing_ground_txt_pos=0;
            for (int i=0;i<512;i++)
              MapEditor.typing_ground_txt[i]=0;
          } else if (MapEditor.is_typing_search) {
          } else {
            if (game_audio)
              PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
            SaveMELvl(hwnd,hdc);
            //flag_load_melevel=TRUE;
            back_to_menu=TRUE;
          }
        }
      }
      break;
  }

}

