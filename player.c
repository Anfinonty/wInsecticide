
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



void InitRDGrid()
{
  RDGrid.x1=player.x-(GR_WIDTH/2)-2*GRID_SIZE;//top left corner x
  RDGrid.y1=player.y-(GR_HEIGHT/2)-2*GRID_SIZE;//top left corner y
  RDGrid.x2=player.x+(GR_WIDTH/2)+2*GRID_SIZE;//down right corner x
  RDGrid.y2=player.y+(GR_HEIGHT/2)+2*GRID_SIZE;//down right cornder y
  for (int i=0;i<player.rendered_enemy_num;i++) {
    player_render_enemies[i]=-1;
  }
  player.rendered_enemy_num=0;

  for (int i=0;i<ENEMY_NUM;i++) {
    if (Enemy[i].x>RDGrid.x1 && Enemy[i].x<RDGrid.x2 && Enemy[i].y>RDGrid.y1 && Enemy[i].y<RDGrid.y2) {
      player_render_enemies[player.rendered_enemy_num]=i;
      player.rendered_enemy_num++;
    }
  }
}


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

bool NearCrawler()
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


void CleanUpPlayer()
{
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
  player.saved_ground_id=-1;
  player.walk_cycle=0;
  player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
  player.key_jump_timer=0;
  player.time_breaker_tick=0;
  player.color=player_color; //displayed color
  player.load_color=player_load_color; //orginal color

  player.rendered_vgrid_num=0;
  for (i=0;i<VRDGRID_NUM;i++) {
    player.render_vgrids[i]=-1;
  }

  player.rendered_enemy_num=0;
  for (i=0;i<MAX_ENEMY_NUM;i++) {
    player_render_enemies[i]=-1;
  }

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
  player.potential_energy=-20;
  player.counter_potential_energy=-20;
  player.launch_angle=0;
  player.fling_distance=0;
  player.fling_left=FALSE;
  player.previous_web_placed=-1;

  player.is_rebounding=FALSE;


  player.angle_of_incidence=0;
  player.angle_of_reflection=0;

  player.rebound_above=FALSE;
  player.rebound_below=FALSE;

  player.spin_timer=0;
  

  InitPlayerCamera();
  InitRDGrid();
}

