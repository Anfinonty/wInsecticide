
void InitBullet()
{
  int i=0;
  current_bullet_id=0;
  for (i=0;i<BULLET_NUM;i++) {
    Bullet[i].playsnd=FALSE;
    Bullet[i].shot=FALSE;
    Bullet[i].near_miss=FALSE;
    Bullet[i].range=0;
    Bullet[i].start_range=0;
    Bullet[i].speed=-1;
    Bullet[i].speed_multiplier=0;
    Bullet[i].damage=0;
    Bullet[i].saved_pos=-1;
    Bullet[i].color=TRANSPARENT;
    Bullet[i].from_enemy_id=-1;
    Bullet[i].start_x=Bullet[i].x=-20;
    Bullet[i].start_y=Bullet[i].y=-20;
    Bullet[i].sprite_x=-20;//Bullet[i].x+player.cam_x+player.cam_move_x;
    Bullet[i].sprite_y=-20;//Bullet[i].y+player.cam_y+player.cam_move_y;
    Bullet[i].graphics_type=0;
    Bullet[i].angle=0;
    Bullet[i].saved_ground_id=-1;
  }
}




void ShootBullet(
  int bullet_id,
  int saved_pos,
  int color,
  int graphics_type,
  int range,
  double speed,
  int speed_multiplier,
  double damage,
  int enemy_id,
  double start_x,
  double start_y,
  double source_x,
  double source_y,
  double target_x,
  double target_y,
  double off_angle)
{
  double x1,x2,y1,y2;
  Bullet[bullet_id].shot=TRUE;
  Bullet[bullet_id].graphics_type=graphics_type;
  Bullet[bullet_id].saved_pos=saved_pos;
  Bullet[bullet_id].color=color;
  Bullet[bullet_id].start_range=range/2*NODE_SIZE;
  Bullet[bullet_id].range=range/2*NODE_SIZE;
  Bullet[bullet_id].speed=speed;
  Bullet[bullet_id].speed_multiplier=speed_multiplier;
  //Bullet[bullet_id].msprite_hold_timer_max=1;
  Bullet[bullet_id].damage=damage;
  Bullet[bullet_id].from_enemy_id=enemy_id;
  Bullet[bullet_id].start_x=Bullet[bullet_id].x=start_x;
  Bullet[bullet_id].start_y=Bullet[bullet_id].y=start_y;
  //Gradient angle related
  if (target_x<source_x) { //Left
    x1=target_x;
    x2=source_x;
    y1=target_y;
    y2=source_y;
  } else { //Right
    x2=target_x;
    x1=source_x;
    y2=target_y;
    y1=source_y;
  }
  //cos(angle) = adjacent/hypothenuse
  /*0 . 90 degs*/

  //determine angle quadrant
  if (target_x<source_x) {// LEFT
    if (target_y<source_y) { //UP
      Bullet[bullet_id].angle=M_PI+GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));
    } else {//DOWN
      Bullet[bullet_id].angle=M_PI-GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));
    }
  } else {// RIGHT
    if (target_y<source_y) { //UP
      Bullet[bullet_id].angle=2*M_PI-GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));
    } else { //DOWN
      Bullet[bullet_id].angle=GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));
    }
  }

  Bullet[bullet_id].angle+=off_angle;
  if (Bullet[bullet_id].angle>2*M_PI) {
    Bullet[bullet_id].angle-=2*M_PI;
  }
  if (Bullet[bullet_id].angle<0) {
    Bullet[bullet_id].angle+=2*M_PI;
  }

}


bool HitPlayer(int bullet_id)
{
  double dist=GetDistance(player.x,player.y,Bullet[bullet_id].x,Bullet[bullet_id].y);
  if (!player.time_breaker && player.hit_cooldown_timer<=0) {//near miss
    if (dist>10 && dist<=22) {
      //combo_timer[0]=PLAYER_COMBO_TIME_LIMIT,
      Bullet[bullet_id].near_miss=TRUE;
    }
  }
  if (0<=dist && dist<=10) {
    return TRUE;
  }
  return FALSE;
}


