


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
  Ground[0].x1=200;
  Ground[0].y1=100;
  Ground[0].x2=400;
  Ground[0].y2=150;

  Ground[1].x1=100;
  Ground[1].y1=100;
  Ground[1].x2=700;
  Ground[1].y2=750;



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
      /*Ground[i].x1=saved_ground_x1[i];
      Ground[i].y1=saved_ground_y1[i];
      Ground[i].x2=saved_ground_x2[i];
      Ground[i].y2=saved_ground_y2[i];
      Ground[i].x3=saved_ground_x3[i];
      Ground[i].y3=saved_ground_y3[i];*/
 
      //Ground[i].type=saved_ground_type[i];
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
      /*if (Ground[i].x3==Ground[i].x1 || Ground[i].x3==Ground[i].x2) {
	    Ground[i].x3++;
      }*/


/*      if (Ground[i].type==3) {//trifill
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
    }*/
    //Ground[i].health=-1;
    //rendered_ground_num=0;
      Ground[i].height_from_player_x=0;
    //Ground[i].within_render_distance=FALSE;
      Ground[i].angle=0;
      SetGround(i);
    }
  }
}


int GetGridId(int x,int y,int width, int size,int max)
{
  int id=-1,row=0,column=0;
  row=y/size;
  column=x/size;
  id=row*(width/size)+column;
  if (-1<id && id<max && 0<x && x<width) {
    return id;
  }
  return -1;
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


void GroundAct(int pid,int g) {
  //gravity
  //collision
  /*if ((Ground[pid].y1-2<=player.y && player.y<=Ground[pid].y2) &&
       ((Ground[pid].x1-16<=player.x && player.x<=Ground[pid].x2+16) ||
        (Ground[pid].x1-16<=player.x && player.x<=Ground[pid].x2+16)
      )) {
    player.grav=1;
    player.jump_height=64;
    player.y-=2;
//    player.in_air=false;
  }*/
}


void DrawGrounds(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  for (int i=0;i<GROUND_NUM;i++) {
    GrLine(hwnd,hdc,ps,Ground[i].x1,Ground[i].y1,Ground[i].x2,Ground[i].y2,RGB(255,255,255));
  }
}






