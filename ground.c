

//Ground
double GetLineTargetAngle(int Ground_id,double x,double y)
{

  double E=0,
      hypothenuse=0,
      adjacent=0;
  if (x>(Ground[Ground_id]->x1+Ground[Ground_id]->x2)/2) {//right of Ground
    hypothenuse=
      GetDistance(
	  Ground[Ground_id]->x1,
	  Ground[Ground_id]->y1,
	  x,
	  y
	);
    adjacent=x-Ground[Ground_id]->x1;
    E=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  Ground[Ground_id]->x1,
	  Ground[Ground_id]->y1
      )<=0) {
      E=-E;
    }
  } else {//left of Ground
    hypothenuse=
      GetDistance(
	  Ground[Ground_id]->x2,
	  Ground[Ground_id]->y2,
	  x,
	  y
	);
    adjacent=Ground[Ground_id]->x2-x;
    E=GetCosAngle(adjacent,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  Ground[Ground_id]->x2,
	  Ground[Ground_id]->y2
        )>0) {
      E=-E;
    }
  }
  return E;
}

double GetLineTargetHeight(int Ground_id,double E,double x,double y)
{
  double h=0;
  if (x>(Ground[Ground_id]->x1+Ground[Ground_id]->x2)/2) {//right of Ground
    h=sin(Ground[Ground_id]->angle-E)*
      GetDistance(
	Ground[Ground_id]->x1,
	Ground[Ground_id]->y1,
	x,
	y
    );
  } else {//left of Ground
    h=sin(2*M_PI-Ground[Ground_id]->angle-E)*
      GetDistance(
	Ground[Ground_id]->x2,
	Ground[Ground_id]->y2,
	x,
	y
    );
  }
  return h;
}


/*
double GetLineTargetAngleII(int Ground_id,double x,double y)
{

  double E=0,
      hypothenuse=0,
      opposite=0,x1=0,y1=0,x2=0,y2=0;
  if (Ground[Ground_id]->y1<Ground[Ground_id]->y2) {
    x1=Ground[Ground_id]->x1; //1 is the upper
    y1=Ground[Ground_id]->y1;
    x2=Ground[Ground_id]->x2;
    y2=Ground[Ground_id]->y2;
  } else {
    x2=Ground[Ground_id]->x1;
    y2=Ground[Ground_id]->y1;
    x1=Ground[Ground_id]->x2;
    y1=Ground[Ground_id]->y2;
  }
  if (y>(Ground[Ground_id]->y1+Ground[Ground_id]->y2)/2) {//down of ground
    hypothenuse=
      GetDistance(
	  x1,
	  y1,
	  x,
	  y
	);
    opposite=y-y1;
    E=GetSinAngle(opposite,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  x1,
	  y1
      )<=0) {
      E=-E;
    }
  } else {//up of Ground
    hypothenuse=
      GetDistance(
	  x2,
	  y2,
	  x,
	  y
	);
    opposite=y2-y;
    E=GetSinAngle(opposite,hypothenuse);
    if (GetGradient(
	  x,
	  y,
	  x2,
	  y2
        )>0) {
      E=-E;
    }
  }
  return E;
}*/



double GetLineTargetHeightII(int Ground_id,double E,double x,double y)
{
  double h=0,x1=0,y1=0,x2=0,y2=0;
  if (Ground[Ground_id]->y1<Ground[Ground_id]->y2) {
    x1=Ground[Ground_id]->x1; //1 is the upper
    y1=Ground[Ground_id]->y1;
    x2=Ground[Ground_id]->x2;
    y2=Ground[Ground_id]->y2;
  } else {
    x2=Ground[Ground_id]->x1;
    y2=Ground[Ground_id]->y1;
    x1=Ground[Ground_id]->x2;
    y1=Ground[Ground_id]->y2;
  }
  if (y>(Ground[Ground_id]->y1+Ground[Ground_id]->y2)/2) {//down of ground
    h=sin(Ground[Ground_id]->angle-E)*
      GetDistance(
	x1,
	y1,
	x,
	y
    );
  } else {//up of Ground
    h=sin(2*M_PI-Ground[Ground_id]->angle-E)*
      GetDistance(
    x2,
	y2,
	x,
	y
    );
  }
  return h;
}

