
void InitBullet()
{
  int i=0,j=0;
  current_bullet_id=0;
  for (i=0;i<BULLET_NUM;i++) {
    Bullet[i].shot=FALSE;
    Bullet[i].left=FALSE;
    Bullet[i].near_miss=FALSE;
    Bullet[i].range=0;
    Bullet[i].speed=-1;
    Bullet[i].speed_multiplier=0;
    Bullet[i].damage=0;
    Bullet[i].saved_pos=-1;
    Bullet[i].color=TRANSPARENT;
    Bullet[i].from_enemy_id=-1;
    Bullet[i].start_x=Bullet[i].x=-20;
    Bullet[i].start_y=Bullet[i].y=-20;
    Bullet[i].sprite_x=Bullet[i].x+player.cam_x+player.cam_move_x;
    Bullet[i].sprite_y=Bullet[i].y+player.cam_y+player.cam_move_y;
    Bullet[i].graphics_type=0;
    Bullet[i].angle=0;
//
/*    Bullet[i].msprite_hold_timer=0;
    Bullet[i].msprite_hold_timer_max=0,
    Bullet[i].current_sm=0;
    for (j=0;j<MULTI_SPRITE_NUM;j++) {
      Bullet[i].appear_timer[j]=0;
      Bullet[i].msprite_x[j]=0;
      Bullet[i].msprite_y[j]=0;
    }*/
//
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
  int damage,
  int enemy_id,
  double start_x,
  double start_y,
  double source_x,
  double source_y,
  double target_x,
  double target_y)
{
  double bullet_gradient,bullet_c,x1,x2,y1,y2;
  Bullet[bullet_id].shot=TRUE;
  Bullet[bullet_id].graphics_type=graphics_type;
  Bullet[bullet_id].saved_pos=saved_pos;
  Bullet[bullet_id].color=color;
  Bullet[bullet_id].range=range/2*NODE_SIZE;
  Bullet[bullet_id].speed=speed;
  Bullet[bullet_id].speed_multiplier=speed_multiplier;
  //Bullet[bullet_id].msprite_hold_timer_max=1;
  Bullet[bullet_id].damage=damage;
  Bullet[bullet_id].from_enemy_id=enemy_id;
  Bullet[bullet_id].start_x=Bullet[bullet_id].x=start_x;
  Bullet[bullet_id].start_y=Bullet[bullet_id].y=start_y;
  //Gradient angle related
  if (target_x<source_x) {
    x1=target_x;
    x2=source_x;
    y1=target_y;
    y2=source_y;
    Bullet[bullet_id].left=TRUE;
  } else {
    x2=target_x;
    x1=source_x;
    y2=target_y;
    y1=source_y;
    Bullet[bullet_id].left=FALSE;
  }
  bullet_gradient=GetGradient(x1,y1,x2,y2);
  bullet_c=GetGroundC(x1,y1,bullet_gradient);
  Bullet[bullet_id].angle=GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));//cos(angle) = adjacent/hypothenuse
 // Bullet[bullet_id].angle+=player_angle-player_angle;
  if (bullet_gradient<=0) {
    Bullet[bullet_id].angle=-Bullet[bullet_id].angle;
  }
}

