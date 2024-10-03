


struct console
{
  //DPAD
  //bool rst_d_up;
  //bool rst_d_down;
  //bool rst_d_left;
  //bool rst_d_right;

  bool connected;

  //Right pad
  bool rst_cross;
  //bool rst_square;
  //bool rst_circle;
  bool rst_triangle;
  bool rst_l3;
  bool rst_r3;

  //Corners
  //bool rst_l1;
  //bool rst_l2;

  //bool rst_r1;
  //bool rst_r2;

  //joystick
  //bool rst_l3;
  //bool rst_r3;

  //misc
  //bool rst_select;
  //bool rst_start;
  
  //DPAD
  int rst_d_up_timer;
  int rst_d_down_timer;
  int rst_d_left_timer;
  int rst_d_right_timer;

  //joystick

} ccontroller;


void InitCController()
{
  ccontroller.connected=FALSE;

  ccontroller.rst_d_up_timer=0;
  ccontroller.rst_d_down_timer=0;
  ccontroller.rst_d_left_timer=0;
  ccontroller.rst_d_right_timer=0;

  ccontroller.rst_cross=FALSE;
  ccontroller.rst_triangle=FALSE;

  ccontroller.rst_l3=FALSE;
  ccontroller.rst_r3=FALSE;
}




int MinusOneMenuCKeyPress(BYTE *rawData)
{
    //LEFT D-Pad
    switch (rawData[5] & 0xF) { //RIGHT digit only
      case 0x00:
      case 0x07: //UP
       ccontroller.rst_d_up_timer--;
       if (ccontroller.rst_d_up_timer<=0) {
         ccontroller.rst_d_up_timer=10;
         select_main_menu=LimitValue(select_main_menu-1,0,4);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
       }
       break;

      case 0x04: //DOWN
        ccontroller.rst_d_down_timer--;
        if (ccontroller.rst_d_down_timer<=0) {
          ccontroller.rst_d_down_timer=10;
          select_main_menu=LimitValue(select_main_menu+1,0,4);
          if (game_audio)
            PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        }
        break;

      case 0x0F: //Default value
        ccontroller.rst_d_up_timer=10;
        ccontroller.rst_d_down_timer=10;
        break;
    }


    //RIGHTPAD [5]
    switch (rawData[5] & 0xF0) { //LEFT digit only
      case 0x10: //TRIANGLE 0x10
        ccontroller.rst_triangle=TRUE;
        break;
      case 0x40: //CROSS 0x40
        ccontroller.rst_cross=TRUE;
        break;
      case 0x00: //Default value
        if (ccontroller.rst_cross) {
          ccontroller.rst_cross=FALSE;
          if (select_main_menu>=0 && select_main_menu<=1) {
            main_menu_chosen=select_main_menu;
            if (game_audio)
              PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
          }
          if (select_main_menu==2) { //exit
            if (game_audio)
              PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
            PostQuitMessage(0);
            return 0;
          }
         if (select_main_menu==3) {
           is_khmer=!is_khmer;
           if (game_audio)
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
         }

        }
        if (ccontroller.rst_triangle) {
          ccontroller.rst_triangle=FALSE;
          PostQuitMessage(0);
          return 0;
        }
        break;
    }
}

void ZeroMenuCKeyPress(HWND hwnd, HDC hdc, BYTE *rawData)
{
    //LEFT D-Pad
    switch (rawData[5] & 0xF) { //RIGHT digit only
      case 0x00:
      case 0x07: //UP //-- lvl chosen
        ccontroller.rst_d_up_timer--;
        if (ccontroller.rst_d_up_timer<=0) {
          ccontroller.rst_d_up_timer=10;
          level_chosen=LimitValue(level_chosen-1,0,level_num);
          if (game_audio)
            PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        }
        break;
      case 0x04: //DOWN //++ lvl chosen
        ccontroller.rst_d_down_timer--;
        if (ccontroller.rst_d_down_timer<=0) {
          ccontroller.rst_d_down_timer=10;
          level_chosen=LimitValue(level_chosen+1,0,level_num);
          if (game_audio)
            PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        }
        break;
      case 0x0F: //Default value
        ccontroller.rst_d_up_timer=10;
        ccontroller.rst_d_down_timer=10;
        break;
    }


    //RIGHTPAD [5]
    switch (rawData[5] & 0xF0) { //LEFT digit only
      case 0x10: //TRIANGLE 0x10 //Back to -1
        ccontroller.rst_triangle=TRUE;
        break;
      case 0x40: //CROSS 0x40
        ccontroller.rst_cross=TRUE;
        break;
      case 0x00: //Default value
        if (ccontroller.rst_triangle) {
          ccontroller.rst_triangle=FALSE;
          if (in_main_menu) {
            main_menu_chosen=-1;
           if (game_audio)
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
          }
        }
        if (ccontroller.rst_cross) {
          ccontroller.rst_cross=FALSE;
          if (player_color>-1 && player_color<COLORS_NUM) { //Enter Level
            if (game_audio)
              PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
            if (level_chosen>=0 && level_chosen<level_num && main_menu_chosen==0)
              InitLevel(hwnd, hdc);
          }
        }
        break;
    }

}