void SetGround(int i)
{
  double tmp=Ground[i]->x2;
  if (Ground[i]->x1>=Ground[i]->x2) {
    Ground[i]->x2=Ground[i]->x1+0.0001;
    Ground[i]->x1=tmp;
  }
  //Set Ground's gradient
  Ground[i]->gradient=GetGradient(
    Ground[i]->x1,
    Ground[i]->y1,
    Ground[i]->x2,
    Ground[i]->y2);
  //Set Ground's C
  Ground[i]->c=GetGroundC(Ground[i]->x1,Ground[i]->y1,Ground[i]->gradient);
  //Set Ground's angle
  Ground[i]->angle=GetCosAngle(Ground[i]->x2-Ground[i]->x1,
                                GetDistance(Ground[i]->x1,
                                            Ground[i]->y1,
                                            Ground[i]->x2,
                                            Ground[i]->y2)
                    );//cos(angle) = adjacent/hypothenuse
  if (Ground[i]->gradient<=0) {
    Ground[i]->angle=-Ground[i]->angle;
  }
}


void InitGround(bool is_max)
{
  int i,j;
  int max=GROUND_NUM;
  if (is_max)
    max=GROUND_NUM+MAX_WEB_NUM;
//Set ground default and Web
  for (i=0;i<max;i++) {
    //printf("-Ground%d/%d\n",i,GROUND_NUM);
    for (j=0;j<VGRID_NUM;j++) {
      Ground[i]->already_in_grid[j]=FALSE;
      Ground[i]->saved_pos_in_grid[j]=-1;
    }
    
    //printf("%d saving ground\n",i);
    if (i<GROUND_NUM) {
    //set the saved data
      Ground[i]->x1=(double)saved_ground_x1[i];
      //printf("%d %1.0f=%1.0f\n",i,Ground[i]->x1,saved_ground_x1[i]);
      Ground[i]->y1=(double)saved_ground_y1[i];
      Ground[i]->x2=(double)saved_ground_x2[i];
      Ground[i]->y2=(double)saved_ground_y2[i];
      Ground[i]->x3=(double)saved_ground_x3[i];
      Ground[i]->y3=(double)saved_ground_y3[i];
 
      Ground[i]->type=saved_ground_type[i];
      if (Ground[i]->x1<1) {
        Ground[i]->x1++;
      }
      if (Ground[i]->y1<1) {
        Ground[i]->y1++;
      }
      if (Ground[i]->y2<1) {
        Ground[i]->y2++;
      }
      if (Ground[i]->x2<=Ground[i]->x1) {
        Ground[i]->x2=Ground[i]->x1+1;
      }
      if (Ground[i]->x3==Ground[i]->x1 || Ground[i]->x3==Ground[i]->x2) {
	    Ground[i]->x3++;
      }


      if (Ground[i]->type==3 || Ground[i]->type==1) {//trifill
	    if (Ground[i]->y1==Ground[i]->y2) {
	      Ground[i]->y2++;
        }
        if (Ground[i]->type==1) {
          has_water=TRUE;
        }
      }
      if (Ground[i]->y3==Ground[i]->y1) {
	    Ground[i]->y3+=2;
      }
      if (Ground[i]->y3==Ground[i]->y2) {
	    Ground[i]->y3+=2;
      }
      Ground[i]->color_id=saved_ground_color[i];
      Ground[i]->color=rgbPaint[saved_ground_color[i]];//color_arr[saved_ground_color[i]];
      wcsncpy(Ground[i]->text,saved_ground_text[i],512);
      Ground[i]->font_size=saved_ground_text_size[i];
      //printf("\n%d ghost?%d",i,saved_ground_is_ghost[i]);
      if (saved_ground_is_ghost[i]) {
        Ground[i]->is_ghost=TRUE;
      } else {
        Ground[i]->is_ghost=FALSE;
      }
    } else { 
      Ground[i]->x1=-25;
      Ground[i]->y1=2;
      Ground[i]->x2=-20;
      Ground[i]->y2=2;
      Ground[i]->x3=-15;
      Ground[i]->y3=2;
      Ground[i]->color=CYAN;//TRANSPARENT;
      Ground[i]->type=0;
      /*for (j=0;j<256;j++) {
        Ground[i]->text[j]=0;
      }*/
      Ground[i]->text[0]='\0';
      Ground[i]->font_size=0;
      Ground[i]->is_ghost=FALSE;
    }
    Ground[i]->health=-1;
    //player.rendered_ground_num=0;
    //Ground[i]->height_from_player_x=0;
    Ground[i]->within_render_distance=FALSE;
    Ground[i]->angle=0;
    SetGround(i);
    //printf("--%d\n",saved_ground_is_ghost[i]);
  }
}



