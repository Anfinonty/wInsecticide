


bool rst_space=FALSE;

bool keydown(int key) //https://stackoverflow.com/questions/47667367/is-there-a-way-to-check-for-shift-held-down-then-control-held-down-for-each-inpu
{
    return (GetAsyncKeyState(key) & 0x8000) != 0;
}



bool keydownalt()
{
    return (GetAsyncKeyState(VK_RMENU) & 0x8000)!= 0;
}

/*bool keydownlalt()
{
    return (GetAsyncKeyState(VK_LMENU) & 0x8000)!= 0;
}*/


void KeyChangePlayerColor() 
{
     bool c=FALSE;
     //LIVE change color of player
     if (old_player_color!=player_color) { //change when not same
       old_player_color=player_color;
       c=TRUE;
     }

     if (old_player_iris_color!=player_iris_color) {
       old_player_iris_color=player_iris_color;
       c=TRUE;
     }

     if (old_player_pupil_color!=player_pupil_color) {
       old_player_pupil_color=player_pupil_color;
       c=TRUE;
     }

     if (c) {
       player_load_iris_color=player_iris_color;
       player_load_color=
       player.load_color=player_color;
       player.flag_revert_palette=TRUE;
       player.time_breaker_tick=0;
       CopyReplaceColorPalette(PlayerSprite[0].PlayerPalette,rgbColorsDefault,167,rgbPaint[player_color]);
       CopyReplaceColorPalette(PlayerSprite[0].PlayerPalette,PlayerSprite[0].PlayerPalette,151,LTGRAY); //border
       CopyReplaceColorPalette(PlayerSprite[0].PlayerPalette,PlayerSprite[0].PlayerPalette,199,rgbPaint[player_iris_color]);

       CopyReplaceColorPalette(PlayerSprite[0].PlayerBlurPalette,rgbColorsDefault,167,rgbPaint[player_color]);
       CopyReplaceColorPalette(PlayerSprite[0].PlayerBlurPalette,PlayerSprite[0].PlayerBlurPalette,151,LTGRAY); //border
       CopyReplaceColorPalette(PlayerSprite[0].PlayerBlurPalette,PlayerSprite[0].PlayerBlurPalette,199,rgbPaint[player_pupil_color]);

       CopyReplaceColorPalette(player_cursor_palette,rgbColorsDefault,167,rgbPaint[player_color]); //LTGREEN
       CopyReplaceColorPalette(player_cursor_palette,player_cursor_palette,151,rgbPaint[player_iris_color]); //LTBLUE
       CopyReplaceColorPalette(player_cursor_palette,player_cursor_palette,199,rgbPaint[player_pupil_color]);//LTRED
       if (blank_level) {
         flag_load_player_sprite=TRUE;
       }
    }
}

