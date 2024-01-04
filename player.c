bool once=true;
//Player

void move_x(double x) {
  player.x+=x;
}


void move_y(double y) {
  player.y+=y;
}

void InitPlayer() {
  player.rst_down=false;
  player.rst_left=false;
  player.rst_right=false;
  player.rst_up=false;
  player.last_left=false;


  player.angle=0;
  player.x=GR_WIDTH/2;
  player.y=GR_HEIGHT/2;
  player.WIDTH=PLAYER_WIDTH;
  player.HEIGHT=PLAYER_HEIGHT;
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


  //player.in_air_cooldown=0;

//  player_attack_timer=0;
//  enemy_kills=0;


//Bool
  //player.hiding=false;
  //player.attack=false;
  //player.uppercut=false;
  //player.valid_web=false;
  player.jump=false;
  //player.change_view=false;
  //player.rst_speed_break=false;
  
//Ground
  //player.print_current_above=false;
  //player.print_current_below=false;
  player.current_above=false;
  player.current_below=false;
  player.previous_above=false;
  player.previous_below=false;
  //player.destroy_ground=false;

  if (once) {
    player.sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.sprite_jump = (HBITMAP) LoadImageW(NULL, L"sprites/player3-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    once=false;
  }
}

void PlayerAct() {
  //Trigger movements
  if (player.rst_left || player.rst_right) {
    if (player.rst_left) {
      player.last_left=true;
    } else {
      player.last_left=false;
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
    player.jump=true;
    player.on_ground_timer=0;
    player.key_jump_timer=player.player_jump_height;
  }
  if (player.jump && player.jump_height==0) {
    player.jump_height=player.player_jump_height;
  }


 //Get player grid id
 //player.speed
  int i=0,speed=0,grav_speed=0;
  double cur_dist=0,cur_angle=0;//,grad_x1=0,grad_y1=0,grad_x2=0,grad_y2=0;
  bool allow_act=false;
  for (speed=0;speed<player.speed;speed++) {
    for (grav_speed=0;grav_speed<player.grav;grav_speed++) {
      player.on_ground_id=GetOnGroundId(player.x,player.y,5,4,true);    //Get Ground id
   //hiding?
      /*if (NodeGrid[GetGridId(above_player.x,above_player.y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)].node_solid) {
        player.hiding=TRUE;
      } else {
        player.hiding=false;
      }*/
   //Destroy Ground (regainable)
      /*if (destroy_ground) {
        if (on_ground_id>=GROUND_NUM && on_ground_id!=web_being_shot) {
          DestroyGround(on_ground_id);  
	  RegainWeb(on_ground_id);
        }
        destroy_ground=false;
      }*/

   //Ground action
   //on a ground
    //change in ground
      if (player.saved_ground_id!=player.on_ground_id) {
      //outwards from ground
        if (0<=Ground[player.saved_ground_id].height_from_player_x && Ground[player.saved_ground_id].height_from_player_x<10) { //above ground
          player.previous_above=true;
          move_x(-cos(Ground[player.saved_ground_id].angle+M_PI_2));
          move_y(-sin(Ground[player.saved_ground_id].angle+M_PI_2));
        } else if (Ground[player.saved_ground_id].height_from_player_x>-10 &&
                 Ground[player.saved_ground_id].height_from_player_x<0) { //below ground
          player.previous_below=true;
          move_x(-cos(Ground[player.saved_ground_id].angle-M_PI_2));
          move_y(-sin(Ground[player.saved_ground_id].angle-M_PI_2));
        }
      }
      if (player.on_ground_id!=-1) {
	//
        /*if (!IsSpeedBreaking) {//reset stats when normal
          player.jump_height=DEFAULT_player.JUMP_HEIGHT;
	      if (!rst_key_sprint) {
            player.speed=DEFAULT_player.SPEED;
	      }
        }*/
        if ((Ground[player.on_ground_id].x1-5<=player.x && player.x<=Ground[player.on_ground_id].x2+5) && //within x
            ((Ground[player.on_ground_id].y1-5<=player.y && player.y<=Ground[player.on_ground_id].y2+5) ||
             (Ground[player.on_ground_id].y2-5<=player.y && player.y<=Ground[player.on_ground_id].y1+5))) {
          player.angle=Ground[player.on_ground_id].angle;
        //outwards from ground
          if (0<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10) { //above ground
            player.current_above=true;
            move_x(-cos(player.angle+M_PI_2));
            move_y(-sin(player.angle+M_PI_2));
          } else if (Ground[player.on_ground_id].height_from_player_x>-10 &&
		     Ground[player.on_ground_id].height_from_player_x<0) { //below ground
            player.current_below=true;
            move_x(-cos(player.angle-M_PI_2));
            move_y(-sin(player.angle-M_PI_2));
          }
        //inwards to ground
          if (3<=Ground[player.on_ground_id].height_from_player_x && Ground[player.on_ground_id].height_from_player_x<10){ //above ground
            move_x(cos(player.angle+M_PI_2));
            move_y(sin(player.angle+M_PI_2));
          } else {//below ground
	        allow_act=false;
	        if (Ground[player.on_ground_id].height_from_player_x>-10) {
	          if (abs(Ground[player.on_ground_id].gradient)>0.4 &&
                Ground[player.on_ground_id].height_from_player_x<-5) {
	            allow_act=true;
              } else if (abs(Ground[player.on_ground_id].gradient)<=0.4 &&
		        Ground[player.on_ground_id].height_from_player_x<-3) {
	            allow_act=true;
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
        player.jump=false;
      }
      if (player.current_above && player.on_ground_id!=-1) {
        player.on_ground_timer=10;
      } else if (player.current_below) {
        player.on_ground_timer=2;
        player.jump_height=0;
        player.jump=false;
      }
      if (abs(Ground[player.on_ground_id].angle)>M_PI_2-0.01) {
        player.on_ground_timer=2;
        player.jump_height=0;
        player.jump=false;
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
          player.jump=false;
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
            player.jump=false;
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
	          //cam_move_x*=-1;
	        }
	        //cam_move_y/=-1.5;
	      }
          player.grav=2;
          player.player_grav=0.5;
        }
      }
      if (player.y-player.HEIGHT/2<0) { //Y axis cap
        move_y(player.player_grav);
      } else if (player.y+player.HEIGHT/2>MAP_HEIGHT) {
        move_y(-player.player_grav);
        //player.health--;
      }
     //X movement
      //allow_act=false;
      //if (player.on_ground_id==-1) { //player is not on ground
	  //  allow_act=true;
      //} /*else if (player.block_timer==0) { //OR player is not blocking
	  //  allow_act=true;
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
      if (player.x-player.WIDTH/2<0) {
        move_x(1);
      } else if (player.x+player.WIDTH/2>MAP_WIDTH) {
        move_x(-1);
      }
     //misc
      /*print_current_above=current_above;
      print_current_below=current_below;*/


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

      player.current_above=false;
      player.current_below=false;
      player.previous_above=false;
      player.previous_below=false;
      player.saved_ground_id=player.on_ground_id;
   //Set Character's Axis
      /*if (print_current_above) {
        above_player.x=player.x+(claws_l)*Cos(player.angle-pi/2);
        above_player.y=player.y+(claws_l)*Sin(player.angle-pi/2);
        above_player.x2=player.x+(claws_l*2)*Cos(player.angle-pi/2);
        above_player.y2=player.y+(claws_l*2)*Sin(player.angle-pi/2);
        if (last_left) {
          player.claws_x=player.x-(claws_l)*Cos(player.angle);
          player.claws_y=player.y-(claws_l)*Sin(player.angle);
        } else {
          player.claws_x=player.x+(claws_l)*Cos(player.angle);
          player.claws_y=player.y+(claws_l)*Sin(player.angle);
        }
      } else if (print_current_below) {
        above_player.x=player.x+(claws_l)*Cos(player.angle+pi/2);
        above_player.y=player.y+(claws_l)*Sin(player.angle+pi/2);
        above_player.x2=player.x+(claws_l*2)*Cos(player.angle+pi/2);
        above_player.y2=player.y+(claws_l*2)*Sin(player.angle+pi/2);
        if (last_left) {
          player.claws_x=player.x+(claws_l)*Cos(player.angle);
          player.claws_y=player.y+(claws_l)*Sin(player.angle);
        } else {
          player.claws_x=player.x-(claws_l)*Cos(player.angle);
          player.claws_y=player.y-(claws_l)*Sin(player.angle);
        }
      } else {
        above_player.x=player.x;
        above_player.y=player.y;
        above_player.x2=player.x;
        above_player.y2=player.y;
        if (last_left) {
         player.claws_x=player.x-(claws_l);
          player.claws_y=player.y;
        } else {
          player.claws_x=player.x+(claws_l);
          player.claws_y=player.y;
        }
      }*/
    }
  }
 //misc
  if (player.on_ground_timer>0) {
    player.on_ground_timer--;
  }
 //
 //
  /*if (player.grav>4) {
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


void DrawPlayer(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  //GrRect(hwnd,hdc,ps,player.x-player.WIDTH,player.y-player.HEIGHT,player.WIDTH,player.HEIGHT,RGB(34,139,34));
  if (player.on_ground_timer>0) {
    if (player.walk_cycle<2) {
      GrSprite(hwnd,hdc,ps,player.x,player.y,player.sprite_angle,player.sprite_1,player.last_left);
    } else {
      GrSprite(hwnd,hdc,ps,player.x,player.y,player.sprite_angle,player.sprite_2,player.last_left);
    }
  } else { //in_air
    GrSprite(hwnd,hdc,ps,player.x,player.y-6,0,player.sprite_jump,player.last_left);
  }
}