void StopBullet(int bullet_id,bool is_player)
{
  Bullet[bullet_id].shot=FALSE;
  Bullet[bullet_id].near_miss=FALSE;
  Bullet[bullet_id].start_range=0;
  Bullet[bullet_id].range=0;
  Bullet[bullet_id].from_enemy_id=-1;
  Bullet[bullet_id].speed=0;
  Bullet[bullet_id].saved_ground_id=-1;
  Bullet[bullet_id].speed_multiplier=0;
  Bullet[bullet_id].x=-20;
  Bullet[bullet_id].y=-20;
  Bullet[bullet_id].sprite_x=-20;//Bullet[bullet_id].x+player.cam_x+player.cam_move_x;
  Bullet[bullet_id].sprite_y=-20;//Bullet[bullet_id].y+player.cam_y+player.cam_move_y;
  if (is_player) {
    Bullet[bullet_id].saved_pos=-1;
  }
}


void EnemyBulletAct(int bullet_id,int enemy_id)
{
  int bullet_on_ground_id=-1,bk;
  bool hit_player=FALSE,allow_act=FALSE;
  if (GetDistance(Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22) {
    Bullet[bullet_id].angle=RandAngle(0,360,Enemy[enemy_id]->seed);//RandNum(-M_PI_2*100,M_PI_2*100,Enemy[enemy_id]->seed)/100;
    Bullet[bullet_id].speed=Bullet[player.bullet_shot].speed;
    Bullet[bullet_id].speed_multiplier=Bullet[player.bullet_shot].speed_multiplier;
    if (game_audio) {
      Bullet[player.bullet_shot].playsnd=TRUE;
    }
  //Bullet[player.bullet_shot].range-=2;
  }


  for (int k=0;k<player.bullet_shot_num;k++) {//playerknives interact with enemy bullet
    bk=player.bullet[k];
    if (GetDistance(Bullet[bk].x,Bullet[bk].y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22) {
      if (!Bullet[bk].playsnd && game_audio) {
        Bullet[bk].playsnd=TRUE;
      }


      Bullet[bullet_id].angle=RandAngle(0,360,player.seed); //scatter type 6
      Bullet[bullet_id].speed=Bullet[bk].speed;
      Bullet[bullet_id].speed_multiplier=Bullet[bk].speed_multiplier;
      Bullet[bullet_id].range-=3;


      /*if (Bullet[bk].speed_multiplier<7) { //if shotgun bullet, pierce through for the first few ranges
      }*/

      if (Bullet[bk].graphics_type!=6) { //hit enemy bullet, scatter if NOT type 6
        Bullet[bk].angle=RandAngle(0,360,player.seed);
        Bullet[bk].range-=4;
      }
    }
  } //end of for,
  hit_player=HitPlayer(bullet_id);

  bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,2,2);
  allow_act=FALSE;
  if (Enemy[enemy_id]->health>0) {
    if (hit_player) {
            //player.time_breaker_units=0;
            //player.sleep_timer=DEFAULT_SLEEP_TIMER;
      allow_act=TRUE;
    } else if ( //Bullet has hit something
      bullet_on_ground_id!=-1 || //on a gound
       IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT) ||
        Bullet[bullet_id].range<=0 //end of range
      ) {
        allow_act=TRUE;
      }
    }
//^^ condition
    double blocked_bullet_dmg=Bullet[bullet_id].damage;
    if (allow_act) {
      if (hit_player) {
        if (player.block_timer<=0 || player.block_health<=0) {
          blocked_bullet_dmg=Bullet[bullet_id].damage;
          if (player.health>PLAYER_LOW_HEALTH+1) { //usual response
            player.health-=blocked_bullet_dmg;
          } else { //Player when low health
            if (player.health<PLAYER_LOW_HEALTH) {
              player.health-=0.1;
            } else {
              player.health-=1;
            }
          }
        //player_snd_dur=DEFAULT_PLAYER_SND_DURATION;
    //cancel combos
        /*player_hit_cooldown_timer=player_hit_cooldown_timer_max;
	    if (player_speed_breaker_recharge_minus>0) {
  	        player_speed_breaker_recharge_minus-=2;
	    }*/

        } else {//player is blocking
         // Bullet[bullet_id].angle=RandAngle(0,360,Enemy[enemy_id]->seed);
          blocked_bullet_dmg=Bullet[bullet_id].damage;
          if (player.block_timer>23) {//non-perfect block
            if (player.on_ground_id!=-1) {//on ground
	          if (player.block_timer<26) {
                player.block_health-=blocked_bullet_dmg/4;
	          } else {
                player.block_health-=blocked_bullet_dmg/2;
	          }
            } else {//in air
	          if (player.block_timer<26) {
                player.block_health-=blocked_bullet_dmg/4;
	          } else {
                player.block_health-=blocked_bullet_dmg;
	          }
            }
            /*if (player.speed<6) {
              player.health-=(player.block_health_max-player.block_health+1)/player.block_health_max*Bullet[bullet_id].damage;
            }*/
            //blocked_bullet_dmg=0;
          } else {//perfect block , 23 or less than
            blocked_bullet_dmg=0;
            if (player.time_breaker_units<player.time_breaker_units_max) { //causes speed to increase
              player.time_breaker_units++;
            }
          }
        }
        
        if (blocked_bullet_dmg>0) {
          if (!player.time_breaker) { //penalty for hitting a bullet
            if (player.speed>5) {
              player.speed--;
            } else { //penalty only at low speed
              if (player.time_breaker_units>1) {
                player.time_breaker_units=1;
              }
            }
          }
        } else {
          if (!IsSpeedBreaking()) {
            player.decceleration_timer=0;
            player.speed++;
          }
        }

      //End of Hit Player
      } else if (bullet_on_ground_id>=GROUND_NUM && bullet_on_ground_id!=player.web_being_shot) { //Not on web being shot
        Ground[bullet_on_ground_id]->health-=Bullet[bullet_id].damage;
        if (Ground[bullet_on_ground_id]->health<=0) {//completely destroy web at 0 health (can be regained after '4')
          DestroyGround(bullet_on_ground_id); 
          player.cdwebs[player.cdweb_pos]=bullet_on_ground_id;
          player.cdweb_pos++;
          if (player.cdweb_pos>=player.max_web_num) {
            player.cdweb_pos=0;
          }
          player.cdweb_num++;
        }
      }
	//bullet dodged
	  /*if (player_hit_cooldown_timer==0 && !player_blocking) {
	    if (Bullet[bullet_id].near_miss) {
	      combo_timer[0]=PLAYER_COMBO_TIME_LIMIT,
	      combo_streak[0]++;
	      combo_timer[2]=751;//killstreak
	      if (sound_on) {
                player_snd_dur=2;
                player_snd_dur=PlaySound(player_snd_dur,35,2);	
	      }
        }
	  }*/
      if (hit_player || Bullet[bullet_id].graphics_type!=5 || Bullet[bullet_id].range<=0) {
        StopBullet(bullet_id,FALSE); 
        //Enemy bullet shot array arrangement
	    for (int j=Bullet[bullet_id].saved_pos;j<Enemy[enemy_id]->bullet_shot_num-1;j++) { //shift to left in enemy bullet shot arr from bullet shot
	      Enemy[enemy_id]->bullet_shot_arr[j]=Enemy[enemy_id]->bullet_shot_arr[j+1];
          Bullet[Enemy[enemy_id]->bullet_shot_arr[j]].saved_pos--;
        }
        Bullet[bullet_id].saved_pos=-1;
	    Enemy[enemy_id]->bullet_shot_arr[Enemy[enemy_id]->bullet_shot_num-1]=-1; //remove bullet from arr
        Enemy[enemy_id]->bullet_shot_num--;
      } else {
        Bullet[bullet_id].angle=2*M_PI-Bullet[bullet_id].angle+2*Ground[bullet_on_ground_id]->angle; //real
        if (Bullet[bullet_id].angle>2*M_PI) { //hreater
          Bullet[bullet_id].angle-=2*M_PI;
        }
        if (Bullet[bullet_id].angle<0) {
          Bullet[bullet_id].angle+=2*M_PI;
        }
     
        if (Bullet[bullet_id].saved_ground_id==bullet_on_ground_id) { //prevents riding of wall
          Bullet[bullet_id].range=-1;
        }
        Bullet[bullet_id].saved_ground_id=bullet_on_ground_id;
      }
    } //end of allow act
}



