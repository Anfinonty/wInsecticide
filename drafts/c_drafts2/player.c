



void move_x(double x)
{
  player_x+=x;
  cam_x-=x;
}

void move_y(double y)
{
  player_y+=y;
  cam_y-=y;
}

void InitRDGrid()
{
  int i=0,j=0,k=0,on_grid_id=0,column=0,row=0,
      start_x=0,start_y=0;
  //reset
  for (i=0;i<rendered_grid_num;i++) {
    Grid[render_grids[i]].within_render_distance=FALSE;
    render_grids[i]=-1;
  }
  for (i=0;i<rendered_enemy_num;i++) {
    Enemy[render_enemies[i]].within_render_distance=FALSE;
    render_enemies[i]=-1;
  }
  for (i=0;i<rendered_ground_num;i++) {
    Ground[render_grounds[i]].within_render_distance=FALSE;
    render_grounds[i]=-1;
  }
  rendered_grid_num=0;
  rendered_enemy_num=0;
  rendered_ground_num=0;
  start_x=player_x-(RENDER_DIST/2*GRID_SIZE),
  start_y=player_y-(RENDER_DIST/2*GRID_SIZE);
  //start all over again
  for (i=0;i<RDGRID_NUM;i++) {
    RDGrid[i].x=start_x+column*GRID_SIZE;
    RDGrid[i].y=start_y+row*GRID_SIZE;
    if (0<RDGrid[i].x<MAP_WIDTH && //within range
        0<RDGrid[i].y<MAP_HEIGHT) {
      on_grid_id=GetGridId(RDGrid[i].x,RDGrid[i].y,MAP_WIDTH,GRID_SIZE,GRID_NUM);
      //grid
      Grid[on_grid_id].within_render_distance=TRUE;
      render_grids[rendered_grid_num]=on_grid_id;//cannot be i (what if out of bounds)
      rendered_grid_num++;
      //enemy
      for (j=0;j<Grid[on_grid_id].enemy_occupy_num;j++) {
	k=Grid[on_grid_id].enemy_occupy[j];
 	if (!Enemy[k].within_render_distance) {
	  Enemy[k].within_render_distance=TRUE;
          render_enemies[rendered_enemy_num]=k;
	  rendered_enemy_num++;
	}
      }
      //grounds
      for (j=0;j<Grid[on_grid_id].max_ground_num;j++) {
	k=Grid[on_grid_id].ground_ids[j];
 	if (!Ground[k].within_render_distance) {
	  Ground[k].within_render_distance=TRUE;
          render_grounds[rendered_ground_num]=k;
	  rendered_ground_num++;
	}      
      }
    }
    column++;
    if (column>=RENDER_DIST) {
      row++;
      column=0;
    }
  }  
}

void RegainWeb(int web_id)
{
  while (player_web_storage[destroyed_web_pos]!=-1) {//prevent overwrite of existing
    destroyed_web_pos=LimitValue(destroyed_web_pos+1,0,player_max_web_num);
  }
  player_web_storage[destroyed_web_pos]=web_id;
  destroyed_web_pos=LimitValue(destroyed_web_pos+1,0,player_max_web_num);
  placed_web_num--;
}

int PlayerRegen(int souls,int type,bool g_o)
{
  int s=souls,i,j=1;
  bool subtract=FALSE,good_performance=FALSE;
  if (s>0 && !g_o) {
    if ((DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-10)*3/4<=
        player_speed_breaker_recharge_minus) 
    {
      good_performance=TRUE;
    }
    switch (type) {
      case 0: //hp
	if (!good_performance) {
          player_health++;
	} else {
          player_health+=2;
	}
	subtract=TRUE;
        break;
      case 1: //speedbrk
        if (player_speed_breaker_units<player_speed_breaker_max-5) {
	  if (!good_performance) {
            player_speed_breaker_units+=20;
	  } else {
            player_speed_breaker_units+=30;
	  }
	  subtract=TRUE;
        }
        break;
      case 2: //web
	if (good_performance && player_max_web_num<MAX_WEB_NUM-1) {
	  j=2;
	}
	for (i=0;i<j;i++) {
	  if (cdweb_num>0) {//completely destroyed web
	    RegainWeb(cdwebs[cdweb_player_pos]);
            cdweb_player_pos++;
            if (cdweb_player_pos>=player_max_web_num) {
	      cdweb_player_pos=0;
            }
            subtract=TRUE;
            cdweb_num--;
	  } else { //increase max only when there are 0 destroyed webs
            if (player_web_storage[placed_web_pos]==-1) {
              placed_web_pos=player_max_web_num;
            }          
            player_max_web_num++;
          }
	}
        subtract=TRUE;
        break;
    }
    if (subtract) {
      animate_regen_timer=40;
      s--;
    }
  }
  return s;
}

