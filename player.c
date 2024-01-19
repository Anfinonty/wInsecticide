//Player

void move_x(double x) {
  player.x+=x;
  player.cam_x-=x;
}


void move_y(double y) {
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
  //bg_cam_fall_cooldown=0;
  //background_cam_move_x=0;
  //background_cam_move_y=0;
  CameraInit(player.saved_x,player.saved_y+PLAYER_HEIGHT/2); //idk scaling is weird for sprite
}


void CleanUpPlayer()
{
  DeleteObject(player.sprite_1_cache);
  DeleteObject(player.sprite_2_cache);
}


void InitPlayer() {
  int i;
  //CleanUpPlayer();
  player.saved_x=saved_player_x;
  player.saved_y=saved_player_y;

  player.rst_down=FALSE;
  player.rst_left=FALSE;
  player.rst_right=FALSE;
  player.rst_up=FALSE;
  player.last_left=FALSE;
  player.rst_key_sprint=FALSE;

  player.hiding=FALSE;

  player.key_jump_timer=0;
  player.cam_move_x=0;
  player.cam_move_y=0;
  player.angle=0;
  player.x=player.saved_x;
  player.y=player.saved_y;

  player.above_x=player.above_x2=player.x;
  player.above_y=player.above_y2=player.y;

  player.player_jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
  player.jump_height=0;
  player.speed=DEFAULT_PLAYER_SPEED;

//
  player.sprite_angle=0;
  player.walk_cycle=0;
  player.in_air_timer=0;
  player.grav=2;
  player.jump_height=0;
  player.player_grav=0.5;

  player.on_ground_timer=0;
  player.on_ground_id=-1;
  player.saved_ground_id=-1;

  player.saved_angle=0;

  player.sprite_x=GR_WIDTH/2;
  player.sprite_y=GR_HEIGHT/2;

  //player.in_air_cooldown=0;

//  player_attack_timer=0;
//  enemy_kills=0;


//Bool
  //player.hiding=FALSE;
  //player.attack=FALSE;
  //player.uppercut=FALSE;
  //player.valid_web=FALSE;
  player.jump=FALSE;
  //player.change_view=FALSE;
  //player.rst_speed_break=FALSE;
  
//Ground
  player.print_current_above=FALSE;
  player.print_current_below=FALSE;
  player.current_above=FALSE;
  player.current_below=FALSE;
  player.previous_above=FALSE;
  player.previous_below=FALSE;
  //player.destroy_ground=FALSE;

  player.rendered_grid_num=0;
  player.rendered_vgrid_num=0;
  player.rendered_enemy_num=0;
  player.rendered_ground_num=0;

  for (i=0;i<RDGRID_NUM;i++) {
    player.render_grids[i]=-1;
  }
  for (i=0;i<VRDGRID_NUM;i++) {
    player.render_vgrids[i]=-1;
  }
  for (i=0;i<ENEMY_NUM;i++) {
    player.render_enemies[i]=-1;
  }
  for (i=0;i<GROUND_NUM/*+MAX_WEB_NUM*/;i++) {
    player.render_grounds[i]=-1;
  }
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


void PlayerAct() {
  //Initialize RD Grid
  if (YesInitRDGrid()) {
    InitRDGrid();
  }
  if (YesInitVRDGrid()) {
    InitVRDGrid();
  }

  //Sprinting
  if (player.rst_key_sprint) {
    if (player.on_ground_id!=-1) {//on ground
      player.speed=3;
      /*if (IsSpeedBreaking()) {
        player_speed=5;
      }*/
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

  //Trigger jump
  if (player.rst_up && 5<=player.on_ground_timer && player.on_ground_timer<=10) {
    player.jump=TRUE;
    player.on_ground_timer=0;
    player.key_jump_timer=player.player_jump_height;
  }
  if (player.jump && player.jump_height==0) {
    player.jump_height=player.player_jump_height;
  }


 //Get player grid id
 //player.speed
  int i=0,speed=0,grav_speed=0,claws_l=NODE_SIZE;
  double cur_dist=0,cur_angle=0;//,grad_x1=0,grad_y1=0,grad_x2=0,grad_y2=0;
  bool allow_act=FALSE;
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
      /*if (destroy_ground) {
        if (on_ground_id>=GROUND_NUM && on_ground_id!=web_being_shot) {
          DestroyGround(on_ground_id);  
	  RegainWeb(on_ground_id);
        }
        destroy_ground=FALSE;
      }*/

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
      if (player.on_ground_id!=-1) {
	//
        //if (!IsSpeedBreaking()) {//reset stats when normal
          //player.jump_height=DEFAULT_PLAYER_JUMP_HEIGHT;
	      if (!player.rst_key_sprint) {
            player.speed=DEFAULT_PLAYER_SPEED;
	      }
        //}
        if ((Ground[player.on_ground_id].x1-5<=player.x && player.x<=Ground[player.on_ground_id].x2+5) && //within x
            ((Ground[player.on_ground_id].y1-5<=player.y && player.y<=Ground[player.on_ground_id].y2+5) ||
             (Ground[player.on_ground_id].y2-5<=player.y && player.y<=Ground[player.on_ground_id].y1+5))) {
          player.angle=Ground[player.on_ground_id].angle;
        //outwards from ground
          if (0<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10) { //above ground
            player.current_above=TRUE;
            move_x(-cos(player.angle+M_PI_2));
            move_y(-sin(player.angle+M_PI_2));
          } else if (Ground[player.on_ground_id].height_from_player_x>-10 &&
		     Ground[player.on_ground_id].height_from_player_x<0) { //below ground
            player.current_below=TRUE;
            move_x(-cos(player.angle-M_PI_2));
            move_y(-sin(player.angle-M_PI_2));
          }
        //inwards to ground
          if (3<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10){ //above ground
            move_x(cos(player.angle+M_PI_2));
            move_y(sin(player.angle+M_PI_2));
          } else {//below ground
	        allow_act=FALSE;
	        if (Ground[player.on_ground_id].height_from_player_x>-10) {
	          if (abs(Ground[player.on_ground_id].gradient)>0.4 &&
                Ground[player.on_ground_id].height_from_player_x<-5) {
	            allow_act=TRUE;
              } else if (abs(Ground[player.on_ground_id].gradient)<=0.4 &&
		        Ground[player.on_ground_id].height_from_player_x<-3) {
	            allow_act=TRUE;
	          }
	        }
	        //^^ condition
	        if (allow_act) {
              move_x(cos(player.angle-M_PI_2));
              move_y(sin(player.angle-M_PI_2));
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
          }
        } else {
      //not on ground
          player.in_air_timer=0;
          player.on_ground_id=-1;
        }
      } else { //not on a ground
    //fall down normally
        player.on_ground_id=-1;
        player.angle=0;
      }
    //Y movement
    //Condition to jump
      if (player.on_ground_id==-1 && player.rst_down) {
	    player.jump_height=0;
        player.jump=FALSE;
      }
      if (player.current_above && player.on_ground_id!=-1) {
        player.on_ground_timer=10;
      } else if (player.current_below) {
        player.on_ground_timer=2;
        player.jump_height=0;
        player.jump=FALSE;
      }
      if (abs(Ground[player.on_ground_id].angle)>M_PI_2-0.01) {
        player.on_ground_timer=2;
        player.jump_height=0;
        player.jump=FALSE;
      }
    //Gravity
      if (grav_speed==0 && speed==0) {
        if (player.on_ground_id==-1) {
          player.in_air_timer++;
	      if (player.jump_height==0) {
            if (player.in_air_timer%20==0 && player.grav<=100) {
              player.grav++;
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
      if (speed==0) {
        if (player.jump_height>0) {
          player.player_grav=1;
          player.jump_height-=player.player_grav;
          move_y(-player.player_grav*2); //jump go against gravity
          /*if (fall_camera) {
	        cam_move_y+=0.4;
	      }*/
          if (player.jump_height<=0) {
            player.jump=FALSE;
          }
        } else {
          player.player_grav=0.5;
        }
        if (player.on_ground_id==-1) {
          if (player.in_air_timer>11) {
            move_y(player.player_grav); //Falling via gravity
	      } else {
	        move_y(-1);//grav switch
	        if (player.grav>2) {
	          move_y(1);
            }
          }
        } else { //on ground
          player.in_air_timer=0;
	      if (player.grav>=3) {
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
        //player.health--;
      }
     //X movement
      //allow_act=FALSE;
      //if (player.on_ground_id==-1) { //player is not on ground
	  //  allow_act=TRUE;
      //} /*else if (player.block_timer==0) { //OR player is not blocking
	  //  allow_act=TRUE;
      //}*/


      if (grav_speed==0 /*&& allow_act*/) {
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
     //x-axis cap
      if (player.x-PLAYER_WIDTH/2<0) {
        move_x(1);
      } else if (player.x+PLAYER_WIDTH/2>MAP_WIDTH) {
        move_x(-1);
      }
     //misc
      player.print_current_above=player.current_above;
      player.print_current_below=player.current_below;


      if (player.current_above) {
        player.sprite_angle=player.angle;
      } else if (player.current_below) {
        player.sprite_angle=M_PI+player.angle;
      } else {
        player.sprite_angle=0;
      }

      if (!player.last_left) {
        player.sprite_angle*=-1;
      }

      player.current_above=FALSE;
      player.current_below=FALSE;
      player.previous_above=FALSE;
      player.previous_below=FALSE;
      player.saved_ground_id=player.on_ground_id;
   //Set Character's Axis
      if (player.print_current_above) {
        player.above_x=player.x+(claws_l)*cos(player.angle-M_PI/2);
        player.above_y=player.y+(claws_l)*sin(player.angle-M_PI/2);
        player.above_x2=player.x+(claws_l*2)*cos(player.angle-M_PI/2);
        player.above_y2=player.y+(claws_l*2)*sin(player.angle-M_PI/2);
        /*if (last_left) {
          player.claws_x=player.x-(claws_l)*Cos(player.angle);
          player.claws_y=player.y-(claws_l)*Sin(player.angle);
        } else {
          player.claws_x=player.x+(claws_l)*Cos(player.angle);
          player.claws_y=player.y+(claws_l)*Sin(player.angle);
        }*/
      } else if (player.print_current_below) {
        player.above_x=player.x+(claws_l)*cos(player.angle+M_PI/2);
        player.above_y=player.y+(claws_l)*sin(player.angle+M_PI/2);
        player.above_x2=player.x+(claws_l*2)*cos(player.angle+M_PI/2);
        player.above_y2=player.y+(claws_l*2)*sin(player.angle+M_PI/2);
        /*if (last_left) {
          player.claws_x=player.x+(claws_l)*Cos(player.angle);
          player.claws_y=player.y+(claws_l)*Sin(player.angle);
        } else {
          player.claws_x=player.x-(claws_l)*Cos(player.angle);
          player.claws_y=player.y-(claws_l)*Sin(player.angle);
        }*/
      } else {
        player.above_x=player.x;
        player.above_y=player.y;
        player.above_x2=player.x;
        player.above_y2=player.y;
        /*if (last_left) {
         player.claws_x=player.x-(claws_l);
          player.claws_y=player.y;
        } else {
          player.claws_x=player.x+(claws_l);
          player.claws_y=player.y;
        }*/
      }
    }
  }
 //misc
  if (player.on_ground_timer>0) {
    player.on_ground_timer--;
  }
 //camshake

  /*player.sprite_angle+=0.1;
  if (player.sprite_angle>M_PI*2) {
    player.sprite_angle=0;
  }*/
  player.sprite_x=GR_WIDTH/2+player.cam_move_x;
  player.sprite_y=GR_HEIGHT/2+player.cam_move_y-PLAYER_HEIGHT/2;
 //
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
        //if (!player_blocking) {
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
        //}
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
  /*if (bg_cam_fall_cooldown==0) {
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
  }*/
 //
 //
  /*if (grav>4) {
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
  }*/

}

void DrawPlayer(HDC hdc) {
  //GrRect(hdc,player.x-PLAYER_WIDTH,player.y-PLAYER_HEIGHT,PLAYER_WIDTH,PLAYER_HEIGHT,RGB(34,139,34));
  if (player.sprite_angle!=player.saved_angle) {
    DeleteObject(player.sprite_1_cache);
    DeleteObject(player.sprite_2_cache);
    player.sprite_1_cache = RotateSprite(hdc, player.sprite_1,player.sprite_angle,LTGREEN,BLACK);
    player.sprite_2_cache = RotateSprite(hdc, player.sprite_2,player.sprite_angle,LTGREEN,BLACK);

    player.saved_angle=player.sprite_angle;
  }
  if (player.on_ground_timer>0) {
    if (player.walk_cycle<2) {
      GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_1_cache,player.last_left);
    } else {
      GrSprite(hdc,player.sprite_x,player.sprite_y,player.sprite_2_cache,player.last_left);
    }
  } else { //in_air
    GrSprite(hdc,player.sprite_x,player.sprite_y-6,player.sprite_jump_cache,player.last_left);
    //DrawSprite(hdc,player.sprite_x,player.sprite_y-6,player_sprite_jump_cache);
  }
}

