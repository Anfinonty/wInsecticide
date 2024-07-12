


#define SONG_NUM 10
#define SONG_FOLDER_NUM 23

int current_hour;
int current_min;
int current_sec;

int song_num=0;
int song_mode=0;
int song_rand_num=-1;
wchar_t song_names[2000][256];
bool is_flac[2000];
bool is_wav[2000];
bool is_mp3[2000];
bool play_new_song=TRUE;
bool stop_playing_song=FALSE;
bool toggle_stop_playing_song=FALSE;
bool loading_mp3=FALSE;
bool loading_flac=FALSE;
bool loading_wav=FALSE;
bool playing_wav=FALSE;
bool skip_song=FALSE;

unsigned long long time_song_start;
unsigned long long time_song_end;
unsigned long long current_song_time;


//https://github.com/audiojs/sample-rate

//https://riptutorial.com/winapi/example/5736/create-a-file-and-write-to-it
//https://stackoverflow.com/questions/51864328/unable-to-create-dlls-getting-dll-is-not-a-valid-win32-application
//https://learn.microsoft.com/en-us/windows/win32/fileio/creating-and-opening-files
//https://learn.microsoft.com/en-us/windows/win32/coreaudio/capturing-a-stream
//https://stackoverflow.com/questions/3774343/what-libraries-how-to-play-wav-file-on-windows32-in-c
//https://stackoverflow.com/questions/1565439/how-to-playsound-in-c-using-windows-api
//https://stackoverflow.com/questions/59483071/trying-to-create-a-wav-file-with-capturing-a-stream-from-win32-wasapi-c

//https://stackoverflow.com/questions/2302841/win32-playsound-how-to-control-the-volume
//https://stackoverflow.com/questions/16905458/reduce-the-volume-of-a-wav-audio-file-using-c// Adjusts the volume of the WAV samples by a percentage
//https://web.archive.org/web/20090827003349/http://ccrma.stanford.edu/courses/422/projects/WaveFormat/
//https://blog.devgenius.io/how-to-increase-volume-of-an-audio-file-with-c-6d23e04a03a6
//http://soundfile.sapp.org/doc/WaveFormat/


/*char* ReadWavFileIntoMemory(const char* filename, DWORD *fsize)
{
  char* sounddata;
  FILE* file = fopen(filename, "rb");
  if (file) {
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    sounddata = (char*)malloc(file_size);
    *fsize = file_size;
    fread(sounddata, 1, file_size, file);
    
    fclose(file);
    return sounddata;
  } else {
    sounddata=NULL;
    *fsize=0;
  }
  return "";
}*/



//https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const wchar_t *get_filename_ext(const wchar_t *filename) 
{
  const wchar_t *dot = wcsrchr(filename, '.');
  if(!dot || dot == filename) return L"";
  return dot + 1;
}

//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
int GetSongsInDir(const wchar_t *dirname,const wchar_t *indirname, int song_num)
{
  _WDIR *d;
  struct _wdirent *dir; //https://castor-project.org/doc/castor3_0/struct__wdirent.html
  d = _wopendir(dirname);//
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
      //printf("status: %d\n",PathIsDirectoryA(indir)); //https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathisdirectorya
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for songs in folder
        wchar_t indir2[256];
        swprintf(indir2,256,L"%s/%s",indirname,dir->d_name);
        song_num=GetSongsInDir(indir,indir2,song_num);
      } else {
        const wchar_t *ext=get_filename_ext(dir->d_name);
        /*wchar_t lowext[6];
        for (int i=0;i<6;i++) {
          lowext[i]=tolower(ext[i]);
        }*/
      //printf("\n--%s ;; %s",ext,lowext);
        if (_wcsicmp(ext,L"wav")==0 || 
            _wcsicmp(ext,L"mp3")==0 || 
            //_wcsicmp(ext,L"wma")==0 || 
            //_wcsicmp(ext,L"midi")==0 || 
            _wcsicmp(ext,L"flac")==0/* ||
            strcmp(lowext,"mpg")==0 ||
            strcmp(lowext,"mpeg")==0*/) {
        //printf("%d|-> %s\n",song_num,dir->d_name);
        //song_names[song_num]=dir->d_name;
          if (_wcsicmp(ext,L"flac")==0) {
            is_flac[song_num]=TRUE;
          } else {
            is_flac[song_num]=FALSE;
          }


          if (_wcsicmp(ext,L"wav")==0) {
            is_wav[song_num]=TRUE;
          } else {
            is_wav[song_num]=FALSE;
          }


          if (_wcsicmp(ext,L"mp3")==0) {
            is_mp3[song_num]=TRUE;
          } else {
            is_mp3[song_num]=FALSE;
          }


          wchar_t indir[256];
          swprintf(indir,256,L"%s/%s",indirname,dir->d_name);
          wcsncpy(song_names[song_num],indir,256);
          //printf("%d|-> %s\n",song_num,song_names[song_num]);
          song_num++;
          if (song_num>=2000) {
            break;
          }
        }      
      }
    }
    _wclosedir(d);
  }
  return song_num;
}


