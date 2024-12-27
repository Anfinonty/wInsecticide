




bool keydown(int key) //https://stackoverflow.com/questions/47667367/is-there-a-way-to-check-for-shift-held-down-then-control-held-down-for-each-inpu
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}



bool keydownalt()
{
    return (GetAsyncKeyState(VK_RMENU) & 0x8000)!= 0;
}


void KeyChangePlayerColor() 
{
     //LIVE change color of player
     if (old_player_color!=player_color) { //change when not same
       for (int i=0;i<16;i++) {
         FreeDrawSprite(&draw_player_cursor_body[i]);
         DeleteObject(player_cursor_body[i]);
         player_cursor_body[i]=RotateSprite(NULL, player_cursor[i],0,LTGREEN,BLACK,rgbPaint[player_color]/*draw_color_arr[player_color]*/,-1);
         GenerateDrawSprite(&draw_player_cursor_body[i],player_cursor_body[i]);
       }
       old_player_color=player_color;
     }


     if (old_player_iris_color!=player_iris_color) {
       for (int i=0;i<16;i++) {
         FreeDrawSprite(&draw_player_cursor_iris[i]);
         DeleteObject(player_cursor_iris[i]);
         player_cursor_iris[i]=RotateSpriteExclude(NULL, player_cursor[i],0,LTBLUE,rgbPaint[player_iris_color]/*draw_color_arr[player_iris_color]*/);
         GenerateDrawSprite(&draw_player_cursor_iris[i],player_cursor_iris[i]);
       }
       old_player_iris_color=player_iris_color;
     }

     if (old_player_pupil_color!=player_pupil_color) {
       for (int i=0;i<2;i++) {
         FreeDrawSprite(&draw_player_cursor_pupil[i]);
         DeleteObject(player_cursor_pupil[i]);
         if (i==0) {
           player_cursor_pupil[i]=RotateSpriteExclude(NULL, player_cursor[0],0,LTRED,rgbPaint[player_pupil_color]/*draw_color_arr[player_pupil_color]*/);
         } else {
           player_cursor_pupil[i]=RotateSpriteExclude(NULL, player_cursor[8],0,LTRED,rgbPaint[player_pupil_color]/*draw_color_arr[player_pupil_color]*/);
         }
         GenerateDrawSprite(&draw_player_cursor_pupil[i],player_cursor_pupil[i]);
       }

       old_player_pupil_color=player_pupil_color;
     }
}


void ColorKeypressDown(WPARAM wParam, int *dest_color_id)
{
  switch (wParam) {
    case VK_LEFT:
      color_chooser.color_id_choosing--;
      if (game_audio)
        PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint

      if (color_chooser.color_id_choosing<0)
        color_chooser.color_id_choosing+=256;
      break;

    case VK_RIGHT:
      color_chooser.color_id_choosing++;
      if (game_audio)
        PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
      if (color_chooser.color_id_choosing>255)
         color_chooser.color_id_choosing-=256;
      break;

    case VK_UP:
      color_chooser.color_id_choosing-=16;
      if (game_audio)
        PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
      if (color_chooser.color_id_choosing<0)
        color_chooser.color_id_choosing+=256;
      break;

    case VK_DOWN:
      color_chooser.color_id_choosing+=16;
      if (game_audio)
        PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
      if (color_chooser.color_id_choosing>255)
         color_chooser.color_id_choosing-=256;
      break;

    case VK_RETURN:
       color_chooser.color_id=color_chooser.color_id_choosing;
       if (game_audio)
         PlaySound(keySoundEffectCache[6].audio, NULL, SND_MEMORY | SND_ASYNC); //start
       *dest_color_id=color_chooser.color_id_choosing;
       color_chooser.is_choosing_color=FALSE;
       KeyChangePlayerColor();
       break;
  }
}


void ColorKeypressUp(WPARAM wParam, int *dest_color_id)
{
  switch (wParam) {
    case VK_UP:
    case VK_DOWN:
    case VK_LEFT:
    case VK_RIGHT:
    case 'W':
    case 'A':
    case 'S':
    case 'D':
      *dest_color_id=color_chooser.color_id_choosing;
      KeyChangePlayerColor();
      break;


    case VK_ESCAPE: //shift esc no save color
      if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
        color_chooser.is_choosing_color=FALSE;
        if (game_audio)
           PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
        *dest_color_id=color_chooser.color_id;
        KeyChangePlayerColor();
      }
      break;
  }
}


