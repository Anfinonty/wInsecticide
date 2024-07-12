




bool keydown(int key) //https://stackoverflow.com/questions/47667367/is-there-a-way-to-check-for-shift-held-down-then-control-held-down-for-each-inpu
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}



bool keydownalt()
{
    return (GetAsyncKeyState(VK_RMENU) & 0x8000)!= 0;
}




void GlobalKeypressDown(WPARAM wParam)
{
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




void GlobalKeypressUp (HWND hwnd,WPARAM wParam)
{
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
            if (!hide_taskbar) {
              //ShowWindow(hShellWnd, SW_HIDE); //hide taskbar

              //https://stackoverflow.com/questions/2398746/removing-window-border
              LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
              lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
              SetWindowLong(hwnd, GWL_STYLE, lStyle);


              SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);


              SetForegroundWindow(hwnd); //return back focus

              hide_taskbar=TRUE;
            } else {
              //ShowWindow(hShellWnd, SW_SHOW); //show taskbar again


              LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
              lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
              SetWindowLong(hwnd, GWL_STYLE, lStyle);

              SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);

              SetForegroundWindow(hwnd);
              hide_taskbar=FALSE;
            }
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





void GameKeypressDown(WPARAM wParam)
{
    switch (wParam) {
    //Holding Down Down Arrow or 'S'
      case 'S':
      case VK_DOWN:
        player.rst_down=TRUE;
        break;


    //Holding Down Right Arrow or 'D'
      case 'D':
      case VK_RIGHT:
        player.rst_right=TRUE;
        break;


    //Holding Down Left Arrow or 'A'
      case 'A':
      case VK_LEFT:
        player.rst_left=TRUE;
        break;


    //Holding Down Up Arrow or 'W''
      case 'W':
      case VK_UP:
        player.rst_up=TRUE;
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


    //Holding down 'Z' Key
      case 'Z':
        if (!player.time_breaker && player.time_breaker_units==player.time_breaker_units_max) {
          player.time_breaker=TRUE;
          if (game_audio)
            PlaySound(spamSoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //tb_start_audio
            //PlaySound(tb_start_audio_cache, NULL, SND_MEMORY | SND_ASYNC);
          //}
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
      case VK_DOWN:
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

    //Release Space key
      case ' ': 
        player.sleep_timer=DEFAULT_SLEEP_TIMER;
        break;


    //Release '1' Key
      case '1':
        player.attack_rst=TRUE;
        break;


    //Release '2' Key
      case '2':
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
        break;

    //Release '3' Key'
      case '3':
        player.low_jump=!player.low_jump;
        break;

    //Release 'E' Key
      case 'E':
        if (player.uppercut) {
          player.uppercut=FALSE;
        }
        break;
    }
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
         switch (option_choose) {
           case 0: //Change color of player ++
             player_color=LimitValue(player_color+1,0,COLORS_NUM);
             if (game_audio)
               PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
             break;
           case 1: //Change color of player iris ++
             player_iris_color=LimitValue(player_iris_color+1,0,COLORS_NUM);
             if (game_audio)
               PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
             break;


           case 2: //Change color of player pupil ++
             player_pupil_color=LimitValue(player_pupil_color+1,0,COLORS_NUM);
             if (game_audio)
               PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
             break;


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

/*
           case 6:
             song_volume+=0.01;
             if (song_volume>1.0)//max song volume
               song_volume=0.0;
             flag_adjust_song_audio=TRUE;
             if (game_audio)
               PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
             break;
*/

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


           case 8://togle borders
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
             SetForegroundWindow(hwnd); //return back focus
             hide_taskbar=!hide_taskbar;
             break;


           case 9: //toggle resolution, holding right button
             resolution_choose=LimitValue(resolution_choose+1,0,3);
             if (!hide_taskbar) {
               SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
             } else {
               SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
             }

             SetForegroundWindow(hwnd); //return back focus
             if (game_audio) {
               PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             break;


           case 10: //right button, center
             windowx=SCREEN_WIDTH/2-GR_WIDTH/2;
             windowy=SCREEN_HEIGHT/2-GR_HEIGHT/2;
             if (!hide_taskbar) {
               SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
             } else {
               SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
             }
             SetForegroundWindow(hwnd); //return back focus
             if (game_audio) {
               PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             break;

           case 11: //toggle show fps
             if (game_audio) {
               if (show_fps)
                 PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
               else
                 PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             show_fps=!show_fps;             
             break;
         }
         break;


    //Holding Down Left Arrow or 'A'
      case 'A':
      case VK_LEFT:
        switch (option_choose) {
          case 0: //Change color of player --
            player_color=LimitValue(player_color-1,0,COLORS_NUM);
            if (game_audio)
              PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
            break;
          case 1: //Change color of player iris --
            player_iris_color=LimitValue(player_iris_color-1,0,COLORS_NUM);
            if (game_audio)
              PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
            break;
          case 2: //Change color of player pupil --
            player_pupil_color=LimitValue(player_pupil_color-1,0,COLORS_NUM);
            if (game_audio)
              PlaySound(keySoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //paint
            break;


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

           case 5:
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
/*
           case 6:
             song_volume-=0.01;
             if (song_volume<0.0)
               song_volume=1.0;
             if (game_audio)
               PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
             flag_adjust_song_audio=TRUE;
             break;
*/
           case 6:
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

           case 8://togle borders
             if (game_audio) {
               if (hide_taskbar)
                 PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
               else
                 PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             if (resolution_choose==2) {
               windowx=0;
               windowy=0;
             }
             if (!hide_taskbar) {
               LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
               lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
               SetWindowLong(hwnd, GWL_STYLE, lStyle);
               if (resolution_choose==2)
                 SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
               else
                 SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
             } else {
               LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
               lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
               SetWindowLong(hwnd, GWL_STYLE, lStyle);
               SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
             }
             SetForegroundWindow(hwnd); //return back focus
             hide_taskbar=!hide_taskbar;
             break;


           case 9: //toggle resolution, lower
             resolution_choose=LimitValue(resolution_choose-1,0,3);
             if (!hide_taskbar) {
               SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
             } else {
               SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
             }
             SetForegroundWindow(hwnd); //return back focus
             if (game_audio) {
               PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             break;

           case 10: //left button, corner
             windowx=0;
             windowy=0;
             if (!hide_taskbar) {
               SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
             } else {
               SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose], SWP_FRAMECHANGED);
             }
             SetForegroundWindow(hwnd); //return back focus            
             if (game_audio) {
               PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             break;

           case 11: //toggle show fps
             if (game_audio) {
               if (show_fps)
                 PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
               else
                 PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
             }
             show_fps=!show_fps;             
             break;
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
           main_menu_chosen=-1;

           if (old_player_color!=player_color) {
             DeleteObject(mouse_cursor_sprite_cache);
             mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);

             DeleteObject(mouse_cursor_sprite_cache2);
             mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);
             old_player_color=player_color;
           }


           if (old_player_iris_color!=player_iris_color) {
             DeleteObject(mouse_cursor_sprite_iris_cache);
             mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,draw_color_arr[player_iris_color]);

             DeleteObject(mouse_cursor_sprite_iris_cache2);
             mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,draw_color_arr[player_iris_color]);
             old_player_iris_color=player_iris_color;
           }




           if (old_player_pupil_color!=player_pupil_color) {
             DeleteObject(mouse_cursor_sprite_pupil_cache);
             mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,draw_color_arr[player_pupil_color]);

             DeleteObject(mouse_cursor_sprite_pupil_cache2);
             mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,draw_color_arr[player_pupil_color]);
             old_player_pupil_color=player_pupil_color;


           }

           //adjust volume
           if (old_game_volume!=game_volume) {
              for (int i=0;i<KEY_SFX_NUM;i++) {
                //keySoundEffectCache[i].audio=adjustVolumeA(keySoundEffect[i].audio,keySoundEffect[i].filesize,game_volume);
                adjustSFXVolume(&keySFX[i],game_volume,FALSE);
              }

              old_game_volume=game_volume;
            }

            if (game_audio)
              PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
              //PlaySound(mkey_esc_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //esc
         }
         break;
       case 'A':    //Release LEFT key
       case VK_LEFT:
       case 'D':    //Release RIGHT key
       case VK_RIGHT:
         //LIVE adjust volume
         if (old_game_volume!=game_volume) { //change when not the same
            for (int i=0;i<KEY_SFX_NUM;i++) {
              //keySoundEffectCache[i].audio=adjustVolumeA(keySoundEffect[i].audio,keySoundEffect[i].filesize,game_volume);
             adjustSFXVolume(&keySFX[i],game_volume,FALSE);
            }

            old_game_volume=game_volume;
         }

         //LIVE change color of player
         if (old_player_color!=player_color) { //change when not same

           DeleteObject(mouse_cursor_sprite_cache);
           mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);   
           DeleteObject(mouse_cursor_sprite_cache2);
           mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);

           old_player_color=player_color;
         }


         if (old_player_iris_color!=player_iris_color) {
           DeleteObject(mouse_cursor_sprite_iris_cache);
           mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,draw_color_arr[player_iris_color]);

           DeleteObject(mouse_cursor_sprite_iris_cache2);
           mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,draw_color_arr[player_iris_color]);
           old_player_iris_color=player_iris_color;
         }




         if (old_player_pupil_color!=player_pupil_color) {
           DeleteObject(mouse_cursor_sprite_pupil_cache);
           mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,draw_color_arr[player_pupil_color]);

           DeleteObject(mouse_cursor_sprite_pupil_cache2);
           mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,draw_color_arr[player_pupil_color]);
           old_player_pupil_color=player_pupil_color;
         }
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
     select_main_menu=LimitValue(select_main_menu+1,0,6);
     if (game_audio)
       PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
     break;

  //Holding Down Up Arrow or 'W''
   case 'W':
   case VK_UP:
     //level_chosen=LimitValue(level_chosen-1,0,level_num);
     select_main_menu=LimitValue(select_main_menu-1,0,6);
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
         if (select_main_menu>=0 && select_main_menu<=2) {
           main_menu_chosen=select_main_menu;
           if (game_audio)
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
         }
         if (select_main_menu==5) { //exit
           if (game_audio)
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
           PostQuitMessage(0);
           return 0;
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
         if (player_color>-1 && player_color<COLORS_NUM) {         
           if (game_audio)
             PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
           if (level_chosen>=0 && level_chosen<level_num && main_menu_chosen==0)
             InitLevel(hwnd, hdc);
         }
         break;

     } //End of switch statement
}





void ZeroMenuKeypressUp(WPARAM wParam)
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




void TwoMenuKeypressUp(WPARAM wParam)
{
    switch (wParam) {
       case VK_ESCAPE:
         if ((keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) { //ESC + L/RSHIFT = QUIT
           main_menu_chosen=-1;
           if (game_audio)
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
         }
         break;

        case VK_RETURN:
          break;
    }
}