void PlayerBulletAct(int bullet_id,int enemy_id)
{      
    bool allow_act;
    int bullet_on_ground_id=-1;
    bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,2,2);
    allow_act=FALSE;
	if (bullet_on_ground_id!=-1) {//not hit self but another platform
	  allow_act=TRUE;
    } else if (IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)) {//out of bounds
	  allow_act=TRUE;
    }
	if (allow_act) {//reaching end of range
      //player_web_swinging related
      if (enemy_id==-1) {
        if (Bullet[bullet_id].range>0) {
          if (bullet_on_ground_id!=-1) {
            player.pivot_x=Bullet[bullet_id].x;
            player.pivot_y=Bullet[bullet_id].y;
            player.is_swinging=TRUE;
          }
	      //---web related-------
          /*if (bullet_on_ground_id>=GROUND_NUM) {
            Ground[bullet_on_ground_id]->health+=2;//heal ground
          }*/
          player.web_being_shot=-1;
          player.bullet_shot=-1;
        //---------------------
          StopBullet(bullet_id,FALSE);
        }
      } else if (enemy_id==-2) { //ricochet bullet
        if (IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)) {
          Bullet[bullet_id].range=-1;
        } else { //Ricochet off ground
/*
         Negative                                                     Positive


            /           /                                     \
           /         /                                         \
          /        /                                            \       
         /      /                                                \
        /     /                                                   \
       /   /                                                       \
      / /                                                           \
     /                                                               \


                    (Clockwise)


                    0 . M_PI_2     |       -M_PI_2 . 0
            M_PI . M_PI+M_PI_2     |       M_PI+M_PI_2. 2*M_PI       
                    Positive        |       Negative
                    --------------/m.\\---------------
                    Negative        |       Positive
                    -M_PI_2 . 0    |       0 . M_PI_2
                     M_PI_2 . M_PI         0 . M_PI_2

                        left              !left

                                            



     \          \                                                  /           /
       \        \                                                  /         /
         \       \                                                /        /
           \     \                                                /      /
             \    \                                              /     /
               \  \                                              /   /
                 \ \                                            / /
                   \                                            /
            
         Positive gradient                                  Negative Gradient
          Positive Angle                                     Negative Angle


        


   (Clockwise)
    Right Side:
        Upwards: M_PI+M_PI_2 . 2*M_PI
      Downwards: 0 . M_PI_2

    Left Side:
      Downwards: M_PI_2 . M_PI 
        Upwards: M_PI . M_PI+M_PI_2




Ascii art woo!! :D

                            ________________________
        _ _ _ _ _ _ _ _ _ _\  _/  _ _ _ _ _ _ _ _ _ |
 Ground _______________     \ ground_angle          |
                        -----------------           |
                         -- / \ /        -----------|   
                      --   /   \   j                |
                   --     /     \                   |
                --    i  /  i    \                  |
             --         /         \                 |
          --           /           \                |
       (O)2           /             \               |
    --               /               \              |
                    /                 \             |
                   /                   (O)1---------|




    O1 = Original Bullet Angle
    O2 = New Bullet Angle
    i = angle of incidence
    j = outer angle of incidence
    ground_angle = angle of ground


    Find O2

    j = 2pi - pi/2 - pi/2 -  (2pi - O1) - ground_angle
      = 2pi - pi - 2pi + O1 - ground_angle
      = -pi + O1 - ground_angle

    i = pi/2 - j
      = pi/2 - (-pi + O1 - ground_angle)
      = pi/2 + pi - O1 + ground_angle
      = 3/2 * pi - O1 + ground_angle


    O2 = ground_angle + j + i + i
       = ground_angle + j + 2i
       = ground_angle + (-pi + O1 - ground_angle) + 2*(3/2 * pi - O1 + ground_angle)
       = ground_angle -pi + O1 - ground_angle + 3pi - 2*O1 + 2*ground_angle
       = 2pi -O1 + 2*ground_angle


   *Al-Khwarizmi
*/

          Bullet[bullet_id].angle=2*M_PI-Bullet[bullet_id].angle+2*Ground[bullet_on_ground_id]->angle; //real
          if (bullet_on_ground_id>=GROUND_NUM) { //elastic web            
            Bullet[bullet_id].range+=80;
            Bullet[bullet_id].speed_multiplier+=2;
            if (Bullet[bullet_id].graphics_type!=7)
              Bullet[bullet_id].damage+=1;
            else
              Bullet[bullet_id].damage+=0.2;
            Bullet[bullet_id].start_range=Bullet[bullet_id].range;
          }
          if (Bullet[bullet_id].angle>2*M_PI) { //hreater
            Bullet[bullet_id].angle-=2*M_PI;
          }
          if (Bullet[bullet_id].angle<0) {
            Bullet[bullet_id].angle+=2*M_PI;
          }
          //if (abs(Ground[bullet_on_ground_id]->angle)-0.05<=abs(Bullet[bullet_id].angle) && abs(Bullet[bullet_id].angle)<=abs(Ground[bullet_on_ground_id]->angle)+0.05) { //destroy bullet in invalid state
            //Bullet[bullet_id].range=-1;
          //}
          if (Bullet[bullet_id].saved_ground_id==bullet_on_ground_id) { //prevents riding of wall
            Bullet[bullet_id].range=-1;
          }
          Bullet[bullet_id].saved_ground_id=bullet_on_ground_id;
        }
      }
    }
}