//Keybinds
void GlobalKeypressDown(WPARAM wParam)
{
  if (!MapEditor.is_ground_txt_typing) {
    switch (wParam) {
    //Holding down '9' or '9' Key
      case '9'://skip song, upwnwards (previous)
      case '0'://skip song, downwards (next)
        call_help_timer=0;
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          if (song_mode<=2) {
            skipping_song=TRUE;

            if (wParam=='0')
              song_rand_num=LimitValue(song_rand_num+1,0,song_num);
            else if (wParam=='9')
              song_rand_num=LimitValue(song_rand_num-1,0,song_num);
          }
        }
        if (song_mode>2)
          skipping_song=FALSE;
        break;
    }
  }
}




void GlobalKeypressUp (HWND hwnd,WPARAM wParam)
{
  if (!MapEditor.is_ground_txt_typing) {
    switch (wParam) {
    //Letting go '0' or '9' Key
      case '9':
      case '0':
        call_help_timer=0;        
        if (song_mode<=2) {
          if (skipping_song) {
            if (playing_wav) {
              skip_song=TRUE;
              play_new_song=TRUE;
            }
            skipping_song=FALSE;
          }
        }
        break;
        //Release 'T' key holding SHIFT
        case 'T': //Hide or Show Taskbar
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
             windowx=0;
             windowy=0;
             if (!hide_taskbar) {
               LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
               lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
               SetWindowLong(hwnd, GWL_STYLE, lStyle);
               resolution_choose=0;
             } else {
               resolution_choose=MAX_RESOLUTION_I-1;
               LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
               lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
               SetWindowLong(hwnd, GWL_STYLE, lStyle);
    //           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
             }
             GR_WIDTH=RESOLUTION_X[resolution_choose];
             GR_HEIGHT=RESOLUTION_Y[resolution_choose];
             flag_resolution_change=TRUE;
             SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
             if (!hide_taskbar) {
               SetForegroundWindow(hwnd); //return back focus
             }
             hide_taskbar=!hide_taskbar;
          }
          break;


        //Release N key while holding SHIFT or not
        //Release M key while holding SHIFT or not
        case 'N':
        case 'M':
          call_help_timer=0;
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (wParam=='M') {
              song_mode=LimitValue(song_mode+1,0,4);
            } else if (wParam=='N'){
              song_mode=LimitValue(song_mode-1,0,4);
            }

            if (song_mode==3) {
              if (!stop_playing_song) {
                stop_playing_song=TRUE;
                toggle_stop_playing_song=TRUE;
              }
            } else {
              if (stop_playing_song) {//reenable song
                remove("music/tmp/tmp.wav");
                rmdir("music/tmp"); //remove tmp
                InitSongBank();
                song_rand_num=LimitValue(-1,0,song_num);
                stop_playing_song=FALSE;
                play_new_song=TRUE;
              }
            }

          } else {
            if (!stop_playing_song) {
              play_new_song=TRUE;
              loading_mp3=FALSE;
              loading_flac=FALSE;
              loading_wav=FALSE;
              playing_wav=FALSE;
            }
          }
          break;//end current song

        //Release 'L' Key
        case 'L':
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (yes_unifont) {
              yes_unifont=FALSE;
            } else {
              yes_unifont=TRUE;
            }
          }
          break;
    } //end of global key release
  }
}





