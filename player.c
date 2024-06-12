
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
  /*for (int i=0;i<player.rendered_enemy_num;i++) {
    player_render_enemies[i]=-1;
  }*/
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
  CameraInit(player.saved_x,player.saved_y+PLAYER_HEIGHT/2); //idk scaling is weird for sprite
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
}


void PlayerBulletLimitAct()
{
  /*if (player.max_web_num-player.placed_web_num>=3 && player.knives_per_throw==5) {
    player.knives_per_throw=13;
  }*/
  if (player.max_web_num-player.placed_web_num>=3) {          
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,PLAYER_BULLET_NUM+1); //limit to 1,3,5,15
  } else if (player.max_web_num-player.placed_web_num>0){ //limit to 1,3,5
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,6);
  } else { //limit to 1,3
    player.knives_per_throw=LimitValue(player.knives_per_throw,1,4);
  }

}



void InitPlayer() {
  int i;
  player.hiding=FALSE;
  player.rst_left_click=FALSE;
  player.rst_right_click=FALSE;
  player.left_click=FALSE;
  player.right_click=FALSE;
  player.rst_down=FALSE;
  player.rst_left=FALSE;
  player.rst_right=FALSE;
  player.rst_up=FALSE;
  player.rst_key_sprint=FALSE;
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

  player.grav=2;
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
  player.key_jump_timer=0;
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
  player.block_recharge_timer_max=300;
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
  

  InitPlayerCamera();
}




void RegainWeb(int web_id)
{
  while (player.web_storage[player.destroyed_web_pos]!=-1) {//prevent overwrite of existing
    player.destroyed_web_pos=LimitValue(player.destroyed_web_pos+1,0,player.max_web_num);
  }
  player.web_storage[player.destroyed_web_pos]=web_id;
  player.destroyed_web_pos=LimitValue(player.destroyed_web_pos+1,0,player.max_web_num);
  player.placed_web_num--;
}