/*
=== +++ ===
=== +++ ===
+++ === +++
+++ === +++
=== +++ ===
=== +++ ===
*/

double x_quad[12]=
{
//top left corner
0,
-VGRID_SIZE,
0,

//top right corner
0,
VGRID_SIZE,
0,

//lower left corner
0,
-VGRID_SIZE,
0,

//lower right corner
0,
VGRID_SIZE,
0
}
;


double y_quad[12]=
{
//Top left corner
-VGRID_SIZE,
0,
VGRID_SIZE,


//Top right corner
-VGRID_SIZE,
0,
VGRID_SIZE,


//Lower left corner
VGRID_SIZE,
0,
-VGRID_SIZE,


//Lower right corner
VGRID_SIZE,
0,
-VGRID_SIZE
};



//!!NO CLIPPING PROTECTION BUT BLAZING FAST
int GetOnGroundId(double x,double y,double min_range_1,double min_range_2)
{
  int i=0,Ground_id=0,on_grid_id=0;
  double ground_entity_E=0,height_from_ground=0;
  if (0<x && x<MAP_WIDTH && 0<y && y<MAP_HEIGHT) { //within bounderies
    on_grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);//maths to get grid
    for (i=0;i<VGrid[on_grid_id]->max_ground_num;i++) {
      Ground_id=VGrid[on_grid_id]->ground_ids[i];
      if (Ground[Ground_id]->x1-min_range_1<=x && x<=Ground[Ground_id]->x2+min_range_1) {//within x
        if ((Ground[Ground_id]->y1-min_range_1<=y && y<=Ground[Ground_id]->y2+min_range_1) ||
            (Ground[Ground_id]->y2-min_range_1<=y && y<=Ground[Ground_id]->y1+min_range_1)) {//within y
          ground_entity_E=GetLineTargetAngle(Ground_id,x,y);
          height_from_ground=GetLineTargetHeight(Ground_id,ground_entity_E,x,y);
          if (!Ground[Ground_id]->is_ghost && Ground[Ground_id]->type!=1 && -min_range_2<height_from_ground && height_from_ground<min_range_2) {
            return Ground_id;
          }
        }
      }
    }
  } //end of if
  return -1;
}



