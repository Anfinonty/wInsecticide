
bool IsSpeedBreaking()
{
  if (player.sleep_timer==SLOWDOWN_SLEEP_TIMER) {
    return TRUE;
  }
  return FALSE;
}




void Click()
{
//Left click
  if (player.rst_left_click) {
    player.left_click=TRUE;
    player.left_click_hold_timer=64;
  }
  if (player.left_click_hold_timer>0) {
    player.left_click_hold_timer--;
  }
  if (player.left_click_hold_timer==0) {
    player.left_click=FALSE;
  }
//Right click
  if (player.rst_right_click) {
    player.right_click=TRUE;
    player.right_click_hold_timer=64;
  }
  if (player.right_click_hold_timer>0) {
    player.right_click_hold_timer--;
//    printf("%d",player.right_click_hold_timer);
  }
  if (player.right_click_hold_timer==0) {
    player.right_click=FALSE;
  }
}


//Player
void PlayerCameraLimiterBorder()
{
  int 
    px=player.x,
    py=player.y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y;

  //left x
  int border_x=(GR_WIDTH/2-px+cx1+cx2+24);

  //right x
  int rborder_w=GR_WIDTH/2-(MAP_WIDTH-px+cx1+cx2-24);

  if (border_x>0) { //near left x border
    player.cam_limiter_x=-border_x;
  } else if (rborder_w>0) {// near right x borderf
    player.cam_limiter_x=(rborder_w);
  } else {
    player.cam_limiter_x=0;

  }



  //top y
  int border_y=GR_HEIGHT/2-py+cy1+cy2+24;

  //lower y
  int d_border_h=GR_HEIGHT/2-(MAP_HEIGHT-py+cy1+cy2-24);

  if (border_y>0) { //near top border
    player.cam_limiter_y=-border_y;
  } else if (d_border_h>0) { //near down border
    player.cam_limiter_y=d_border_h;
  } else {
    player.cam_limiter_y=0;
  }  
}


void move_x(float x)
{
  player.x+=x;
  player.cam_x-=x;
}


void move_y(float y)
{
  player.y+=y;
  player.cam_y-=y;
}


void CameraInit(float x,float y)
{
  while (x<GR_WIDTH/2) {
    player.cam_x++;
    x++;
  }
  while (x>GR_WIDTH/2) {
    player.cam_x--;
    x--;
  }
  while (y<GR_HEIGHT/2) {
    player.cam_y++;
    y++;
  }
  while (y>GR_HEIGHT/2) {
    player.cam_y--;
    y--;
  }
}

void InitPlayerCamera(float target_x,float target_y)
{
//set camera
  player.cam_x=0;
  player.cam_y=0;
  player.cam_limiter_x=0;
  player.cam_limiter_y=0;
  CameraInit(target_x,target_y);
}

void PlayerPlaceWeb()
{
  player.web_storage[player.placed_web_pos]=-1; //make current web empty
  player.placed_web_pos++; //move to next web
  player.placed_web_num++; //increase number of placed web
  if (player.placed_web_pos>=player.max_web_num) { //go back to index 0 if over the limit
    player.placed_web_pos=0;
  }
}


void PlayerActPlaceWeb(int bm_x1,int bm_y1,int bm_x2,int bm_y2)
{  
  if (player.placed_web_pos<player.max_web_num) { //if pointer to web is less than the no. of webs player has currently     
    while (player.web_storage[player.placed_web_pos]==-1) { //find player.web_storage that is not empty
      player.placed_web_pos=LimitValue(player.placed_web_pos+1,0,player.max_web_num); //reset back to 0 if over the max
    }
    int web_id=player.web_storage[player.placed_web_pos];
    if (web_id!=-1) {
      player.previous_web_placed=web_id;
      Ground[web_id]->x1=bm_x1;
      Ground[web_id]->y1=bm_y1;
      Ground[web_id]->x2=bm_x2;
      Ground[web_id]->y2=bm_y2;
      SetGround(web_id);
      SetNodeGridAttributes(web_id);
      PlayerPlaceWeb();            
      Ground[web_id]->health=1500;
      if (game_audio) {
        PlayMemSnd(&channelSoundEffect[2],&channelSoundEffectCache[2],TRUE,3);
      }
    }
  }
}

void InitRotatedSpriteAndCanvas(DRAWSPRITE *target_sprite, DRAWSPRITE *target_dithered_sprite, HBITMAP canvas, HBITMAP loaded_sprite,float radians)
{
    //create rotated sprite
    RotateBitBlt8Bit(canvas, loaded_sprite, radians, 167, canvas);
    ReplaceBitmapColor2(canvas,LTGREEN,YELLOW,BLACK,LTGREEN);
    ReplaceBitmapColor(canvas,YELLOW,BLACK);
    GenerateDrawSpriteIII(target_sprite,canvas,radians,loaded_sprite);

    //Craete dithhered roated sprite
    DitherBitmapColor(canvas,-1,BLACK);
    GenerateDrawSpriteIII(target_dithered_sprite,canvas,radians,loaded_sprite); 
}


int prelude_player_step=0;
int prelude_player_step_i=-1;//transitioning from step 0 to step 1, 0 has to be included

void InitPlayerSpritesAll()
{
  //HBITMAP tmp_bitmap;

  //bee_sprite
  /*tmp_bitmap=CopyCrunchyBitmap(LoadPlayerSprite.sprite_bee_1,SRCCOPY);
  ReplaceBitmapColor2(tmp_bitmap,LTGREEN,BLACK,8,LTGREEN);
  GenerateDrawSprite(&PlayerSprite[0].sprite_bee_1,tmp_bitmap);
  DeleteObject(tmp_bitmap);

  tmp_bitmap=CopyCrunchyBitmap(LoadPlayerSprite.sprite_bee_2,SRCCOPY);
  ReplaceBitmapColor2(tmp_bitmap,LTGREEN,BLACK,8,LTGREEN);
  GenerateDrawSprite(&PlayerSprite[0].sprite_bee_2,tmp_bitmap);
  DeleteObject(tmp_bitmap);

  tmp_bitmap=CopyCrunchyBitmap(LoadPlayerSprite.sprite_bee_aero_1,SRCCOPY);
  ReplaceBitmapColor2(tmp_bitmap,LTGREEN,BLACK,8,LTGREEN);
  GenerateDrawSprite(&PlayerSprite[0].sprite_bee_aero_1,tmp_bitmap);
  DeleteObject(tmp_bitmap);

  tmp_bitmap=CopyCrunchyBitmap(LoadPlayerSprite.sprite_bee_aero_2,SRCCOPY);
  ReplaceBitmapColor2(tmp_bitmap,LTGREEN,BLACK,8,LTGREEN);
  GenerateDrawSprite(&PlayerSprite[0].sprite_bee_aero_2,tmp_bitmap);
  DeleteObject(tmp_bitmap);*/
  float angle_rn;
  float t_angle;

  int i=prelude_player_step_i;
  int k_max=1;
  if (prelude_player_step>=2 && prelude_player_step<=10) {
    k_max=8;    
  }
  for (int k=0;k<k_max;k++) {


  if (prelude_player_step>=2 && prelude_player_step<=10) {
    i=prelude_player_step_i;
    angle_rn=M_PI_2-M_PI_32*i;
  }

  switch (prelude_player_step) {
    case 0:
  //spider jump sprites
      BitBlt8BitTransparent(small_entity_canvas, 0,0, 32,32, LoadPlayerSprite.sprite_jump, 0, 0, BROWN, FALSE);
      ReplaceBitmapColor2(small_entity_canvas,LTGREEN,YELLOW,BLACK,LTGREEN);
      ReplaceBitmapColor(small_entity_canvas,YELLOW,BLACK);
      GenerateDrawSpriteII(&PlayerSprite[0].sprite_jump,small_entity_canvas,LoadPlayerSprite.sprite_jump);

      DitherBitmapColor(small_entity_canvas,-1,BLACK);
      GenerateDrawSpriteII(&PlayerSprite[0].dithered_sprite_jump,small_entity_canvas,LoadPlayerSprite.sprite_jump);
  
      //spiderSprite - blur sprite jump
      BitBlt8BitTransparent(small_entity_canvas, 0,0, 32,32, LoadPlayerSprite.sprite_jump, 0, 0, BROWN, FALSE);
      ReplaceBitmapColor2(small_entity_canvas,LTGREEN,YELLOW,BLACK,LTGREEN); //set backgroudn as yellow, set player to ltgreen
      ReplaceBitmapColor(small_entity_canvas,YELLOW,BLACK); //set yellow background as black
      DitherBitmapColor(small_entity_canvas,LTBLUE,BLACK); //dither borders black
      ReplaceBitmapColor(small_entity_canvas,LTBLUE,LTRED); //change remaining borders to iris color
      DitherBitmapColor(small_entity_canvas,LTGREEN,BLACK); //dither player sprite
      GenerateDrawSpriteII(&PlayerSprite[0].blur_sprite_jump,small_entity_canvas,LoadPlayerSprite.sprite_jump);
      prelude_player_step++;
      break;

    case 1:
      switch (i) {
        case 0:t_angle=0.1;break;
        case 1:t_angle=0.1+M_PI_2;break;
        case 2:t_angle=0.1+M_PI;break;
        case 3:t_angle=0.1+M_PI+M_PI_2;break;
      }
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].spin_sprite[i],&PlayerSprite[0].dithered_spin_sprite[i],small_entity_canvas,LoadPlayerSprite.spin_sprite, t_angle);

      //create blur sprite
      RotateBitBlt8Bit(small_entity_canvas, LoadPlayerSprite.spin_sprite, t_angle, 167, small_entity_canvas);
      ReplaceBitmapColor2(small_entity_canvas,LTGREEN,YELLOW,BLACK,LTGREEN);
      ReplaceBitmapColor(small_entity_canvas,YELLOW,BLACK);
      DitherBitmapColor(small_entity_canvas,LTBLUE,BLACK);
      ReplaceBitmapColor(small_entity_canvas,LTBLUE,LTRED);
      DitherBitmapColor(small_entity_canvas,LTGREEN,BLACK);
      GenerateDrawSpriteIII(&PlayerSprite[0].blur_spin_sprite[i],small_entity_canvas,t_angle,LoadPlayerSprite.spin_sprite);
      break;

    case 2:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].sprite_1[i],&PlayerSprite[0].dithered_sprite_1[i],small_entity_canvas,LoadPlayerSprite.sprite_1, angle_rn);    
      break;
    case 3: 
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].sprite_2[i],&PlayerSprite[0].dithered_sprite_2[i],small_entity_canvas,LoadPlayerSprite.sprite_2, angle_rn);
      break;
    case 4:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].attack_sprite_1[i],&PlayerSprite[0].dithered_attack_sprite_1[i],small_entity_canvas,LoadPlayerSprite.attack_sprite_1, angle_rn);
      break;
    case 5:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].attack_sprite_2[i],&PlayerSprite[0].dithered_attack_sprite_2[i],small_entity_canvas,LoadPlayerSprite.attack_sprite_2, angle_rn);
      break;
    case 6:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].attack_sprite_3[i],&PlayerSprite[0].dithered_attack_sprite_3[i],small_entity_canvas,LoadPlayerSprite.attack_sprite_3, angle_rn);
      break;
    case 7: 
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].attack_sprite_4[i],&PlayerSprite[0].dithered_attack_sprite_4[i],small_entity_canvas,LoadPlayerSprite.attack_sprite_4, angle_rn);
      break;
    case 8:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].block_sprite_1[i],&PlayerSprite[0].dithered_block_sprite_1[i],small_entity_canvas,LoadPlayerSprite.block_sprite_1, angle_rn);
      break;
    case 9:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].block_sprite_2[i],&PlayerSprite[0].dithered_block_sprite_2[i],small_entity_canvas,LoadPlayerSprite.block_sprite_2, angle_rn);
      break;
    case 10:
      InitRotatedSpriteAndCanvas(&PlayerSprite[0].block_sprite_3[i],&PlayerSprite[0].dithered_block_sprite_3[i],small_entity_canvas,LoadPlayerSprite.block_sprite_3, angle_rn);
      break;


  //change player color palette
    case 11:
      CopyReplaceColorPalette(PlayerSprite[0].PlayerPalette,rgbColorsDefault,167,rgbPaint[player_color]);
      CopyReplaceColorPalette(PlayerSprite[0].PlayerPalette,PlayerSprite[0].PlayerPalette,151,LTGRAY); //border
      CopyReplaceColorPalette(PlayerSprite[0].PlayerPalette,PlayerSprite[0].PlayerPalette,199,rgbPaint[player_iris_color]);

      CopyReplaceColorPalette(PlayerSprite[0].PlayerBlurPalette,rgbColorsDefault,167,rgbPaint[player_color]);
      CopyReplaceColorPalette(PlayerSprite[0].PlayerBlurPalette,PlayerSprite[0].PlayerBlurPalette,151,LTGRAY); //border
      CopyReplaceColorPalette(PlayerSprite[0].PlayerBlurPalette,PlayerSprite[0].PlayerBlurPalette,199,rgbPaint[player_pupil_color]);
      break;
  } //end of switch statement

  if (prelude_player_step>=2 && prelude_player_step<=10) {
    loading_numerator++;
    prelude_player_step_i++;
    if (prelude_player_step_i==PLAYER_ROTATED_SPRITE_NUM) {
      prelude_player_step++;
      prelude_player_step_i=0;
    }
  }
  if (prelude_player_step==1) {
    loading_numerator++;
    prelude_player_step_i++;
    if (prelude_player_step_i==4) {
      prelude_player_step++;
      prelude_player_step_i=0;
    }
  }



  } //end of for loop
}


void InitPlayerFlingWeb()
{
  for (int i=0;i<PLAYER_FLING_WEB_NUM;i++) {
    player_fling_web.x[i]=0;
    player_fling_web.y[i]=0;
    player_fling_web.sprite_x[i]=-40;
    player_fling_web.sprite_y[i]=-40;
  }
}



