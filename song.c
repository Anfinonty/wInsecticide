
int current_hour;
int current_min;
int current_sec;

int song_num=0;
int song_mode=0;
int song_rand_num=-1;
wchar_t song_names[2000][256];
bool is_flac[2000];
bool play_new_song=FALSE;
bool stop_playing_song=FALSE;
bool toggle_stop_playing_song=FALSE;
bool loading_flac=FALSE;
bool skip_song=FALSE;

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
            _wcsicmp(ext,L"wma")==0 || 
            _wcsicmp(ext,L"midi")==0 || 
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
    if (song_num>0) {
      if (!stop_playing_song) {
        wchar_t my_status[16];
        //char my_length[16];
        mciSendString(L"status music mode",my_status,16,NULL); //periodically check status
        //printf("\nstatus: %s\n",my_status);
        if ((_wcsicmp(my_status,L"stopped")==0 || _wcsicmp(my_status,L"")==0 || play_new_song)) //song status: stopped
        {
      
          //play new music
          if (!loading_flac) {
            mciSendString(L"close music",NULL,0,NULL);
            remove("music/tmp/tmp.wav");
            rmdir("music/tmp"); //remove tmp, manually because C is like that

             if (!skip_song) {
               switch (song_mode) {
               case 0: //Play Songs acending
                  song_rand_num=LimitValue(song_rand_num+1,0,song_num);
                  break;
                case 1: //Play songs decending
                 song_rand_num=LimitValue(song_rand_num-1,0,song_num);
                 break;
                case 2: //play songs shuffle
                  song_rand_num=RandNum(0,song_num-1,1);
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
            }
          }

          wchar_t songname[512];
          if (!is_flac[song_rand_num]) {
            swprintf(songname,512,L"open \"music/%s\" alias music",song_names[song_rand_num]);
          } else {
            swprintf(songname,512,L"open \"music/tmp/tmp.wav\" alias music"); //play flac
          }
          mciSendString(songname,NULL,0,NULL);
          mciSendString(L"play music",NULL,0,NULL);
          play_new_song=FALSE;
        } else {
          if (loading_flac) { //flac has finished loading
            if (_wcsicmp(my_status,L"playing")==0) {
              loading_flac=FALSE;
            }
          }
        }
      } else { //song is playing
        if (toggle_stop_playing_song) {
          mciSendString(L"close music",NULL,0,NULL);
          remove("music/tmp/tmp.wav");
          rmdir("music/tmp"); //remove tmp
          loading_flac=FALSE;

          toggle_stop_playing_song=FALSE;
        }
      }
    }
    //End of song Task

    //Persian time update if new day
    get_current_time(&current_hour,&current_min,&current_sec);
    if (current_hour==0 && current_min==0 && current_sec<=1) {//next day
      int64_t timenow=int64_current_timestamp();
      PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day);
      PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift);
    }


    //Play Game Souond
    for (int i=0;i<player.bullet_shot_num;i++) {
      BulletSndAct(player.bullet[i]);
    }
    for (int i=0;i<player.rendered_enemy_num;i++) {
      EnemySndAct(player_render_enemies[i]);
    }
    PlayerSndAct();



    if (!in_main_menu) {
      Sleep(6); //fast loop
    } else {
      Sleep(1000); //eepy loop
    }
  }
}



