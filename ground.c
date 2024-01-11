
bool IsInvertedBackground()
{
  if (map_background==1) {
    return TRUE;
  } else if (is_inverted && map_background==2) {
    return TRUE;
  }
  return FALSE;
}

//Ground
double GetLineTargetAngle(int Ground_id,double x,double y)
{

  double E=0,
      hypothenuse=0,
      adjacent=0;
  if (x>(Ground[Ground_id].x1+Ground[Ground_id].x2)/2) {//right of Ground
    hypothenuse=
      GetDistance(
	  Ground[Ground_id].x1,
	  Ground[Ground_id].y1,
	  x,
	  y
	);
    adjacent=x-Ground[Ground_id].x1;
    E=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  Ground[Ground_id].x1,
	  Ground[Ground_id].y1
      )<=0) {
      E=-E;
    }
  } else {//left of Ground
    hypothenuse=
      GetDistance(
	  Ground[Ground_id].x2,
	  Ground[Ground_id].y2,
	  x,
	  y
	);
    adjacent=Ground[Ground_id].x2-x;
    E=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  Ground[Ground_id].x2,
	  Ground[Ground_id].y2
        )>0) {
      E=-E;
    }
  }
  return E;
}

double GetLineTargetHeight(int Ground_id,double E,double x,double y)
{
  double h=0;
  if (x>(Ground[Ground_id].x1+Ground[Ground_id].x2)/2) {//right of Ground
    h=sin(Ground[Ground_id].angle-E)*
      GetDistance(
	Ground[Ground_id].x1,
	Ground[Ground_id].y1,
	x,
	y
    );
  } else {//left of Ground
    h=sin(2*M_PI-Ground[Ground_id].angle-E)*
      GetDistance(
	Ground[Ground_id].x2,
	Ground[Ground_id].y2,
	x,
	y
    );
  }
  return h;
}

void SetGround(int i) {
  //Set Ground's gradient
  Ground[i].gradient=GetGradient(
    Ground[i].x1,
    Ground[i].y1,
    Ground[i].x2,
    Ground[i].y2);
  //Set Ground's C
  Ground[i].c=GetGroundC(Ground[i].x1,Ground[i].y1,Ground[i].gradient);
  //Set Ground's angle
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


void InitGround() {
  int i,j;
//Set ground default and Web
  //current_gsm=0;
  //gsm_hold_timer=0;

  for (i=0;i<GROUND_NUM;i++) {
    for (j=0;j<GRID_NUM;j++) {
      Ground[i].saved_pos_in_grid[j]=-1;
      Ground[i].already_in_grid[j]=FALSE;
    }
    /*for (j=0;j<MULTI_SPRITE_NUM;j++) {
      Ground[i].msprite_x1[j]=
      Ground[i].msprite_y1[j]=
      Ground[i].msprite_x2[j]=
      Ground[i].msprite_y2[j]=-20;
      Ground[i].appear_timer[j]=0;
    }*/
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
      //if (!IsInvertedBackground()) {
      Ground[i].color=color_arr[saved_ground_color[i]];
      /*} else {
        Ground[i].color=palette_dark_arr[saved_ground_color[i]];
      }*/
      Ground[i].text=saved_ground_text[i];
      if (saved_ground_is_ghost[i]) {
        Ground[i].is_ghost=TRUE;
      } else {
        Ground[i].is_ghost=FALSE;
      }
    /*} else { 
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
    }*/
      Ground[i].health=-1;
      player.rendered_ground_num=0;
      Ground[i].height_from_player_x=0;
      Ground[i].within_render_distance=FALSE;
      Ground[i].angle=0;
      SetGround(i);
    }
  }
}

void InitGround2()
{
  int i=0;
  for (i=0;i<GROUND_NUM/*TOTAL_GROUNDS*/;i++) {
    Ground[i].sprite_x1=Ground[i].x1+player.cam_x;//+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y1=Ground[i].y1+player.cam_y;//+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x2=Ground[i].x2+player.cam_x;//+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y2=Ground[i].y2+player.cam_y;//+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x3=Ground[i].x3+player.cam_x;//+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y3=Ground[i].y3+player.cam_y;//+cam_move_y-PLAYER_HEIGHT/2;
  }
}