void InitPlayer() {
  int i;
  //player.hiding=FALSE;
  player.type=0;
  //player.type=1;
  if (player.type==0) {
    player_bullet_color=WHITE;
  } else {
    player_bullet_color=YELLOW;
  }

  player.rng_i=0;
  player.death_bullet_rng_i=0;
  player.bullet_rng_i=0;
  player.sniper_bullet_rng_i=0;

  player.rst_left_click=FALSE;
  player.rst_right_click=FALSE;
  player.left_click=FALSE;
  player.right_click=FALSE;
  player.rst_down=FALSE;
  player.rst_left=FALSE;
  player.rst_right=FALSE;
  player.rst_up=FALSE;
  player.flag_death=FALSE;

  player.above_ground_edge=FALSE;
  player.below_ground_edge=FALSE;


  player.rst_arrow_down=FALSE;
  player.rst_arrow_left=FALSE;
  player.rst_arrow_right=FALSE;
  player.rst_arrow_up=FALSE;
  player.rst_right_click_snd=FALSE;
  player.play_gun_snd=0;

  player.low_jump=TRUE;
  //player.rst_key_sprint=TRUE;
  player.last_left=FALSE;
  player.jump=FALSE;
  player.current_above=FALSE;
  player.current_below=FALSE;
  player.previous_above=FALSE;
  player.previous_below=FALSE;
  player.print_current_above=FALSE;
  player.print_current_below=FALSE;
  player.time_breaker=FALSE;
  player.attack=FALSE;
  player.blocking=FALSE;
  player.print_valid_web=FALSE;
  player.valid_web=FALSE;
  player.attack_rst=FALSE;
  player.destroy_ground=FALSE;
  player.uppercut=FALSE;
  player.flag_revert_palette=FALSE;
  player.flag_noir_palette=FALSE;
  player.is_on_ground_edge=FALSE;
  //player.is_on_ground_edge2=FALSE;
  player.is_on_left_ground_edge=FALSE;
  player.is_on_right_ground_edge=FALSE;
  player.on_ground_edge_id=-1;
  player.saved_on_ground_edge_id=-1;

  player.show_health_timer=HP_SHOW_TIMER_NUM;
  player.show_block_health_timer=HP_SHOW_TIMER_NUM;
  player.show_exp_timer=HP_SHOW_TIMER_NUM;

  player.grav=2;
  player.decceleration_timer=0;
  player.jump_height=0;
  player.sprite_timer=0;
  player.in_air_timer=0;
  player.speed=DEFAULT_PLAYER_SPEED;
  player.on_ground_timer=0;
  player.in_water_timer=0;
  player.on_ground_id=-1;
  player.below_ground_edge_timer=0;

//  player.on_ground_id_u1=-1;
  player.on_ground_id_u2=-1;
//  player.on_ground_id_d1=-1;
//  player.on_ground_id_d2=-1;

  player.edge_angle=0;

  player.above_head_x=0;
  player.above_head_y=0;

  player.on_a_ground=FALSE;
  player.saved_ground_id=-1;
  player.walk_cycle=0;
  player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
//  player.key_jump_timer=0;
  player.time_breaker_tick=0;
  player.color=player_color; //displayed color
  player.load_color=player_load_color; //orginal color

  //player.death_x=
  player.saved_x=saved_player_x;
  //player.death_y=
  player.saved_y=saved_player_y;
  player.x=player.saved_x;
  player.y=player.saved_y;
  player.claws_x=player.x;
  player.claws_y=player.y;
  player.claws_attack_x=player.x;
  player.claws_attack_y=player.y;

  player.below_x1=player.x;
  player.below_y1=player.y;

  player.below_x2=player.x;
  player.below_y2=player.y;

  player.above_x1=player.x;
  player.above_y1=player.y;

  player.above_x2=player.x;
  player.above_y2=player.y;

  //player.saved_sprite_angle=0;
  //player.saved_attack_sprite_angle=0;
  //player.saved_block_sprite_angle=0;
  player.sprite_angle=0;
  player.angle=0;
  player.saved_angle=0;
  player.player_grav=0.5;
  player.exp=0;
  player.invalid_shoot_timer=0;
  player.dmg_taken_timer=0;

  player.cam_x=0;
  player.cam_y=0;
  player.cam_move_x=0;
  player.cam_move_y=0;
  player.sprite_x=GR_WIDTH/2;
  player.sprite_y=GR_HEIGHT/2;

  player.death_timer=0;
  player.left_click_hold_timer=0;
  player.right_click_hold_timer=0;
  player.attack_timer=1;
  player.sleep_timer=DEFAULT_SLEEP_TIMER;
  player.hit_cooldown_timer=0;
  player.hurt_snd_timer=0;
  player.block_timer=0;
  player.block_recharge_timer=0;
  player.block_recharge_timer_max=100;//300;
  player.block_cooldown=0;
  player.block_cooldown_max=300;
  player.block_health_max=DEFAULT_PLAYER_BLOCK_HEALTH_MAX;
  player.block_health=DEFAULT_PLAYER_BLOCK_HEALTH_MAX;

  player.time_breaker_deplete_cooldown=0;
  player.time_breaker_units=0;
  player.time_breaker_units_max=DEFAULT_PLAYER_TIME_BREAKER_MAX;
  player.time_breaker_cooldown=0;
  player.time_breaker_cooldown_max=DEFAULT_PLAYER_TIME_BREAKER_COOLDOWN_MAX;
  player.time_breaker_recharge_timer=0;
  player.time_breaker_recharge_timer_max=DEFAULT_PLAYER_TIME_BREAKER_RECHARGE_MAX;
  player.time_breaker_units_tick=DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX;
  player.time_breaker_units_tick_max=DEFAULT_PLAYER_TIME_BREAKER_TICK_MAX;

  player.health=DEFAULT_PLAYER_HEALTH;
  player.knockback_strength=DEFAULT_PLAYER_KNOCKBACK_STRENGTH;
  player.knockback_speed=0.5;
  player.knockback_speed_multiplier=2;
  player.attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;

  player.max_web_num=DEFAULT_PLAYER_WEB_NUM;
  player.bullet_num=20;

  player.bullet_shot=-1;

  player.placed_web_pos=0;
  player.placed_web_num=0;
  player.destroyed_web_pos=0;


  player.knives_per_throw=1;
  player.shoot_knife_duration=0;


  player.mouse_dist=0;
  player.mouse_angle=0;


  player.in_water=FALSE;
  player.phase_web=FALSE;
  player.phase_web2=FALSE;
  player.hiding=FALSE;
  player.in_web_corner=FALSE;
  player.key_b4_corner_is_left=FALSE;
  player.key_b4_corner_is_right=FALSE;

  player.web_burned=FALSE;

  player.web_being_shot=-1;
  for (i=0;i<MAX_WEB_NUM;i++) {
    player.web_storage[i]=GROUND_NUM+i;
  }

  player.cdweb_player_pos=0;
  player.cdweb_pos=0;
  player.cdweb_num=0;
  for (i=0;i<MAX_WEB_NUM;i++) {
    player.cdwebs[i]=-1;
  }

  player.bullet_shot_num=0;
  for (i=0;i<PLAYER_BULLET_NUM;i++) {
    player.bullet[i]=-1;
  }

  player.pivot_x=-20;
  player.pivot_y=-20;
  player.pivot_on_ground_id=-1;
  player.pivot_angle=-20;
  player.pivot_length=-20;
  player.is_swinging=FALSE;
  player.jump_angle=0;
  player.jump_angle2=0;
  player.fling_distance=0;
  player.previous_web_placed=-1;

  player.is_rebounding=FALSE;

  player.angle_of_incidence=0;
  player.angle_of_reflection=0;
  player.spin_timer=0;
  
  player.current_blur_sprite=0;
  for (i=0;i<PLAYER_BLUR_NUM;i++) {
    player.blur_timer[i]=0;
    player.blur_x[i]=0;
    player.blur_y[i]=0;
  }

  InitPlayerCamera(player.saved_x,player.saved_y);

  InitPlayerFlingWeb();

  PlayerCameraLimiterBorder();
}




void RegainWeb(int web_id)
{
  while (player.web_storage[player.destroyed_web_pos]!=-1) {//prevent overwrite of existing
    player.destroyed_web_pos=LimitValue(player.destroyed_web_pos+1,0,player.max_web_num);
  }
  player.web_storage[player.destroyed_web_pos]=web_id;
  player.destroyed_web_pos=LimitValue(player.destroyed_web_pos+1,0,player.max_web_num);

  //player.placed_web[player.placed_web_num]=-1;
  player.placed_web_num--;
}


void PlayerRevive()
{
  player.flag_death=FALSE;
  player.block_health=0;
  player.time_breaker_units=0;
  if (!game_hard) {
    player.health=player.bullet_shot_num;
  } else {
    player.health=1+player.bullet_shot_num/2;
  }
  player.speed=30;
  player.death_timer=0;
  for (int f=0;f<player.bullet_shot_num;f++) { //reset bullets
    StopBullet(player.bullet[f],TRUE);
  }
  player.bullet_shot_num=0;
  InitPlayerCamera(player.x,player.y);
}

void PlayerActGroundEdgeMovement()
{

  float edge_dist1;
  float edge_dist2;
  float distl=20;

  if (!player.blocking && !player.rst_up) {
      if (!player.is_on_ground_edge) {
        if (player.on_ground_id!=-1) {
          edge_dist1=GetDistance(player.x,player.y,Ground[player.on_ground_id]->x1,Ground[player.on_ground_id]->y1); //left edge
          edge_dist2=GetDistance(player.x,player.y,Ground[player.on_ground_id]->x2,Ground[player.on_ground_id]->y2); //right edge
          if (edge_dist1<distl || edge_dist2<distl) {
            if (edge_dist1<distl) {
              player.is_on_left_ground_edge=TRUE;
              player.is_on_right_ground_edge=FALSE;
            } else if (edge_dist2<distl) {
              player.is_on_left_ground_edge=FALSE;
              player.is_on_right_ground_edge=TRUE;
            }
            player.on_ground_edge_id=player.on_ground_id;
            player.saved_on_ground_edge_id=player.on_ground_edge_id;
            player.is_on_ground_edge=TRUE;
          }
        }
      } else { //on ground edge id present
        //player.in_air_timer=0;
        if (player.is_on_left_ground_edge) {
          edge_dist1=GetDistance(player.x,player.y,Ground[player.on_ground_edge_id]->x1,Ground[player.on_ground_edge_id]->y1); //left edge
          player.edge_angle=GetCosAngle(player.x-Ground[player.on_ground_edge_id]->x1,edge_dist1);
          if (player.y<Ground[player.on_ground_edge_id]->y1) { //above pivot
              player.above_ground_edge=TRUE;
              player.below_ground_edge=FALSE;
              if (player.rst_right) { //clockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=FALSE;
              } else if (player.rst_left) { //anticlockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=TRUE;
              }
          } else { //below pivot
              player.above_ground_edge=FALSE;
              player.below_ground_edge=TRUE;
              player.below_ground_edge_timer=5;
              if (player.rst_right) { //clockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=TRUE;
              } else if (player.rst_left) { //anticlockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=FALSE;
              }
          }
          if (edge_dist1>distl+1) {
            player.on_ground_edge_id=-1;
            player.saved_on_ground_edge_id=-1;
            player.is_on_ground_edge=FALSE;
            player.is_on_left_ground_edge=FALSE;
            player.is_on_right_ground_edge=FALSE;
          }
        } else if (player.is_on_right_ground_edge) {
          edge_dist2=GetDistance(player.x,player.y,Ground[player.on_ground_edge_id]->x2,Ground[player.on_ground_edge_id]->y2); //right edge
          player.edge_angle=GetCosAngle(player.x-Ground[player.on_ground_edge_id]->x2,edge_dist2);
          if (player.y<Ground[player.on_ground_edge_id]->y2) { //above pivot
              player.above_ground_edge=TRUE;
              player.below_ground_edge=FALSE;
              if (player.rst_right) { //clockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=FALSE;
              } else if (player.rst_left) { //anticlockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=TRUE;
              }
          } else { //below pivot
              player.above_ground_edge=FALSE;
              player.below_ground_edge=TRUE;
              player.below_ground_edge_timer=5;
              if (player.rst_right) { //clockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=TRUE;
              } else if (player.rst_left) { //anticlockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.2);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.2);
                player.last_left=FALSE;
              }
          }
          if (edge_dist2>distl+1) {
            player.on_ground_edge_id=-1;
            player.saved_on_ground_edge_id=-1;
            player.is_on_ground_edge=FALSE;
            player.is_on_left_ground_edge=FALSE;
            player.is_on_right_ground_edge=FALSE;
          }
        }
      }
  } else { //player is blocking
    player.on_ground_edge_id=-1;
    player.saved_on_ground_edge_id=-1;
    player.is_on_ground_edge=FALSE;
    player.is_on_left_ground_edge=FALSE;
    player.is_on_right_ground_edge=FALSE;
  }

  if (player.on_ground_id!=-1 && player.on_ground_id!=player.on_ground_edge_id) {
    player.on_ground_edge_id=player.on_ground_id;
    player.saved_on_ground_edge_id=player.on_ground_id;
    player.is_on_ground_edge=FALSE;
    player.is_on_left_ground_edge=FALSE;
    player.is_on_right_ground_edge=FALSE;
//    player.below_ground_edge_timer=0;
  }

}

void PlayerActXMovement(int grav_speed)
{
  /*if (player.is_on_ground_edge && player.grav==1) {
    if (player.rst_left) {
      player.last_left=TRUE;
    } else if (player.rst_right) {
      player.last_left=FALSE;
    }
    grav_speed=2;
    player.on_ground_edge_id=-1;
    player.is_on_ground_edge=FALSE;
    player.is_on_left_ground_edge=FALSE;
    player.is_on_right_ground_edge=FALSE;
  }*/

  bool allow_act=FALSE;
  if (player.fling_distance<=0 && !player.phase_web && !player.phase_web2 && !player.in_web_corner) { //player's fling distance is 0
    player.previous_web_placed=-1;
    if (!player.is_swinging && !player.is_on_ground_edge) { // player is not swinging and not on ground edge
      if (player.on_ground_id==-1) { //player is not on ground
        allow_act=TRUE;
      } else if (!player.blocking) { //on a ground but not blocking
        allow_act=TRUE;
      } 
    } else if (player.on_ground_id!=-1 && !player.is_on_ground_edge) { //is swinging but on ground
      if (player.pivot_length<DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE) { //allow movement if within swing distance
        allow_act=TRUE;
      }
    }
  }
  if (grav_speed==0 && player.is_on_ground_edge && player.speed>5 && player.on_ground_id==-1) {
    player.decceleration_timer+=2;
  }
  if (grav_speed==0 && allow_act) { //ground movement
    if (player.speed>10 && player.on_ground_timer>0 && !player.is_on_ground_edge) {
      player.decceleration_timer+=2;
    }
    if (player.rst_left || player.rst_right) {
      player.is_rebounding=FALSE;
    }
    if (player.print_current_above) { //player is above ground
      if (player.rst_right) {
        move_x(cos(player.angle));
        move_y(sin(player.angle));
        player.last_left=FALSE;
      } else if (player.rst_left) {
        move_x(-cos(player.angle));
        move_y(-sin(player.angle));
        player.last_left=TRUE;
      }
    } else if (player.print_current_below) { //player is below ground
      if (player.rst_right) {
        move_x(-cos(player.angle));
        move_y(-sin(player.angle));
        player.last_left=FALSE;
      } else if (player.rst_left) {
        move_x(cos(player.angle));
        move_y(sin(player.angle));
        player.last_left=TRUE;
      }
    } else { //player is not on ground
      if (player.fling_distance==0) {
        if (player.type==1) { //bee up down
          if (player.rst_up) {
            move_y(-1);
          } else if (player.rst_down) {
            move_y(1);
          }
        }
        if (player.rst_left) {
          if (player.below_ground_edge_timer==0) {
            move_x(-1);
            player.last_left=TRUE;
          } else {
            move_x(1);
            player.last_left=FALSE;
          }
        } else if (player.rst_right) {
          if (player.below_ground_edge_timer==0) {
            move_x(1);
            player.last_left=FALSE;
          } else {
            move_x(-1);
            player.last_left=TRUE;
          }
        }
      } else {
        if (player.angle_of_reflection>M_PI_2 && player.angle_of_reflection<M_PI+M_PI_2) {//lefft side
          if (player.rst_right) {
            move_x(1);
            player.last_left=TRUE;
          }
        } else {
          if (player.rst_left) {
            move_x(-1);
            player.last_left=FALSE;
          }
        }
      }
    }
  }

}

