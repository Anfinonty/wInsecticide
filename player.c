
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
  if (player.rst_speed_break && player.speed_breaker_units>0) {
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


bool IsCollideCrawler(double x1,double y1,double x2,double y2,double gradient,double c)
{
  int on_grid_id=0,i=0,enemy_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (x1!=x2) {
    if (-1<gradient && gradient<1) { // y=mx+c
      for (x=x1;x<=x2;x++) {
        lg_y=x*gradient+c;
        on_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        for (i=0;i<Grid[on_grid_id].enemy_occupy_num;i++) {
    	  enemy_id=Grid[on_grid_id].enemy_occupy[i];
          if (Enemy[enemy_id].species==1 && Enemy[enemy_id].health>0) {
    	    if (GetDistance(x,lg_y,Enemy[enemy_id].x,Enemy[enemy_id].y)<=NODE_SIZE*2) {
    	      return TRUE;
    	    }
          }
        }
      }
    } else { // x=(y-c)/m
      if (y1<y2) {
        min=y1;
        max=y2;
      } else {
        min=y2;
        max=y1;
      }
      for (y=min;y<=max;y++) {
        lg_x=(y-c)/gradient;
        on_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        for (i=0;i<Grid[on_grid_id].enemy_occupy_num;i++) {
	      enemy_id=Grid[on_grid_id].enemy_occupy[i];
          if (Enemy[enemy_id].species==1 && Enemy[enemy_id].health>0) {
    	    if (GetDistance(lg_x,y,Enemy[enemy_id].x,Enemy[enemy_id].y)<=NODE_SIZE*2) {
    	      return TRUE;
    	    }
          }
        }
      }
    }
  } else {
    return TRUE;
  }
  return FALSE;
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

void InitVRDGrid()
{
  int i=0,j=0,k=0,on_grid_id=0,column=0,row=0,
      start_x=0,start_y=0;

  for (i=0;i<player.rendered_vgrid_num;i++) {
    VGrid[player.render_vgrids[i]].within_render_distance=FALSE; //all rendered-grids are no-longer within render distance
    player.render_vgrids[i]=-1;  //unrender all rendered grids
  }
  for (i=0;i<player.rendered_ground_num;i++) {
    Ground[player.render_grounds[i]].within_render_distance=FALSE; //All rendered-grounds are no-longer within render distance
    player.render_grounds[i]=-1; //unrender all rendered grounds
  }

  player.rendered_ground_num=0;
  player.rendered_vgrid_num=0;

  //Begin rendering
  start_x=player.x-(dyn_vrenderdist/2*VGRID_SIZE); //Top left corner of render distance grids to bottom right corner
  start_y=player.y-(dyn_vrenderdist/2*VGRID_SIZE);
  //"What happens when you lose everything? You just art again. Start all over again" - Maximo Park
  for (i=0;i<dyn_vrenderdist_num;i++) { //all render distance grids from top-left to bottom-right
    VRDGrid[i].x=start_x+column*VGRID_SIZE;
    VRDGrid[i].y=start_y+row*VGRID_SIZE;
    if (0<VRDGrid[i].x && VRDGrid[i].x<MAP_WIDTH && //render distance grid is within range
        0<VRDGrid[i].y && VRDGrid[i].y<MAP_HEIGHT) {

      //vgrid
      on_grid_id=GetGridId(VRDGrid[i].x,VRDGrid[i].y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);//get grid id based on renderdistance grid axes
      VGrid[on_grid_id].within_render_distance=TRUE; //append grid to render_grids array
      player.render_vgrids[player.rendered_vgrid_num]=on_grid_id;//cannot be i (what if out of bounds)
      player.rendered_vgrid_num++; //count amount of grids rendered

      //grounds
      for (j=0;j<VGrid[on_grid_id].max_ground_num;j++) { //fetch all grounds that are occupying the grid
	    k=VGrid[on_grid_id].ground_ids[j]; //ground_id in grid
 	    if (!Ground[k].within_render_distance) { //ground is now within render distance
	      Ground[k].within_render_distance=TRUE;
          player.render_grounds[player.rendered_ground_num]=k; //append ground to render_grounds array
	      player.rendered_ground_num++; //count number of rendered grounds
	    } 
      }

    }
    column++; //Next column
    if (column>=dyn_vrenderdist) { //if the column is beyond the render distance
      row++; //move to the next row
      column=0; //go back to first column
    }
  } 
}

void InitRDGrid()
{
  int i=0,j=0,k=0,on_grid_id=0,column=0,row=0,
      start_x=0,start_y=0;
  //reset
  for (i=0;i<player.rendered_grid_num;i++) {
    Grid[player.render_grids[i]].within_render_distance=FALSE; //all rendered-grids are no-longer within render distance
    player.render_grids[i]=-1;  //unrender all rendered grids
  }
  for (i=0;i<player.rendered_enemy_num;i++) {
    Enemy[player.render_enemies[i]].within_render_distance=FALSE; //all rendered-enemies are no-longer within render distance
    player.render_enemies[i]=-1; //unrender all rendered enemies
  }

  player.rendered_grid_num=0;
  player.rendered_enemy_num=0;

  //Begin rendering
  start_x=player.x-(RENDER_DIST/2*GRID_SIZE); //Top left corner of render distance grids to bottom right corner
  start_y=player.y-(RENDER_DIST/2*GRID_SIZE);
  //"What happens when you lose everything? You just art again. Start all over again" - Maximo Park
  for (i=0;i<RDGRID_NUM;i++) { //all render distance grids from top-left to bottom-right
    RDGrid[i].x=start_x+column*GRID_SIZE;
    RDGrid[i].y=start_y+row*GRID_SIZE;
    if (0<RDGrid[i].x && RDGrid[i].x<MAP_WIDTH && //render distance grid is within range
        0<RDGrid[i].y && RDGrid[i].y<MAP_HEIGHT) {

      //grid
      on_grid_id=GetGridId(RDGrid[i].x,RDGrid[i].y,MAP_WIDTH,GRID_SIZE,GRID_NUM);//get grid id based on renderdistance grid axes
      Grid[on_grid_id].within_render_distance=TRUE; //append grid to render_grids array
      player.render_grids[player.rendered_grid_num]=on_grid_id;//cannot be i (what if out of bounds)
      player.rendered_grid_num++; //count amount of grids rendered

      //enemy
      for (j=0;j<Grid[on_grid_id].enemy_occupy_num;j++) { //fetch enemies that are occupying the grid
	    k=Grid[on_grid_id].enemy_occupy[j]; //enemy_id in grid
 	    if (!Enemy[k].within_render_distance) { //enemy is now within render distance
	      Enemy[k].within_render_distance=TRUE;
          player.render_enemies[player.rendered_enemy_num]=k; //append grid to render_enemies array
	      player.rendered_enemy_num++; //count number of rendered enemies 
	    }
      }
    }
    column++; //Next column
    if (column>=RENDER_DIST) { //if the column is beyond the render distance
      row++; //move to the next row
      column=0; //go back to first column
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
    j=player.render_enemies[i];
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
  player.web_storage[player.placed_web_pos]=-1;
  player.placed_web_pos++;
  player.placed_web_num++;
  if (player.placed_web_pos>=player.max_web_num) {
    player.placed_web_pos=0;
  }
}


void CleanUpPlayer()
{
  if (player.sprite_1_cache!=NULL) {
    DeleteObject(player.sprite_1_cache);
  }
  if (player.sprite_2_cache!=NULL) { 
    DeleteObject(player.sprite_2_cache);
  }
  if (player.sprite_jump_cache!=NULL) { 
    DeleteObject(player.sprite_jump_cache);
  }
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
}


void InitPlayer() {
  int i;
  //CleanUpPlayer();
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
  player.rst_speed_break=FALSE;
  player.attack=FALSE;
  player.blocking=FALSE;
  player.print_valid_web=FALSE;
  player.valid_web=FALSE;
  player.attack_rst=FALSE;
  player.destroy_ground=FALSE;
  player.uppercut=FALSE;

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

  player.rendered_grid_num=0;
  for (i=0;i<RDGRID_NUM;i++) {
    player.render_grids[i]=-1;
  }

  player.rendered_vgrid_num=0;
  for (i=0;i<VRDGRID_NUM;i++) {
    player.render_vgrids[i]=-1;
  }

  player.rendered_enemy_num=0;
  for (i=0;i<ENEMY_NUM;i++) {
    player.render_enemies[i]=-1;
  }

  player.rendered_ground_num=0;
  for (i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {
    player.render_grounds[i]=-1;
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

  player.speed_breaker_units=0;
  player.speed_breaker_max=DEFAULT_PLAYER_SPEED_BREAKER_MAX;
  player.speed_breaker_cooldown=DEFAULT_PLAYER_SPEED_BREAKER_COOLDOWN;
  player.speed_breaker_cooldown_max=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN;
  player.speed_breaker_recharge_cooldown=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN;
  player.speed_breaker_recharge_cooldown_max=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN;

  player.left_click_hold_timer=0;
  player.right_click_hold_timer=0;
  player.attack_timer=0;
  player.sleep_timer=DEFAULT_SLEEP_TIMER;
  player.hit_cooldown_timer=0;
  player.block_timer=0;
  player.block_recharge_timer=0;
  player.block_recharge_timer_max=300;
  player.block_cooldown=0;
  player.block_cooldown_max=300;
  player.block_health_max=DEFAULT_PLAYER_BLOCK_HEALTH_MAX;
  player.block_health=DEFAULT_PLAYER_BLOCK_HEALTH_MAX;

  player.health=DEFAULT_PLAYER_HEALTH;
  player.knockback_strength=DEFAULT_PLAYER_KNOCKBACK_STRENGTH;
  player.knockback_speed=0.5;
  player.knockback_speed_multiplier=2;
  player.attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;

  player.lock_web_type=1;
//  player.lock_web_type=2;

  player.max_web_num=DEFAULT_PLAYER_WEB_NUM;

  player.bullet_shot=-1;

  player.placed_web_pos=0;
  player.placed_web_num=0;
  player.destroyed_web_pos=0;

  player.web_type=1;
//  player.web_type=2;
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

  InitPlayerCamera();
  InitRDGrid();
  InitVRDGrid();
}

bool YesInitRDGrid()
{
  if (GRID_SIZE*2<player.x && player.x<MAP_WIDTH-GRID_SIZE*2) {
    if (player.x<RDGrid[0].x+GRID_SIZE*2 || player.x>RDGrid[RENDER_DIST-1].x-GRID_SIZE*2) {
      return TRUE;
    }
  }
  if (GRID_SIZE*2<player.y && player.y<MAP_HEIGHT-GRID_SIZE*2) {
    if (player.y<RDGrid[0].y+GRID_SIZE*2 || player.y>RDGrid[RDGRID_NUM-1].y-GRID_SIZE*2) {
      return TRUE;
    }
  }
  return FALSE;
}

bool YesInitVRDGrid()
{
  int dyn=2;
  if (GR_WIDTH>=1440) { //increased frequency
    dyn=4;
  }
  if (VGRID_SIZE*2<player.x && player.x<MAP_WIDTH-VGRID_SIZE*2) {
    if (player.x<VRDGrid[0].x+VGRID_SIZE*dyn || player.x>VRDGrid[dyn_vrenderdist-1].x-VGRID_SIZE*dyn) {
      return TRUE;
    }
  }
  if (VGRID_SIZE*2<player.y && player.y<MAP_HEIGHT-VGRID_SIZE*2) {
    if (player.y<VRDGrid[0].y+VGRID_SIZE*dyn || player.y>VRDGrid[dyn_vrenderdist_num-1].y-VGRID_SIZE*dyn) {
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
  if (YesInitVRDGrid()) {
    InitVRDGrid();
  }


  //======================
  if (player.attack_timer>=0) {
    if (player.grav>10) {
      player.attack_strength=2;
    } else {
      player.attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;
    }
    player.attack_timer--;
  }
  //======================

  //Sprinting
  if (player.rst_key_sprint) {
    if (player.on_ground_id!=-1) {//on ground
      player.speed=3;
      if (IsSpeedBreaking()) {
        player.speed=5;
      }
    }
  }

  //Speed Breaker
  if (!player.time_breaker && IsSpeedBreaking()) {
    allow_act=TRUE;
  }
  //^^ condition
  if (allow_act) {
    player.speed_breaker_units--; //speedbreak
    player.speed_breaker_cooldown=player.speed_breaker_cooldown_max;
    if (player.on_ground_id!=-1) {
      player.speed=5;
      player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT*2;
    }
    if (player.speed_breaker_units==0) {
      player.rst_speed_break=FALSE;
    }
    player.sleep_timer=SLOWDOWN_SLEEP_TIMER;//maiden heaven
  } else {
    player.sleep_timer=DEFAULT_SLEEP_TIMER; //normal game speed
    if (player.speed_breaker_cooldown>0) {//at max when speedbreak
      player.speed_breaker_cooldown--;
    }
    if (player.speed_breaker_recharge_cooldown>0) {//auto
      player.speed_breaker_recharge_cooldown--;
    }
    if (player.speed_breaker_cooldown<=0 &&
	    player.speed_breaker_units<player.speed_breaker_max) { //Recharging
      if (player.speed_breaker_recharge_cooldown<=0) {
        player.speed_breaker_units++;
	    player.speed_breaker_recharge_cooldown=
	      player.speed_breaker_recharge_cooldown_max;
      }
    }
  }

  //Clicking
  if (player.bullet_shot!=-1) {
    player.web_being_shot=player.web_storage[player.placed_web_pos];
    BulletAct(player.bullet_shot);
  }
  Click();
  if (player.left_click_hold_timer==62) {//Left click to Attack
    player.attack=TRUE;
    player.blocking=FALSE; //unblock
  }
  if (player.right_click_hold_timer==62) {//Right click to Shoot Agsin
    if (player.bullet_shot!=-1 && !player.is_swinging) {
      StopBullet(player.bullet_shot,TRUE);
      player.web_being_shot=-1;
      player.bullet_shot=-1;
    }
  }
  if (player.web_type==0 && player.on_ground_id==-1) {//in air
    player.web_type=1;
  }
  if (player.lock_web_type==0 && player.web_type==1 && player.on_ground_id!=-1) {
    player.web_type=player.lock_web_type;//go back to mode 0
  }

  if (player.is_swinging) {
    if (player.y>player.pivot_y) { //player is below pivot
      player.launch_angle=player.pivot_angle+M_PI_2;
    } else { //player is above pivot
      player.launch_angle=-player.pivot_angle-M_PI_2;
    }
  }

  if (!player.attack_rst && !player.is_swinging) {
    switch (player.web_type) {
      case 0://legacy
        break;
      case 1://current
        allow_act=FALSE;
        if (player.right_click_hold_timer==62 &&  //Right click to shoot
	        !NearCrawler()) {
          if (player.placed_web_num<player.max_web_num && //webs > 0 
	        player.bullet_shot==-1) {
	        allow_act=TRUE;
	      }
        }
        if (allow_act) {
          //int pivot_node_grid_id=0,edge_node_grid_id=0;
          //int edge_x=0,edge_y=0;
          //double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0,bm_gradient=0,pivot_x=0,pivot_y=0;
          grad_x1=player.above_x+player.cam_move_x;
          grad_y1=player.above_y+player.cam_move_y;
          grad_x2=mouse_x-player.cam_x+player.cam_move_x;
          grad_y2=mouse_y-player.cam_y+player.cam_move_y;
          //edge_node_grid_id=GetGridId(mouse_x+player.cam_x+player.cam_move_x,mouse_y+player.cam_y+player.cam_move_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);


          player.attack=TRUE; 
    	  player.bullet_shot=current_bullet_id;
          ShootBullet(current_bullet_id,
		-1,
		CYAN,
        2, //graphics type
		DEFAULT_PLAYER_BUILD_RANGE, //range
	    1, //speed
		10, //speed multiplier
   		1, //damage
		-1,
		player.above_x, //so it doest get stuck to ground
		player.above_y,
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
        break;
    }//end switch
  } else { //meelee attack only
    if (player.right_click_hold_timer==62) {
      player.attack=TRUE;
      player.blocking=FALSE; //unblock
     /* if (player.bullet_shot!=-1) {
          player.web_being_shot=-1;
          player.bullet_shot=-1;
      }else*/
      if (player.is_swinging) { //fling
        /*if (player.y>player.pivot_y) { //player is below pivot
          player.launch_angle=-player.pivot_angle+M_PI_2;
        } else { //player is above pivot
          player.launch_angle=-player.pivot_angle-M_PI_2;
        }*/
        player.is_swinging=FALSE;
        player.fling_distance+=player.pivot_length*2;
        player.key_jump_timer=player.player_jump_height;
        double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
    //player place web after swing
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
            //double q=0.2*(((DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE)-Bullet[bullet_id].range)/10);
            Ground[web_id].health=10;//-q;
          }
        }
      }
    }
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
  if (player.rst_up && 5<=player.on_ground_timer && player.on_ground_timer<=10 && player.fling_distance<=0) {
    player.jump=TRUE;
    player.on_ground_timer=0;
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
        if (player.on_ground_id>=GROUND_NUM /*&& player.on_ground_id!=player.web_being_shot*/) {
          DestroyGround(player.on_ground_id);  
    	  RegainWeb(player.on_ground_id);
        }
        player.destroy_ground=FALSE;
      }

   //Ground action
   //on a ground
    //change in ground
      if (player.saved_ground_id!=player.on_ground_id) {
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
          //player.jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
	      if (!player.rst_key_sprint) {
            player.speed=DEFAULT_PLAYER_SPEED;
	      }
        }
        if ((Ground[player.on_ground_id].x1-5<=player.x && player.x<=Ground[player.on_ground_id].x2+5) && //within x
            ((Ground[player.on_ground_id].y1-5<=player.y && player.y<=Ground[player.on_ground_id].y2+5) ||
             (Ground[player.on_ground_id].y2-5<=player.y && player.y<=Ground[player.on_ground_id].y1+5))) {
          player.angle=Ground[player.on_ground_id].angle;
        //outwards from ground
          if (0<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10) { //above ground
            player.current_above=TRUE;
            player.in_air_timer=0;
            move_x(-cos(player.angle+M_PI_2));
            move_y(-sin(player.angle+M_PI_2));
            if (player.fling_distance<=0) {
              player.launch_angle=player.angle+M_PI_2;
            } else {
              player.fling_distance=0;
            }
          } else if (Ground[player.on_ground_id].height_from_player_x>-10 &&
		     Ground[player.on_ground_id].height_from_player_x<0) { //below ground
             player.current_below=TRUE;
             player.in_air_timer=0;
             move_x(-cos(player.angle-M_PI_2));
             move_y(-sin(player.angle-M_PI_2));
            if (player.fling_distance<=0) {
              player.launch_angle=player.angle-M_PI_2;
            } else {
              player.fling_distance=0;
            }
          }
        //inwards to ground
          if (3<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10){ //above ground
            move_x(cos(player.angle+M_PI_2));
            move_y(sin(player.angle+M_PI_2));
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
              player.on_ground_timer=2;
	        }
          }
          if (!player.current_above && !player.current_below) {
            player.angle=0;
            player.on_ground_id=-1;
          }
          if (player.current_below && player.rst_down) { //unstick (un-inwards)
	        player.in_air_timer=20;
	        player.on_ground_id=-1;
            move_x(-cos(player.angle-M_PI_2));
            move_y(-sin(player.angle-M_PI_2));

            player.jump=TRUE;
            player.on_ground_timer=0;
            player.key_jump_timer=player.player_jump_height;

          }
        } else {
      //not on ground
          player.in_air_timer=0;
          player.on_ground_id=-1;
        }
      } else { //not on a ground
    //fall down normally
        player.on_ground_id=-1;
        //player.angle=0;
      }
    //Y movement
    //Condition to jump
      /*if (player.on_ground_id==-1 && player.rst_down) {
	    player.jump_height=0;
        player.jump=FALSE;
      }*/
      if (player.current_above && player.on_ground_id!=-1) {
        player.on_ground_timer=10;
      } else if (player.current_below) {
        player.on_ground_timer=2;
        //player.jump_height=0;
        //player.jump=FALSE;
      }
      if (abs(Ground[player.on_ground_id].angle)>M_PI_2-0.01) {
        player.on_ground_timer=2;
        //player.jump_height=0;
        //player.jump=FALSE;
      }
      if (player.on_ground_timer>0) {
        player.previous_web_placed=-1;
      }
    //Gravity
      if (grav_speed==0 && speed==0) {
        if (player.on_ground_id==-1 && player.fling_distance<=0) {
          player.in_air_timer++;
	      if (player.jump_height==0) {
            if (player.in_air_timer%20==0 && player.grav<=100) {
              player.grav++;
	          player.player_grav=0.5;
              if (player.is_swinging) {
                player.grav=3;
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
      //if (player.on_ground_id==-1) {
        //player.launch_angle=0;
      //} else {
      //}
      if (speed==0) {
        if (player.jump_height>0) {
          player.player_grav=0.5;
          player.jump_height-=player.player_grav;
          move_x(2*player.player_grav*-cos(player.launch_angle));
          move_y(2*player.player_grav*-sin(player.launch_angle));
	      //player.cam_move_y+=0.4;
          //move_x(-player.player_grav*2);
          //move_y(-player.player_grav*2); //jump go against gravity
          if (player.jump_height<=0) {
            player.jump=FALSE;
          }
        } else {
          player.player_grav=0.5;
        }
        if (player.on_ground_id==-1 && player.jump_height<=0) {
          if (!player.is_swinging && player.fling_distance<=0) {
            if (player.in_air_timer>11) {
              //move_x(-player.player_grav*2*cos(player.launch_angle));
              //move_y(player.player_grav*2*sin(player.launch_angle)); //Falling via gravity
              move_y(player.player_grav);
	        } else {
              move_x(-player.player_grav*2*cos(player.launch_angle));
              move_y(player.player_grav*2*sin(player.launch_angle)); //grav switch
	          if (player.grav>2) { //Falling via gravity
	            move_y(1);
              }
            }
          }
        } else { //on ground
          //player.in_air_timer=0;
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
      if (player.y-PLAYER_HEIGHT/2<0) { //Y axis cap
        move_y(player.player_grav);
      } else if (player.y+PLAYER_HEIGHT/2>MAP_HEIGHT) {
        move_y(-player.player_grav);
        player.health--;
      }


     //X movement
      allow_act=FALSE;
      if (player.fling_distance<=0) {
        player.previous_web_placed=-1;
        if (!player.is_swinging) {
          if (player.on_ground_id==-1) { //player is not on ground
	        allow_act=TRUE;
          } else if (player.block_timer==0) { //OR player is not blocking
	        allow_act=TRUE;
          }
        } else if (player.on_ground_id!=-1) { //is swinging but on ground
          if (player.pivot_length<DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE) {
            allow_act=TRUE;
          }
        }
      }
      if (grav_speed==0 && allow_act) {
        if (player.current_above) {
          if (player.rst_right) {
            move_x(cos(player.angle));
            move_y(sin(player.angle));
          } else if (player.rst_left) {
            move_x(-cos(player.angle));
            move_y(-sin(player.angle));
          }
        } else if (player.current_below) {
          if (player.rst_right) {
            move_x(-cos(player.angle));
            move_y(-sin(player.angle));
          } else if (player.rst_left) {
            move_x(cos(player.angle));
            move_y(sin(player.angle));
          }
        } else {
          if (player.rst_left) {
            move_x(-1);
          } else if (player.rst_right) {
            move_x(1);
          }
        }
      }


      //fling movement
      if (grav_speed==0 && player.fling_distance>0) {
        if (!player.fling_left) {
          move_x(-cos(player.launch_angle));
          move_y(-sin(player.launch_angle));
        } else {
          move_x(cos(player.launch_angle));
          move_y(sin(player.launch_angle));
        }          
        player.fling_distance--;
      }


      //swinging movement
      if (player.is_swinging && (grav_speed==0 || grav_speed==1)) {
      /*if (player.right_click_hold_timer>0) {
      //double launch_angle=GetCosAngle(GR_WIDTH/2-mouse_x+player.cam_x,0.01);
        double launch_angle=GetCosAngle(GR_WIDTH/2-mouse_x,GetDistance(mouse_x,mouse_y,GR_WIDTH/2,GR_HEIGHT/2));
        //double launch_angle=M_PI;
        move_x(-cos(-launch_angle));
        if (mouse_y<GR_HEIGHT/2)
          move_y(sin(-launch_angle));
        else
          move_y(-sin(-launch_angle));

      }*/
        player.pivot_length=GetDistance(player.pivot_x,player.pivot_y,player.x,player.y);
        player.pivot_angle=GetCosAngle(player.x-player.pivot_x,player.pivot_length);

        if (grav_speed==0) {
          if (player.on_ground_id==-1) {
            if (player.y>player.pivot_y) { //below pivot
              if (player.rst_left) {
                player.fling_left=TRUE;
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else if (player.rst_right) {
                player.fling_left=FALSE;
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else if (player.rst_up){
                move_x(-cos(-player.pivot_angle));
                move_y(sin(-player.pivot_angle));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else if (player.rst_down) {
                move_x(cos(-player.pivot_angle));
                move_y(-sin(-player.pivot_angle));
                player.potential_energy=0;
                player.counter_potential_energy=0;
              } else {//nokey default movement
                if (player.x<player.pivot_x) {
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
                } else {
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
              if (player.rst_left) {
                player.fling_left=FALSE;
                move_x(cos(-player.pivot_angle+M_PI_2));
                move_y(sin(-player.pivot_angle+M_PI_2));
              } else if (player.rst_right) {
                player.fling_left=TRUE;
                move_x(-cos(-player.pivot_angle+M_PI_2));
                move_y(-sin(-player.pivot_angle+M_PI_2));
              } else if (player.rst_up){
                move_x(-cos(-player.pivot_angle));
                move_y(-sin(-player.pivot_angle));
              } else if (player.rst_down) {
                move_x(cos(-player.pivot_angle));
                move_y(sin(-player.pivot_angle));
              } else {//nokey default movement
                if (player.x<player.pivot_x) {
                  player.fling_left=TRUE;
                  move_x(-cos(-player.pivot_angle+M_PI_2));
                  move_y(-sin(-player.pivot_angle+M_PI_2));
                } else { //right side of pivot
                  player.fling_left=FALSE;
                  move_x(cos(-player.pivot_angle+M_PI_2));
                  move_y(sin(-player.pivot_angle+M_PI_2));
                }
              }
            }
          }
        } else if (grav_speed==1){
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
      } //end of grav==0


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
 //misc
  if (player.on_ground_timer>0) {
    player.on_ground_timer--;
  }
 //sprite rotation
  /*if (player.print_current_above) {
    player.print_current_below=FALSE;
  } else if (player.print_current_below) {
    player.print_current_above=FALSE;
  }*/
  
  /*if (!player.last_left && on_ground_id!=-1) {
    player.sprite_angle*=-1;
  }*/

//block
  allow_act=FALSE;
  if (player.attack_timer<=0) {
    if (player.rst_down) {
      if (player.on_ground_id==-1 || player.print_current_above) {
        allow_act=TRUE;
      }
    } else if (player.rst_up && player.print_current_below) {
      allow_act=TRUE;
    } else if (player.is_swinging && player.on_ground_id==-1) {
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
    if (player.on_ground_id==-1) { //player jumping && blocking
      player.speed=3;
    }
  } else {//player not blocking -regen block
    if (player.block_timer>0) {
      player.block_timer--;
    }
    if (player.block_cooldown>0) {
      player.block_cooldown--;
    } else {//3 seconds has passed
      if (player.block_recharge_timer>0) {
	    player.block_recharge_timer--;
      } else if (player.block_health<player.block_health_max) {//below max
        player.block_health++;
	    player.block_recharge_timer=player.block_recharge_timer_max;
      }
    }
  }

  if (player.is_swinging && player.on_ground_id==-1) { //fast when swinging
    player.speed=5;
  }
  if (player.fling_distance>0) {
    player.speed=5;
  }

  if (player.on_ground_id==-1 && player.block_timer>0) {
    player.sprite_angle-=0.1;
  }
  if (player.sprite_angle<-M_PI*2) {
    player.sprite_angle=0;
  }

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
	            player.cam_move_x+=0.75*RandNum(-2,2,1);//shaky cam
	            player.cam_move_y+=0.75*RandNum(-2,2,1);
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
    //if (fall_camera) { //falling_camera
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
    	  player.cam_move_x+=RandNum(-2,2,1);//shaky x
          player.cam_move_y+=RandNum(-2,2,1);//shaky y
        }
      }
    //}
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
  //GrRect(hdc,player.x-PLAYER_WIDTH,player.y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT,RGB(34,139,34));
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
  if (player.saved_sprite_angle!=player.sprite_angle && player.on_ground_id!=-1) {
    DeleteObject(player.sprite_1_cache);
    DeleteObject(player.sprite_2_cache);

    player.sprite_1_cache = RotateSprite(hdc, player.sprite_1,player.sprite_angle,LTGREEN,BLACK,-1);
    player.sprite_2_cache = RotateSprite(hdc, player.sprite_2,player.sprite_angle,LTGREEN,BLACK,-1);

    player.saved_sprite_angle=player.sprite_angle;
  }
  if (player.block_timer>0 && player.saved_block_sprite_angle!=player.sprite_angle) {
    DeleteObject(player.block_sprite_1_cache);
    DeleteObject(player.block_sprite_2_cache);
    DeleteObject(player.block_sprite_3_cache);

    player.block_sprite_1_cache = RotateSprite(hdc, player.block_sprite_1,player.sprite_angle,LTGREEN,BLACK,-1);
    player.block_sprite_2_cache = RotateSprite(hdc, player.block_sprite_2,player.sprite_angle,LTGREEN,BLACK,-1);
    player.block_sprite_3_cache = RotateSprite(hdc, player.block_sprite_3,player.sprite_angle,LTGREEN,BLACK,-1);

    player.saved_block_sprite_angle=player.sprite_angle;
  }
  if (player.attack_timer>=0 && player.saved_attack_sprite_angle!=player.sprite_angle) {
    DeleteObject(player.attack_sprite_1_cache);
    DeleteObject(player.attack_sprite_2_cache);
    DeleteObject(player.attack_sprite_3_cache);
    DeleteObject(player.attack_sprite_4_cache);

    player.attack_sprite_1_cache = RotateSprite(hdc, player.attack_sprite_1,player.sprite_angle,LTGREEN,BLACK,-1);
    player.attack_sprite_2_cache = RotateSprite(hdc, player.attack_sprite_2,player.sprite_angle,LTGREEN,BLACK,-1);
    player.attack_sprite_3_cache = RotateSprite(hdc, player.attack_sprite_3,player.sprite_angle,LTGREEN,BLACK,-1);
    player.attack_sprite_4_cache = RotateSprite(hdc, player.attack_sprite_4,player.sprite_angle,LTGREEN,BLACK,-1);
    player.saved_attack_sprite_angle=player.sprite_angle;
  }

  if (player.attack_timer==-1) { //not attacking
    if (player.block_timer==0) { //not blocking
      if (player.on_ground_timer>0) { // on ground
        if (player.walk_cycle<2) {
          GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_1_cache,player.last_left);
        } else {
          GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_2_cache,player.last_left);
        }
      } else { //in_air
        GrSprite(hdc,player.sprite_x,player.sprite_y-6,player.sprite_jump_cache,player.last_left);
      }
    } else { //blocking
      if (0<player.block_timer && player.block_timer<=5) {
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.block_sprite_1_cache,player.last_left);
      } else if (5<player.block_timer && player.block_timer<=10) {
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.block_sprite_2_cache,player.last_left);
      } else {
        GrSprite(hdc,player.sprite_x,player.sprite_y,player.block_sprite_3_cache,player.last_left);
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

  if (player.is_swinging) {
    GrLine(hdc,player.sprite_x,player.sprite_y,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,LTCYAN);
    if (!IsInvertedBackground()) {
      GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,WHITE,-1);
    } else {
      GrCircle(hdc,player.pivot_x+player.cam_x+player.cam_move_x,player.pivot_y+player.cam_y+player.cam_move_y,DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE,BLACK,-1);
    }
    if (player.fling_left) {
      GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x+100*cos(player.launch_angle),player.sprite_y+100*sin(player.launch_angle),LTGREEN);
    } else {
      GrLine(hdc,player.sprite_x,player.sprite_y,player.sprite_x-100*cos(player.launch_angle),player.sprite_y-100*sin(player.launch_angle),LTGREEN);
    }
  }

  if (player.bullet_shot!=-1) {
    if (Bullet[player.bullet_shot].sprite_x-(player.cam_x+player.cam_move_x)>0 &&
        Bullet[player.bullet_shot].sprite_y-(player.cam_y+player.cam_move_y)>0
    ) { //calculate x based on sprite_x 
      DrawBullet(hdc,player.bullet_shot);
      GrLine(hdc,player.sprite_x,player.sprite_y,Bullet[player.bullet_shot].sprite_x,Bullet[player.bullet_shot].sprite_y,LTCYAN);    
    }
  }

}

