
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


void SetGridLineArray(int grid_id,int ground_id)
{
  if (!Ground[ground_id].already_in_grid[grid_id] && VGrid[grid_id].max_ground_num<MAX_GROUNDS_WITHIN_GRID) {//if not in the grid
    //Ground related
    Ground[ground_id].saved_pos_in_grid[grid_id]=VGrid[grid_id].max_ground_num;
    Ground[ground_id].already_in_grid[grid_id]=TRUE;
    //grid related
    VGrid[grid_id].ground_ids[VGrid[grid_id].max_ground_num]=ground_id;
    VGrid[grid_id].max_ground_num++;
  }
}


void UnSetGridLineArray(int grid_id,int ground_id)
{
  int i=0;
  if (Ground[ground_id].already_in_grid[grid_id]) {
   //grid related
    for (i=Ground[ground_id].saved_pos_in_grid[grid_id];i<VGrid[grid_id].max_ground_num-1;i++) {
      VGrid[grid_id].ground_ids[i]=VGrid[grid_id].ground_ids[i+1];
      Ground[VGrid[grid_id].ground_ids[i]].saved_pos_in_grid[grid_id]--;
    }
    VGrid[grid_id].ground_ids[VGrid[grid_id].max_ground_num-1]=-1;
    VGrid[grid_id].max_ground_num--;
   //ground related
    Ground[ground_id].already_in_grid[grid_id]=FALSE;
    Ground[ground_id].saved_pos_in_grid[grid_id]=-1;
  }
}


void InitGrid() 
{
  int i=0,j=0,x=0,y=0;
  for (i=0;i<VGRID_NUM;i++) {
    VGrid[i].within_render_distance=FALSE;
    VGrid[i].max_ground_num=0;
    for (j=0;j<MAX_GROUNDS_WITHIN_GRID;j++) {
      VGrid[i].ground_ids[j]=-1;
    }
    VGrid[i].x1=x;
    VGrid[i].y1=y;
    VGrid[i].x2=VGrid[i].x1+VGRID_SIZE;
    VGrid[i].y2=VGrid[i].y1+VGRID_SIZE;
    x+=VGRID_SIZE;
    if (x>MAP_WIDTH-VGRID_SIZE) {
      x=0;
      y+=VGRID_SIZE;
    }
  }

  x=0;
  y=0;
  for (i=0;i<GRID_NUM;i++) {
    Grid[i].within_render_distance=FALSE;
    Grid[i].enemy_occupy_num=0;
    for (j=0;j<ENEMY_NUM;j++) {
      Grid[i].enemy_occupy[j]=-1;
    }
    Grid[i].x1=x;
    Grid[i].y1=y;
    Grid[i].x2=Grid[i].x1+GRID_SIZE;
    Grid[i].y2=Grid[i].y1+GRID_SIZE;
    x+=GRID_SIZE;
    if (x>MAP_WIDTH-GRID_SIZE) {
      x=0;
      y+=GRID_SIZE;
    }
  }
}


void DrawGrid(HDC hdc) {
  /*GrLine(dc,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2);
  GrLine(dc,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2);
  GrLine(dc,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2);
  GrLine(dc,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2);*/
  for (int i=0;i<GRID_NUM;i++) {
//    GrLine(hdc,Grid[i].x1,Grid[i].y1,Grid[i].x2,Grid[i].y1,RGB(255,255,255));
//    GrLine(hdc,Grid[i].x1,Grid[i].y1,Grid[i].x1,Grid[i].y2,RGB(255,255,255));
//    GrLine(hdc,Grid[i].x2,Grid[i].y1,Grid[i].x2,Grid[i].y2,RGB(255,255,255));
//    GrLine(hdc,Grid[i].x1,Grid[i].y2,Grid[i].x2,Grid[i].y2,RGB(255,255,255));

    GrLine(hdc,Grid[i].x1+player.cam_x,Grid[i].y1+player.cam_y,Grid[i].x2+player.cam_x,Grid[i].y1+player.cam_y,RGB(8,39,225));
    GrLine(hdc,Grid[i].x1+player.cam_x,Grid[i].y1+player.cam_y,Grid[i].x1+player.cam_x,Grid[i].y2+player.cam_y,RGB(8,39,225));
    GrLine(hdc,Grid[i].x2+player.cam_x,Grid[i].y1+player.cam_y,Grid[i].x2+player.cam_x,Grid[i].y2+player.cam_y,RGB(8,39,225));
    GrLine(hdc,Grid[i].x1+player.cam_x,Grid[i].y2+player.cam_y,Grid[i].x2+player.cam_x,Grid[i].y2+player.cam_y,RGB(8,39,225));
  }
}





