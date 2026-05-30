



void SetFGround(int i,int FGround_Ground_id)
{
  float tmp=F_GROUND[i].x2[FGround_Ground_id];
  float tmp2=F_GROUND[i].y2[FGround_Ground_id];
  if (F_GROUND[i].x1[FGround_Ground_id]>=F_GROUND[i].x2[FGround_Ground_id]) {
    F_GROUND[i].x2[FGround_Ground_id]=F_GROUND[i].x1[FGround_Ground_id]+0.0001;
    F_GROUND[i].x1[FGround_Ground_id]=tmp;

    F_GROUND[i].y2[FGround_Ground_id]=F_GROUND[i].y1[FGround_Ground_id];
    F_GROUND[i].y1[FGround_Ground_id]=tmp2;
  }
  //Set Ground's gradient
  F_GROUND[i].gradient[FGround_Ground_id]=GetGradient(
    F_GROUND[i].x1[FGround_Ground_id],
    F_GROUND[i].y1[FGround_Ground_id],
    F_GROUND[i].x2[FGround_Ground_id],
    F_GROUND[i].y2[FGround_Ground_id]);
  //Set Ground's C
  //F_GROUND[i].c[FGround_Ground_id]=GetGroundC(F_GROUND[i].x1[FGround_Ground_id],F_GROUND[i].y1[FGround_Ground_id],F_GROUND[i].gradient[FGround_Ground_id]);
  //Set Ground's angle
  F_GROUND[i].angle[FGround_Ground_id]=
        GetCosAngle(F_GROUND[i].x2[FGround_Ground_id]-F_GROUND[i].x1[FGround_Ground_id],
                                GetDistance(F_GROUND[i].x1[FGround_Ground_id],
                                            F_GROUND[i].y1[FGround_Ground_id],
                                            F_GROUND[i].x2[FGround_Ground_id],
                                            F_GROUND[i].y2[FGround_Ground_id])
                    );//cos(angle) = adjacent/hypothenuse
  if (F_GROUND[i].gradient[FGround_Ground_id]<=0) {
    F_GROUND[i].angle[FGround_Ground_id]=-F_GROUND[i].angle[FGround_Ground_id];
  }
}