void InitBulletRain()
{
  for (int i=SHOOT_BULLET_NUM;i<BULLET_NUM;i++) {
    int rand_x=RandNum(player.x-GR_WIDTH/2+6,player.x+GR_WIDTH/2-6,frame_tick); //so it doest get stuck to ground
    if (rand_x<10)
      rand_x=RandNum(10,player.x+GR_WIDTH/2-6,frame_tick);
    else if (rand_x>MAP_WIDTH)
      rand_x=RandNum(player.x-GR_WIDTH/2,MAP_WIDTH-10,frame_tick);
    ShootBullet(
        i,
        -1,
        BLUE,
        8, //graphics type
        MAP_HEIGHT, //range ==>
        1, //speed
        10, //speed multiplier
        0, //damage
        -3,
        rand_x, //so it doest get stuck to ground
        10,
        0,
        0,
        8,
        20,
        0 //angle            
    );
  }
}



void RainBulletAct(int bullet_id)
{
  int allow_act=-1;
  int bullet_on_ground_id=-1,bullet_on_node_grid_id=-1;
  bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,2,2);
  bullet_on_node_grid_id=GetGridId(Bullet[bullet_id].x,Bullet[bullet_id].y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);

  if (IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)) { //out of bounds
    allow_act=1;
  } else if (Bullet[bullet_id].x<player.x-GR_WIDTH/2-player.cam_mouse_move_x || 
             Bullet[bullet_id].x>player.x+GR_WIDTH/2-player.cam_mouse_move_x || 
             Bullet[bullet_id].y>player.y+GR_HEIGHT/2-player.cam_mouse_move_y) {//out of player view
    allow_act=1;
  } else if (bullet_on_node_grid_id!=-1) {
    if (NodeGrid[bullet_on_node_grid_id]->node_no_rain || !NodeGrid[bullet_on_node_grid_id]->node_no_shade) {
      allow_act=1;
      bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,NODE_SIZE,NODE_SIZE);
    }
  } 

  if (bullet_on_ground_id!=-1) {//hit platform
    allow_act=0;
  }

  if (allow_act!=-1) { //perform action
    if (allow_act==1) { //out of range
      Bullet[bullet_id].range=-1;
    } else if (allow_act==0){ //Ricochet off ground
      Bullet[bullet_id].angle=2*M_PI-Bullet[bullet_id].angle+2*Ground[bullet_on_ground_id]->angle+RandAngle(-15,15,frame_tick); //slight random when hit
      Bullet[bullet_id].speed_multiplier=1;
      Bullet[bullet_id].speed=0.4;
      Bullet[bullet_id].range=13;
      Bullet[bullet_id].graphics_type=3;
    }

    if (Bullet[bullet_id].angle>2*M_PI) { //hreater
      Bullet[bullet_id].angle-=2*M_PI;
    }
    if (Bullet[bullet_id].angle<0) {
      Bullet[bullet_id].angle+=2*M_PI;
    }
    if (Bullet[bullet_id].saved_ground_id==bullet_on_ground_id) { //prevents riding of wall
      Bullet[bullet_id].range=-1;
    }
    Bullet[bullet_id].saved_ground_id=bullet_on_ground_id;
  }
}