//Node Grid


void InitNodeGrid()
{
  int i=0,x=0,y=0;
  for (i=0;i<MAP_NODE_NUM;i++) {
    NodeGrid[i].node_solid=FALSE;
    NodeGrid[i].x1=x;
    NodeGrid[i].y1=y;
    NodeGrid[i].x2=NodeGrid[i].x1+NODE_SIZE;
    NodeGrid[i].y2=NodeGrid[i].y1+NODE_SIZE;
    x+=NODE_SIZE;
    if (x>MAP_WIDTH-NODE_SIZE) {
      x=0;
      y+=NODE_SIZE;
    }
  }
}

bool IsCollideSolid(double x1,double y1,double x2,double y2,double gradient,double c)
{
  int node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x,lg_y;
  if (-1<gradient && gradient<1) { // y=mx+c
    for (x=x1;x<=x2;x++) {
      lg_y=x*gradient+c;
      node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (NodeGrid[node_grid_id].node_solid) {
	return TRUE;
      }
    }
  } else { // x=(y-c)/m
    if (y1<y2) {
      min=y1;
      max=y2;
    } else {
      min=y2;
      max=y1;
    }
    for (y=min;y<=max;y++) {
      lg_x=(y-c)/gradient;
      node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (NodeGrid[node_grid_id].node_solid) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

bool IsHasNeighbours(int nx,int ny) 
{
  int i=0,node_grid_id=0,x=0,y=0;
  for (i=0;i<8;i++) {
    x=nx;
    y=ny;
    if (i<=2) {
      x-=NODE_SIZE; //case 1: left
      switch (i) {
        case 0:
	  y-=NODE_SIZE;
          break;
        case 2:
	  y+=NODE_SIZE;
          break;
      }
    } else if (3<=i<=5) {
      x+=NODE_SIZE; // case 4: right
      switch (i) {
        case 3:
          y-=NODE_SIZE;
          break;
        case 5:
          y+=NODE_SIZE;
          break;
      }
    } else {
      switch (i) {
        case 6: //above
          y-=NODE_SIZE;
          break;
        case 7: //below
          y+=NODE_SIZE;
          break;
      }
    }
    if (0<x && x<MAP_WIDTH && 0<y && y<MAP_HEIGHT) {
      node_grid_id=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (NodeGrid[node_grid_id].node_solid) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

void SetNodeGridAttributes2(int i)
{
  int j=0,x=0,y=0,lg_grid_id=0;
  double gradient[2],c[2],
      min=0,max=0,x1=0,y1=0,lg_x=0,lg_y=0;
  gradient[0]=GetGradient(Ground[i].x1,Ground[i].y1,Ground[i].x3,Ground[i].y3);
  gradient[1]=GetGradient(Ground[i].x2,Ground[i].y2,Ground[i].x3,Ground[i].y3);
  c[0]=GetGroundC(Ground[i].x3,Ground[i].y3,gradient[0]);
  c[1]=GetGroundC(Ground[i].x3,Ground[i].y3,gradient[1]);
  for (j=0;j<2;j++) {
    switch (j) {
      case 0:
	x1=Ground[i].x1;
	y1=Ground[i].y1;
	break;
      case 1:
	x1=Ground[i].x2;
	y1=Ground[i].y2;
	break;
    }
    if (-1<gradient[i] && gradient[i]<1) {//y=mx+c
      if (x1<Ground[i].x3) { //x1 is lower than x3
	min=x1;
	max=Ground[i].x3;
      } else {
	min=Ground[i].x3;
	max=x1;
      }
      for (x=min;x<max;x++) {
        lg_y=x*gradient[j]+c[j];
        lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        SetGridLineArray(lg_grid_id,i);
      }
    } else {// x=(y-c)/m
      if (y1<Ground[i].y3) { //y1 is lower than x3
	min=y1;
	max=Ground[i].y3;
      } else {
	min=Ground[i].y3;
	max=y1;
      }
      for (y=min;y<max;y++) {
        lg_x=(y-c[j])/gradient[j];
        lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        SetGridLineArray(lg_grid_id,i);
      }
    }
  }
}



void SetNodeGridAttributes(int i)
{
  int lg_grid_id=0,node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (-1<Ground[i].gradient && Ground[i].gradient<1) { // y=mx+c
    for (x=Ground[i].x1;x<Ground[i].x2;x++) {
      lg_y=x*Ground[i].gradient+Ground[i].c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(lg_grid_id,i);
      if (!Ground[i].is_ghost) { //Not a ghost
        node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
        NodeGrid[node_grid_id].node_solid=TRUE;
      }
    }
  } else { // x=(y-c)/m
    if (Ground[i].y1<Ground[i].y2) {
      min=Ground[i].y1;
      max=Ground[i].y2;
    } else {
      min=Ground[i].y2;
      max=Ground[i].y1;
    }
    for (y=min;y<max;y++) {
      lg_x=(y-Ground[i].c)/Ground[i].gradient;
      lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(lg_grid_id,i);
      if (!Ground[i].is_ghost) {
        node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
        NodeGrid[node_grid_id].node_solid=TRUE;
      }
    }
  }
  if (Ground[i].type==3) {//triangle
    SetNodeGridAttributes2(i);
  }
}

void InitNodeGridAttributes()
{
  for (int i=0;i<GROUND_NUM;i++) {
    SetNodeGridAttributes(i);
  }
}






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
      lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
      node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      NodeGrid[node_grid_id].node_solid=FALSE;
    }
  }
  Ground[i].health=-1;
  Ground[i].x1=Ground[i].y1=Ground[i].x2=Ground[i].y2=-20;
}




bool IsCollideCrawler(double x1,double y1,double x2,double y2,double gradient,double c)
{
  int on_grid_id=0,i=0,enemy_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (x1!=x2) {
    if (-1<gradient<1) { // y=mx+c
      for (x=x1;x<=x2;x++) {
        lg_y=x*gradient+c;
        on_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        for (i=0;i<Grid[on_grid_id].enemy_occupy_num;i++) {
    	  enemy_id=Grid[on_grid_id].enemy_occupy[i];
          if (Enemy[enemy_id].species==1 && Enemy[enemy_id].health>0) {
    	    if (GetDistance(x,lg_y,Enemy[enemy_id].x,Enemy[enemy_id].y)<=NODE_SIZE*2) {
    	      return TRUE;
    	    }
          }
        }
      }
    } else { // x=(y-c)/m
      if (y1<y2) {
        min=y1;
        max=y2;
      } else {
        min=y2;
        max=y1;
      }
      for (y=min;y<=max;y++) {
        lg_x=(y-c)/gradient;
        on_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        for (i=0;i<Grid[on_grid_id].enemy_occupy_num;i++) {
	      enemy_id=Grid[on_grid_id].enemy_occupy[i];
          if (Enemy[enemy_id].species==1 && Enemy[enemy_id].health>0) {
    	    if (GetDistance(lg_x,y,Enemy[enemy_id].x,Enemy[enemy_id].y)<=NODE_SIZE*2) {
    	      return TRUE;
    	    }
          }
        }
      }
    }
  } else {
    return TRUE;
  }
  return FALSE;
}

