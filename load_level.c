

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

  adjustSFXVolume(&spamSoundEffectCache[3],&spamSoundEffect[3],game_volume/4,FALSE);//block
  adjustSFXVolume(&spamSoundEffectCache[4],&spamSoundEffect[4],game_volume/4,FALSE);//block perfect
  adjustSFXVolume(&spamSoundEffectCache[5],&spamSoundEffect[5],game_volume/4,FALSE);//hurt

  adjustSFXVolume(&channelSoundEffectCache[0],&channelSoundEffect[0],game_volume/5,TRUE); //speed
  adjustSFXVolume(&channelSoundEffectCache[1],&channelSoundEffect[1],game_volume,TRUE); //clang_death
  adjustSFXVolume(&channelSoundEffectCache[2],&channelSoundEffect[2],game_volume/5,TRUE); //knife throw
  adjustSFXVolume(&channelSoundEffectCache[3],&channelSoundEffect[3],game_volume/10,TRUE); //rain
  adjustSFXVolume(&channelSoundEffectCache[4],&channelSoundEffect[4],game_volume/30,TRUE); //rain softer

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
        play_new_song=TRUE;
      }
      _wclosedir(d);
    }
  }
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
  InitGround(TRUE);
  InitNodeGridAttributes();
  InitEnemy();
  InitPlayer();


  InitScreenRainDrop();
  InitBulletRain();
  BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);

  mem_snd_interrupt[0]=TRUE;
  waveOutReset(hWaveOut[0]);

  mem_snd_interrupt[1]=TRUE;
  waveOutReset(hWaveOut[1]);

  mem_snd_interrupt[3]=TRUE;
  waveOutReset(hWaveOut[3]);



  if (!run_after_once) {
    run_after_once=TRUE;
  } else {
    PlaySound(NULL, NULL, SND_ASYNC);
  }
}



void InitPlatformsSprite(HWND hwnd, HDC hdc)
{


  wchar_t bmp_save[64];
  swprintf(bmp_save,64,L"saves/%s/map.bmp",level_names[level_chosen]);
  wchar_t bmp_save_shadow[64];
  swprintf(bmp_save_shadow,64,L"saves/%s/map_shadow.bmp",level_names[level_chosen]);
  //SaveBitmapToFile2(map_platforms_sprite,rgbColorsDefault,bmp_save);

  map_platforms_sprite=(HBITMAP)LoadImageW(
        NULL,               // Handle to the instance (NULL for file)
        bmp_save,           // File name of the bitmap
        IMAGE_BITMAP,       // Type of image
        0, 0,               // Desired width and height (0 to use actual size)
        LR_LOADFROMFILE   // Load from file
        | LR_CREATEDIBSECTION // Create a DIB section
    );
  map_platforms_shadow_shader = (HBITMAP)LoadImageW(
        NULL,               // Handle to the instance (NULL for file)
        bmp_save_shadow,           // File name of the bitmap
        IMAGE_BITMAP,       // Type of image
        0, 0,               // Desired width and height (0 to use actual size)
        LR_LOADFROMFILE   // Load from file
        | LR_CREATEDIBSECTION // Create a DIB section
  );

  if (map_platforms_sprite==NULL || map_platforms_shadow_shader==NULL) {
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
    CreatePlatformShadowBitmap(hdc2);//,map_platforms_sprite);
    SaveBitmapToFile2(map_platforms_shadow_shader,rgbColorsDefault, bmp_save_shadow);
    //}  

    DeleteDC(hdc2);
    EndPaint(hwnd, &ps);
    SaveBitmapToFile2(map_platforms_sprite,rgbColorsDefault, bmp_save);
    //BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
  }

  map_platforms_sprite_mask=CreateBitmapMask(map_platforms_sprite,MYCOLOR1,NULL); //create mask where black becomes   //end of platform sprite creation
  //printf("mask created\n");

  //map_platforms_shadow_shader_mask=CreateBitmapMask(map_platforms_shadow_shader,MYCOLOR1,NULL);
}