void InitFallingGround(int j)
{
    F_GROUND[j].x=F_GROUND[j].x_start;
    F_GROUND[j].y=F_GROUND[j].y_start;
    F_GROUND[j].speed=(float)F_GROUND[j].ospeed/10;

    F_GROUND[j].spin_angle=deg2rad((float)F_GROUND[j].ospin_angle/100.0);
    F_GROUND[j].spin_angle_delta=deg2rad((float)F_GROUND[j].ospin_angle_delta/100.0);
    F_GROUND[j].spin_angle_min=deg2rad((float)F_GROUND[j].ospin_angle_min/100.0);
    F_GROUND[j].spin_angle_max=deg2rad((float)F_GROUND[j].ospin_angle_max/100.0);

    F_GROUND[j].travel_dist=0;
    F_GROUND[j].travel_dist_max=GetDistance(F_GROUND[j].x_start,F_GROUND[j].y_start,F_GROUND[j].x_end,F_GROUND[j].y_end);
    

    //if (F_GROUND[j].ox_oscillation_angle_delta>0)
    F_GROUND[j].x_oscillation_angle_delta=deg2rad((float)F_GROUND[j].ox_oscillation_angle_delta/100.0);
    //else
      //F_GROUND[j].x_oscillation_angle_delta=abs(deg2rad((float)F_GROUND[j].ox_oscillation_angle_delta/10000.0));
    F_GROUND[j].x_oscillation_angle_max=deg2rad((float)F_GROUND[j].ox_oscillation_angle_max/100.0);
    F_GROUND[j].x_oscillation_angle=0;//deg2rad((float)F_GROUND[j].ox_oscillation_angle/100.0);

    //if (F_GROUND[j].oy_oscillation_angle_delta>0)
      F_GROUND[j].y_oscillation_angle_delta=deg2rad((float)F_GROUND[j].oy_oscillation_angle_delta/100.0);
    //else
      //F_GROUND[j].y_oscillation_angle_delta=abs(deg2rad((float)F_GROUND[j].oy_oscillation_angle_delta/10000.0));
    F_GROUND[j].y_oscillation_angle_max=deg2rad((float)F_GROUND[j].oy_oscillation_angle_max/100.0);
    F_GROUND[j].y_oscillation_angle=deg2rad((float)F_GROUND[j].oy_oscillation_angle/100.0);

    //F_GROUND[j].x_oscillation_max_timer=
    //F_GROUND[j].x_oscillation_timer=F_GROUND[j].ox_oscillation_angle;

    /*float height=F_GROUND[j].y_start-F_GROUND[j].y_end;
    float length=F_GROUND[j].x_start-F_GROUND[j].x_end;
    if (length==0)
      length=1;
    F_GROUND[j].travel_angle=GetTanAngle(height,length); //O A*/
    

    float length=F_GROUND[j].x_end-F_GROUND[j].x_start;
    float dist=GetDistance(F_GROUND[j].x_start,F_GROUND[j].y_start,F_GROUND[j].x_end,F_GROUND[j].y_end);
    if (F_GROUND[j].y_end<F_GROUND[j].y_start) {
      F_GROUND[j].travel_angle=-GetCosAngle(length,dist); //A H
    } else {
      F_GROUND[j].travel_angle=GetCosAngle(length,dist); //A H
    }

    for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {
      F_GROUND[j].solid_ground_ids[i]=0;
    }

    F_GROUND[j].solid_ground_num=0;
    for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {
      F_GROUND[j].center_length1[i]=GetDistance(0,0,F_GROUND[j].ox1[i]-FGROUND_SIZE/2,F_GROUND[j].oy1[i]-FGROUND_SIZE/2); //runs on init only
      F_GROUND[j].center_length2[i]=GetDistance(0,0,F_GROUND[j].ox2[i]-FGROUND_SIZE/2,F_GROUND[j].oy2[i]-FGROUND_SIZE/2); //runs on init only
      F_GROUND[j].center_length3[i]=GetDistance(0,0,F_GROUND[j].ox3[i]-FGROUND_SIZE/2,F_GROUND[j].oy3[i]-FGROUND_SIZE/2); //runs on init only

      F_GROUND[j].center_angle1[i]=atan2(F_GROUND[j].oy1[i]-FGROUND_SIZE/2,F_GROUND[j].ox1[i]-FGROUND_SIZE/2); //runs on init only
      F_GROUND[j].center_angle2[i]=atan2(F_GROUND[j].oy2[i]-FGROUND_SIZE/2,F_GROUND[j].ox2[i]-FGROUND_SIZE/2); //runs on init only
      F_GROUND[j].center_angle3[i]=atan2(F_GROUND[j].oy3[i]-FGROUND_SIZE/2,F_GROUND[j].ox3[i]-FGROUND_SIZE/2); //runs on init only


      F_GROUND[j].x1[i]=cos(F_GROUND[j].spin_angle+F_GROUND[j].center_angle1[i])*F_GROUND[j].center_length1[i] +F_GROUND[j].x_start;
      F_GROUND[j].y1[i]=sin(F_GROUND[j].spin_angle+F_GROUND[j].center_angle1[i])*F_GROUND[j].center_length1[i] +F_GROUND[j].y_start;
      F_GROUND[j].x2[i]=cos(F_GROUND[j].spin_angle+F_GROUND[j].center_angle2[i])*F_GROUND[j].center_length2[i] +F_GROUND[j].x_start;
      F_GROUND[j].y2[i]=sin(F_GROUND[j].spin_angle+F_GROUND[j].center_angle2[i])*F_GROUND[j].center_length2[i] +F_GROUND[j].y_start;
      F_GROUND[j].x3[i]=cos(F_GROUND[j].spin_angle+F_GROUND[j].center_angle3[i])*F_GROUND[j].center_length3[i] +F_GROUND[j].x_start;
      F_GROUND[j].y3[i]=sin(F_GROUND[j].spin_angle+F_GROUND[j].center_angle3[i])*F_GROUND[j].center_length3[i] +F_GROUND[j].y_start;

      //Set Solid Grounds
      if ((F_GROUND[j].type[i]==0 || F_GROUND[j].type[i]==1) && !F_GROUND[j].is_ghost[i]) {
        F_GROUND[j].solid_ground_ids[F_GROUND[j].solid_ground_num]=i;
        F_GROUND[j].solid_ground_num++;        
      }

      SetFGround(j,i);
    }


}