void BulletAct(int bullet_id)
{
  int enemy_id=Bullet[bullet_id].from_enemy_id;
  //double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
  bool allow_act=FALSE;
  if (Bullet[bullet_id].shot) {
    for (int i=0;i<Bullet[bullet_id].speed_multiplier;i++) {
      allow_act=FALSE;
      Bullet[bullet_id].sprite_x=Bullet[bullet_id].x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x;
      Bullet[bullet_id].sprite_y=Bullet[bullet_id].y+player.cam_y+player.cam_move_y+player.cam_mouse_move_y;
  //----------------
      if (enemy_id<0) {//player bullet movement
        if (enemy_id==-1) { //web-snipe
          if (Bullet[bullet_id].range>0) {
            allow_act=TRUE;
          } else { //web too long, retract
            StopBullet(player.bullet_shot,TRUE); //Stop the web
            PlayerPlaceWeb(); //Web related
            PlayerBulletLimitAct();
            player.web_being_shot=-1;
            player.bullet_shot=-1;
          }
        } else if (enemy_id==-2) { //knives throw
          if (Bullet[bullet_id].graphics_type==6 && Bullet[bullet_id].range<=Bullet[bullet_id].start_range-120) {
            Bullet[bullet_id].graphics_type=5;
          }
          if (Bullet[bullet_id].range>0) {
            if (!player.time_breaker) {
	          allow_act=TRUE;
            } else if (Bullet[bullet_id].start_range-Bullet[bullet_id].range<50) {
	          allow_act=TRUE;
            }
          } else { //out of range knife, stop it
            StopBullet(bullet_id,FALSE);
            for (int j=Bullet[bullet_id].saved_pos;j<player.bullet_shot_num-1;j++) { //shift to left in player bullet shot arr from bullet shot
              player.bullet[j]=player.bullet[j+1];
              Bullet[player.bullet[j]].saved_pos--;
            }
            Bullet[bullet_id].saved_pos=-1;
            player.bullet[player.bullet_shot_num-1]=-1; //remove bullet from arr
            player.bullet_shot_num--;        
          }
        } else if (enemy_id==-3) { //rain
          if (Bullet[bullet_id].range>0) {
            if (!player.time_breaker) {
              allow_act=TRUE;
            } else if (Bullet[bullet_id].start_range-Bullet[bullet_id].range<50) {
              allow_act=TRUE;
            }
          } else { //out of range rain, reset 
            StopBullet(bullet_id,FALSE);
            int rand_x=RandNum(player.x-GR_WIDTH/2+6-player.cam_mouse_move_x,player.x+GR_WIDTH/2-6-player.cam_mouse_move_x,frame_tick); //so it doest get stuck to ground
            if (rand_x<10)
              rand_x=RandNum(10,player.x+GR_WIDTH/2-6-player.cam_mouse_move_x,frame_tick);
            else if (rand_x>MAP_WIDTH)
              rand_x=RandNum(player.x-GR_WIDTH/2-player.cam_mouse_move_x,MAP_WIDTH-10,frame_tick);


            int rand_y=RandNum(player.y-GR_HEIGHT/2+6-player.cam_mouse_move_y,player.y+GR_HEIGHT/2-6-player.cam_mouse_move_y,frame_tick); //so it doest get stuck to ground
            if (rand_y<10)
              rand_y=RandNum(10,player.y+GR_HEIGHT/2-6-player.cam_mouse_move_y,frame_tick);
            else if (rand_y>MAP_HEIGHT)
              rand_y=RandNum(player.y-GR_HEIGHT/2-player.cam_mouse_move_y,MAP_HEIGHT-10,frame_tick);

            ShootBullet(
                bullet_id,
                -1,
                BLUE,
                8, //graphics type
                MAP_HEIGHT, //range ==>
                1, //speed
                10, //speed multiplier
                0, //damage
                -3,
                stickyTo(rand_x,NODE_SIZE*4), //so it doest get stuck to ground
                stickyTo(rand_y,NODE_SIZE*4),
                0,
                0,
                8,
                20,
                0 //angle            
            );
          }
        }
      } else if (!player.time_breaker || Enemy[enemy_id]->time_breaker_immune) {//enemy
        if (Bullet[bullet_id].graphics_type==6 && Bullet[bullet_id].range<=Bullet[bullet_id].start_range-120) {
          Bullet[bullet_id].graphics_type=1;
        }
        if (!player.time_breaker) {
          allow_act=TRUE;
        } else if (Bullet[bullet_id].start_range-Bullet[bullet_id].range<50) {
          allow_act=TRUE;
        }
      } //End of stopping bullet


      if (allow_act) { //bullet movement default
        Bullet[bullet_id].x+=cos(Bullet[bullet_id].angle)*Bullet[bullet_id].speed;
        Bullet[bullet_id].y+=sin(Bullet[bullet_id].angle)*Bullet[bullet_id].speed;
        Bullet[bullet_id].range-=Bullet[bullet_id].speed;
      }
 
      if (enemy_id>-1) { //Enemy bullet
        EnemyBulletAct(bullet_id,enemy_id);
      } else {//player bullet while travelling
        if (enemy_id>-3) { //player
          PlayerBulletAct(bullet_id,enemy_id);
        } else if (enemy_id==-3) {
          RainBulletAct(bullet_id);
        }
      }
    } // end of speedy for loop
  } // end of if bullet shot
}


