
F64 GetLineTargetAngle(I64 ground_id,F64 x,F64 y)
{
/***
$SP,"<1>",BI=1$




















***/
  F64 é=0,
      hypothenuse=0,
      adjacent=0;
  if (x>(Ground[ground_id].x1+Ground[ground_id].x2)/2) {//right of ground
    hypothenuse=
      GetDistance(
	  Ground[ground_id].x1,
	  Ground[ground_id].y1,
	  x,
	  y
	);
    adjacent=x-Ground[ground_id].x1;
    é=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  Ground[ground_id].x1,
	  Ground[ground_id].y1
      )<=0) {
      é=-é;
    }
  } else {//left of ground
    hypothenuse=
      GetDistance(
	  Ground[ground_id].x2,
	  Ground[ground_id].y2,
	  x,
	  y
	);
    adjacent=Ground[ground_id].x2-x;
    é=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  Ground[ground_id].x2,
	  Ground[ground_id].y2
        )>0) {
      é=-é;
    }
  }
  return é;
}

F64 GetLineTargetHeight(I64 ground_id,F64 é,F64 x,F64 y)
{
/***
$SP,"<2>",BI=2$










$SP,"<3>",BI=3$  













Sin(é) = Opposite/Hypothenuse
Opposite = Sin(é)*Hypothenuse
***/
  F64 h=0;
  if (x>(Ground[ground_id].x1+Ground[ground_id].x2)/2) {//right of ground
    h=Sin(Ground[ground_id].angle-é)*
      GetDistance(
	Ground[ground_id].x1,
	Ground[ground_id].y1,
	x,
	y
    );
  } else {//left of ground
    h=Sin(2*pi-Ground[ground_id].angle-é)*
      GetDistance(
	Ground[ground_id].x2,
	Ground[ground_id].y2,
	x,
	y
    );
  }
  return h;
}

U0 SetGround(I64 i)
{
  //Set Ground's gradient
  Ground[i].gradient=GetGradient(
    Ground[i].x1,
    Ground[i].y1,
    Ground[i].x2,
    Ground[i].y2);
  //Set ground's C
  Ground[i].c=GetGroundC(Ground[i].x1,Ground[i].y1,Ground[i].gradient);
  //Set ground's angle
  Ground[i].angle=GetCosAngle(Ground[i].x2-Ground[i].x1,
                                GetDistance(Ground[i].x1,
                                            Ground[i].y1,
                                            Ground[i].x2,
                                            Ground[i].y2)
                    );//cos(angle) = adjacent/hypothenuse
  if (Ground[i].gradient<=0) {
    Ground[i].angle=-Ground[i].angle;
  }
}

U0 GroundInit()
{
  I64 i,j;
//Set ground default and Web
  current_gsm=0;
  gsm_hold_timer=0;
  for (i=0;i<TOTAL_GROUNDS;i++) {
    for (j=0;j<GRID_NUM;j++) {
      Ground[i].saved_pos_in_grid[j]=-1;
      Ground[i].already_in_grid[j]=FALSE;
    }
    for (j=0;j<MULTI_SPRITE_NUM;j++) {
      Ground[i].msprite_x1[j]=
      Ground[i].msprite_y1[j]=
      Ground[i].msprite_x2[j]=
      Ground[i].msprite_y2[j]=-20;
      Ground[i].appear_timer[j]=0;
    }
    if (i<GROUND_NUM) {
    //set the saved data
      Ground[i].x1=saved_ground_x1[i];
      Ground[i].y1=saved_ground_y1[i];
      Ground[i].x2=saved_ground_x2[i];
      Ground[i].y2=saved_ground_y2[i];
      Ground[i].x3=saved_ground_x3[i];
      Ground[i].y3=saved_ground_y3[i];
 
      Ground[i].type=saved_ground_type[i];
      if (Ground[i].x1<1) {
        Ground[i].x1++;
      }
      if (Ground[i].y1<1) {
        Ground[i].y1++;
      }
      if (Ground[i].y2<1) {
        Ground[i].y2++;
      }
      if (Ground[i].x2<=Ground[i].x1) {
        Ground[i].x2=Ground[i].x1+1;
      }
      if (Ground[i].x3==Ground[i].x1 || Ground[i].x3==Ground[i].x2) {
	Ground[i].x3++;
      }
      if (Ground[i].type==3) {//trifill
	if (Ground[i].y1==Ground[i].y2) {
	  Ground[i].y2++;
        }
      }
      if (Ground[i].y3==Ground[i].y1) {
	Ground[i].y3+=2;
      }
      if (Ground[i].y3==Ground[i].y2) {
	Ground[i].y3+=2;
      }
      Ground[i].color=saved_ground_color[i];
      Ground[i].text=saved_ground_text[i];
      if (saved_ground_is_ghost[i]) {
        Ground[i].is_ghost=TRUE;
      } else {
        Ground[i].is_ghost=FALSE;
      }
    } else { 
      Ground[i].x1=-25;
      Ground[i].y1=2;
      Ground[i].x2=-20;
      Ground[i].y2=2;
      Ground[i].x3=-15;
      Ground[i].y3=2;
      Ground[i].color=TRANSPARENT;
      Ground[i].type=0;
      Ground[i].text="";
      Ground[i].is_ghost=FALSE;
    }
    Ground[i].health=-1;
    rendered_ground_num=0;
    Ground[i].height_from_player_x=0;
    Ground[i].within_render_distance=FALSE;
    Ground[i].angle=0;
    SetGround(i);
  }
}

