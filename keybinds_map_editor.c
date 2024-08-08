

void MapEditorKeypressDown(WPARAM wParam)
{
  switch (wParam) {
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
        if (!MapEditor.is_typing_search && !MapEditor.is_ground_txt_typing) {
          MapEditor.selected_option=LimitValue(MapEditor.selected_option+1,0,5);
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
              MapEditor.selected_ground_option=LimitValue(MapEditor.selected_ground_option+1,0,4);
            break;
          case 2:
            MapEditor.selected_enemy_option=LimitValue(MapEditor.selected_enemy_option+1,0,2);
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
              MapEditor.selected_ground_option=LimitValue(MapEditor.selected_ground_option-1,0,4);
            break;
          case 2:
            MapEditor.selected_enemy_option=LimitValue(MapEditor.selected_enemy_option-1,0,2);
            break;
        }
        break;



    //Holding Down 'G' key
      case 'G':
        if (keydown(VK_CONTROL)) {//CTRL + 'G' ==> Sticky increase
          MapEditor.sticky_level=LimitValue(MapEditor.sticky_level+4,0,33);
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
                MapEditor.selected_ground_id=LimitValue(MapEditor.selected_ground_id-1,0,GROUND_NUM);
                MapEditor.selected_ground_pivot=0;
                break;
              case 1: // type
                if (Ground[MapEditor.selected_ground_id]->type==0)
                  Ground[MapEditor.selected_ground_id]->is_ghost = TRUE;
                Ground[MapEditor.selected_ground_id]->type=LimitValue(Ground[MapEditor.selected_ground_id]->type-1,0,4);
                MapEditor.selected_ground_pivot=0;
                break;
              case 2: // color
                Ground[MapEditor.selected_ground_id]->color_id=LimitValue(Ground[MapEditor.selected_ground_id]->color_id-1,0,COLORS_NUM);
                break;
              case 3: // is_ghost
                if (Ground[MapEditor.selected_ground_id]->type==0)
                  Ground[MapEditor.selected_ground_id]->is_ghost = !Ground[MapEditor.selected_ground_id]->is_ghost;
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
                MapEditor.selected_enemy_id=LimitValue(MapEditor.selected_enemy_id-1,0,ENEMY_NUM);
                break;
              case 1:
                MEEnemy[MapEditor.selected_enemy_id]->type=LimitValue(MEEnemy[MapEditor.selected_enemy_id]->type-1,0,ENEMY_TYPE_NUM);
                break;                
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
                  MapEditor.selected_ground_id=LimitValue(MapEditor.selected_ground_id+1,0,GROUND_NUM);
                  MapEditor.selected_ground_pivot=0;
                  break;
                case 1: // type
                  if (Ground[MapEditor.selected_ground_id]->type==0)
                    Ground[MapEditor.selected_ground_id]->is_ghost = TRUE;
                  Ground[MapEditor.selected_ground_id]->type=LimitValue(Ground[MapEditor.selected_ground_id]->type+1,0,4);
                  MapEditor.selected_ground_pivot=0;
                  break;
                case 2: // color
                  Ground[MapEditor.selected_ground_id]->color_id=LimitValue(Ground[MapEditor.selected_ground_id]->color_id+1,0,COLORS_NUM);
                  break;
                case 3: // is_ghost
                  if (Ground[MapEditor.selected_ground_id]->type==0)
                    Ground[MapEditor.selected_ground_id]->is_ghost = !Ground[MapEditor.selected_ground_id]->is_ghost;
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
                MapEditor.selected_enemy_id=LimitValue(MapEditor.selected_enemy_id+1,0,ENEMY_NUM);
                break;
              case 1:
                MEEnemy[MapEditor.selected_enemy_id]->type=LimitValue(MEEnemy[MapEditor.selected_enemy_id]->type+1,0,ENEMY_TYPE_NUM);
                break;                
            }
            break;
        }
        break;


  //Holding down ENTER key
    case VK_RETURN:
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
            MapEditor.selected_ground_id=LimitValue(MapEditor.typing_search_id,0,GROUND_NUM-1);
            break;
          case 2: //Searching for Enemy
            MapEditor.selected_enemy_id=LimitValue(MapEditor.typing_search_id,0,ENEMY_NUM-1);
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


void MapEditorKeypressUp(WPARAM wParam)
{
  switch (wParam) {

    //Release S or Down key
      case 'S':
      case VK_DOWN:
        if (keydown(VK_CONTROL) && wParam=='S') {//CTRL + 'S' ==> SAVE
          //printf("Level Saved!\n");
          if (game_audio)
            PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start        
          SaveMELvl();
        }
        player.rst_down=FALSE;
        break;

    //Release D or Right key
      case 'D':
      case VK_RIGHT:
        player.rst_right=FALSE;
        break;


    //Release A or Left key
      case 'A':
      case VK_LEFT:
        player.rst_left=FALSE;
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
            SaveMELvl();
            back_to_menu=TRUE;
          }
        }
      }
      break;
  }

}

