

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

void SetGround(int i)
{
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


void InitGround()
{
  int i,j;
//Set ground default and Web
  for (i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {
    //printf("-Ground%d/%d\n",i,GROUND_NUM);
    for (j=0;j<MAX_VGRID_NUM;j++) {
      Ground[i].already_in_grid[j]=FALSE;
      Ground[i].saved_pos_in_grid[j]=-1;
    }
    
    //printf("%d saving ground\n",i);
    if (i<GROUND_NUM) {
    //set the saved data
      Ground[i].x1=(double)saved_ground_x1[i];
      //printf("%d %1.0f=%1.0f\n",i,Ground[i].x1,saved_ground_x1[i]);
      Ground[i].y1=(double)saved_ground_y1[i];
      Ground[i].x2=(double)saved_ground_x2[i];
      Ground[i].y2=(double)saved_ground_y2[i];
      Ground[i].x3=(double)saved_ground_x3[i];
      Ground[i].y3=(double)saved_ground_y3[i];
 
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
      //for (j=0;j<256;j++) {
        //Ground[i].text[j]=saved_ground_text[i][j];
      //}

      //char* b=saved_ground_text[i];
      //printf("%s\n",b);
      strncpy(Ground[i].text,saved_ground_text[i],512);
      //printf("\n%d ghost?%d",i,saved_ground_is_ghost[i]);
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
      Ground[i].color=CYAN;//TRANSPARENT;
      Ground[i].type=0;
      /*for (j=0;j<256;j++) {
        Ground[i].text[j]=0;
      }*/
      Ground[i].text[0]='\0';
      Ground[i].is_ghost=FALSE;
    }
    Ground[i].health=-1;
    //player.rendered_ground_num=0;
    Ground[i].height_from_player_x=0;
    Ground[i].within_render_distance=FALSE;
    Ground[i].angle=0;
    SetGround(i);
    //printf("--%d\n",saved_ground_is_ghost[i]);
  }
}

/*void InitGround2()
{
  int i=0;
  for (i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {
    Ground[i].sprite_x1=-20;//Ground[i].x1+player.cam_x;//+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y1=-20;//Ground[i].y1+player.cam_y;//+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x2=-20;//Ground[i].x2+player.cam_x;//+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y2=-20;//Ground[i].y2+player.cam_y;//+cam_move_y-PLAYER_HEIGHT/2;
    Ground[i].sprite_x3=-20;//Ground[i].x3+player.cam_x;//+cam_move_x-PLAYER_WIDTH/2;
    Ground[i].sprite_y3=-20;//Ground[i].y3+player.cam_y;//+cam_move_y-PLAYER_HEIGHT/2;
  }
}*/


int GetOnGroundId(double x,double y,double min_range_1,double min_range_2,bool is_player)
{
  int i=0,j=-1,ground_id=0,on_grid_id=0;
  double ground_entity_E=0,height_from_ground=0;
  if (0<x && x<MAP_WIDTH && 0<y && y<MAP_HEIGHT) { //within bounderies
    on_grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);//maths to get grid
    for (i=0;i<VGrid[on_grid_id].max_ground_num;i++) {
      ground_id=VGrid[on_grid_id].ground_ids[i];
      ground_entity_E=GetLineTargetAngle(ground_id,x,y);
      height_from_ground=GetLineTargetHeight(ground_id,ground_entity_E,x,y);
      if (is_player) {
        Ground[ground_id].height_from_player_x=height_from_ground;
      }
      if (Ground[ground_id].x1-min_range_1<=x && x<=Ground[ground_id].x2+min_range_1) {//within x
        if ((Ground[ground_id].y1-min_range_1<=y && y<=Ground[ground_id].y2+min_range_1) ||
            (Ground[ground_id].y2-min_range_1<=y && y<=Ground[ground_id].y1+min_range_1)) {//within y
          if (is_player && -min_range_2<Ground[ground_id].height_from_player_x && Ground[ground_id].height_from_player_x<min_range_2) { //change in ground
	        if (ground_id!=player.saved_ground_id && !Ground[ground_id].is_ghost) {
              j=ground_id;
	        }
          } else if (!Ground[ground_id].is_ghost && -min_range_2<height_from_ground && height_from_ground<min_range_2) {
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

/*void GroundAct() //Mainly for graphics
{
  int i=0,j=0;//,k=0;
  for (j=0;j<player.rendered_ground_num;j++) {
    i=player.render_grounds[j];    

    Ground[i].sprite_x1=Ground[i].x1+player.cam_x+player.cam_move_x;
    Ground[i].sprite_y1=Ground[i].y1+player.cam_y+player.cam_move_y;
    Ground[i].sprite_x2=Ground[i].x2+player.cam_x+player.cam_move_x;
    Ground[i].sprite_y2=Ground[i].y2+player.cam_y+player.cam_move_y;
    Ground[i].sprite_x3=Ground[i].x3+player.cam_x+player.cam_move_x;
    Ground[i].sprite_y3=Ground[i].y3+player.cam_y+player.cam_move_y;
}*/


void DestroyGround(int i)
{
  int lg_grid_id=0,node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (-1<Ground[i].gradient<1) {
    for (x=Ground[i].x1;x<Ground[i].x2;x++) {
      lg_y=x*Ground[i].gradient+Ground[i].c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (!NodeGrid[node_grid_id].non_web) { //only unsolid web nodegrids
        NodeGrid[node_grid_id].node_solid=FALSE;
      }
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
      lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (!NodeGrid[node_grid_id].non_web) { //only unsolid web nodegrids
        NodeGrid[node_grid_id].node_solid=FALSE;
      }
    }
  }
  Ground[i].health=-1;
  Ground[i].x1=Ground[i].y1=Ground[i].x2=Ground[i].y2=-20;
}



void DrawWebs(HDC hdc)
{ int id;
  for (int i=GROUND_NUM;i<GROUND_NUM+player.max_web_num;i++) {
    id=i;
    if (id<GROUND_NUM+MAX_WEB_NUM && Ground[id].x1>-20) {
      GrLine(hdc,
        Ground[id].x1+player.cam_x+player.cam_move_x,
        Ground[id].y1+player.cam_y+player.cam_move_y,
        Ground[id].x2+player.cam_x+player.cam_move_x,
        Ground[id].y2+player.cam_y+player.cam_move_y,
        CYAN
      );
    }
  }
}


void DrawGround(HDC hdc)
{
  int i=0;//,j=0;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i].type==0) {
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          GrLine(hdc,
                Ground[i].x1,
                Ground[i].y1,
                Ground[i].x2,
                Ground[i].y2,
                Ground[i].color);

      }
    }
  }
}



void DrawGroundText(HDC hdc)
{
  int i=0;//,j=0;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i].type==2) { 
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	    GrPrint(hdc,
            Ground[i].x1,
            Ground[i].y1,
            Ground[i].text,
            Ground[i].color);
      }
    }
  }
}


void DrawGroundTriFill(HDC hdc)
{
  int i=0,c=0;//,j=0;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i].type==3) { 
	  c=Ground[i].color;
      if (!IsOutOfBounds(Ground[i].x1,Ground[i].y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i].x2,Ground[i].y2,1,MAP_WIDTH,MAP_HEIGHT)) {
        if (!IsInvertedBackground()) {
	      DrawTriFill(hdc,c,
                Ground[i].x1,
				Ground[i].y1,
				Ground[i].x2,
				Ground[i].y2,
				Ground[i].x3,
				Ground[i].y3,FALSE,0);
        } else {
	      DrawTriFill(hdc,c,
                Ground[i].x1,
				Ground[i].y1,
				Ground[i].x2,
				Ground[i].y2,
				Ground[i].x3,
				Ground[i].y3,TRUE,HS_BDIAGONAL);
        }
      }
    }
  }
}