int GetOnGroundIdPlayer(double x,double y,double min_range_1,double min_range_2)
{
  int i=0,j=-1,Ground_id=0,on_grid_id=0;
  double ground_entity_E=0,height_from_ground=0,_x=x,_y=y;


  int main_grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
  if (main_grid_id!=-1) {
  if (VGrid[main_grid_id]->max_ground_num>0) {
  double main_grid_x=(VGrid[main_grid_id]->x1+VGrid[main_grid_id]->x2)/2;
  double main_grid_y=(VGrid[main_grid_id]->y1+VGrid[main_grid_id]->y2)/2;
  int in_quadrant=0; //0=topleft, 1=topright, 2=lowerleft, 3=lowerright
  int index_quadrant=0;
  if (y<main_grid_y) {
    if (x>=main_grid_x) {//Top right corner of grid
      in_quadrant=1;
    }
  } else {
    if (x<main_grid_x) {//Lower left corner of grid
      in_quadrant=2;
    } else {  //Lower right corner of grid
      in_quadrant=3;
    }
  }

  for (int k=0;k<4;k++) {
    if (k>0) {
      _x+=x_quad[index_quadrant]; //1:0; 2:1; 3:2
      _y+=y_quad[index_quadrant];
      index_quadrant++;
    } else {
      _x=x;
      _y=y;
      switch (in_quadrant) {
        case 0: //top left corner
          index_quadrant=0;
          break;
        case 1: //top right corner
          index_quadrant=3;
          break;
        case 2: //lower left corner
          index_quadrant=6;
          break;
        case 3: //lower right corner
          index_quadrant=9;
          break;
      }
    }

    if (0<_x && _x<MAP_WIDTH && 0<_y && _y<MAP_HEIGHT) { //within bounderies
      on_grid_id=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);//maths to get grid
      if (on_grid_id!=-1) {
      for (i=0;i<VGrid[on_grid_id]->max_ground_num;i++) {
        Ground_id=VGrid[on_grid_id]->ground_ids[i];
        if (Ground_id!=-1) {
        if (Ground[Ground_id]->x1-min_range_1<=x && x<=Ground[Ground_id]->x2+min_range_1) {//within x
          if ((Ground[Ground_id]->y1-min_range_1<=y && y<=Ground[Ground_id]->y2+min_range_1) ||
              (Ground[Ground_id]->y2-min_range_1<=y && y<=Ground[Ground_id]->y1+min_range_1)) {//within y
            ground_entity_E=GetLineTargetAngle(Ground_id,x,y);
            height_from_ground=GetLineTargetHeight(Ground_id,ground_entity_E,x,y);

            if (-min_range_2<height_from_ground && height_from_ground<min_range_2) { //change in ground
              if (Ground_id!=player.saved_ground_id && !Ground[Ground_id]->is_ghost && Ground[Ground_id]->type!=1) {
                j=Ground_id;
                if (j!=-1) {
                  return j;
                }
                break;
              }
            }
          }
        }
        }
      }
    } //end of if
    }
  } //end of for
  }
  if (j==-1) {
    return player.saved_ground_id;
  }
  }
  return j;
}



/*int GetOnGroundIdE(double x,double y,double min_range_1,double min_range_2,int enemy_id)
{
  int i=0,j=-1,Ground_id=0,on_grid_id=0;
  double ground_entity_E=0,height_from_ground=0,_x=x,_y=y;


  int main_grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);

  if (main_grid_id!=-1) {
  if (VGrid[main_grid_id]->max_ground_num>0) {
  double main_grid_x=(VGrid[main_grid_id]->x1+VGrid[main_grid_id]->x2)/2;
  double main_grid_y=(VGrid[main_grid_id]->y1+VGrid[main_grid_id]->y2)/2;
  int in_quadrant=0; //0=topleft, 1=topright, 2=lowerleft, 3=lowerright
  int index_quadrant=0;
  if (y<main_grid_y) {
    if (x>=main_grid_x) {//Top right corner of grid
      in_quadrant=1;
    }
  } else {
    if (x<main_grid_x) {//Lower left corner of grid
      in_quadrant=2;
    } else {  //Lower right corner of grid
      in_quadrant=3;
    }
  }

  for (int k=0;k<4;k++) {
    if (k>0) {
      _x+=x_quad[index_quadrant];
      _y+=y_quad[index_quadrant];
      index_quadrant++;
    } else {
      _x=x;
      _y=y;
      switch (in_quadrant) {
        case 0: //top left corner
          index_quadrant=0;
          break;
        case 1: //top right corner
          index_quadrant=3;
          break;
        case 2: //lower left corner
          index_quadrant=6;
          break;
        case 3: //lower right corner
          index_quadrant=9;
          break;
      }
    }

    if (0<_x && _x<MAP_WIDTH && 0<_y && _y<MAP_HEIGHT) { //within bounderies
      on_grid_id=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);//maths to get grid
      if (on_grid_id!=-1) {
      for (i=0;i<VGrid[on_grid_id]->max_ground_num;i++) {
        Ground_id=VGrid[on_grid_id]->ground_ids[i];
        if (Ground_id!=-1) {
        if (Ground[Ground_id]->x1-min_range_1<=x && x<=Ground[Ground_id]->x2+min_range_1) {//within x
          if ((Ground[Ground_id]->y1-min_range_1<=y && y<=Ground[Ground_id]->y2+min_range_1) ||
              (Ground[Ground_id]->y2-min_range_1<=y && y<=Ground[Ground_id]->y1+min_range_1)) {//within y
            ground_entity_E=GetLineTargetAngle(Ground_id,x,y);
            height_from_ground=GetLineTargetHeight(Ground_id,ground_entity_E,x,y);
            if (-min_range_2<height_from_ground && height_from_ground<min_range_2) { //change in ground
	          if (Ground_id!=Enemy[enemy_id]->saved_ground_id && !Ground[Ground_id]->is_ghost && Ground[Ground_id]->type!=1) {
                j=Ground_id;
                if (j!=-1) {
                  return j;
                }
                break;
              }
            }
          }
        }
        }
      }
    } //end of if
    }
  } //end of for
  }
  if (j==-1) {
    return Enemy[enemy_id]->saved_ground_id;
  }
  }
  return j;
}*/