void OptionsCKeyPress(HWND hwnd, BYTE *rawData)
{
    //LEFT D-Pad
    switch (rawData[5] & 0xF) { //RIGHT digit only
      case 0x06: //LEFT
        ccontroller.rst_d_left_timer--;
        if (ccontroller.rst_d_left_timer<=0) {
          ccontroller.rst_d_left_timer=10;
          OptionKeyPressLeft(hwnd,option_choose);
        }
        break;
      case 0x02: //RIGHT
        ccontroller.rst_d_right_timer--;
        if (ccontroller.rst_d_right_timer<=0) {
          ccontroller.rst_d_right_timer=10;
          OptionKeyPressRight(hwnd,option_choose);
        }
        break;
      case 0x00:
      case 0x07: //UP //-- option chosen
        ccontroller.rst_d_up_timer--;
        if (ccontroller.rst_d_up_timer<=0) {
          ccontroller.rst_d_up_timer=10;
          option_choose=LimitValue(option_choose-1,0,GAME_OPTIONS_NUM);
          if (game_audio)
            PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        }
        break;
      case 0x04: //DOWN //++ option chosen
        ccontroller.rst_d_down_timer--;
        if (ccontroller.rst_d_down_timer<=0) {
          ccontroller.rst_d_down_timer=10;
          option_choose=LimitValue(option_choose+1,0,GAME_OPTIONS_NUM);
          if (game_audio)
            PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        }
        break;
      case 0x0F: //Default value
        ccontroller.rst_d_up_timer=10;
        ccontroller.rst_d_down_timer=10;
        ccontroller.rst_d_left_timer=10;
        ccontroller.rst_d_right_timer=10;
        OptionKeyPressRelease2();
        break;
    }


    //RIGHTPAD [5]
    switch (rawData[5] & 0xF0) { //LEFT digit only
      case 0x10: //TRIANGLE 0x10 //Back to -1
        ccontroller.rst_triangle=TRUE;
        break;
      case 0x00: //Default value
        if (ccontroller.rst_triangle) {
          ccontroller.rst_triangle=FALSE;
          OptionKeyPressRelease1();
        }
        break;
    }

}

/*void TwoMenuCKeyPress(BYTE *rawData)
{

}*/

/*void ThreeMenuCKeyPress(BYTE *rawData)
{

}*/


void GlobalJoystickMove(BYTE *rawData)
{
    //RIGHT Joystick LR[1] UD[2]
    if (rawData[1]>0x7F) {//right
      if (mouse_x<GR_WIDTH)
        mouse_x+=(rawData[1]-0x7F)/12;
    } else if (rawData[1]<0x7F) {//left
      if (mouse_x>0)
        mouse_x-=(0x7F-rawData[1])/12;
    }

    if (rawData[2]>0x7F) {//down
      if (mouse_y<GR_HEIGHT)
        mouse_y+=(rawData[2]-0x7F)/12;
    } else if (rawData[2]<0x7F) {//up
      if (mouse_y>0)
        mouse_y-=(0x7F-rawData[2])/12;
    }


    //LEFT Joystick LR[3] UD[4]
    if (rawData[3]>0x7F) {//right
      if (mouse_x<GR_WIDTH)
        mouse_x+=(rawData[3]-0x7F)/12;
    } else if (rawData[3]<0x7F) {//left
      if (mouse_x>0)
        mouse_x-=(0x7F-rawData[3])/12;
    }

    if (rawData[4]>0x7F) {//down
      if (mouse_y<GR_HEIGHT)
        mouse_y+=(rawData[4]-0x7F)/12;
    } else if (rawData[4]<0x7F) {//up
      if (mouse_y>0)
        mouse_y-=(0x7F-rawData[4])/12;
    }
}