int GetOnGroundId(double x,double y,double min_range_1,double min_range_2,bool is_player)
{
  int i=0,j=-1,ground_id=0,on_grid_id=0;
  double ground_entity_E=0,height_from_ground=0;
  if (0<x && x<MAP_WIDTH && 0<y && y<MAP_HEIGHT) { //within bounderies
    on_grid_id=GetGridId(x,y,MAP_WIDTH,GRID_SIZE,GRID_NUM);//maths to get grid
    for (i=0;i<Grid[on_grid_id].max_ground_num;i++) {
      ground_id=Grid[on_grid_id].ground_ids[i];
      ground_entity_E=GetLineTargetAngle(ground_id,x,y);
      height_from_ground=GetLineTargetHeight(ground_id,ground_entity_E,x,y);
      if (is_player) {
        Ground[ground_id].height_from_player_x=height_from_ground;
      }
      if (Ground[ground_id].x1-min_range_1<=x && x<=Ground[ground_id].x2+min_range_1) {//within x
        if ((Ground[ground_id].y1-min_range_1<=y && y<=Ground[ground_id].y2+min_range_1) ||
            (Ground[ground_id].y2-min_range_1<=y && y<=Ground[ground_id].y1+min_range_1)) {//within y
          if (is_player && -min_range_2<Ground[ground_id].height_from_player_x && Ground[ground_id].height_from_player_x<min_range_2) { //change in ground
	        if (ground_id!=player.saved_ground_id /*&& !Ground[ground_id].is_ghost*/) {
              j=ground_id;
	        }
          } else if (/*!Ground[ground_id].is_ghost &&*/ -min_range_2<height_from_ground && height_from_ground<min_range_2) {
	        return ground_id;
	      }
	    }
      }
    }
    if (is_player) {
      if (j==-1) {
	    return player.saved_ground_id;
      } else {
	    return j;
      }
    } else {
      return -1;
    }
  }
  return -1;
}


void GroundAct() //Mainly for graphics
{
  int i=0,j=0;//,k=0;
  //gsm_hold_timer++;
  for (j=0;j<player.rendered_ground_num/*+MAX_WEB_NUM*/;j++) {
    /*if (j<rendered_ground_num) {
      i=render_grounds[j];
    } else {
      i=GROUND_NUM+(j-rendered_ground_num);
    }*/
    i=player.render_grounds[j];    
    /*Ground[i].sprite_x1=Ground[i].x1+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y1=Ground[i].y1+cam_y+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x2=Ground[i].x2+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y2=Ground[i].y2+cam_y+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x3=Ground[i].x3+cam_x+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y3=Ground[i].y3+cam_y+cam_move_y-PLAYER_HEIGHT/2;*/

    Ground[i].sprite_x1=Ground[i].x1+player.cam_x+player.cam_move_x;
    Ground[i].sprite_y1=Ground[i].y1+player.cam_y+player.cam_move_y;
    Ground[i].sprite_x2=Ground[i].x2+player.cam_x+player.cam_move_x;
    Ground[i].sprite_y2=Ground[i].y2+player.cam_y+player.cam_move_y;
    Ground[i].sprite_x3=Ground[i].x3+player.cam_x+player.cam_move_x;
    Ground[i].sprite_y3=Ground[i].y3+player.cam_y+player.cam_move_y;


//multisprite
    //if (fade_ground) {
      //k=0;
      /*if (gsm_hold_timer>1) {
        if (YesLongFade2 || player_in_air_cooldown>0) {
          Ground[i].appear_timer[current_gsm]=50;
        }
        Ground[i].msprite_x1[current_gsm]=Ground[i].sprite_x1;
        Ground[i].msprite_y1[current_gsm]=Ground[i].sprite_y1;
        Ground[i].msprite_x2[current_gsm]=Ground[i].sprite_x2;
        Ground[i].msprite_y2[current_gsm]=Ground[i].sprite_y2;
      }*/
      /*for (k=0;k<MULTI_SPRITE_NUM;k++) {
        if (Ground[i].appear_timer[k]>0 && !the_bravery_tyrant) {
          Ground[i].appear_timer[k]--;
        }    
      }*/
    //}
  } //
  /*if (fade_ground && gsm_hold_timer>1) {    
    gsm_hold_timer=0;
    current_gsm++;
    if (current_gsm>=MULTI_SPRITE_NUM-1) {
      current_gsm=0;
    }
  }*/
}


