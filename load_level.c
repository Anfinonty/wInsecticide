


void CleanupGameAudio()
{
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

  for (int i=0;i<SPAM_SFX_NUM;i++) {
    freeSoundEffectCache(&spamSoundEffectCache[i]);
  }
  for (int i=0;i<CHANNEL_SFX_NUM;i++) {
    freeSoundEffectCache(&channelSoundEffectCache[i]);
  }
}


void AdjustGameAudio()
{
  adjustSFXVolume(&spamSoundEffectCache[0],&spamSoundEffect[0],game_volume/4,FALSE); //start
  adjustSFXVolume(&spamSoundEffectCache[1],&spamSoundEffect[1],game_volume/4,FALSE); //stop
  adjustSFXVolume(&spamSoundEffectCache[2],&spamSoundEffect[2],game_volume/4,FALSE); //enemy hurt

  adjustSFXVolume(&spamSoundEffectCache[3],&spamSoundEffect[3],game_volume/4,FALSE);//block
  adjustSFXVolume(&spamSoundEffectCache[4],&spamSoundEffect[4],game_volume/4,FALSE);//block perfect
  adjustSFXVolume(&spamSoundEffectCache[5],&spamSoundEffect[5],game_volume/4,FALSE);//hurt
  adjustSFXVolume(&spamSoundEffectCache[6],&spamSoundEffect[6],game_volume/30,FALSE);//enemy block
  adjustSFXVolume(&spamSoundEffectCache[7],&spamSoundEffect[7],game_volume,FALSE); //clang
  adjustSFXVolume(&spamSoundEffectCache[8],&spamSoundEffect[8],game_volume/3,FALSE); //powerup

  adjustSFXVolume(&channelSoundEffectCache[0],&channelSoundEffect[0],game_volume,TRUE); //player death
  adjustSFXVolume(&channelSoundEffectCache[1],&channelSoundEffect[1],game_volume/2+game_volume/10,TRUE); //death //clang_death
  adjustSFXVolume(&channelSoundEffectCache[2],&channelSoundEffect[2],game_volume,TRUE); //knife throw
  adjustSFXVolume(&channelSoundEffectCache[3],&channelSoundEffect[3],game_volume/10,TRUE); //rain
  adjustSFXVolume(&channelSoundEffectCache[4],&channelSoundEffect[4],game_volume/30,TRUE); //rain softer <-- 2 layers
  adjustSFXVolume(&channelSoundEffectCache[5],&channelSoundEffect[5],game_volume/3,TRUE); //shotgun
  adjustSFXVolume(&channelSoundEffectCache[6],&channelSoundEffect[6],game_volume/2,TRUE); //sniper
  adjustSFXVolume(&channelSoundEffectCache[7],&channelSoundEffect[7],game_volume,TRUE); //reload shotgun
  adjustSFXVolume(&channelSoundEffectCache[8],&channelSoundEffect[8],game_volume,TRUE); //gun shoot
  adjustSFXVolume(&channelSoundEffectCache[9],&channelSoundEffect[9],game_volume,TRUE); //load knife
  adjustSFXVolume(&channelSoundEffectCache[10],&channelSoundEffect[10],game_volume,TRUE); //load 3 knife
  adjustSFXVolume(&channelSoundEffectCache[11],&channelSoundEffect[11],game_volume,TRUE); //load gun
  adjustSFXVolume(&channelSoundEffectCache[12],&channelSoundEffect[12],game_volume,TRUE); //gun empty
}



void InitOnce() {
  GR_WIDTH=SCREEN_WIDTH;
  GR_HEIGHT=SCREEN_HEIGHT;

  player.cam_move_x=0;
  player.cam_move_y=0;

  player_load_color=player_color;
  player_load_iris_color=player_iris_color;
  player_load_pupil_color=player_pupil_color;
  player_bullet_color=WHITE;
}


void InitLoadLvlSong()
{
  //Load custom song
  wchar_t dirname[64];
  swprintf(dirname,64,L"saves/%s/music",level_names[level_chosen]);
  int tmp_song_num=CountSongsInDir(dirname,L"",0);

  if (tmp_song_num>0) { //only reload songs are present
    _WDIR *d;
    struct _wdirent *dir;
    d = _wopendir(dirname);
    if (d) {
      lvl_has_song=TRUE;
      if (tmp_song_num>0) {
        ResetSongBank();
        song_num=GetSongsInDir(dirname,L"",0);
        swprintf(src_music_dir,64,L"saves/%s/music",level_names[level_chosen]);
        play_new_song[0]=TRUE;
      }
      _wclosedir(d);
    }
  } //else {
    //InitSongBank();
    //play_new_song=TRUE;
  //}
}