int GetOnGroundIdE(double x,double y,double min_range_1,double min_range_2,int enemy_id)
{
  //checks 1 grid only for efficiency but theres a risk of clipping
  int i=0,j=-1,Ground_id=0;
  double ground_entity_E=0,height_from_ground=0,_x=x,_y=y;

  int on_grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
  if (on_grid_id!=-1) {
  if (VGrid[on_grid_id]->max_ground_num>0) {
    if (0<_x && _x<MAP_WIDTH && 0<_y && _y<MAP_HEIGHT) { //within bounderies
      if (on_grid_id!=-1) {
      for (i=0;i<VGrid[on_grid_id]->max_ground_num;i++) {
        Ground_id=VGrid[on_grid_id]->ground_ids[i];
        if (Ground_id!=-1) {
        if (Ground[Ground_id]->x1-min_range_1<=x && x<=Ground[Ground_id]->x2+min_range_1) {//within x
          if ((Ground[Ground_id]->y1-min_range_1<=y && y<=Ground[Ground_id]->y2+min_range_1) ||
              (Ground[Ground_id]->y2-min_range_1<=y && y<=Ground[Ground_id]->y1+min_range_1)) {//within y
            ground_entity_E=GetLineTargetAngle(Ground_id,x,y);
            height_from_ground=GetLineTargetHeight(Ground_id,ground_entity_E,x,y);
            if (-min_range_2<height_from_ground && height_from_ground<min_range_2) { //change in ground
	          if (Ground_id!=Enemy[enemy_id]->saved_ground_id && !Ground[Ground_id]->is_ghost && Ground[Ground_id]->type!=1) {
                j=Ground_id;
                if (j!=-1) {
                  return j;
                }
                break;
              }
            }
          }
        }
        }
      }
      } //end of if
    }
  }
  }
  if (j==-1) {
    return Enemy[enemy_id]->saved_ground_id;
  }
  return j;
}