void PlayerOnGroundAction(int speed, int grav, int height_from_player_x)
{
  player.angle=Ground[player.on_ground_id]->angle; //set player angle

  if (player.in_air_timer>0) {
    if (player.in_air_timer>1000) { //make player rebound
      player.in_air_timer=1000;
    }
    if (player.type==0) {
    if (player.rst_down) { //spinning
      player.is_rebounding=TRUE;
      //if (player.rst_left || player.rst_right) { //continue spin-rebound
        player.in_air_timer=1000;
      //}
    } else { //not reboubding
      player.is_rebounding=FALSE;
      player.in_air_timer=1;

      player.jump_height=0; //Stop Jump & stick to ground
      //if (player.uppercut) {
        //player.jump_height=-1; //Stop Jump & stick to ground
      //}
      player.jump=FALSE;
    }
    }
    player.in_air_timer--;
  }


  
//outwards from ground
  //above ground
  if (0<height_from_player_x && height_from_player_x<10) {
    player.current_above=TRUE;
    player.current_below=FALSE;
    player.previous_above=TRUE;
    player.previous_below=FALSE;
    player.on_a_ground=TRUE;
    player.on_ground_timer=20;

    if (player.on_ground_id<GROUND_NUM) {
          if (player.on_ground_id_u2!=-1 || player.in_web_corner) {
            move_x(-cos(player.angle+M_PI_2)*2);
            move_y(-sin(player.angle+M_PI_2)*2);
            if (!player.in_web_corner) {
              if (player.rst_left) {
                player.key_b4_corner_is_left=TRUE;
              } else if (player.rst_right) {
                player.key_b4_corner_is_right=TRUE;
              }
              player.in_web_corner=TRUE;
            }
          } else {
            if (height_from_player_x<5 || player.is_rebounding /*|| player.is_swinging*/ || player.type==1) {
              if (player.is_rebounding) {
                move_x(-cos(player.angle+M_PI_2)*2);
                move_y(-sin(player.angle+M_PI_2)*2);
              } else {
                move_x(-cos(player.angle+M_PI_2));
                move_y(-sin(player.angle+M_PI_2));
              }
            }
          }
    } else {
      if (!player.phase_web) {
        if (height_from_player_x<5 || player.is_rebounding || player.type==1) {
          if (player.on_ground_id_u2!=-1 && !player.is_rebounding) {
            if (!player.in_water) {
              move_x(-cos(player.angle-M_PI_2)*0.5);
              move_y(-sin(player.angle-M_PI_2)*0.5);
            }
            if (!player.in_web_corner) {
              if (player.rst_left) {
                player.key_b4_corner_is_left=TRUE;
              } else if (player.rst_right) {
                player.key_b4_corner_is_right=TRUE;
              }
              player.in_web_corner=TRUE;
            }
          } else {
            move_x(-cos(player.angle+M_PI_2)*1);
            move_y(-sin(player.angle+M_PI_2)*1);
          }
        }
      } else if (!player.is_on_ground_edge && !player.is_swinging) {
        move_x(-cos(player.angle-M_PI_2));
        move_y(-sin(player.angle-M_PI_2));
      }
    }
    //if (speed==0 && grav==0) {
      //player.on_ground_edge_id=-1;
      //player.is_on_ground_edge=FALSE;
      //player.is_on_left_ground_edge=FALSE;
      //player.is_on_right_ground_edge=FALSE;
    //}
    player.jump_angle=player.angle+M_PI_2;
    if (player.angle<0) {/*Slope -. /*/
      player.jump_angle2=2*M_PI+player.angle-M_PI_2;
    } else {/*Slope -. \*/
      player.jump_angle2=player.angle-M_PI_2;
    }
    player.fling_distance=0; //on ground, stop flinging

    //angle of incidence and reflection
    player.angle_of_reflection=
        //2*M_PI-player.angle_of_incidence+2*player.angle; //real
        GetBounceAngle(player.angle_of_incidence,player.angle);


    if (!player.is_swinging && player.bullet_shot==-1) {
      player.angle_of_incidence=player.angle_of_reflection;
    }

  //below ground
  } else if (-10<height_from_player_x && height_from_player_x<=0) { 
    player.below_ground_edge_timer=5;
    player.current_below=TRUE;
    player.current_above=FALSE;
    player.previous_above=FALSE;
    player.previous_below=TRUE;
    player.on_a_ground=TRUE;
    player.on_ground_timer=20;
    if (player.on_ground_id<GROUND_NUM) {
      if (player.on_ground_id_u2!=-1 || player.in_web_corner) {
            move_x(-cos(player.angle-M_PI_2)*2); //go outwards 
            move_y(-sin(player.angle-M_PI_2)*2);
            if (!player.in_web_corner) {
              if (player.rst_left) {
                player.key_b4_corner_is_left=TRUE;
              } else if (player.rst_right) {
                player.key_b4_corner_is_right=TRUE;
              }
              player.in_web_corner=TRUE;
            }
      } else {
        if (height_from_player_x>-5 || player.is_rebounding/* || player.is_swinging*/ || player.type==1) {
          if (player.is_rebounding) {
            move_x(-cos(player.angle-M_PI_2)*2); //go outwards 
            move_y(-sin(player.angle-M_PI_2)*2);
          } else {
            move_x(-cos(player.angle-M_PI_2)); //go outwards 
            move_y(-sin(player.angle-M_PI_2));
          }
        }
      }
    } else {
      if (!player.phase_web2) {
        if (height_from_player_x>-5 || player.is_rebounding || player.type==1) {
          if (player.on_ground_id_u2!=-1 && !player.is_rebounding) { //go inwards to web if corner
            if (!player.in_water) {
              move_x(-cos(player.angle+M_PI_2)*0.5);
              move_y(-sin(player.angle+M_PI_2)*0.5);
            } 
            if (!player.in_web_corner) {
              if (player.rst_left) {
                player.key_b4_corner_is_left=TRUE;
              } else if (player.rst_right) {
                player.key_b4_corner_is_right=TRUE;
              }
            }
            player.in_web_corner=TRUE;
          } else { //act normal if no corners
            move_x(-cos(player.angle-M_PI_2)*1);
            move_y(-sin(player.angle-M_PI_2)*1);
          }
        }
      } else if (!player.is_on_ground_edge && !player.is_swinging) {
        move_x(-cos(player.angle+M_PI_2));
        move_y(-sin(player.angle+M_PI_2));
      }
    }
    //if (speed==0 && grav==0) {
      //player.on_ground_edge_id=-1;
      //player.is_on_ground_edge=FALSE;
      //player.is_on_left_ground_edge=FALSE;
      //player.is_on_right_ground_edge=FALSE;
    //}
    player.jump_angle=player.angle-M_PI_2;
    if (player.angle<0) {/*Slope -. /*/
      player.jump_angle2=M_PI_2+player.angle;
    } else {/*Slope -. \*/
      player.jump_angle2=player.angle+M_PI_2;
    }
    player.fling_distance=0; //on ground, stop flinging

    //angle of incidence and reflection
    player.angle_of_reflection=GetBounceAngle(player.angle,player.angle_of_incidence);
    //2*M_PI-player.angle_of_incidence+2*player.angle; //real
    if (!player.is_swinging && player.bullet_shot==-1) {
      player.angle_of_incidence=player.angle_of_reflection;
    }
  }


  /*if (player.angle_of_reflection>2*M_PI) {
    player.angle_of_reflection-=2*M_PI;
  }
  if (player.angle_of_reflection<0) {
    player.angle_of_reflection+=2*M_PI;
  }*/

  if (height_from_player_x>9 || height_from_player_x<-9) {//leave ground when out of circular range
    player.on_a_ground=FALSE;
    player.on_ground_timer=0;

    player.in_web_corner=FALSE;
    player.key_b4_corner_is_left=FALSE;
    player.key_b4_corner_is_right=FALSE;
  }



  if (player.key_b4_corner_is_left && player.rst_right) {
    player.in_web_corner=FALSE;
    player.key_b4_corner_is_left=FALSE;
  }
  if (player.key_b4_corner_is_right && player.rst_left) {
    player.in_web_corner=FALSE;
    player.key_b4_corner_is_right=FALSE;
  }

  /*if (player.is_on_ground_edge && (player.rst_left || player.rst_right)) {
    //player.grav=1;
    player.in_web_corner=FALSE;
    player.key_b4_corner_is_left=FALSE;
    player.key_b4_corner_is_right=FALSE;
    player.is_on_ground_edge=FALSE;
  }*/
}