void TaskbarChangeAct(HWND hwnd)
{
     windowx=0;
     windowy=0;
     //resolution_choose=1;
     GR_WIDTH=RESOLUTION_X[resolution_choose];
     GR_HEIGHT=RESOLUTION_Y[resolution_choose];
     if (!hide_taskbar) {
       LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
       lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
       SetWindowLong(hwnd, GWL_STYLE, lStyle);
       SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
     } else {
       LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
       lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
       SetWindowLong(hwnd, GWL_STYLE, lStyle);
       SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
       ShowWindow(hwnd, SW_RESTORE);
     }
     if (!hide_taskbar) {
       SetForegroundWindow(hwnd); //return back focus
     }
    // flag_resolution_change=TRUE;
     hide_taskbar=!hide_taskbar;
     if (!hide_taskbar) {
       SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
       ShowWindow(hwnd, SW_RESTORE);
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
  /*
        '6'+SHIFT:  change buffer rate

        hWaveOut[2]: '7'[gct=0], '7'[gct=1];*****
        hWaveOut[6]: '8'[gct=0], '8'[gct=1];*****
        '7''8':          Play Sample*****
        '7''8' + SHIFT:  SAVE SAMPLE*****
        

                            SHIFT+'9': song--; SHIFT+'0': song++;

        R: switch wav

                                    I: Fast Forwards
                                        K: Rewind

                            H+SHIFT: jump pos1, save pos1          L: Start Loop position/End Loop position [LET GO] //discards old loops
                                J+SHIFT: jump pos2, save pos2      L+SHIFT: PLAY LOOP/STOP LOOP [LET GO]

        VB: decrease,increaase tempo
        VB+SHIFT: decrease,increase sample persec, nightcore


                    N+SHIFT: next song, change song mode++  M+SHIFT: next song, change song mode--



 */


  if (!MapEditor.is_ground_txt_typing) {
    switch (wParam) {
      case ' ':
        rst_space=TRUE;
        break;
      case 'L':
        if (!(keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) { //play loop no shift
          if (!audioData[gct].play_loop) {            
            audioData[gct].play_loop=TRUE;
          } else {
            audioData[gct].play_loop=FALSE;
          }
        } else {//record loop/stop recording loop
          if (!audioData[gct].record_loop) {
            audioData[gct].record_loop=TRUE;
            audioData[gct].loop_start=audioData[gct].current_filesize;
            audioData[gct].loop_read=audioData[gct].queue_read_buffer;
            audioData[gct].loop_play=audioData[gct].queue_play_buffer;
            audioData[gct].saved_loop_double_buffer=audioData[gct].double_buffer;
          } else  {
            audioData[gct].record_loop=FALSE;
            audioData[gct].loop_end=audioData[gct].current_filesize;
          }
        }
        break;
      case 'P': //pause
        if (!song_pause[gct]) {
          song_pause[gct]=TRUE;
          LiveWavePause(gct);
        } else {
          song_pause[gct]=FALSE;
          LiveWaveResume(gct);
        }
        break;
      case 'I': //fast forward
      case 'K': //rewind
      {
        if (!stop_playing_song[gct] && audioData[gct].playing_wav) {
        audioData[gct].song_rewind=TRUE;
        if (audioData[gct].current_filesize>audioData[gct].read_size*2 && audioData[gct].current_filesize<audioData[gct].filesize-audioData[gct].read_size*2) { //stutters when rewinding, hardware
            if (wParam=='K') {
              if (audioData[gct].queue_read_buffer>0) {audioData[gct].queue_read_buffer--;} else {audioData[gct].queue_read_buffer=READ_BUFFER_NUM-1;}
              if (audioData[gct].queue_play_buffer>0) {audioData[gct].queue_play_buffer--;} else {audioData[gct].queue_play_buffer=READ_BUFFER_NUM-1;} 
              audioData[gct].current_filesize-=audioData[gct].read_size;
            } else {
              if (audioData[gct].queue_read_buffer<=18) {audioData[gct].queue_read_buffer++;} else {audioData[gct].queue_read_buffer=0;}
              if (audioData[gct].queue_play_buffer<=18) {audioData[gct].queue_play_buffer++;} else {audioData[gct].queue_play_buffer=0;} 
              audioData[gct].current_filesize+=audioData[gct].read_size;
            }

            //memcpy(audioData[gct].buffer1,audioData[gct].read_buffer[audioData[gct].queue_play_buffer],audioData[gct].read_size);
            //memcpy(audioData[gct].buffer2,audioData[gct].read_buffer[audioData[gct].queue_play_buffer],audioData[gct].read_size);
            adjustBufferVol(audioData[gct].buffer1,audioData[gct].read_buffer[audioData[gct].queue_play_buffer],audioData[gct].read_size,audioData[gct].volume);
            adjustBufferVol(audioData[gct].buffer2,audioData[gct].read_buffer[audioData[gct].queue_play_buffer],audioData[gct].read_size,audioData[gct].volume);

            audioData[gct].read_filesize=audioData[gct].current_filesize-(audioData[gct].read_size*READ_BUFFER_NUM/2);

            fseek(audioData[gct].music_file, audioData[gct].current_filesize, SEEK_SET);
            fread(audioData[gct].read_buffer[audioData[gct].queue_read_buffer], sizeof(BYTE), audioData[gct].read_size, audioData[gct].music_file);  //copy then go backwards/forwards
            if (audioData[gct].queue_read_buffer>0) {
              PassFilter(audioData[gct].read_buffer[audioData[gct].queue_read_buffer],audioData[gct].read_buffer[audioData[gct].queue_read_buffer-1],&audioData[gct],chosen_buffer_length);
            } else {
              PassFilter(audioData[gct].read_buffer[0],audioData[gct].read_buffer[19],&audioData[gct],chosen_buffer_length);
            }


            fseek(audioData[gct].music_file, audioData[gct].current_filesize, SEEK_SET);
        }
        }
        }
        break;


      case 'J': //jump1
        if (!stop_playing_song[gct] && audioData[gct].playing_wav) {
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //save jumpto
          audioData[gct].jump1=audioData[gct].current_filesize; //spindle, not read
          audioData[gct].saved_play_buffer1=audioData[gct].queue_play_buffer;
          audioData[gct].saved_read_buffer1=audioData[gct].queue_read_buffer;
          audioData[gct].saved_double_buffer=audioData[gct].double_buffer;
        } else { //jump to
          audioData[gct].current_filesize=audioData[gct].jump1;
          audioData[gct].queue_play_buffer=audioData[gct].saved_play_buffer1;
          audioData[gct].queue_read_buffer=audioData[gct].saved_read_buffer1;
          audioData[gct].double_buffer=audioData[gct].saved_double_buffer;
          audioData[gct].read_filesize=audioData[gct].current_filesize-(audioData[gct].read_size*READ_BUFFER_NUM/2);
          fseek(audioData[gct].music_file, audioData[gct].read_filesize, SEEK_SET);
          InitAudioBuffer(gct);
        }
        }
        break;



      case 'H': //jump2
        if (!stop_playing_song[gct] && audioData[gct].playing_wav) {
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) { //save jump
          audioData[gct].jump2=audioData[gct].current_filesize;//spindle, not read
          audioData[gct].saved_play_buffer2=audioData[gct].queue_play_buffer;
          audioData[gct].saved_read_buffer2=audioData[gct].queue_read_buffer;
          audioData[gct].saved_double_buffer=audioData[gct].double_buffer;
        } else { //jump to
          audioData[gct].current_filesize=audioData[gct].jump2; //to read filesize
          audioData[gct].queue_play_buffer=audioData[gct].saved_play_buffer2;
          audioData[gct].queue_read_buffer=audioData[gct].saved_read_buffer2;
          audioData[gct].double_buffer=audioData[gct].saved_double_buffer;
          audioData[gct].read_filesize=audioData[gct].current_filesize-(audioData[gct].read_size*READ_BUFFER_NUM/2);
          fseek(audioData[gct].music_file, audioData[gct].read_filesize, SEEK_SET);
          InitAudioBuffer(gct);
        }
        }
        break;



      case 'V':
        if (!in_map_editor) {
        if (!(keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) {
          if (audioData[gct].sps_offset>-audioData[gct].sps_o) {
             LiveWaveClose(gct);
             audioData[gct].sps_offset-=audioData[gct].sps_o/20;
             audioData[gct].awfx_music.nSamplesPerSec=audioData[gct].sps_o+audioData[gct].sps_offset;
             LiveWaveReOpen(gct);
             BelieveWaveClose();
             BelieveWaveReOpen(gct);
          }
        } else {
          if (audioData[gct].tempo>1.0) {
             audioData[gct].tempo-=0.25;
          }
        }
        }
        break;


      case 'B':
        if (!in_map_editor) {
        if (!(keydown(VK_LSHIFT) || keydown(VK_RSHIFT))) {
          if (audioData[gct].sps_offset<audioData[gct].sps_o*2) {
             LiveWaveClose(gct);
             audioData[gct].sps_offset+=audioData[gct].sps_o/20;
             audioData[gct].awfx_music.nSamplesPerSec=audioData[gct].sps_o+audioData[gct].sps_offset;
             LiveWaveReOpen(gct);
             BelieveWaveClose();
             BelieveWaveReOpen(gct);
          }
        } else {
          if (audioData[gct].tempo<2.0) {
             audioData[gct].tempo+=0.25;
          }
        }
        }
        break;


      //take sample
      case '7':{
        int iw=audioData[gct].queue_play_buffer;
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          for (int w=0;w<4;w++) {
            iw+=w;
            if (iw>19) {
              iw=0;
            }
            //memcpy(audioData[gct].sample1+
              //  (audioData[gct].read_size/2*w),audioData[gct].read_buffer[iw],audioData[gct].read_size);
            adjustBufferVol(audioData[gct].sample1+
                (audioData[gct].read_size/2*w),audioData[gct].read_buffer[iw],audioData[gct].read_size,audioData[gct].volume);

          }
        } else { //play sample
          waveOutReset(hWaveOut[2]);
          waveOutWrite(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
        }}
        break;

      case '8':{
        int iw=audioData[gct].queue_play_buffer;
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          for (int w=0;w<4;w++) {
            iw+=w;
            if (iw>19) {
              iw=0;
            }
            //memcpy(audioData[gct].sample2+
              //  (audioData[gct].read_size/2*w),audioData[gct].read_buffer[iw],audioData[gct].read_size);
            adjustBufferVol(audioData[gct].sample2+
                (audioData[gct].read_size/2*w),audioData[gct].read_buffer[iw],audioData[gct].read_size,audioData[gct].volume);
          }
        } else { //play sample
          waveOutReset(hWaveOut[6]);
          waveOutWrite(hWaveOut[6], &whdr[6], sizeof(WAVEHDR));
        }}
        break;

    //Holding down '9' or '9' Key
      case '9'://skip song, upwnwards (previous)
      case '0'://skip song, downwards (next)
        call_help_timer=0;
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          if (song_mode[gct]<=2) {
            skipping_song[gct]=TRUE;

            if (wParam=='0')
              song_rand_num[gct]=LimitValue(song_rand_num[gct]+1,0,song_num);
            else if (wParam=='9')
              song_rand_num[gct]=LimitValue(song_rand_num[gct]-1,0,song_num);

            for (int w=0; w<48; w++) {
              print_song_playing[gct][w] = song_names[song_rand_num[gct]][w]; 
            }

          }
        }
        if (song_mode[gct]>2)
          skipping_song[gct]=FALSE;
        break;
    }
  }
}




