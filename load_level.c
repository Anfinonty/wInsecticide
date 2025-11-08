

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
  adjustSFXVolume(&spamSoundEffectCache[0],&spamSoundEffect[0],game_volume,FALSE); //timebreaker start
  adjustSFXVolume(&spamSoundEffectCache[1],&spamSoundEffect[1],game_volume/2,FALSE); //timebreaker stop
  adjustSFXVolume(&spamSoundEffectCache[2],&spamSoundEffect[2],game_volume/4,FALSE); //enemy hurt

  adjustSFXVolume(&spamSoundEffectCache[3],&spamSoundEffect[3],game_volume/4,FALSE);//block
  adjustSFXVolume(&spamSoundEffectCache[4],&spamSoundEffect[4],game_volume/4,FALSE);//block perfect
  adjustSFXVolume(&spamSoundEffectCache[5],&spamSoundEffect[5],game_volume,FALSE);//player hurt
  adjustSFXVolume(&spamSoundEffectCache[6],&spamSoundEffect[6],game_volume/8,FALSE);//enemy block
  adjustSFXVolume(&spamSoundEffectCache[7],&spamSoundEffect[7],game_volume/3,FALSE); //clang
  adjustSFXVolume(&spamSoundEffectCache[8],&spamSoundEffect[8],game_volume/2,FALSE); //powerup

  adjustSFXVolume(&channelSoundEffectCache[0],&channelSoundEffect[0],game_volume,TRUE); //player death
  adjustSFXVolume(&channelSoundEffectCache[1],&channelSoundEffect[1],game_volume/2,TRUE); //enemy death 
  adjustSFXVolume(&channelSoundEffectCache[2],&channelSoundEffect[2],game_volume,TRUE); //knife throw
  //adjustSFXVolume(&channelSoundEffectCache[3],&channelSoundEffect[3],game_volume/10,TRUE); //rain
  //adjustSFXVolume(&channelSoundEffectCache[4],&channelSoundEffect[4],game_volume/30,TRUE); //rain softer <-- 2 layers
  adjustSFXVolume(&channelSoundEffectCache[5],&channelSoundEffect[5],game_volume,TRUE); //shotgun
  adjustSFXVolume(&channelSoundEffectCache[6],&channelSoundEffect[6],game_volume,TRUE); //sniper
  adjustSFXVolume(&channelSoundEffectCache[7],&channelSoundEffect[7],game_volume,TRUE); //reload shotgun
  adjustSFXVolume(&channelSoundEffectCache[8],&channelSoundEffect[8],game_volume,TRUE); //gun shoot
  adjustSFXVolume(&channelSoundEffectCache[9],&channelSoundEffect[9],game_volume/2,TRUE); //load knife
  adjustSFXVolume(&channelSoundEffectCache[10],&channelSoundEffect[10],game_volume/2,TRUE); //load 3 knife
  adjustSFXVolume(&channelSoundEffectCache[11],&channelSoundEffect[11],game_volume,TRUE); //load gun
  adjustSFXVolume(&channelSoundEffectCache[12],&channelSoundEffect[12],game_volume,TRUE); //gun empty
}


void InitLoadLvlSong()
{
  //Load custom song
  wchar_t dirname[64];
  swprintf(dirname,64,L"saves/%ls/music",level_names[level_chosen]);
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
        swprintf(src_music_dir,64,L"saves/%ls/music",level_names[level_chosen]);
        play_new_song[0]=TRUE;
      }
      _wclosedir(d);
    }
  } //else {
    //InitSongBank();
    //play_new_song=TRUE;
  //}
}


void Init() { //Repeatable
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

  flag_draw_game_background_sprite=TRUE;

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
  //global_update_reflection_timer=501;

  //fire_rng_i=0;

  //Initialize Level
  InitClouds();
  InitSun();
  InitStars();
  InitMoon();
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
  InitShootingStars();
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






void InitLevel(bool load_lvl)
{
  wchar_t txt[128];
  wchar_t lvl_name[128];
  level_loading=TRUE;
  //printf("Loading_level...\n");
  loading_numerator=0;
  loading_denominator=0;

  if (load_lvl) {
    swprintf(lvl_name,128,L"%ls",level_names[level_chosen]);    
    swprintf(txt,128,L"saves/%ls/level.txt",level_names[level_chosen]);
    if (!free_will) {
      if (!game_hard) {
        swprintf(save_level,128,L"saves/%ls/scores.txt",level_names[level_chosen]);
      } else {
        swprintf(save_level,128,L"saves/%ls/scores_hard.txt",level_names[level_chosen]);
      }
    } else {
      if (!game_hard) {
        swprintf(save_level,128,L"saves/%ls/scores_fw.txt",level_names[level_chosen]);
      } else {
        swprintf(save_level,128,L"saves/%ls/scores_hard_fw.txt",level_names[level_chosen]);
      }
    }
  } else { //load main menu level
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
    return;
  } else {
    blank_level=FALSE;
  }

  flag_update_background=TRUE;
  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;
  

  srand(time(NULL));
  timeBeginPeriod(1);

  GR_WIDTH=SCREEN_WIDTH;
  GR_HEIGHT=SCREEN_HEIGHT;

  player.cam_move_x=0;
  player.cam_move_y=0;

  player_load_color=player_color;
  player_load_iris_color=player_iris_color;
  player_load_pupil_color=player_pupil_color;
  player_bullet_color=WHITE;

  Init(); //Repeatable, Load Save via \n

  //Load nalloc-able objects
  InitGroundWaterObj();
  InitGroundFireObj();
  InitGridTiles(lvl_name);
  InitEnemySprites();

  //declare size of denominator
  loading_denominator=SHADOW_GRID_NUM+PLATFORM_GRID_NUM/*+FOREGROUND_GRID_NUM*/+ENEMY_TYPE_NUM+(LARGE_ENEMY_TYPE_NUM*ROTATED_SPRITE_NUM*2)+LARGER_ENEMY_TYPE_NUM*ROTATED_SPRITE_NUM;

  InitEnemySpritesObj();
  InitPFEnemyObj();
  InitEnemyPathfindingNodes();

  if (load_lvl) { //not in main menu
    in_main_menu=FALSE;
  } else { //going to main menu
    int dice=abs(RandNum(0,100,&misc_rng_i,-1)); //random weather
    if (dice<30) {
      if (dice>15) {
        map_weather=1; //rain
      } else if (dice<7) {
        map_weather=2; //snow
      } else {
        map_weather=3; //hailstorm
      }
    } else {
      map_weather=0; //clear
    }
  }


  flag_begin_drawing_tiles=TRUE; //flag draw level tiles using screen 
  loading_tile_grid_prog=1;
  //allocate smallest to biggest

  if (!stop_playing_song[0] && stop_playing_song[1] && load_lvl) {//main turntable open only please
    InitLoadLvlSong();
  }
  //printf("level_loaded\n\n");
}