void PlayerActFlingWeb(int speed)
{
  float _a=player.x;
  float _b=player.y;
  float _l=player.pivot_length/PLAYER_FLING_WEB_NUM;
  int tmp_ground_id=-1;
  int tmp_ngid=-1;
  int dist1=-1;
  int dist2=-1;
  for (int i=0;i<PLAYER_FLING_WEB_NUM;i++) {
    player_fling_web.x[i]=_a;
    player_fling_web.y[i]=_b;          
    player_fling_web.sprite_x[i]=_a+player.cam_x+player.cam_move_x+player.cam_mouse_move_x+player.cam_limiter_x;
    player_fling_web.sprite_y[i]=_b+player.cam_y+player.cam_move_y+ player.cam_mouse_move_y+player.cam_limiter_y;

  //to allow web bending       
    tmp_ground_id=GetOnGroundId(player_fling_web.x[i],player_fling_web.y[i],5,4);
    tmp_ngid=GetGridId(player_fling_web.x[i],player_fling_web.y[i],MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
    if (tmp_ngid!=-1 && speed==3) { //web bend and placement
      if (NodeGrid[tmp_ngid]->node_fire) {
        player.attack_rst=TRUE;
        player.web_burned=TRUE;
        InitPlayerFlingWeb();
        break;
      }
    }
    //printf("tmp_ground_id:%d\n",tmp_ground_id);
    if (tmp_ground_id!=-1 && speed==3) { //web bend and placement
      dist1=GetDistance(player_fling_web.x[i],player_fling_web.y[i],Ground[tmp_ground_id]->x1,Ground[tmp_ground_id]->y1);
      dist2=GetDistance(player_fling_web.x[i],player_fling_web.y[i],Ground[tmp_ground_id]->x2,Ground[tmp_ground_id]->y2);
      if ((dist1<=NODE_SIZE) || (dist2<=NODE_SIZE)) { //placement if wihtin dist
      //player place web after swing
        if (player.max_web_num-player.placed_web_num>1 && 
            GetDistance(player.pivot_x,player.pivot_y,player_fling_web.x[i],player_fling_web.y[i])>NODE_SIZE*2 /*&& 
            !player.hiding*/) {//prevent build web if web too short
          player.pivot_on_ground_id=tmp_ground_id;
          float bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
          if (player.x<player.pivot_x) {
            bm_x1=player_fling_web.x[i];
            bm_y1=player_fling_web.y[i];	
            bm_x2=player.pivot_x;
            bm_y2=player.pivot_y;
          } else {
            bm_x1=player.pivot_x;
            bm_y1=player.pivot_y;
            bm_x2=player_fling_web.x[i];
            bm_y2=player_fling_web.y[i];
          }
          PlayerActPlaceWeb(bm_x1,bm_y1,bm_x2,bm_y2);
        }
        if (player.max_web_num-player.placed_web_num<=1 || !player.low_jump) {
          player.attack_rst=TRUE;
        }

        //change pivot
        player.pivot_x=player_fling_web.x[i];
        player.pivot_y=player_fling_web.y[i];

        InitPlayerFlingWeb();
        break;
      }
    }

    if (player.y>player.pivot_y) {
      _a-=_l*cos(player.pivot_angle);
      _b-=_l*sin(player.pivot_angle);
    } else {
      _a-=_l*cos(player.pivot_angle);
      _b+=_l*sin(player.pivot_angle);
    }


  }
}





void PlayerActSwinging(int grav_speed)
{
    player.grav=5;
    player.is_rebounding=FALSE;
    player.pivot_length=GetDistance(player.pivot_x,player.pivot_y,player.x,player.y);
    player.pivot_angle=GetCosAngle(player.x-player.pivot_x,player.pivot_length);
    bool yes_rubberband=TRUE;

    //>>>Calculate the Angle of incidence of player when swung from pivot
    if (player.y>player.pivot_y) { //lower quad
      if(player.last_left) {
        player.angle_of_incidence=player.pivot_angle+M_PI_2;
      } else {
        player.angle_of_incidence=player.pivot_angle-M_PI_2;
      }
    } else {//upper quad
      if(player.last_left) {
        player.angle_of_incidence=2*M_PI-player.pivot_angle-M_PI_2;
      } else {
        player.angle_of_incidence=2*M_PI-player.pivot_angle+M_PI_2;
      }
    }
    player.angle_of_reflection=player.angle_of_incidence;

    if (player.on_ground_id==-1) {
      if (player.phase_web) { 
        player.phase_web=FALSE;
      }
      if (player.phase_web2) {
        player.phase_web2=FALSE;
      }
    }

    if (player.pivot_x-60<player.x && player.x<player.pivot_x+60 && player.y>player.pivot_y) {
      yes_rubberband=FALSE;
    }


    if (grav_speed==3 && !player.is_on_ground_edge) {
      if (player.y>player.pivot_y) { //below pivot
        if (player.rst_right && player.on_ground_id==-1) { //Clockwize
          //if (!player.uppercut) { //move the usual direction
            move_x(-cos(-player.pivot_angle+M_PI_2));
            move_y(sin(-player.pivot_angle+M_PI_2));
            player.last_left=TRUE;
          /*} else { //move the opposite direction, anticlockwize
            move_x(cos(-player.pivot_angle+M_PI_2));
            move_y(-sin(-player.pivot_angle+M_PI_2));
            player.last_left=FALSE;
          }*/
        }
        if (player.rst_left && player.on_ground_id==-1) {  //Anticlockwize
          //if (!player.uppercut) {
            move_x(cos(-player.pivot_angle+M_PI_2));
            move_y(-sin(-player.pivot_angle+M_PI_2));
            player.last_left=FALSE;
          /*} else { //clockwize
            move_x(-cos(-player.pivot_angle+M_PI_2));
            move_y(sin(-player.pivot_angle+M_PI_2));
            player.last_left=TRUE;
          }*/
        } else if (player.rst_up){ //Retract Web
          move_x(-cos(-player.pivot_angle));
          move_y(sin(-player.pivot_angle));
        } else if (player.rst_down && (!yes_rubberband || player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2)) {
          if (player.on_ground_id==-1) {
            move_x(cos(-player.pivot_angle)); //extend web
            move_y(-sin(-player.pivot_angle));
          } /*else {
          }*/
        }
      } else { //above pivot
        //yes_rubberband=TRUE;
        if (player.rst_right && player.on_ground_id==-1) { //Clockwize
          move_x(cos(-player.pivot_angle+M_PI_2));
          move_y(sin(-player.pivot_angle+M_PI_2));
          player.last_left=FALSE;
        }
        if (player.rst_left && player.on_ground_id==-1) { //Anticlockwize
          move_x(-cos(-player.pivot_angle+M_PI_2));
          move_y(-sin(-player.pivot_angle+M_PI_2));
          player.last_left=TRUE;
        } else if (player.rst_up){ //Retract web
          move_x(-cos(-player.pivot_angle));
          move_y(-sin(-player.pivot_angle));
        } else if (player.rst_down && player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) {
          if (player.on_ground_id==-1) {
            move_x(cos(-player.pivot_angle)); //extend web
            move_y(sin(-player.pivot_angle));
          } /*else {
          }*/
        }
      }
    }


    if (grav_speed==4 && yes_rubberband) {//only occurs right after grav_speed==3
      if (player.pivot_length>NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) { //rubber band back if pivot length too long
        if (player.y>player.pivot_y) {
          move_x(-cos(-player.pivot_angle));
          move_y(sin(-player.pivot_angle));
        } else {
          move_x(-cos(-player.pivot_angle));
          move_y(-sin(-player.pivot_angle));
        }
      }       
    }
}



void PlayerActGravityMovement(int grav_speed,int speed)
{
  if (grav_speed==0 && speed==0) {
    if (player.on_ground_id==-1 && !player.is_swinging && !player.is_on_ground_edge) {
      player.in_air_timer++;
      if (player.in_air_timer>2002) { //dont go above this limit
        player.in_air_timer--;
      }
      if (player.jump_height==0 && player.in_water_timer==0 && player.type==0) {
        if (player.in_air_timer<1100) { //note: 1001 is post fling or rebounding
          if (player.in_air_timer%14/*20*/==0 && player.grav<=100) {
            player.grav+=2;
          }
          if (player.fling_distance==0 && !player.is_rebounding && player.in_air_timer>74 && player.in_air_timer%25==0 && player.speed<15) {
            player.speed++;
          }
        } else {
          if (player.in_air_timer%7/*12*/==0 && player.grav<=100 && player.in_water_timer==0) {
            player.grav+=2;
          }
          if (player.fling_distance<-200 && !player.is_rebounding && player.in_air_timer%50==0 && player.speed<9) {
            player.speed++;
          }
        }
      }
    }
  }


  if (((player.speed<5) || (player.speed>=5 && grav_speed==0)) && player.type==0) {

    if (player.jump_height>0) { //Jumping action
      player.jump_height-=player.player_grav;
      //if (player.uppercut && (player.rst_left || player.rst_right)) {
        //player.jump_height=0;
      //}
      move_x(2*player.player_grav*-cos(player.jump_angle));
      move_y(2*player.player_grav*-sin(player.jump_angle)); //jump go against gravity and perpendicular from platform
      if (player.jump_height<=0) {
        player.jump=FALSE;
      }
    }
  }


  if (speed==0 && player.type==0) { //Gravity runs on grav speed
    if (player.on_ground_id==-1 && player.jump_height<=0) { //Credit: y4my4m for pushing me to pursue this gameplay aspect
      if (!player.is_swinging) { //not swinigng and player is not flinging
        if ((player.in_air_timer>4 || player.fling_distance<0)) {
          move_y(player.player_grav); //include while being rebounding and flinging
        }
      }
    } else { //landed on ground
      if (player.grav>=3 && !player.is_swinging && !player.is_on_ground_edge) { //no gravity if on ground edge
        if (player.grav>7) {
          player.cam_move_x*=-1;
        }
        player.cam_move_y/=-1.5;
      }
      if (!player.is_swinging)
        player.grav=2;
    }
  }
}



void PlayerActFlingMovement(int grav_speed)
{
  if (grav_speed==0 && !player.is_swinging && !player.is_rebounding) { 
    if (((player.rst_left || player.rst_right) && player.fling_distance<-100) || player.uppercut || player.on_ground_id!=-1 || player.web_burned) { //cancel flinging when left or right key is pressed
      player.fling_distance=0;
      //if (player.uppercut)
        //player.rst_down=FALSE;
    }
    if (player.web_burned) {
      player.web_burned=FALSE;
    }

    if (player.fling_distance>0) { //fling and against gravity
      move_x(cos(player.angle_of_reflection));
      move_y(sin(player.angle_of_reflection));
      player.fling_distance--;
      if (player.fling_distance==1) { //cap in air timer right before end of fling distance
        player.in_air_timer=1002;
        player.fling_distance=-1;
      }
    } else if (player.fling_distance<0){ //Continue moving but now theres gravity
      move_x(cos(player.angle_of_reflection));
      move_y(sin(player.angle_of_reflection));
      player.fling_distance--;
    }
  }

}

void PlayerActLoadWeaponSnd()
{
  if (game_audio) {
  switch (player.knives_per_throw) {
    case 1:
      PlayMemSnd(&channelSoundEffect[9],&channelSoundEffectCache[9],TRUE,5); 
      break;
    case 3:
      PlayMemSnd(&channelSoundEffect[10],&channelSoundEffectCache[10],TRUE,5); 
      break;
    case 5:
      PlayMemSnd(&channelSoundEffect[11],&channelSoundEffectCache[11],TRUE,5); 
      break;
    case 15:
      PlayMemSnd(&channelSoundEffect[7],&channelSoundEffectCache[7],TRUE,5); 
      break;
  }
  }
}

void PlayerActReboundActions(int grav_speed, int speed)
{
  if (player.is_rebounding && !player.is_swinging && grav_speed==0) {
    move_x(cos(player.angle_of_reflection));
    move_y(sin(player.angle_of_reflection));
    if (player.on_ground_id!=-1 && player.on_ground_id<GROUND_NUM && speed<=1 /*&& !player.time_breaker*/) {
      if (player.speed>10) {
        player.speed=1+12*player.speed/20;//1+player.speed*5/6;
      } else if (player.speed>5) {
        player.speed-=2;//player.speed*5/6;
      }
    }
  }
  if (player.is_rebounding) {//rebounding
    if ((M_PI_2<player.angle_of_incidence && player.angle_of_incidence<M_PI+M_PI_2) || 
        (M_PI_2<player.angle_of_reflection && player.angle_of_reflection<M_PI+M_PI_2)) {
      player.last_left=TRUE;
    } else {
      player.last_left=FALSE;
    }
  }
}


void PlayerActMouseMovement()
{
  player.mouse_dist=GetDistance(GR_WIDTH/2,GR_HEIGHT/2,mouse_x,mouse_y);
  player.mouse_angle=GetCosAngle(mouse_x-GR_WIDTH/2,player.mouse_dist);

  int divider=2;
  player.cam_mouse_move_x=-cos(player.mouse_angle)*player.mouse_dist/divider;
  if (mouse_x==GR_WIDTH/2)
    player.cam_mouse_move_x=0;

  if (mouse_y>GR_HEIGHT/2) //below half of screen
    player.cam_mouse_move_y=-sin(player.mouse_angle)*player.mouse_dist/divider;
  else if (mouse_y<GR_HEIGHT/2)
    player.cam_mouse_move_y=sin(player.mouse_angle)*player.mouse_dist/divider;
  else
    player.cam_mouse_move_y=0;


  /*if (player.x-(mouse_x+player.cam_mouse_move_x)<0)
    player.cam_mouse_move_x=0;*/
}


void PlayerActMouseClick()
{
  float grad_x1=0,grad_y1=0,grad_x2=0,grad_y2=0;
  bool allow_act=FALSE;
  if (player.attack_rst) {//Left click to Attack
    player.attack=TRUE;
    player.blocking=FALSE; //unblock

    if (player.bullet_shot!=-1) {
      if (game_audio) {
        PlayMemSnd(&channelSoundEffect[2],&channelSoundEffectCache[2],TRUE,3);
      }
      StopBullet(player.bullet_shot,TRUE); //Stop the web
      PlayerPlaceWeb(); //Web related
      player.web_being_shot=-1;
      player.bullet_shot=-1;
      //StopBullet(player.bullet_shot,TRUE); //stop all web bullets from acting
      //player.web_being_shot=-1;
    } else if (player.bullet_shot_num<PLAYER_BULLET_NUM && 
        (PLAYER_BULLET_NUM-player.bullet_shot_num>=player.knives_per_throw) && // a/b whehere a>=b a is bullet in storage, b is bullet consumption
      !player.is_swinging
    ) {



      int b_speed_m=9;
      float b_dmg_m=1;
      int b_g_type=7; //default type shootknives==3
      int b_range=MAX_WEB_LENGTH*2;
      /*if (player.speed>10) {
        b_speed_m=9;
      } else if (player.speed>5) {
        b_speed_m=7;      
      }*/

      if (player.speed>24)
        b_dmg_m=4;
      else if (player.speed>10)
        b_dmg_m=2;

      switch (player.knives_per_throw) {
        case 0:
        case 3:
          player.play_gun_snd=0;
          break;
        case 5:
          player.play_gun_snd=1;
          break;
        case 15:
          player.play_gun_snd=2;
          break;
      }


      grad_x1=player.sprite_x;
      grad_y1=player.sprite_y;
      grad_x2=mouse_x;
      grad_y2=mouse_y;
      float tmp_angle=0;
      if (player.knives_per_throw>4) {
        if (player.knives_per_throw==5) {
          b_range=MAX_WEB_LENGTH*3;
          b_speed_m=15;
          b_dmg_m=4;
          b_g_type=10; 
        } else if (player.knives_per_throw==15) {
          b_range=65;
          b_g_type=6; //shotgun
        }
        b_dmg_m=4;
      //} else if (player.knives_per_throw==3) { 
      } else if (player.knives_per_throw==1) {
        //b_range=145;
        b_g_type=9;
        b_dmg_m=3;
      }

      int kpt=player.knives_per_throw;
      switch (kpt) {
        case 5:if (player.bullet_num-1<0) {kpt=0;}break;
        case 15:if (player.bullet_num-3<0) {kpt=0;}break;
      }
      if (kpt>0) {
        if (game_audio) {player.shoot_knife_duration=1;}
        } else {player.invalid_shoot_timer=10;}

      for (int q=0;q<kpt;q++) {
        if (q>0) {   
          if (kpt!=5) {
              if (q%2==0) {//even
                tmp_angle+=M_PI_2/16*q;
              } else {
                tmp_angle-=M_PI_2/16*q;
              }
          } else {
              if (q%2==0) {//even
                tmp_angle+=M_PI_2/64*q;
              } else {
                tmp_angle-=M_PI_2/64*q;
              }
          }
        }
	    player.bullet[player.bullet_shot_num]=current_bullet_id;
        ShootBullet(
            current_bullet_id,
	        player.bullet_shot_num,
	        player_bullet_color,
            b_g_type, //graphics type
	        b_range/*MAX_WEB_LENGTH*/+player.speed, //range ==>
            1, //speed
	        b_speed_m, //speed multiplier
	        b_dmg_m, //damage
	        -2,
	        player.x, //so it doest get stuck to ground
	        player.y,
	        grad_x1,
	        grad_y1,
	        grad_x2,
	        grad_y2,
            tmp_angle //angle            
         );
        if (player.knives_per_throw>4 && q%5==0) {
          player.bullet_num--;
        }
        player.bullet_shot_num++;
        current_bullet_id++;
        if (current_bullet_id>=SHOOT_BULLET_NUM-1) {
          current_bullet_id=0;
        }

      }
    } else {
      if (!player.is_swinging) {
        player.invalid_shoot_timer=10;
      }
    }
  }

  if (player.right_click_hold_timer==RCLICK_HOLD_COOLDOWN) {//Right click to Shoot Again
    if (player.bullet_shot!=-1 && !player.is_swinging) {
      StopBullet(player.bullet_shot,TRUE); //Stop the web
      PlayerPlaceWeb(); //Web related
      player.web_being_shot=-1;
      player.bullet_shot=-1;
      if (game_audio) {
        PlayMemSnd(&channelSoundEffect[2],&channelSoundEffectCache[2],TRUE,3);
      }
    }
  }


  if (!player.attack_rst && !player.is_swinging) {
    allow_act=FALSE;
    if (player.right_click_hold_timer==RCLICK_HOLD_COOLDOWN && player.type==0) { //right click to shoot
      if (player.placed_web_num<player.max_web_num && //webs > 0 
        player.bullet_shot==-1) {
        player.rst_right_click_snd=TRUE;
        allow_act=TRUE;
      } else {
        player.invalid_shoot_timer=10;
      }
    }
    InitPlayerFlingWeb();
    if (allow_act) {
      grad_x1=player.sprite_x;
      grad_y1=player.sprite_y;
      grad_x2=mouse_x;
      grad_y2=mouse_y;

      player.attack=TRUE; 
	  player.bullet_shot=current_bullet_id;
      int sb_range=DEFAULT_PLAYER_BUILD_RANGE*2;
      if (player.speed>=24)
        sb_range=DEFAULT_PLAYER_BUILD_RANGE*5;
      ShootBullet(current_bullet_id,
	        -1,
	        //CYAN,
            WHITE,
            5, //graphics type
            sb_range,//*4,
            1, //speed
	        30, //speed multiplier
	        10+player.attack_strength*2, //damage
	        -1,
	        player.x,//player.above_x2, //so it doest get stuck to ground
	        player.y,//player.above_y2,
	        grad_x1,
	        grad_y1,
	        grad_x2,
	        grad_y2,
            0
      );
      current_bullet_id++; //public
      if (current_bullet_id>=SHOOT_BULLET_NUM-1) {
        current_bullet_id=0;
      } 
    }
  } else { //meelee attack only
    if (player.is_swinging) {
      if (player.left_click_hold_timer==62 || player.attack_rst || player.right_click_hold_timer==RCLICK_HOLD_COOLDOWN) { //swing but no web is placed
        if (player.on_ground_id!=-1) { 
          player.fling_distance=0;
        } else { //begin flinging!!
          player.fling_distance=player.pivot_length;
        }
        player.grav=3; //grav when swing let go
        player.in_air_timer=1000;
        player.decceleration_timer=0;
        if (player.on_ground_timer==0 && !player.is_on_ground_edge && !player.uppercut /*&& !player.rst_up && !player.rst_down*/ && player.type==0) {
          if (player.speed<5)
            player.speed+=3;
          else //if (player.speed<10)
            //player.speed+=2;
          //else
            player.speed++;
        }

        if (M_PI_2<player.angle_of_incidence && player.angle_of_incidence<M_PI+M_PI_2)
          player.last_left=TRUE;
        else
          player.last_left=FALSE;
      }
    }
    
    if ((player.left_click_hold_timer==62 || player.attack_rst || player.right_click_hold_timer==RCLICK_HOLD_COOLDOWN)) {
      if (player.is_swinging) {//prevent build web if web too short and hiding
        if (game_audio) {
          PlayMemSnd(&channelSoundEffect[2],&channelSoundEffectCache[2],TRUE,3);
        }
        player.is_swinging=FALSE; //stop swinging
        player.attack=TRUE;
        player.blocking=FALSE; //unblock


        //Ensures willingly placed webs never touch the ground or each other
        if (player.pivot_length>NODE_SIZE*5 && (player.right_click_hold_timer==RCLICK_HOLD_COOLDOWN)) {

    //player place web after swing
        float bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
        float bm_x1__,bm_y1__,bm_x2__,bm_y2__;
        if (player.x<player.pivot_x) {
          bm_x1=player.x;
          bm_y1=player.y;	
          bm_x2=player.pivot_x;
          bm_y2=player.pivot_y;
        } else {
          bm_x1=player.pivot_x;
          bm_y1=player.pivot_y;
          bm_x2=player.x;
          bm_y2=player.y;
        }


        if (player.y<player.pivot_y) { //player is above pivot
          //player is left
          if (player.x<player.pivot_x) {
            bm_x1-=cos(player.pivot_angle)*NODE_SIZE;
            bm_y1+=sin(player.pivot_angle)*NODE_SIZE;
            bm_x2+=cos(player.pivot_angle)*NODE_SIZE/2;
            bm_y2-=sin(player.pivot_angle)*NODE_SIZE/2;
          } else { //player is right
            bm_x1+=cos(player.pivot_angle)*NODE_SIZE/2;
            bm_y1-=sin(player.pivot_angle)*NODE_SIZE/2;
            bm_x2-=cos(player.pivot_angle)*NODE_SIZE;
            bm_y2+=sin(player.pivot_angle)*NODE_SIZE;
          }
        } else { //player is below pivot
          //player is left
          if (player.x<player.pivot_x) {
            bm_x1-=cos(player.pivot_angle)*NODE_SIZE;
            bm_y1-=sin(player.pivot_angle)*NODE_SIZE;
            bm_x2+=cos(player.pivot_angle)*NODE_SIZE/2;
            bm_y2+=sin(player.pivot_angle)*NODE_SIZE/2;
          } else { //player is right
            bm_x1+=cos(player.pivot_angle)*NODE_SIZE/2;
            bm_y1+=sin(player.pivot_angle)*NODE_SIZE/2;
            bm_x2-=cos(player.pivot_angle)*NODE_SIZE;
            bm_y2-=sin(player.pivot_angle)*NODE_SIZE;
          }
        }
        if (bm_x1>=bm_x2) {
          bm_x1__=bm_x2;
          bm_y1__=bm_y2;
          bm_x2__=bm_x1+0.00001;
          bm_y2__=bm_y1;
        } else {
          bm_x1__=bm_x1;
          bm_y1__=bm_y1;
          bm_x2__=bm_x2;
          bm_y2__=bm_y2;
        }
        
        PlayerActPlaceWeb(bm_x1__,bm_y1__,bm_x2__,bm_y2__);
        }
      }
    }
  }
  if (player.attack_rst) {
    player.attack_rst=FALSE;
  }
}

void PlayerActDestroyGround()
{
  if (player.destroy_ground) {
    if (player.is_on_ground_edge) { 
      if (player.saved_on_ground_edge_id>=GROUND_NUM && player.saved_on_ground_edge_id!=player.previous_web_placed) {
        DestroyGround(player.saved_on_ground_edge_id);  
	    RegainWeb(player.saved_on_ground_edge_id);
      }
    } else {
      if (player.on_ground_id>=GROUND_NUM && player.on_ground_id!=player.previous_web_placed) {
        DestroyGround(player.on_ground_id);  
  	    RegainWeb(player.on_ground_id);
      }
    }
    player.destroy_ground=FALSE;
  }


  if (player.health<=PLAYER_LOW_HEALTH && player.type==0) {
    if (player.speed<11) {
      player.speed=11;
    }
  }


  if (player.spin_timer>0 && player.speed<4) {
    player.speed=4;
    player.decceleration_timer=0;
  }
}


void PlayerActDecceleration()
{
  if (player.decceleration_timer==0) {
    if (!player.is_swinging) {
      player.decceleration_timer=100; //idling, goback to 100
      if (player.speed>=24) {
        if (IsSpeedBreaking())
          player.decceleration_timer=1500;//335;
        else
          player.decceleration_timer=1000;//250;
      } else if (player.speed>10) {
        if (IsSpeedBreaking())
          player.decceleration_timer=1000;//800;//320;
        else
          player.decceleration_timer=1500;//500;//117;
      } else {
        player.decceleration_timer=1500;
      }
    } else {
      if (player.speed>10) {
        if (IsSpeedBreaking()) {
          player.decceleration_timer=1000;//300;
        } else {
          player.decceleration_timer=1500;//320;
        }
      } else if (player.speed>5) {
        if (IsSpeedBreaking()) {
          player.decceleration_timer=0;//280;
        } else {
          player.decceleration_timer=0;//150;
        }
      }
    }
  }
  //if (player.speed<24 && player.uppercut) {
    //player.decceleration_timer=0;
  //} else if (player.decceleration_timer>0) {
    player.decceleration_timer++;
  //}
  if (player.decceleration_timer>3000/*350*/) {
    if (player.speed>1 && player.type==0) {
      if (!player.time_breaker) {
        if ((player.is_swinging || player.is_on_ground_edge)) {
          player.speed--;        
        } else if (player.speed>1 && player.on_ground_id<GROUND_NUM  && player.on_ground_id>-1) {
          player.speed--;        
        }
      }
    }
    player.decceleration_timer=0;
  }
}


void PlayerAct() 
{
  int speed=0,grav_speed=0,claws_l2=NODE_SIZE/2,claws_l=NODE_SIZE/2;
  bool allow_act=FALSE;

  //========Player attacking timer==============
  if (player.attack_timer>=0) {
    player.attack_timer--;
  }

  if (player.speed>10) {
    player.attack_strength=4;
  } else {
    if (player.speed>5) {
      player.attack_strength=2;
    } else {
      player.attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;
    }
  }

  if (player.speed>24) {
    player.attack_strength*=2;
  }

  //======================


  //Player Bullet Actions;
  if (player.bullet_shot!=-1) {
    player.web_being_shot=player.web_storage[player.placed_web_pos];
    BulletAct(player.bullet_shot);
  }
  for (int i=0;i<player.bullet_shot_num;i++) {
    BulletAct(player.bullet[i]);
  }

  if (player.health<=0) {
    if (player.rst_left && player.x>10)
      move_x(-5);
    else if (player.rst_right && player.x<MAP_WIDTH-10)
      move_x(5);

    if (player.rst_up && player.y>10)
      move_y(-5);
    else if (player.rst_down && player.y<MAP_HEIGHT-10)
      move_y(5);
  }
  if (player.health<=0 && (player.rst_left_click || player.rst_key_1) && player.death_timer>150) {
    int 
        px=player.cam_x,
        py=player.cam_y,
        cx1=player.cam_mouse_move_x,
        cy1=player.cam_mouse_move_y,
        cx2=player.cam_move_x,
        cy2=player.cam_move_y;
    for (int f=0;f<player.bullet_shot_num;f++) {
        int bk=player.bullet[f];
        float pbt=0.2;
        float pbdist=GetDistance(mouse_x,mouse_y,Bullet[bk].sprite_x,Bullet[bk].sprite_y);
        for (int c=0;c<Bullet[bk].speed_multiplier;c++) {
        if (pbdist<250) {
          if (pbdist<150) {
            pbt=0.5;
          }
          if (Bullet[bk].sprite_x<mouse_x) {
            Bullet[bk].x+=pbt;
          } else {
            Bullet[bk].x-=pbt;
          }

          if (Bullet[bk].sprite_y<mouse_y) {
            Bullet[bk].y+=pbt;
          } else {
            Bullet[bk].y-=pbt;
          }
        }
        }
      }
    }


  PlayerActMouseMovement();

  if (player.health>0 && !player.flag_death) {
      //Mouse Actions
      Click(); //check for click
      PlayerActMouseClick(); //action after click


      //Trigger movements
      if (player.type==0) {
        if (player.rst_left || player.rst_right) {
          player.walk_cycle++; //Walk sprite cycle
          if (player.walk_cycle>=4) {
            player.walk_cycle=0;
          }
        } else {
          player.walk_cycle=0;
        }
      } else if (player.type==1) {//bee lol
        player.walk_cycle++;
        if (player.walk_cycle==4) {
          player.walk_cycle=0;
        }
      }
      //Attack mode
      if (player.attack) {
        player.attack=FALSE;
        player.attack_timer=40;
      }

      //Trigger jump
      if (!player.rst_up) { //not jumping
        player.jump=FALSE;
      }

      if (!player.uppercut) { //no more uppercut, revert jump height
        if (player.jump_height==-1)
          player.jump_height=0;
      }

      if (player.type==0 && player.rst_up && (player.on_a_ground || (player.in_water && !player.is_swinging))) { //on ground and jumping
        //player.key_jump_timer=player.player_jump_height;
        if (player.in_water && player.on_ground_id==-1 && player.jump_height==0) {
          player.jump_angle=M_PI_2;
          player.jump_angle2=M_PI_2;
        }
        player.jump=TRUE;
        player.below_ground_edge_timer=0;
        player.on_ground_edge_id=-1;
        player.saved_on_ground_edge_id=-1;
        player.is_on_ground_edge=FALSE;
        player.is_on_left_ground_edge=FALSE;
        player.is_on_right_ground_edge=FALSE;
      }

      if (player.jump && player.jump_height==0) {
        if (!player.in_water) {
          player.jump_height=player.player_jump_height;
        } else {
          player.jump_height=player.player_jump_height/4;
        }
      }

      //PLAYER ACT DECELERATION ACTIONS
      PlayerActDecceleration();

      //PLAYER TRUE SPEED LIMITER
      float speed_limiter=player.speed;
      /*if (player.speed<5 && player.jump) {
        speed_limiter=4;
      }*/

      if (player.is_swinging) {
        speed_limiter=10;
      } else if ((player.fling_distance!=0)) {
        if (player.speed<10) {
          speed_limiter=8;
        } else if (player.speed<15) {
          speed_limiter=10;
        } else if (player.speed<25) {
          speed_limiter=15;
        }
        if (player.fling_distance>0) {
          speed_limiter=speed_limiter+2;
        } else {
          speed_limiter=speed_limiter;//(speed_limiter+speed_limiter/4+1);
        }
      } else if (player.fling_distance==0 && player.bullet_shot==-1 && !player.is_rebounding/*&& (player.jump || player.on_ground_id!=-1 || player.is_on_ground_edge)*/) { //normal walking
        /*if (player.speed<5) {
          speed_limiter=4;
        } else*/ if (player.speed<10) {
          speed_limiter=5;
        } else if (player.speed<24){
          speed_limiter=8;
        }
      }

      if (player.uppercut && player.bullet_shot==-1) { //crouching + uppercut, speed limiter
        //if (player.speed>4) {
          //speed_limiter=5;
        //} else {
          speed_limiter=4;
        //}
      }



      //PLAYER ACT SPPED IN WATER
      if (player.in_water) {
        if (player.grav<2) {
          player.grav++;
        }
        if (player.grav>2) {
          player.grav--;
        }
        speed_limiter=speed_limiter/2+1;
      } else  {
        if (player.in_water_timer>0) {
          if (player.in_water_timer%7/*5*//*20*/==0 && player.grav<=100) {
            player.grav++;
          }
          player.in_water_timer--;
        } 
      }


      if (player.is_on_ground_edge) { //ground edge speed limiter
         speed_limiter*=5;//10;
      }  else if (player.bullet_shot!=-1 && player.speed<10) { //parachuting speed limiter
         speed_limiter=10;
      }

      if (IsSpeedBreaking()) { //speedbreaking speed limiter
        if (player.on_ground_id!=-1) {
          speed_limiter=player.speed;
        }
        if (player.is_on_ground_edge) { //ground edge speed limiter
           speed_limiter*=5;//10;
        } else if (!player.is_on_ground_edge && player.on_ground_id!=-1) {
          if (game_hard) {
            speed_limiter*=2;
          } else {
            speed_limiter=speed_limiter+speed_limiter/2+1;
          }
        }
      }



      if (player.below_ground_edge_timer>0) {
        player.below_ground_edge_timer--;
      }


      for (speed=0;speed<speed_limiter;speed++) {
        for (grav_speed=0;grav_speed<player.grav;grav_speed++) {

          //limit border and spritex
          PlayerCameraLimiterBorder();


          player.on_ground_id=GetOnGroundIdPlayer(player.x,player.y,5,4);
          //player.on_ground_id_u1=GetOnGroundId(player.above_x1,player.above_y1,5,4); //left up
          //player.on_ground_id_d1=GetOnGroundIdPlayer(player.below_x1,player.below_y1,5,4); //left down
          player.on_ground_id_u2=GetOnGroundId(player.above_x2,player.above_y2,5,4); //right up
          //player.on_ground_id_d2=GetOnGroundIdPlayer(player.below_x2,player.below_y2,5,4); //right down

       //hiding?    (legacy feature)
          /*int tmp_node_grid=GetGridId(player.above_x,player.above_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
          if (tmp_node_grid!=-1) {
            if (NodeGrid[tmp_node_grid]->node_solid) {
              player.hiding=TRUE;
            } else {
              player.hiding=FALSE;          
            }
          }*/


          //PLAYER IN NODE GRID ACTIONS
          int in_node_grid_id=GetGridId(player.x,player.y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
          if (in_node_grid_id!=-1) {
            if (NodeGrid[in_node_grid_id]->node_fire && (player.block_timer>23 || player.block_timer==0)) {
              player.health-=1;
            }
            if (NodeGrid[in_node_grid_id]->node_water) {
              if (!player.in_water) {//entering water
                player.in_air_timer=1002;
                player.in_water_timer=150;
              }
              if (player.in_water_timer<200) {
                player.in_water_timer++;
                if (player.in_water_timer==198  && player.fling_distance!=0) {
                  player.fling_distance=0;
                }
              }
              player.in_water=TRUE;
            } else {
              if (player.in_water) { //leaving water
                player.jump_angle=
                player.jump_angle2=M_PI_2;
                player.in_air_timer=20;
                if (player.speed>5 && !player.time_breaker) {
                  player.speed/=2;
                }
              }
              player.in_water=FALSE;
            }
          } else {
            if (player.in_water) {
                player.jump_angle=
                player.jump_angle2=M_PI_2;
                player.in_air_timer=20;
                if (player.speed>5 && !player.time_breaker) {
                  player.speed/=2;
                }
            }
            player.in_water=FALSE;
          }



          //PLAYER CORNER U2 ACTION
          if (player.on_ground_id_u2==-1) {
            player.in_web_corner=FALSE;
            player.key_b4_corner_is_left=FALSE;
            player.key_b4_corner_is_right=FALSE;
          }


        //timebreaker slowdown disabled on hardmode
         if (game_hard && player.time_breaker && IsSpeedBreaking()) {
            player.sleep_timer = DEFAULT_SLEEP_TIMER;
          }


       //Destroy Ground (regainable)
          PlayerActDestroyGround();

       //Ground action
       //on a ground
          if (player.on_ground_id!=-1 && player.on_ground_id!=player.previous_web_placed) {

            //get dist between player and ground
            float ground_entity_E=GetLineTargetAngle(player.on_ground_id,player.x,player.y);
            float height_from_player_x=GetLineTargetHeight(player.on_ground_id,ground_entity_E,player.x,player.y);


            //player speed when on ground
            if (player.block_timer>0) {
              player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT*2;
            } else {
              player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
            }


            //player ground interaction
            if  ((Ground[player.on_ground_id]->x1-10<player.x &&  player.x<Ground[player.on_ground_id]->x2+10) &&
                  ((Ground[player.on_ground_id]->y1-10<player.y && player.y<Ground[player.on_ground_id]->y2+10) ||
                   (Ground[player.on_ground_id]->y2-10<player.y && player.y<Ground[player.on_ground_id]->y1+10)))
            {
             //Within ground axes
              PlayerOnGroundAction(speed,grav_speed,height_from_player_x);
            } else { //not in range based on ground's x and y boundaries
              player.on_a_ground=FALSE;
              player.on_ground_timer=0;

              player.in_web_corner=FALSE;
              player.key_b4_corner_is_left=FALSE;
              player.key_b4_corner_is_right=FALSE;
            }
          }

        //Y movement
        //Condition to jump
          if (player.on_ground_timer>0) {
            player.previous_web_placed=-1;
          }




          if (player.y<6) { //Y axis cap
            move_y(1);
            player.in_air_timer++;
          } else if (player.y+PLAYER_HEIGHT/2>MAP_HEIGHT-6) {
            move_y(-player.player_grav);
            player.health--;
          }
       //===========================



         //x-axis cap
          if (player.x-PLAYER_WIDTH/2<6) {
            move_x(1);
          } else if (player.x+PLAYER_WIDTH/2>MAP_WIDTH-6) {
            move_x(-1);
          }



            //parachuting spider action
            if (player.bullet_shot!=-1 && !player.in_water) {
              float pdist_from_bullet=GetDistance(player.x,player.y,Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y);
              float pangle=GetCosAngle(player.x-Bullet[player.bullet_shot].x,player.pivot_length);
              if (pdist_from_bullet>DEFAULT_PLAYER_BUILD_RANGE*3 /*&& player.on_ground_id==-1*/) {
                if (player.speed<6) {
                  player.speed=6;
                }
                if (player.speed<24) {
                  Bullet[player.bullet_shot].speed_multiplier=10;
                }
                player.fling_distance=0;
                player.grav=2;
                player.in_air_timer=2;
                player.is_rebounding=FALSE;
                player.on_ground_edge_id=-1;
                player.is_on_ground_edge=FALSE;
                player.jump=FALSE;
                player.jump_height=0;
                player.is_swinging=FALSE;

                //>>>Calculate the Angle of incidence of player
                if (player.y>Bullet[player.bullet_shot].y) { //lower quad
                  if(player.x<Bullet[player.bullet_shot].x) {
                    player.angle_of_incidence=pangle+M_PI_2;
                  } else {
                    player.angle_of_incidence=pangle-M_PI_2;
                  }
                } else {//upper quad
                  if(player.x<Bullet[player.bullet_shot].x) {
                    player.angle_of_incidence=2*M_PI-pangle-M_PI_2;
                  } else {
                    player.angle_of_incidence=2*M_PI-pangle+M_PI_2;
                  }
                }
                player.angle_of_reflection=player.angle_of_incidence;


                if (grav_speed==1) {
                  if (player.y>Bullet[player.bullet_shot].y) {
                    move_x(-cos(-pangle));
                    move_y(sin(-pangle));
                  } else {
                    move_x(-cos(-pangle));
                    move_y(-sin(-pangle));
                  }
                  if (player.rst_left) {
                    player.last_left=TRUE;
                  } else if (player.rst_right) {
                    player.last_left=FALSE;
                  }
                }
              }
            } else {
          //==========PLAYER ACT REBOUND ACTIONS==========
          PlayerActReboundActions(grav_speed,speed);


          //====PLAYER GROUND EDGE MOVEMENT==== //player edge interaction, mini web rotation
          if (grav_speed==1) {
            PlayerActGroundEdgeMovement();
          }

          //==========Gravity=========
          PlayerActGravityMovement(grav_speed,speed);
          //limit border and spritex
          PlayerCameraLimiterBorder();

          //X movement
          PlayerActXMovement(grav_speed);

          //====PLAYER CIRCULAR WEB SWINGING MOVEMENT======
          if (player.is_swinging) {
            player.below_ground_edge_timer=0;
            PlayerActSwinging(grav_speed);
            //set axes of player fling web
            if (grav_speed==3 && speed==3) {//only occurs right after grav_speed==0
              PlayerActFlingWeb(speed);
            }
          } //End of player swinging movement



          //======PLAYER ACT FLING MOVEMENT======
          PlayerActFlingMovement(grav_speed);

        }

         //misc
          player.print_current_above=player.current_above;
          player.print_current_below=player.current_below;

          if (player.in_air_timer>0) {
            player.current_above=FALSE;
            player.current_below=FALSE;
          }

          player.saved_ground_id=player.on_ground_id;

           //limit border and spritex
          PlayerCameraLimiterBorder();


       //Set Character's Axis
          float clawlx;
          float clawly;
          if (player.current_above) {
            player.above_x=player.x+(claws_l)*cos(player.angle-M_PI/2);
            player.above_y=player.y+(claws_l)*sin(player.angle-M_PI/2);


            if (player.last_left) {
              player.claws_x=player.x-(claws_l)*cos(player.angle);
              player.claws_y=player.y-(claws_l)*sin(player.angle);
              player.claws_attack_x=player.x-(claws_l2)*cos(player.angle);
              player.claws_attack_y=player.y-(claws_l2)*sin(player.angle);
              clawlx=player.x+(claws_l)*cos(player.angle);
              clawly=player.y+(claws_l)*sin(player.angle);
            } else {
              player.claws_x=player.x+(claws_l)*cos(player.angle);
              player.claws_y=player.y+(claws_l)*sin(player.angle);
              player.claws_attack_x=player.x+(claws_l2)*cos(player.angle);
              player.claws_attack_y=player.y+(claws_l2)*sin(player.angle);
              clawlx=player.x-(claws_l)*cos(player.angle);
              clawly=player.y-(claws_l)*sin(player.angle);
            }
            player.below_x1=clawlx-(claws_l)*cos(player.angle-M_PI/2); 
            player.below_y1=clawly-(claws_l)*sin(player.angle-M_PI/2);

            player.above_x1=clawlx-(claws_l)*cos(player.angle+M_PI/2);
            player.above_y1=clawly-(claws_l)*sin(player.angle+M_PI/2);

            player.below_x2=player.claws_x+(claws_l)*cos(player.angle+M_PI/2); //top right
            player.below_y2=player.claws_y+(claws_l)*sin(player.angle+M_PI/2);


            player.above_x2=player.claws_x+(claws_l)*cos(player.angle-M_PI/2); //top right
            player.above_y2=player.claws_y+(claws_l)*sin(player.angle-M_PI/2);


            player.above_head_x=player.x+cos(player.angle-M_PI/2)*NODE_SIZE*2;
            player.above_head_y=player.y+sin(player.angle-M_PI/2)*NODE_SIZE*2;

          } else if (player.current_below) {        
            player.above_x=player.x+(claws_l)*cos(player.angle+M_PI/2);
            player.above_y=player.y+(claws_l)*sin(player.angle+M_PI/2);
            if (player.last_left) {
              player.claws_x=player.x+(claws_l)*cos(player.angle);
              player.claws_y=player.y+(claws_l)*sin(player.angle);
              player.claws_attack_x=player.x+(claws_l2)*cos(player.angle);
              player.claws_attack_y=player.y+(claws_l2)*sin(player.angle);
              clawlx=player.x-(claws_l)*cos(player.angle);
              clawly=player.y-(claws_l)*sin(player.angle);
            } else {
              player.claws_x=player.x-(claws_l)*cos(player.angle);
              player.claws_y=player.y-(claws_l)*sin(player.angle);
              player.claws_attack_x=player.x-(claws_l2)*cos(player.angle);
              player.claws_attack_y=player.y-(claws_l2)*sin(player.angle);
              clawlx=player.x+(claws_l)*cos(player.angle);
              clawly=player.y+(claws_l)*sin(player.angle);
            }

            player.below_x1=clawlx-(claws_l)*cos(player.angle+M_PI/2);
            player.below_y1=clawly-(claws_l)*sin(player.angle+M_PI/2);

            player.above_x1=clawlx-(claws_l)*cos(player.angle-M_PI/2);
            player.above_y1=clawly-(claws_l)*sin(player.angle-M_PI/2);

            player.below_x2=player.claws_x+(claws_l)*cos(player.angle-M_PI/2);
            player.below_y2=player.claws_y+(claws_l)*sin(player.angle-M_PI/2);

            player.above_x2=player.claws_x+(claws_l)*cos(player.angle+M_PI/2);
            player.above_y2=player.claws_y+(claws_l)*sin(player.angle+M_PI/2);

            player.above_head_x=player.x+cos(player.angle+M_PI/2)*NODE_SIZE*2;
            player.above_head_y=player.y+sin(player.angle+M_PI/2)*NODE_SIZE*2;

          } else {
            player.above_x=player.x;
            player.above_y=player.y-(claws_l);

            if (player.last_left) {
              player.claws_x=player.x-(claws_l);
              player.claws_y=player.y;
              player.claws_attack_x=player.x-(claws_l2);
              player.claws_attack_y=player.y;
              clawlx=player.x+(claws_l);
              clawly=player.y;
            } else {
              player.claws_x=player.x+(claws_l);
              player.claws_y=player.y;
              player.claws_attack_x=player.x+(claws_l2);
              player.claws_attack_y=player.y;
              clawlx=player.x-(claws_l);
              clawly=player.y;
            }

            player.below_x1=clawlx; //down left
            player.below_y1=clawly+(claws_l);

            player.below_x2=player.claws_x; //down right
            player.below_y2=player.claws_y+(claws_l);

            player.above_x1=clawlx; //up left
            player.above_y1=clawly-(claws_l);

            player.above_x2=player.claws_x; //up right
            player.above_y2=player.claws_y-(claws_l);

            if (player.is_on_ground_edge) {
              if (player.above_ground_edge) {
                player.above_head_x=player.x+cos(player.edge_angle-M_PI/2)*NODE_SIZE*2;
                player.above_head_y=player.y+sin(player.edge_angle-M_PI/2)*NODE_SIZE*2;
              } else if (player.below_ground_edge) {
                player.above_head_x=player.x+cos(player.edge_angle+M_PI/2)*NODE_SIZE*2;
                player.above_head_y=player.y+sin(player.edge_angle+M_PI/2)*NODE_SIZE*2;
              }
            } else {
              player.above_head_x=player.x;//;+sin(player.angle+M_PI/2)*NODE_SIZE*2;
              player.above_head_y=player.y;//+cos(player.angle+M_PI/2)*NODE_SIZE*2;
            }
          }
        }

      }



      
      //Calculating new angle of incidence
      if (!player.is_swinging && player.bullet_shot==-1) {
        if (player.jump_height>0) { //jumping and not swinging
          if (player.rst_left || player.rst_right) { // jump + holding left/right
            float t_speed1=player.speed*cos(player.jump_angle2);
            float t_speed2=player.speed;
            float t_speed=t_speed1+t_speed2;
            float t_grav=player.speed*sin(player.jump_angle2);
            float grav_dist=GetDistance(t_speed,0,0,t_grav);
            player.angle_of_incidence=GetCosAngle(t_speed,grav_dist);
            if (player.last_left) {
              player.angle_of_incidence=M_PI+player.angle_of_incidence-M_PI_2;
            }
            if (player.previous_above) {
              player.angle_of_incidence=2*M_PI-player.angle_of_incidence;
            }
          } else { //jump soley
            player.angle_of_incidence=player.jump_angle2;
          }
        } else { //end of jump height, not jumping
          if (!player.on_a_ground) { //in air and holding left or right key
            if (player.rst_left || player.rst_right) {
              float grav_dist=GetDistance(player.speed,0,0,player.grav*player.player_grav);
              player.angle_of_incidence=GetCosAngle(player.speed,grav_dist);
              if (player.last_left) {
                player.angle_of_incidence=M_PI-player.angle_of_incidence;
              }
            } else { //not holding left or right but still in air
              if ((player.is_rebounding || player.fling_distance!=0) && player.bullet_shot==-1) {//flinging or rebounding
                float t_speed=player.speed*cos(player.angle_of_reflection); //rate of change in x . player travel to refleciton angle
                float t_grav1=player.speed*sin(player.angle_of_reflection); //rate of change in y . player travel to reflection angle
                float t_grav2=player.grav*player.player_grav; //rate of change in y . Gravity
                float t_grav=t_grav1+t_grav2; //dy/dx
                float grav_dist=GetDistance(t_speed,0,0,t_grav);
                if (t_grav<0) { //still going upwards
                  player.angle_of_incidence=2*M_PI-GetCosAngle(t_speed,grav_dist);
                } else { //going downwards
                  player.angle_of_incidence=GetCosAngle(t_speed,grav_dist);
                }
              } else { //falling down normally
                player.angle_of_incidence=M_PI_2;
              }
            }
          }
        }
      }



      //angle of incidence is only bettween 0 to 2*M_PI
      if (player.angle_of_incidence>2*M_PI) {
        player.angle_of_incidence-=2*M_PI;
      }
      if (player.angle_of_incidence<0) {
        player.angle_of_incidence+=2*M_PI;
      }






     //misc
      if (player.on_ground_timer>0) {
        if (!player.on_a_ground) {
          player.on_ground_timer--;
        }
      } else {
        player.on_ground_id=-1;
        player.saved_ground_id=-1;
      }


    //block
      allow_act=FALSE;
      //if (player.attack_timer<=0) { // not attacking
        if (player.rst_down && player.type==0) { //pressing down button
          allow_act=TRUE;
        }
      //}
      if (allow_act) {
        player.blocking=TRUE;
        if (player.block_timer<5)
          player.attack=TRUE;
        //player.attack_timer=40;
      } else {
        player.blocking=FALSE;
      }
      if (player.blocking) {
        player.block_cooldown=player.block_cooldown_max;
        player.block_recharge_timer=player.block_recharge_timer_max;
        if (player.block_timer<30) {
          player.block_timer++;
        }
        if (player.block_health<=0) {
          player.block_health=0;
        }
      } else {//player not blocking -regen block/dont regen when speedbreaking
        if (player.block_timer>0) {
          player.block_timer--;
        }
        if (!IsSpeedBreaking()) {
          if (player.block_cooldown>0) {
            player.block_cooldown--;
          } else {//3 seconds has passed
            if (player.block_recharge_timer>0) {
	          player.block_recharge_timer--;
            } else if (player.block_health<player.block_health_max) {//below max
              player.block_health++;
              if (player.block_health>player.block_health_max) {
                player.block_health=player.block_health_max;
              }
	          player.block_recharge_timer=player.block_recharge_timer_max;
            }
          }
        }
      }


      //Time breaker
      if (!player.time_breaker) {
        if (IsSpeedBreaking()) {
          if (player.time_breaker_units>0) {
            if (!game_hard) {
              player.time_breaker_units--;
              if (player.time_breaker_units%3==0) {
                if (player.type==0) {
                  player.speed++;
                }
                player.decceleration_timer=0;
              }
            } else {
              if (player.time_breaker_deplete_cooldown>0) {
                player.time_breaker_deplete_cooldown--;
              } else {
                player.time_breaker_units--;
                if (player.time_breaker_units%3==0) {
                  if (player.type==0) {
                    player.speed++;
                  }
                  player.decceleration_timer=0;
                }
                player.time_breaker_deplete_cooldown=25;
              }
            }
          }
          if (game_hard) {
            if (player.time_breaker_units<=0) {
              player.sleep_timer=DEFAULT_SLEEP_TIMER;
            }
          }
          player.time_breaker_recharge_timer=player.time_breaker_recharge_timer_max;
          if (player.speed<10) {
            player.time_breaker_cooldown=player.time_breaker_cooldown_max;
          } else {
            if (player.speed<=24) {
              player.time_breaker_cooldown=player.time_breaker_cooldown_max/4;
            } else {
              player.time_breaker_cooldown=player.time_breaker_cooldown_max/8;
            }
          }
        }
        if (player.time_breaker_cooldown>0) {
          player.time_breaker_cooldown--;
        } else {
          if (player.time_breaker_recharge_timer>0) {
            player.time_breaker_recharge_timer--;
          } else if (player.time_breaker_units<player.time_breaker_units_max && !player.is_swinging) {
            player.time_breaker_units++;
            if (player.speed<10) {
              player.time_breaker_recharge_timer=player.time_breaker_recharge_timer_max;
            } else {
              if (player.speed<=24) {
                player.time_breaker_recharge_timer=player.time_breaker_recharge_timer_max/4;
              } else {
                player.time_breaker_recharge_timer=player.time_breaker_recharge_timer_max/8;
              }
            }
          }
        }
      } else {
        if (player.time_breaker_units>0) {
          if (player.time_breaker_units_tick>0) {
            player.time_breaker_units_tick--;
          } else {
            player.time_breaker_units--;
            player.time_breaker_units_tick=player.time_breaker_units_tick_max;
            if (player.time_breaker_units==0) {
              player.time_breaker=FALSE;
              player.flag_revert_palette=TRUE;
              if (game_audio) {
                PlaySound(spamSoundEffectCache[1].audio, NULL, SND_MEMORY | SND_ASYNC);
              }
            }
          }
        }
      }

      //swinging
      if ((player.fling_distance>0 || player.is_swinging) && player.speed<6 && player.in_water_timer==0) { //fast when swinging
        player.speed=6;
      }

      //Spinning
      if (player.rst_down && player.on_ground_id==-1) { //not on ground and in air
        player.spin_timer++;
        if (player.speed>10) {
          player.spin_timer++;
        }
      } else {
        if (player.spin_timer>0) {
          player.spin_timer++;
          if (player.speed>10) {
            player.spin_timer++;
          }
        }
      }
      if (player.spin_timer>40) {
        player.spin_timer=0;
      }


      //show hp timer
      if (player.show_health_timer>0)
        player.show_health_timer--;
      if (player.show_block_health_timer>0)
        player.show_block_health_timer--;
      if (player.show_exp_timer>0)
        player.show_exp_timer--;


      //
      player.blur_timer[player.current_blur_sprite]++;
      if (player.blur_timer[player.current_blur_sprite]>1) {
        player.blur_x[player.current_blur_sprite]=player.x;
        player.blur_y[player.current_blur_sprite]=player.y;
        player.blur_timer[player.current_blur_sprite]=0;
        player.current_blur_sprite++;
        if (player.current_blur_sprite>=PLAYER_BLUR_NUM) {
          player.current_blur_sprite=0;
        } 
      }

      for (int i=0;i<PLAYER_BLUR_NUM;i++) {
        player.blur_sprite_x[i]=player.blur_x[i]+player.cam_x+player.cam_move_x+player.cam_mouse_move_x+player.cam_limiter_x;
        player.blur_sprite_y[i]=player.blur_y[i]+player.cam_y+player.cam_move_y+player.cam_mouse_move_y+player.cam_limiter_y;
      }

  } else { //player.health<=0
    if (player.death_timer<200) {
      player.death_timer++;
    }
    //immediate death action
      if (!player.flag_death) {
        if (game_audio) {
          PlayMemSnd(&channelSoundEffect[0],&channelSoundEffectCache[0],TRUE,1); 
        }

        player.flag_death=TRUE;
        player.flag_revert_palette=TRUE;
        player.time_breaker_tick=0;
        player.time_breaker=FALSE;
        player.time_breaker_units=0;
        player.is_swinging=FALSE; //stop swinging
        InitPlayerFlingWeb();
        //player.death_x=player.x;
        //player.death_y=player.y;
        if (player.bullet_shot!=-1) {
          StopBullet(player.bullet_shot,TRUE); //stop all web bullets from acting
          player.web_being_shot=-1;
          player.bullet_shot=-1;
        }

        for (int j=0;j<player.bullet_shot_num;j++) { //reset bullets
          StopBullet(player.bullet[j],FALSE);
        }
        player.bullet_shot_num=0;

        for (int n=0;n</*PLAYER_BULLET_NUM*/8;n++) {
          int seed=player.seed*n;
          if (!free_will) seed=-1;
          int rand_range=NODE_SIZE*50+NODE_SIZE*RandNum(1,5,&player.death_bullet_rng_i,seed);
	        player.bullet[player.bullet_shot_num]=current_bullet_id;

            ShootBullet(current_bullet_id,
	            player.bullet_shot_num,
	            rgbPaint[player_load_color],
                -9, //graphics type
                rand_range, // range
                0.1, //speed
	            12+RandNum(1,10,&player.death_bullet_rng_i,seed), //speed multiplier
	            10, //damage
	            -2,
	            player.x, //so it doest get stuck to ground
	            player.y,
	            player.x,
	            player.y,
	            player.x+RandNum(-50,50,&player.death_bullet_rng_i,seed),
	            player.y+RandNum(-30,30,&player.death_bullet_rng_i,seed),
                0         
            );
            player.bullet_shot_num++;
            current_bullet_id++;
            if (current_bullet_id>=SHOOT_BULLET_NUM-1) {
              current_bullet_id=0;
            }        
          }
      }
  }
}


void PlayerSndAct()
{
  if (player.hurt_snd_timer>0) {
    player.hurt_snd_timer--;
  }

  if (player.rst_right_click_snd) {
    if (game_audio) {
      PlayMemSnd(&channelSoundEffect[6],&channelSoundEffectCache[6],TRUE,3);
    }
    player.rst_right_click_snd=FALSE;
  } else {
    if (player.shoot_knife_duration==1) { //sound effect knife throw
      switch (player.play_gun_snd) {
        case 0: //knife throw
          PlayMemSnd(&channelSoundEffect[2],&channelSoundEffectCache[2],TRUE,3);
          break;
        case 1: //pistol
          PlayMemSnd(&channelSoundEffect[8],&channelSoundEffectCache[8],TRUE,3);          
          break;
        case 2: //shotgun/
          PlayMemSnd(&channelSoundEffect[5],&channelSoundEffectCache[5],TRUE,3);
          break;
      }
    }
  }
  //sound timers limiter
  //player shooting knife sound limiter
  if (player.shoot_knife_duration>0)
    player.shoot_knife_duration+=10;//6;
  if (player.shoot_knife_duration>330/*341*/) {//special case, old audio has popping noise at the end
    player.shoot_knife_duration=0;
  }


}

void PlayerCameraShake()
{
  int i;
//camshake
  //if (!the_bravery_tyrant && IsNormalView) {
    float y_bob=0,x_bob=0;
    //if (sprint_bobbing) {  //if sprint_bobbing
    if (!player.is_swinging) {
      if (player.on_ground_id!=-1 && player.cam_limiter_x==0 && player.cam_limiter_y==0) {//not in air, on ground
        if (!player.blocking) {
          if (player.rst_left || player.rst_right || (player.type==1 && (player.rst_up || player.rst_down))) {
	        if (player.speed>=5 || player.type==1) {
              x_bob=2.5;
              player.cam_move_x+=0.75*RandNum(-1,1,&misc_rng_i,player.seed);//shaky cam
              player.cam_move_y+=0.75*RandNum(-1,1,&misc_rng_i,player.seed);
	        } else {
              x_bob=1.5;
	        }
            if (player.print_current_below) {//upside down
	          x_bob=-abs(x_bob);
      	    }
      	    if (player.rst_left) {
              player.cam_move_x+=x_bob*cos(player.angle);
              player.cam_move_y+=x_bob*sin(player.angle);
      	    }
      	    if (player.rst_right) {
              player.cam_move_x-=x_bob*cos(player.angle);
              player.cam_move_y-=x_bob*sin(player.angle);
      	    }
	      }
        }
      }
    //}
    
    //if ((player.grav>3 || player.speed>=5) && (!player.is_on_ground_edge)) { //falling cam effect
    if (!player.is_on_ground_edge) {
      y_bob=(player.grav)/6;

      switch (player.speed) {
         case 1:
    	 case 2:
           x_bob=RandNum(1,2,&misc_rng_i,player.seed)*0.5;//move x	
    	   break;
    	 case 3:
           x_bob=RandNum(1,2,&misc_rng_i,player.seed);//move x		
    	   break;
      }

      if (player.grav>5 && player.cam_limiter_y==0) {
        if (player.rst_left) {
	      player.cam_move_x+=x_bob;
        }
        if (player.rst_right) {
	      player.cam_move_x-=x_bob;
        }
        player.cam_move_y-=y_bob;//y_bob;//increase y
      }

      if ((player.grav>5  && player.cam_limiter_y==0) ||
            ((player.cam_limiter_x==0 && player.cam_limiter_y==0) && 
              (/*player.grav>5 ||*/ (player.speed>=5 && (player.rst_left || player.rst_right)))
            )
          || player.type==1) {
        //player.cam_move_x+=RandNum(-1,1,&misc_rng_i,player.seed);//shaky x
        player.cam_move_y+=RandNum(-1,1,&misc_rng_i,player.seed);//shaky y
      }
    //}
    x_bob=0;
    y_bob=0;
  }

  }

  for (i=0;i<abs(player.cam_move_x);i++) { //cam stablizer
    if (player.cam_move_x>0) {
      player.cam_move_x-=0.1;
    } else if (player.cam_move_x<0) {
      player.cam_move_x+=0.1;
    }
  }
  for (i=0;i<abs(player.cam_move_y);i++) {
    if (player.cam_move_y>0) {
      player.cam_move_y-=0.1;
    }  else if(player.cam_move_y<0) {
      player.cam_move_y+=0.1;
    }
  }
  if (-0.1<=player.cam_move_x && player.cam_move_x<=0.1) { //prevent spam shake
    player.cam_move_x=0;
  }
  if (-0.1<=player.cam_move_y && player.cam_move_y<=0.1) {
    player.cam_move_y=0;
  }
}



void InitPlayerSpritesObjColor(HDC hdc,HDC hdc2)
{
  for (int i=0;i<PLAYER_ROTATED_SPRITE_NUM;i++) {
    BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_1[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_2[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].attack_sprite_1[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].attack_sprite_2[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].attack_sprite_3[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].attack_sprite_4[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].block_sprite_1[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].block_sprite_2[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].block_sprite_3[i].sprite_paint,PlayerSprite[0].PlayerPalette);

    //dithered sprite
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_sprite_1[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_sprite_2[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_attack_sprite_1[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_attack_sprite_2[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_attack_sprite_3[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_attack_sprite_4[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_block_sprite_1[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_block_sprite_2[i].sprite_paint,PlayerSprite[0].PlayerPalette);
    BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_block_sprite_3[i].sprite_paint,PlayerSprite[0].PlayerPalette);
  }
/*
  BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_bee_1.sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_bee_2.sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_bee_aero_1.sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_bee_aero_2.sprite_paint,PlayerSprite[0].PlayerPalette);
*/

  BitmapPalette(hdc,hdc2,PlayerSprite[0].sprite_jump.sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].blur_sprite_jump.sprite_paint,PlayerSprite[0].PlayerBlurPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].spin_sprite[0].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].spin_sprite[1].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].spin_sprite[2].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].spin_sprite[3].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].blur_spin_sprite[0].sprite_paint,PlayerSprite[0].PlayerBlurPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].blur_spin_sprite[1].sprite_paint,PlayerSprite[0].PlayerBlurPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].blur_spin_sprite[2].sprite_paint,PlayerSprite[0].PlayerBlurPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].blur_spin_sprite[3].sprite_paint,PlayerSprite[0].PlayerBlurPalette);

  //dithered sprite
  BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_sprite_jump.sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_spin_sprite[0].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_spin_sprite[1].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_spin_sprite[2].sprite_paint,PlayerSprite[0].PlayerPalette);
  BitmapPalette(hdc,hdc2,PlayerSprite[0].dithered_spin_sprite[3].sprite_paint,PlayerSprite[0].PlayerPalette);

}