void Init() {

  //Load Best Score
  //Folder & file creation
  FILE *fptr;
  if (_waccess(save_level, F_OK)==0) { //if file exists
  //do nothing
  } else if (ENOENT==errno) { //if file doesnt exist
    fptr = _wfopen(save_level,L"a");
    fprintf(fptr,"2147483646\n");
    fclose(fptr);
  }


  int_best_score=0;
  FILE *fr; //Begin setting best score
  int c;
  int current_int;
  fr = _wfopen(save_level,L"r"); //check if scoresave data of levelname
  while ((c=fgetc(fr))!=EOF) {
    if (c>='0' && c<='9') {
      current_int=c-'0';
      int_best_score*=10; //shift digit by left,  //Read from file while !=EOF score*=10
      int_best_score+=current_int; //add to right digit //after reading, convert into (double)/1000
    }
  }
  fclose(fr);
  double_best_score=(double)int_best_score/1000;


  //Start level
  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;
  game_timer=0;
  enemy_kills=0;
  game_over=FALSE;
  frame_tick=-10;

  //fire_rng_i=0;

  //Initialize Level
  InitBullet(BULLET_NUM);
  InitGrid();
  InitNodeGrid();
  InitGround(TRUE);
  InitGroundFire();
  InitNodeGridAttributes();
  InitEnemy();
  InitPlayer();

  InitRDGrid();

  InitScreenRainDrop();
  InitBulletRain();
  //BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
  //BitmapPalette(hdc,map_water_platforms_sprite,rgbColorsDefault);

  mem_snd_interrupt[0]=TRUE;
  waveOutReset(hWaveOut[0]);

  mem_snd_interrupt[1]=TRUE;
  waveOutReset(hWaveOut[1]);

  mem_snd_interrupt[3]=TRUE;
  waveOutReset(hWaveOut[3]);

  mem_snd_interrupt[4]=TRUE;
  waveOutReset(hWaveOut[4]);

  mem_snd_interrupt[5]=TRUE;
  waveOutReset(hWaveOut[5]);

  if (!run_once_only) {
    run_once_only=TRUE;
  }

  if (!run_after_once) {
    run_after_once=TRUE;
  } else {
    PlaySound(NULL, NULL, SND_ASYNC);
    InitEnemyPathfindingNodes();
  }
  time_begin=current_timestamp();
}




/*void InitPlatformsSprite(HWND hwnd, HDC hdc)
{
  wchar_t bmp_save[64];
  swprintf(bmp_save,64,L"saves/%s/map.bmp",level_names[level_chosen]);
  wchar_t bmp_water_save[64];
//  swprintf(bmp_water_save,64,L"saves/%s/map_water.bmp",level_names[level_chosen]);
  swprintf(bmp_water_save,64,L"saves/%s/map_foreground.bmp",level_names[level_chosen]);
  wchar_t bmp_save_shadow[64];
  swprintf(bmp_save_shadow,64,L"saves/%s/map_shadow.bmp",level_names[level_chosen]);
  //SaveBitmapToFile2(map_platforms_sprite,rgbColorsDefault,bmp_save);

  /*map_platforms_sprite=(HBITMAP)LoadImageW(
        NULL,               // Handle to the instance (NULL for file)
        bmp_save,           // File name of the bitmap
        IMAGE_BITMAP,       // Type of image
        0, 0,               // Desired width and height (0 to use actual size)
        LR_LOADFROMFILE   // Load from file
        | LR_CREATEDIBSECTION // Create a DIB section
    );*/

  /*map_water_platforms_sprite=(HBITMAP)LoadImageW(
        NULL,               // Handle to the instance (NULL for file)
        bmp_water_save,           // File name of the bitmap
        IMAGE_BITMAP,       // Type of image
        0, 0,               // Desired width and height (0 to use actual size)
        LR_LOADFROMFILE   // Load from file
        | LR_CREATEDIBSECTION // Create a DIB section
    );*/


  /*if (is_shadows && game_shadow) {
  map_platforms_shadow_shader = (HBITMAP)LoadImageW(
        NULL,               // Handle to the instance (NULL for file)
        bmp_save_shadow,           // File name of the bitmap
        IMAGE_BITMAP,       // Type of image
        0, 0,               // Desired width and height (0 to use actual size)
        LR_LOADFROMFILE   // Load from file
        | LR_CREATEDIBSECTION // Create a DIB section
  );
  }*/

  /*if (map_platforms_sprite==NULL) {
    PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
    hdc=BeginPaint(hwnd, &ps);
    HDC hdc2=CreateCompatibleDC(hdc);


    map_platforms_sprite=CreateCrunchyBitmap(MAP_WIDTH,MAP_HEIGHT);
  //map_platforms_sprite=CreateLargeBitmap(MAP_WIDTH,MAP_HEIGHT);

    SelectObject(hdc2,map_platforms_sprite);

    GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,MYCOLOR1); //Create Background with random color over platforms

    DrawGroundTriFill(hdc2);
    DrawGround(hdc2);
    DrawGroundText(hdc2);

    //if (map_platforms_shadow_shader==NULL) {
    //CreatePlatformShadowBitmap(hdc2);//,map_platforms_sprite);
    //SaveBitmapToFile2(map_platforms_shadow_shader,rgbColorsDefault, bmp_save_shadow);
    //}  

    DeleteDC(hdc2);
    EndPaint(hwnd, &ps);
    //SaveBitmapToFile2(map_platforms_sprite,rgbColorsDefault, bmp_save);
    //BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
  }*/

  /*if (map_water_platforms_sprite==NULL && has_water) {
    PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
    hdc=BeginPaint(hwnd, &ps);
    HDC hdc2=CreateCompatibleDC(hdc);

    map_water_platforms_sprite=CreateCrunchyBitmap(MAP_WIDTH,MAP_HEIGHT);
    SelectObject(hdc2,map_water_platforms_sprite);
    GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,MYCOLOR1); //Create Background with random color over platforms
    DrawWaterTriFill(hdc2);

    DeleteDC(hdc2);
    EndPaint(hwnd, &ps);
  }*/


  //map_platforms_sprite_mask=CreateBitmapMask(map_platforms_sprite,MYCOLOR1,NULL); //create mask where black becomes   //end of platform sprite creation

  /*if (has_water) {
    map_water_platforms_sprite_mask=CreateBitmapMask(map_water_platforms_sprite,MYCOLOR1,NULL); //create mask where black becomes   //end of platform sprite creation
  }*/
  //printf("mask created\n");

  //map_platforms_shadow_shader_mask=CreateBitmapMask(map_platforms_shadow_shader,MYCOLOR1,NULL);