U0 GroundInit2()
{
  I64 i=0;
  for (i=0;i<TOTAL_GROUNDS;i++) {
    Ground[i].sprite_x1=Ground[i].x1+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y1=Ground[i].y1+cam_y+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x2=Ground[i].x2+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y2=Ground[i].y2+cam_y+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x3=Ground[i].x3+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y3=Ground[i].y3+cam_y+cam_move_y-PLAYER_HEIGHT/2;
  }
}

U0 DestroyGround(I64 i)
{
  I64 lg_grid_id=0,node_grid_id=0,x=0,y=0,min=0,max=0;
  F64 lg_x=0,lg_y=0;
  if (-1<Ground[i].gradient<1) {
    for (x=Ground[i].x1;x<Ground[i].x2;x++) {
      lg_y=x*Ground[i].gradient+Ground[i].c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,GRID_SIZE,GRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      NodeGrid[node_grid_id].node_solid=FALSE;
    }
  } else { // x=(y-c)/m
    if (Ground[i].y1>Ground[i].y2) {
      min=Ground[i].y2;
      max=Ground[i].y1;
    } else {
      min=Ground[i].y1;
      max=Ground[i].y2;
    }
    for (y=min;y<max;y++) {
      lg_x=(y-Ground[i].c)/Ground[i].gradient;
      lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,GRID_SIZE,GRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      NodeGrid[node_grid_id].node_solid=FALSE;
    }
  }
  Ground[i].health=-1;
  Ground[i].x1=Ground[i].y1=Ground[i].x2=Ground[i].y2=-20;
}

I64 GetOnGroundId(F64 x,F64 y,F64 min_range_1,F64 min_range_2,Bool player)
{
  I64 i=0,j=-1,ground_id=0,on_grid_id=0;
  F64 ground_entity_é=0,height_from_ground=0;
  if (0<x<MAP_WIDTH && 0<y<MAP_HEIGHT) { //within bounderies
    on_grid_id=GetGridId(x,y,MAP_WIDTH,GRID_SIZE,GRID_NUM);//maths to get grid
    for (i=0;i<Grid[on_grid_id].max_ground_num;i++) {
      ground_id=Grid[on_grid_id].ground_ids[i];
      ground_entity_é=GetLineTargetAngle(ground_id,x,y);
      height_from_ground=GetLineTargetHeight(ground_id,ground_entity_é,x,y);
      if (player) {
        Ground[ground_id].height_from_player_x=height_from_ground;
      }
      if (Ground[ground_id].x1-min_range_1<=x<=Ground[ground_id].x2+min_range_1) {//within x
        if (Ground[ground_id].y1-min_range_1<=y<=Ground[ground_id].y2+min_range_1 ||
            Ground[ground_id].y2-min_range_1<=y<=Ground[ground_id].y1+min_range_1) {//within y
          if (player && -min_range_2<Ground[ground_id].height_from_player_x<min_range_2) { //change in ground
	    if (ground_id!=saved_ground_id && !Ground[ground_id].is_ghost) {
              j=ground_id;
	    }
          } else if (!Ground[ground_id].is_ghost && -min_range_2<height_from_ground<min_range_2) {
	    return ground_id;
	  }
	}
      }
    }
    if (player) {
      if (j==-1) {
	return saved_ground_id;
      } else {
	return j;
      }
    } else {
      return -1;
    }
  }
  return -1;
}

//--------------------------------
Bool IsSpeedBreaking()
{
  if (rst_speed_break && player_speed_breaker_units>0) {
    return TRUE;
  }
  return FALSE;
}