bool HitPlayer(int bullet_id)
{
  if (!player.time_breaker && player.hit_cooldown_timer<=0) {//near miss
    if (GetDistance(player.x,player.y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22) {
      //combo_timer[0]=PLAYER_COMBO_TIME_LIMIT,
      Bullet[bullet_id].near_miss=TRUE;
    }
  }
  if (GetDistance(player.x,player.y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=5) {
    return TRUE;
  }
  return FALSE;
}

void StopBullet(int bullet_id,bool is_player)
{
  Bullet[bullet_id].shot=FALSE;
  Bullet[bullet_id].near_miss=FALSE;
  Bullet[bullet_id].range=0;
  Bullet[bullet_id].from_enemy_id=-1;
  Bullet[bullet_id].speed=0;
  Bullet[bullet_id].speed_multiplier=0;
  Bullet[bullet_id].x=-20;
  Bullet[bullet_id].y=-20;
  Bullet[bullet_id].sprite_x=Bullet[bullet_id].x+player.cam_x+player.cam_move_x;
  Bullet[bullet_id].sprite_y=Bullet[bullet_id].y+player.cam_y+player.cam_move_y;
  if (is_player) {
    Bullet[bullet_id].saved_pos=-1;
  }
}


void BulletAct(int bullet_id)
{
  int i=0,j=0,
      web_id=0,
      bullet_on_ground_id=-1,
      enemy_id=Bullet[bullet_id].from_enemy_id;
  //double bm_x1=0,bm_y1=0,bm_x2=0,bm_y2=0;
  bool hit_player=FALSE,allow_act=FALSE;
  if (Bullet[bullet_id].shot) {
    for (i=0;i<Bullet[bullet_id].speed_multiplier;i++) {
      Bullet[bullet_id].sprite_x=Bullet[bullet_id].x+player.cam_x+player.cam_move_x;
      Bullet[bullet_id].sprite_y=Bullet[bullet_id].y+player.cam_y+player.cam_move_y;
  //----------------
      if (enemy_id==-1) {//player
        if (Bullet[bullet_id].range>0) {
	      allow_act=TRUE;
        } else {
          if (Bullet[bullet_id].left) {
            if (Bullet[bullet_id].angle>-M_PI_2) {
              if ((int)Bullet[bullet_id].range%10==0) {
                Bullet[bullet_id].angle-=0.02;
              }
            }
            Bullet[bullet_id].x-=cos(Bullet[bullet_id].angle)*Bullet[bullet_id].speed/2;
            Bullet[bullet_id].y-=sin(Bullet[bullet_id].angle)*Bullet[bullet_id].speed/2;
          } else {
            if (Bullet[bullet_id].angle<M_PI_2) {
              if ((int)Bullet[bullet_id].range%10==0) {
                Bullet[bullet_id].angle+=0.02;
              }
            }
            Bullet[bullet_id].x+=cos(Bullet[bullet_id].angle)*Bullet[bullet_id].speed/2;
            Bullet[bullet_id].y+=sin(Bullet[bullet_id].angle)*Bullet[bullet_id].speed/2;
          }
          Bullet[bullet_id].range--;
          /*if (i==0) {
            Bullet[bullet_id].speed_multiplier+=exp(abs(Bullet[bullet_id].range))/1000;
          }*/
        }
      } else if (!player.time_breaker || Enemy[enemy_id].time_breaker_immune) {//enemy
	    allow_act=TRUE;
      }
      if (allow_act) {
        if (Bullet[bullet_id].left) {
          Bullet[bullet_id].x-=cos(Bullet[bullet_id].angle)*Bullet[bullet_id].speed;
          Bullet[bullet_id].y-=sin(Bullet[bullet_id].angle)*Bullet[bullet_id].speed;
        } else {
          Bullet[bullet_id].x+=cos(Bullet[bullet_id].angle)*Bullet[bullet_id].speed;
          Bullet[bullet_id].y+=sin(Bullet[bullet_id].angle)*Bullet[bullet_id].speed;
        }
        Bullet[bullet_id].range-=Bullet[bullet_id].speed;
      }
      /*if (enemy_id==-1) {//player place web
        if (Bullet[bullet_id].left) {
	      bm_x2=Bullet[bullet_id].start_x;
	      bm_y2=Bullet[bullet_id].start_y;
	      bm_x1=Bullet[bullet_id].x;
	      bm_y1=Bullet[bullet_id].y;	
	    } else {
	      bm_x1=Bullet[bullet_id].start_x;
	      bm_y1=Bullet[bullet_id].start_y;
	      bm_x2=Bullet[bullet_id].x;
	      bm_y2=Bullet[bullet_id].y;
        }
        if (player.placed_web_pos<player.max_web_num) { //if pointer to web is less than the no. of webs player has currently     
          while (player.web_storage[player.placed_web_pos]==-1) { //find player.web_storage that is not empty
            player.placed_web_pos=LimitValue(player.placed_web_pos+1,0,player.max_web_num); //reset back to 0 if over the max
          }
          web_id=player.web_storage[player.placed_web_pos];
          if (web_id!=-1) {
            Ground[web_id].x1=bm_x1;
            Ground[web_id].y1=bm_y1;
            Ground[web_id].x2=bm_x2;
            Ground[web_id].y2=bm_y2;
            SetGround(web_id);
            SetNodeGridAttributes(web_id);
            double q=0.2*(((DEFAULT_PLAYER_BUILD_RANGE/2*NODE_SIZE)-Bullet[bullet_id].range)/10);
	        Ground[web_id].health=10-q;
	      }
	    }
      }*/


 
      //
      if (enemy_id!=-1) { //Enemy bullet
        hit_player=HitPlayer(bullet_id);
      //bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,0.5,0.5,FALSE);
        bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,2,2,FALSE);
	    allow_act=FALSE;
	    if (hit_player) {
	      allow_act=TRUE;
	    } else if ( //Bullet has hit something
	        bullet_on_ground_id!=-1 || //on a gound
            IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT) ||
	        Bullet[bullet_id].range<=0 || //end of range
            GetDistance(Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22
        ) {
	      allow_act=TRUE;
        }
	//^^ condition
        if (allow_act) {
          if (hit_player) {
            if (!player.blocking || player.block_health<1) {
              player.health-=Bullet[bullet_id].damage;
            //player_snd_dur=DEFAULT_PLAYER_SND_DURATION;
	    //cancel combos
	        /*player_hit_cooldown_timer=player_hit_cooldown_timer_max;
    	    if (player_speed_breaker_recharge_minus>0) {
      	        player_speed_breaker_recharge_minus-=2;
    	    }*/
	        } else {//player is blocking
	          if (player.block_timer>15) {//non-perfect block
	            double blocked_bullet_dmg=Bullet[bullet_id].damage;
	            if (player.on_ground_id!=-1) {//on ground
		          if (player.block_timer<25) {
	                player.block_health-=blocked_bullet_dmg/2;
		          } else {
	                player.block_health-=blocked_bullet_dmg/4;
		          }
	            } else {//in air
		          if (player.block_timer<25) {
	                player.block_health-=blocked_bullet_dmg/4;
		          } else {
	                player.block_health-=blocked_bullet_dmg/8;
		          }
	            }
	            blocked_bullet_dmg=0;
	        /*if (sound_on) {
                  player_snd_dur=2;
                  player_snd_dur=PlaySound(player_snd_dur,50,7);
	        }*/
	        } else {//perfect block
	        /*if (sound_on) {
                player_snd_dur=5;
                player_snd_dur=PlaySound(player_snd_dur,100,15);
	        }*/
	        }
	      } //end of hit player
        } else if (bullet_on_ground_id>=GROUND_NUM && bullet_on_ground_id!=player.web_being_shot) { //Not on web being shot
	      Ground[bullet_on_ground_id].health-=Bullet[bullet_id].damage;
	      if (Ground[bullet_on_ground_id].health<=0) {//completely destroy web at 0 health (can be regained after '4')
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
	  StopBullet(bullet_id,FALSE);

        //Enemy bullet shot array arrangement
	  for (j=Bullet[bullet_id].saved_pos;j<Enemy[enemy_id].bullet_shot_num-1;j++) {
	    Enemy[enemy_id].bullet_shot_arr[j]=Enemy[enemy_id].bullet_shot_arr[j+1];
        Bullet[Enemy[enemy_id].bullet_shot_arr[j]].saved_pos--;
      }
      Bullet[bullet_id].saved_pos=-1;
	  Enemy[enemy_id].bullet_shot_arr[Enemy[enemy_id].bullet_shot_num-1]=-1;
      Enemy[enemy_id].bullet_shot_num--;
    }




  } else {//player bullet while travelling
    bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,NODE_SIZE,NODE_SIZE,FALSE);
    allow_act=FALSE;
	if (bullet_on_ground_id!=-1/* && bullet_on_ground_id!=web_id*/) {//not hit self but another platform
	  allow_act=TRUE;
    } else if (/*Ground[web_id].health<=0 || */IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)) {//out of bounds
	  allow_act=TRUE;
    }// else if (Bullet[bullet_id].range<=0) { 
	  //allow_act=TRUE;
	//}
	if (allow_act) {//reaching end of range
	  if (IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)) { //Destroy ground
        DestroyGround(web_id); //completely destroy web (can be regained after '4')
        player.cdwebs[player.cdweb_pos]=web_id;
        player.cdweb_pos++;
        if (player.cdweb_pos>=player.max_web_num) {
          player.cdweb_pos=0;
        }
        player.cdweb_num++;
      } else if (bullet_on_ground_id!=-1) {
      //player_web_swinging related
        player.pivot_x=Bullet[bullet_id].x;
        player.pivot_y=Bullet[bullet_id].y;
        player.potential_energy=0;
        player.counter_potential_energy=0;
        player.is_swinging=TRUE;
      }
      StopBullet(bullet_id,TRUE);
	  //---web related-------
      if (bullet_on_ground_id>=GROUND_NUM) {
        Ground[bullet_on_ground_id].health+=2;//heal ground
      }
      PlayerPlaceWeb();
      player.web_being_shot=-1;
      player.bullet_shot=-1;
      //---------------------
        }
      }
    } // end of speedy for loop
  } // end of if bullet shot
}

void DrawBullet2(HDC hdc,int i,double x,double y,int color)
{
  int j=0;
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
      GrCircle(hdc,x,y,RandNum(0,3,i),color,-1);
      break;
    case 4:
      for (j=RandNum(0,3,i);j>0;j--) {
        GrCircle(hdc,x,y,j,color,-1);
      }
      break;
  }
}

void DrawBullet(HDC hdc,int i)
{
  DrawBullet2(hdc,i,Bullet[i].sprite_x,Bullet[i].sprite_y,Bullet[i].color);
}