void InitFallingGrounds()
{
  for (int j=0;j<FGROUND_NUM;j++) {
    InitFallingGround(j);
  } 
}



float FGGetLineTargetAngle(int fground_id,int FGround_Ground_id,float x,float y)
{

  float E=0,
      hypothenuse=0,
      adjacent=0;
  if (x>(F_GROUND[fground_id].x1[FGround_Ground_id]+F_GROUND[fground_id].x2[FGround_Ground_id])/2) {//right of Ground
    hypothenuse=
      GetDistance(
	  F_GROUND[fground_id].x1[FGround_Ground_id],
	  F_GROUND[fground_id].y1[FGround_Ground_id],
	  x,
	  y
	);
    adjacent=x-F_GROUND[fground_id].x1[FGround_Ground_id];
    E=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  F_GROUND[fground_id].x1[FGround_Ground_id],
	  F_GROUND[fground_id].y1[FGround_Ground_id]
      )<=0) {
      E=-E;
    }
  } else {//left of Ground
    hypothenuse=
      GetDistance(
	  F_GROUND[fground_id].x2[FGround_Ground_id],
	  F_GROUND[fground_id].y2[FGround_Ground_id],
	  x,
	  y
	);
    adjacent=F_GROUND[fground_id].x2[FGround_Ground_id]-x;
    E=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  F_GROUND[fground_id].x2[FGround_Ground_id],
	  F_GROUND[fground_id].y2[FGround_Ground_id]
        )>0) {
      E=-E;
    }
  }
  return E;
}

float FGGetLineTargetHeight(int fground_id,int FGround_Ground_id,float E,float x,float y)
{
  float h=0;
  if (x>(F_GROUND[fground_id].x1[FGround_Ground_id]+F_GROUND[fground_id].x2[FGround_Ground_id])/2) {//right of Ground
    h=sin(F_GROUND[fground_id].angle[FGround_Ground_id]-E)*
      GetDistance(
	F_GROUND[fground_id].x1[FGround_Ground_id],
	F_GROUND[fground_id].y1[FGround_Ground_id],
	x,
	y
    );
  } else {//left of Ground
    h=sin(2*M_PI-F_GROUND[fground_id].angle[FGround_Ground_id]-E)*
      GetDistance(
	F_GROUND[fground_id].x2[FGround_Ground_id],
	F_GROUND[fground_id].y2[FGround_Ground_id],
	x,
	y
    );
  }
  return h;
}

int GetOnFGroundGroundId(int fground_id,float x,float y,float min_range_1,float min_range_2)
{
  int j=-1;
  int i=0,FGround_Ground_id=0,on_grid_id=0;
  float ground_entity_E=0,height_from_ground=0;
  if (F_GROUND[fground_id].x-FGROUND_DETECT_SIZE/2<x && x<F_GROUND[fground_id].x+FGROUND_DETECT_SIZE/2 && 
      F_GROUND[fground_id].y-FGROUND_DETECT_SIZE/2<y && y<F_GROUND[fground_id].y+FGROUND_DETECT_SIZE/2) { //within bounderies
    for (i=0;i<F_GROUND[fground_id].solid_ground_num;i++) {
      FGround_Ground_id=F_GROUND[fground_id].solid_ground_ids[i];
      if (F_GROUND[fground_id].x1[FGround_Ground_id]-min_range_1<=x && x<=F_GROUND[fground_id].x2[FGround_Ground_id]+min_range_1) {//within x
        if ((F_GROUND[fground_id].y1[FGround_Ground_id]-min_range_1<=y && y<=F_GROUND[fground_id].y2[FGround_Ground_id]+min_range_1) ||
            (F_GROUND[fground_id].y2[FGround_Ground_id]-min_range_1<=y && y<=F_GROUND[fground_id].y1[FGround_Ground_id]+min_range_1)) {//within y
          ground_entity_E=FGGetLineTargetAngle(fground_id,FGround_Ground_id,x,y);
          height_from_ground=FGGetLineTargetHeight(fground_id,FGround_Ground_id,ground_entity_E,x,y);
          if (-min_range_2<height_from_ground && height_from_ground<min_range_2 && FGround_Ground_id!=player.saved_fground_ground_id ) {
            j=FGround_Ground_id;
            if (j!=-1) {
              return j;
            }
            break;
          }
        }
      }
    }
  } //end of if

  if (j==-1) {
    return player.saved_fground_ground_id;
  }
  return -1;
}