void GlobalKeypressUp (HWND hwnd,WPARAM wParam)
{
  if (!MapEditor.is_ground_txt_typing) {
    switch (wParam) {
      case ' ':
        rst_space=FALSE;
        break;
      case 'R':
        switch (gct) { //switch buffer used
          case 0:   
            gct=1;
            break;
          case 1: 
            gct=0;
            break;
        }
        BelieveWaveClose();
        BelieveWaveReOpen(gct);
        break;

      case 'Y':
        hide_mm=!hide_mm;
        break;

      case 'U':
        if (wav_mode==3) {
          wav_mode=0;
        } else {
          wav_mode++;
        }
        break;

      case '6': //adjust buffer size rate
        if (audioData[0].playing_wav || audioData[1].playing_wav) {
        if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
          int a=casbs_i;a++;if (a==3) {a=0;}
          casbs_i=a;
          chosen_buffer_length_o=buffer_length_arr[casbs_i];
          chosen_buffer_size_o=buffer_size_arr[casbs_i];
          chosen_buffer_length=buffer_length_arr[casbs_i];
          chosen_buffer_size=buffer_size_arr[casbs_i];

          LiveWaveClose(gct);
          LiveWaveReOpen(gct);          
        }
        }
        break;

      case 'I':
      case 'K':
        audioData[gct].song_rewind=FALSE;
        if (!stop_playing_song[gct] && audioData[gct].playing_wav) {
          InitAudioBuffer(gct);
        }
        break;

    //Letting go '0' or '9' Key
      case '9':
      case '0':
        call_help_timer=0;        
        if (song_mode[gct]<=2) {
          if (skipping_song[gct]) {
             play_new_song[gct]=TRUE;
             loading_mp3[gct]=FALSE;
             loading_flac[gct]=FALSE;
             loading_wav[gct]=FALSE;
             audioData[gct].playing_wav=FALSE;
          }
        }
        break;
        //Release 'T' key holding SHIFT
        case 'T': //Hide or Show Taskbar
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
             TaskbarChangeAct(hwnd);
          }
          break;


        //Release N key while holding SHIFT or not
        //Release M key while holding SHIFT or not
        case 'N':
        case 'M':
          call_help_timer=0;
          if (keydown(VK_LSHIFT) || keydown(VK_RSHIFT)) {
            if (wParam=='M') {
              song_mode[gct]=LimitValue(song_mode[gct]+1,0,4);
            } else if (wParam=='N'){
              song_mode[gct]=LimitValue(song_mode[gct]-1,0,4);
            }

            if (song_mode[gct]==3) { //stop playing song
              bool allow_act=FALSE;
              switch (gct) {
                case 0:
                  if (stop_playing_song[1]) {//[0] is not
                    allow_act=TRUE;
                  } else {
                    if (wParam=='M') {
                      song_mode[gct]=LimitValue(song_mode[gct]+1,0,4);
                    } else if (wParam=='N'){
                      song_mode[gct]=LimitValue(song_mode[gct]-1,0,4);
                    }
                  }
                  break;
                case 1:
                  allow_act=TRUE;
                  break;
              }
              if (!stop_playing_song[gct] && allow_act) {
                stop_playing_song[gct]=TRUE;
                toggle_stop_playing_song[gct]=TRUE;
              }


            } else {
              //[1] cannot be renabled unless [0] is reenabled, idk why but this workaround resolves it
              //[0] must always bee opened first, [1] cannot be openned first
              bool allow_act=FALSE;
              switch (gct) {
                case 0:
                  allow_act=TRUE;
                  break;
                case 1: //only opened when 0 is running
                  if (!stop_playing_song[0]) {//[0] is playing
                    allow_act=TRUE;
                  }
                  break;
              }
              if (allow_act) {
                  if (stop_playing_song[gct]) {//reenable song
                    switch (gct) {
                      case 0:
                        remove("music_tmp/tmp/tmp.wav");
                        rmdir("music_tmp/tmp"); //remove tmp
                        break;
                      case 1:
                        remove("music_tmp/tmp2/tmp.wav");
                        rmdir("music_tmp/tmp2"); //remove tmp
                        break;
                    }
                    InitSongBank();
                    if (gct==0) {
                      song_rand_num[gct]=LimitValue(-1,0,song_num);
                    } else {
                      song_rand_num[gct]=song_num-1;
                    }
                    stop_playing_song[gct]=FALSE;
                    play_new_song[gct]=TRUE;
                  }
              } 
            }

          } else { //not holding shift
            if (!stop_playing_song[gct]) {
              play_new_song[gct]=TRUE;
              loading_mp3[gct]=FALSE;
              loading_flac[gct]=FALSE;
              loading_wav[gct]=FALSE;
              audioData[gct].playing_wav=FALSE;
            }
          }          
          break;//end current song

        //Release '5' Key
        case '5':
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


void MMKeypressDown(WPARAM wParam)
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

    //Holding Down Right Arrow or 'D'
      case 'D':
        player.rst_left=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        player.rst_right=TRUE;        
        break;


    //Holding Down Left Arrow or 'A'
      case 'A':
        player.rst_right=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        player.rst_left=TRUE;
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
  }
}