bool NearCrawler()
{
  int i=0,j=0;
  for (i=0;i<rendered_enemy_num;i++) {
    j=render_enemies[i];
    if (Enemy[j].species==1 && Enemy[j].health>0) {
      if (Enemy[j].dist_from_player<=NODE_SIZE*3) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

int GetPerformanceGrade(int t)
{
  int i=0;
  if (combo_streak[t]<player_combo_min_arr[t][3]) {
    for (i=0;i<3;i++) {
      if (player_combo_min_arr[t][i]<=
	    combo_streak[t]<
	      player_combo_min_arr[t][i+1])
      {
        return i;
      }
    }
  } else {
    over_performance_txt=over_performance_txt_arr[t][RandNum(0,2)];
    return 3;
  }
  return -1;
}


bool YesInitRDGrid()
{
  if (GRID_SIZE*2<player_x<MAP_WIDTH-GRID_SIZE*2) {
    if (player_x<RDGrid[0].x+GRID_SIZE*2 || player_x>RDGrid[RENDER_DIST-1].x-GRID_SIZE*2) {
      return TRUE;
    }
  }
  if (GRID_SIZE*2<player_y<MAP_HEIGHT-GRID_SIZE*2) {
    if (player_y<RDGrid[0].y+GRID_SIZE*2 || player_y>RDGrid[RDGRID_NUM-1].y-GRID_SIZE*2) {
      return TRUE;
    }
  }
  return FALSE;
}


void PlayerAct()
{
  int i=0,web_id=0,speed=0,grav_speed=0,claws_l=NODE_SIZE;
  double cur_dist=0,cur_angle=0,grad_x1=0,grad_y1=0,grad_x2=0,grad_y2=0;
  bool allow_act=FALSE;
  //playe cursor
  player_cur_x=mouse.pos.x-cam_x+PLAYER_WIDTH/2;
  player_cur_y=mouse.pos.y-6-cam_y+PLAYER_HEIGHT/2;
  //regen animate
  if (animate_regen_timer>0) {
    animate_regen_timer--;
  }
  //sound
  if (player_snd_dur>0 && sound_on) {
    player_snd_dur=PlaySound(player_snd_dur,player_snd_pitch,player_snd_rand);
  }
  //RDGrid
  if (YesInitRDGrid) {
    InitRDGrid;
  }
  if (player_attack_timer>=0) {
    if (YesLongFade || grav>10) {
      player_attack_strength=2;
    } else {
      player_attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;
    }
    player_attack_timer--;
  }
  if (rst_key_sprint) {
    if (on_ground_id!=-1) {//on ground
      player_speed=3;
      if (IsSpeedBreaking) {
        player_speed=5;
      }
    }
  }
  //speed breaker
  if (!the_bravery_tyrant && IsSpeedBreaking) {
    allow_act=TRUE;
  }
  //^^ condition
  if (allow_act) {
    player_speed_breaker_units--; //speedbreak
    player_speed_breaker_cooldown=player_speed_breaker_cooldown_max;
    if (on_ground_id!=-1) {
      player_speed=5;
      player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT*2;
    }
    if (player_speed_breaker_units==0) {
      rst_speed_break=FALSE;
    }
    sleep_timer=_SLOWDOWN_SLEEP_TIMER;//maiden heaven
  } else {
    sleep_timer=_DEFAULT_SLEEP_TIMER; //normal game speed
    if (player_speed_breaker_cooldown>0) {//at max when speedbreak
      player_speed_breaker_cooldown--;
    }
    if (player_speed_breaker_recharge_cooldown>0) {//auto
      player_speed_breaker_recharge_cooldown--;
    }
    if (player_speed_breaker_cooldown<=0 &&
	player_speed_breaker_units<player_speed_breaker_max) { //Recharging
      if (player_speed_breaker_recharge_cooldown<=0) {
        player_speed_breaker_units++;
	player_speed_breaker_recharge_cooldown=
	  player_speed_breaker_recharge_cooldown_max;
      }
    }
  }
  //tbt alert affects
  if (player_speed_breaker_units>=player_speed_breaker_max || units_since_sb_max>0) {
    units_since_sb_max++; 
  }
  if (sound_on) {
    if (units_since_sb_max==1) {
      Noise(157,63,77);
    }
  }
  if (screen_shake) {
    if (0<units_since_sb_max<23) {
      cam_move_x=RandNum(-7,7);
      cam_move_y=RandNum(-7,7);
    } 
  }
  if (units_since_sb_max>=25 && player_speed_breaker_units<player_speed_breaker_max) {
    units_since_sb_max=0;
  }
  //Clicking
  if (player_bullet_shot!=-1) {
    web_being_shot=player_web_storage[placed_web_pos];
    BulletAct(player_bullet_shot);
  }
  Click;
  if (left_click_hold_timer==62) {//Left click to Attack
    player_attack=TRUE;
    player_blocking=FALSE; //unblock
  }
  if (player_web_type==0 && on_ground_id==-1) {//in air
    player_web_type=1;
  }
  if (player_lock_web_type==0 && player_web_type==1 && on_ground_id!=-1) {
    player_web_type=player_lock_web_type;//go back to mode 0
  }
  int pivot_node_grid_id=0,edge_node_grid_id=0;
  double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0,bm_gradient=0,pivot_x=0,pivot_y=0;
    //angling 
  grad_x1=above_player_x+cam_move_x;
  grad_y1=above_player_y+cam_move_y;
  grad_x2=player_cur_x+cam_move_x;
  grad_y2=player_cur_y+cam_move_y;
  if (!IsNormalView) {//cursor pointing
    cur_dist=GetDistance(above_player_x,above_player_y,player_cur_x,player_cur_y),
    cur_angle=GetCosAngle(player_cur_x-above_player_x,cur_dist);
    if (print_current_above) {
      if (mouse.pos.y-6<GR_HEIGHT/2) {
	if (mouse.pos.x<GR_WIDTH/2) {	//cursor top-left
          grad_x2=above_player_x+cur_dist*Cos(pi-cur_angle+player_angle);
          grad_y2=above_player_y+cur_dist*Sin(pi-cur_angle+player_angle);
        } else {	//cursor top-right
      	  grad_x2=above_player_x-cur_dist*Cos(pi-cur_angle+player_angle);
	  grad_y2=above_player_y-cur_dist*Sin(pi-cur_angle+player_angle);
        }
      } else {
	if (mouse.pos.x<GR_WIDTH/2) {	//cursor below-left
          grad_x2=above_player_x-cur_dist*Cos(cur_angle+player_angle);
      	  grad_y2=above_player_y-cur_dist*Sin(cur_angle+player_angle);
	} else {
      	  grad_x2=above_player_x+cur_dist*Cos(cur_angle+player_angle);
      	  grad_y2=above_player_y+cur_dist*Sin(cur_angle+player_angle);
        }
      }
    } else if (print_current_below) {
      if (mouse.pos.y-6<GR_HEIGHT/2) {
	if (mouse.pos.x<GR_WIDTH/2) {	//cursor top-left
      	  grad_x2=above_player_x+cur_dist*Cos(-cur_angle+player_angle);
	  grad_y2=above_player_y+cur_dist*Sin(-cur_angle+player_angle);
        } else {	//cursor top-right
      	  grad_x2=above_player_x-cur_dist*Cos(-cur_angle+player_angle);
	  grad_y2=above_player_y-cur_dist*Sin(-cur_angle+player_angle);
        }
      } else {
	if (mouse.pos.x<GR_WIDTH/2) {	//cursor below-left
          grad_x2=above_player_x-cur_dist*Cos(pi+cur_angle+player_angle);
          grad_y2=above_player_y-cur_dist*Sin(pi+cur_angle+player_angle);
	} else {
      	  grad_x2=above_player_x+cur_dist*Cos(pi+cur_angle+player_angle);
   	  grad_y2=above_player_y+cur_dist*Sin(pi+cur_angle+player_angle);
	}
      }
    }
    edge_node_grid_id=GetGridId(grad_x2+NODE_SIZE,grad_y2,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
  } else {
    edge_node_grid_id=GetGridId(player_cur_x+NODE_SIZE,player_cur_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
  }
  if (!player_attack_rst) {
    switch (player_web_type) {
      case 0://legacy
        valid_web=FALSE;
        pivot_node_grid_id=GetGridId(above_player_x,above_player_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
        pivot_x=NodeGrid[pivot_node_grid_id].x1+NODE_SIZE/2;
        pivot_y=NodeGrid[pivot_node_grid_id].y1+NODE_SIZE/2;
        edge_x=NodeGrid[edge_node_grid_id].x1+NODE_SIZE/2;
        edge_y=NodeGrid[edge_node_grid_id].y1+NODE_SIZE/2;
        if (pivot_x<edge_x) {
          bm_x1=pivot_x;
          bm_y1=pivot_y;
          bm_x2=edge_x;
          bm_y2=edge_y;
        } else {
          bm_x1=edge_x;
          bm_y1=edge_y;
          bm_x2=pivot_x;
          bm_y2=pivot_y;
        }
        bm_gradient=GetGradient(bm_x1,bm_y1,bm_x2,bm_y2);
        allow_act=FALSE;
        if (player_bullet_shot==-1) {
          if (GetDistance(pivot_x,pivot_y,edge_x,edge_y)<=DEFAULT_PLAYER_SHORT_BUILD_RANGE*NODE_SIZE
	    && !IsCollideSolid(bm_x1,bm_y1,bm_x2,bm_y2,bm_gradient,GetGroundC(bm_x1,bm_y1,bm_gradient))
	  ) {
	  if (!IsCollideCrawler(bm_x1,bm_y1,bm_x2,bm_y2,bm_gradient,GetGroundC(bm_x1,bm_y1,bm_gradient))
              && pivot_x!=edge_x) {
	      allow_act=TRUE;
	    }
	  }
        }
      //^^ condition
        if (allow_act) {
          valid_web=
	  print_valid_web=TRUE;
        } else {
	  print_valid_web=FALSE;
        }    
        allow_act=FALSE;
        if (on_ground_id!=-1 && placed_web_num<player_max_web_num) {
	  allow_act=TRUE;
        }
        if (right_click_hold_timer==62 && allow_act)//Right click to shoot
        {
          if (valid_web) {//place web
            player_attack=TRUE;
	    valid_web=FALSE;
            if (placed_web_pos<player_max_web_num) {      
              while (player_web_storage[placed_web_pos]==-1) {
                placed_web_pos=LimitValue(placed_web_pos+1,0,player_max_web_num);
              }
              web_id=player_web_storage[placed_web_pos];
              if (web_id!=-1) {
                Ground[web_id].x1=bm_x1;
                Ground[web_id].y1=bm_y1;
                Ground[web_id].x2=bm_x2;
                Ground[web_id].y2=bm_y2;
                SetGround(web_id);
                SetNodeGridAttributes(web_id);
	        Ground[web_id].health=5;
                player_web_storage[placed_web_pos]=-1;
                placed_web_pos++;
	        placed_web_num++;
	      }
	      if (placed_web_pos>=player_max_web_num) {
	        placed_web_pos=0;
	      }
            }
          }
        }
        break;
      case 1://current
        allow_act=FALSE;
        if (right_click_hold_timer==62 &&  //Right click to shoot
	    !NearCrawler) {
          if (placed_web_num<player_max_web_num && //webs > 0 
	      player_bullet_shot==-1) {
	    allow_act=TRUE;
	  }
        }
        if (allow_act) {
          player_attack=TRUE; 
	  player_bullet_shot=current_bullet_id;
          int player_bullet_type=0;
          if (YesLongFade) {
	    player_bullet_type=4;
	  }
          ShootBullet(current_bullet_id,
		-1,
		CYAN,
		player_bullet_type,//graphics type
		DEFAULT_PLAYER_BUILD_RANGE,
		1,
		4,
		1,
		-1,
		above_player_x,
		above_player_y,
		grad_x1,
		grad_y1,
		grad_x2,
		grad_y2
          );
	  player_bullet_type=NULL;
          current_bullet_id++;
          if (current_bullet_id>=BULLET_NUM-1) {
            current_bullet_id=0;
          } 
        }
        break;
    }//end switch
  } else {
    if (right_click_hold_timer==62) {
      player_attack=TRUE;
      player_blocking=FALSE; //unblock
    }
  }
  //Trigger movements
  if (rst_left || rst_right) {
    if (rst_left) {
      last_left=TRUE;
    } else {
      last_left=FALSE;
    }
    walk_cycle++; //Walk sprite cycle
    if (walk_cycle>=4) {
      walk_cycle=0;
    }
  } else {
    walk_cycle=0;      
  }
  //Attack mode
  if (player_attack) {
    player_attack=FALSE;
    player_attack_timer=40;
  }
  //Trigger jump
  if (rst_up && 5<=on_ground_timer<=10) {
    jump=TRUE;
    on_ground_timer=0;
    key_jump_timer=player_jump_height;
  }
  if (jump && jump_height==0) {
    jump_height=player_jump_height;
  }
 //Get player grid id
 //player_speed
  for (speed=0;speed<player_speed;speed++) {
    for (grav_speed=0;grav_speed<grav;grav_speed++) {
      on_ground_id=GetOnGroundId(player_x,player_y,5,4,TRUE);    //Get Ground id
   //hiding?
      if (NodeGrid[GetGridId(above_player_x,above_player_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)].node_solid) {
        player_hiding=TRUE;
      } else {
        player_hiding=FALSE;
      }
   //Destroy Ground (regainable)
      if (destroy_ground) {
        if (on_ground_id>=GROUND_NUM && on_ground_id!=web_being_shot) {
          DestroyGround(on_ground_id);  
	  RegainWeb(on_ground_id);
        }
        destroy_ground=FALSE;
      }
   //Ground action
   //on a ground
    //change in ground
      if (saved_ground_id!=on_ground_id) {
      //outwards from ground
        if (0<=Ground[saved_ground_id].height_from_player_x<10) { //above ground
          previous_above=TRUE;
          move_x(-Cos(Ground[saved_ground_id].angle+pi/2));
          move_y(-Sin(Ground[saved_ground_id].angle+pi/2));
        } else if (Ground[saved_ground_id].height_from_player_x>-10 &&
                 Ground[saved_ground_id].height_from_player_x<0) { //below ground
          previous_below=TRUE;
          move_x(-Cos(Ground[saved_ground_id].angle-pi/2));
          move_y(-Sin(Ground[saved_ground_id].angle-pi/2));
        }
      }
      if (on_ground_id!=-1) {
	//
        if (!IsSpeedBreaking) {//reset stats when normal
          player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
	  if (!rst_key_sprint) {
            player_speed=DEFAULT_PLAYER_SPEED;
	  }
        }
        if (Ground[on_ground_id].x1-5<=player_x<=Ground[on_ground_id].x2+5 && //within x
            (Ground[on_ground_id].y1-5<=player_y<=Ground[on_ground_id].y2+5 ||
             Ground[on_ground_id].y2-5<=player_y<=Ground[on_ground_id].y1+5)) {
          player_angle=Ground[on_ground_id].angle;
        //outwards from ground
          if (0<=Ground[on_ground_id].height_from_player_x<10) { //above ground
            current_above=TRUE;
            move_x(-Cos(player_angle+pi/2));
            move_y(-Sin(player_angle+pi/2));
          } else if (Ground[on_ground_id].height_from_player_x>-10 &&
		     Ground[on_ground_id].height_from_player_x<0) { //below ground
            current_below=TRUE;
            move_x(-Cos(player_angle-pi/2));
            move_y(-Sin(player_angle-pi/2));
          }
        //inwards to ground
          if (3<=Ground[on_ground_id].height_from_player_x<10){ //above ground
            move_x(Cos(player_angle+pi/2));
            move_y(Sin(player_angle+pi/2));
          } else {//below ground
	    allow_act=FALSE;
	    if (Ground[on_ground_id].height_from_player_x>-10) {
	      if (Abs(Ground[on_ground_id].gradient)>0.4 &&
                  Ground[on_ground_id].height_from_player_x<-5) {
	        allow_act=TRUE;
              } else if (Abs(Ground[on_ground_id].gradient)<=0.4 &&
		       Ground[on_ground_id].height_from_player_x<-3) {
	        allow_act=TRUE;
	      }
	    }
	    //^^ condition
	    if (allow_act) {
              move_x(Cos(player_angle-pi/2));
              move_y(Sin(player_angle-pi/2));
	    }
          }
          if (!current_above && !current_below) {
            player_angle=0;
            on_ground_id=-1;
          }
          if (current_below && rst_down) { //unstick (un-inwards)
	    player_in_air_timer=20;
	    on_ground_id=-1;
            move_x(-Cos(player_angle-pi/2));
            move_y(-Sin(player_angle-pi/2));
          }
        } else {
      //not on ground
          player_in_air_timer=0;
          on_ground_id=-1;
        }
      } else { //not on a ground
    //fall down normally
        on_ground_id=-1;
        player_angle=0;
      }
    //Y movement
    //Condition to jump
      if (on_ground_id==-1 && rst_down) {
	jump_height=0;
        jump=FALSE;
      }
      if (current_above && on_ground_id!=-1) {
        on_ground_timer=10;
      } else if (current_below) {
        on_ground_timer=2;
        jump_height=0;
        jump=FALSE;
      }
      if (Abs(Ground[on_ground_id].angle)>pi/2-0.01) {
        on_ground_timer=2;
        jump_height=0;
        jump=FALSE;
      }
    //Gravity
      if (grav_speed==0 && speed==0) {
        if (on_ground_id==-1) {
          player_in_air_timer++;
	  if (jump_height==0) {
            if (player_in_air_timer%20==0 && grav<=100) {
              grav++;
	      player_grav=0.5;
            }
	  }
        }
      //Jump movement
        if (key_jump_timer>0) {
          key_jump_timer--;
        } else {
          jump_height=0;
          jump=FALSE;
        }
      }
      if (speed==0) {
        if (jump_height>0) {
          player_grav=1;
          jump_height-=player_grav;
          move_y(-player_grav*2); //jump go against gravity
          if (fall_camera) {
	    cam_move_y+=0.4;
	  }
          if (jump_height<=0) {
            jump=FALSE;
          }
        } else {
          player_grav=0.5;
        }
        if (on_ground_id==-1) {
          if (player_in_air_timer>11) {
            move_y(player_grav); //Falling via gravity
	  } else {
	    move_y(-1);//grav switch
	    if (grav>2) {
	      move_y(1);
            }
          }
        } else { //on ground
          player_in_air_timer=0;
	  if (grav>=3) {
	    if (grav>7) {
	      cam_move_x*=-1;
	    }
	    cam_move_y/=-1.5;
	  }
          grav=2;
          player_grav=0.5;
        }
      }
      if (player_y-PLAYER_HEIGHT/2<0) { //Y axis cap
        move_y(player_grav);
      } else if (player_y+PLAYER_HEIGHT/2>MAP_HEIGHT) {
        move_y(-player_grav);
        player_health--;
      }
     //X movement
      allow_act=FALSE;
      if (on_ground_id==-1) {
	allow_act=TRUE;
      } else if (player_block_timer==0) {
	allow_act=TRUE;
      }
      if (grav_speed==0 && allow_act) {
        if (current_above) {
          if (rst_right) {
            move_x(Cos(player_angle));
            move_y(Sin(player_angle));
          } else if (rst_left) {
            move_x(-Cos(player_angle));
            move_y(-Sin(player_angle));
          }
        } else if (current_below) {
          if (rst_right) {
            move_x(-Cos(player_angle));
            move_y(-Sin(player_angle));
          } else if (rst_left) {
            move_x(Cos(player_angle));
            move_y(Sin(player_angle));
          }
        } else {
          if (rst_left) {
            move_x(-1);
          } else if (rst_right) {
            move_x(1);
          }
        }
      }
     //x-axis cap
      if (player_x-PLAYER_WIDTH/2<0) {
        move_x(1);
      } else if (player_x+PLAYER_WIDTH/2>MAP_WIDTH) {
        move_x(-1);
      }
     //misc
      print_current_above=current_above;
      print_current_below=current_below;
      current_above=
      current_below=
      previous_above=
      previous_below=FALSE;
      saved_ground_id=on_ground_id;
   //Set Character's Axis
      if (print_current_above) {
        above_player_x=player_x+(claws_l)*Cos(player_angle-pi/2);
        above_player_y=player_y+(claws_l)*Sin(player_angle-pi/2);
        above_player_x2=player_x+(claws_l*2)*Cos(player_angle-pi/2);
        above_player_y2=player_y+(claws_l*2)*Sin(player_angle-pi/2);
        if (last_left) {
          player_claws_x=player_x-(claws_l)*Cos(player_angle);
          player_claws_y=player_y-(claws_l)*Sin(player_angle);
        } else {
          player_claws_x=player_x+(claws_l)*Cos(player_angle);
          player_claws_y=player_y+(claws_l)*Sin(player_angle);
        }
      } else if (print_current_below) {
        above_player_x=player_x+(claws_l)*Cos(player_angle+pi/2);
        above_player_y=player_y+(claws_l)*Sin(player_angle+pi/2);
        above_player_x2=player_x+(claws_l*2)*Cos(player_angle+pi/2);
        above_player_y2=player_y+(claws_l*2)*Sin(player_angle+pi/2);
        if (last_left) {
          player_claws_x=player_x+(claws_l)*Cos(player_angle);
          player_claws_y=player_y+(claws_l)*Sin(player_angle);
        } else {
          player_claws_x=player_x-(claws_l)*Cos(player_angle);
          player_claws_y=player_y-(claws_l)*Sin(player_angle);
        }
      } else {
        above_player_x=player_x;
        above_player_y=player_y;
        above_player_x2=player_x;
        above_player_y2=player_y;
        if (last_left) {
         player_claws_x=player_x-(claws_l);
          player_claws_y=player_y;
        } else {
          player_claws_x=player_x+(claws_l);
          player_claws_y=player_y;
        }
      }
    }
  }
 //misc
  if (on_ground_timer>0) {
    on_ground_timer--;
  }
//camshake
  if (!the_bravery_tyrant && IsNormalView) {
    double y_bob=0,x_bob=0;
    if (sprint_bobbing) {  //if sprint_bobbing
      if (on_ground_id!=-1) {//not in air
        if (!player_blocking) {
          if (rst_left || rst_right) {
	    if (rst_key_sprint || player_speed>=5) {
              x_bob=2.5;
	      if (bg_cam_fall_cooldown==0) {
	        cam_move_x+=0.75*RandNum(-2,2);//shaky cam
	        cam_move_y+=0.75*RandNum(-2,2);
	      }
	    } else {
              x_bob=1.5;
	    }
            if (print_current_below) {//upside down
	      x_bob=-Abs(x_bob);
      	    }
      	    if (rst_left) {
              cam_move_x+=x_bob*Cos(player_angle);
              cam_move_y+=x_bob*Sin(player_angle);
      	    }
      	    if (rst_right) {
              cam_move_x-=x_bob*Cos(player_angle);
              cam_move_y-=x_bob*Sin(player_angle);
      	    }
	  }
        }
      }
    }
    if (fall_camera) { //falling_camera
      if (grav>3 || player_speed>=5) { //falling cam effect
        y_bob=(grav-2)/2;
        switch (player_speed) {
	  case 1:
	  case 2:
            x_bob=RandNum(1,2)*0.5;//move x	
	    break;
	  case 3:
            x_bob=RandNum(1,2);//move x		
	    break;
        }
        if (rst_left) {
	  cam_move_x+=x_bob;
        }
        if (rst_right) {
	  cam_move_x-=x_bob;
        }
        cam_move_y-=y_bob;//increase y
        if (grav>5 || player_speed>=5) {
	  cam_move_x+=RandNum(-2,2);//shaky x
          cam_move_y+=RandNum(-2,2);//shaky y
        }
      }
    }
    x_bob=NULL;
    y_bob=NULL;
  }
  for (i=0;i<Abs(cam_move_x);i++) { //cam stablizer
    if (cam_move_x>0) {
      cam_move_x-=0.1;
    } else if (cam_move_x<0) {
      cam_move_x+=0.1;
    }
  }
  for (i=0;i<Abs(cam_move_y);i++) {
    if (cam_move_y>0) {
      cam_move_y-=0.1;
    }  else if(cam_move_y<0) {
      cam_move_y+=0.1;
    }
  }
  if (-0.1<=cam_move_x<=0.1) { //prevent spam shake
    cam_move_x=0;
  }
  if (-0.1<=cam_move_y<=0.1) {
    cam_move_y=0;
  }
  if (bg_cam_fall_cooldown==0) {
    background_cam_move_x=cam_move_x/2;
    background_cam_move_y=cam_move_y/2;
  } else if (bg_cam_fall_cooldown<=46) {
    if (background_cam_move_x<cam_move_x/2) {
      background_cam_move_x+=1;
    } else {
      background_cam_move_x-=1;
    }
    if (background_cam_move_y<cam_move_y/2) {
      background_cam_move_y+=1;
    } else {
      background_cam_move_y-=1;
    }  
    if (cam_move_x/2-1<=background_cam_move_x<=cam_move_x/2+1 &&
	cam_move_y/2-1<=background_cam_move_y<=cam_move_y/2+1) {
      bg_cam_fall_cooldown--;
    }
  } else if (47<=bg_cam_fall_cooldown<=48) {
    background_cam_move_x/=-1;
    if (rst_left||rst_right) {
      background_cam_move_y/=-1.5;
    } else {
      background_cam_move_y/=-3.5;
    }
  } else {
    if (background_cam_move_x<cam_move_x) {
      background_cam_move_x+=1;
    } else {
      background_cam_move_x-=1;
    }
    if (background_cam_move_y<cam_move_y) {
      background_cam_move_y+=1;
    } else {
      background_cam_move_y-=1;
    }  
  }
 //
 //
  if (grav>4) {
    bg_cam_fall_cooldown=50;
  } else if (bg_cam_fall_cooldown>0) {
    bg_cam_fall_cooldown--;
  }
  if (grav>5) {
    player_in_air_cooldown+=5;
    if (player_in_air_cooldown>20) {
      player_in_air_cooldown=20;
    }
  } else if (player_in_air_cooldown>0) {
    player_in_air_cooldown--;
  }
 //player got hit
  if (player_hit_cooldown_timer>0) {
    player_hit_cooldown_timer--;
    if (player_hit_cooldown_timer==player_hit_cooldown_timer_max-1) {
      performance_type=-1;
      for (i=0;i<COMBO_NUM;i++) {
        combo_timer[i]=0;
      }
    }
    if (screen_shake) {
      if (player_hit_cooldown_timer>player_hit_cooldown_timer_max-25) {//hurt
        cam_move_x=RandNum(-20,20);
        cam_move_y=RandNum(-20,20);
      }
    }
  }
  //hit enemy
  if (screen_shake) {
    if (enemy_hit_global_timer>0) {
      enemy_hit_global_timer--;
      cam_move_x=RandNum(-5,5);
      cam_move_y=RandNum(-5,5);
    } else if (enemy_hit_global_timer==0) {
      enemy_hit_global_timer=-1;
    }
  }
  //
  //game rating is abve 0
  if (player_speed_breaker_recharge_minus>0) {
    if (game_rating_timer>0) {//gradually decrease game rating timer
      game_rating_timer--;
    } else {//game rating timer at 0
      player_speed_breaker_recharge_minus--;//decrease game rating
      game_rating_timer=game_rating_timer_max;
    }
  } else {//game rating is at 0
    game_rating_timer=game_rating_timer_max;
  }
  if (player_speed_breaker_recharge_minus>=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-1) {
    player_speed_breaker_recharge_minus=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-1;
  }
  player_speed_breaker_recharge_cooldown_max=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-player_speed_breaker_recharge_minus;
//comboing
  for (i=0;i<COMBO_NUM;i++) {
    if (combo_timer[i]>0) {
      if (!the_bravery_tyrant) {
        combo_timer[i]--;
      }
      performance_grade_arr[i]=GetPerformanceGrade(i);
    } else if (combo_timer[i]==0) {
      performance_type=-1;
      if (performance_grade_arr[i]>-1) {
	performance_type=i;
	int psbrm_add=0;
      //Addition part 1
	if (performance_grade_arr[performance_type]<3) {
          psbrm_add=player_sbrm_reward[performance_type][performance_grade_arr[performance_type]];
	} else {
          psbrm_add=player_sbrm_reward[performance_type][2];
	}
      //Addition part 2
	switch (performance_type) {
	  case 0://dodge
	    switch (performance_grade_arr[0]) {
	      case 0:
	        psbrm_add+=combo_streak[0]*2;
	        break;
	      case 1:
	        psbrm_add+=(combo_streak[0]-15)*2;
		break;
	    }
	    break;
	  case 1://hit
	    if (performance_grade_arr[1]>=2) {
	      psbrm_add+=(combo_streak[1]-25)*2;
            }
	    break;
        }
	//subtraction
        if (player_hit_cooldown_timer>0) {
	  psbrm_add=psbrm_add/4;
        }
        player_speed_breaker_recharge_minus+=psbrm_add;
	if (player_hit_cooldown_timer==0) {
          if (sound_on) {
	    switch (performance_grade_arr[performance_type]) {
	      case 0:
                Noise(100,40,60);
		break;
	      case 1:
                Noise(100,50,70);
		break;
	      case 2:
                Noise(150,63,90);
		break;
	      case 3:
        	Noise(157,63,77);
                //Noise(250,40,97);
		break;
	    }
          }
          performance_txt_timer=PERFORMANCE_TXT_TIMER_MAX;
	}
      }
      combo_streak[i]=0;
      combo_timer[i]=-1;
    }
  }
//block
  allow_act=FALSE;
  if (player_attack_timer<=0) {
    if (rst_down) {
      if (on_ground_id==-1 || print_current_above) {
        allow_act=TRUE;
      }
    } else if (rst_up && print_current_below) {
      allow_act=TRUE;
    }
  }
  if (allow_act) {
    player_blocking=TRUE;
  } else {
    player_blocking=FALSE;
  }
  if (player_blocking) {
    if (player_block_health>0) {//above 0
      player_block_cooldown=player_block_cooldown_max;
      player_block_recharge_timer=player_block_recharge_timer_max;
      if (player_block_timer<30) {
        player_block_timer++;
      }
    } else { //below 0
      if (player_block_health<=0) {
        player_block_health=0;
      }
      player_blocking=FALSE; //disable blocking
    }
  } else {//player not blocking -regen block
    if (player_block_timer>0) {
      player_block_timer--;
    }
    if (player_block_cooldown>0) {
      player_block_cooldown--;
    } else {//3 seconds has passed
      if (player_block_recharge_timer>0) {
	player_block_recharge_timer--;
      } else if (player_block_health<player_block_health_max) {//below max
        player_block_health++;
	player_block_recharge_timer=player_block_recharge_timer_max;
      }
    }
  }
//For rotate view
  /*player_E=GetRotateAngle(player_x,player_y);
  if (on_ground_timer>0) {
    if (print_current_above) {
      player_E_x=GetDistance(player_x,player_y,0,0)*Cos(player_E-player_angle);
      player_E_y=GetDistance(player_x,player_y,0,0)*Sin(player_E-player_angle);
    } else if (print_current_below) {
      player_E_x=GetDistance(player_x,player_y,0,0)*Cos(pi+player_E-player_angle);
      player_E_y=GetDistance(player_x,player_y,0,0)*Sin(pi+player_E-player_angle);
    }
    if (!change_view) {//normal view
      if (print_current_above) {
        player_sprite_E=player_angle;
      } else if (print_current_below) {
        player_sprite_E=pi+player_angle;
      } else {
        player_sprite_E=0;
      }
    }
  }*/
  //PlayerSpriteMulti
  if (fade_player) {
  if (YesLongFade2) {
    psm_hold_timer++;
    if (!the_bravery_tyrant) {
      if (IsSpeedBreaking) {
        psmht_max=5;
        cpsm_max=5;
      } else if (YesLongFade) { 
        psmht_max=5;
        cpsm_max=5;
      } else if (grav>10) {//long blur
        psmht_max=5;
        cpsm_max=5;
      } else {//short blur
        psmht_max=2;
        cpsm_max=5;
      }
    } else {//tbt
      psmht_max=5;
      cpsm_max=PSM_NUM;
    }
    if (psm_hold_timer>psmht_max) {
      psm_hold_timer=0;
      if (!the_bravery_tyrant) {//normal
        PSM[current_psm].appear_timer=50;
      } else {//shutoff all
        PSM[current_psm].appear_timer=5;
      }
      if (on_ground_timer<=0) {
        PSM[current_psm].angle=0;
	PSM[current_psm].is_jump=TRUE;
      } else {
        PSM[current_psm].angle=player_sprite_E;
  	    PSM[current_psm].is_jump=FALSE;
      }
      if (!the_bravery_tyrant) {
        PSM[current_psm].x=player_x+cam_move_x;
        PSM[current_psm].y=player_y+cam_move_y;    
      } else {
        PSM[current_psm].x=player_x;
        PSM[current_psm].y=player_y;    
      }
      PSM[current_psm].walk_cycle=walk_cycle;
      PSM[current_psm].attack_timer=player_attack_timer;
      PSM[current_psm].is_left=last_left;
      PSM[current_psm].block_timer=player_block_timer;
      current_psm++;
      if (current_psm>=cpsm_max) {
        current_psm=0;
      }
    }
  }
  for (i=0;i<cpsm_max;i++) {
    PSM[i].sprite_x=PSM[i].x+cam_x-PLAYER_WIDTH/2;
    PSM[i].sprite_y=PSM[i].y+cam_y-PLAYER_HEIGHT/2;
    if (PSM[i].appear_timer>0 && !the_bravery_tyrant) {
      PSM[i].appear_timer--;
    }    
  }
  }
  player_sprite_x=GR_WIDTH/2+cam_move_x;
  player_sprite_y=GR_HEIGHT/2+cam_move_y;
 //Debug
  //print_on_ground_id=on_ground_id;
}

void PlayerCameraInit()
{
//set camera
  cam_x=0;
  cam_y=0;
  bg_cam_fall_cooldown=0;
  background_cam_move_x=0;
  background_cam_move_y=0;
  CameraInit(saved_player_x-PLAYER_WIDTH/2,saved_player_y-PLAYER_HEIGHT/2);
}

void PlayerInit()
{
//Set default player conditions
  int i=0;
  player_sprite_x=GR_WIDTH/2;
  player_sprite_y=GR_HEIGHT/2;
  psmht_max=5;
  units_since_sb_max=0;
//Dynamic Player Attributes
  player_speed_breaker_recharge_cooldown=
  player_speed_breaker_recharge_cooldown_max=DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN;
  player_speed_breaker_cooldown_max=
  player_speed_breaker_cooldown=DEFAULT_PLAYER_SPEED_BREAKER_COOLDOWN;
//blocking
  player_block_timer=0;
  player_blocking=FALSE;
  player_block_recharge_timer=0,
  player_block_cooldown=0,
  player_block_health=DEFAULT_PLAYER_BLOCK_HEALTH_MAX,
//
  player_speed_breaker_units=0;
  player_speed_breaker_max=DEFAULT_PLAYER_SPEED_BREAKER_MAX;
  player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
  player_attack_strength=DEFAULT_PLAYER_ATTACK_STRENGTH;
  player_health=DEFAULT_PLAYER_HEALTH;
  player_speed=DEFAULT_PLAYER_SPEED;
  player_knockback_strength=DEFAULT_PLAYER_KNOCKBACK_STRENGTH;
  player_start_x=player_x=saved_player_x;
  player_start_y=player_y=saved_player_y;
  //Web
  player_max_web_num=DEFAULT_PLAYER_WEB_NUM;
  player_web_health=DEFAULT_PLAYER_WEB_HEALTH;
  player_build_range=DEFAULT_PLAYER_BUILD_RANGE;
//Game function attributes
  player_in_air_cooldown=0;
  enemy_hit_global_timer=0;
  animate_regen_timer=0;
  regen_type=0;
  sleep_timer=_DEFAULT_SLEEP_TIMER;
  grav=2;
  player_in_air_timer=0;
  player_knockback_speed=0.5;
  player_knockback_speed_multiplier=2;
  jump_height=0;
  player_grav=0.5;
//
  player_snd_dur=0;
  player_snd_pitch=DEFAULT_PLAYER_SND_PITCH;
  player_snd_rand=DEFAULT_PLAYER_SND_RAND;
//
  on_ground_timer=0;
  on_ground_id=-1;
  saved_ground_id=-1;
  player_attack_timer=0;
  enemy_kills=0;
  //Web
  placed_web_num=0;
  placed_web_pos=0;
  destroyed_web_pos=0;
  for (i=0;i<MAX_WEB_NUM;i++) {
    player_web_storage[i]=GROUND_NUM+i;
  }
  //cdWeb
  cdweb_num=0;
  cdweb_pos=0;
  cdweb_player_pos=0;
  for (i=0;i<MAX_WEB_NUM;i++) {
    cdwebs[i]=-1;
  }
  //bullet_shot_arr
  /*for (i=0;i<MAX_WEB_NUM;i++) {
    player_shot_bullet_arr[i]=-1;
  }*/
  //misc
  player_cur_x=mouse.pos.x-cam_x+PLAYER_WIDTH/2;
  player_cur_y=mouse.pos.y-6-cam_y+PLAYER_HEIGHT/2;
  player_bullet_shot=-1;
  web_being_shot=-1;
  player_E=0;
  player_E_x=0;
  player_E_y=0;
  player_sprite_E=0;
  above_player_x=above_player_x2=player_x;
  above_player_y=above_player_y2=player_y;
  rendered_grid_num=0;
  rendered_enemy_num=0;
  rendered_ground_num=0;
  for (i=0;i<RDGRID_NUM;i++) {
    render_grids[i]=-1;
  }
  for (i=0;i<ENEMY_NUM;i++) {
    render_enemies[i]=-1;
  }
  for (i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {
    render_grounds[i]=-1;
  }
  //bool
  player_hiding=FALSE;
  player_attack=FALSE;
  uppercut=FALSE;
  valid_web=FALSE;
  jump=FALSE;
  change_view=FALSE;
  rst_speed_break=FALSE;
  //move
  rst_left=FALSE,
  last_left=FALSE;
  rst_right=FALSE;
  rst_up=FALSE;
  rst_down=FALSE;
  rst_key_sprint=FALSE;
  player_attack_rst=FALSE;
  //Ground
  print_current_above=FALSE;
  print_current_below=FALSE;
  current_above=FALSE;
  current_below=FALSE;
  previous_above=FALSE;
  previous_below=FALSE;
  destroy_ground=FALSE;
  //Other init
  PlayerCameraInit;
  InitRDGrid;
//
  current_psm=0;
  cpsm_max=5;
  psm_hold_timer=0;
  for (i=0;i<PSM_NUM;i++) {
    PSM[i].appear_timer=0;
    PSM[i].is_jump=FALSE;
    PSM[i].is_left=FALSE;
    PSM[i].walk_cycle=0;
    PSM[i].attack_timer=0;
    PSM[i].block_timer=0;
    PSM[i].angle=0;
    PSM[i].x=player_x;
    PSM[i].y=player_y;
    PSM[i].sprite_x=PSM[i].x+cam_x-PLAYER_WIDTH;
    PSM[i].sprite_y=PSM[i].y+cam_y-PLAYER_HEIGHT;
  }
}

/*void DrawPlayerSpriteMulti(CTask *,CDC *dc)
{
  int *matrix,i=0;
  if (!change_view) {
    for (i=0;i<cpsm_max;i++) {
      matrix=Mat4x4IdentNew;
      if (PSM[i].is_left) {//normal view
    	Mat4x4RotY(matrix,pi);
      }
      Mat4x4RotZ(matrix,PSM[i].angle);
      if (PSM[i].appear_timer>0) {
	dc->color=player_color+custom_map_background_color<<16+ROPF_DITHER;
        if (PSM[i].attack_timer<=0) {
	  if (!PSM[i].is_jump) {
	    if (PSM[i].block_timer<=0) {
              if (PSM[i].walk_cycle<2) {
                Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<1>",BI=1$,matrix);
              } else {
                Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<2>",BI=2$,matrix);
              }
	    } else {
              if (0<PSM[i].block_timer<=5) {
          	Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<8>",BI=8$,matrix);
              } else if (5<PSM[i].block_timer<=10) {
      	        Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<9>",BI=9$,matrix);
  	      } else {
          	Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<10>",BI=10$,matrix);
  	      }
            }
	  } else {
	    if (PSM[i].block_timer<=0) {
              Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<3>",BI=3$,matrix);
	    } else {
              if (0<PSM[i].block_timer<=5) {
                Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<11>",BI=11$,matrix);
              } else if (5<PSM[i].block_timer<=10) {
                Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<12>",BI=12$,matrix);
  	      } else {
                Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<13>",BI=13$,matrix);
	      }
	    }
	  }
        } else {
          if (30<PSM[i].attack_timer<=40) {//attack sprite
            Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<4>",BI=4$,matrix);
          } else if (20<PSM[i].attack_timer<=30) {
            Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<5>",BI=5$,matrix);
          } else if (10<PSM[i].attack_timer<=20) {
            Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<6>",BI=6$,matrix);
          } else if (0<PSM[i].attack_timer<=10) {
            Sprite3Mat4x4B(dc,PSM[i].sprite_x,PSM[i].sprite_y,0,$IB,"<7>",BI=7$,matrix);
          }
        }
      }
      Free(matrix);     
    }
  }
}*/


