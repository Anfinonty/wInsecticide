

void InitOnce() {
  GR_WIDTH=SCREEN_WIDTH;
  GR_HEIGHT=SCREEN_HEIGHT;

  player.cam_move_x=0;
  player.cam_move_y=0;

//  alloc_enemy_once=TRUE;
  player_load_color=player_color;
  player_load_iris_color=player_iris_color;
  player_load_pupil_color=player_pupil_color;
  player_bullet_color=WHITE;

  if (IsInvertedBackground()) { //invert player color if background is inverted
    player_load_color=COLORS_NUM-player_color-1;
    player_load_iris_color=COLORS_NUM-player_iris_color-1;
    player_load_pupil_color=COLORS_NUM-player_pupil_color-1;

    player_bullet_color=BLACK;
  }

  adjustSFXVolume(&spamSoundEffectCache[0],&spamSoundEffect[0],game_volume/4,FALSE); //start
  adjustSFXVolume(&spamSoundEffectCache[1],&spamSoundEffect[1],game_volume/4,FALSE); //stop
  adjustSFXVolume(&spamSoundEffectCache[2],&spamSoundEffect[2],game_volume,FALSE); //clang

  adjustSFXVolume(&channelSoundEffectCache[0],&channelSoundEffect[0],game_volume/5,TRUE); //speed
  adjustSFXVolume(&channelSoundEffectCache[1],&channelSoundEffect[1],game_volume,TRUE); //clang_death
}



void Init(HDC hdc) {
  time_begin=current_timestamp();

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


  //Initialize Level
  InitBullet();
  InitGrid();
  InitNodeGrid();
  InitGround();
  InitNodeGridAttributes();
  InitEnemy();
  InitPlayer();
  BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
  for (int i=0;i<SND_THREAD_NUM-1;i++) {
    mem_snd_interrupt[i]=TRUE;
    waveOutReset(hWaveOut[i]);
  }

  if (!run_after_once) {
    run_after_once=TRUE;
  } else {
    PlaySound(NULL, NULL, SND_ASYNC);
  }
}



void InitPlatformsSprite(HWND hwnd, HDC hdc)
{
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

  DeleteDC(hdc2);
  EndPaint(hwnd, &ps);

  //map_platforms_sprite=ReplaceColor(tmp_map_platforms_sprite,MYCOLOR1,BLACK,NULL);
  map_platforms_sprite_mask=CreateBitmapMask(map_platforms_sprite,MYCOLOR1,NULL); //create mask where black becomes   //end of platform sprite creation
}



void InitLevel(HWND hwnd, HDC hdc)
{
  wchar_t txt[128];
  swprintf(txt,128,L"saves/%s/level.txt",level_names[level_chosen]);
  swprintf(save_level,128,L"saves/%s/scores.txt",level_names[level_chosen]);

  LoadSave(txt);

  srand(time(NULL));
  timeBeginPeriod(1);


  InitOnce();//cannot be repeatedly run
  Init(hdc);



  //Load Player Cosmetics
  DeleteObject(player.sprite_1);
  DeleteObject(player.sprite_2);
  DeleteObject(player.sprite_jump);

  DeleteObject(player.attack_sprite_1);
  DeleteObject(player.attack_sprite_2);
  DeleteObject(player.attack_sprite_3);
  DeleteObject(player.attack_sprite_4);

  DeleteObject(player.block_sprite_1);
  DeleteObject(player.block_sprite_2);
  DeleteObject(player.block_sprite_3);

  DeleteObject(player.spin_sprite);



  player.sprite_1 = RotateSprite(NULL, player.osprite_1,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.sprite_2 = RotateSprite(NULL, player.osprite_2,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.sprite_jump = RotateSprite(NULL, player.osprite_jump,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);

  player.attack_sprite_1 = RotateSprite(NULL, player.oattack_sprite_1,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.attack_sprite_2 = RotateSprite(NULL, player.oattack_sprite_2,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.attack_sprite_3 = RotateSprite(NULL, player.oattack_sprite_3,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.attack_sprite_4 = RotateSprite(NULL, player.oattack_sprite_4,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);

  player.block_sprite_1 = RotateSprite(NULL, player.oblock_sprite_1,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.block_sprite_2 = RotateSprite(NULL, player.oblock_sprite_2,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);
  player.block_sprite_3 = RotateSprite(NULL, player.oblock_sprite_3,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);

  player.spin_sprite = RotateSprite(NULL, player.ospin_sprite,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);




  //Load Player Sprites
  player.sprite_jump_cache = RotateSprite(NULL, player.sprite_jump,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.sprite_1_cache = RotateSprite(NULL, player.sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.sprite_2_cache = RotateSprite(NULL, player.sprite_2,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  player.attack_sprite_1_cache = RotateSprite(NULL, player.attack_sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.attack_sprite_2_cache = RotateSprite(NULL, player.attack_sprite_2,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.attack_sprite_3_cache = RotateSprite(NULL, player.attack_sprite_3,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.attack_sprite_4_cache = RotateSprite(NULL, player.attack_sprite_4,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  player.block_sprite_1_cache = RotateSprite(NULL, player.block_sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.block_sprite_2_cache = RotateSprite(NULL, player.block_sprite_2,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.block_sprite_3_cache = RotateSprite(NULL, player.block_sprite_3,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  player.spin_sprite_1_cache = RotateSprite(NULL, player.spin_sprite,0.1,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.spin_sprite_2_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI_2,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.spin_sprite_3_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  player.spin_sprite_4_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI+M_PI_2,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  //moon sprite
  DeleteObject(moon_sprite_cache);
  HBITMAP tmp_moon_sprite=CopyCrunchyBitmap(moon_sprite,NOTSRCCOPY);
  moon_sprite_cache=RotateSprite(NULL, tmp_moon_sprite,0,PURPLE,BLACK,BLACK,-1);
  DeleteObject(tmp_moon_sprite);

  DeleteObject(mouse_cursor_sprite_cache);
  DeleteObject(mouse_cursor_sprite_cache2);
  DeleteObject(mouse_cursor_sprite_iris_cache);
  DeleteObject(mouse_cursor_sprite_iris_cache2);
  DeleteObject(mouse_cursor_sprite_pupil_cache);
  DeleteObject(mouse_cursor_sprite_pupil_cache2);

  mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);



  mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,draw_color_arr[player_load_iris_color]);

  mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,draw_color_arr[player_load_iris_color]);



  mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,draw_color_arr[player_load_pupil_color]);

  mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,draw_color_arr[player_load_pupil_color]);


  //Load Enemy cache spritesF
  InitEnemySprites();

  InitPlatformsSprite(hwnd,hdc);

  /*printf("clang_audio:%lu\n",clang_audio_filesize);
  printf("tb_start_audio:%lu\n",tb_start_audio_filesize);
  printf("tb_death_audio:%lu\n",tb_stop_audio_filesize);
  printf("clang_death_audio:%lu\n",clang_death_audio_filesize);*/


  //allocate smallest to biggest




  //load_sound=TRUE;
  level_loaded=TRUE;
  in_main_menu=FALSE;
}


