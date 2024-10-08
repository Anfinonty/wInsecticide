
bool IsInvertedBackground()
{
  if (map_background==1) {
    return TRUE;
  } else if (is_inverted && map_background==2) {
    return TRUE;
  }
  return FALSE;
}



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

void move_x(double x)
{
  player.x+=x;
  player.cam_x-=x;
}


void move_y(double y)
{
  player.y+=y;
  player.cam_y-=y;
}


/*
void InitRDGrid()
{
  RDGrid.x1=player.x-(GR_WIDTH/2)-2*GRID_SIZE;//top left corner x
  RDGrid.y1=player.y-(GR_HEIGHT/2)-2*GRID_SIZE;//top left corner y
  RDGrid.x2=player.x+(GR_WIDTH/2)+2*GRID_SIZE;//down right corner x
  RDGrid.y2=player.y+(GR_HEIGHT/2)+2*GRID_SIZE;//down right cornder y
  for (int i=0;i<player.rendered_enemy_num;i++) {
    player_render_enemies[i]=-1;
  }
  //player.rendered_enemy_num=0;

  /*for (int i=0;i<ENEMY_NUM;i++) {
    if (Enemy[i]->x>RDGrid.x1 && Enemy[i]->x<RDGrid.x2 && Enemy[i]->y>RDGrid.y1 && Enemy[i]->y<RDGrid.y2) {
      player_render_enemies[player.rendered_enemy_num]=i;
      player.rendered_enemy_num++;
    }
  }
}*/


void CameraInit(double x,double y)
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

void InitPlayerCamera()
{
//set camera
  player.cam_x=0;
  player.cam_y=0;
  CameraInit(player.saved_x,player.saved_y); //idk scaling is weird for sprite
}

/*bool NearCrawler()
{
  int i=0,j=0;
  for (i=0;i<player.rendered_enemy_num;i++) {
    j=player_render_enemies[i];
    if (Enemy[j].species==1 && Enemy[j].health>0) {
      if (Enemy[j].dist_from_player<=NODE_SIZE*3) {
        return TRUE;
      }
    }
  }
  return FALSE;
}*/


void PlayerPlaceWeb()
{
  player.web_storage[player.placed_web_pos]=-1; //make current web empty
  player.placed_web_pos++; //move to next web
  player.placed_web_num++; //increase number of placed web
  if (player.placed_web_pos>=player.max_web_num) { //go back to index 0 if over the limit
    player.placed_web_pos=0;
  }
}


void CleanUpPlayer()
{
  player.speed=DEFAULT_PLAYER_SPEED;

  //Walk sprites
  if (player.sprite_1_cache!=NULL) {
    DeleteObject(player.sprite_1_cache);
  }
  if (player.sprite_2_cache!=NULL) { 
    DeleteObject(player.sprite_2_cache);
  }
  if (player.sprite_jump_cache!=NULL) { 
    DeleteObject(player.sprite_jump_cache);
  }

  //Attack sprites
  if (player.attack_sprite_1_cache!=NULL) { 
    DeleteObject(player.attack_sprite_1_cache);
  }
  if (player.attack_sprite_2_cache!=NULL) { 
    DeleteObject(player.attack_sprite_2_cache);
  }
  if (player.attack_sprite_3_cache!=NULL) { 
    DeleteObject(player.attack_sprite_3_cache);
  }
  if (player.attack_sprite_4_cache!=NULL) { 
    DeleteObject(player.attack_sprite_4_cache);
  }

  //Block sprites
  if (player.block_sprite_1_cache!=NULL) {
    DeleteObject(player.block_sprite_1_cache);
  }
  if (player.block_sprite_2_cache!=NULL) {
    DeleteObject(player.block_sprite_2_cache);
  }
  if (player.block_sprite_3_cache!=NULL) {
    DeleteObject(player.block_sprite_3_cache);
  }

  //spin sprites
  if (player.spin_sprite_1_cache!=NULL) {
    DeleteObject(player.spin_sprite_1_cache);
  }
  if (player.spin_sprite_2_cache!=NULL) {
    DeleteObject(player.spin_sprite_2_cache);
  }
  if (player.spin_sprite_3_cache!=NULL) {
    DeleteObject(player.spin_sprite_3_cache);
  }
  if (player.spin_sprite_4_cache!=NULL) {
    DeleteObject(player.spin_sprite_4_cache);
  }

  //blur sprites
  if (player.blur_sprite_jump_cache!=NULL) { 
    DeleteObject(player.blur_sprite_jump_cache);
  }
  if (player.spin_blur_sprite_1_cache!=NULL) {
    DeleteObject(player.spin_blur_sprite_1_cache);
  }
  if (player.spin_blur_sprite_2_cache!=NULL) {
    DeleteObject(player.spin_blur_sprite_2_cache);
  }
  if (player.spin_blur_sprite_3_cache!=NULL) {
    DeleteObject(player.spin_blur_sprite_3_cache);
  }
  if (player.spin_blur_sprite_4_cache!=NULL) {
    DeleteObject(player.spin_blur_sprite_4_cache);
  }


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
}




void InitPlayerFlingWeb()
{
  for (int i=0;i<PLAYER_FLING_WEB_NUM;i++) {
    player_fling_web.x[i]=0;
    player_fling_web.y[i]=0;
    player_fling_web.sprite_x[i]=0;
    player_fling_web.sprite_y[i]=0;
  }
}




void PlayerBulletLimitAct()
{
  /*if (player.max_web_num-player.placed_web_num>=3 && player.knives_per_throw==5) {
    player.knives_per_throw=13;
  }*/

  /*if (player.max_web_num-player.placed_web_num>5) {
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,30+1);
  } else*/ if (player.max_web_num-player.placed_web_num>2) {
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,15+1); //limit to 1,3,5,15
  } else if (player.max_web_num-player.placed_web_num>0){ //limit to 1,3,5
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,6);
  } else { //limit to 1,3
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,4);
  }

}



