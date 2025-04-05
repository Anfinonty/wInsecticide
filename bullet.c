
void InitBullet(int max_bullet_num)
{
  int i=0;
  current_bullet_id=0;
  for (i=0;i<max_bullet_num;i++) {
    Bullet[i].bounce_timer=0;
    Bullet[i].playsnd=FALSE;
    Bullet[i].shot=FALSE;
    Bullet[i].near_miss=FALSE;
    Bullet[i].in_water=FALSE;
    Bullet[i].range=0;
    Bullet[i].start_range=0;
    Bullet[i].ospeed=-1;
    Bullet[i].speed=-1;
    Bullet[i].ospeed_multiplier=0;
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
    Bullet[i].saved_node_grid_id=-1;
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
  Bullet[bullet_id].in_water=FALSE;
  Bullet[bullet_id].graphics_type=graphics_type;
  Bullet[bullet_id].saved_pos=saved_pos;
  Bullet[bullet_id].color=color;
  Bullet[bullet_id].start_range=range/2*NODE_SIZE;
  Bullet[bullet_id].range=range/2*NODE_SIZE;
  Bullet[bullet_id].ospeed=speed;
  Bullet[bullet_id].speed=speed;
  Bullet[bullet_id].ospeed_multiplier=speed_multiplier;
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


void BulletDamagePlayerAct(int bullet_id)
{
    double blocked_bullet_dmg=Bullet[bullet_id].damage;
    if (player.block_timer<=0 || player.block_health<=0) {
      if (game_audio && player.health>0) {
        PlaySound(spamSoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //hurt snd
      }
      player.show_health_timer=HP_SHOW_TIMER_NUM;
      player.show_block_health_timer=HP_SHOW_TIMER_NUM;
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

    } else {//player is blocking
     // Bullet[bullet_id].angle=RandAngle(0,360,Enemy[enemy_id]->seed);
      blocked_bullet_dmg=Bullet[bullet_id].damage;
      if (player.block_timer>23) {//non-perfect block
        player.show_block_health_timer=HP_SHOW_TIMER_NUM;
        if (game_audio && player.health>0) {
          PlaySound(spamSoundEffectCache[3].audio, NULL, SND_MEMORY | SND_ASYNC); //block normal
        }
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
        //blocked_bullet_dmg=0;
      } else {//perfect block , 23 or less than
        if (game_audio && player.health>0) {
          PlaySound(spamSoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //block perfect
        }
        blocked_bullet_dmg=0;
        /*if (player.time_breaker_units<player.time_breaker_units_max) { //causes speed to increase
          player.time_breaker_units++;
        }*/
      }
    }
    
    if (blocked_bullet_dmg>0) {
      if (!player.time_breaker) { //penalty for hitting a bullet
        if (player.block_health<=0 || player.block_timer==0) {
          if (player.speed>5) {
            player.speed--;
          } else { //penalty only at low speed
            if (player.time_breaker_units>1) {
              player.invalid_shoot_timer=9;
              player.time_breaker_units=1;
            }
          }
        }
      }
    } else {
      if (!IsSpeedBreaking()) {
        player.decceleration_timer=0;
        //player.speed++;
      }
    }
}



bool HitPlayer(int bullet_id,int r1,int r2)
{
  double dist=GetDistance(player.x,player.y,Bullet[bullet_id].x,Bullet[bullet_id].y);
  if (!player.time_breaker && player.hit_cooldown_timer<=0) {//near miss
    if (dist>r1 && dist<=r2) {
      //combo_timer[0]=PLAYER_COMBO_TIME_LIMIT,
      Bullet[bullet_id].near_miss=TRUE;
    }
  }
  if (0<=dist && dist<=r1) {
    return TRUE;
  }
  return FALSE;
}


void StopBullet(int bullet_id,bool is_player)
{
  Bullet[bullet_id].shot=FALSE;
  Bullet[bullet_id].near_miss=FALSE;
  Bullet[bullet_id].in_water=FALSE;
  Bullet[bullet_id].start_range=0;
  Bullet[bullet_id].range=0;
  Bullet[bullet_id].from_enemy_id=-1;
  Bullet[bullet_id].speed=0;
  Bullet[bullet_id].ospeed=0;
  Bullet[bullet_id].bounce_timer=0;
  if (Bullet[bullet_id].saved_node_grid_id!=-1) {
    NodeGrid[Bullet[bullet_id].saved_node_grid_id]->tmp_wet=FALSE;
  }
  Bullet[bullet_id].saved_node_grid_id=-1;
  Bullet[bullet_id].saved_ground_id=-1;
  Bullet[bullet_id].speed_multiplier=0;
  Bullet[bullet_id].ospeed_multiplier=0;
  Bullet[bullet_id].x=-20;
  Bullet[bullet_id].y=-20;
  Bullet[bullet_id].sprite_x=-20;//Bullet[bullet_id].x+player.cam_x+player.cam_move_x;
  Bullet[bullet_id].sprite_y=-20;//Bullet[bullet_id].y+player.cam_y+player.cam_move_y;
  if (is_player) {
    Bullet[bullet_id].saved_pos=-1;
  }
}


void RainBulletTransitNodeGrid(int bullet_id)
{
  int on_node_grid_id=GetGridId(Bullet[bullet_id].x,Bullet[bullet_id].y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
  if (on_node_grid_id!=Bullet[bullet_id].saved_node_grid_id) {
    if (Bullet[bullet_id].saved_node_grid_id!=-1)
      NodeGrid[Bullet[bullet_id].saved_node_grid_id]->tmp_wet=FALSE;
    NodeGrid[on_node_grid_id]->tmp_wet=TRUE;
    Bullet[bullet_id].saved_node_grid_id=on_node_grid_id;
  }
}


void EnemyBulletAct(int bullet_id,int enemy_id)
{
  int bullet_on_ground_id=-1,bk;
  bool hit_player=FALSE,allow_act=FALSE;

  //sniper bullet act with enemy bullet
  if (Bullet[bullet_id].graphics_type!=10) {
  if (GetDistance(Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22) {
    Bullet[bullet_id].angle=RandAngle(0,360,Enemy[enemy_id]->seed);//RandNum(-M_PI_2*100,M_PI_2*100,Enemy[enemy_id]->seed)/100;
    Bullet[bullet_id].speed=Bullet[player.bullet_shot].speed;
    Bullet[bullet_id].speed_multiplier=Bullet[player.bullet_shot].speed_multiplier;
    if (game_audio && player.health>0) {
      Bullet[player.bullet_shot].playsnd=TRUE;
    }
  //Bullet[player.bullet_shot].range-=2;
  }
  }

  double saved_angle;
  for (int k=0;k<player.bullet_shot_num;k++) {//playerknives interact with enemy bullet
    saved_angle=Bullet[bullet_id].angle;
    bk=player.bullet[k];
    if (GetDistance(Bullet[bk].x,Bullet[bk].y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22) {
      if (!Bullet[bk].playsnd && game_audio && Bullet[bullet_id].graphics_type!=10 && Bullet[bullet_id].graphics_type!=-1 && player.health>0) {
        Bullet[bk].playsnd=TRUE;
      }


      if (Bullet[bullet_id].graphics_type!=10 && Bullet[bullet_id].graphics_type!=-1) {
        if (Bullet[bullet_id].bounce_timer==0) {
          Bullet[bullet_id].angle=GetMarbleAngle(Bullet[bullet_id].angle,Bullet[bk].angle)+RandAngle(-12,12,frame_tick);//GetBounceAngle(Bullet[bk].angle,Bullet[bullet_id].angle);//RandAngle(0,360,player.seed); //scatter type 6
          Bullet[bullet_id].bounce_timer=10;
          Bullet[bk].bounce_timer=0;
        }
        Bullet[bullet_id].speed=Bullet[bk].speed;
        Bullet[bullet_id].speed_multiplier=Bullet[bk].speed_multiplier;
        Bullet[bullet_id].range-=3;
      } else { //death bullet
        Bullet[bullet_id].angle=Bullet[bk].angle+RandAngle(-65,65,player.seed); //scatter type 6
      }

      /*if (Bullet[bk].speed_multiplier<7) { //if shotgun bullet, pierce through for the first few ranges
      }*/

      if (Bullet[bk].graphics_type!=6 && Bullet[bullet_id].graphics_type!=10 && Bullet[bullet_id].graphics_type!=-1) { //hit enemy bullet, scatter if NOT type 6
        if (Bullet[bk].bounce_timer==0) {
          Bullet[bk].angle=-GetMarbleAngle(Bullet[bk].angle, saved_angle)+RandAngle(-12,12,player.seed);
          Bullet[bk].bounce_timer=10;
        }
        switch (Bullet[bk].graphics_type) {
          case 5: //kpt 3
            //Bullet[bk].range/=4;
            Bullet[bk].range-=16;
            break;
          case 9: //kpt 1
            Bullet[bk].range-=8;
            break;
          default:
            Bullet[bk].range-=4;
            break;
        }
      }
    }
  } //end of for,

     //hit player web
     //web swinging affects enemies bullet
     if (player.is_swinging) {
       for (int k=0;k<PLAYER_FLING_WEB_NUM;k++) {
         //knockback
         double dist_from_web_string=GetDistance(player_fling_web.x[k],player_fling_web.y[k],Bullet[bullet_id].x,Bullet[bullet_id].y);
         if (dist_from_web_string<NODE_SIZE*2) {
           Bullet[bullet_id].angle=player.angle_of_incidence;//player.pivot_angle+M_PI_2;
         }  
       }
     }


  if (Enemy[enemy_id]->health>0) {
    if ((player.block_timer>0 && player.block_timer<=23)){
      hit_player=HitPlayer(bullet_id,player.block_timer*2,player.block_timer*2);
    } else {
      hit_player=HitPlayer(bullet_id,10,22);
    }
    
  } else { //death bullet
    if (Enemy[enemy_id]->damage_taken_timer<256) {
      hit_player=HitPlayer(bullet_id,22,22);
    } else {
      hit_player=FALSE;
    }
  }


   
  //enemy gib movement after its death
  bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,2,2);
  if (bullet_on_ground_id==-1 && Enemy[enemy_id]->health<=0 && (Enemy[enemy_id]->death_timer>100) && player.health>0) {
    double pbdist=GetDistance(player.x,player.y,Bullet[bullet_id].x,Bullet[bullet_id].y);
    double pbt=0.2;    
    if (pbdist<250) {
      if (game_hard) {
        pbt=0.1;
      }
      if (pbdist<150) {
        if (!game_hard) {
          pbt=0.5;
        } else {
          pbt=0.1;
        }
      }
      if (Bullet[bullet_id].x<player.x) {
        Bullet[bullet_id].x+=pbt;
      } else {
        Bullet[bullet_id].x-=pbt;
      }

      if (Bullet[bullet_id].y<player.y) {
        Bullet[bullet_id].y+=pbt;
      } else {
        Bullet[bullet_id].y-=pbt;
      }
    }

  }


  //}
  allow_act=FALSE;
  //if (Enemy[enemy_id]->health>0) {
    if (hit_player) {
      //player.time_breaker_units=0;
      //player.sleep_timer=DEFAULT_SLEEP_TIMER;
      if (Enemy[enemy_id]->health>0) {
        allow_act=TRUE;
      } else if (Enemy[enemy_id]->damage_taken_timer<256) {
        allow_act=TRUE;
      }
    } else if ( //Bullet has hit something
        bullet_on_ground_id!=-1 || //on a gound
        IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT) ||
        Bullet[bullet_id].range<=0 //end of range
      ) {
        allow_act=TRUE;
    }
  //}


//^^ condition
    if (allow_act) {
      if (hit_player) {
        if (Enemy[enemy_id]->health>0 && !in_main_menu && Bullet[bullet_id].graphics_type!=-1) {
          BulletDamagePlayerAct(bullet_id);
        }
      //End of Hit Player======
      } else if (bullet_on_ground_id>=GROUND_NUM && bullet_on_ground_id!=player.web_being_shot) { //Not on web being shot
        if (Bullet[bullet_id].graphics_type!=10) {
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
        } else {
          Bullet[bullet_id].speed=0.01;
          Bullet[bullet_id].speed_multiplier=1;
          Bullet[bullet_id].range=100;
          Bullet[bullet_id].saved_ground_id=-1;
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



      bool predicate=FALSE;
      if (Bullet[bullet_id].graphics_type==10 || Bullet[bullet_id].graphics_type==11) {
        if (hit_player) {
          if (Enemy[enemy_id]->health<=0 && Enemy[enemy_id]->damage_taken_timer<250 && Enemy[enemy_id]->death_timer>10) {
            predicate=TRUE;
          }
        }
      } else {
        if (HitPlayer(bullet_id,35,35)) { //enemy gib to be absorbed
          predicate=TRUE;
        }
      }

      if ((Bullet[bullet_id].graphics_type!=5 && Bullet[bullet_id].graphics_type!=10) || 
            predicate || 
            Bullet[bullet_id].range<=0 || 
            IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)
          ) {
          StopBullet(bullet_id,FALSE); 


          if (predicate && (Bullet[bullet_id].graphics_type==10 || Bullet[bullet_id].graphics_type==11) && player.health>0) { //heal player
            if (game_audio) {
              PlaySound(spamSoundEffectCache[8].audio, NULL, SND_MEMORY | SND_ASYNC); //gain snd
            }

            player.health+=0.2;
            player.block_health+=3;
            if (player.block_health>player.block_health_max) {
              player.block_health=player.block_health_max;
              player.health+=0.2;
            }
            player.exp++;

            if ((!game_hard && player.exp>MAX_EXP_NUM_NORMAL) || (game_hard && player.exp>MAX_EXP_NUM_HARD)) {
              player.exp=0;
              player.bullet_num++;
              if (player.max_web_num<MAX_WEB_NUM) {
                player.max_web_num++;
              } else {
                player.health+=0.5;
              }
            }
            player.show_health_timer=HP_SHOW_TIMER_NUM;
            player.show_block_health_timer=HP_SHOW_TIMER_NUM;
            player.show_exp_timer=HP_SHOW_TIMER_NUM;
          }


        //Enemy bullet shot array arrangement
	      for (int j=Bullet[bullet_id].saved_pos;j<Enemy[enemy_id]->bullet_shot_num-1;j++) { //shift to left in enemy bullet shot arr from bullet shot
	        Enemy[enemy_id]->bullet_shot_arr[j]=Enemy[enemy_id]->bullet_shot_arr[j+1];
            Bullet[Enemy[enemy_id]->bullet_shot_arr[j]].saved_pos--;
          }
          Bullet[bullet_id].saved_pos=-1;
	      Enemy[enemy_id]->bullet_shot_arr[Enemy[enemy_id]->bullet_shot_num-1]=-1; //remove bullet from arr
          Enemy[enemy_id]->bullet_shot_num--;
      } else if (bullet_on_ground_id!=-1) {
        Bullet[bullet_id].angle=GetBounceAngle(Bullet[bullet_id].angle,Ground[bullet_on_ground_id]->angle);
            //2*M_PI-Bullet[bullet_id].angle+2*Ground[bullet_on_ground_id]->angle; //ricochet/rebounding
        /*if (Bullet[bullet_id].angle>2*M_PI) { //hreater
          Bullet[bullet_id].angle-=2*M_PI;
        }
        if (Bullet[bullet_id].angle<0) {
          Bullet[bullet_id].angle+=2*M_PI;
        }*/
     
        if (Bullet[bullet_id].saved_ground_id==bullet_on_ground_id && Bullet[bullet_id].graphics_type!=10) { //prevents riding of wall
          Bullet[bullet_id].range=-1;
        }
        Bullet[bullet_id].saved_ground_id=bullet_on_ground_id;
      }
    } //end of allow act
}



void MapEditorBulletAct(int bullet_id)
{
  if (Bullet[bullet_id].range<=0) {
    StopBullet(bullet_id,FALSE); 
  }
}


void PlayerBulletAct(int bullet_id,int enemy_id)
{      
  
    bool allow_act;
    int bullet_on_ground_id=-1;
    bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,2,2);
    allow_act=FALSE;
    if (Bullet[bullet_id].bounce_timer>0) {
      Bullet[bullet_id].bounce_timer--;
    }
	if (bullet_on_ground_id!=-1) {//not hit self but another platform
	  allow_act=TRUE;
    } else if (IsOutOfBounds(Bullet[bullet_id].x,Bullet[bullet_id].y,5,MAP_WIDTH,MAP_HEIGHT)) {//out of bounds
	  allow_act=TRUE;
    } else { //travelling player bullet
       if (enemy_id==-1) {//travelling sniper bullet
        player.pivot_length=GetDistance(Bullet[bullet_id].x,Bullet[bullet_id].y,player.x,player.y);
        player.pivot_angle=GetCosAngle(player.x-Bullet[bullet_id].x,player.pivot_length);
        double _a=player.x;
        double _b=player.y;
        double _l=player.pivot_length/PLAYER_FLING_WEB_NUM;
        int tmp_ground_id=-1;
        for (int i=0;i<PLAYER_FLING_WEB_NUM;i++) {
          player_fling_web.x[i]=_a;
          player_fling_web.y[i]=_b;          
          player_fling_web.sprite_x[i]=-20;//_a+player.cam_x+player.cam_move_x+player.cam_mouse_move_x;
          player_fling_web.sprite_y[i]=-20;//_b+player.cam_y+player.cam_move_y+ player.cam_mouse_move_y;
          //printf("a");

          //to allow web bending
          tmp_ground_id=GetOnGroundId(player_fling_web.x[i],player_fling_web.y[i],2,2);
          if (tmp_ground_id!=-1) {
            if (GetDistance(player_fling_web.x[i],player_fling_web.y[i],Ground[tmp_ground_id]->x1,Ground[tmp_ground_id]->y1)<=NODE_SIZE ||
                GetDistance(player_fling_web.x[i],player_fling_web.y[i],Ground[tmp_ground_id]->x2,Ground[tmp_ground_id]->y2)<=NODE_SIZE) {
              //change pivot
              //player.pivot_x=player_fling_web.x[i];
              //player.pivot_y=player_fling_web.y[i];
              //InitPlayerFlingWeb();
              Bullet[bullet_id].x=player_fling_web.x[i];
              Bullet[bullet_id].y=player_fling_web.y[i];
              bullet_on_ground_id=tmp_ground_id;
        	  allow_act=TRUE;
              break;
            }
          }
          if (player.y>Bullet[bullet_id].y) {
            _a-=_l*cos(player.pivot_angle);
           _b-=_l*sin(player.pivot_angle);
           } else {
            _a-=_l*cos(player.pivot_angle);
            _b+=_l*sin(player.pivot_angle);
          }
        }
      }     
    }
	if (allow_act) {//reaching end of range
      //player_web_swinging related
      if (enemy_id==-1) { //sniping
        if (Bullet[bullet_id].range>0) {
          if (bullet_on_ground_id!=-1) {
            player.pivot_on_ground_id=bullet_on_ground_id;
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
          Bullet[bullet_id].angle=GetBounceAngle(Bullet[bullet_id].angle,Ground[bullet_on_ground_id]->angle);
                //2*M_PI-Bullet[bullet_id].angle+2*Ground[bullet_on_ground_id]->angle; //real
          if (bullet_on_ground_id>=GROUND_NUM) { //elastic web            
            Bullet[bullet_id].range+=80;
            Bullet[bullet_id].speed_multiplier+=2;
            if (Bullet[bullet_id].graphics_type!=7)
              Bullet[bullet_id].damage+=1;
            else
              Bullet[bullet_id].damage+=0.2;
            Bullet[bullet_id].start_range=Bullet[bullet_id].range;
          } else {
            switch (Bullet[bullet_id].graphics_type) {
              case 5: //kpt 3, 5
                Bullet[bullet_id].range/=12;
                break;
              case 9: //kpt 1
                Bullet[bullet_id].range/=2;
                break;
              default:
                Bullet[bullet_id].range/=4;
                break;
            }
          }
          /*if (Bullet[bullet_id].angle>2*M_PI) { //hreater
            Bullet[bullet_id].angle-=2*M_PI;
          }
          if (Bullet[bullet_id].angle<0) {
            Bullet[bullet_id].angle+=2*M_PI;
          }*/
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


void ResetBulletRain()
{
  for (int i=SHOOT_BULLET_NUM;i<BULLET_NUM;i++) {
    Bullet[i].bounce_timer=0;
    Bullet[i].playsnd=FALSE;
    Bullet[i].shot=FALSE;
    Bullet[i].near_miss=FALSE;
    Bullet[i].in_water=FALSE;
    Bullet[i].range=0;
    Bullet[i].start_range=0;
    Bullet[i].ospeed=-1;
    Bullet[i].speed=-1;
    Bullet[i].ospeed_multiplier=0;
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
    Bullet[i].saved_node_grid_id=-1;
  }
}


void InitBulletRain()
{
  int max_bullet_num;
  if (GR_WIDTH>800) {
    max_bullet_num=BULLET_NUM;
  } else {
    max_bullet_num=SHOOT_BULLET_NUM+(RAIN_NUM/5);
  }
  for (int i=SHOOT_BULLET_NUM;i<max_bullet_num;i++) {
  //for (int i=SHOOT_BULLET_NUM;i<BULLET_NUM;i++) {
    int rand_x=RandNum(player.x-GR_WIDTH/2,player.x+GR_WIDTH/2,frame_tick); //so it doest get stuck to ground
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
        rain_grad_run,
        rain_grad_rise,
        0 //angle            
    );
  }
}

void RainBulletAct(int bullet_id)
{
  bool hit_player=FALSE;
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
    if (NodeGrid[bullet_on_node_grid_id]->node_water) {
      allow_act=1;
      bullet_on_ground_id=-1;
    } else if ((NodeGrid[bullet_on_node_grid_id]->node_no_rain || !NodeGrid[bullet_on_node_grid_id]->node_no_shade)) {
      bullet_on_ground_id=GetOnGroundId(Bullet[bullet_id].x,Bullet[bullet_id].y,NODE_SIZE,NODE_SIZE);
    }
  }


  /*if (GetDistance(Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y,Bullet[bullet_id].x,Bullet[bullet_id].y)<=22) {
    //Bullet[bullet_id].angle=RandAngle(0,360,player.seed);//RandNum(-M_PI_2*100,M_PI_2*100,Enemy[enemy_id]->seed)/100;
    //Bullet[bullet_id].speed=Bullet[player.bullet_shot].speed;
    //Bullet[bullet_id].speed_multiplier=Bullet[player.bullet_shot].speed_multiplier;
    if (game_audio) {
      Bullet[player.bullet_shot].playsnd=TRUE;
    }
    allow_act=0;
  }*/

  /*int bk;
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

      if (Bullet[bk].graphics_type!=6) { //hit enemy bullet, scatter if NOT type 6
        Bullet[bk].angle=RandAngle(0,360,player.seed);
        Bullet[bk].range-=4;
      }
      allow_act=0;
    }
  }*/ //end of for,

  hit_player=HitPlayer(bullet_id,22,22);
  if (hit_player) { //hit player
    if (player.rain_wet_timer==0) {
      rain_sound_duration=0;
    }
    player.rain_wet_timer=260;//60;
    allow_act=2;
    //BulletDamagePlayerAct(bullet_id);
  }

  if (player.rain_wet_timer==0) {
    if (Bullet[bullet_id].sprite_x>0 && Bullet[bullet_id].sprite_y<GR_WIDTH) {
      if (Bullet[bullet_id].sprite_y>0 && Bullet[bullet_id].sprite_y<GR_HEIGHT) {
        if (player.visible_rain_wet_timer==0) {
          rain_sound_duration=0;
        }
          player.visible_rain_wet_timer=160;
      }
    }
  }

  RainBulletTransitNodeGrid(bullet_id);

  if (bullet_on_ground_id!=-1) {//hit platform
    allow_act=0;
  }

  if (allow_act!=-1) { //perform action
    if (allow_act==1) { //out of range
      Bullet[bullet_id].range=-1;
    } else if (allow_act==0 || allow_act==2){ //Ricochet off ground
      if (allow_act==0) {
        Bullet[bullet_id].angle=GetBounceAngle(Bullet[bullet_id].angle,Ground[bullet_on_ground_id]->angle)
/*2*M_PI-Bullet[bullet_id].angle+2*Ground[bullet_on_ground_id]->angle*/+RandAngle(-15,15,frame_tick); //slight random when hit
      } else if (allow_act==2) {
        Bullet[bullet_id].angle=RandAngle(-360,360,frame_tick); //slight random when hit
      }
      Bullet[bullet_id].speed_multiplier=1;
      Bullet[bullet_id].speed=1;//0.4;
      Bullet[bullet_id].range=26;//13;
      Bullet[bullet_id].graphics_type=12; //rain type
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
    if (!in_map_editor) {
        int on_node_grid_id=GetGridId(Bullet[bullet_id].x,Bullet[bullet_id].y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
        if (on_node_grid_id!=-1) {
          if (NodeGrid[on_node_grid_id]->node_water && !Bullet[bullet_id].in_water) {
            Bullet[bullet_id].in_water=TRUE;
            if (enemy_id>-1 && enemy_id<ENEMY_NUM) {
              if (Enemy[enemy_id]->species==3) {
                Bullet[bullet_id].speed=Bullet[bullet_id].ospeed*2;
                Bullet[bullet_id].speed_multiplier=Bullet[bullet_id].ospeed_multiplier*2;
              } else {
                Bullet[bullet_id].speed=Bullet[bullet_id].ospeed/4;
              }
            } else {
              Bullet[bullet_id].speed=Bullet[bullet_id].ospeed/4;
            }
            //Bullet[bullet_id].speed=Bullet[bullet_id].ospeed/4;
          } else if (!NodeGrid[on_node_grid_id]->node_water && Bullet[bullet_id].in_water){
            Bullet[bullet_id].in_water=FALSE;
            Bullet[bullet_id].speed_multiplier=Bullet[bullet_id].ospeed_multiplier;
            Bullet[bullet_id].speed=Bullet[bullet_id].ospeed;
            Bullet[bullet_id].range/=8;
          }
        }
    }
    for (int i=0;i<Bullet[bullet_id].speed_multiplier;i++) {
      allow_act=FALSE;
      Bullet[bullet_id].sprite_x=(int)Bullet[bullet_id].x+(int)player.cam_x+(int)player.cam_move_x+(int)player.cam_mouse_move_x;
      Bullet[bullet_id].sprite_y=(int)Bullet[bullet_id].y+(int)player.cam_y+(int)player.cam_move_y+(int)player.cam_mouse_move_y;
  //----------------
      if (enemy_id<0) {//player bullet movement
        if (enemy_id==-1) { //web-snipe
          if (Bullet[bullet_id].range>0) {
            allow_act=TRUE;
          } else { //web too long, retract
            StopBullet(player.bullet_shot,TRUE); //Stop the web
            PlayerPlaceWeb(); //Web related
            //PlayerBulletLimitAct();
            player.web_being_shot=-1;
            player.bullet_shot=-1;
          }
          //allow_act=TRUE;
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

            if (PLAYER_BULLET_NUM-player.bullet_shot_num==15 && player.knives_per_throw>=15 && game_audio && player.health>0) { //reload
              PlayMemSnd(&channelSoundEffect[7],&channelSoundEffectCache[7],TRUE,5); 
            }

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
            
            //if (frame_tick%2==0)
              //rand_x=stickyTo(rand_x,VGRID_SIZE*2);
            //else
              //rand_x=stickyTo(rand_x,VGRID_SIZE);

            int rand_y=RandNum(player.y-GR_HEIGHT/2+6-player.cam_mouse_move_y,player.y+GR_HEIGHT/2-6-player.cam_mouse_move_y,frame_tick); //so it doest get stuck to ground
            if (rand_y<10)
              rand_y=RandNum(10,player.y+GR_HEIGHT/2-6-player.cam_mouse_move_y,frame_tick);
            else if (rand_y>MAP_HEIGHT)
              rand_y=RandNum(player.y-GR_HEIGHT/2-player.cam_mouse_move_y,MAP_HEIGHT-10,frame_tick);

            //if (frame_tick%2==0)
              //rand_y=stickyTo(rand_y,VGRID_SIZE*2);
            //else
              //rand_y=stickyTo(rand_y,VGRID_SIZE);

            bool allow_spawn=TRUE;
            //check if below spawned web
            double check_x1,check_x2,c1,c2;
            double rain_gradient=rain_grad_rise/rain_grad_run;

            int spawn_on_ng=GetGridId(rand_x,rand_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
            if (spawn_on_ng!=-1) {
              if (NodeGrid[spawn_on_ng]->node_no_rain || !NodeGrid[spawn_on_ng]->node_no_shade) {
                allow_spawn=FALSE;
              }
            } else { //==-1
              allow_spawn=FALSE;
            }


            if (allow_spawn) {
              for (int q=GROUND_NUM;q<GROUND_NUM+MAX_WEB_NUM;q++) {                
                if (Ground[q]->x1>-1 && Ground[q]->y1>-1) {
                  c1=GetGroundC(Ground[q]->x1-NODE_SIZE,Ground[q]->y1,rain_gradient);
                  c2=GetGroundC(Ground[q]->x2+NODE_SIZE,Ground[q]->y2,rain_gradient);
                  check_x1=GetX(rand_y,rain_gradient,c1);
                  check_x2=GetX(rand_y,rain_gradient,c2);

                  if (check_x1-NODE_SIZE<=rand_x && rand_x<=check_x2+NODE_SIZE 
                      && (rand_y>=min(Ground[q]->y1,Ground[q]->y2)-NODE_SIZE*10)
                     ) 
                    {
                    allow_spawn=FALSE;
                    break;
                 }
                }
              }
            }


            int c=BLUE;//Highlight(IsInvertedBackground(),BLUE,YELLOW);

            if (allow_spawn) {
              ShootBullet(
                bullet_id,
                -1,
                c,
                8, //graphics type
                MAP_HEIGHT, //range ==>
                1, //speed
                10, //speed multiplier
                0, //damage
                -3,
                stickyTo(rand_x,NODE_SIZE*4),
                stickyTo(rand_y,NODE_SIZE*4),
                0,
                0,
                rain_grad_run,
                rain_grad_rise,
                0 //angle            
              );
            } else {
              ShootBullet(
                bullet_id,
                -1,
                c,
                8, //graphics type
                -1, //range ==>
                1, //speed
                1, //speed multiplier
                0, //damage
                -3,
                0,//x
                0,//y
                0,
                0,
                rain_grad_run,
                rain_grad_rise,
                0 //angle            
              );
            }
          }
        } else if (enemy_id==-10) {
          if (Bullet[bullet_id].graphics_type==6 && Bullet[bullet_id].range<=Bullet[bullet_id].start_range-120) {
            Bullet[bullet_id].graphics_type=5;
          }
          allow_act=TRUE;
        }
      } else if (!player.time_breaker || Enemy[enemy_id]->time_breaker_immune || Enemy[enemy_id]->health<=0) {//enemy bullet movement
        if (Bullet[bullet_id].graphics_type==6 && Bullet[bullet_id].range<=Bullet[bullet_id].start_range-120) {
          //Bullet[bullet_id].graphics_type=1;
          Bullet[bullet_id].graphics_type=5;
        }
        if (!player.time_breaker) {
          allow_act=TRUE;
        } else if (Bullet[bullet_id].start_range-Bullet[bullet_id].range<50) { //timebreaker enabled
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
        } else if (enemy_id==-10) { //for mapeditor
          MapEditorBulletAct(bullet_id);
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
      PlaySound(spamSoundEffectCache[7].audio,NULL, SND_MEMORY | SND_ASYNC); //clang
    }
    Bullet[i].playsnd=FALSE;
  }
}



void RainAct()
{
  if (GR_WIDTH>800) {
    for (int i=SHOOT_BULLET_NUM;i<BULLET_NUM;i++) {
      BulletAct(i);
    }
  } else {
    for (int i=SHOOT_BULLET_NUM;i<SHOOT_BULLET_NUM+(RAIN_NUM/5);i++) {
      BulletAct(i);
    }
  }
}



void DrawBullet2(HDC hdc,int i,double x,double y,int color)
{
  int c;
  switch (Bullet[i].graphics_type) {
    case -1: //type breakout of web
      GrCircle(hdc,x,y,2,color,LTRED);
      break;
    case 0:
      GrCircle(hdc,x,y,4,color,-1);
      break;
    case 1:
      GrCircle(hdc,x,y,3,color,-1);
      break;
    case 2:
      GrCircle(hdc,x,y,4,color,color);
      break;
    case 3:
    case 6: //no fill glitery buullet
      GrCircle(hdc,x,y,3,color,-1);
      GrCircle(hdc,x,y,RandNum(0,5,frame_tick*player.seed),color,-1);
      break;
    case 4: //fill glitery bullet
      GrCircle(hdc,x,y,3,color,color);
      GrCircle(hdc,x,y,RandNum(0,5,frame_tick*player.seed),color,color);
      break;
    case 5: //long bullet 0
      {
      GrLine(hdc,x,y,x+10*cos(Bullet[i].angle),y+10*sin(Bullet[i].angle),color);
      }
      break;
    case 7: //long bullet 1
      {
      GrLine(hdc,x,y,x+6*cos(Bullet[i].angle),y+6*sin(Bullet[i].angle),color);
      }
      break;
    case 8: //rain
      {
      GrLine(hdc,x,y,x-128*cos(Bullet[i].angle),y-128*sin(Bullet[i].angle),color);
      }
      break;
    case 9: //long bullet 2
      {
      GrLine(hdc,x,y,x-20*cos(Bullet[i].angle),y-20*sin(Bullet[i].angle),color);
      }
      break;
    case 10: //death bullet
      {
        //GrCircle(hdc,x,y,Bullet[i]->size,color,color);
        GrCircle(hdc,x,y,2,color,color);
        c=WHITE;//Highlight(IsInvertedBackground(),WHITE,BLACK);
        GrCircle(hdc,x,y,RandNum(1,5,frame_tick*player.seed),c,-1);
      }
      break;

    case 11: //death player
      {
        //GrCircle(hdc,x,y,Bullet[i]->size,color,color);
        GrCircle(hdc,x,y,4,color,color);
        c=rgbPaint[player_pupil_color];
        GrCircle(hdc,x,y,RandNum(1,10,frame_tick*player.seed),c,-1);
        c=rgbPaint[player_iris_color];
        GrCircle(hdc,x,y,RandNum(1,10,(player.seed+10)*(frame_tick+4)),c,-1);
      }
      break;
    case 12: //no fill gliterry bullet, RAIN
      GrCircle(hdc,x,y,13,color,-1);
      GrCircle(hdc,x,y,RandNum(0,13,frame_tick*player.seed),color,-1);
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