void FallingGroundAct()
{
  for (int j=0;j<FGROUND_NUM;j++) {
    //Move Falling Ground Axis
    if (F_GROUND[j].travel_dist>=F_GROUND[j].travel_dist_max
    ) {
      F_GROUND[j].travel_dist=0;
      F_GROUND[j].x=F_GROUND[j].x_start;
      F_GROUND[j].y=F_GROUND[j].y_start;
    } else {
      for (int k=0;k<F_GROUND[j].speed_multiplier;k++) {
        F_GROUND[j].x+=F_GROUND[j].speed*cos(F_GROUND[j].travel_angle+F_GROUND[j].x_oscillation_angle);
        F_GROUND[j].y+=F_GROUND[j].speed*sin(F_GROUND[j].travel_angle+F_GROUND[j].y_oscillation_angle);
      }
    }

    //Spin Angle
    F_GROUND[j].spin_angle+=F_GROUND[j].spin_angle_delta;
    if (F_GROUND[j].spin_angle>=F_GROUND[j].spin_angle_max || F_GROUND[j].spin_angle<=F_GROUND[j].spin_angle_min) {
      F_GROUND[j].spin_angle_delta=-F_GROUND[j].spin_angle_delta;
    }

    //X Oscillation
    F_GROUND[j].x_oscillation_angle+=F_GROUND[j].x_oscillation_angle_delta;
    if (F_GROUND[j].x_oscillation_angle>F_GROUND[j].x_oscillation_angle_max || F_GROUND[j].x_oscillation_angle<-F_GROUND[j].x_oscillation_angle_max) {
      F_GROUND[j].x_oscillation_angle_delta=-F_GROUND[j].x_oscillation_angle_delta;
    }

    //Y Oscillation
    F_GROUND[j].y_oscillation_angle+=F_GROUND[j].y_oscillation_angle_delta;
    if (abs(F_GROUND[j].y_oscillation_angle)>F_GROUND[j].y_oscillation_angle_max) {
      F_GROUND[j].y_oscillation_angle_delta=-F_GROUND[j].y_oscillation_angle_delta;
    }


    //Friction move player as well depending on spin rate
    

    //Drag Axis along 
    if (F_GROUND[j].speed_multiplier>0) {
      F_GROUND[j].travel_dist+=F_GROUND[j].speed;
      for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {
        F_GROUND[j].x1[i]=cos(F_GROUND[j].spin_angle+F_GROUND[j].center_angle1[i])*F_GROUND[j].center_length1[i] +F_GROUND[j].x;
        F_GROUND[j].y1[i]=sin(F_GROUND[j].spin_angle+F_GROUND[j].center_angle1[i])*F_GROUND[j].center_length1[i] +F_GROUND[j].y;
        F_GROUND[j].x2[i]=cos(F_GROUND[j].spin_angle+F_GROUND[j].center_angle2[i])*F_GROUND[j].center_length2[i] +F_GROUND[j].x;
        F_GROUND[j].y2[i]=sin(F_GROUND[j].spin_angle+F_GROUND[j].center_angle2[i])*F_GROUND[j].center_length2[i] +F_GROUND[j].y;
        F_GROUND[j].x3[i]=cos(F_GROUND[j].spin_angle+F_GROUND[j].center_angle3[i])*F_GROUND[j].center_length3[i] +F_GROUND[j].x;
        F_GROUND[j].y3[i]=sin(F_GROUND[j].spin_angle+F_GROUND[j].center_angle3[i])*F_GROUND[j].center_length3[i] +F_GROUND[j].y;
        SetFGround(j,i);
      }
    }

  }

}


