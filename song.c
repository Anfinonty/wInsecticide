


#define SONG_NUM 10
#define SONG_FOLDER_NUM 23

int current_hour;
int current_min;
int current_sec;

int song_num=0;
int song_mode=0;
int song_rand_num=-1;
int song_audio_duration;
wchar_t song_names[2000][256];
bool is_flac[2000];
bool is_wav[2000];
bool is_mp3[2000];
bool play_new_song=TRUE;
bool stop_playing_song=FALSE;
bool toggle_stop_playing_song=FALSE;
bool playing_wav=FALSE;
bool loading_mp3=FALSE;
bool loading_flac=FALSE;
bool loading_wav=FALSE;
bool skip_song=FALSE;
bool skipping_song=FALSE;
bool clean_up_song=FALSE;
bool clean_up_sound=FALSE;
wchar_t wav_song_playing[256];

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


int CountSongsInDir(const wchar_t *dirname,const wchar_t *indirname, int song_num)
{
  _WDIR *d;
  struct _wdirent *dir; //https://castor-project.org/doc/castor3_0/struct__wdirent.html
  d = _wopendir(dirname);//
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for songs in folder
        wchar_t indir2[256];
        swprintf(indir2,256,L"%s/%s",indirname,dir->d_name);
        song_num=CountSongsInDir(indir,indir2,song_num);
      } else {
        const wchar_t *ext=get_filename_ext(dir->d_name);
        if (_wcsicmp(ext,L"wav")==0 || 
            _wcsicmp(ext,L"mp3")==0 || 
            _wcsicmp(ext,L"flac")==0) {
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

void ResetSongBank() {
  for (int i=0;i<song_num;i++) {
    song_names[i][0]='\0';
    is_flac[i]=is_mp3[i]=is_wav[i]=FALSE;
  }
  song_num=0;
}


void InitSongBank() {
  ResetSongBank();
  song_num=GetSongsInDir(L"music",L"",0);
  swprintf(src_music_dir,64,L"music"); //set dir of music
}
//https://www.codeproject.com/Articles/8396/Using-DirectSound-to-Play-Audio-Stream-Data
//im not using direct sound just learning its concept

#define AUDIO_STREAM_BUFFER_NUM    2  //2mb
#define AUDIO_STREAM_BUFFER_SIZE   524288//8192//524288 8192  minimum //int16_t *524288 = 1mb //0.00049mb
struct AudioStreamBuffer
{
  AWavHeader wav_header[44];
  bool double_buffer;
//  int duration;

  long current_filesize; //spindle
  long filesize;
  long buffer_size;

  int16_t audio[AUDIO_STREAM_BUFFER_NUM][AUDIO_STREAM_BUFFER_SIZE];
} buffer;




void LoadBufferSFX(const wchar_t* filename)
{
  FILE* file = _wfopen(filename, L"rb");
  int wav_header_size=sizeof(AWavHeader); //44
  long mb_1_size=AUDIO_STREAM_BUFFER_SIZE*sizeof(int16_t);
  //printf("@size:%d",mb_1_size);
  buffer.current_filesize=wav_header_size;
  for (int i=0;i<AUDIO_STREAM_BUFFER_NUM;i++) {
    memset(buffer.audio[i], 0, sizeof(buffer.audio[i]));
  }

  if (file) {
    fseek(file, 0, SEEK_END);
    long filesize;
    filesize = ftell(file) - wav_header_size; 

    //Alloc wav buffer header
    fseek(file, 0, SEEK_SET);
    fread(buffer.wav_header,1,wav_header_size,file);

    wfx_wav_music.wFormatTag = WAVE_FORMAT_PCM;
    wfx_wav_music.nChannels = buffer.wav_header->NumOfChan;
    wfx_wav_music.nSamplesPerSec = buffer.wav_header->SamplesPerSec;
    wfx_wav_music.nAvgBytesPerSec = buffer.wav_header->bytesPerSec;
    wfx_wav_music.nBlockAlign = buffer.wav_header->blockAlign;
    wfx_wav_music.wBitsPerSample = buffer.wav_header->bitsPerSample;
    wfx_wav_music.cbSize = 0;

    buffer.buffer_size=mb_1_size;
    buffer.filesize=filesize;
    song_audio_duration=(double)filesize / (buffer.wav_header->SamplesPerSec * buffer.wav_header->NumOfChan * buffer.wav_header->bitsPerSample/8) *1000;
    //buffer.duration=(double)buffer.buffer_size*1000/ (double)(buffer.wav_header->SamplesPerSec * buffer.wav_header->NumOfChan * buffer.wav_header->bitsPerSample/8); //duration per 1mb
    //printf("duration:%d\n",buffer.duration);
   //Alloc actual audio int16_t*
    //fseek(file, buffer.current_filesize, SEEK_SET);
    for (int i=0;i<AUDIO_STREAM_BUFFER_NUM;i++) {
      if (buffer.current_filesize>buffer.filesize) {
        buffer.current_filesize=buffer.filesize;
      }
      fread(buffer.audio[i],1,mb_1_size,file);
      buffer.current_filesize+=mb_1_size;
    }

    fclose(file);

    waveOutOpen(&hWaveOut[2], WAVE_MAPPER, &wfx_wav_music, 0, 0, CALLBACK_NULL);
    waveOutPrepareHeader(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
    waveOutSetVolume(hWaveOut[2], VolumeValue(wav_out_volume*100,1));

    waveOutOpen(&hWaveOut[6], WAVE_MAPPER, &wfx_wav_music, 0, 0, CALLBACK_NULL);
    waveOutPrepareHeader(hWaveOut[6], &whdr[6], sizeof(WAVEHDR));
    waveOutSetVolume(hWaveOut[6], VolumeValue(wav_out_volume*100,1));


    whdr[2].lpData = (LPSTR)buffer.audio[0];
    whdr[2].dwBufferLength = buffer.buffer_size;

    whdr[6].lpData = (LPSTR)buffer.audio[1];
    whdr[6].dwBufferLength = buffer.buffer_size;

    buffer.double_buffer=FALSE;
  }
}


void LoadNextBufferSFX(const wchar_t* filename,int i)
{
  FILE* file = _wfopen(filename, L"rb");
  const int mb_1_size=AUDIO_STREAM_BUFFER_SIZE*sizeof(int16_t);
  if (file) {
    //Alloc actual audio int16_t*
    fseek(file, buffer.current_filesize, SEEK_SET);
    if (buffer.current_filesize>=buffer.filesize) {
      buffer.current_filesize=buffer.filesize;
      fclose(file);
      return;
    }
    fread(buffer.audio[i],1,mb_1_size,file);
    buffer.current_filesize+=mb_1_size;
    fclose(file);
  }
}





void PlayThreadStream()
{
  if (!stop_playing_song) {
  if (playing_wav && !play_new_song && !loading_flac && !loading_mp3 && !loading_wav){
    if (!buffer.double_buffer) {
      //printf("@1\n");
        // Write the audio data
        waveOutWrite(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));    
        while (TRUE) {
          if (whdr[2].dwFlags & WHDR_DONE) {
            buffer.double_buffer=TRUE;
            LoadNextBufferSFX(wav_song_playing,0);
            whdr[2].lpData = (LPSTR)buffer.audio[0];
            whdr[2].dwBufferLength = buffer.buffer_size;
            break;
          } else {
            Sleep(10);
          }
        }
    } else {
      //printf("@2\n");
        // Write the audio data
        waveOutWrite(hWaveOut[6], &whdr[6], sizeof(WAVEHDR));    
        while (TRUE) {
          if (whdr[6].dwFlags & WHDR_DONE) {
            buffer.double_buffer=FALSE;
            LoadNextBufferSFX(wav_song_playing,1);
            whdr[6].lpData = (LPSTR)buffer.audio[1];
            whdr[6].dwBufferLength = buffer.buffer_size;
            break;
          } else {
            Sleep(10);
          }
        }
      } 
    } else {
      Sleep(1000);
    }
  } else {
    Sleep(1000);
  }
}



DWORD WINAPI PlayMemSnd3(LPVOID lpParam)
{
  while (TRUE) {
    PlayThreadStream();
  }
}




DWORD WINAPI SoundTask(LPVOID lpArg) {
  srand(time(NULL));
  while (TRUE) {
    //SONG Tasks
    if (song_num>0) {
      if (!stop_playing_song) {

        if (playing_wav) { //hi im currently playing a flac/wav
          
          current_song_time=current_timestamp();
          if (current_song_time>time_song_end || play_new_song) { //stop playing flac when duration is over //second delay
            play_new_song=TRUE;
            playing_wav=FALSE;
            loading_mp3=FALSE;
            loading_flac=FALSE;
            loading_wav=FALSE;
            time_song_start=0;
            time_song_end=-1;
            current_song_time=-1;
          }
        }


        if (!playing_wav) {//im not playing a wav music
          if ((loading_flac || loading_mp3 || loading_wav)) {//loading flac or wav, flac
            call_help_timer=0;
            current_song_time=-1;
            if (loading_wav) {
              swprintf(wav_song_playing,256,L"%s/%s",src_music_dir,song_names[song_rand_num]);
            } else {
              swprintf(wav_song_playing,256,L"music/tmp/tmp.wav");
            }

            LoadBufferSFX(wav_song_playing);
            time_song_start=current_timestamp();
            time_song_end=time_song_start+song_audio_duration;//songAudio->duration;
            loading_flac=FALSE;
            loading_mp3=FALSE;
            loading_wav=FALSE;
            play_new_song=FALSE;
            playing_wav=TRUE;


          } else if (!loading_flac && !loading_mp3 && !loading_wav) { //not loading song 
            if (play_new_song) //song status: stopped
            {
              //mem_snd_interrupt[2]=TRUE;
              //mem_snd_interrupt[6]=TRUE;
              waveOutReset(hWaveOut[2]);
              waveOutReset(hWaveOut[6]);
              waveOutClose(hWaveOut[2]);
              waveOutUnprepareHeader(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
              waveOutClose(hWaveOut[6]);
              waveOutUnprepareHeader(hWaveOut[6], &whdr[6], sizeof(WAVEHDR));

            //play new music
              call_help_timer=0;
              remove("music/tmp/tmp.wav");
              rmdir("music/tmp"); //remove tmp, manually because C is like that

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
          
              //stop .wav player
              current_song_time=-1;
              time_song_end=-1;

              if (is_flac[song_rand_num]) { //loaded song is a flac
                wchar_t my_command[512];
                loading_flac=TRUE;
                system("mkdir \"music/tmp\""); //make new tmp
                swprintf(my_command,512,L"flac.exe --totally-silent -d -f \"%s/%s\" -o music/tmp/tmp.wav",src_music_dir,song_names[song_rand_num]);
                _wsystem(my_command);
              } else if (is_mp3[song_rand_num]) {
                wchar_t my_command[512];
                loading_mp3=TRUE;
                system("mkdir \"music/tmp\""); //make new tmp
                //http://mpg123.de/download/win32/mpg123-1.10.1-static-x86.zip //currently used to decode mp3
                //swprintf(my_command,512,L"madplay.exe -b 16 -Q -R 44100  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]); //not compatible with unicode/utf16
                //swprintf(my_command,512,L"lame.exe --decode  \"music/%s\" -o music/tmp/tmp.wav",song_names[song_rand_num]); //unable to decode to a specific desired sample rate

                swprintf(my_command,512,L"mpg123.exe -q -w \"music/tmp/tmp.wav\"  \"%s/%s\"",src_music_dir,song_names[song_rand_num]);
                _wsystem(my_command);
              } else if (is_wav[song_rand_num]) {
                loading_wav=TRUE;
              }
              play_new_song=FALSE;
            }
          } //else { //song status: playing
        }  
      }
    }
    //End of song Task


    if (toggle_stop_playing_song) {
      waveOutReset(hWaveOut[2]);
      waveOutReset(hWaveOut[6]);
      waveOutClose(hWaveOut[2]);
      waveOutUnprepareHeader(hWaveOut[2], &whdr[2], sizeof(WAVEHDR));
      waveOutClose(hWaveOut[6]);
      waveOutUnprepareHeader(hWaveOut[6], &whdr[6], sizeof(WAVEHDR));

    //play new music
      call_help_timer=0;
      remove("music/tmp/tmp.wav");
      rmdir("music/tmp"); //remove tmp, manually because C is like that

      playing_wav=FALSE;
      loading_wav=
      loading_flac=
      loading_mp3=FALSE;
      play_new_song=FALSE;

      stop_playing_song=TRUE;
      toggle_stop_playing_song=FALSE;
    }

    if (flag_adjust_wav_out_audio) {
      long int vol=VolumeValue(wav_out_volume*100,1);
      waveOutSetVolume(hWaveOut[2],vol);
      waveOutSetVolume(hWaveOut[6],vol);
      flag_adjust_wav_out_audio=FALSE;
    }


    //SOUND Tasks
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

      if (call_help_timer<2000/*5000*/) {
        call_help_timer+=6;
      }
      Sleep(6); //fast loop
    } else { //In Main Menu
      //persian time update if day change
      //get_current_time(&current_hour,&current_min,&current_sec);

        if (clean_up_sound) {
          for (int i=0;i<MAX_ENEMY_NUM;i++) {
            PlaySound(NULL, NULL, SND_ASYNC);
          }

          waveOutReset(hWaveOut[0]);
          mem_snd_interrupt[0]=TRUE;

          waveOutReset(hWaveOut[1]);
          mem_snd_interrupt[1]=TRUE;

          waveOutReset(hWaveOut[3]);
          mem_snd_interrupt[3]=TRUE;

          waveOutReset(hWaveOut[4]);
          mem_snd_interrupt[4]=TRUE;

          waveOutReset(hWaveOut[5]);
          mem_snd_interrupt[5]=TRUE;

          if (lvl_has_song) {
            //play new music
              call_help_timer=0;
              remove("music/tmp/tmp.wav");
              rmdir("music/tmp"); //remove tmp, manually because C is like that
              lvl_has_song=FALSE;
              InitSongBank();
              play_new_song=TRUE;
          }


          for (int i=0;i<SPAM_SFX_NUM;i++) {
            freeSoundEffectCache(&spamSoundEffectCache[i]);
          }
          for (int i=0;i<CHANNEL_SFX_NUM;i++) {
            freeSoundEffectCache(&channelSoundEffectCache[i]);
          }
          clean_up_sound=FALSE;
        }

      if (current_hour==0 && current_min==0 && current_sec<=1) {//next day
        int64_t timenow=int64_current_timestamp();
        PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day);
        PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift);
      }
      Sleep(1000); //eepy loop
    }
  }
}