void GameKeypressDown(WPARAM wParam)
{
    switch (wParam) {
    //Holding Down Down Arrow or 'S'
      case 'S':
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          if (player.print_current_above && !player.phase_web2) {
            player.phase_web=TRUE;
          }
          if (player.print_current_below && !player.phase_web) {
            player.phase_web2=TRUE;
          }
        } else {
          player.rst_down=TRUE;
        }
        player.rst_up=FALSE;
        break;


      case VK_DOWN:
        player.rst_arrow_down=TRUE;
        break;


    //Holding Down Right Arrow or 'D'
      case 'D':
        player.rst_left=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        player.rst_right=TRUE;        
        break;

      case VK_RIGHT:
        player.rst_arrow_right=TRUE;
        break;


    //Holding Down Left Arrow or 'A'
      case 'A':
        player.rst_right=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        player.rst_left=TRUE;
        break;


      case VK_LEFT:
        player.rst_arrow_left=TRUE;
        break;


    //Holding Down Up Arrow or 'W''
      case 'W':
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          if (player.print_current_below && !player.phase_web) {
            player.phase_web2=TRUE;
          }
          if (player.print_current_above && !player.phase_web2) {
            player.phase_web=TRUE;
          }
        } else {
          player.rst_up=TRUE;
        }
        player.rst_down=FALSE;
        break;

      case VK_UP:
        player.rst_arrow_up=TRUE;
        break;


    //Holding down ENTER key
      case VK_RETURN:
        flag_restart=TRUE;
        break;

    //Holding down SPACE key
      case ' ':
        player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
        break;


    //Holding down 'E' key
      case 'E':
        player.uppercut=TRUE;
        break;

    //Holding down 'C' Key
      case 'C':
        if (player.sleep_timer==DEFAULT_SLEEP_TIMER) {
          player.sleep_timer=SLOWDOWN_SLEEP_TIMER;
        } else {
          player.sleep_timer=DEFAULT_SLEEP_TIMER;
        }
        break;

    //Holding down '4' Key
      case '4':
        player.rst_right_click=TRUE;
        break;

    //'5'
      //case '5':
        //mouse_x=GR_WIDTH/2;
        //mouse_y=GR_HEIGHT/2-100;
        //break;

    //Holding down 'Z' Key
      case 'Z':
        if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
          player.flag_noir_palette=TRUE;
          player.time_breaker=TRUE;
          if (game_audio)
            PlaySound(spamSoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //tb_start_audio
          player.time_breaker_cooldown=player.time_breaker_cooldown_max;
          player.speed+=player.time_breaker_units_max/2-1;
        }
        break;
    } //End of Keys switch statement
}





void GameKeypressUp(WPARAM wParam)
{
    switch (wParam) {
    //Holding Down Shift && Escape
      case VK_ESCAPE:
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
          if (level_loaded) { //allow back to menu only if level is fully loaded
            back_to_menu=TRUE;
          }
        }
        break;


      //Release '8' key holding SHIFT
      case '8':
        call_help_timer=0;
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
          if (!display_controls) {
            display_controls=TRUE;
          } else {
            display_controls=FALSE;
          }
        }
        break;


      //Release Q key
      case 'Q':
        player.destroy_ground=TRUE;
        break;

    //Release S or Down key
      case 'S':
        player.rst_down=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        break;

      case VK_DOWN:
        player.rst_arrow_down=FALSE;
        break;

    //Release D or Right key
      case 'D':
        player.rst_right=FALSE;
        break;

      case VK_RIGHT:
        player.rst_arrow_right=FALSE;
        break;


    //Release A or Left key
      case 'A':
        player.rst_left=FALSE;
        break;
      case VK_LEFT:
        player.rst_arrow_left=FALSE;
        break;

    //Release W or Up key
      case 'W':
        player.rst_up=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        //player.phase_web2=FALSE;
        break;

      case VK_UP:
        player.rst_arrow_up=FALSE;
        break;

    //Release Space key
      case ' ': 
        player.sleep_timer=DEFAULT_SLEEP_TIMER;
        break;


    //Release '1' Key
      case '1':
        player.attack_rst=TRUE;
        break;

    //Release '4' Key    
      case '4':
        player.rst_right_click=FALSE;
        break;

    //Release '2' Key
      case '2':
        switch (player.knives_per_throw) {
          case 1:
            player.play_gun_snd=0;
            player.knives_per_throw=3;
            break;
          case 3:
            player.play_gun_snd=1;
            player.knives_per_throw=5;
            break;
          case 5:
            player.play_gun_snd=2;
            player.knives_per_throw=15;
            break;
          case 15:
            player.play_gun_snd=0;
            player.knives_per_throw=1;
            break;
        }
        PlayerActLoadWeaponSnd();
        break;

    //Release '3' Key'
      case '3':
        player.low_jump=!player.low_jump;
        break;

    //Release 'E' Key
      case 'E':
        player.show_health_timer=HP_SHOW_TIMER_NUM;
        player.show_block_health_timer=HP_SHOW_TIMER_NUM;
        if (player.uppercut) {
          player.uppercut=FALSE;        
        }
        break;
    }
}