Bool YesLongFade()
{
  if (((DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-10)*3/4<=player_speed_breaker_recharge_minus) ||
      performance_grade_arr[0]>=2) {
    return TRUE;
  } else if (performance_grade_arr[1]>=2 ||
             performance_grade_arr[2]>=2) {
    return TRUE;
  }
  return FALSE;
}


Bool YesLongFade2()
{
  if (YesLongFade || IsSpeedBreaking) {
    return TRUE;
  } else if (grav>5 || the_bravery_tyrant) {
    return TRUE;
  } else if (player_speed>=5) {
    return TRUE;
  }
  return FALSE;
}


Bool IsInvertedBackground()
{
  if (map_background==1) {
    return TRUE;
  } else if (is_inverted && map_background==2) {
    return TRUE;
  }
  return FALSE;
}
//--------------------------------


U0 GroundAct()
{
  I64 i=0,j=0,k=0;
  gsm_hold_timer++;
  for (j=0;j<rendered_ground_num+MAX_WEB_NUM;j++) {
    if (j<rendered_ground_num) {
      i=render_grounds[j];
    } else {
      i=GROUND_NUM+(j-rendered_ground_num);
    }
    Ground[i].sprite_x1=Ground[i].x1+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y1=Ground[i].y1+cam_y+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x2=Ground[i].x2+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y2=Ground[i].y2+cam_y+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x3=Ground[i].x3+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y3=Ground[i].y3+cam_y+cam_move_y-PLAYER_HEIGHT/2;
//multisprite
    if (fade_ground) {
      k=0;
      if (gsm_hold_timer>1) {
        if (YesLongFade2 || player_in_air_cooldown>0) {
          Ground[i].appear_timer[current_gsm]=50;
        }
        Ground[i].msprite_x1[current_gsm]=Ground[i].sprite_x1;
        Ground[i].msprite_y1[current_gsm]=Ground[i].sprite_y1;
        Ground[i].msprite_x2[current_gsm]=Ground[i].sprite_x2;
        Ground[i].msprite_y2[current_gsm]=Ground[i].sprite_y2;
      }
      for (k=0;k<MULTI_SPRITE_NUM;k++) {
        if (Ground[i].appear_timer[k]>0 && !the_bravery_tyrant) {
          Ground[i].appear_timer[k]--;
        }    
      }
    }
  } //
  if (fade_ground && gsm_hold_timer>1) {    
    gsm_hold_timer=0;
    current_gsm++;
    if (current_gsm>=MULTI_SPRITE_NUM-1) {
      current_gsm=0;
    }
  }
}

Bool IsNormalView()
{
  if (!change_view) {
    return TRUE;
  } else if (on_ground_timer<1 && print_current_below) {
    return TRUE;    
  } else if(on_ground_timer<9) {
    if (print_current_above) {
      return TRUE;
    } else if (!print_current_below) {
      return TRUE;
    }
  }
  return FALSE;
}