void MMKeypressUp(WPARAM wParam)
{
    switch (wParam) {
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

    //Release D or Right key
      case 'D':
        player.rst_right=FALSE;
        break;


    //Release A or Left key
      case 'A':
        player.rst_left=FALSE;
        break;

    //Release W or Up key
      case 'W':
        player.rst_up=FALSE;
        player.phase_web=FALSE;
        player.phase_web2=FALSE;
        break;

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


    //Holding down '1' key
      case '1':
        player.rst_key_1=TRUE;
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
          if (player.type==0)
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
        player.rst_key_1=FALSE;
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
        } else {
          player.uppercut=TRUE;
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


       /*case 6:
         wav_out_volume+=0.1;
         if (wav_out_volume>1.0) { //max song volume
            wav_out_volume=0.0;
         }
         flag_adjust_wav_out_audio=TRUE;
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
         break;*/


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

         windowx=0;
         windowy=0;
         //resolution_choose=0;
         GR_WIDTH=RESOLUTION_X[resolution_choose];
         GR_HEIGHT=RESOLUTION_Y[resolution_choose];

         if (!hide_taskbar) { //Hide taskbar
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
         } else { //return taskbar
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
           ShowWindow(hwnd, SW_RESTORE);
         }
         if (!hide_taskbar) {
           SetForegroundWindow(hwnd); //return back focus
         }
         //flag_resolution_change=TRUE;
         hide_taskbar=!hide_taskbar;
         if (!hide_taskbar) {
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
           ShowWindow(hwnd, SW_RESTORE);
         }
         break;

       case 10: //toggle resolution, holding right button
         resolution_choose=LimitValue(resolution_choose+1,0,MAX_RESOLUTION_I);
         windowx=0;
         windowy=0;
         if (!hide_taskbar) {
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
           ShowWindow(hwnd, SW_RESTORE);
         }
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


       case 13: //toggle show hijiri
         if (game_audio) {
           if (show_hijiri)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         show_hijiri=!show_hijiri;
         break;




       case 14: //toggle difficulty
         if (game_audio) {
           if (game_hard)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         game_hard=!game_hard;
         flag_difficulty_change=TRUE;
         break;

       case 15: //toggle free will
         if (game_audio) {
           if (free_will)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         free_will=!free_will;
         if (!free_will) {
          for (int i=0;i<ENEMY_TYPE_NUM;i++) {
            CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,199,LTRED); 
          }
         } else {
          for (int i=0;i<ENEMY_TYPE_NUM;i++) {
            CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,199,LTCYAN); 
          }
         }
         player.time_breaker_tick=0;
         player.flag_revert_palette=TRUE;
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

       /*case 6: //Adjust raw wav sfx
         wav_out_volume-=0.1;
         if (wav_out_volume<0.0) {
           wav_out_volume=1.0;
         }
         if (game_audio)
           PlaySound(keySoundEffectCache[2].audio, NULL, SND_MEMORY | SND_ASYNC); //false
         flag_adjust_wav_out_audio=TRUE;
         break;*/

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
         //resolution_choose=1;
         GR_WIDTH=RESOLUTION_X[resolution_choose];
         GR_HEIGHT=RESOLUTION_Y[resolution_choose];
         if (!hide_taskbar) {
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           SetWindowPos(hwnd,HWND_TOPMOST,windowx,windowy,SCREEN_WIDTH,SCREEN_HEIGHT, SWP_FRAMECHANGED);
         } else {
           LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
           lStyle |= (WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
           SetWindowLong(hwnd, GWL_STYLE, lStyle);
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
           ShowWindow(hwnd, SW_RESTORE);
         }
         SetForegroundWindow(hwnd); //return back focus
         flag_resolution_change=TRUE;
         hide_taskbar=!hide_taskbar;

         if (!hide_taskbar) {
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,GR_WIDTH,GR_HEIGHT, SWP_FRAMECHANGED);
           ShowWindow(hwnd, SW_RESTORE);
         }
         break;


       case 10: //toggle resolution, lower
         resolution_choose=LimitValue(resolution_choose-1,0,MAX_RESOLUTION_I);
         windowx=0;
         windowy=0;
         GR_WIDTH=RESOLUTION_X[resolution_choose];
         GR_HEIGHT=RESOLUTION_Y[resolution_choose];
         if (!hide_taskbar) {
           SetWindowPos(hwnd,HWND_NOTOPMOST,windowx,windowy,RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],SWP_FRAMECHANGED);
           ShowWindow(hwnd, SW_RESTORE);
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


       case 13: //toggle show hijiri
         if (game_audio) {
           if (show_hijiri)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         show_hijiri=!show_hijiri;
         break;

       case 14: //toggle difficulty
         if (game_audio) {
           if (game_hard)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         game_hard=!game_hard;
         flag_difficulty_change=TRUE;
         break;


       case 15: //toggle free will
         if (game_audio) {
           if (free_will)
             PlaySound(keySoundEffectCache[2].audio,NULL,SND_MEMORY | SND_ASYNC); //false
           else
             PlaySound(keySoundEffectCache[3].audio,NULL,SND_MEMORY | SND_ASYNC); //true
         }
         free_will=!free_will;
         if (!free_will) {
          for (int i=0;i<ENEMY_TYPE_NUM;i++) {
            CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,199,LTRED); 
          }
         } else {
          for (int i=0;i<ENEMY_TYPE_NUM;i++) {
            CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,199,LTCYAN); 
          }
         }
         player.time_breaker_tick=0;
         player.flag_revert_palette=TRUE;
         break;


    }
}




void OptionKeyPressRelease1()
{
   main_menu_chosen=-1;
   //LIVE change color of player
   //KeyChangePlayerColor();
   SaveOptions();

   //adjust volume
   if (old_game_volume!=game_volume) {
      CleanupGameAudio();
      AdjustGameAudio();
      for (int i=0;i<KEY_SFX_NUM;i++) {
        freeSoundEffectCache(&keySoundEffectCache[i]);
      }
      for (int i=0;i<KEY_SFX_NUM;i++) {
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
        CleanupGameAudio();
        AdjustGameAudio();
         for (int i=0;i<KEY_SFX_NUM;i++) {
           freeSoundEffectCache(&keySoundEffectCache[i]);
         }
        for (int i=0;i<KEY_SFX_NUM;i++) {
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
      //case 'W':
      case VK_UP:
        option_choose=LimitValue(option_choose-1,0,GAME_OPTIONS_NUM);
        if (option_choose==6) { //skip sound
          option_choose=5;
        }
        if (game_audio)
          PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
        break;


    //Holding Down Down Arrow or 'S'
       //case 'S':
       case VK_DOWN:
         option_choose=LimitValue(option_choose+1,0,GAME_OPTIONS_NUM);
        if (option_choose==6) {
          option_choose=7;
        }
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
         break;


    //Holding Down Right Arrow or 'D'
       //case 'D':
       case VK_RIGHT:
          OptionKeyPressRight(hwnd,option_choose);
          break;


    //Holding Down Left Arrow or 'A'
      //case 'A':
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
       //case 'A':    //Release LEFT key
       case VK_LEFT:
       //case 'D':    //Release RIGHT key
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
   //case 'S':
   case VK_DOWN:
     select_main_menu=LimitValue(select_main_menu+1,0,4);
     if (game_audio)
       PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
     break;

  //Holding Down Up Arrow or 'W''
   //case 'W':
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
           if (game_audio) {
             PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
            }
            if (level_loaded) {
              SaveOptions();
            }
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
          if (level_loaded) {
            SaveOptions();
          }
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
       //case 'S':
       case VK_DOWN:
         level_chosen=LimitValue(level_chosen+1,0,level_num);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
           //PlaySound(mkey_down_up_audio_cache, NULL, SND_MEMORY | SND_ASYNC); //up down
         break;

      //Holding Down Up Arrow or 'W''
       //case 'W':
       case VK_UP:
         level_chosen=LimitValue(level_chosen-1,0,level_num);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
         break;


      //Holding down ENTER key
       case VK_RETURN:
         if (player_color>-1 && player_color<256 && level_loaded) {         
           if (game_audio)
             PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
           if (level_chosen>=0 && level_chosen<level_num && main_menu_chosen==0) {
             flag_load_level=TRUE;
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
        flag_load_esll=TRUE;
        if (game_audio)
          PlaySound(keySoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //esc
        }
        break;


      case '3': //Build Selected Level
        if (player_color>-1 && player_color<256)
        {
          if (game_audio)
            PlaySound(keySoundEffectCache[0].audio, NULL, SND_MEMORY | SND_ASYNC); //start
          main_menu_chosen=4;            
          flag_load_melevel=TRUE;
        }
        break;
    }
}




void TwoMenuKeypressDown(WPARAM wParam)
{
  switch (wParam) {
  //Holding Down Down Arrow or 'S'
    //case 'S':
    case VK_DOWN:
       if ((wParam=='S' && create_lvl_option_choose>0) || wParam==VK_DOWN) {
         create_lvl_option_choose=LimitValue(create_lvl_option_choose+1,0,5);
         if (game_audio)
           PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
       }
       break;

  //Holding Down Up Arrow or 'W''
    //case 'W':
    case VK_UP:
     //level_chosen=LimitValue(level_chosen-1,0,level_num);
      if ((wParam=='W' && create_lvl_option_choose>0) || wParam==VK_UP) {
        create_lvl_option_choose=LimitValue(create_lvl_option_choose-1,0,5);
        if (game_audio)
          PlaySound(keySoundEffectCache[1].audio,NULL,SND_MEMORY | SND_ASYNC); //up down
      }
      break;

    //case 'A':
    case VK_LEFT:
      //if ((wParam=='A' && create_lvl_option_choose>0) || wParam==VK_LEFT) {
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
      //}
      break;


    //case 'D':
    case VK_RIGHT:
      //if ((wParam=='D' && create_lvl_option_choose>0) || wParam==VK_RIGHT) {
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
      //}
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
      swprintf(create_lvl_name_,32,L"saves/%ls",typing_lvl_name);
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

            flag_update_background=TRUE;
            InitLevel(FALSE);
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

        flag_update_background=TRUE;
        InitLevel(FALSE);
      }
    }
    break;
  }
}



void DJKeys(WPARAM wParam)
{
  switch (wParam) {
    case ',': //decrease volume
      if (audioData[gct].volume>0.1) {
        audioData[gct].volume-=0.1;
      }
      break;
    case '.': //increase volum
      if (audioData[gct].volume<9.9) {
        audioData[gct].volume+=0.1;
      }
      break;
    case '<': //decrease volume
      if (audioData[gct].volume>0.5) {
        audioData[gct].volume-=0.5;
      } else {
        audioData[gct].volume=0;
      }
      break;

    case '>': //increase volume
      if (audioData[gct].volume<9.5) {
        audioData[gct].volume+=0.5;
      } else {
        audioData[gct].volume=10.0;
      }
      break;




    case '{':
      if (audioData[gct].HIGH_CUTOFF_FREQUENCY>2500.0) {
        audioData[gct].HIGH_CUTOFF_FREQUENCY-=500.0;
      } else {
        if (audioData[gct].HIGH_CUTOFF_FREQUENCY>20.0) {
          audioData[gct].HIGH_CUTOFF_FREQUENCY-=20.0;
        } else {
          audioData[gct].HIGH_CUTOFF_FREQUENCY=19500.0;
        }
      }
      break;

    case '}':
      if (audioData[gct].HIGH_CUTOFF_FREQUENCY>1980.0) {
        if (audioData[gct].HIGH_CUTOFF_FREQUENCY<19500.0) {
          audioData[gct].HIGH_CUTOFF_FREQUENCY+=500.0;
        } else {
          audioData[gct].HIGH_CUTOFF_FREQUENCY=20;
        }
      } else {
          audioData[gct].HIGH_CUTOFF_FREQUENCY+=20.0;
      }
      break;


    case '[':
      if (audioData[gct].LOW_CUTOFF_FREQUENCY>2500.0) {
          audioData[gct].LOW_CUTOFF_FREQUENCY-=500.0;
      } else {
        if (audioData[gct].LOW_CUTOFF_FREQUENCY>20.0)
          audioData[gct].LOW_CUTOFF_FREQUENCY-=20.0;
        else
          audioData[gct].LOW_CUTOFF_FREQUENCY=19500.0;
      }
      break;
    case ']':
      if (audioData[gct].LOW_CUTOFF_FREQUENCY>1980.0) {
        if (audioData[gct].LOW_CUTOFF_FREQUENCY<19500.0) {
          audioData[gct].LOW_CUTOFF_FREQUENCY+=500.0;
        } else {
          audioData[gct].LOW_CUTOFF_FREQUENCY=20.0;
        }
      } else {
        audioData[gct].LOW_CUTOFF_FREQUENCY+=20.0;
      }
      break;



    case '|':
      if (!rst_space)
        audioData[gct].hpf_on=!audioData[gct].hpf_on;
      else
        audioData[gct].high_eq_on=!audioData[gct].high_eq_on;
      break;

    case '\\':
      if (!rst_space)
        audioData[gct].lpf_on=!audioData[gct].lpf_on;
      else
        audioData[gct].low_eq_on=!audioData[gct].low_eq_on;
      break;


    case ':': //high eq gain decrease
      if (audioData[gct].high_gain_db>-60.0) {
        if (audioData[gct].high_gain_db>0.1)
          audioData[gct].high_gain_db-=0.1;
        else
          audioData[gct].high_gain_db-=1;
      } else {
        audioData[gct].high_gain_db=20.0;
      }
      break;

    case '"': //high eq gain increase
      if (audioData[gct].high_gain_db<20.0) {
        if (audioData[gct].high_gain_db>-1)
          audioData[gct].high_gain_db+=0.1;
        else
          audioData[gct].high_gain_db+=1;
      } else {
        audioData[gct].high_gain_db=-60.0;
      }
      break;

    case ';': //low eq gain decrease
      if (audioData[gct].low_gain_db>-60.0) {
        audioData[gct].low_gain_db-=1.0;
      } else {
        audioData[gct].low_gain_db=20.0;
      }
      break;

    case '\'': //low eq gain increase
      if (audioData[gct].low_gain_db<20.0) {
        audioData[gct].low_gain_db+=1.0;
      } else {
        audioData[gct].low_gain_db=-60.0;
      }
      break;




    case '-': //shift starting loop to left
      if (audioData[gct].loop_start>audioData[gct].read_size){
        audioData[gct].loop_start-=audioData[gct].read_size;
        audioData[gct].saved_loop_double_buffer=!audioData[gct].saved_loop_double_buffer;
        if (audioData[gct].loop_read>0) {audioData[gct].loop_read--;} else {audioData[gct].loop_read=READ_BUFFER_NUM-1;}
        if (audioData[gct].loop_play>0) {audioData[gct].loop_play--;} else {audioData[gct].loop_play=READ_BUFFER_NUM-1;} 
      }
      break;
    case '=': //shift starting loop to right
      if (audioData[gct].loop_start<audioData[gct].filesize-audioData[gct].read_size) {
        audioData[gct].loop_start+=audioData[gct].read_size;
        audioData[gct].saved_loop_double_buffer=!audioData[gct].saved_loop_double_buffer;
        if (audioData[gct].loop_read<=18) {audioData[gct].loop_read++;} else {audioData[gct].loop_read=0;}
        if (audioData[gct].loop_play<=18) {audioData[gct].loop_play++;} else {audioData[gct].loop_play=0;}
      }
      break;

    case '_': //shift ending loop to left
      if (audioData[gct].loop_end>audioData[gct].read_size)
        audioData[gct].loop_end-=audioData[gct].read_size;
      break;
    case '+': //shift ending loop to right
      if (audioData[gct].loop_end<audioData[gct].filesize-audioData[gct].read_size)
        audioData[gct].loop_end+=audioData[gct].read_size;
      break;

  }
}