void GameCKeyPress(BYTE *rawData)
{
    //LEFT D-Pad
    switch (rawData[5] & 0xF) { //RIGHT digit only
      case 0x06: //LEFT
        player.rst_left=TRUE;
        break;
      case 0x02: //RIGHT
        player.rst_right=TRUE;
        break;
      case 0x00:
      case 0x07: //UP change weapon
        ccontroller.rst_d_up_timer=10;
        break;
      case 0x04: //DOWN //pick up web
        ccontroller.rst_d_down_timer=10;
        break;
      case 0x0F: //Default value
        if (player.rst_left)
          player.rst_left=FALSE;
        if (player.rst_right)
          player.rst_right=FALSE;
        if (ccontroller.rst_d_up_timer>0) {
          ccontroller.rst_d_up_timer--;
        }
        if (ccontroller.rst_d_up_timer==8) {
          if (player.max_web_num-player.placed_web_num>=3 && player.knives_per_throw==5) {
            player.knives_per_throw=13;
          }
          if (player.max_web_num-player.placed_web_num>2) {          
            player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,15+1); //limit to 1,3,5,15
          } else if (player.max_web_num-player.placed_web_num>0){ //limit to 1,3,5
            player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,5+1);
          } else { //limit to 1,3
            player.knives_per_throw=LimitValue(player.knives_per_throw+2,1,3+1);
          }
        }
        if (ccontroller.rst_d_down_timer>0) {
          ccontroller.rst_d_down_timer--;
        }
        if (ccontroller.rst_d_down_timer==8) {
          player.destroy_ground=TRUE;
        }
        break;
    }

    //RIGHTPAD [5]
    switch (rawData[5] & 0xF0) { //LEFT digit only
      case 0x10: //TRIANGLE 0x10
        ccontroller.rst_triangle=TRUE;
        break;
      case 0x80: //SQUARE 0x80 //toggle timebreaker
        if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
          player.time_breaker=TRUE;
          if (game_audio)
            PlaySound(spamSoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //tb_start_audio
          player.time_breaker_cooldown=player.time_breaker_cooldown_max;
          player.speed+=player.time_breaker_units_max/2-1;
        }
        break;
      case 0x40: //CROSS 0x40
        player.rst_up=TRUE;
        break;
      case 0x20: //CIRCLE 0x20
        player.rst_down=TRUE;
        break;
      case 0x00: //Default value
        if (player.rst_up)
          player.rst_up=FALSE;
        if (player.rst_down)
          player.rst_down=FALSE;
        if (ccontroller.rst_triangle) {
          player.show_health_timer=HP_SHOW_TIMER_NUM;
          player.show_block_health_timer=HP_SHOW_TIMER_NUM;

          ccontroller.rst_triangle=FALSE;
          player.rst_left_click=TRUE;
        }
        break;
    }



        
    //L1,L2; R1,R2 [6]
    switch (rawData[6] & 0xF) { //RIGHT digit only
      //L1 --> Hold Slow Mo
      case 0x01:
      //L2 --> Hold Slow Mo
      case 0x04:
        player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
        if (player.rst_left_click) {
          player.rst_left_click=FALSE;
          player.attack_rst=TRUE;
        }
        if (player.rst_right_click)
          player.rst_right_click=FALSE;
        break;


      case 0x00: //default value
        if (player.rst_left_click) {
          player.rst_left_click=FALSE;
          player.attack_rst=TRUE;
        }

        if (player.rst_right_click)
          player.rst_right_click=FALSE;

        if (player.sleep_timer==SLOWDOWN_SLEEP_TIMER)
          player.sleep_timer=DEFAULT_SLEEP_TIMER;
        break;
    }


    switch (rawData[6] & 0xF) { //RIGHT digit only
      //R1 --> Left Click
      case 0x02:
      case 0x03: //plus slowmo L1
      case 0x06: //plus slowmo L2
        player.rst_left_click=TRUE;
        break;

      //R2 --> Right Click 0x08
      case 0x08:
      case 0x09://plus slowmo L1
      case 0x0C://plus slowmo L2
        player.rst_right_click=TRUE;
        break;
    }

    switch (rawData[6] & 0xF0) { //Left digit only
      //L3
      case 0x40:
        ccontroller.rst_l3=TRUE;
        break;
      //R3
      case 0x80:
        ccontroller.rst_r3=TRUE;
        break;
      //Exit
      case 0x10: //select //Quit
        if (level_loaded) { //allow back to menu only if level is fully loaded
          back_to_menu=TRUE;
        }
        break;

      case 0x20: //Start
        //Restart
        flag_restart=TRUE;
        break;

      case 0x00: //default
        if (ccontroller.rst_l3) {
          ccontroller.rst_l3=FALSE;
          player.uppercut=!player.uppercut;
        }
        if (ccontroller.rst_r3) {
          player.low_jump=!player.low_jump;        
          ccontroller.rst_r3=FALSE;
        }
        break;
    }

}