U0 DrawGround(CTask *task,CDC *dc)
{
  I64 i=0,j=0;
  for (j=0;j<rendered_ground_num+MAX_WEB_NUM;j++) {
    if (j<rendered_ground_num) {
      i=render_grounds[j];
    } else {
      i=GROUND_NUM+(j-rendered_ground_num);
    }
    if (Ground[i].type==0) {
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
        if (IsNormalView) {//normal view
	  if (fade_ground) {
      	    if (i<GROUND_NUM) {
              Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color],dc);
            } else {
              dc->color=CYAN+TRANSPARENT<<16+ROPF_DITHER;
      	    }
	    I64 k=0;
      	    for (k=0;k<MULTI_SPRITE_NUM;k++) {
	      if (Ground[i].appear_timer[k]>0) {
       	        GrLine(dc,Ground[i].msprite_x1[k],Ground[i].msprite_y1[k],Ground[i].msprite_x2[k],Ground[i].msprite_y2[k]);
	      }
	    }
	  }//
      	  if (i<GROUND_NUM) {
            Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color]/*+WHITE<<16+ROPF_DITHER*/,dc);
          } else {
            if (Ground[i].health<=player_web_health/2) {
              dc->color=CYAN+TRANSPARENT<<16+ROPF_DITHER;
            } else {
              dc->color=CYAN;
            }
      	  }
          GrLine(dc,Ground[i].sprite_x1,Ground[i].sprite_y1,
		    Ground[i].sprite_x2,Ground[i].sprite_y2);
          if (i>=GROUND_NUM) {//Web
            if (Ground[i].x1<=player_x<=Ground[i].x2) { //pos of hearts
	      DrawHeart(task,dc,
			(Ground[i].x1+Ground[i].x2)/2,
			(Ground[i].y1+Ground[i].y2)/2,
			Ground[i].health,
			1);
	    } else if (player_x<Ground[i].x1) {
	      DrawHeart(task,dc,
			Ground[i].x1,
			Ground[i].y1,
			Ground[i].health,
			1);
            } else {
	      DrawHeart(task,dc,
			Ground[i].x2,
			Ground[i].y2,
			Ground[i].health,
			1);
            }
          }
        } else { //pov view
   	  F64 g_x1=0,g_y1=0,g_x2=0,g_y2=0,
      	    é1=0, 
      	    é2=0,
      	    ground_é_x1=0,
      	    ground_é_y1=0,
      	    ground_é_x2=0,
      	    ground_é_y2=0;
        //angles
          é1=GetRotateAngle(Ground[i].x1,Ground[i].y1);
          é2=GetRotateAngle(Ground[i].x2,Ground[i].y2);
        //Draw ground's ground
          if (print_current_above) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(é2-Ground[on_ground_id].angle);
          } else if (print_current_below) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(pi+é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(pi+é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(pi+é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(pi+é2-Ground[on_ground_id].angle);
          } 
	  g_x1=GR_WIDTH/2+ground_é_x1-player_é_x;
	  g_y1=GR_HEIGHT/2+ground_é_y1-player_é_y;
	  g_x2=GR_WIDTH/2+ground_é_x2-player_é_x;
	  g_y2=GR_HEIGHT/2+ground_é_y2-player_é_y;
      	  if (i<GROUND_NUM) {
            Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color]/*+WHITE<<16+ROPF_DITHER*/,dc);
          } else {
            if (Ground[i].health<=player_web_health/2) {
              dc->color=CYAN+TRANSPARENT<<16+ROPF_DITHER;
            } else {
              dc->color=CYAN;
            }
      	  }
          GrLine(dc,g_x1,g_y1,g_x2,g_y2);
          if (i>=GROUND_NUM) {
	    if (i==on_ground_id) { //standing on top of ground
	      DrawHeart(task,dc,player_x,
			player_y+24,
			Ground[i].health,
			1);
            } else { //put to center
	      DrawHeart(task,dc,GR_WIDTH/2-cam_x+cam_move_x+(ground_é_x1+ground_é_x2)/2-player_é_x,
			      GR_HEIGHT/2-cam_y+cam_move_y+(ground_é_y1+ground_é_y2)/2-player_é_y,
			      Ground[i].health,
			      1);
	    }
          }
        }
      }
    }
  }
}

U0 DrawGroundFloodfill(CTask *,CDC *dc)
{
  I64 i=0,j=0;
  for (j=0;j<rendered_ground_num;j++) {
    i=render_grounds[j];
    if (Ground[i].type==1) { 
      Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color]+WHITE<<16+ROPF_DITHER,dc);
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
        if (IsNormalView) {//normal view
	  GrFloodFill(dc,Ground[i].sprite_x1,Ground[i].sprite_y1);
	  GrFloodFill(dc,Ground[i].sprite_x2,Ground[i].sprite_y2);
        } else { //pov view
  	  F64 g_x1=0,g_y1=0,g_x2=0,g_y2=0,
            é1=0, 
      	    é2=0,
      	    ground_é_x1=0,
      	    ground_é_y1=0,
      	    ground_é_x2=0,
      	    ground_é_y2=0;
     //angles
          é1=GetRotateAngle(Ground[i].x1,Ground[i].y1);
          é2=GetRotateAngle(Ground[i].x2,Ground[i].y2);
     //Draw ground's ground
          if (print_current_above) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(é2-Ground[on_ground_id].angle);
          } else if (print_current_below) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(pi+é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(pi+é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(pi+é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(pi+é2-Ground[on_ground_id].angle);
          } 
          g_x1=GR_WIDTH/2+ground_é_x1-player_é_x;
          g_y1=GR_HEIGHT/2+ground_é_y1-player_é_y;
          g_x2=GR_WIDTH/2+ground_é_x2-player_é_x;
          g_y2=GR_HEIGHT/2+ground_é_y2-player_é_y;
          GrFloodFill(dc,g_x1,g_y1);
          GrFloodFill(dc,g_x2,g_y2);
        }
      }
    }
  }
}