bool YesInitRDGrid()
{
  if (GRID_SIZE*2<player.x && player.x<MAP_WIDTH-GRID_SIZE*2) {
    if (player.x<RDGrid.x1+GRID_SIZE*2 || player.x>RDGrid.x2-GRID_SIZE*2) {
      return TRUE;
    }
  }
  if (GRID_SIZE*2<player.y && player.y<MAP_HEIGHT-GRID_SIZE*2) {
    if (player.y<RDGrid.y1+GRID_SIZE*2 || player.y>RDGrid.y2-GRID_SIZE*2) {
      return TRUE;
    }
  }
  return FALSE;
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
  bool allow_act=FALSE;
  //Initialize RD Grid
  if (YesInitRDGrid()) {
    InitRDGrid();
  }


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


    if (player.bullet_shot_num<PLAYER_BULLET_NUM && !player.is_swinging) {
      grad_x1=player.x+player.cam_move_x;
      grad_y1=player.y+player.cam_move_y;
      grad_x2=mouse_x-player.cam_x;
      grad_y2=mouse_y-player.cam_y;

	  player.bullet[player.bullet_shot_num]=current_bullet_id;
      ShootBullet(current_bullet_id,
	player.bullet_shot_num,
	player_bullet_color,
    5, //graphics type
	DEFAULT_PLAYER_BUILD_RANGE+player.speed*2, //range
    1, //speed
	5, //speed multiplier
	1, //damage
	-2,
	player.x,//player.above_x2, //so it doest get stuck to ground
	player.y,//player.above_y2,
	grad_x1,
	grad_y1,
	grad_x2,
	grad_y2
      );
      player.bullet_shot_num++;
      current_bullet_id++;
      if (current_bullet_id>=BULLET_NUM-1) {
        current_bullet_id=0;
      }
    }
  }

  if (player.right_click_hold_timer==62) {//Right click to Shoot Agsin
    if (player.bullet_shot!=-1 && !player.is_swinging) {
      StopBullet(player.bullet_shot,TRUE);
      player.web_being_shot=-1;
      player.bullet_shot=-1;
    }
  }


  if (!player.attack_rst && !player.is_swinging) {
    allow_act=FALSE;
    if (player.right_click_hold_timer==62 /*&&  //Right click to shoot
        !NearCrawler()*/) {
      if (player.placed_web_num<player.max_web_num && //webs > 0 
        player.bullet_shot==-1) {
        allow_act=TRUE;
      }
    }
    if (allow_act) {
      //int pivot_node_grid_id=0,edge_node_grid_id=0;
      //int edge_x=0,edge_y=0;
      //double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0,bm_gradient=0,pivot_x=0,pivot_y=0;
      grad_x1=player.x+player.cam_move_x;
      grad_y1=player.y+player.cam_move_y;
      grad_x2=mouse_x-player.cam_x;
      grad_y2=mouse_y-player.cam_y;
      //edge_node_grid_id=GetGridId(mouse_x+player.cam_x+player.cam_move_x,mouse_y+player.cam_y+player.cam_move_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);


      player.attack=TRUE; 
	  player.bullet_shot=current_bullet_id;
      ShootBullet(current_bullet_id,
	-1,
	CYAN,
    2, //graphics type
	DEFAULT_PLAYER_BUILD_RANGE+player.speed*2, //range
    1, //speed
	10+player.speed, //speed multiplier
	1, //damage
	-1,
	player.x,//player.above_x2, //so it doest get stuck to ground
	player.y,//player.above_y2,
	grad_x1,
	grad_y1,
	grad_x2,
	grad_y2
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
        player.fling_distance+=player.pivot_length*2;
        player.key_jump_timer=player.player_jump_height;
        player.speed+=2;
        player.in_air_timer=1000;
        player.grav=3;
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
          bm_x1=player.x;
          bm_y1=player.y;	
        } else {
          bm_x1=player.pivot_x;
          bm_y1=player.pivot_y;
          bm_x2=player.x;
          bm_y2=player.y;
        }
        if (player.placed_web_pos<player.max_web_num) { //if pointer to web is less than the no. of webs player has currently     
          while (player.web_storage[player.placed_web_pos]==-1) { //find player.web_storage that is not empty
            player.placed_web_pos=LimitValue(player.placed_web_pos+1,0,player.max_web_num); //reset back to 0 if over the max
          }
          int web_id=player.web_storage[player.placed_web_pos];
          if (web_id!=-1) {
            player.previous_web_placed=web_id;
            Ground[web_id].x1=bm_x1;
            Ground[web_id].y1=bm_y1;
            Ground[web_id].x2=bm_x2;
            Ground[web_id].y2=bm_y2;
            SetGround(web_id);
            SetNodeGridAttributes(web_id);
            PlayerPlaceWeb();
            Ground[web_id].health=150;//-q;
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
  if (player.rst_up && 1<=player.on_ground_timer && player.on_ground_timer<=10 && player.fling_distance<=0) {
    player.jump=TRUE;
    player.key_jump_timer=player.player_jump_height;
  }
  if (player.jump && player.jump_height==0) {
    player.jump_height=player.player_jump_height;
  }


 //Get player grid id
 //player.speed
  for (speed=0;speed<player.speed;speed++) {
    for (grav_speed=0;grav_speed<player.grav;grav_speed++) {
//      player.on_ground_id=GetOnGroundId(player.x,player.y,5,4,TRUE);    //Get Ground id
      player.on_ground_id=GetOnGroundId(player.x,player.y,4,3,TRUE);    //Get Ground id

   //hiding?
      if (NodeGrid[GetGridId(player.above_x,player.above_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)].node_solid) {
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
    //change in ground //attempt in preventing clipping through corners
      if (player.saved_ground_id!=player.on_ground_id  && player.on_ground_id!=player.previous_web_placed) {
      //outwards from ground
        if (0<=Ground[player.saved_ground_id].height_from_player_x && Ground[player.saved_ground_id].height_from_player_x<10) { //above ground
          player.previous_above=TRUE;
          move_x(-cos(Ground[player.saved_ground_id].angle+M_PI_2));
          move_y(-sin(Ground[player.saved_ground_id].angle+M_PI_2));
        } else if (Ground[player.saved_ground_id].height_from_player_x>-10 &&
                 Ground[player.saved_ground_id].height_from_player_x<0) { //below ground
          player.previous_below=TRUE;
          move_x(-cos(Ground[player.saved_ground_id].angle-M_PI_2));
          move_y(-sin(Ground[player.saved_ground_id].angle-M_PI_2));
        }
      }
      if (player.on_ground_id!=-1 && player.on_ground_id!=player.previous_web_placed) {
	//
        if (!IsSpeedBreaking()) {//reset stats when normal
          player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
        } else {
          player.player_jump_height=150;
          if (player.speed<5) {
            player.speed++;
          }
        }

        if (player.fling_distance<=0 && player.on_ground_timer>=1 && speed==0 && grav_speed==0) {
          if (!player.is_rebounding) {
            if (!IsSpeedBreaking()) {//reset stats when normal            
              if (!player.rst_key_sprint) {
                if (player.speed>DEFAULT_PLAYER_SPEED)
                  player.speed--;
              } else {
                if (player.speed>6)
                  player.speed--;
              }
            } else {
              if (player.speed>8)
                player.speed--;
            }
          }
        }

        if (player.rst_key_sprint || IsSpeedBreaking() || player.time_breaker) {
          player.player_jump_height+=player.speed*NODE_SIZE;
        }


        if ((Ground[player.on_ground_id].x1-10<=player.x && player.x<=Ground[player.on_ground_id].x2+10) && //within x
            ((Ground[player.on_ground_id].y1-10<=player.y && player.y<=Ground[player.on_ground_id].y2+10) || //within y
             (Ground[player.on_ground_id].y2-10<=player.y && player.y<=Ground[player.on_ground_id].y1+10))) {

         //Within ground axes
          player.angle=Ground[player.on_ground_id].angle; //set player angle
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

              player.jump_height=0; //Stop Jump & stick to ground
              player.jump=FALSE;
              player.key_jump_timer=0;
            }
            player.in_air_timer--;
          }




        //outwards from ground
          if (0<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10) { //above ground
            player.current_above=TRUE;
            player.rebound_above=TRUE;
            player.rebound_below=FALSE;
            move_x(-cos(player.angle+M_PI_2));
            move_y(-sin(player.angle+M_PI_2));

            if (player.fling_distance<=0) { //regular jumping
              player.launch_angle=player.angle+M_PI_2;
            }
            player.fling_distance=0; //on ground, stop flinging

            if (abs(Ground[player.on_ground_id].gradient)<=0.5) { //nonsteep slope
              if (Ground[player.on_ground_id].gradient>0) {
                player.angle_of_reflection=-player.angle-player.angle_of_incidence;
                if (player.is_rebounding) 
                  player.last_left=FALSE;
              } else {
                player.angle_of_reflection=-player.angle+M_PI+player.angle_of_incidence;
                if (player.is_rebounding) 
                  player.last_left=TRUE;
              }
            } else { //steep slope
              /*if (Ground[player.on_ground_id].gradient>0) {
                player.angle_of_reflection=-player.angle-player.angle_of_incidence;
                if (player.is_rebounding) 
                  player.last_left=FALSE;
              } else {
                player.angle_of_reflection=-player.angle+M_PI+player.angle_of_incidence;
                if (player.is_rebounding) 
                  player.last_left=TRUE;
              }*/
              if (Ground[player.on_ground_id].gradient>0) {
                player.angle_of_reflection=-player.angle+player.angle_of_incidence;
                if (player.is_rebounding) 
                  player.last_left=FALSE;
              } else {
                player.angle_of_reflection=-player.angle+M_PI-player.angle_of_incidence;
                if (player.is_rebounding) 
                  player.last_left=TRUE;
              }
            }

          } else if (Ground[player.on_ground_id].height_from_player_x>-10 && Ground[player.on_ground_id].height_from_player_x<0) { //below ground
            player.current_below=TRUE;
            player.rebound_above=FALSE;
            player.rebound_below=TRUE;
            move_x(-cos(player.angle-M_PI_2));
            move_y(-sin(player.angle-M_PI_2));

            if (player.fling_distance<=0) {
              player.launch_angle=player.angle-M_PI_2;
            }
            player.fling_distance=0; //on ground, stop flinging
 
           //bouncing below the surface the steepness doesnt matter
            if (Ground[player.on_ground_id].gradient>0) {
                if (player.is_rebounding) 
                  player.last_left=TRUE;
              player.angle_of_reflection=(player.angle+M_PI+player.angle_of_incidence)+M_PI_4;
            } else {
                if (player.is_rebounding) 
                  player.last_left=FALSE;
              player.angle_of_reflection=(player.angle-player.angle_of_incidence)-M_PI_4;
            }
          }









        //inwards to ground
          if (3<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10){ //above ground
            move_x(cos(player.angle+M_PI_2));
            move_y(sin(player.angle+M_PI_2));
            if (player.on_ground_timer<2) {
              player.on_ground_timer++;
            }
          } else {//below ground
	        allow_act=FALSE;
	        if (Ground[player.on_ground_id].height_from_player_x>-10) {
	          if (abs(Ground[player.on_ground_id].gradient)>0.1 &&
                Ground[player.on_ground_id].height_from_player_x<-5) {
	            allow_act=TRUE;
              } else if (abs(Ground[player.on_ground_id].gradient)<=0.1 &&
		        Ground[player.on_ground_id].height_from_player_x<-3) {
	            allow_act=TRUE;
	          }
	        }
	        //^^ condition
	        if (allow_act) {
              move_x(cos(player.angle-M_PI_2));
              move_y(sin(player.angle-M_PI_2));
              if (player.on_ground_timer<2) {
                player.on_ground_timer++;
              }
	        }
          }
          if (!player.current_above && !player.current_below) {
            player.angle=0;
            player.on_ground_id=-1;
          }

        } else {
      //not on ground
          player.on_ground_id=-1;
        }
      } else { //not on a ground
    //fall down normally
        player.on_ground_id=-1;
      }




    //Y movement
    //Condition to jump
      if (player.current_above && player.on_ground_id!=-1) {
        if (player.on_ground_timer<10)
          player.on_ground_timer++;
      } else if (player.current_below) {
        if (player.on_ground_timer<10)
          player.on_ground_timer++;
      }
      if (abs(Ground[player.on_ground_id].angle)>M_PI_2-0.01) {
        if (player.on_ground_timer<10)
          player.on_ground_timer++;
      }
      if (player.on_ground_timer>0) {
        player.previous_web_placed=-1;
      }



      //==========REBOUND ACTIONS==========
      if (player.is_rebounding && grav_speed==0) {
        if (player.rebound_above) {
          move_x(cos(player.angle_of_reflection));
          move_y(sin(player.angle_of_reflection));
        } else if (player.rebound_below){
          move_x(-cos(player.angle_of_reflection));
          move_y(-sin(player.angle_of_reflection));
        }
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
	          player.player_grav=0.5;
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
        if (player.jump_height>0) {
          player.player_grav=0.5;
          player.jump_height-=player.player_grav;
          move_x(2*player.player_grav*-cos(player.launch_angle));
          move_y(2*player.player_grav*-sin(player.launch_angle)); //jump go against gravity and perpendicular from platform
          if (player.jump_height<=0) {
            player.jump=FALSE;
          }
        } else {
          player.player_grav=0.5;
        } 
        if (player.on_ground_id==-1 && player.jump_height<=0) { //Credit: y4my4m for pushing me to pursue this gameplay aspect
          if (!player.is_swinging && (player.fling_distance==0 || player.fling_distance<-100)) { //not swinigng and player is not flinging
            if (player.in_air_timer>11) {
              move_y(player.player_grav);
	        } else {
              move_x(-player.player_grav*2*cos(player.launch_angle));
              move_y(player.player_grav*2*sin(player.launch_angle)); //grav switch
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
          player.player_grav=0.5;
        }
      }
      if (player.y<0) { //Y axis cap
        move_y(1);
        //move_y(player.player_grav);
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
        } else if (player.on_ground_id!=-1) { //is swinging but on ground
          if (player.pivot_length<DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE) { //allow movement if within swing distance
            allow_act=TRUE;
          }
        }
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
          if (player.rst_left) {
            move_x(-1);
          } else if (player.rst_right) {
            move_x(1);
          }
        }
      }


      //====PLAYER CIRCULAR WEB SWINGING MOVEMENT======
      if (player.is_swinging && (grav_speed==0 || grav_speed==1)) {
        player.is_rebounding=FALSE;
        player.pivot_length=GetDistance(player.pivot_x,player.pivot_y,player.x,player.y);
        player.pivot_angle=GetCosAngle(player.x-player.pivot_x,player.pivot_length);


        //>>>Calculate the Launch Angle of player when swung from pivot
        if (player.y>player.pivot_y) { //player is below pivot
          player.launch_angle=player.pivot_angle+M_PI_2;
        } else { //player is above pivot
          player.launch_angle=-player.pivot_angle-M_PI_2;
        }

        if (grav_speed==0) {
          if (player.on_ground_id==-1) {
            if (player.y>player.pivot_y) { //below pivot
              if (player.rst_right) {
                player.fling_left=TRUE;
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              }
              if (player.rst_left) {
                player.fling_left=FALSE;
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else if (player.rst_up && player.on_ground_id==-1 && player.pivot_length>2){ //Retract Web
                move_x(-cos(-player.pivot_angle));
                move_y(sin(-player.pivot_angle));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else if (player.rst_down && player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) {
                move_x(cos(-player.pivot_angle));
                move_y(-sin(-player.pivot_angle));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else {//nokey default movement
                if (player.x<player.pivot_x) {
                  if (!player.rst_right) {
                    if (player.counter_potential_energy>0){
                      player.fling_left=TRUE;
                      move_x(-cos(-player.pivot_angle+M_PI_2));
                      move_y(sin(-player.pivot_angle+M_PI_2));
                      player.counter_potential_energy-=1;
                    } else {
                      player.fling_left=FALSE;
                      move_x(cos(-player.pivot_angle+M_PI_2));
                      move_y(-sin(-player.pivot_angle+M_PI_2));
                      player.potential_energy+=1;
                    }
                  }
                } else if (!player.rst_right) {
                  if (player.potential_energy>0) {
                    player.fling_left=FALSE;
                    move_x(cos(-player.pivot_angle+M_PI_2));
                    move_y(-sin(-player.pivot_angle+M_PI_2));
                    player.potential_energy-=1;
                  } else {
                    player.fling_left=TRUE;
                    move_x(-cos(-player.pivot_angle+M_PI_2));
                    move_y(sin(-player.pivot_angle+M_PI_2));
                    player.counter_potential_energy+=1;
                  }
                }
              }
            } else { //above pivot
              if (player.rst_right) {
                player.fling_left=FALSE;
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
              }
              if (player.rst_left) {
                player.fling_left=TRUE;
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
              } else if (player.rst_up && player.on_ground_id==-1 && player.pivot_length>2){ //Retract web
                move_x(-cos(-player.pivot_angle));
                move_y(-sin(-player.pivot_angle));
              } else if (player.rst_down && player.pivot_length<NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) {
                move_x(cos(-player.pivot_angle));
                move_y(sin(-player.pivot_angle));
              } else {//nokey default movement
                if (player.x<player.pivot_x) {
                  if (!player.rst_right) {
                    player.fling_left=TRUE;
                    move_x(-cos(-player.pivot_angle+M_PI_2));
                    move_y(-sin(-player.pivot_angle+M_PI_2));
                  }
                } else if (!player.rst_right){ //right side of pivot
                  player.fling_left=FALSE;
                  move_x(cos(-player.pivot_angle+M_PI_2));
                  move_y(sin(-player.pivot_angle+M_PI_2));
                }
              }
            }
          }
        } else if (grav_speed==1) {//only occurs right after grav_speed==0
          if (player.pivot_length>NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) { //rubber band back if pivot length too long
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
        if (player.pivot_length>NODE_SIZE*DEFAULT_PLAYER_BUILD_RANGE/2) { //if pivot length is more than normal build range
          if (player.fling_distance<0) { //allow cancel flinging when fling distance is less than 0
            if (player.rst_left || player.rst_right) { //cancel flinging when left or right key is pressed
              player.fling_distance=0;
            }
          }
        } 


        if (player.fling_distance>0) { //fling and against gravity
          if (!player.fling_left) {
            move_x(-cos(player.launch_angle));
            move_y(-sin(player.launch_angle));
            player.last_left=FALSE;
          } else { //fling left
            move_x(cos(player.launch_angle));
            move_y(sin(player.launch_angle));
            player.last_left=TRUE;
          }
          player.fling_distance--;
          if (player.fling_distance==1) { //cap in air timer right before end of fling distance
            player.in_air_timer=1002;
            player.fling_distance=-1;
          }
        } else if (player.fling_distance<0){ //Continue moving but not against gravity
          if (!player.fling_left) {
            move_x(-cos(player.launch_angle));
          } else { //fling left
            move_x(cos(player.launch_angle));
          }
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
      if (player.print_current_above!=player.current_above) {
        player.previous_above=player.current_above;
      }
      if (player.print_current_below!=player.current_below) {
        player.previous_below=player.current_below;
      }

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


  double grav_dist;
  //if (player.speed>3) {
  grav_dist=GetDistance(3,0,0,player.grav);
 player.angle_of_incidence=GetCosAngle(3,grav_dist);
  /*} else {
    grav_dist=GetDistance(2,0,0,player.grav);
    player.angle_of_incidence=GetCosAngle(2,grav_dist);
  }*/
  if (player.fling_distance>0) { //angle of incidence when flinging
    if (player.fling_left) {
      player.angle_of_incidence=player.launch_angle;
    } else {
      player.angle_of_incidence=-M_PI+player.launch_angle;
    }
  }

 //misc
  if (player.on_ground_timer>0) {
    player.on_ground_timer--;
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
          PlaySound(L"snd/timebreaker__stop.wav", NULL, SND_FILENAME | SND_ASYNC);
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


void PlayerCameraShake()
{
  int i;
//camshake
  //if (!the_bravery_tyrant && IsNormalView) {
    float y_bob=0,x_bob=0;
    //if (sprint_bobbing) {  //if sprint_bobbing
      if (player.on_ground_id!=-1) {//not in air
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
    if (!player.is_rebounding) { //falling_camera
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

    if (player.flag_revert_palette && player.time_breaker_tick<=0) {
      BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);
      for (int i=0;i<ENEMY_NUM;i++) {
        if (Enemy[i].health>0) {
          if (Enemy[i].species==1) {
            Enemy[i].saved_angle=Enemy[i].sprite_angle-1;
          }
          if (Enemy[i].sprite_1!=NULL) {
            BitmapPalette(hdc,Enemy[i].sprite_1,rgbColorsDefault);
          }
          if (Enemy[i].sprite_2!=NULL) {
            BitmapPalette(hdc,Enemy[i].sprite_2,rgbColorsDefault);
          }
          if (Enemy[i].sprite_3!=NULL) {
            BitmapPalette(hdc,Enemy[i].sprite_3,rgbColorsDefault);
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
        if (Enemy[i].health>0) {
        if (Enemy[i].sprite_1!=NULL && !Enemy[i].time_breaker_immune) {
          BitmapPalette(hdc,Enemy[i].sprite_1,rgbColorsNoir);
        }
        if (Enemy[i].sprite_2!=NULL && !Enemy[i].time_breaker_immune) {
          BitmapPalette(hdc,Enemy[i].sprite_2,rgbColorsNoir);
        }
        if (Enemy[i].sprite_3!=NULL && !Enemy[i].time_breaker_immune) {
          BitmapPalette(hdc,Enemy[i].sprite_3,rgbColorsNoir);
        }
        }
      }
      BitmapPalette(hdc,map_platforms_sprite,rgbColorsNoir);
    }
  } else {
    if (player.time_breaker_tick>0) {
      //mciSendStringA("play snd/timeskip.mp3",NULL,0,NULL);
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
    GrLine(hdc,player.sprite_x,player.sprite_y,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,LTCYAN);
    if (!IsInvertedBackground()) {
      GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,WHITE,-1);
    } else {
      GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,BLACK,-1);
    }
    if (player.fling_left) {  //line showing where player will launc/fling to
      GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+100*cos(player.launch_angle),player.sprite_y+100*sin(player.launch_angle),LTGREEN);
    } else {
      GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x-100*cos(player.launch_angle),player.sprite_y-100*sin(player.launch_angle),LTGREEN);
    }
  }

  //Draw the bullet the player shoots
  if (player.bullet_shot!=-1) {
    if (player.right_click_hold_timer<62) {
      DrawBullet(hdc,player.bullet_shot);
      GrLine(hdc,player.sprite_x,player.sprite_y,Bullet[player.bullet_shot].sprite_x,Bullet[player.bullet_shot].sprite_y,LTCYAN);    
    }
  }
  
  for (int i=0;i<player.bullet_shot_num;i++) {
    DrawBullet(hdc,player.bullet[i]);
  }
  //Draw angle of incidence
  //GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+cos(player.angle_of_incidence)*100,player.sprite_y+sin(player.angle_of_incidence)*100,BROWN);
  //GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+cos(player.angle_of_reflection)*100,player.sprite_y+sin(player.angle_of_reflection)*100,YELLOW);

}