void InitPlayer() {
  int i;
  //player.hiding=FALSE;
  player.rst_left_click=FALSE;
  player.rst_right_click=FALSE;
  player.left_click=FALSE;
  player.right_click=FALSE;
  player.rst_down=FALSE;
  player.rst_left=FALSE;
  player.rst_right=FALSE;
  player.rst_up=FALSE;

  player.rst_arrow_down=FALSE;
  player.rst_arrow_left=FALSE;
  player.rst_arrow_right=FALSE;
  player.rst_arrow_up=FALSE;

  player.low_jump=FALSE;
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
  player.is_on_ground_edge=FALSE;
  player.is_on_left_ground_edge=FALSE;
  player.is_on_right_ground_edge=FALSE;
  player.on_ground_edge_id=-1;
  player.saved_on_ground_edge_id=-1;

  player.show_health_timer=HP_SHOW_TIMER_NUM;
  player.show_block_health_timer=HP_SHOW_TIMER_NUM;

  player.grav=2;
  player.decceleration_timer=0;
  player.jump_height=0;
  player.sprite_timer=0;
  player.in_air_timer=0;
  player.speed=DEFAULT_PLAYER_SPEED;
  player.on_ground_timer=0;
  player.on_ground_id=-1;
  player.on_a_ground=FALSE;
  player.saved_ground_id=-1;
  player.walk_cycle=0;
  player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
//  player.key_jump_timer=0;
  player.time_breaker_tick=0;
  player.color=player_color; //displayed color
  player.load_color=player_load_color; //orginal color

  player.saved_x=saved_player_x;
  player.saved_y=saved_player_y;
  player.x=player.saved_x;
  player.y=player.saved_y;
  player.claws_x=player.x;
  player.claws_y=player.y;
  player.above_x=player.x;
  player.above_y=player.y;
  player.above_x2=player.x;
  player.above_y2=player.y;
  player.saved_sprite_angle=0;
  player.saved_attack_sprite_angle=0;
  player.saved_block_sprite_angle=0;
  player.sprite_angle=0;
  player.angle=0;
  player.saved_angle=0;
  player.player_grav=0.5;

  player.seed=0;
  player.cam_x=0;
  player.cam_y=0;
  player.cam_move_x=0;
  player.cam_move_y=0;
  player.sprite_x=GR_WIDTH/2;
  player.sprite_y=GR_HEIGHT/2;

  player.left_click_hold_timer=0;
  player.right_click_hold_timer=0;
  player.attack_timer=1;
  player.sleep_timer=DEFAULT_SLEEP_TIMER;
  player.hit_cooldown_timer=0;
  player.block_timer=0;
  player.block_recharge_timer=0;
  player.block_recharge_timer_max=100;//300;
  player.block_cooldown=0;
  player.block_cooldown_max=300;
  player.block_health_max=DEFAULT_PLAYER_BLOCK_HEALTH_MAX;
  player.block_health=DEFAULT_PLAYER_BLOCK_HEALTH_MAX;

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

  player.bullet_shot=-1;

  player.placed_web_pos=0;
  player.placed_web_num=0;
  player.destroyed_web_pos=0;


  player.knives_per_throw=1;
  player.fast_duration=0;
  player.shoot_knife_duration=0;


  player.mouse_dist=0;
  player.mouse_angle=0;



  player.rain_wet_timer=0;
  player.visible_rain_wet_timer=0;
  player.in_water=FALSE;


  /*player_fling_web.length=0;
  for (i=0;i<64;i++) {
    player_fling_web.on_ground_id[i]=-1;
    player_fling_web.angle[i]=-1;
    player_fling_web.x[i]=-1;
    player_fling_web.y[i]=-1;
  }*/


  player.web_being_shot=-1;
  for (i=0;i<MAX_WEB_NUM;i++) {
    player.web_storage[i]=GROUND_NUM+i;
  }

  /*for (i=0;i<MAX_WEB_NUM;i++) {
    player.placed_web[i]=-1;
  }*/

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
  player.pivot_angle=-20;
  player.pivot_length=-20;
  player.is_swinging=FALSE;
  player.jump_angle=0;
  player.jump_angle2=0;
  player.fling_distance=0;
  player.previous_web_placed=-1;

  player.is_rebounding=FALSE;

  //player.dist_angle=0;
  player.angle_of_incidence=0;
  player.angle_of_reflection=0;
  player.spin_timer=0;
  
  player.current_blur_sprite=0;
  for (i=0;i<PLAYER_BLUR_NUM;i++) {
    player.blur_timer[i]=0;
    player.blur_x[i]=0;
    player.blur_y[i]=0;
  }

  InitPlayerCamera();

  InitPlayerFlingWeb();
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



void PlayerAct() {
  int speed=0,grav_speed=0,claws_l=NODE_SIZE;
  double grad_x1=0,grad_y1=0,grad_x2=0,grad_y2=0;
  bool allow_act=FALSE;

  if (player.rain_wet_timer>0) {
    player.rain_wet_timer--;
  }
  if (player.visible_rain_wet_timer>0) {
    player.visible_rain_wet_timer--;
  }

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

  //Sprinting
  /*if (player.rst_key_sprint) {
    if (player.on_ground_id!=-1) {//on ground
      if (player.speed<3) 
        player.speed++;
    }
  }*/



  //Mouse Actions
  player.mouse_dist=GetDistance(GR_WIDTH/2,GR_HEIGHT/2,mouse_x,mouse_y);
  player.mouse_angle=GetCosAngle(mouse_x-GR_WIDTH/2,player.mouse_dist);

  int divider=2;
  /*if (player.mouse_dist>GRID_SIZE) {
    divider=2;
  }*/
  player.cam_mouse_move_x=-cos(player.mouse_angle)*player.mouse_dist/divider;
  if (mouse_x==GR_WIDTH/2)
    player.cam_mouse_move_x=0;

  if (mouse_y>GR_HEIGHT/2) //below half of screen
    player.cam_mouse_move_y=-sin(player.mouse_angle)*player.mouse_dist/divider;
  else if (mouse_y<GR_HEIGHT/2)
    player.cam_mouse_move_y=sin(player.mouse_angle)*player.mouse_dist/divider;
  else
    player.cam_mouse_move_y=0;

  //Clicking Actions
  if (player.bullet_shot!=-1) {
    player.web_being_shot=player.web_storage[player.placed_web_pos];
    BulletAct(player.bullet_shot);
  }
  for (int i=0;i<player.bullet_shot_num;i++) {
    BulletAct(player.bullet[i]);
  }

  Click();
  if (player.attack_rst) {//Left click to Attack
    player.attack=TRUE;
    player.blocking=FALSE; //unblock

    if (player.bullet_shot!=-1) {
      StopBullet(player.bullet_shot,TRUE); //stop all web bullets from acting
      player.web_being_shot=-1;
      player.bullet_shot=-1;
    }

    int b_speed_m=5;
    double b_dmg_m=1;
    int b_g_type=5;
    if (player.speed>10) {
      b_speed_m=9;
    } else if (player.speed>5) {
      b_speed_m=7;      
    }

    /*if (player.knives_per_throw==3) {
      b_dmg_m=0.25;
      b_g_type=7;
      if (player.speed>10)
        b_dmg_m=0.35;
      if (player.speed>24)
        b_dmg_m=0.5;
    } else {
    }*/

    if (player.speed>10)
      b_dmg_m=2;
    if (player.speed>24)
      b_dmg_m=4;


    if (player.bullet_shot_num<PLAYER_BULLET_NUM && 
        !player.is_swinging && 
        (PLAYER_BULLET_NUM-player.bullet_shot_num>=player.knives_per_throw) // a/b whehere a>=b a is bullet in storage, b is bullet consumption
    ) {
      grad_x1=player.sprite_x;
      grad_y1=player.sprite_y;
      grad_x2=mouse_x;
      grad_y2=mouse_y;
      double tmp_angle=0;

      if (player.max_web_num-player.placed_web_num<3) {  //0,1,3,5
        player.knives_per_throw=LimitValue(player.knives_per_throw,1,6);
      } 
      if (player.max_web_num-player.placed_web_num<1) {  //0,1,3
        player.knives_per_throw=LimitValue(player.knives_per_throw,1,4);
      }
      if (player.knives_per_throw>4) {
        b_g_type=6;
      } else if (player.knives_per_throw==1) {
        b_g_type=9;
        b_dmg_m=3;
      }


      for (int q=0;q<player.knives_per_throw;q++) {
        if (q>0) {        
          if (q%2==0) {//even
            tmp_angle+=M_PI_2/16*q;
          } else {
            tmp_angle-=M_PI_2/16*q;
          }
        }
	    player.bullet[player.bullet_shot_num]=current_bullet_id;
        if (game_audio) {
          player.shoot_knife_duration=1;
        }
        ShootBullet(
            current_bullet_id,
	        player.bullet_shot_num,
	        player_bullet_color,
            b_g_type, //graphics type
	        MAX_WEB_LENGTH+player.speed*3, //range ==>
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
        player.bullet_shot_num++;
        current_bullet_id++;
        if (current_bullet_id>=SHOOT_BULLET_NUM-1) {
          current_bullet_id=0;
        }


        if ((q+1)%5==0) {
          PlayerPlaceWeb(); //Web related
          /*if (player.knives_per_throw==15) {
            
          }*/
          PlayerBulletLimitAct();
          if (player.max_web_num-player.placed_web_num==0) {
            player.knives_per_throw=1;
          }
        }



        if (player.max_web_num-player.placed_web_num==0 && q>5) {
          break;
        }
      }
    }
  }

  if (player.right_click_hold_timer==62) {//Right click to Shoot Again
    if (player.bullet_shot!=-1 && !player.is_swinging) {
      StopBullet(player.bullet_shot,TRUE);
      player.web_being_shot=-1;
      player.bullet_shot=-1;
    }
  }


  if (!player.attack_rst && !player.is_swinging) {
    allow_act=FALSE;
    if (player.right_click_hold_timer==62) { //right click to shoot
      if (player.placed_web_num<player.max_web_num && //webs > 0 
        player.bullet_shot==-1) {
        allow_act=TRUE;
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
      if (game_audio) {
        player.shoot_knife_duration=1;
      }
      int b_speed_mul=15+player.speed*2; //speed multiplier
      if (b_speed_mul>25)
        b_speed_mul=25;
      ShootBullet(current_bullet_id,
	-1,
	CYAN,
    5, //graphics type
	MAX_WEB_LENGTH, //range
    1, //speed
	b_speed_mul, //speed multiplier
	4+player.attack_strength*2, //damage
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
      if (player.left_click_hold_timer==62 || player.attack_rst || player.right_click_hold_timer==62) { //swing but no web is placed
        if (game_audio) {
          player.shoot_knife_duration=1;
        }
        player.is_swinging=FALSE; //stop swinging
        if (player.uppercut) {
          player.fling_distance=0;
        } else {
          player.fling_distance=player.pivot_length;
        }
        //player.key_jump_timer=player.player_jump_height;
        player.grav=3; //grav when swing let go
        player.in_air_timer=1000;
        player.decceleration_timer=0;
        if (player.speed<10)
          player.speed+=2;


        if (M_PI_2<player.angle_of_incidence && player.angle_of_incidence<M_PI+M_PI_2)
          player.last_left=TRUE;
        else
          player.last_left=FALSE;
      }
    }
    
    if (player.right_click_hold_timer==62) {
      if (player.pivot_length>NODE_SIZE*5) {//prevent build web if web too short
        player.attack=TRUE;
        player.blocking=FALSE; //unblock
    //player place web after swing
    //BUG: sometimes web placed doesnt get placed as expected
        double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
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
            bm_x1-=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y1+=sin(player.pivot_angle)*NODE_SIZE*2;
            bm_x2+=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y2-=sin(player.pivot_angle)*NODE_SIZE*2;
          } else { //player is right
            bm_x1+=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y1-=sin(player.pivot_angle)*NODE_SIZE*2;
            bm_x2-=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y2+=sin(player.pivot_angle)*NODE_SIZE*2;
          }
        } else { //player is below pivot
          //player is left
          if (player.x<player.pivot_x) {
            bm_x1-=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y1-=sin(player.pivot_angle)*NODE_SIZE*2;
            bm_x2+=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y2+=sin(player.pivot_angle)*NODE_SIZE*2;
          } else { //player is right
            bm_x1+=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y1+=sin(player.pivot_angle)*NODE_SIZE*2;
            bm_x2-=cos(player.pivot_angle)*NODE_SIZE*2;
            bm_y2-=sin(player.pivot_angle)*NODE_SIZE*2;
          }
        }

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
            //player.placed_web[player.placed_web_num]=web_id;
            PlayerPlaceWeb();            
            PlayerBulletLimitAct();
            Ground[web_id]->health=150;//-q;
          }
        }
      }
    }
  }
  if (player.attack_rst) {
    player.attack_rst=FALSE;
  }

  //Trigger movements
  if (player.rst_left || player.rst_right) {
    if (player.rst_left) {
      player.last_left=TRUE;
    } else {
      player.last_left=FALSE;
    }
    player.walk_cycle++; //Walk sprite cycle
    if (player.walk_cycle>=4) {
      player.walk_cycle=0;
    }
  } else {
    player.walk_cycle=0;
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

  if (player.rst_up && player.on_a_ground) { //on ground and jumping
    //player.key_jump_timer=player.player_jump_height;
    player.jump=TRUE;
    player.on_ground_edge_id=-1;
    player.is_on_ground_edge=FALSE;
    player.is_on_left_ground_edge=FALSE;
    player.is_on_right_ground_edge=FALSE;
  }

  if (player.jump && player.jump_height==0) {
    player.jump_height=player.player_jump_height;
  }


  if (player.decceleration_timer==0) {
    player.decceleration_timer=100;
    if (player.speed>=10) {
      if (IsSpeedBreaking())
        player.decceleration_timer=320;
      else
        player.decceleration_timer=117;
    }
    if (player.speed>=24) {
      if (IsSpeedBreaking())
        player.decceleration_timer=335;
      else
        player.decceleration_timer=250;
    }
  }
  if (player.decceleration_timer>0 && !player.is_swinging) {
    player.decceleration_timer++;
  }
  if (player.decceleration_timer>350) {
    if (player.speed>1 && player.on_ground_id<GROUND_NUM && !player.time_breaker) {
      player.speed--;        
    }
    player.decceleration_timer=0;
  }


  int speed_limiter=player.speed;
  if (player.speed>24) {
    speed_limiter=player.speed;//17+(player.speed-24)/8;
  } else if (player.speed>10) {
    speed_limiter=10+(player.speed-10)/4;
  } else if (player.speed>5) {
    speed_limiter=5+(player.speed-5)/2;
  }

  if (player.in_water) {
    player.grav=1;
    speed_limiter=speed_limiter/2+1;
  }


  for (speed=0;speed<speed_limiter;speed++) {
    for (grav_speed=0;grav_speed<player.grav;grav_speed++) {
      //player.on_ground_id=GetOnGroundIdPlayer(player.x,player.y,5,4);
      player.on_ground_id=GetOnGroundIdPlayer(player.x,player.y,5,4);
      //player.on_ground_id=GetOnGroundId(player.x,player.y,5,4,TRUE);

   //hiding?    (legacy feature)
      /*if (NodeGrid[GetGridId(player.above_x,player.above_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)]->node_solid) {
        player.hiding=TRUE;
      } else {
        player.hiding=FALSE;
      }*/

      int in_node_grid_id=GetGridId(player.x,player.y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (in_node_grid_id!=-1) {
        //if (NodeGrid[in_node_grid_id]->tmp_wet)
          //player.rain_wet_timer=260;
        if (NodeGrid[in_node_grid_id]->node_water) {
          player.in_water=TRUE;
        } else {
          player.in_water=FALSE;
        }
      } else {
        player.in_water=FALSE;
      }
      if (player.time_breaker && player.rain_wet_timer>0) {
        player.rain_wet_timer=160;
      }




   //Destroy Ground (regainable)
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


      if (player.health<=PLAYER_LOW_HEALTH) {
        if (player.speed<11) {
          player.speed=11;
        }
      }


      if (player.spin_timer>0 && player.speed<4) {
        player.speed=4;
        player.decceleration_timer=0;
      }



      /*if (player.in_air_timer>=1 && player.in_air_timer<=4 && player.on_ground_id==-1) { //Grav switch //runs on speed and grav_speed //retiring
        if (player.jump_height<=0 && !player.is_swinging) {
          if (player.previous_below && speed<=24 && grav_speed<=2)
            move_y(-player.player_grav*1.5);
        }
      }*/

   //Ground action
   //on a ground
      if (player.on_ground_id!=-1 && player.on_ground_id!=player.previous_web_placed) {

        //get dist between player and ground
        double ground_entity_E=GetLineTargetAngle(player.on_ground_id,player.x,player.y);
        double height_from_player_x=GetLineTargetHeight(player.on_ground_id,ground_entity_E,player.x,player.y);


        //player speed when on ground
        if (player.speed<10)
          player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
        else 
          player.player_jump_height=150;

        if (!player.low_jump || player.speed<10)
          player.player_jump_height+=player.speed*NODE_SIZE;
        else
          player.player_jump_height+=9*NODE_SIZE;

        //player speed when on ground
        /*if (!player.is_swinging && player.fling_distance==0 && player.on_ground_timer>=1 && speed==0 && grav_speed==0) {
          if (!player.is_rebounding) {
            //if (!player.rst_key_sprint) {
              //if (player.speed>DEFAULT_PLAYER_SPEED)
                //player.speed--;
            //} else {
              if (player.speed>1)
                player.speed--;
            //}
          }
        }*/


        //player ground interaction
        if  ((Ground[player.on_ground_id]->x1-10<player.x &&  player.x<Ground[player.on_ground_id]->x2+10) &&
              ((Ground[player.on_ground_id]->y1-10<player.y && player.y<Ground[player.on_ground_id]->y2+10) ||
               (Ground[player.on_ground_id]->y2-10<player.y && player.y<Ground[player.on_ground_id]->y1+10)))
          {
         //Within ground axes
          player.angle=Ground[player.on_ground_id]->angle; //set player angle




          if (player.in_air_timer>0) {
            if (player.in_air_timer>1000) { //make player rebound
              player.in_air_timer=1000;
            }
            if (player.rst_down) { //spinning
              player.is_rebounding=TRUE;
              //if (player.rst_left || player.rst_right) { //continue spin-rebound
                player.in_air_timer=1000;
              //}
            } else { //not reboubding
              player.is_rebounding=FALSE;
              player.in_air_timer=1;

              player.jump_height=0; //Stop Jump & stick to ground
              if (player.uppercut) {
                player.jump_height=-1; //Stop Jump & stick to ground
              }
              player.jump=FALSE;
            }
            player.in_air_timer--;
            /*if (player.is_rebounding && player.in_air_timer==998) {
              player.speed--; //lower speed on landing
            }*/
          }

        //outwards from ground
          if (0<height_from_player_x && height_from_player_x<10) { //above ground
            //if (height_from_player_x>4) {
              player.current_above=TRUE;
              player.current_below=FALSE;
              player.previous_above=TRUE;
              player.previous_below=FALSE;
            //}

            player.on_a_ground=TRUE;
            player.on_ground_timer=20;
            if (height_from_player_x<5 || player.is_rebounding/* || player.is_swinging*/) {
              move_x(-cos(player.angle+M_PI_2));
              move_y(-sin(player.angle+M_PI_2));
            }
            player.is_on_ground_edge=FALSE;
            player.is_on_left_ground_edge=FALSE;
            player.is_on_right_ground_edge=FALSE;
            player.jump_angle=player.angle+M_PI_2;
            if (player.angle<0) {/*Slope -. /*/
              player.jump_angle2=2*M_PI+player.angle-M_PI_2;
            } else {/*Slope -. \*/
              player.jump_angle2=player.angle-M_PI_2;
            }
            player.fling_distance=0; //on ground, stop flinging

            //angle of incidence and reflection
            player.angle_of_reflection=2*M_PI-player.angle_of_incidence+2*player.angle; //real
            if (!player.is_swinging)
              player.angle_of_incidence=player.angle_of_reflection;
          } else if (-10<height_from_player_x && height_from_player_x<=0) { //below ground
            //if (height_from_player_x<-4) {
              player.current_below=TRUE;
              player.current_above=FALSE;
              player.previous_above=FALSE;
              player.previous_below=TRUE;
            //}
            player.on_a_ground=TRUE;
            player.on_ground_timer=20;
            if (height_from_player_x>-5 || player.is_rebounding/* || player.is_swinging*/) {
              move_x(-cos(player.angle-M_PI_2));
              move_y(-sin(player.angle-M_PI_2));
            }
            player.is_on_ground_edge=FALSE;
            player.is_on_left_ground_edge=FALSE;
            player.is_on_right_ground_edge=FALSE;
            player.jump_angle=player.angle-M_PI_2;
            if (player.angle<0) {/*Slope -. /*/
              player.jump_angle2=M_PI_2+player.angle;
            } else {/*Slope -. \*/
              player.jump_angle2=player.angle+M_PI_2;
            }
            player.fling_distance=0; //on ground, stop flinging

            //angle of incidence and reflection
            player.angle_of_reflection=2*M_PI-player.angle_of_incidence+2*player.angle; //real
            if (!player.is_swinging)
              player.angle_of_incidence=player.angle_of_reflection;
          }


          if (player.angle_of_reflection>2*M_PI) {
            player.angle_of_reflection-=2*M_PI;
          }
          if (player.angle_of_reflection<0) {
            player.angle_of_reflection+=2*M_PI;
          }


          if (player.is_rebounding) {//rebounding
            if (M_PI_2<player.angle_of_reflection && player.angle_of_reflection<M_PI+M_PI_2) {
              player.last_left=TRUE;
            } else {
              player.last_left=FALSE;
            }
          }

          if (height_from_player_x>9 || height_from_player_x<-9) {//leave ground when out of circular range
            player.on_a_ground=FALSE;
            player.on_ground_timer=0;
          }

        } else { //not in range based on ground's x and y boundaries
          player.on_a_ground=FALSE;
          player.on_ground_timer=0;
        }
      }

    //Y movement
    //Condition to jump
      if (player.on_ground_timer>0) {
        player.previous_web_placed=-1;
      }



      //==========REBOUND ACTIONS==========
      if (player.is_rebounding && !player.is_swinging && grav_speed==0) {
        move_x(cos(player.angle_of_reflection));
        move_y(sin(player.angle_of_reflection));
      }

    //Gravity
      if (grav_speed==0 && speed==0) {
        if (player.on_ground_id==-1 && !player.is_swinging && !player.is_on_ground_edge) {
          player.in_air_timer++;
          if (player.in_air_timer>2002) { //dont go above this limit
            player.in_air_timer--;
          }
	      if (player.jump_height==0) {
            if (player.in_air_timer%20==0 && player.grav<=100) {
              player.grav++;
              /*if (player.is_rebounding && player.speed<4) {
                player.speed++;
              }*/
            }
	      }
        }
      //Jump movement
        /*if (player.key_jump_timer>0) {
          player.key_jump_timer--;
        } else {
          player.jump_height=0;
          player.jump=FALSE;
        }*/
      }


      //PLAYER GRAVITY MOVEMENT
      if ((speed==0 && player.speed<5) || (player.speed>=5 && grav_speed==0)) {

        if (player.jump_height>0) { //Jumping action
          player.jump_height-=player.player_grav;
          if (player.uppercut && (player.rst_left || player.rst_right)) {
            player.jump_height=0;
          }
          move_x(2*player.player_grav*-cos(player.jump_angle));
          move_y(2*player.player_grav*-sin(player.jump_angle)); //jump go against gravity and perpendicular from platform
          if (player.jump_height<=0) {
            player.jump=FALSE;
          }
        }
      }


      if (speed==0) { //runs on grav speed
        if (player.on_ground_id==-1 && player.jump_height<=0) { //Credit: y4my4m for pushing me to pursue this gameplay aspect
          if (!player.is_swinging) { //not swinigng and player is not flinging
            if ((player.in_air_timer>4 || player.fling_distance<0) || (player.in_water && player.blocking)) {
              move_y(player.player_grav); //include while being rebounding and flinging
	        } //else {
              //if (player.previous_below)
              /*if (player.previous_below)
                move_y(player.player_grav*sin(player.jump_angle));
              else
                move_y(-player.player_grav*sin(player.jump_angle));*/
            //}
          }
        } else { //landed on ground
	      if (player.grav>=3 && !player.is_swinging && !player.is_on_ground_edge) {
	        if (player.grav>7) {
	          player.cam_move_x*=-1;
	        }
	        player.cam_move_y/=-1.5;
	      }
          if (!player.is_swinging)
            player.grav=2;
        }
      }

      if (player.y<0) { //Y axis cap
        move_y(1);
        player.in_air_timer++;
      } else if (player.y+PLAYER_HEIGHT/2>MAP_HEIGHT) {
        move_y(-player.player_grav);
        player.health--;
      }



     //X movement
      allow_act=FALSE;
      if (player.fling_distance<=0) { //player's fling distance is 0
        player.previous_web_placed=-1;
        if (!player.is_swinging && !player.is_on_ground_edge) { // player is not swinging and not on ground edge
          if (player.on_ground_id==-1) { //player is not on ground
            allow_act=TRUE;
          } else if (!player.blocking) { //on a ground but not blocking
            allow_act=TRUE;
          }  
        } /*else if (player.on_ground_id!=-1) { //is swinging but on ground
          if (player.pivot_length<DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE) { //allow movement if within swing distance
            allow_act=TRUE;
          }
        }*/
      }
      if (grav_speed==0 && allow_act) {
        if (player.rst_left || player.rst_right) {
          player.is_rebounding=FALSE;
        }
        if (player.current_above) { //player is above ground
          if (player.rst_right) {
            move_x(cos(player.angle));
            move_y(sin(player.angle));
          } else if (player.rst_left) {
            move_x(-cos(player.angle));
            move_y(-sin(player.angle));
          }
        } else if (player.current_below) { //player is below ground
          if (player.rst_right) {
            move_x(-cos(player.angle));
            move_y(-sin(player.angle));
          } else if (player.rst_left) {
            move_x(cos(player.angle));
            move_y(sin(player.angle));
          }
        } else { //player is not on ground
          if (player.fling_distance==0) {
            if (player.rst_left) {
              move_x(-1);
            } else if (player.rst_right) {
              move_x(1);
            }
          } else {
            if (player.angle_of_reflection>M_PI_2 && player.angle_of_reflection<M_PI+M_PI_2) {//lefft side
              if (player.rst_right) {
                move_x(1);
              }
            } else {
              if (player.rst_left) {
                move_x(-1);
              }
            }
          }
        }
      }


      //====PLAYER GROUND EDGE MOVEMENT====
      //player edge interaction, mini web rotation
      if (grav_speed==1) {
      player.on_ground_edge_id=player.on_ground_id;//GetOnGroundId(player.x,player.y,10,9);//player.on_ground_id;//GetOnGroundId(player.x,player.y,10,9);
      int tmp_on_ground_id=player.on_ground_edge_id;
      if (tmp_on_ground_id==-1) {
        tmp_on_ground_id=player.saved_on_ground_edge_id;
      }
      if (tmp_on_ground_id!=-1) {
        if (player.on_ground_edge_id!=-1) {
          player.saved_on_ground_edge_id=player.on_ground_edge_id;
        }
        if (!player.blocking && !player.rst_up) {
          double edge_dist1=GetDistance(player.x,player.y,Ground[tmp_on_ground_id]->x1,Ground[tmp_on_ground_id]->y1); //left edge
          double edge_dist2=GetDistance(player.x,player.y,Ground[tmp_on_ground_id]->x2,Ground[tmp_on_ground_id]->y2);
          if (edge_dist1<20 || edge_dist2<20) {  
            player.is_on_ground_edge=TRUE;
            player.in_air_timer=0;
            if (edge_dist1<20) {  //at edge 1
              player.is_on_left_ground_edge=TRUE;
              player.is_on_right_ground_edge=FALSE;
              double edge_angle=GetCosAngle(player.x-Ground[tmp_on_ground_id]->x1,edge_dist1);
              if (player.y<Ground[tmp_on_ground_id]->y1) { //above pivot
                /*if (edge_dist1<25) {
                  move_x(cos(-edge_angle));
                  move_y(sin(-edge_angle));
                  //player.on_ground_edge_id=-1;
                  //player.is_on_ground_edge=FALSE;
                } else {*/
                  if (player.rst_right) { //clockwize
                    move_x(cos(-edge_angle+M_PI_2));
                    move_y(sin(-edge_angle+M_PI_2));
                    player.last_left=FALSE;
                  } else if (player.rst_left) { //anticlockwize
                    move_x(-cos(-edge_angle+M_PI_2));
                    move_y(-sin(-edge_angle+M_PI_2));
                    player.last_left=TRUE;
                  }
                //}
              } else { //below pivot
                /*if (edge_dist1<25) {
                  move_x(cos(-edge_angle));
                  move_y(-sin(-edge_angle));
                 // player.on_ground_edge_id=-1;
                 // player.is_on_ground_edge=FALSE;
                } else {*/
                  if (player.rst_right) { //clockwize
                    move_x(-cos(-edge_angle+M_PI_2));
                    move_y(sin(-edge_angle+M_PI_2));
                    player.last_left=TRUE;
                  } else if (player.rst_left) { //anticlockwize
                    move_x(cos(-edge_angle+M_PI_2));
                    move_y(-sin(-edge_angle+M_PI_2));
                    player.last_left=FALSE;
                  }
                //}
              }
            } else if (edge_dist2<20) {//at edge 2
              player.is_on_right_ground_edge=TRUE;
              player.is_on_left_ground_edge=FALSE;
              double edge_angle=GetCosAngle(player.x-Ground[tmp_on_ground_id]->x2,edge_dist2);
              if (player.y<Ground[tmp_on_ground_id]->y2) { //above pivot
                /*if (edge_dist2<25) {
                  move_x(cos(-edge_angle));
                  move_y(sin(-edge_angle));
                  //player.on_ground_edge_id=-1;
                  //player.is_on_ground_edge=FALSE;
                 } else {*/
                  if (player.rst_right) { //clockwize
                    move_x(cos(-edge_angle+M_PI_2));
                    move_y(sin(-edge_angle+M_PI_2));
                    player.last_left=FALSE;
                  } else if (player.rst_left) { //anticlockwize
                    move_x(-cos(-edge_angle+M_PI_2));
                    move_y(-sin(-edge_angle+M_PI_2));
                    player.last_left=TRUE;
                  }
                //}
              } else { //below pivot
                /*if (edge_dist2<25) {
                  move_x(cos(-edge_angle));
                  move_y(-sin(-edge_angle));
                  //player.on_ground_edge_id=-1;
                  //player.is_on_ground_edge=FALSE;
                } else {*/
                  if (player.rst_right) { //clockwize
                    move_x(-cos(-edge_angle+M_PI_2));
                    move_y(sin(-edge_angle+M_PI_2));
                    player.last_left=TRUE;
                  } else if (player.rst_left) { //anticlockwize
                    move_x(cos(-edge_angle+M_PI_2));
                    move_y(-sin(-edge_angle+M_PI_2));
                    player.last_left=FALSE;
                  }
                //}
              }
            } // end of edge 2
          } else { //neither edge 1 or 2
            player.on_ground_edge_id=-1;
            player.is_on_ground_edge=FALSE;
            player.is_on_left_ground_edge=FALSE;
            player.is_on_right_ground_edge=FALSE;
          }
        } else { //player is blocking
          player.on_ground_edge_id=-1;
          player.is_on_ground_edge=FALSE;
          player.is_on_left_ground_edge=FALSE;
          player.is_on_right_ground_edge=FALSE;
        }
      } else {
        player.on_ground_edge_id=-1;
      }
      }

      //====PLAYER CIRCULAR WEB SWINGING MOVEMENT======
      if (player.is_swinging && !player.is_on_ground_edge) {
        player.grav=5;
        player.is_rebounding=FALSE;
        player.pivot_length=GetDistance(player.pivot_x,player.pivot_y,player.x,player.y);
        player.pivot_angle=GetCosAngle(player.x-player.pivot_x,player.pivot_length);


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

        if (grav_speed==3) {
          if (player.y>player.pivot_y) { //below pivot
            if (player.rst_right) { //Clockwize
              if (!player.uppercut) { //move the usual direction
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
                player.last_left=TRUE;
              } else { //move the opposite direction, anticlockwize
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
                player.last_left=FALSE;
              }
            }
            if (player.rst_left) {  //Anticlockwize
              if (!player.uppercut) {
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
                player.last_left=FALSE;
              } else { //clockwize
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
                player.last_left=TRUE;
              }
            } else if (player.rst_up && !player.on_a_ground && player.pivot_length>2){ //Retract Web
              move_x(-cos(-player.pivot_angle));
              move_y(sin(-player.pivot_angle));
            } else if (player.rst_down && player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) {
              move_x(cos(-player.pivot_angle));
              move_y(-sin(-player.pivot_angle));
            }
          } else { //above pivot
            if (player.rst_right) { //Clockwize
              //if (!player.uppercut) {
              move_x(cos(-player.pivot_angle+M_PI_2));
              move_y(sin(-player.pivot_angle+M_PI_2));
              player.last_left=FALSE;
              /*} else {
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
                player.last_left=TRUE;
              }*/
            }
            if (player.rst_left) { //Anticlockwize
              //if (!player.uppercut) {
              move_x(-cos(-player.pivot_angle+M_PI_2));
              move_y(-sin(-player.pivot_angle+M_PI_2));
              player.last_left=TRUE;
              /*} else {
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
                player.last_left=FALSE;
              }*/
            } else if (player.rst_up && !player.on_a_ground && player.pivot_length>2){ //Retract web
              move_x(-cos(-player.pivot_angle));
              move_y(-sin(-player.pivot_angle));
            } else if (player.rst_down && player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) {
              move_x(cos(-player.pivot_angle));
              move_y(sin(-player.pivot_angle));
            }
          }
        }

        if (grav_speed==4) {//only occurs right after grav_speed==0
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

        //set axes of player fling web
        if (grav_speed==4) {//only occurs right after grav_speed==0
          double _a=player.x;
          double _b=player.y;
          double _l=player.pivot_length/PLAYER_FLING_WEB_NUM;
          int tmp_ground_id=-1;
          for (int i=0;i<PLAYER_FLING_WEB_NUM;i++) {
            player_fling_web.x[i]=_a;
            player_fling_web.y[i]=_b;          
            player_fling_web.sprite_x[i]=_a+player.cam_x+player.cam_move_x+player.cam_mouse_move_x;
            player_fling_web.sprite_y[i]=_b+player.cam_y+player.cam_move_y+ player.cam_mouse_move_y;

          //to allow web bending
            tmp_ground_id=GetOnGroundId(player_fling_web.x[i],player_fling_web.y[i],2,2);
            if (tmp_ground_id!=-1) {
              if (GetDistance(player_fling_web.x[i],player_fling_web.y[i],Ground[tmp_ground_id]->x1,Ground[tmp_ground_id]->y1)<=NODE_SIZE ||
                  GetDistance(player_fling_web.x[i],player_fling_web.y[i],Ground[tmp_ground_id]->x2,Ground[tmp_ground_id]->y2)<=NODE_SIZE) {
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
      } //End of player swinging movement



      //======FLING MOVEMENT======
      if (grav_speed==0 && !player.is_swinging && !player.is_rebounding) { 
        if (((player.rst_left || player.rst_right) && player.fling_distance<-100) || player.uppercut) { //cancel flinging when left or right key is pressed
          player.fling_distance=0;
          if (player.uppercut)
            player.rst_down=FALSE;
        }


        if (player.fling_distance>0) { //fling and against gravity
          move_x(cos(player.angle_of_reflection));
          move_y(sin(player.angle_of_reflection));
          player.fling_distance--;
          if (player.fling_distance==1) { //cap in air timer right before end of fling distance
            player.in_air_timer=1002;
            player.fling_distance=-1;
            //player.speed+=2;
          }
        } else if (player.fling_distance<0){ //Continue moving but now theres gravity
          move_x(cos(player.angle_of_reflection));
          move_y(sin(player.angle_of_reflection));
          player.fling_distance--;
        }
      }




     //x-axis cap
      if (player.x-PLAYER_WIDTH/2<0) {
        move_x(1);
      } else if (player.x+PLAYER_WIDTH/2>MAP_WIDTH) {
        move_x(-1);
      }


     //misc
      player.print_current_above=player.current_above;
      player.print_current_below=player.current_below;

      if (player.in_air_timer>0) {
        player.current_above=FALSE;
        player.current_below=FALSE;
      }

      player.saved_ground_id=player.on_ground_id;


   //Set Character's Axis
      if (player.print_current_above) {
        player.above_x=player.x+(claws_l)*cos(player.angle-M_PI/2);
        player.above_y=player.y+(claws_l)*sin(player.angle-M_PI/2);
        player.above_x2=player.x+(claws_l*2)*cos(player.angle-M_PI/2);
        player.above_y2=player.y+(claws_l*2)*sin(player.angle-M_PI/2);
        if (player.last_left) {
          player.claws_x=player.x-(claws_l)*cos(player.angle);
          player.claws_y=player.y-(claws_l)*sin(player.angle);
        } else {
          player.claws_x=player.x+(claws_l)*cos(player.angle);
          player.claws_y=player.y+(claws_l)*sin(player.angle);
        }
      } else if (player.print_current_below) {
        player.above_x=player.x+(claws_l)*cos(player.angle+M_PI/2);
        player.above_y=player.y+(claws_l)*sin(player.angle+M_PI/2);
        player.above_x2=player.x+(claws_l*2)*cos(player.angle+M_PI/2);
        player.above_y2=player.y+(claws_l*2)*sin(player.angle+M_PI/2);
        if (player.last_left) {
          player.claws_x=player.x+(claws_l)*cos(player.angle);
          player.claws_y=player.y+(claws_l)*sin(player.angle);
        } else {
          player.claws_x=player.x-(claws_l)*cos(player.angle);
          player.claws_y=player.y-(claws_l)*sin(player.angle);
        }
      } else {
        player.above_x=player.x;
        player.above_y=player.y;
        player.above_x2=player.x;
        player.above_y2=player.y;
        if (player.last_left) {
          player.claws_x=player.x-(claws_l);
          player.claws_y=player.y;
        } else {
          player.claws_x=player.x+(claws_l);
          player.claws_y=player.y;
        }
      }
    }
  }




  
  //Calculating new angle of incidence
  if (!player.is_swinging) {
    if (player.jump_height>0) { //jumping and not swinging
      if (player.rst_left || player.rst_right) { // jump + holding left/right
        double t_speed1=player.speed*cos(player.jump_angle2);
        double t_speed2=player.speed;
        double t_speed=t_speed1+t_speed2;
        double t_grav=player.speed*sin(player.jump_angle2);
        double grav_dist=GetDistance(t_speed,0,0,t_grav);
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
          double grav_dist=GetDistance(player.speed,0,0,player.grav*player.player_grav);
          player.angle_of_incidence=GetCosAngle(player.speed,grav_dist);
          if (player.last_left) {
            player.angle_of_incidence=M_PI-player.angle_of_incidence;
          }
        } else { //not holding left or right but still in air
          if (player.is_rebounding || player.fling_distance!=0) {//flinging or rebounding
            double t_speed=player.speed*cos(player.angle_of_reflection); //rate of change in x . player travel to refleciton angle
            double t_grav1=player.speed*sin(player.angle_of_reflection); //rate of change in y . player travel to reflection angle
            double t_grav2=player.grav*player.player_grav; //rate of change in y . Gravity
            double t_grav=t_grav1+t_grav2; //dy/dx
            double grav_dist=GetDistance(t_speed,0,0,t_grav);
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
  if (player.attack_timer<=0) { // not attacking
    if (player.rst_down) { //pressing down button
      /*if (player.on_ground_id==-1 || player.print_current_above) { //in air or on above ground
        allow_act=TRUE;
      } else if (player.print_current_below) { // below ground
        allow_act=TRUE;
      }*/
      allow_act=TRUE;
    }
  }
  if (allow_act) {
    player.blocking=TRUE;
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
    /*if (player.on_ground_id==-1) { //player jumping && blocking
      if (player.speed<3)
        player.speed++;
    }*/
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
        player.time_breaker_units--;
        if (player.time_breaker_units%3==0) {
          player.speed++;
          player.decceleration_timer=0;
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
            //PlaySound(L"snd/timebreaker__stop.wav", NULL, SND_FILENAME | SND_ASYNC);
            //PlaySound(tb_stop_audio_cache, NULL, SND_MEMORY | SND_ASYNC);
            PlaySound(spamSoundEffectCache[1].audio, NULL, SND_MEMORY | SND_ASYNC);
          }
        }
      }
    }
  }

  //swinging
  if ((player.fling_distance>0 || player.is_swinging) && player.speed<6) { //fast when swinging
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
  if (player.sprite_angle<-M_PI*2) {
    player.sprite_angle=0;
  }


  //show hp timer
  if (player.show_health_timer>0)
    player.show_health_timer--;
  if (player.show_block_health_timer>0)
    player.show_block_health_timer--;

  //sprite axes
  player.sprite_x=GR_WIDTH/2+player.cam_move_x+player.cam_mouse_move_x;
  player.sprite_y=GR_HEIGHT/2+player.cam_move_y+player.cam_mouse_move_y;


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
    player.blur_sprite_x[i]=player.blur_x[i]+player.cam_x+player.cam_move_x+player.cam_mouse_move_x;
    player.blur_sprite_y[i]=player.blur_y[i]+player.cam_y+player.cam_move_y+player.cam_mouse_move_y;
  }

//  Enemy[i]->sprite_x=Enemy[i]->x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x;
//  Enemy[i]->sprite_y=Enemy[i]->y+player.cam_y+player.cam_move_y+ player.cam_mouse_move_y;


 //
}


void PlayerSndAct()
{
  //rain snd act
  if (player.rain_wet_timer>60) {
    if (player.visible_rain_wet_timer>0) {
      player.visible_rain_wet_timer=0;
      rain_duration=0;
      mem_snd_interrupt[4]=TRUE; 
      waveOutReset(hWaveOut[4]);
    }

    if (rain_duration>=channelSoundEffect[3].duration/2) {
      rain_duration=0;
    }
    if (rain_duration==0 && player.rain_wet_timer>0) { //sound effect fast sound effect
      PlayMemSnd(&channelSoundEffect[3],&channelSoundEffectCache[3],TRUE,4); 
    }

    if (rain_duration>0 && player.rain_wet_timer==0) {
      rain_duration=0;
      mem_snd_interrupt[4]=TRUE;
      waveOutReset(hWaveOut[4]);
    }
    rain_duration+=6;

  //soft rain snd act
  } else {
    if (player.rain_wet_timer==60) {
      player.visible_rain_wet_timer=160;
      rain_duration=0;
      mem_snd_interrupt[4]=TRUE; 
      waveOutReset(hWaveOut[4]);
    }
    /*if (rain_duration>0 && player.rain_wet_timer>0) {
      //player.visible_rain_wet_timer=160;
      //player.rain_wet_timer=0;
      mem_snd_interrupt[4]=TRUE; 
      waveOutReset(hWaveOut[4]);
    }*/

    if (rain_duration>=channelSoundEffect[4].duration/2) {
      rain_duration=0;
    }
    if (rain_duration==0 && player.visible_rain_wet_timer>0) { //sound effect fast sound effect
      PlayMemSnd(&channelSoundEffect[4],&channelSoundEffectCache[4],TRUE,4); 
    }

    if (rain_duration>0 && player.visible_rain_wet_timer==0) {
      rain_duration=0;
      mem_snd_interrupt[4]=TRUE;
      waveOutReset(hWaveOut[4]);
    }
    rain_duration+=6;
  }




  if (player.fast_duration>=channelSoundEffect[0].duration/2) {
    player.fast_duration=0;
  }
  if (player.fast_duration==0 && player.speed>10) { //sound effect fast sound effect
    PlayMemSnd(&channelSoundEffect[0],&channelSoundEffectCache[0],TRUE,1); 
  }
  if (player.fast_duration>0 && player.speed<=10) {
    player.fast_duration=0;
    mem_snd_interrupt[1]=TRUE;
    waveOutReset(hWaveOut[1]);
  }
  if (player.shoot_knife_duration==1) { //sound effect knife throw
    PlayMemSnd(&channelSoundEffect[2],&channelSoundEffectCache[2],TRUE,3);
  }

  //sound timers limiter
  //player shooting knife sound limiter
  if (player.shoot_knife_duration>0)
    player.shoot_knife_duration+=6;
  if (player.shoot_knife_duration>330/*341*/) {//special case, old audio has popping noise at the end
    mem_snd_interrupt[3]=TRUE;
    waveOutReset(hWaveOut[3]);
    player.shoot_knife_duration=0;
  }


  //player running fast sound limiter
  if (player.speed>10) {
    player.fast_duration+=6;
  } else {
    player.fast_duration=0;
  }
}


void PlayerCameraShake()
{
  int i;
//camshake
  //if (!the_bravery_tyrant && IsNormalView) {
    double y_bob=0,x_bob=0;
    //if (sprint_bobbing) {  //if sprint_bobbing
      if (player.on_ground_id!=-1) {//not in air, on ground
        if (!player.blocking) {
          if (player.rst_left || player.rst_right || player.health<=PLAYER_LOW_HEALTH) {
	        if (player.speed>=5) {
              x_bob=2.5;
	          //if (bg_cam_fall_cooldown==0) {
	            player.cam_move_x+=0.75*RandNum(-1,1,1);//shaky cam
	            player.cam_move_y+=0.75*RandNum(-1,1,1);
	          //}
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
    if ((player.grav>3 || player.speed>=5) && (!player.is_on_ground_edge)) { //falling cam effect
      y_bob=0;//(player.grav-2)/2;

      switch (player.speed) {
         case 1:
    	 case 2:
           x_bob=RandNum(1,2,1)*0.5;//move x	
    	   break;
    	 case 3:
           x_bob=RandNum(1,2,1);//move x		
    	   break;
      }
      if (player.rst_left) {
	    player.cam_move_x+=x_bob;
      }
      if (player.rst_right) {
	    player.cam_move_x-=x_bob;
      }
      player.cam_move_y-=y_bob;//increase y
      if (player.grav>5 || (player.speed>=5 && (player.rst_left || player.rst_right))) {
        player.cam_move_x+=RandNum(-1,1,1);//shaky x
        player.cam_move_y+=RandNum(-1,1,1);//shaky y
      }
    }
    x_bob=0;
    y_bob=0;
  //}
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



void DrawPlayer(HDC hdc)
{
  //Platform bitmap palette conversion
  if (player.rst_arrow_left) {
    if (mouse_x>0)
      mouse_x-=20;
  } else if (player.rst_arrow_right) {
    if (mouse_x<GR_WIDTH)
      mouse_x+=20;
  }


  if (player.rst_arrow_up) {
    if (mouse_y>0)
      mouse_y-=20;
  } else if (player.rst_arrow_down) {
    if (mouse_y<GR_HEIGHT)
      mouse_y+=20;
  }

  if (player.flag_revert_palette && player.time_breaker_tick<=0) {
    BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
    for (int i=0;i<ENEMY_NUM;i++) {
      if (Enemy[i]->health>0) {
        if (Enemy[i]->species==1) {
          Enemy[i]->saved_angle=Enemy[i]->sprite_angle-1;
        }
        if (EnemySprite[i]->sprite_1!=NULL) {
          BitmapPalette(hdc,EnemySprite[i]->draw_sprite_1.sprite_paint,rgbColorsDefault);
        }
        if (EnemySprite[i]->sprite_2!=NULL) {
          BitmapPalette(hdc,EnemySprite[i]->draw_sprite_2.sprite_paint,rgbColorsDefault);
        }
        if (EnemySprite[i]->sprite_3!=NULL) {
          BitmapPalette(hdc,EnemySprite[i]->draw_sprite_3.sprite_paint,rgbColorsDefault);
        }
      }
    }
    player.flag_revert_palette=FALSE;
    player.time_breaker_tick=0;
  }

  if (player.is_on_ground_edge) {
    int tmp_ground_id=player.on_ground_edge_id;
    if (tmp_ground_id==-1)
      tmp_ground_id=player.saved_on_ground_edge_id;
    if (player.is_on_left_ground_edge) {
      GrLine(hdc,player.sprite_x,player.sprite_y,Ground[tmp_ground_id]->x1+player.cam_x+player.cam_move_x+player.cam_mouse_move_x,Ground[tmp_ground_id]->y1+player.cam_y+player.cam_move_y+player.cam_mouse_move_y,LTCYAN);
    } else if (player.is_on_right_ground_edge) {
      GrLine(hdc,player.sprite_x,player.sprite_y,Ground[tmp_ground_id]->x2+player.cam_x+player.cam_move_x+player.cam_mouse_move_x,Ground[tmp_ground_id]->y2+player.cam_y+player.cam_move_y+player.cam_mouse_move_y,LTCYAN);
    }
  }

  //GrRect(hdc,player.x-PLAYER_WIDTH,player.y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT,RGB(34,139,34));
  //Mathematics
  if (player.on_ground_id!=-1) {
    if (player.print_current_above) {
      player.sprite_angle=player.angle;
    } else if (player.print_current_below) {
      player.sprite_angle=M_PI+player.angle;
    }
    if (!player.last_left) {
      player.sprite_angle*=-1;
    }
  } else {
    if (player.attack_timer>=0) {
      player.sprite_angle=0;
    }
  }

  if (player.time_breaker) {
    if (player.time_breaker_tick<GR_WIDTH || player.time_breaker_tick<GR_HEIGHT) {
      player.time_breaker_tick+=1;
      player.time_breaker_tick+=player.time_breaker_tick;
      if (!IsInvertedBackground()) {
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick,WHITE,-1);
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick-1,WHITE,-1);
      } else {
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick,BLACK,-1);
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick-1,BLACK,-1);
      }
    } else {
      for (int i=0;i<ENEMY_NUM;i++) {
        if (Enemy[i]->health>0) {
        if (EnemySprite[i]->sprite_1!=NULL && !Enemy[i]->time_breaker_immune) {
          BitmapPalette(hdc,EnemySprite[i]->draw_sprite_1.sprite_paint,rgbColorsNoir);
        }
        if (EnemySprite[i]->sprite_2!=NULL && !Enemy[i]->time_breaker_immune) {
          BitmapPalette(hdc,EnemySprite[i]->draw_sprite_2.sprite_paint,rgbColorsNoir);
        }
        if (EnemySprite[i]->sprite_3!=NULL && !Enemy[i]->time_breaker_immune) {
          BitmapPalette(hdc,EnemySprite[i]->draw_sprite_3.sprite_paint,rgbColorsNoir);
        }
        }
      }
      BitmapPalette(hdc,map_platforms_sprite,rgbColorsNoir);
    }
  } else {
    if (player.time_breaker_tick>0) {
      player.time_breaker_tick--;
      player.time_breaker_tick-=player.time_breaker_tick/2;
      if (!IsInvertedBackground()) {
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick,WHITE,-1);
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick-1,WHITE,-1);
      } else {
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick,BLACK,-1);
        GrCircle(hdc,player.sprite_x,player.sprite_y,player.time_breaker_tick-1,BLACK,-1);
      }
    }
  }

  if (player.saved_sprite_angle!=player.sprite_angle && player.on_ground_id!=-1) { //detect chnage in walk sprite angle
    DeleteObject(player.sprite_1_cache);
    DeleteObject(player.sprite_2_cache);
    FreeDrawSprite(&player.draw_sprite_1);
    FreeDrawSprite(&player.draw_sprite_2);

    player.sprite_1_cache = RotateSprite(hdc, player.sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);
    player.sprite_2_cache = RotateSprite(hdc, player.sprite_2,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);

    GenerateDrawSprite(&player.draw_sprite_1,player.sprite_1_cache);
    GenerateDrawSprite(&player.draw_sprite_2,player.sprite_2_cache);

    player.saved_sprite_angle=player.sprite_angle;
  }


  if (player.block_timer>0 && player.saved_block_sprite_angle!=player.sprite_angle) { //detect change in block sprite angle
    DeleteObject(player.block_sprite_1_cache);
    DeleteObject(player.block_sprite_2_cache);
    DeleteObject(player.block_sprite_3_cache);

    FreeDrawSprite(&player.draw_block_sprite_1);
    FreeDrawSprite(&player.draw_block_sprite_2);
    FreeDrawSprite(&player.draw_block_sprite_3);

    player.block_sprite_1_cache = RotateSprite(hdc, player.block_sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);
    player.block_sprite_2_cache = RotateSprite(hdc, player.block_sprite_2,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);
    player.block_sprite_3_cache = RotateSprite(hdc, player.block_sprite_3,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);


    GenerateDrawSprite(&player.draw_block_sprite_1,player.block_sprite_1_cache);
    GenerateDrawSprite(&player.draw_block_sprite_2,player.block_sprite_2_cache);
    GenerateDrawSprite(&player.draw_block_sprite_3,player.block_sprite_3_cache);


    player.saved_block_sprite_angle=player.sprite_angle;
  }
  if (player.attack_timer>=0 && player.saved_attack_sprite_angle!=player.sprite_angle) {//detect change in attack sprite angle
    DeleteObject(player.attack_sprite_1_cache);
    DeleteObject(player.attack_sprite_2_cache);
    DeleteObject(player.attack_sprite_3_cache);
    DeleteObject(player.attack_sprite_4_cache);

    FreeDrawSprite(&player.draw_attack_sprite_1);
    FreeDrawSprite(&player.draw_attack_sprite_2);
    FreeDrawSprite(&player.draw_attack_sprite_3);
    FreeDrawSprite(&player.draw_attack_sprite_4);


    player.attack_sprite_1_cache = RotateSprite(hdc, player.attack_sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);
    player.attack_sprite_2_cache = RotateSprite(hdc, player.attack_sprite_2,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);
    player.attack_sprite_3_cache = RotateSprite(hdc, player.attack_sprite_3,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);
    player.attack_sprite_4_cache = RotateSprite(hdc, player.attack_sprite_4,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player.load_color],-1);

    GenerateDrawSprite(&player.draw_attack_sprite_1,player.attack_sprite_1_cache);
    GenerateDrawSprite(&player.draw_attack_sprite_2,player.attack_sprite_2_cache);
    GenerateDrawSprite(&player.draw_attack_sprite_3,player.attack_sprite_3_cache); 
    GenerateDrawSprite(&player.draw_attack_sprite_4,player.attack_sprite_4_cache);

    player.saved_attack_sprite_angle=player.sprite_angle;
  }
  bool is_blink=TRUE;
  if (player.speed>24 && frame_tick%2==0) {
    is_blink=FALSE;
  }
  if (is_blink) {
    if (player.attack_timer==-1) { //not attacking
      if (player.block_timer==0) { //not blocking
        if (player.on_ground_timer>0) { // on ground
          if (player.walk_cycle<2) {
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_sprite_1,player.last_left);
          } else {
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_sprite_2,player.last_left);
          }
        } else { //in_air
          if (!player.is_on_ground_edge) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,player.blur_sprite_x[j],player.blur_sprite_y[j],&player.draw_blur_sprite_jump,player.last_left);
            }
          }
          DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_sprite_jump,player.last_left);
        }
      } else { //blocking
        if (player.on_ground_id==-1 && player.spin_timer>0) { //not on ground
          if (player.spin_timer<10) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,player.blur_sprite_x[j],player.blur_sprite_y[j],&player.draw_spin_blur_sprite_1,!player.last_left);
            }
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_spin_sprite_1,!player.last_left);
          } else if (player.spin_timer>10 && player.spin_timer<20) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,player.blur_sprite_x[j],player.blur_sprite_y[j],&player.draw_spin_blur_sprite_2,!player.last_left);
            }
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_spin_sprite_2,!player.last_left);
          } else if (player.spin_timer>20 && player.spin_timer<30) {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,player.blur_sprite_x[j],player.blur_sprite_y[j],&player.draw_spin_blur_sprite_3,!player.last_left);
            }
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_spin_sprite_3,!player.last_left);
          } else {
            for (int j=0;j<PLAYER_BLUR_NUM;j++) {
              DrawSprite(hdc,player.blur_sprite_x[j],player.blur_sprite_y[j],&player.draw_spin_blur_sprite_4,!player.last_left);
            }
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_spin_sprite_4,!player.last_left);
          }
        } else { //on ground
          if (0<player.block_timer && player.block_timer<=5) {
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_block_sprite_1,player.last_left);
          } else if (5<player.block_timer && player.block_timer<=10) {
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_block_sprite_2,player.last_left);
          } else {
            DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_block_sprite_3,player.last_left);
          }
        }
      }
    } else {
      if (!(player.on_ground_timer>0)) {
        for (int j=0;j<PLAYER_BLUR_NUM;j++) {
          DrawSprite(hdc,player.blur_sprite_x[j],player.blur_sprite_y[j],&player.draw_blur_sprite_jump,player.last_left);
        }
      }
      if (30<player.attack_timer && player.attack_timer<=40) {//attack sprite
        DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_attack_sprite_1,player.last_left);
      } else if (20<player.attack_timer && player.attack_timer<=30) {
        DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_attack_sprite_3,player.last_left);
      } else if (10<player.attack_timer && player.attack_timer<=20) {
        DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_attack_sprite_3,player.last_left);
      } else if (-1<player.attack_timer && player.attack_timer<=10) {
        DrawSprite(hdc,player.sprite_x,player.sprite_y,&player.draw_attack_sprite_4,player.last_left);
      }
    }
  }

  //Shapes Drawn when swinging to show direction of swing
  int color=Highlight(IsInvertedBackground(),WHITE,BLACK);
  if (player.is_swinging) {
    GrLine(hdc,player.sprite_x,
               player.sprite_y,
                player.pivot_x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x,
                player.pivot_y+player.cam_y+player.cam_move_y+player.cam_mouse_move_y,
                color);
    //line showing where player will launc/fling to
    /*GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x,
                 player.pivot_y+player.cam_y+player.cam_move_y+player.cam_mouse_move_y,
                DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,color,-1);
    GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+100*cos(player.angle_of_incidence),player.sprite_y+100*sin(player.angle_of_incidence),LTGREEN);*/
  }

  //Draw the bullet the player shoots
  if (player.bullet_shot!=-1) {
    if (player.right_click_hold_timer<62) {
      DrawBullet(hdc,player.bullet_shot);
      GrLine(hdc,player.sprite_x,player.sprite_y,Bullet[player.bullet_shot].sprite_x,Bullet[player.bullet_shot].sprite_y,color);    
    }
  }
  
  //GrSprite(hdc,player.sprite_x,player.sprite_y,canny,player.last_left);

  for (int i=0;i<player.bullet_shot_num;i++) {
    DrawBullet(hdc,player.bullet[i]);
  }

  for (int i=0;i<PLAYER_FLING_WEB_NUM;i++) {
    GrCircle(hdc,player_fling_web.sprite_x[i],player_fling_web.sprite_y[i],2,color,color);
  }


  /*char hi[5];
  sprintf(hi,"%d",player.on_ground_timer);
  GrPrint(hdc,player.sprite_x,player.sprite_y-30,hi,BLACK);*/

  /*char hi2[5];
  int tmp_id=GetOnGroundIdPlayer(player.x,player.y,5,4);
  sprintf(hi2,"%d",tmp_id);
  GrPrint(hdc,player.sprite_x,player.sprite_y-50,hi2,BLACK);


  char hi3[5];
  int tmp_id2=player.saved_ground_id;//GetOnGroundIdPlayer(player.x,player.y,30,29);
  sprintf(hi3,"%d",tmp_id2);
  GrPrint(hdc,player.sprite_x,player.sprite_y-70,hi3,BLACK);*/
}