U0 DrawGroundText(CTask *,CDC *dc)
{
  I64 i=0,j=0;
  for (j=0;j<rendered_ground_num;j++) {
    i=render_grounds[j];
    if (Ground[i].type==2) { 
      Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color],dc);
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
        if (!change_view || (on_ground_timer<9 && (print_current_above || (!print_current_above && !print_current_below))) ||
			  (on_ground_timer<1 && print_current_below)
	 ) {//normal view
	  GrPrint(dc,Ground[i].sprite_x1,Ground[i].sprite_y1,Ground[i].text);
        } else { //pov view
  	  F64 g_x1=0,g_y1=0,g_x2=0,g_y2=0,
      	    é1=0, 
      	    é2=0,
      	    ground_é_x1=0,
      	    ground_é_y1=0,
      	    ground_é_x2=0,
      	    ground_é_y2=0;
     //angles
          é1=GetRotateAngle(Ground[i].x1,Ground[i].y1);
          é2=GetRotateAngle(Ground[i].x2,Ground[i].y2);
     //Draw ground's ground
          if (print_current_above) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(é2-Ground[on_ground_id].angle);
          } else if (print_current_below) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(pi+é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(pi+é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(pi+é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(pi+é2-Ground[on_ground_id].angle);
          } 
          g_x1=GR_WIDTH/2+ground_é_x1-player_é_x;
          g_y1=GR_HEIGHT/2+ground_é_y1-player_é_y;
          g_x2=GR_WIDTH/2+ground_é_x2-player_é_x;
          g_y2=GR_HEIGHT/2+ground_é_y2-player_é_y;
	  GrPrint(dc,g_x1,g_y1,Ground[i].text);
	}
      }
    }
  }
}

U0 DrawGroundTriFill(CTask *task,CDC *dc)
{
  I64 i=0,j=0,c=0;
  for (j=0;j<rendered_ground_num;j++) {
    i=render_grounds[j];
    if (Ground[i].type==3) { 
      if (!IsInvertedBackground) {
	c=Ground[i].color;
      } else {
	c=palette_dark_arr[Ground[i].color]+WHITE<<16+ROPF_DITHER;
      }
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
        if (IsNormalView) {//normal view
	  DrawTriFill(task,dc,c,Ground[i].sprite_x1,
				Ground[i].sprite_y1,
				Ground[i].sprite_x2,
				Ground[i].sprite_y2,
				Ground[i].sprite_x3,
				Ground[i].sprite_y3);
        } else { //pov view
  	  F64 g_x1=0,g_y1=0,g_x2=0,g_y2=0,g_x3=0,g_y3=0,
     	    é1=0, 
     	    é2=0,
     	    é3=0,
     	    ground_é_x1=0,
      	    ground_é_y1=0,
      	    ground_é_x2=0,
      	    ground_é_y2=0,
      	    ground_é_x3=0,
      	    ground_é_y3=0;
     //angles
          é1=GetRotateAngle(Ground[i].x1,Ground[i].y1);
          é2=GetRotateAngle(Ground[i].x2,Ground[i].y2);
          é3=GetRotateAngle(Ground[i].x3,Ground[i].y3);
     //Draw ground's ground
          if (print_current_above) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(é2-Ground[on_ground_id].angle);
            ground_é_x3=GetDistance(Ground[i].x3,Ground[i].y3,0,0)*Cos(é3-Ground[on_ground_id].angle);
            ground_é_y3=GetDistance(Ground[i].x3,Ground[i].y3,0,0)*Sin(é3-Ground[on_ground_id].angle);
          } else if (print_current_below) {
            ground_é_x1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Cos(pi+é1-Ground[on_ground_id].angle);
            ground_é_y1=GetDistance(Ground[i].x1,Ground[i].y1,0,0)*Sin(pi+é1-Ground[on_ground_id].angle);
            ground_é_x2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Cos(pi+é2-Ground[on_ground_id].angle);
            ground_é_y2=GetDistance(Ground[i].x2,Ground[i].y2,0,0)*Sin(pi+é2-Ground[on_ground_id].angle);
            ground_é_x3=GetDistance(Ground[i].x3,Ground[i].y3,0,0)*Cos(pi+é3-Ground[on_ground_id].angle);
            ground_é_y3=GetDistance(Ground[i].x3,Ground[i].y3,0,0)*Sin(pi+é3-Ground[on_ground_id].angle);
          } 
          g_x1=GR_WIDTH/2+ground_é_x1-player_é_x;
          g_y1=GR_HEIGHT/2+ground_é_y1-player_é_y;
          g_x2=GR_WIDTH/2+ground_é_x2-player_é_x;
          g_y2=GR_HEIGHT/2+ground_é_y2-player_é_y;
          g_x3=GR_WIDTH/2+ground_é_x3-player_é_x;
          g_y3=GR_HEIGHT/2+ground_é_y3-player_é_y;
	  DrawTriFill(task,dc,c,g_x1,g_y1,g_x2,g_y2,g_x3,g_y3);
	}
      }
    }
  }