void PlayerOnFGroundAction(int speed, int grav, int height_from_player_x)
{
  player.angle=F_GROUND[player.on_fground_id].angle[player.on_fground_ground_id];//Ground[player.on_ground_id]->angle; //set player angle

  //Player sticking to FGROUND, without this the player will be bounced off
  //move_x(-cos(player.angle+M_PI_2)*0.01);
  //move_y(-sin(player.angle+M_PI_2)*0.01);
  
  if (player.in_air_timer>0) {
    if (player.in_air_timer>1000) { //make player rebound
      player.in_air_timer=1000;
    }
    if (player.type==0) {
    if (player.rst_down) { //spinning
      player.is_rebounding=TRUE;
      //if (player.rst_left || player.rst_right) { //continue spin-rebound
        player.in_air_timer=1000;
      //}
    } else { //not reboubding
      player.is_rebounding=FALSE;
      player.in_air_timer=1;

      player.jump_height=0; //Stop Jump & stick to ground
      //if (player.uppercut) {
        //player.jump_height=-1; //Stop Jump & stick to ground
      //}
      player.jump=FALSE;
    }
    }
    player.in_air_timer--;
  }

//outwards from ground
  //above ground
  if (0<height_from_player_x && height_from_player_x<10) {
    player.current_above=TRUE;
    player.current_below=FALSE;
    player.previous_above=TRUE;
    player.previous_below=FALSE;
    player.on_a_ground=TRUE;
    player.on_ground_timer=20;

    if (height_from_player_x<5 || player.is_rebounding /*|| player.is_swinging*/ || player.type==1) {
    //if (height_from_player_x<3 || player.is_rebounding /*|| player.is_swinging*/ || player.type==1) {
      if (player.is_rebounding) {
        move_x(-cos(player.angle+M_PI_2)*2);
        move_y(-sin(player.angle+M_PI_2)*2);
      } else {
        move_x(-cos(player.angle+M_PI_2));
        move_y(-sin(player.angle+M_PI_2));
      }
    }

    player.jump_angle=player.angle+M_PI_2;
    if (player.angle<0) {/*Slope -. /*/
      player.jump_angle2=2*M_PI+player.angle-M_PI_2;
    } else {/*Slope -. \*/
      player.jump_angle2=player.angle-M_PI_2;
    }
    player.fling_distance=0; //on ground, stop flinging

    //angle of incidence and reflection
    player.angle_of_reflection=
        //2*M_PI-player.angle_of_incidence+2*player.angle; //real
        GetBounceAngle(player.angle_of_incidence,player.angle);


    if (!player.is_swinging && player.bullet_shot==-1) {
      player.angle_of_incidence=player.angle_of_reflection;
    }

  //below ground
  } else if (-10<height_from_player_x && height_from_player_x<=0) { 
    player.below_fground_ground_edge_timer=5;
    player.current_below=TRUE;
    player.current_above=FALSE;
    player.previous_above=FALSE;
    player.previous_below=TRUE;
    player.on_a_ground=TRUE;
    player.on_ground_timer=20;
    if (height_from_player_x>-5 || player.is_rebounding/* || player.is_swinging*/ || player.type==1) {
    //if (height_from_player_x>-3 || player.is_rebounding/* || player.is_swinging*/ || player.type==1) {
      if (player.is_rebounding) {
        move_x(-cos(player.angle-M_PI_2)*2); //go outwards 
        move_y(-sin(player.angle-M_PI_2)*2);
      } else {
        move_x(-cos(player.angle-M_PI_2)); //go outwards 
        move_y(-sin(player.angle-M_PI_2));
      }
    }

    player.jump_angle=player.angle-M_PI_2;
    if (player.angle<0) {/*Slope -. /*/
      player.jump_angle2=M_PI_2+player.angle;
    } else {/*Slope -. \*/
      player.jump_angle2=player.angle+M_PI_2;
    }
    player.fling_distance=0; //on ground, stop flinging

    //angle of incidence and reflection
    player.angle_of_reflection=GetBounceAngle(player.angle,player.angle_of_incidence);
    //2*M_PI-player.angle_of_incidence+2*player.angle; //real
    if (!player.is_swinging && player.bullet_shot==-1) {
      player.angle_of_incidence=player.angle_of_reflection;
    }
  }

  if (height_from_player_x>9 || height_from_player_x<-9) {//leave ground when out of circular range
    player.on_a_ground=FALSE;
    player.on_ground_timer=0;
  }
}