//}*/




void InitLevel(bool load_lvl)
{
  wchar_t txt[128];
  wchar_t lvl_name[128];
  level_loading=TRUE;
  loading_numerator=0;
  loading_denominator=0;

  if (load_lvl) {
    swprintf(lvl_name,128,L"%s",level_names[level_chosen]);    
    swprintf(txt,128,L"saves/%s/level.txt",level_names[level_chosen]);
    if (!free_will) {
      if (!game_hard) {
        swprintf(save_level,128,L"saves/%s/scores.txt",level_names[level_chosen]);
      } else {
        swprintf(save_level,128,L"saves/%s/scores_hard.txt",level_names[level_chosen]);
      }
    } else {
      if (!game_hard) {
        swprintf(save_level,128,L"saves/%s/scores_fw.txt",level_names[level_chosen]);
      } else {
        swprintf(save_level,128,L"saves/%s/scores_hard_fw.txt",level_names[level_chosen]);
      }
    }
  } else {
    swprintf(lvl_name,128,L"__006__");    
    swprintf(txt,128,L"saves/__006__/level.txt");
    swprintf(save_level,128,L"saves/__006__/scores.txt");
  }


  bool saves_exist=LoadSave(txt,TRUE);

  if (!saves_exist) {
    in_main_menu=TRUE;
    blank_level=TRUE;
    level_loaded=TRUE;
    level_loading=FALSE;

    /*int dice=abs(RandNum(0,100,&misc_rng_i,-1));
    if (dice<30) {
      rain_grad_rise=2;
      rain_grad_run=1;
      if (dice>15) {
        map_weather=1;
      } else {
        map_weather=2;
      }
    } else {
      map_weather=0;
    }*/

    return;
  } else {
    blank_level=FALSE;
  }

  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;
  

  srand(time(NULL));
  timeBeginPeriod(1);


  InitOnce();//cannot be repeatedly run
  Init();


  //InitOnce2
  InitGroundWaterObj();
  InitGroundFireObj();
  //Load Enemy cache sprites
  InitGridTiles(lvl_name);
  InitEnemySprites();
  loading_denominator=SHADOW_GRID_NUM+PLATFORM_GRID_NUM+FOREGROUND_GRID_NUM+ENEMY_TYPE_NUM+(LARGE_ENEMY_TYPE_NUM*ROTATED_SPRITE_NUM*2)+LARGER_ENEMY_TYPE_NUM*ROTATED_SPRITE_NUM;

  InitEnemySpritesObj();
  InitPFEnemyObj();
  InitEnemyPathfindingNodes();

  if (load_lvl) {
    in_main_menu=FALSE;
  } else {
    int dice=abs(RandNum(0,100,&misc_rng_i,-1));
    if (dice<30) {
      if (dice>15) {
        map_weather=1;
      } else {
        map_weather=2;
      }
    } else {
      map_weather=0;
    }
  }


  flag_begin_drawing_tiles=TRUE; //flag draw level tiles using screen 
  loading_tile_grid_prog=1;
  //allocate smallest to biggest

  if (!stop_playing_song[0] && stop_playing_song[1] && load_lvl) {//main turntable open only please
    InitLoadLvlSong();
  }
}