void InitSongBank() {
  song_num=GetSongsInDir(L"music",L"",0);
}






DWORD WINAPI SongTask(LPVOID lpArg) {
//void SongAct() {
  srand(time(NULL));
  while (TRUE) {
    //All Song Tasks
    if (flag_adjust_wav_out_audio) {
      long int vol=VolumeValue(wav_out_volume*100,1);
      waveOutSetVolume(hWaveOut[2],vol);
      flag_adjust_wav_out_audio=FALSE;
    }
    if (song_num>0) {
      if (!stop_playing_song) {

        if (playing_wav) { //hi im currently playing a flac/wav
          
          current_song_time=current_timestamp();
          if (current_song_time>time_song_end) { //stop playing flac when duration is over
            play_new_song=TRUE;
            loading_mp3=FALSE;
            loading_flac=FALSE;
            loading_wav=FALSE;
            playing_wav=FALSE;
            time_song_start=0;
            time_song_end=0;
            //if (song_audio!=NULL)
              //free(song_audio);
            freeSoundEffect(&songAudio);
          }

          if (flag_adjust_song_audio) {
            flag_adjust_song_audio=FALSE;
          }



        } else {//im not playing a wav music
          //char my_length[16];
          //printf("\nstatus: %ls\n",my_status);
          if (loading_flac) {//loading flac or wav, flac
            playing_wav=TRUE;
            loading_flac=FALSE;
            loading_mp3=FALSE;
            loading_wav=FALSE;
            play_new_song=FALSE;

            loadSoundEffect(&songSFX,L"music/tmp/tmp.wav",wfx_wav_music,FALSE);
            time_song_start=current_timestamp();
            time_song_end=time_song_start+songAudio.duration;
            PlayMemSnd(&songSFX,FALSE,2);

            remove("music/tmp/tmp.wav");
            rmdir("music/tmp"); //remove tmp, manually because C is like that
          } else if (loading_mp3) {
            playing_wav=TRUE;
            loading_flac=FALSE;
            loading_mp3=FALSE;
            loading_wav=FALSE;
            play_new_song=FALSE;

            loadSoundEffect(&songSFX,L"music/tmp/tmp.wav",wfx_wav_music,FALSE);
            time_song_start=current_timestamp();
            time_song_end=time_song_start+songAudio.duration;
            PlayMemSnd(&songSFX,FALSE,2);

            remove("music/tmp/tmp.wav");
            rmdir("music/tmp"); //remove tmp, manually because C is like that

          }else if (loading_wav) {//loading flac or wav, wav
            playing_wav=TRUE;
            loading_flac=FALSE;
            loading_mp3=FALSE;
            loading_wav=FALSE;
            play_new_song=FALSE;

            wchar_t wav_song_playing[256];
            swprintf(wav_song_playing,256,L"music/%s",song_names[song_rand_num]);
            //song_audio=LoadMusicWavW(wav_song_playing, &song_audio_filesize, &song_duration);
            loadSoundEffect(&songSFX,wav_song_playing,wfx_wav_music,FALSE);
            time_song_start=current_timestamp();
            time_song_end=time_song_start+songAudio.duration;
            PlayMemSnd(&songSFX,FALSE,2);

            //attempt to remove left overs
            remove("music/tmp/tmp.wav");
            rmdir("music/tmp"); //remove tmp, manually because C is like that
          }        

          if (play_new_song) //song status: stopped
          {
            //play new music
            if (!loading_wav && !loading_flac && !loading_mp3 && !playing_wav) {            
              call_help_timer=0;
              remove("music/tmp/tmp.wav");
              rmdir("music/tmp"); //remove tmp, manually because C is like that

              //stop .wav player
              mem_snd_interrupt[2]=TRUE;
              waveOutReset(hWaveOut[2]);
              CloseHandle(hMemSndArray[2]);

              freeSoundEffect(&songAudio);

              if (!skip_song) {
                switch (song_mode) {
                  case 0: //play songs shuffle
                    song_rand_num=RandNum(0,song_num-1,1);
                    break;
                  case 1: //Play Songs acending
                    song_rand_num=LimitValue(song_rand_num+1,0,song_num);
                    break;
                  case 2: //Play songs decending
                    song_rand_num=LimitValue(song_rand_num-1,0,song_num);
                    break;
                }
              } else {
                skip_song=FALSE;
              }
          
              
              if (is_flac[song_rand_num]) { //loaded song is a flac
                wchar_t my_command[512];
                loading_flac=TRUE;
                system("mkdir \"music/tmp\""); //make new tmp
                swprintf(my_command,512,L"flac.exe --totally-silent -d -f \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]);
                _wsystem(my_command);
              } else if (is_mp3[song_rand_num]) {
                wchar_t my_command[512];
                loading_mp3=TRUE;
                system("mkdir \"music/tmp\""); //make new tmp
                //http://mpg123.de/download/win32/mpg123-1.10.1-static-x86.zip //currently used to decode mp3
                //swprintf(my_command,512,L"madplay.exe -b 16 -Q -R 44100  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]); //not compatible with unicode/utf16
                //swprintf(my_command,512,L"lame.exe --decode  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]); //unable to decode to a specific desired sample rate
                swprintf(my_command,512,L"mpg123.exe -q -r 44100 -w \"music/tmp/tmp.wav\"  \"music/%s\"",song_names[song_rand_num]);
                _wsystem(my_command);
              } else if (is_wav[song_rand_num]) {
                loading_wav=TRUE;
              }
            }
            play_new_song=FALSE;
          } //else { //song status: playing

          //}
        }  
      }
    }
    //End of song Task


    if (toggle_stop_playing_song) {
        //stop mp3 player
      call_help_timer=0;
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp

        //stop .wav player
      mem_snd_interrupt[2]=TRUE;
      waveOutReset(hWaveOut[2]);
      CloseHandle(hMemSndArray[2]);

      playing_wav=FALSE;
      loading_flac=FALSE;
      loading_mp3=FALSE;
      loading_wav=FALSE;
      freeSoundEffect(&songAudio);


      toggle_stop_playing_song=FALSE;
    }

    if (!in_main_menu) { //NOT in main menu, gaming
      //Play Game Souond
      if (game_audio && level_loaded) {
        if (!player.time_breaker) { //player sounds made by player bullets
          for (int i=0;i<player.bullet_shot_num;i++) {
            BulletSndAct(player.bullet[i]);
          }
        }
        if (player.bullet_shot!=-1) { //player sounds made by sniper player bullets
          BulletSndAct(player.bullet_shot);
        }
        if (level_loaded) {
          if (!flag_restart_audio) {
            for (int i=0;i<ENEMY_NUM;i++) {
              EnemySndAct(i);
            }
            PlayerSndAct();       
          } else {
            for (int i=0;i<ENEMY_NUM;i++) {
              PlaySound(NULL, NULL, SND_ASYNC);
            }
            flag_restart_audio=FALSE;
          }
        }
      }


      if (player.speed>10) {
        player.fast_duration+=6;
      } else {
        player.fast_duration=0;
      }

      if (call_help_timer<5000) {
        call_help_timer+=6;
      }
      Sleep(6); //fast loop
    } else { //In Main Menu
      //persian time update if day change
      get_current_time(&current_hour,&current_min,&current_sec);
      if (current_hour==0 && current_min==0 && current_sec<=1) {//next day
        int64_t timenow=int64_current_timestamp();
        PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day);
        PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift);
      }

      if (clean_up_sound) {
        for (int i=0;i<MAX_ENEMY_NUM;i++) {
          PlaySound(NULL, NULL, SND_ASYNC);
        }
        for (int i=0;i<SND_THREAD_NUM-1;i++) {
          waveOutReset(hWaveOut[i]);
          mem_snd_interrupt[i]=TRUE;
        }
        clean_up_sound=FALSE;
      }
      Sleep(1000); //eepy loop
    }
  }
}