void DrawFallingGround(HDC hdc, HDC hdc2)
{
  int c;
  int fgcx1;
  int fgcy1;
  int fgcx2;
  int fgcy2;
  int fgcx3;
  int fgcy3;
  int 
    px=player.cam_x,
    py=player.cam_y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y,
    cx3=player.cam_limiter_x,
    cy3=player.cam_limiter_y;
  //NOTE: STARTS AT CENTER
  char mytxt[16];
  for (int j=0;j<FGROUND_NUM;j++) {
    //Draw Falling Ground


    //debug
    /*fgcx1=(int)F_GROUND[j].x-FGROUND_SIZE/2+px+cx1+cx2+cx3;
    fgcy1=(int)F_GROUND[j].y-FGROUND_SIZE/2+py+cy1+cy2+cy3;
    for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {   
      sprintf(mytxt,"%d:%d,",i,F_GROUND[j].solid_ground_ids[i]);   
      GrPrint(hdc,fgcx1-16,fgcy1+i*16,mytxt,LTRED);
    }*/

    int pcamx=px+cx1+cx2+cx3;
    int pcamy=py+cy1+cy2+cy3;
    if (
      (
        (0<= F_GROUND[j].x+FGROUND_SIZE/2+pcamx && F_GROUND[j].x+FGROUND_SIZE/2+pcamx <=GR_WIDTH) ||
        (0<= F_GROUND[j].x-FGROUND_SIZE/2+pcamx && F_GROUND[j].x-FGROUND_SIZE/2+pcamx <=GR_WIDTH)
      ) &&
      (
        (0<= F_GROUND[j].y+FGROUND_SIZE/2+pcamy && F_GROUND[j].y+FGROUND_SIZE/2+pcamy <=GR_HEIGHT) ||
        (0<= F_GROUND[j].y-FGROUND_SIZE/2+pcamy && F_GROUND[j].y-FGROUND_SIZE/2+pcamy <=GR_HEIGHT)
      )
    ) { //only draw if visible on screen
    for (int i=0;i<GROUND_IN_FGROUND_NUM;i++) {
        
      fgcx1=(int)F_GROUND[j].x1[i]+pcamx;
      fgcy1=(int)F_GROUND[j].y1[i]+pcamy;
      fgcx2=(int)F_GROUND[j].x2[i]+pcamx;
      fgcy2=(int)F_GROUND[j].y2[i]+pcamy;
      fgcx3=(int)F_GROUND[j].x3[i]+pcamx;
      fgcy3=(int)F_GROUND[j].y3[i]+pcamy;

      c=rgbPaintBrightness[lvl_map_background.dark_lvl][F_GROUND[j].color_id[i]];
      switch (F_GROUND[j].type[i]) {

        case 0: //line
        case 1: //line but floor not ceiling
          GrLine(hdc,fgcx1,fgcy1,fgcx2,fgcy2,c);
          break;
        case 2: //trifill
	        DrawTriFill(hdc,c,
               fgcx1,fgcy1,fgcx2,fgcy2,fgcx3,fgcy3,FALSE,0);
          break;
        case 3: //texture trifill
          {
          int texture_type=F_GROUND[j].texture_type[i];
          if (texture_type>=0 && texture_type<PLATFORM_TEXTURES_NUM) {
            DrawTexturedTriangle(hdc,hdc2,
              fgcx1,fgcy1,fgcx2,fgcy2,fgcx3,fgcy3,
              GamePlatformTextures[texture_type].palette_sprite);
          }
          }
          break;
      }
    }  
    }
  }
}











