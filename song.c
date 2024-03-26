
int song_num=0;
int song_mode=0;
int song_rand_num=-1;
char song_names[1000][256];
bool play_new_song=FALSE;
bool stop_playing_song=FALSE;
bool toggle_stop_playing_song=FALSE;


//https://stackoverflow.com/questions/5309471/getting-file-extension-in-c
const char *get_filename_ext(const char *filename) 
{
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot + 1;
}


//https://stackoverflow.com/questions/4204666/how-to-list-files-in-a-directory-in-a-c-program
int GetSongsInDir(const char *dirname)
{
  int song_num=0;
  DIR *d;
  struct dirent *dir;
  d = opendir(dirname);
  if (d) {
    while ((dir=readdir(d))!=NULL) {
      const char *ext=get_filename_ext(dir->d_name);
      if (strcmp(ext,"wav")==0 || strcmp(ext,"mp3")==0 || strcmp(ext,"wma")==0) {
        //printf("%d|-> %s\n",song_num,dir->d_name);
        //song_names[song_num]=dir->d_name;
        strncpy(song_names[song_num],dir->d_name,256);
        //printf("%d|-> %s\n",song_num,song_names[song_num]);
        song_num++;
        if (song_num>=1000) {
          break;
        }
      }
    }
    closedir(d);
  }
  return song_num;
}


void InitSongBank() {
  song_num=GetSongsInDir("music");
}



//https://stackoverflow.com/questions/8991192/check-the-file-size-without-opening-file-in-c
/*int64_t FileSize(char* name)
{
    HANDLE hFile = CreateFileA(name, GENERIC_READ, 
        FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 
        FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile==INVALID_HANDLE_VALUE)
        return -1; // error condition, could call GetLastError to find out more

    LARGE_INTEGER size;
    if (!GetFileSizeEx(hFile, &size))
    {
        CloseHandle(hFile);
        return -1; // error condition, could call GetLastError to find out more
    }

    CloseHandle(hFile);
    return size.QuadPart;
}*/


DWORD WINAPI SongTask(LPVOID lpArg) {
//void SongAct() {
  srand(time(NULL));
  while (true) {
    if (song_num>0) {
      if (!stop_playing_song) {
        char my_status[16];
        //char my_length[16];
        mciSendStringA("status music mode",my_status,16,NULL); //periodically check status
        //printf("\nstatus: %s\n",my_status);
        if (strcmp(my_status,"stopped")==0 || strcmp(my_status,"")==0 || play_new_song) //song status: stopped
        {
      
      //play new music
          mciSendStringA("close music",NULL,0,NULL);

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


          char songname[128];
          sprintf(songname,"open \"music/%s\" alias music",song_names[song_rand_num]);
          mciSendStringA(songname,NULL,0,NULL);
          mciSendStringA("play music",NULL,0,NULL);
          play_new_song=FALSE;
        }
      } else {
        if (toggle_stop_playing_song) {
          mciSendStringA("close music",NULL,0,NULL);
          toggle_stop_playing_song=FALSE;
        }
      }
    }
    Sleep(1000); //eepy loop
  }
}






/*
https://copyprogramming.com/howto/playsound-in-c-console-application
DWORD WINAPI SoundTask(LPVOID lpArg) {
  while (true) {
    if (play_a_sound==1) {
      mciSendStringA("play music/00/5.wav",NULL,0,NULL); 
    } else if (play_a_sound==2) {
      mciSendStringA("play music/00/4.wav",NULL,0,NULL); 
    }
    Sleep(1000); //eepy loop
  }
}*/