void InitLevel(HWND hwnd, HDC hdc)
{
  wchar_t txt[128];
  swprintf(txt,128,L"saves/%s/level.txt",level_names[level_chosen]);
  swprintf(save_level,128,L"saves/%s/scores.txt",level_names[level_chosen]);

  LoadSave(txt,TRUE);

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


  FreeDrawSprite(&player.draw_sprite_1);
  FreeDrawSprite(&player.draw_sprite_2);
  FreeDrawSprite(&player.draw_sprite_jump);
  FreeDrawSprite(&player.draw_attack_sprite_1);
  FreeDrawSprite(&player.draw_attack_sprite_2);
  FreeDrawSprite(&player.draw_attack_sprite_3);
  FreeDrawSprite(&player.draw_attack_sprite_4);
  FreeDrawSprite(&player.draw_block_sprite_1);
  FreeDrawSprite(&player.draw_block_sprite_2);
  FreeDrawSprite(&player.draw_block_sprite_3);
  FreeDrawSprite(&player.draw_spin_sprite_1);
  FreeDrawSprite(&player.draw_spin_sprite_2);
  FreeDrawSprite(&player.draw_spin_sprite_3);
  FreeDrawSprite(&player.draw_spin_sprite_4);


  FreeDrawSprite(&player.draw_blur_sprite_jump);
  FreeDrawSprite(&player.draw_spin_blur_sprite_1);
  FreeDrawSprite(&player.draw_spin_blur_sprite_2);
  FreeDrawSprite(&player.draw_spin_blur_sprite_3);
  FreeDrawSprite(&player.draw_spin_blur_sprite_4);



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




  player.blur_sprite_jump_cache = RotateSprite(NULL, player.sprite_jump,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],TRANSPARENT);

  player.spin_blur_sprite_1_cache = RotateSprite(NULL, player.spin_sprite,0.1,LTGREEN,BLACK,draw_color_arr[player_load_color],TRANSPARENT);
  player.spin_blur_sprite_2_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI_2,LTGREEN,BLACK,draw_color_arr[player_load_color],TRANSPARENT);
  player.spin_blur_sprite_3_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI,LTGREEN,BLACK,draw_color_arr[player_load_color],TRANSPARENT);
  player.spin_blur_sprite_4_cache = RotateSprite(NULL, player.spin_sprite,0.1+M_PI+M_PI_2,LTGREEN,BLACK,draw_color_arr[player_load_color],TRANSPARENT);


  GenerateDrawSprite(&player.draw_sprite_jump,player.sprite_jump_cache);

  GenerateDrawSprite(&player.draw_sprite_1,player.sprite_1_cache);
  GenerateDrawSprite(&player.draw_sprite_2,player.sprite_2_cache);

  GenerateDrawSprite(&player.draw_attack_sprite_1,player.attack_sprite_1_cache);
  GenerateDrawSprite(&player.draw_attack_sprite_2,player.attack_sprite_2_cache);
  GenerateDrawSprite(&player.draw_attack_sprite_3,player.attack_sprite_3_cache); 
  GenerateDrawSprite(&player.draw_attack_sprite_4,player.attack_sprite_4_cache);

  GenerateDrawSprite(&player.draw_block_sprite_1,player.block_sprite_1_cache);
  GenerateDrawSprite(&player.draw_block_sprite_2,player.block_sprite_2_cache);
  GenerateDrawSprite(&player.draw_block_sprite_3,player.block_sprite_3_cache);

  GenerateDrawSprite(&player.draw_spin_sprite_1,player.spin_sprite_1_cache);
  GenerateDrawSprite(&player.draw_spin_sprite_2,player.spin_sprite_2_cache);
  GenerateDrawSprite(&player.draw_spin_sprite_3,player.spin_sprite_3_cache);
  GenerateDrawSprite(&player.draw_spin_sprite_4,player.spin_sprite_4_cache);


  GenerateDrawSprite(&player.draw_blur_sprite_jump,player.blur_sprite_jump_cache);
  GenerateDrawSprite(&player.draw_spin_blur_sprite_1,player.spin_blur_sprite_1_cache);
  GenerateDrawSprite(&player.draw_spin_blur_sprite_2,player.spin_blur_sprite_2_cache);
  GenerateDrawSprite(&player.draw_spin_blur_sprite_3,player.spin_blur_sprite_3_cache);
  GenerateDrawSprite(&player.draw_spin_blur_sprite_4,player.spin_blur_sprite_4_cache);


  //moon sprite
  DeleteObject(moon_sprite_cache);
  FreeDrawSprite(&draw_moon_sprite);

  HBITMAP tmp_moon_sprite=CopyCrunchyBitmap(moon_sprite,NOTSRCCOPY);
  moon_sprite_cache=RotateSprite(NULL, tmp_moon_sprite,0,LPURPLE,BLACK,BLACK,-1);
  DeleteObject(tmp_moon_sprite);
  GenerateDrawSprite(&draw_moon_sprite,moon_sprite_cache);



  DeleteObject(mouse_cursor_sprite_cache);
  DeleteObject(mouse_cursor_sprite_cache2);
  DeleteObject(mouse_cursor_sprite_iris_cache);
  DeleteObject(mouse_cursor_sprite_iris_cache2);
  DeleteObject(mouse_cursor_sprite_pupil_cache);
  DeleteObject(mouse_cursor_sprite_pupil_cache2);


  FreeDrawSprite(&draw_mouse_cursor_sprite);
  FreeDrawSprite(&draw_mouse_cursor_sprite_iris);
  FreeDrawSprite(&draw_mouse_cursor_sprite_pupil);

  FreeDrawSprite(&draw_mouse_cursor_sprite2);
  FreeDrawSprite(&draw_mouse_cursor_sprite_iris2);
  FreeDrawSprite(&draw_mouse_cursor_sprite_pupil2);

  mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);



  mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,draw_color_arr[player_load_iris_color]);

  mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,draw_color_arr[player_load_iris_color]);



  mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,draw_color_arr[player_load_pupil_color]);

  mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,draw_color_arr[player_load_pupil_color]);



  GenerateDrawSprite(&draw_mouse_cursor_sprite,mouse_cursor_sprite_cache);
  GenerateDrawSprite(&draw_mouse_cursor_sprite_iris,mouse_cursor_sprite_iris_cache);
  GenerateDrawSprite(&draw_mouse_cursor_sprite_pupil,mouse_cursor_sprite_pupil_cache);

  GenerateDrawSprite(&draw_mouse_cursor_sprite2,mouse_cursor_sprite_cache2);
  GenerateDrawSprite(&draw_mouse_cursor_sprite_iris2,mouse_cursor_sprite_iris_cache2);
  GenerateDrawSprite(&draw_mouse_cursor_sprite_pupil2,mouse_cursor_sprite_pupil_cache2);




  //Load Enemy cache spritesF
  InitEnemySprites();

  InitPlatformsSprite(hwnd,hdc);


  //allocate smallest to biggest




  //load_sound=TRUE;
  level_loaded=TRUE;
  in_main_menu=FALSE;
}



void LoadMainMenuBackground()
{
  DeleteObject(map_background_sprite);
  HBITMAP tmp_map_background_sprite;
  if (solar_hour>6 && solar_hour<18) { //day
    if (GR_WIDTH<800 && GR_HEIGHT<600) {
      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    } else {
      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/sky_hd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
  } else { //night
    if (GR_WIDTH<800 && GR_HEIGHT<600) {
      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    } else {
      tmp_map_background_sprite=(HBITMAP) LoadImageW(NULL, L"sprites/stars_hd.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    }
  }
  map_background_sprite=CopyStretchBitmap(tmp_map_background_sprite,SRCCOPY,GR_WIDTH,GR_HEIGHT); //note runs once only
  DeleteObject(tmp_map_background_sprite);
}