void InitPlayerCursorColor(HDC hdc,HDC hdc2) {
  for (int i=0;i<16;i++) {
    BitmapPalette(hdc,hdc2,draw_player_cursor[i].sprite_paint,player_cursor_palette);
  }
  /*for (int i=0;i<5;i++) {
    BitmapPalette(hdc,hdc2,draw_player_cursorbee[i].sprite_paint,player_cursor_palette);
  }*/
}


void DrawPlayer(HDC hdc,HDC hdc2,int ptype)
{
  //sprite axes
  player.sprite_x=GR_WIDTH/2+(int)player.cam_move_x+(int)player.cam_mouse_move_x+(int)player.cam_limiter_x;
  player.sprite_y=GR_HEIGHT/2+(int)player.cam_move_y+(int)player.cam_mouse_move_y+(int)player.cam_limiter_y;

  if (player.dmg_taken_timer>0) {
    if (game_cam_shake) {
      player.cam_move_x=RandNum(-5,5,&misc_rng_i,-1);
      player.cam_move_y=RandNum(-5,5,&misc_rng_i,-1);
    }
    player.dmg_taken_timer--;
  }

  if (free_will)
    player.seed=rand();
  else
    player.seed=-1;

  //Keys to controll cursor
  if (player.rst_arrow_left) {
    if (mouse_x>20)
      mouse_x-=20;
  } else if (player.rst_arrow_right) {
    if (mouse_x<GR_WIDTH-20)
      mouse_x+=20;
  }


  if (player.rst_arrow_up) {
    if (mouse_y>20)
      mouse_y-=20;
  } else if (player.rst_arrow_down) {
    if (mouse_y<GR_HEIGHT-20)
      mouse_y+=20;
  }




  //revert palette back to normal
  if (player.flag_revert_palette && player.time_breaker_tick<=0) {    
    InitEnemySpritesObjColor(hdc,hdc2);
    InitPlayerSpritesObjColor(hdc,hdc2);
    InitPlayerCursorColor(hdc,hdc2);

    /*for (int i=0;i<9;i++) {
      BitmapPalette(hdc,hdc2,texture_water[i],waterPalette);
    }*/
    for (int i=0;i<PLATFORM_GRID_NUM;i++) {
      if (map_background!=1) {
        if (map_weather==0)
          BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDefault);
        else
          BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsDarker2);
      } else {
        BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsNight);
      }
    }
    /*for (int i=0;i<FOREGROUND_GRID_NUM;i++) {
      BitmapPalette(hdc,hdc2,TileMapForeground[i]->sprite_paint,rgbColorsDefault);
    }*/

    player.flag_revert_palette=FALSE;
    player.time_breaker_tick=0;
  }



  //turn palette to noir when timebreaking
  if (player.time_breaker) {
    if (player.time_breaker_tick<GR_WIDTH || player.time_breaker_tick<GR_HEIGHT) {
      player.time_breaker_tick+=1;
      player.time_breaker_tick+=player.time_breaker_tick;
      GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick,WHITE,-1);
      GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick-1,WHITE,-1);
    } else if (player.flag_noir_palette) {
      InitEnemySpritesObjColorNoir(hdc,hdc2);
      for (int i=0;i<PLATFORM_GRID_NUM;i++) {
        BitmapPalette(hdc,hdc2,TileMapPlatform[i]->sprite_paint,rgbColorsNoir);
      }
      /*for (int i=0;i<FOREGROUND_GRID_NUM;i++) {
        BitmapPalette(hdc,hdc2,TileMapForeground[i]->sprite_paint,rgbColorsNoir);
      }*/

      player.flag_noir_palette=FALSE;
    }
  } else {
    if (player.time_breaker_tick>0) { //draw speed breaker around player
      player.time_breaker_tick--;
      player.time_breaker_tick-=player.time_breaker_tick/2;
      GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick,WHITE,-1);
      GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick-1,WHITE,-1);
    }
  }






  if (ptype==0) {

  if (player.health>0) {


  //Draw player is blocking
  if (player.block_timer>0 /*&& player.block_health>0*/) {
    float ang=2*M_PI*((float)frame_tick/FPS)*10;//player.block_timer;
    if (player.last_left) {
      ang=-abs(ang);
    }
    float tx=player.sprite_x+cos(ang)*player.block_timer;
    float ty=player.sprite_y+sin(ang)*player.block_timer;
    if (player.block_timer>23) {
      tx=player.sprite_x+cos(ang)*(23-(player.block_timer-23)*2);
      ty=player.sprite_y+sin(ang)*(23-(player.block_timer-23)*2);
    }
    if (player.block_timer<30) {
      if (player.block_timer<=23) {
        int maxh=1;
        if (player.speed>10) {
          if (is_khmer)
            maxh=4;
          else
            maxh=2;
        }
        for (int h=0;h<maxh;h++) {
          GrLine(hdc,tx,ty,player.sprite_x,player.sprite_y,LTCYAN);
          if (is_khmer) {
            int lc=CYAN;
            if (player.last_left) {
              GrLine(hdc,tx,ty,tx-cos(ang-M_PI_2)*10,ty-sin(ang-M_PI_2)*10,lc);
            } else {
              GrLine(hdc,tx,ty,tx+cos(ang-M_PI_2)*10,ty+sin(ang-M_PI_2)*10,lc);
            }
          }
          ang+=M_PI_2;
          GrCircle(hdc,tx,ty,3,LTCYAN,LTCYAN);
          tx=player.sprite_x+cos(ang)*player.block_timer;
          ty=player.sprite_y+sin(ang)*player.block_timer;
        }

      } else {
        GrLine(hdc,tx,ty,player.sprite_x,player.sprite_y,LTGRAY);
        GrCircle(hdc,tx,ty,3,LTGRAY,-1);
      }
    }
  }

  //Player on ground standing angle maths
  //Mathematics
  //sprite angle
  //ON GROUND ACTUAL
  if (player.on_ground_id!=-1) {
    int le_angle=16;
    float le_player_angle=player.angle;
    if (player.angle>0) { //Slope ++ \/
        if (player.print_current_above) {
          if (!player.last_left) {
            le_angle=16+le_player_angle/(M_PI_32);
          } else {
            le_angle=16-le_player_angle/(M_PI_32);
          }
        } else if (player.print_current_below) {
          if (!player.last_left) {
            le_angle=48+(le_player_angle)/(M_PI_32);
          } else {
            le_angle=48-(le_player_angle)/(M_PI_32);
          }
       }
    } else { //Slope -- /
        if (player.print_current_above) {
          if (!player.last_left) {
            le_angle=16+le_player_angle/(M_PI_32);
          } else {
            le_angle=(M_PI_2-le_player_angle)/(M_PI_32);
          }
        } else if (player.print_current_below) {
          if (!player.last_left) {
            le_angle=32+(M_PI_2+le_player_angle)/(M_PI_32);
          } else {
            le_angle=64-(M_PI_2+le_player_angle)/(M_PI_32);
          }
        }
    }
    if (le_angle>63) {
      le_angle-=64;
    } 
    if (le_angle<0) {
      le_angle+=64;
    }
    if (le_angle>-1 && le_angle<64) {
      player.sprite_angle=le_angle;
    }
  } else {
    player.sprite_angle=16;
  }


  //Draw playe line when on ground edge
  if (player.is_on_ground_edge) {
    int tmp_ground_id=player.on_ground_edge_id;
    if (tmp_ground_id==-1)
      tmp_ground_id=player.saved_on_ground_edge_id;
    if (player.is_on_left_ground_edge) {
      GrLine(hdc,player.sprite_x,
                 player.sprite_y,
                 Ground[tmp_ground_id]->x1+player.cam_x+player.cam_move_x+player.cam_mouse_move_x+player.cam_limiter_x,
                 Ground[tmp_ground_id]->y1+player.cam_y+player.cam_move_y+player.cam_mouse_move_y+player.cam_limiter_y,
                 LTCYAN);
    } else if (player.is_on_right_ground_edge) {
      GrLine(hdc,player.sprite_x,
                player.sprite_y,
                Ground[tmp_ground_id]->x2+player.cam_x+player.cam_move_x+player.cam_mouse_move_x+player.cam_limiter_x,
                Ground[tmp_ground_id]->y2+player.cam_y+player.cam_move_y+player.cam_mouse_move_y+player.cam_limiter_y,
                LTCYAN);
    }
  }

  bool is_blink=TRUE;
  if (player.speed>24 && frame_tick%2==0) {
    is_blink=FALSE;
  }
  if (is_blink) {
    if (player.attack_timer==-1 || player.rst_down) { //not attacking
      if (player.block_timer==0) { //not blocking
        if (player.on_ground_timer>0) { // on ground
          if (player.walk_cycle<2) {
            if (player.in_water_timer>0) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_sprite_1[player.sprite_angle],player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].sprite_1[player.sprite_angle],player.last_left);
            }
          } else {
            if (player.in_water_timer>0) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_sprite_2[player.sprite_angle],player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].sprite_2[player.sprite_angle],player.last_left);
            }
          }
        } else { //in_air
          if (!player.is_on_ground_edge) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,hdc2,player.blur_sprite_x[j],player.blur_sprite_y[j],&PlayerSprite[0].blur_sprite_jump,player.last_left);
            }
          }
          if (player.in_water_timer>0) {
            DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_sprite_jump,player.last_left);
          } else {
            DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].sprite_jump,player.last_left);
          }
        }
      } else { //blocking
        if (player.on_ground_id==-1 && player.spin_timer>0) { //not on ground
          if (player.spin_timer<10) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,hdc2,player.blur_sprite_x[j],player.blur_sprite_y[j],&PlayerSprite[0].blur_spin_sprite[0],!player.last_left);
            }
            if (player.in_water_timer>0) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_spin_sprite[0],!player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].spin_sprite[0],!player.last_left);
            }
          } else if (player.spin_timer>10 && player.spin_timer<20) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,hdc2,player.blur_sprite_x[j],player.blur_sprite_y[j],&PlayerSprite[0].blur_spin_sprite[1],!player.last_left);
            }
            if (player.in_water_timer>0) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_spin_sprite[1],!player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].spin_sprite[1],!player.last_left);
            }
          } else if (player.spin_timer>20 && player.spin_timer<30) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,hdc2,player.blur_sprite_x[j],player.blur_sprite_y[j],&PlayerSprite[0].blur_spin_sprite[2],!player.last_left);
            }
            if (player.in_water_timer>0) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_spin_sprite[2],!player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].spin_sprite[2],!player.last_left);
            }
          } else {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,hdc2,player.blur_sprite_x[j],player.blur_sprite_y[j],&PlayerSprite[0].blur_spin_sprite[3],!player.last_left);
            }
            if (player.in_water_timer>0) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_spin_sprite[3],!player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].spin_sprite[3],!player.last_left);
            }
          }
        } else { //on ground
          if (player.in_water_timer>0) {
            if (0<player.block_timer && player.block_timer<=5) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_block_sprite_1[player.sprite_angle],player.last_left);
            } else if (5<player.block_timer && player.block_timer<=10) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_block_sprite_2[player.sprite_angle],player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_block_sprite_3[player.sprite_angle],player.last_left);
            }
          } else {
            if (0<player.block_timer && player.block_timer<=5) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].block_sprite_1[player.sprite_angle],player.last_left);
            } else if (5<player.block_timer && player.block_timer<=10) {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].block_sprite_2[player.sprite_angle],player.last_left);
            } else {
              DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].block_sprite_3[player.sprite_angle],player.last_left);
            }
          }
        }
      }
    } else {
      if (!(player.on_ground_timer>0)) {
        for (int j=0;j<PLAYER_BLUR_NUM;j++) {
          DrawSprite(hdc,hdc2,player.blur_sprite_x[j],player.blur_sprite_y[j],&PlayerSprite[0].blur_sprite_jump,player.last_left);
        }
      }
      if (player.in_water_timer>0) {      
        if (30<player.attack_timer && player.attack_timer<=40) {//attack sprite
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_attack_sprite_1[player.sprite_angle],player.last_left);
        } else if (20<player.attack_timer && player.attack_timer<=30) {
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_attack_sprite_2[player.sprite_angle],player.last_left);
        } else if (10<player.attack_timer && player.attack_timer<=20) {
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_attack_sprite_3[player.sprite_angle],player.last_left);
        } else if (-1<player.attack_timer && player.attack_timer<=10) {
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].dithered_attack_sprite_4[player.sprite_angle],player.last_left);
        }
      } else {
        if (30<player.attack_timer && player.attack_timer<=40) {//attack sprite
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].attack_sprite_1[player.sprite_angle],player.last_left);
        } else if (20<player.attack_timer && player.attack_timer<=30) {
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].attack_sprite_2[player.sprite_angle],player.last_left);
        } else if (10<player.attack_timer && player.attack_timer<=20) {
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].attack_sprite_3[player.sprite_angle],player.last_left);
        } else if (-1<player.attack_timer && player.attack_timer<=10) {
          DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].attack_sprite_4[player.sprite_angle],player.last_left);
        }
      }
    }
  }

  //Shapes Drawn when swinging to show direction of swing
  int color=WHITE;//Highlight(IsInvertedBackground(),WHITE,BLACK);
  if (player.is_swinging) {
    GrLine(hdc,player.sprite_x,
               player.sprite_y,
                player.pivot_x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x+player.cam_limiter_x,
                player.pivot_y+player.cam_y+player.cam_move_y+player.cam_mouse_move_y+player.cam_limiter_y,
                color);
    //line showing where player will launc/fling to
    /*GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x,
                 player.pivot_y+player.cam_y+player.cam_move_y+player.cam_mouse_move_y,
                DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,color,-1);
    GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+100*cos(player.angle_of_incidence),player.sprite_y+100*sin(player.angle_of_incidence),LTGREEN);*/
  }



    //Draw sniper bullet of player
    if (player.bullet_shot!=-1) {
      if (player.right_click_hold_timer<RCLICK_HOLD_COOLDOWN) {
        DrawBullet(hdc,hdc2,player.bullet_shot);
        GrLine(hdc,player.sprite_x,player.sprite_y,Bullet[player.bullet_shot].sprite_x,Bullet[player.bullet_shot].sprite_y,color);    
      }
    }
  }
  } else if (ptype==1) { //bee but moving
    /*if (player.health>0) {
    if (player.rst_left || player.rst_right || player.rst_up || player.rst_down) {
      if (player.walk_cycle<2) {
        DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y+2,&PlayerSprite[0].sprite_bee_aero_1,player.last_left);
      } else {
        DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y+2,&PlayerSprite[0].sprite_bee_aero_2,player.last_left);
      }
    } else {
      if (player.walk_cycle<2) { //bee but hovering
        DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].sprite_bee_1,player.last_left);
      } else {
        DrawSprite(hdc,hdc2,player.sprite_x,player.sprite_y,&PlayerSprite[0].sprite_bee_2,player.last_left);
      }
    }
    }*/
  }
  
  //player draw self bullets even after death
  for (int i=0;i<player.bullet_shot_num;i++) {
    DrawBullet(hdc,hdc2,player.bullet[i]);
  }

  //debug print x,y cam mouse, when see post left gridx==0, x should be negative
  //int le1=GR_WIDTH/2-player.x+player.cam_mouse_move_x+player.cam_move_x+player.cam_limiter_x;
  //int le2=GR_HEIGHT/2-player.y+player.cam_mouse_move_y+player.cam_move_y+player.cam_limiter_y;


  //char printme[16];
  //sprintf(printme,"x:%d,y:%d",le1,le2);
  //GrPrint(hdc,mouse_x,mouse_y+20,printme,LTGREEN);
}