void PlayerActFGroundEdgeMovement()
{

  float edge_dist1;
  float edge_dist2;
  float distl=20;

  if (!player.blocking && !player.rst_up) {
      if (!player.is_on_fground_ground_edge) {
        if (player.on_fground_ground_id!=-1) {
          edge_dist1=GetDistance(player.x,player.y,F_GROUND[player.on_fground_id].x1[player.on_fground_ground_id],F_GROUND[player.on_fground_id].y1[player.on_fground_ground_id]); //left edge
          edge_dist2=GetDistance(player.x,player.y,F_GROUND[player.on_fground_id].x2[player.on_fground_ground_id],F_GROUND[player.on_fground_id].y2[player.on_fground_ground_id]); //right edge
          if (edge_dist1<distl || edge_dist2<distl) {
            if (edge_dist1<distl) {
              player.is_on_left_fground_ground_edge=TRUE;
              player.is_on_right_fground_ground_edge=FALSE;
            } else if (edge_dist2<distl) {
              player.is_on_left_fground_ground_edge=FALSE;
              player.is_on_right_fground_ground_edge=TRUE;
            }
            player.on_fground_ground_edge_id=player.on_fground_ground_id;
            player.saved_on_fground_ground_edge_id=player.on_fground_ground_edge_id;
            player.is_on_fground_ground_edge=TRUE;
          }
        }
      } else { //on ground edge id present
        //player.in_air_timer=0;
        if (player.is_on_left_fground_ground_edge) {
          edge_dist1=GetDistance(player.x,player.y,F_GROUND[player.on_fground_id].x1[player.on_fground_ground_edge_id],F_GROUND[player.on_fground_id].y1[player.on_fground_ground_edge_id]); //left edge
          player.edge_angle=GetCosAngle(player.x-F_GROUND[player.on_fground_id].x1[player.on_fground_ground_edge_id],edge_dist1);
          if (player.y<F_GROUND[player.on_fground_id].y1[player.on_fground_ground_edge_id]) { //above pivot
              player.above_fground_ground_edge=TRUE;
              player.below_fground_ground_edge=FALSE;
              if (player.rst_right) { //clockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=FALSE;
              } else if (player.rst_left) { //anticlockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=TRUE;
              }
          } else { //below pivot
              player.above_fground_ground_edge=FALSE;
              player.below_fground_ground_edge=TRUE;
              player.below_fground_ground_edge_timer=5;
              if (player.rst_right) { //clockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=TRUE;
              } else if (player.rst_left) { //anticlockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=FALSE;
              }
          }
          if (edge_dist1>distl+1) {
            player.on_fground_ground_edge_id=-1;
            player.saved_on_fground_ground_edge_id=-1;
            player.is_on_fground_ground_edge=FALSE;
            player.is_on_left_fground_ground_edge=FALSE;
            player.is_on_right_fground_ground_edge=FALSE;
          }
        } else if (player.is_on_right_fground_ground_edge) {
          edge_dist2=GetDistance(player.x,player.y,F_GROUND[player.on_fground_id].x2[player.on_fground_ground_edge_id],F_GROUND[player.on_fground_id].y2[player.on_fground_ground_edge_id]); //right edge
          player.edge_angle=GetCosAngle(player.x-F_GROUND[player.on_fground_id].x2[player.on_fground_ground_edge_id],edge_dist2);
          if (player.y<F_GROUND[player.on_fground_id].y2[player.on_fground_ground_edge_id]) { //above pivot
              player.above_fground_ground_edge=TRUE;
              player.below_fground_ground_edge=FALSE;
              if (player.rst_right) { //clockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=FALSE;
              } else if (player.rst_left) { //anticlockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=TRUE;
              }
          } else { //below pivot
              player.above_fground_ground_edge=FALSE;
              player.below_fground_ground_edge=TRUE;
              player.below_fground_ground_edge_timer=5;
              if (player.rst_right) { //clockwize
                move_x(-cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=TRUE;
              } else if (player.rst_left) { //anticlockwize
                move_x(cos(-player.edge_angle+M_PI_2)*0.25);
                move_y(-sin(-player.edge_angle+M_PI_2)*0.25);
                player.last_left=FALSE;
              }
          }
          if (edge_dist2>distl+1) {
            player.on_fground_ground_edge_id=-1;
            player.saved_on_fground_ground_edge_id=-1;
            player.is_on_fground_ground_edge=FALSE;
            player.is_on_left_fground_ground_edge=FALSE;
            player.is_on_right_fground_ground_edge=FALSE;
          }
        }
      }
  } else { //player is blocking
    player.on_fground_ground_edge_id=-1;
    player.saved_on_fground_ground_edge_id=-1;
    player.is_on_fground_ground_edge=FALSE;
    player.is_on_left_fground_ground_edge=FALSE;
    player.is_on_right_fground_ground_edge=FALSE;
  }

  if (player.on_fground_ground_id!=-1 && player.on_fground_ground_id!=player.on_fground_ground_edge_id) {
    player.on_fground_ground_edge_id=player.on_fground_ground_id;
    player.saved_on_fground_ground_edge_id=player.on_fground_ground_id;
    player.is_on_fground_ground_edge=FALSE;
    player.is_on_left_fground_ground_edge=FALSE;
    player.is_on_right_fground_ground_edge=FALSE;
//    player.below_fground_ground_edge_timer=0;
  }

}