void DestroyGround(int i)
{
  int lg_grid_id=0,node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (-1<Ground[i]->gradient && Ground[i]->gradient<1) {
    for (x=Ground[i]->x1;x<Ground[i]->x2;x++) {
      lg_y=x*Ground[i]->gradient+Ground[i]->c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (!NodeGrid[node_grid_id]->non_web) { //only unsolid web nodegrids
        NodeGrid[node_grid_id]->node_solid=FALSE;
      }

      for (int n=0;n<4;n++) {
        double llg_x,llg_y;
        switch (n) {
          case 0:llg_x=x-NODE_SIZE;llg_y=lg_y-NODE_SIZE;break;
          case 1:llg_x=x+NODE_SIZE;llg_y=lg_y-NODE_SIZE;break;
          case 2:llg_x=x-NODE_SIZE;llg_y=lg_y+NODE_SIZE;break;
          case 3:llg_x=x+NODE_SIZE;llg_y=lg_y+NODE_SIZE;break;
        }
          lg_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
          if (lg_grid_id!=-1) {
            UnSetGridLineArray(lg_grid_id,i);
          }
          if (!Ground[i]->is_ghost) { //Not a ghost
            node_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
            if (!NodeGrid[node_grid_id]->non_web) { //only unsolid web nodegrids
              NodeGrid[node_grid_id]->node_solid=FALSE;
            }
          }
        }

    }
  } else { // x=(y-c)/m
    if (Ground[i]->y1>Ground[i]->y2) {
      min=Ground[i]->y2;
      max=Ground[i]->y1;
    } else {
      min=Ground[i]->y1;
      max=Ground[i]->y2;
    }
    for (y=min;y<max;y++) {
      lg_x=(y-Ground[i]->c)/Ground[i]->gradient;
      lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (!NodeGrid[node_grid_id]->non_web) { //only unsolid web nodegrids
        NodeGrid[node_grid_id]->node_solid=FALSE;
      }
      for (int n=0;n<4;n++) {
          double llg_x,llg_y;
          switch (n) {
            case 0:llg_x=lg_x-NODE_SIZE;llg_y=y-NODE_SIZE;break;
            case 1:llg_x=lg_x+NODE_SIZE;llg_y=y-NODE_SIZE;break;
            case 2:llg_x=lg_x-NODE_SIZE;llg_y=y+NODE_SIZE;break;
            case 3:llg_x=lg_x+NODE_SIZE;llg_y=y+NODE_SIZE;break;
          }
          lg_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
          if (lg_grid_id!=-1) {
            UnSetGridLineArray(lg_grid_id,i);
          }
          if (!Ground[i]->is_ghost) {
            node_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
            if (!NodeGrid[node_grid_id]->non_web) { //only unsolid web nodegrids
              NodeGrid[node_grid_id]->node_solid=FALSE;
            }
         }
      }
    }
  }
  Ground[i]->health=-1;
  Ground[i]->x1=Ground[i]->y1=Ground[i]->x2=Ground[i]->y2=-20;
}



void DrawWebs(HDC hdc)
{ int id;
  int c;
  int cx=(int)player.cam_x+(int)player.cam_move_x+(int)player.cam_mouse_move_x;
  int cy=(int)player.cam_y+(int)player.cam_move_y+(int)player.cam_mouse_move_y;
  for (int i=GROUND_NUM;i<GROUND_NUM+player.max_web_num;i++) {
    id=i;
    c=LTCYAN;//Highlight(Ground[id]->health%10<5,LTGRAY,LTCYAN);
    if (id<GROUND_NUM+MAX_WEB_NUM && Ground[id]->x1>-20) {
      if (Ground[id]->health>500) {
        GrLine(hdc,
          Ground[id]->x1+cx,
          Ground[id]->y1+cy,
          Ground[id]->x2+cx,
          Ground[id]->y2+cy,
          c
        );
        if (Ground[id]->health%10<5) {
          GrLine(hdc,Ground[id]->x1+cx-1,Ground[id]->y1+cy-1,Ground[id]->x2+cx-1,Ground[id]->y2+cy-1,c);
          GrLine(hdc,Ground[id]->x1+cx+1,Ground[id]->y1+cy+1,Ground[id]->x2+cx+1,Ground[id]->y2+cy+1,c);        
        }
      } else {
        GrDottedLine(hdc,
          Ground[id]->x1+cx,
          Ground[id]->y1+cy,
          Ground[id]->x2+cx,
          Ground[id]->y2+cy,
          c
        );
        if (Ground[id]->health%10<5) {
          GrDottedLine(hdc,Ground[id]->x1+cx-1,Ground[id]->y1+cy-1,Ground[id]->x2+cx-1,Ground[id]->y2+cy-1,c);
          GrDottedLine(hdc,Ground[id]->x1+cx+1,Ground[id]->y1+cy+1,Ground[id]->x2+cx+1,Ground[id]->y2+cy+1,c);        
        }
      }
    }
  }
}