/*void DrawGrounds(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  int id;
  for (int i=0;i<player.rendered_ground_num;i++) {
    id = player.render_grounds[i];
    if (id!=-1) {
      GrLine(hwnd,hdc,ps,
        Ground[id].x1+player.cam_x,
        Ground[id].y1+player.cam_y,
        Ground[id].x2+player.cam_x,
        Ground[id].y2+player.cam_y,
        RGB(255,255,255)
      );     
      /*GrLine(hwnd,hdc,ps,
        Ground[id].x1,
        Ground[id].y1,
        Ground[id].x2,
        Ground[id].y2,
        RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255)));*/
    /*}
  }
}*/


void DrawGround(HWND hwnd, HDC hdc, PAINTSTRUCT ps)
{
  int i=0,j=0;
  for (j=0;j<player.rendered_ground_num/*+MAX_WEB_NUM*/;j++) {
    //if (j<player.rendered_ground_num) {
    i=player.render_grounds[j];
    //} else {
      //i=GROUND_NUM+(j-rendered_ground_num);
    //}
  //for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i].type==0) {
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      /*if (fade_ground) {
      	    if (i<GROUND_NUM) {
              Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color],dc);
            } else {
              dc->color=CYAN+TRANSPARENT<<16+ROPF_DITHER;
      	    }
	        /*int k=0;
      	    for (k=0;k<MULTI_SPRITE_NUM;k++) {
	          if (Ground[i].appear_timer[k]>0) {
       	        GrLine(dc,Ground[i].msprite_x1[k],Ground[i].msprite_y1[k],Ground[i].msprite_x2[k],Ground[i].msprite_y2[k]);
	          }
	        }*/
	      //}//
      	  /*if (i<GROUND_NUM) {
            Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color]/*+WHITE<<16+ROPF_DITHER,dc);
          } else {
            if (Ground[i].health<=player_web_health/2) {
              dc->color=CYAN+TRANSPARENT<<16+ROPF_DITHER;
            } else {
              dc->color=CYAN;
            }
      	  }*/
          GrLine(hwnd,hdc,ps,
                Ground[i].sprite_x1,
                Ground[i].sprite_y1,
                Ground[i].sprite_x2,
                Ground[i].sprite_y2,
                Ground[i].color);
          /*if (i>=GROUND_NUM) {//Web
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
          }*/
      }
    }
  }
}



void DrawGroundText(HWND hwnd, HDC hdc, PAINTSTRUCT ps)
{
  int i=0,j=0;
  for (j=0;j<player.rendered_ground_num;j++) {
    i=player.render_grounds[j];
  //for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i].type==2) { 
      //Highlight(!IsInvertedBackground,Ground[i].color,palette_dark_arr[Ground[i].color],dc);
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	    GrPrint(hwnd,hdc,ps,
            Ground[i].sprite_x1,
            Ground[i].sprite_y1,
            Ground[i].text,
            Ground[i].color);
      }
    }
  }
}



void DrawGroundTriFill(HWND hwnd, HDC hdc, PAINTSTRUCT ps)
{
  int i=0,c=0,j=0;
  for (j=0;j<player.rendered_ground_num;j++) {
    i=player.render_grounds[j];
  //for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i].type==3) { 
      //if (!IsInvertedBackground()) {
	  c=Ground[i].color;
      /*} else {
	    c=palette_dark_arr[Ground[i].color];//+WHITE<<16+ROPF_DITHER;
      }*/
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	    DrawTriFill(hwnd,hdc,ps,c,
                Ground[i].sprite_x1,
				Ground[i].sprite_y1,
				Ground[i].sprite_x2,
				Ground[i].sprite_y2,
				Ground[i].sprite_x3,
				Ground[i].sprite_y3);
      }
    }
  }
}