void PlayerAct() {
  int i=0,speed=0,grav_speed=0,claws_l=NODE_SIZE,web_id=0;
  double cur_dist=0,cur_angle=0,grad_x1=0,grad_y1=0,grad_x2=0,grad_y2=0;
  double x1,x2,y1,y2;
  bool allow_act=FALSE;


  //========Player attacking timer==============
  if (player.attack_timer>=0) {
    if (player.speed>10) {
      player.attack_strength=4;
    } else {
      if (player.speed>5) {
        player.attack_strength=2;
      } else {
        player.attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;
      }
    }

    if (player.grav>10 || player.speed>24) {
      player.attack_strength*=2;
    }
    player.attack_timer--;
  }
  //======================

  //Sprinting
  if (player.rst_key_sprint) {
    if (player.on_ground_id!=-1) {//on ground
      if (player.speed<3) 
        player.speed++;
    }
  }


  //Clicking Actions
  if (player.bullet_shot!=-1) {
    player.web_being_shot=player.web_storage[player.placed_web_pos];
    BulletAct(player.bullet_shot);
  }
  for (int i=0;i<player.bullet_shot_num;i++) {
    BulletAct(player.bullet[i]);
  }

  Click();
  if (player.left_click_hold_timer==62 || player.attack_rst) {//Left click to Attack
    player.attack=TRUE;
    player.blocking=FALSE; //unblock

    if (player.bullet_shot!=-1) {
      StopBullet(player.bullet_shot,TRUE); //stop all web bullets from acting
      player.web_being_shot=-1;
      player.bullet_shot=-1;
    }

    int b_speed_m=5;
    int b_dmg_m=1;
    if (player.speed>10) {
      b_speed_m=9;
      b_dmg_m=3;
    } else if (player.speed>5) {
      b_speed_m=7;      
      b_dmg_m=2;
    }

    if (player.speed>24)
      b_dmg_m=4;


    if (player.bullet_shot_num<PLAYER_BULLET_NUM && 
        !player.is_swinging && 
        (PLAYER_BULLET_NUM-player.bullet_shot_num>=player.knives_per_throw) // a/b whehere a>=b a is bullet in storage, b is bullet consumption
    ) {
      grad_x1=player.x+player.cam_move_x;
      grad_y1=player.y+player.cam_move_y;
      grad_x2=mouse_x-player.cam_x;
      grad_y2=mouse_y-player.cam_y;
      double tmp_angle=0;
      int b_g_type=5;

      if (player.max_web_num-player.placed_web_num<3) {  //0,1,3,5
        player.knives_per_throw=LimitValue(player.knives_per_throw,1,6);
      } 
      if (player.max_web_num-player.placed_web_num<1) {  //0,1,3
        player.knives_per_throw=LimitValue(player.knives_per_throw,1,4);
      }

      if (player.knives_per_throw>4) {
        b_g_type=6;
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

      //Bullet[current_bullet_id].playsnd=TRUE;
        player.bullet_shot_num++;
        current_bullet_id++;
        if (current_bullet_id>=BULLET_NUM-1) {
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
    if (allow_act) {
      grad_x1=player.x+player.cam_move_x;
      grad_y1=player.y+player.cam_move_y;
      grad_x2=mouse_x-player.cam_x;
      grad_y2=mouse_y-player.cam_y;

      player.attack=TRUE; 
	  player.bullet_shot=current_bullet_id;
      ShootBullet(current_bullet_id,
	-1,
	CYAN,
    5, //graphics type
	MAX_WEB_LENGTH, //range
    1, //speed
	15+player.speed*2, //speed multiplier
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
      if (current_bullet_id>=BULLET_NUM-1) {
        current_bullet_id=0;
      } 
    }
  } else { //meelee attack only
    if (player.is_swinging) {
      if (player.left_click_hold_timer==62 || player.attack_rst || player.right_click_hold_timer==62) { //swing but no web is placed
        player.is_swinging=FALSE;
        if (player.uppercut) {
          player.fling_distance=0;
        } else {
          player.fling_distance=player.pivot_length;
        }
        player.key_jump_timer=player.player_jump_height;
        player.speed+=2;
        player.in_air_timer=1000;
        player.grav=3;


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
        double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
        if (player.x<player.pivot_x) {
          bm_x2=player.pivot_x;
          bm_y2=player.pivot_y;
          bm_x1=player.above_x;
          bm_y1=player.above_y;	
        } else {
          bm_x1=player.pivot_x;
          bm_y1=player.pivot_y;
          bm_x2=player.above_x;
          bm_y2=player.above_y;
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
            PlayerPlaceWeb();
            PlayerBulletLimitAct();
            Ground[web_id]->health=150;//-q;
            player.speed++;
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
  if (player.rst_up && 6<=player.on_ground_timer && player.on_ground_timer<=20 && player.fling_distance==0 /*&& player.key_jump_timer==0*/) {
    player.jump=TRUE;
    player.key_jump_timer=player.player_jump_height;
  }
  if (player.jump && player.jump_height==0) {
    player.jump_height=player.player_jump_height;
  }



  for (speed=0;speed<player.speed;speed++) {
    for (grav_speed=0;grav_speed<player.grav;grav_speed++) {
      player.on_ground_id=GetOnGroundIdPlayer(player.x,player.y,5,4);
      //player.on_ground_id=GetOnGroundId(player.x,player.y,5,4,TRUE);

   //hiding?    (legacy feature)
      if (NodeGrid[GetGridId(player.above_x,player.above_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)]->node_solid) {
        player.hiding=TRUE;
      } else {
        player.hiding=FALSE;
      }


   //Destroy Ground (regainable)
      if (player.destroy_ground) {
        if (player.on_ground_id>=GROUND_NUM && player.on_ground_id!=player.previous_web_placed) {
          DestroyGround(player.on_ground_id);  
    	  RegainWeb(player.on_ground_id);
        }
        player.destroy_ground=FALSE;
      }


   //Ground action
   //on a ground
      if (player.on_ground_id!=-1 && player.on_ground_id!=player.previous_web_placed) {

        //get dist between player and ground
        double ground_entity_E=GetLineTargetAngle(player.on_ground_id,player.x,player.y);
        double height_from_player_x=GetLineTargetHeight(player.on_ground_id,ground_entity_E,player.x,player.y);


        //player speed when on ground
        if (!IsSpeedBreaking()) {//reset stats when normal
          player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
        } else {
          player.player_jump_height=150;
          if (player.speed<5) {
            player.speed++;
          }
        }


        //player speed when on ground
        if (!player.is_swinging && player.fling_distance==0 && player.on_ground_timer>=1 && speed==0 && grav_speed==0) {
          if (!player.is_rebounding) {
            if (!IsSpeedBreaking()) {//reset stats when normal            
              if (!player.rst_key_sprint) {
                if (player.speed>DEFAULT_PLAYER_SPEED)
                  player.speed--;
              } else {
                if (player.speed>8)
                  player.speed--;
              }
            } else {
              if (player.speed>8)
                player.speed--;
            }
          }
        }

        if (player.health<=PLAYER_LOW_HEALTH) {
          if (player.speed<6) {
            player.speed=7;
          }
        }

        if (player.rst_key_sprint || IsSpeedBreaking() || player.time_breaker) {
          player.player_jump_height+=player.speed*NODE_SIZE;
        }


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
              player.speed--; //lower speed on landing
            }
            if (player.rst_down) { //spinning
              player.is_rebounding=TRUE;
              if (player.rst_left || player.rst_right) { //continue spin-rebound
                player.in_air_timer=1000;
              }
            } else { //not reboubding
              player.is_rebounding=FALSE;
              player.in_air_timer=1;

              player.key_jump_timer=0;//player.jump_height/2;
              player.jump_height=0; //Stop Jump & stick to ground
              player.jump=FALSE;
            }
            player.in_air_timer--;
          }

        //outwards from ground
          if (0<=height_from_player_x && height_from_player_x<10) { //above ground
            player.current_above=TRUE;
            player.previous_above=TRUE;
            player.previous_below=FALSE;
            player.on_a_ground=TRUE;
            player.on_ground_timer=20;
            if (height_from_player_x<5 || player.is_rebounding || player.is_swinging) {
              move_x(-cos(player.angle+M_PI_2));
              move_y(-sin(player.angle+M_PI_2));
            }
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
          } else if (-10<height_from_player_x && height_from_player_x<0) { //below ground
            player.current_below=TRUE;
            player.previous_above=FALSE;
            player.previous_below=TRUE;
            player.on_a_ground=TRUE;
            player.on_ground_timer=20;
            if (height_from_player_x>-5 || player.is_rebounding || player.is_swinging) {
              move_x(-cos(player.angle-M_PI_2));
              move_y(-sin(player.angle-M_PI_2));
            }
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

          if (height_from_player_x>10 || height_from_player_x<-10) {//leave ground when out of circular range
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
        if (player.on_ground_id==-1 && !player.is_swinging) {
          player.in_air_timer++;
          if (player.in_air_timer>2002) { //dont go above this limit
            player.in_air_timer--;
          }
	      if (player.jump_height==0) {
            if (player.in_air_timer%20==0 && player.grav<=100) {
              player.grav++;
              if (player.is_rebounding && player.speed<4) {
                player.speed++;
              }
            }
	      }
        }
      //Jump movement
        if (player.key_jump_timer>0) {
          player.key_jump_timer--;
        } else {
          player.jump_height=0;
          player.jump=FALSE;
        }
      }


      //PLAYER GRAVITY MOVEMENT

      if (speed==0) {
        if (player.jump_height>0) { //Jumping action
          player.jump_height-=player.player_grav;
          move_x(2*player.player_grav*-cos(player.jump_angle));
          move_y(2*player.player_grav*-sin(player.jump_angle)); //jump go against gravity and perpendicular from platform
          if (player.jump_height<=0) {
            player.jump=FALSE;
          }          
        }




        if (player.on_ground_id==-1 && player.jump_height<=0) { //Credit: y4my4m for pushing me to pursue this gameplay aspect
          if (!player.is_swinging) { //not swinigng and player is not flinging
            if (player.in_air_timer>21 || player.fling_distance<0) {
              move_y(player.player_grav); //include while being rebounding and flinging
	        } else {
              move_x(-player.player_grav*cos(player.jump_angle));
              move_y(player.player_grav*sin(player.jump_angle));
            }
          }
        } else { //landed on ground
	      if (player.grav>=3 && !player.is_swinging) {
	        if (player.grav>7) {
	          player.cam_move_x*=-1;
	        }
	        player.cam_move_y/=-1.5;
	      }
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
        if (!player.is_swinging) { // player is not swinging
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


      //====PLAYER CIRCULAR WEB SWINGING MOVEMENT======
      if (player.is_swinging && grav_speed<=1) {
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

        if (grav_speed==0) {
          if (player.on_ground_id==-1) {
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
              } else if (player.rst_up && player.on_ground_id==-1 && player.pivot_length>2){ //Retract Web
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
              } else if (player.rst_up && player.on_ground_id==-1 && player.pivot_length>2){ //Retract web
                move_x(-cos(-player.pivot_angle));
                move_y(-sin(-player.pivot_angle));
              } else if (player.rst_down && player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) {
                move_x(cos(-player.pivot_angle));
                move_y(sin(-player.pivot_angle));
              }
            }
          }
        } else if (grav_speed==1) {//only occurs right after grav_speed==0
          if (player.pivot_length>NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2 && !player.on_a_ground) { //rubber band back if pivot length too long
            if (player.y>player.pivot_y) {
              move_x(-cos(-player.pivot_angle));
              move_y(sin(-player.pivot_angle));
            } else {
              move_x(-cos(-player.pivot_angle));
              move_y(-sin(-player.pivot_angle));
            }
          }       
        } //End of player swinging movement
      } //end of grav==0



      //======FLING MOVEMENT======
      if (grav_speed==0 && !player.is_swinging && !player.is_rebounding) { 
        if ((player.rst_left || player.rst_right) && (player.uppercut || player.fling_distance<-100)) { //cancel flinging when left or right key is pressed
          player.fling_distance=0;
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




     //x-axis cap
      if (player.x-PLAYER_WIDTH/2<0) {
        move_x(1);
      } else if (player.x+PLAYER_WIDTH/2>MAP_WIDTH) {
        move_x(-1);
      }


     //misc
      player.print_current_above=player.current_above;
      player.print_current_below=player.current_below;
      player.current_above=FALSE;
      player.current_below=FALSE;
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
      if (player.on_ground_id==-1 || player.print_current_above) { //in air or on above ground
        allow_act=TRUE;
      } else if (player.print_current_below) { // below ground
        allow_act=TRUE;
      }
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
    if (player.on_ground_id==-1) { //player jumping && blocking
      if (player.speed<3)
        player.speed++;
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
        player.time_breaker_units--;
        if (player.time_breaker_units%3==0)
          player.speed++;
      }
      player.time_breaker_recharge_timer=player.time_breaker_recharge_timer_max;
      if (player.speed<10) {
        player.time_breaker_cooldown=player.time_breaker_cooldown_max;
      } else {
        if (player.speed<24) {
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
      } else if (player.time_breaker_units<player.time_breaker_units_max) {
        player.time_breaker_units++;
        if (player.speed<10) {
          player.time_breaker_recharge_timer=player.time_breaker_recharge_timer_max;
        } else {
          if (player.speed<24) {
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
            PlaySound(L"snd/timebreaker__stop.wav", NULL, SND_FILENAME | SND_ASYNC);
          }
        }
      }
    }
  }

  //swinging
  if (player.is_swinging && player.speed<5 && player.on_ground_id==-1) { //fast when swinging
    player.speed++;
  }

  if (player.fling_distance>0  && player.speed<5) {
    player.speed++;
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



  //sprite axes
  player.sprite_x=GR_WIDTH/2+player.cam_move_x;
  player.sprite_y=GR_HEIGHT/2+player.cam_move_y-PLAYER_HEIGHT/2;
 //
}


void PlayerSndAct()
{
  wchar_t my_status[16];
  if (player.speed>10) {
    mciSendString(L"open snd/fast.wav alias player_speed",NULL,0,NULL);
    mciSendString(L"status player_speed mode",my_status,16,NULL);
    if (_wcsicmp(my_status,L"stopped")==0) {
      mciSendString(L"seek player_speed to start",NULL,0,NULL);
      mciSendString(L"play player_speed",NULL,0,NULL);
    }
  } else {
    mciSendString(L"pause player_speed",NULL,0,NULL);
    mciSendString(L"close player_speed",NULL,0,NULL);
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
          if (player.rst_left || player.rst_right) {
	        if (player.rst_key_sprint || player.speed>=5) {
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
    if (player.grav>3 || player.speed>=5) { //falling cam effect
      y_bob=(player.grav-2)/2;

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
      if (player.grav>5 || player.speed>=5) {
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
  if (player.flag_revert_palette && player.time_breaker_tick<=0) {
    BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
    for (int i=0;i<ENEMY_NUM;i++) {
      if (Enemy[i]->health>0) {
        if (Enemy[i]->species==1) {
          Enemy[i]->saved_angle=Enemy[i]->sprite_angle-1;
        }
        if (EnemySprite[i].sprite_1!=NULL) {
          BitmapPalette(hdc,EnemySprite[i].sprite_1,rgbColorsDefault);
        }
        if (EnemySprite[i].sprite_2!=NULL) {
          BitmapPalette(hdc,EnemySprite[i].sprite_2,rgbColorsDefault);
        }
        if (EnemySprite[i].sprite_3!=NULL) {
          BitmapPalette(hdc,EnemySprite[i].sprite_3,rgbColorsDefault);
        }
      }
    }
    player.flag_revert_palette=FALSE;
    player.time_breaker_tick=0;
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
    //GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,WHITE,-1);
      if (!IsInvertedBackground()) {
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick,WHITE,-1);
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick-1,WHITE,-1);
      } else {
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick,BLACK,-1);
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick-1,BLACK,-1);
      }
    } else {
      for (int i=0;i<ENEMY_NUM;i++) {
        if (Enemy[i]->health>0) {
        if (EnemySprite[i].sprite_1!=NULL && !Enemy[i]->time_breaker_immune) {
          BitmapPalette(hdc,EnemySprite[i].sprite_1,rgbColorsNoir);
        }
        if (EnemySprite[i].sprite_2!=NULL && !Enemy[i]->time_breaker_immune) {
          BitmapPalette(hdc,EnemySprite[i].sprite_2,rgbColorsNoir);
        }
        if (EnemySprite[i].sprite_3!=NULL && !Enemy[i]->time_breaker_immune) {
          BitmapPalette(hdc,EnemySprite[i].sprite_3,rgbColorsNoir);
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
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick,WHITE,-1);
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick-1,WHITE,-1);
      } else {
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick,BLACK,-1);
        GrCircle(hdc,player.x+player.cam_x+player.cam_move_x,player.y+player.cam_y+player.cam_move_y,player.time_breaker_tick-1,BLACK,-1);
      }
    }
  }

  if (player.saved_sprite_angle!=player.sprite_angle && player.on_ground_id!=-1) { //detect chnage in walk sprite angle
    DeleteObject(player.sprite_1_cache);
    DeleteObject(player.sprite_2_cache);
    player.sprite_1_cache = RotateSprite(hdc, player.sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.sprite_2_cache = RotateSprite(hdc, player.sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.saved_sprite_angle=player.sprite_angle;
  }


  if (player.block_timer>0 && player.saved_block_sprite_angle!=player.sprite_angle) { //detect change in block sprite angle
    DeleteObject(player.block_sprite_1_cache);
    DeleteObject(player.block_sprite_2_cache);
    DeleteObject(player.block_sprite_3_cache);

    player.block_sprite_1_cache = RotateSprite(hdc, player.block_sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.block_sprite_2_cache = RotateSprite(hdc, player.block_sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.block_sprite_3_cache = RotateSprite(hdc, player.block_sprite_3,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);

    player.saved_block_sprite_angle=player.sprite_angle;
  }
  if (player.attack_timer>=0 && player.saved_attack_sprite_angle!=player.sprite_angle) {//detect change in attack sprite angle
    DeleteObject(player.attack_sprite_1_cache);
    DeleteObject(player.attack_sprite_2_cache);
    DeleteObject(player.attack_sprite_3_cache);
    DeleteObject(player.attack_sprite_4_cache);

    player.attack_sprite_1_cache = RotateSprite(hdc, player.attack_sprite_1,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.attack_sprite_2_cache = RotateSprite(hdc, player.attack_sprite_2,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.attack_sprite_3_cache = RotateSprite(hdc, player.attack_sprite_3,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
    player.attack_sprite_4_cache = RotateSprite(hdc, player.attack_sprite_4,player.sprite_angle,LTGREEN,draw_color_arr[player.load_color],-1);
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
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_1_cache,player.last_left);
          } else {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_2_cache,player.last_left);
          }
        } else { //in_air
          GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_jump_cache,player.last_left);
        }
      } else { //blocking
        if (player.on_ground_id==-1 && player.spin_timer>0) { //not on ground
          if (player.spin_timer<10) {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.spin_sprite_1_cache,!player.last_left);
          } else if (player.spin_timer>10 && player.spin_timer<20) {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.spin_sprite_2_cache,!player.last_left);
          } else if (player.spin_timer>20 && player.spin_timer<30) {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.spin_sprite_3_cache,!player.last_left);
          } else {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.spin_sprite_4_cache,!player.last_left);
          }
        } else { //on ground
          if (0<player.block_timer && player.block_timer<=5) {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.block_sprite_1_cache,player.last_left);
          } else if (5<player.block_timer && player.block_timer<=10) {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.block_sprite_2_cache,player.last_left);
          } else {
            GrSprite(hdc,player.sprite_x,player.sprite_y,player.block_sprite_3_cache,player.last_left);
          }
        }
      }
    } else {
      if (30<player.attack_timer && player.attack_timer<=40) {//attack sprite
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.attack_sprite_1_cache,player.last_left);
      } else if (20<player.attack_timer && player.attack_timer<=30) {
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.attack_sprite_2_cache,player.last_left);
      } else if (10<player.attack_timer && player.attack_timer<=20) {
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.attack_sprite_3_cache,player.last_left);
      } else if (-1<player.attack_timer && player.attack_timer<=10) {
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.attack_sprite_4_cache,player.last_left);
      }
    }
  }

  //Shapes Drawn when swinging to show direction of swing
  if (player.is_swinging) {
    GrLine(hdc,player.above_x+player.cam_x+player.cam_move_x,player.above_y+player.cam_y+player.cam_move_y,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,LTCYAN);
    int color=BLACK;
    if (!IsInvertedBackground()) {
      color=WHITE;
    }
    //line showing where player will launc/fling to
    GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,color,-1);
    GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+100*cos(player.angle_of_incidence),player.sprite_y+100*sin(player.angle_of_incidence),LTGREEN);
  }

  //Draw the bullet the player shoots
  if (player.bullet_shot!=-1) {
    if (player.right_click_hold_timer<62) {
      DrawBullet(hdc,player.bullet_shot);
      GrLine(hdc,player.above_x+player.cam_x+player.cam_move_x,player.above_y+player.cam_y+player.cam_move_y,Bullet[player.bullet_shot].sprite_x,Bullet[player.bullet_shot].sprite_y,LTCYAN);    
    }
  }
  
  //GrSprite(hdc,player.sprite_x,player.sprite_y,canny,player.last_left);

  for (int i=0;i<player.bullet_shot_num;i++) {
    DrawBullet(hdc,player.bullet[i]);
  }

  /*char hi[5];
  sprintf(hi,"%d",player.on_ground_timer);
  GrPrint(hdc,player.sprite_x,player.sprite_y-30,hi,BLACK);*/

  /*char hi2[5];
  int tmp_id=GetOnGroundIdPlayer(player.x,player.y,5,4);
  sprintf(hi2,"%d",tmp_id);
  GrPrint(hdc,player.sprite_x,player.sprite_y-50,hi2,BLACK);


  char hi3[5];
  int tmp_id2=GetOnGroundIdPlayer(player.x,player.y,30,29);
  sprintf(hi3,"%d",tmp_id2);
  GrPrint(hdc,player.sprite_x,player.sprite_y-70,hi3,BLACK);*/
}