void DrawGround(HDC hdc)
{
  int i=0;//,j=0;
  int c;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i]->type==0) {
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          c=Ground[i]->color;
          /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
            c=RGB(8,8,8);
          }*/
          GrLine(hdc,
                Ground[i]->x1,
                Ground[i]->y1,
                Ground[i]->x2,
                Ground[i]->y2,
                c);
      }
    }
  }
}



void DrawGroundText(HDC hdc)
{
  int i=0,c;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i]->type==2) { 
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          c=Ground[i]->color;
          /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
            c=RGB(8,8,8);
          }*/
	    GrPrintW(hdc,
            Ground[i]->x1,
            Ground[i]->y1,
            Ground[i]->text,
            "",
            c,
            Ground[i]->font_size,
            //16,
            FALSE,
            yes_unifont);
      }
    }
  }
}

void DrawWaterTriFill(HDC hdc) {
  int i=0,c=0;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i]->type==1) { 
      c=Ground[i]->color;
      /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
        c=RGB(8,8,8);
      }*/
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1,
				Ground[i]->y1,
				Ground[i]->x2,
				Ground[i]->y2,
				Ground[i]->x3,
				Ground[i]->y3,TRUE,HS_FDIAGONAL);
	      DrawTriFill(hdc,c,
                Ground[i]->x1,
				Ground[i]->y1,
				Ground[i]->x2,
				Ground[i]->y2,
				Ground[i]->x3,
				Ground[i]->y3,TRUE,HS_DIAGCROSS);
      }
    }
  }
}


void DrawGroundTriFill(HDC hdc)
{
  int i=0,c=0;
  for (i=0;i<GROUND_NUM;i++) {
    if (Ground[i]->type==3) { 
      c=Ground[i]->color;
      /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
        c=RGB(8,8,8);
      }*/
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1,
				Ground[i]->y1,
				Ground[i]->x2,
				Ground[i]->y2,
				Ground[i]->x3,
				Ground[i]->y3,FALSE,0);
      }
    }
  }
}




void Draw1Ground(HDC hdc,int i,int x, int y)
{
  int c;
    if (Ground[i]->type==0) {
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          c=Ground[i]->color;
          /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
            c=RGB(8,8,8);
          }*/
          GrLine(hdc,
                Ground[i]->x1-x,
                Ground[i]->y1-y,
                Ground[i]->x2-x,
                Ground[i]->y2-y,
                c);
      }
    }
}

void Draw1GroundTriFill(HDC hdc,int i,int x, int y)
{
  int c=0;
    if (Ground[i]->type==3) { 
      c=Ground[i]->color;
      /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
        c=RGB(8,8,8);
      }*/
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1-x,
				Ground[i]->y1-y,
				Ground[i]->x2-x,
				Ground[i]->y2-y,
				Ground[i]->x3-x,
				Ground[i]->y3-y,FALSE,0);
      }
    }
}



void Draw1GroundText(HDC hdc,int i,int x, int y)
{
  int c;
    if (Ground[i]->type==2) { 
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          c=Ground[i]->color;
          /*if (Ground[i]->color_id==0 || Ground[i]->color_id%16==0) {
            c=RGB(8,8,8);
          }*/
	    GrPrintW(hdc,
            Ground[i]->x1-x,
            Ground[i]->y1-y,
            Ground[i]->text,
            "",
            c,
            Ground[i]->font_size,
            //16,
            FALSE,
            yes_unifont);
      }
    }
}



void Draw1WaterTriFill(HDC hdc,int i,int x,int y) {
  int c=0;
    if (Ground[i]->type==1) { 
      c=Ground[i]->color;
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1-x,
				Ground[i]->y1-y,
				Ground[i]->x2-x,
				Ground[i]->y2-y,
				Ground[i]->x3-x,
				Ground[i]->y3-y,TRUE,HS_CROSS);
	      DrawTriFill(hdc,c,
                Ground[i]->x1-x,
				Ground[i]->y1-y,
				Ground[i]->x2-x,
				Ground[i]->y2-y,
				Ground[i]->x3-x,
				Ground[i]->y3-y,TRUE,HS_DIAGCROSS);
      }
    }
}