void BulletSndAct(int i)
{
  if (Bullet[i].playsnd) {
    if (Bullet[i].shot) {
      //https://stackoverflow.com/questions/1382051/what-is-the-c-equivalent-for-reinterpret-cast
      PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC); //clang
    }
    Bullet[i].playsnd=FALSE;
  }
}



void RainAct()
{
  for (int i=SHOOT_BULLET_NUM;i<BULLET_NUM;i++) {
    BulletAct(i);
  }  
}



void DrawBullet2(HDC hdc,int i,double x,double y,int color)
{
  switch (Bullet[i].graphics_type) {
    case 0:
      GrCircle(hdc,x,y,2,color,-1);
      break;
    case 1:
      GrCircle(hdc,x,y,1,color,-1);
      break;
    case 2:
      GrCircle(hdc,x,y,1,color,-1);
      GrCircle(hdc,x,y,2,color,-1);
      break;
    case 3:
    case 6:
      GrCircle(hdc,x,y,RandNum(0,3,frame_tick),color,-1);
      break;
    case 4:
      GrCircle(hdc,x,y,RandNum(0,3,frame_tick),color,color);
      break;
    case 5:
      {
      GrLine(hdc,x,y,x+10*cos(Bullet[i].angle),y+10*sin(Bullet[i].angle),color);
      }
      break;
    case 7:
      {
      GrLine(hdc,x,y,x+6*cos(Bullet[i].angle),y+6*sin(Bullet[i].angle),color);
      }
      break;
    case 8: //rain
      {
      GrLine(hdc,x,y,x-32*cos(Bullet[i].angle),y-32*sin(Bullet[i].angle),color);
      }
      break;
  }
}


void DrawBullet(HDC hdc,int i)
{
  DrawBullet2(hdc,i,Bullet[i].sprite_x,Bullet[i].sprite_y,Bullet[i].color);
}




void DrawRain(HDC hdc)
{
  for (int i=SHOOT_BULLET_NUM;i<BULLET_NUM;i++) {
    DrawBullet(hdc,i);
  }
}