void OptionKeyPressRight(HWND hwnd, int option_choose)
{
     switch (option_choose) {
       case 3: //Enable/Disable camera shaking
         if (game_audio) {
           if (game_cam_shake)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         game_cam_shake=!game_cam_shake;                
         break;


       case 4: //Enable/Disable sound effects
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
         else
           PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         game_audio=!game_audio;
         break;


       case 5:
         if (game_volume>=2.0) {
           game_volume+=1.0;
         } else {
           game_volume+=0.1;
         }
         if (game_volume>20.0) {
            game_volume=0.0;
         }
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
         flag_adjust_audio=TRUE;
         break;


       case 6:
         wav_out_volume+=0.1;
         if (wav_out_volume>1.0) { //max song volume
            wav_out_volume=0.0;
         }
         flag_adjust_wav_out_audio=TRUE;
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
         break;


       case 7: //toggle unifont
         if (game_audio) {
           if (yes_unifont)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         yes_unifont=!yes_unifont;
         break;

       case 8:
         if (game_audio) {
           if (game_shadow)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
          }
          game_shadow=!game_shadow;
          break;

       case 9://togle borders
         if (game_audio) {
           if (hide_taskbar)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         if (!hide_taskbar) { //Hide taskbar
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
         } else { //return taskbar
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
         }
         if (!hide_taskbar) {
           SetForegroundWindow(hwnd); //return back focus
         }
         hide_taskbar=!hide_taskbar;
         break;

       case 10: //toggle resolution, holding right button
         resolution_choose=LimitValue(resolution_choose+1,0,MAX_RESOLUTION_I);
         //SetWindowPos(hwnd,HWND_TOPMOST,0,0,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
         //SetForegroundWindow(hwnd); //return back focus
         //LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
         //lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU); //borderless mode
         //SetWindowLong(hwnd, GWL_STYLE, lStyle);
         //SetWindowPos(hwnd,HWND_NOTOPMOST,0,0,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
         if (!hide_taskbar) {
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
         } /*else {
           SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
         }*/
         SetForegroundWindow(hwnd); //return back focus

         GR_WIDTH=RESOLUTION_X[resolution_choose];
         GR_HEIGHT=RESOLUTION_Y[resolution_choose];
         flag_resolution_change=TRUE;
         if (game_audio) {
           PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         break;

       case 11: //right button, center
         if (!hide_taskbar) {
           windowx=SCREEN_WIDTH/2-GR_WIDTH/2;
           windowy=SCREEN_HEIGHT/2-GR_HEIGHT/2;
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
           SetForegroundWindow(hwnd); //return back focus
           if (game_audio) {
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
           }
         }
         break;

       case 12: //toggle show fps
         if (game_audio) {
           if (show_fps)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         show_fps=!show_fps;             
         break;
    }
}



void OptionKeyPressLeft(HWND hwnd,int option_choose)
{
    switch (option_choose) {
      case 3:  //Enable/Disable camera shaking 
        if (game_audio) {
          if (game_cam_shake)
            PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
          else
            PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
        }
        game_cam_shake=!game_cam_shake;                
        break;


      case 4: //Enable/Disable sound effects
        if (game_audio)
          PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
        else
          PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
        game_audio=!game_audio;
        break;

       case 5: //Adjust sfx volume
         if (game_volume>=3.0) {
           game_volume-=1.0;
         } else {
           game_volume-=0.1;
         }
         if (game_volume<0.0) {
            game_volume=20.0;
         }
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
         flag_adjust_audio=TRUE;
         break;

       case 6: //Adjust raw wav sfx
         wav_out_volume-=0.1;
         if (wav_out_volume<0.0) {
           wav_out_volume=1.0;
         }
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
         flag_adjust_wav_out_audio=TRUE;
         break;

       case 7: //toggle unifont
         if (game_audio) {
           if (yes_unifont)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         yes_unifont=!yes_unifont;
         break;

       case 8: //toggle shadows
         if (game_audio) {
           if (game_shadow)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         game_shadow=!game_shadow;
         break;

       case 9://togle borders
         if (game_audio) {
           if (hide_taskbar)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         windowx=0;
         windowy=0;
         if (!hide_taskbar) {
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           resolution_choose=0;
         } else {
           resolution_choose=MAX_RESOLUTION_I-1;
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
//           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
         }
         GR_WIDTH=RESOLUTION_X[resolution_choose];
         GR_HEIGHT=RESOLUTION_Y[resolution_choose];
         flag_resolution_change=TRUE;
         SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
         SetForegroundWindow(hwnd); //return back focus
         hide_taskbar=!hide_taskbar;
         break;


       case 10: //toggle resolution, lower
         resolution_choose=LimitValue(resolution_choose-1,0,MAX_RESOLUTION_I);
         //SetWindowPos(hwnd,HWND_TOPMOST,0,0,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
         //SetForegroundWindow(hwnd); //return back focus
         GR_WIDTH=RESOLUTION_X[resolution_choose];
         GR_HEIGHT=RESOLUTION_Y[resolution_choose];
         if (!hide_taskbar) {
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
         } 
         flag_resolution_change=TRUE;
         if (game_audio) {
           PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         break;

       case 11: //left button, corner
         if (!hide_taskbar) {
           windowx=0;
           windowy=0;
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
           //SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
           SetForegroundWindow(hwnd); //return back focus            
           if (game_audio) {
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
           }
         }
         break;

       case 12: //toggle show fps
         if (game_audio) {
           if (show_fps)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         show_fps=!show_fps;
         break;
    }
}




void OptionKeyPressRelease1()
{
   main_menu_chosen=-1;
   //LIVE change color of player
   //KeyChangePlayerColor();


   //adjust volume
   if (old_game_volume!=game_volume) {
      for (int i=0;i<KEY_SFX_NUM;i++) {
        freeSoundEffectCache(&keySoundEffectCache[i]);
      }
      for (int i=0;i<KEY_SFX_NUM;i++) {
        //keySoundEffectCache[i].audio=adjustVolumeA(keySoundEffect[i].audio,keySoundEffect[i].filesize,game_volume);
        adjustSFXVolume(&keySoundEffectCache[i],&keySoundEffect[i],game_volume,FALSE);
      }

      old_game_volume=game_volume;
    }

    if (game_audio)
      PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
      //PlaySound(mkey_esc_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //esc
}






void OptionKeyPressRelease2()
{
     //LIVE adjust volume
     if (old_game_volume!=game_volume) { //change when not the same
         for (int i=0;i<KEY_SFX_NUM;i++) {
           freeSoundEffectCache(&keySoundEffectCache[i]);
         }
        for (int i=0;i<KEY_SFX_NUM;i++) {
          //keySoundEffectCache[i].audio=adjustVolumeA(keySoundEffect[i].audio,keySoundEffect[i].filesize,game_volume);
         adjustSFXVolume(&keySoundEffectCache[i],&keySoundEffect[i],game_volume,FALSE);
        }

        old_game_volume=game_volume;
     }

     //LIVE change color of player
     //KeyChangePlayerColor();
}




void OptionsKeypressDown(HWND hwnd, WPARAM wParam)
{
    switch (wParam) {
    //Holding Down Up Arrow or 'W''
      case 'W':
      case VK_UP:
        option_choose=LimitValue(option_choose-1,0,GAME_OPTIONS_NUM);
        if (game_audio)
          PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        break;


    //Holding Down Down Arrow or 'S'
       case 'S':
       case VK_DOWN:
         option_choose=LimitValue(option_choose+1,0,GAME_OPTIONS_NUM);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
         break;


    //Holding Down Right Arrow or 'D'
       case 'D':
       case VK_RIGHT:
          OptionKeyPressRight(hwnd,option_choose);
          break;


    //Holding Down Left Arrow or 'A'
      case 'A':
      case VK_LEFT:
        OptionKeyPressLeft(hwnd,option_choose);
        break;

    //Enter
      case VK_RETURN:
        if (option_choose>=0 && option_choose<=2) {
          color_chooser.is_choosing_color=TRUE;
          if (game_audio)
            PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
          switch (option_choose) {
            case 0:
              color_chooser.color_id=
              color_chooser.color_id_choosing=player_color;
              break;
            case 1:
              color_chooser.color_id=
              color_chooser.color_id_choosing=player_iris_color;
              break;
            case 2:
              color_chooser.color_id=
              color_chooser.color_id_choosing=player_pupil_color;
              break;
          }
        }
        break;

   } //End of switch statement for keys
}



void OptionsKeypressUp(WPARAM wParam)
{
    switch (wParam) {
       case '1':           //Release '1' Key or ESC+SHIFT
       case VK_ESCAPE:
         if ((keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) { //ESC + L/RSHIFT = QUIT
           OptionKeyPressRelease1();
         }
         break;
       case 'A':    //Release LEFT key
       case VK_LEFT:
       case 'D':    //Release RIGHT key
       case VK_RIGHT:
         OptionKeyPressRelease2();
         break;
    }
}










void MinusOneMenuKeypressDown(WPARAM wParam)
{
  //Holding Down Shift && Escape
  switch (wParam) {
  //Holding Down Down Arrow or 'S'
   case 'S':
   case VK_DOWN:
     select_main_menu=LimitValue(select_main_menu+1,0,4);
     if (game_audio)
       PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
     break;

  //Holding Down Up Arrow or 'W''
   case 'W':
   case VK_UP:
     //level_chosen=LimitValue(level_chosen-1,0,level_num);
     select_main_menu=LimitValue(select_main_menu-1,0,4);
     if (game_audio)
       PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
     break;
  }
}


int MinusOneMenuKeypressUp(WPARAM wParam)
{
    switch (wParam) {
      //release ENTER key
       case VK_RETURN:
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
         break;

       //case VK_ESCAPE: //release ESC+SHIFT key, exit
       case 'Q': // release Q and shift
         if ((keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) { //ESC + L/RSHIFT = QUIT
           PostQuitMessage(0);
           return 0;
         }
         break;
    }
}








void ZeroMenuKeypressDown( HWND hwnd,  HDC hdc, WPARAM wParam)
{
     switch (wParam) {
      //Holding Down Down Arrow or 'S'
       case 'S':
       case VK_DOWN:
         level_chosen=LimitValue(level_chosen+1,0,level_num);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
           //PlaySound(mkey_down_up_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //up down
         break;

      //Holding Down Up Arrow or 'W''
       case 'W':
       case VK_UP:
         level_chosen=LimitValue(level_chosen-1,0,level_num);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
         break;


      //Holding down ENTER key
       case VK_RETURN:
         if (player_color>-1 && player_color<256) {         
           if (game_audio)
             PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
           if (level_chosen>=0 && level_chosen<level_num && main_menu_chosen==0) {
             flag_load_level=TRUE;
             //level_loading=TRUE;
             //Sleep(1000);
             //InitLevel(hwnd, hdc,FALSE);
             //InitLevel();
           }
         }
         break;

     } //End of switch statement
}





void ZeroMenuKeypressUp( HWND hwnd,  HDC hdc, WPARAM wParam)
{
    switch (wParam) { //release key shift esc
      case VK_ESCAPE:
        if (in_main_menu) {
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //ESC + L/RSHIFT = QUIT
            main_menu_chosen=-1;
           if (game_audio)
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
          }
        }
        break;
      case '1': // Create New Level
        main_menu_chosen=2;
        if (game_audio)
          PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
        break;
      case '2': // Edit Selected Level Limits
        {
        main_menu_chosen=3;
        /*LOAD selected level details to adjust limits*/
        wchar_t txt[128];
        swprintf(txt,128,L"saves/%s/level.txt",level_names[level_chosen]);
        LoadSave(txt,FALSE); //load saves

        wcsncpy(typing_lvl_name,level_names[level_chosen],16);
        typing_lvl_name_pos=lstrlenW(typing_lvl_name);
        set_ground_amount=GROUND_NUM;
        set_enemy_amount=ENEMY_NUM;
        set_map_width_amount=MAP_WIDTH;
        set_map_height_amount=MAP_HEIGHT;


        if (game_audio)
          PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
        }
        break;
      case '3': //Build Selected Level
        if (player_color>-1 && player_color<COLORS_NUM)
        {
        main_menu_chosen=4;
        if (game_audio)
          PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
        flag_load_melevel=TRUE;
          //InitLevelMapEditor();
        }
        break;
    }
}




void TwoMenuKeypressDown(WPARAM wParam)
{
  switch (wParam) {
  //Holding Down Down Arrow or 'S'
    case 'S':
    case VK_DOWN:
       if ((wParam=='S' && create_lvl_option_choose>0) || wParam==VK_DOWN) {
         create_lvl_option_choose=LimitValue(create_lvl_option_choose+1,0,5);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
       }
       break;

  //Holding Down Up Arrow or 'W''
    case 'W':
    case VK_UP:
     //level_chosen=LimitValue(level_chosen-1,0,level_num);
      if ((wParam=='W' && create_lvl_option_choose>0) || wParam==VK_UP) {
        create_lvl_option_choose=LimitValue(create_lvl_option_choose-1,0,5);
        if (game_audio)
          PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
      }
      break;

    case 'A':
    case VK_LEFT:
      if ((wParam=='A' && create_lvl_option_choose>0) || wParam==VK_LEFT) {
        switch (create_lvl_option_choose) {
          case 1: //Max Ground Num Decrease --
            set_ground_amount=LimitValue(set_ground_amount-10,10,MAX_GROUND_NUM+1);
            break;    
          case 2: //Max Enemy Num Decrease --
            set_enemy_amount=LimitValue(set_enemy_amount-1,1,MAX_ENEMY_NUM+1);
            break;    
          case 3: //Max Width Num Decrease --
            set_map_width_amount=LimitValue(set_map_width_amount-160,640,256001);
            break;    
          case 4: //Max Height Num Decrease --
            set_map_height_amount=LimitValue(set_map_height_amount-160,480,192001);
            break;    
        }
        if (create_lvl_option_choose>0) {
          if (game_audio)
            PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
        }
      }
      break;


    case 'D':
    case VK_RIGHT:
      if ((wParam=='D' && create_lvl_option_choose>0) || wParam==VK_RIGHT) {
        switch (create_lvl_option_choose) {
          case 1: //Max Ground Num Increase ++
            set_ground_amount=LimitValue(set_ground_amount+10,10,MAX_GROUND_NUM+1);
            break;    
          case 2: //Max Enemy Num Increase ++
            set_enemy_amount=LimitValue(set_enemy_amount+1,1,MAX_ENEMY_NUM+1);
            break;
          case 3: //Max Width Num Increase ++
            set_map_width_amount=LimitValue(set_map_width_amount+160,640,256001/*MAX_VGRID_NUM*160*/);
            break;
          case 4: //Max Height Num Increase ++
            set_map_height_amount=LimitValue(set_map_height_amount+160,480,192001/*MAX_VGRID_NUM*160*/);
            break;    
        }
        if (create_lvl_option_choose>0) {
          if (game_audio)
            PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
        }
      }
      break;
  }
}





/*
level.txt format
GROUND_NUM
ENEMY_NUM
MAP_WIDTH
MAP_HEIGHT





saved_ground_x1
saved_ground_y1
saved_ground_x2
saved_ground-y2
saved_ground_x3
saved_ground_y3

saved_ground_is_ghost
saved_ground_is_color

saved_ground_type
saved_ground_text






saved_enemy_type
saved_enemy_x
saved_enemy_y

saved_enemy_type_speed
saved_enemy_type_bullet_speed
saved_enemy_type_species
saved_enemy_type_follow_range
saved_enemy_type_unchase_range
saved_enemy_type_chase_range
saved_enemy_type_color
saved_enemy_type_speed
saved_enemy_type_health
saved_enemy_type_shoot_at_player_range
saved_enemy_type_aim_rand

saved_enemy_type_bullet_cooldown
saved_enemy_type_bullet_fire_cooldown
saved_enemy_type_bullet_fire_at_once
saved_enemy_type_bullet_length
saved_enemy_type_bullet_damage
saved_enemy_type_bullet_speed_multiplier
saved_enemy_type_bullet_range
saved_enemy_type_bullet_color
saved_enemy_type_bullet_graphics_type

saved_enemy_type_time_Breaker_rare
saved_enemy_type_time_Breaker_length
saved_enemy_type_time_Breaker_immune







saved_player_x
saved_player_y
map_background
custom_map_background_color
is_inverted
...
*/


//If folder not found
//Create folder
//|_->images folder
//|_->scores folder
//|_->song folder
//|->level.txt

//Else print error (folder exists!)

void TwoMenuKeypressUp(WPARAM wParam)
{
  switch (wParam) {
    case VK_ESCAPE:
      if ((keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) { //ESC + L/RSHIFT = QUIT
        typing_lvl_name_pos=0;
        for (int i=0;i<16;i++)
          typing_lvl_name[i]=0;

        set_ground_amount=10;
        set_enemy_amount=1;
        set_map_width_amount=640;
        set_map_height_amount=480;


        main_menu_chosen=0;
        if (game_audio)
          PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
        }
        break;

    case VK_RETURN: //Create New Levels
    {
      _WDIR *d;
      struct _wdirent *dir;
      wchar_t create_lvl_name_[32];
      swprintf(create_lvl_name_,32,L"saves/%s",typing_lvl_name);
      if (typing_lvl_name_pos>0 && (set_map_width_amount/160*set_map_height_amount/160)<4801) {
        d = _wopendir(create_lvl_name_);
        if (d) {
          _wclosedir(d);
        } else {
          //Create New Folder
          _wmkdir(create_lvl_name_);
          if (game_audio)
            PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start

          SaveNewCreatedLvl(create_lvl_name_);
          GetSavesInDir(L"saves");


          typing_lvl_name_pos=0;
          for (int i=0;i<16;i++)
            typing_lvl_name[i]=0;

          set_ground_amount=10;
          set_enemy_amount=1;
          set_map_width_amount=640;
          set_map_height_amount=480;

          
          main_menu_chosen=0;
        }
      } else {         
        //printf("Level Not Found\n");
      }
    }
    break;
  }
}




void ThreeMenuKeypressDown(WPARAM wParam)
{
  TwoMenuKeypressDown(wParam);
}




void ThreeMenuKeypressUp(WPARAM wParam, HWND hwnd, HDC hdc)
{
  switch (wParam) {
    case VK_ESCAPE:
      if ((keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) { //ESC + L/RSHIFT = QUIT
        if (game_audio)
          PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc

        main_menu_chosen=0;


        //free saved grounds pointer & Ground
        free(saved_ground_is_ghost);
        free(saved_ground_color);
        free(saved_ground_type);

        //free enemy
        free(saved_enemy_type);
        free(saved_enemy_x);
        free(saved_enemy_y);


        free(saved_ground_x1);
        free(saved_ground_y1);
        free(saved_ground_x2);
        free(saved_ground_y2);
        free(saved_ground_x3);
        free(saved_ground_y3);
        for (int i=0;i<GROUND_NUM;i++) {
          free(saved_ground_text[i]);
        }
        free(saved_ground_text);




        typing_lvl_name_pos=0;
        for (int i=0;i<16;i++)
          typing_lvl_name[i]=0;

        set_ground_amount=10;
        set_enemy_amount=1;
        set_map_width_amount=640;
        set_map_height_amount=480;

        }
        break;


    case VK_RETURN: //Release Enter Key
    {
      if (typing_lvl_name_pos>0 && (set_map_width_amount/160*set_map_height_amount/160)<4801) {
        if (game_audio)
          PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start

        SaveNewLimitAdjustedLvl(hwnd, hdc);

        //free enemy
        //free saved grounds pointer & Ground
        free(saved_ground_is_ghost);
        free(saved_ground_color);
        free(saved_ground_type);

        free(saved_enemy_type);
        free(saved_enemy_x);
        free(saved_enemy_y);

        free(saved_ground_x1);
        free(saved_ground_y1);
        free(saved_ground_x2);
        free(saved_ground_y2);
        free(saved_ground_x3);
        free(saved_ground_y3);

        for (int i=0;i<GROUND_NUM;i++) {
          free(saved_ground_text[i]);
        }
        free(saved_ground_text);

        //reset menu values
        typing_lvl_name_pos=0;
        for (int i=0;i<16;i++)
          typing_lvl_name[i]=0;

        set_ground_amount=10;
        set_enemy_amount=1;
        set_map_width_amount=640;
        set_map_height_amount=480;
        
        main_menu_chosen=0;
      }
    }
    break;
  }
}
