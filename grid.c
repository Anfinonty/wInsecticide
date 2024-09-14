
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
  if (!Ground[ground_id]->already_in_grid[grid_id] && VGrid[grid_id]->max_ground_num<MAX_GROUNDS_WITHIN_GRID) {//if not in the grid
    //Ground related
    Ground[ground_id]->saved_pos_in_grid[grid_id]=VGrid[grid_id]->max_ground_num;
    Ground[ground_id]->already_in_grid[grid_id]=TRUE;
    //grid related
    VGrid[grid_id]->ground_ids[VGrid[grid_id]->max_ground_num]=ground_id;
    VGrid[grid_id]->max_ground_num++;
  }
}


void UnSetGridLineArray(int grid_id,int ground_id)
{
  int i=0;
  if (Ground[ground_id]->already_in_grid[grid_id]) {
   //grid related
    for (i=Ground[ground_id]->saved_pos_in_grid[grid_id];i<VGrid[grid_id]->max_ground_num-1;i++) {
      VGrid[grid_id]->ground_ids[i]=VGrid[grid_id]->ground_ids[i+1];
      Ground[VGrid[grid_id]->ground_ids[i]]->saved_pos_in_grid[grid_id]--;
    }
    VGrid[grid_id]->ground_ids[VGrid[grid_id]->max_ground_num-1]=-1;
    VGrid[grid_id]->max_ground_num--;
   //ground related
    Ground[ground_id]->already_in_grid[grid_id]=FALSE;
    Ground[ground_id]->saved_pos_in_grid[grid_id]=-1;
  }
}


void InitGrid() 
{
  int i=0,j=0,x=0,y=0;
  for (i=0;i<VGRID_NUM;i++) {
    VGrid[i]->within_render_distance=FALSE;
    VGrid[i]->max_ground_num=0;
    
    for (j=0;j<MAX_GROUNDS_WITHIN_GRID;j++) {
      VGrid[i]->ground_ids[j]=-1;
    }
    VGrid[i]->x1=x;
    VGrid[i]->y1=y;
    VGrid[i]->x2=VGrid[i]->x1+VGRID_SIZE;
    VGrid[i]->y2=VGrid[i]->y1+VGRID_SIZE;
    x+=VGRID_SIZE;
    if (x>MAP_WIDTH-VGRID_SIZE) {
      x=0;
      y+=VGRID_SIZE;
    }
  }
}


//void DrawGrid(HDC hdc)
//{
  /*GrLine(dc,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2);
  GrLine(dc,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2);
  GrLine(dc,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y1+cam_y-PLAYER_HEIGHT/2,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2);
  GrLine(dc,Grid[i].x1+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2,Grid[i].x2+cam_x-PLAYER_WIDTH/2,Grid[i].y2+cam_y-PLAYER_HEIGHT/2);*/
  /*for (int i=0;i<GRID_NUM;i++) {
//    GrLine(hdc,Grid[i].x1,Grid[i].y1,Grid[i].x2,Grid[i].y1,RGB(255,255,255));
//    GrLine(hdc,Grid[i].x1,Grid[i].y1,Grid[i].x1,Grid[i].y2,RGB(255,255,255));
//    GrLine(hdc,Grid[i].x2,Grid[i].y1,Grid[i].x2,Grid[i].y2,RGB(255,255,255));
//    GrLine(hdc,Grid[i].x1,Grid[i].y2,Grid[i].x2,Grid[i].y2,RGB(255,255,255));
    GrLine(hdc,Grid[i].x1+player.cam_x,Grid[i].y1+player.cam_y,Grid[i].x2+player.cam_x,Grid[i].y1+player.cam_y,RGB(8,39,225));
    GrLine(hdc,Grid[i].x1+player.cam_x,Grid[i].y1+player.cam_y,Grid[i].x1+player.cam_x,Grid[i].y2+player.cam_y,RGB(8,39,225));
    GrLine(hdc,Grid[i].x2+player.cam_x,Grid[i].y1+player.cam_y,Grid[i].x2+player.cam_x,Grid[i].y2+player.cam_y,RGB(8,39,225));
    GrLine(hdc,Grid[i].x1+player.cam_x,Grid[i].y2+player.cam_y,Grid[i].x2+player.cam_x,Grid[i].y2+player.cam_y,RGB(8,39,225));
  }*/
//}





//Node Grid
void InitNodeGrid()
{
  int i=0,x=0,y=0;
  for (i=0;i<MAP_NODE_NUM;i++) {
    NodeGrid[i]->node_solid=FALSE;
    NodeGrid[i]->non_web=FALSE;
    NodeGrid[i]->node_water=FALSE;
    NodeGrid[i]->node_no_rain=FALSE;
    NodeGrid[i]->node_no_shade=FALSE;
    NodeGrid[i]->tmp_wet=FALSE;
    NodeGrid[i]->x1=x;
    NodeGrid[i]->y1=y;
    NodeGrid[i]->x2=NodeGrid[i]->x1+NODE_SIZE;
    NodeGrid[i]->y2=NodeGrid[i]->y1+NODE_SIZE;
    x+=NODE_SIZE;
    if (x>MAP_WIDTH-NODE_SIZE) {
      x=0;
      y+=NODE_SIZE;
    }
  }
}



void InitNodeShade()
{  
  int on_node_grid_id=-1;
  double gradient=rain_grad_rise/rain_grad_run; //y/x to down-right
  double start_x=0,x=0,y=0;
  while (start_x<MAP_WIDTH) {
    on_node_grid_id=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
    if (on_node_grid_id!=-1) {
      if (NodeGrid[on_node_grid_id]->node_solid || NodeGrid[on_node_grid_id]->node_no_rain || y>=MAP_HEIGHT) { //solid detected, move to next start_x nodegrid
        start_x+=NODE_SIZE;
        x=start_x;
        y=0;
      } else { //keep going down.
        NodeGrid[on_node_grid_id]->node_no_shade=TRUE;
        y+=NODE_SIZE;
        x=start_x+GetX(y,gradient,0);
      }
    } else {
      start_x+=NODE_SIZE;
      x=start_x;
      y=0;
    }
  }
}

/*bool IsCollideSolid(double x1,double y1,double x2,double y2,double gradient,double c)
{
  int node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x,lg_y;
  if (-1<gradient && gradient<1) { // y=mx+c
    for (x=x1;x<=x2;x++) {
      lg_y=x*gradient+c;
      node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (NodeGrid[node_grid_id]->node_solid) {
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
      if (NodeGrid[node_grid_id]->node_solid) {
        return TRUE;
      }
    }
  }
  return FALSE;
}*/

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
    } else if (3<=i && i<=5) {
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
      if (NodeGrid[node_grid_id]->node_solid) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

void SetNodeGridAttributes2(int i)
{
  int j=0,x=0,y=0,lg_grid_id=0,min=0,max=0,x1=0,y1=0;
  double gradient1,gradient2,c1,c2,lg_x=0,lg_y=0,gradient=0,c=0;
  gradient1=GetGradient(Ground[i]->x1,Ground[i]->y1,Ground[i]->x3,Ground[i]->y3);
  gradient2=GetGradient(Ground[i]->x2,Ground[i]->y2,Ground[i]->x3,Ground[i]->y3);
  c1=GetGroundC(Ground[i]->x3,Ground[i]->y3,gradient1);
  c2=GetGroundC(Ground[i]->x3,Ground[i]->y3,gradient2);
  for (j=0;j<2;j++) {
    switch (j) {
      case 0:
	    x1=Ground[i]->x1;
	    y1=Ground[i]->y1;
        gradient=gradient1;
        c=c1;
	    break;
      case 1:
	    x1=Ground[i]->x2;
	    y1=Ground[i]->y2;
        gradient=gradient2;
        c=c2;
	    break;
    }


    if (-1<gradient && gradient<1) {//y=mx+c
      if (x1<Ground[i]->x3) { //x1 is lower than x3
	    min=x1;
	    max=Ground[i]->x3;
      } else {
	    min=Ground[i]->x3;
	    max=x1;
      }
      for (x=min;x<max;x++) {
        lg_y=x*gradient+c;
        lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        SetGridLineArray(lg_grid_id,i);
      }
    } else {// x=(y-c)/m
      if (y1<Ground[i]->y3) { //y1 is lower than x3
	    min=y1;
	    max=Ground[i]->y3;
      } else {
	    min=Ground[i]->y3;
	    max=y1;
      }
      for (y=min;y<max;y++) {
        lg_x=(y-c)/gradient;
        lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
        SetGridLineArray(lg_grid_id,i);
      }
    }
  }
}



void TriFillNodeGridType(int gid)
{
/*
                 ^
               /~~~\
            /~~~~~~!\
         /~~~~~~~~~~~\
      /~~~~~~~~~~~~~~~\
   <_____~~~~~~~~~~~~~~\
          _____ ~~~~~~~~\
                _________>
*/


  if (Ground[gid]->type==1 || Ground[gid]->type==3) {
  double
      gradient_middle1,gradient_middle2,gradient_largest,
      c_middle1,c_middle2,c_largest,
      smallest=INT_MAX,largest=0,
      x_arr[3],y_arr[3];
  x_arr[0]=Ground[gid]->x1;
  x_arr[1]=Ground[gid]->x2;
  x_arr[2]=Ground[gid]->x3;
  y_arr[0]=Ground[gid]->y1;
  y_arr[1]=Ground[gid]->y2;
  y_arr[2]=Ground[gid]->y3;
  int x_1,x_2,i,x,y,k,saved_largest=0,saved_smallest=0,saved_middle=0;
  for (i=0;i<3;i++) {
    if (y_arr[i]<smallest) {
      smallest=y_arr[i];
      saved_smallest=i;
    }
  }
  for (i=0;i<3;i++) {
    if (y_arr[i]>largest) {
      largest=y_arr[i];
      saved_largest=i;
    }
  }
  for (i=0;i<3;i++) {
    if (i!=saved_smallest && i!=saved_largest) {
      saved_middle=i;
    }
  }
  gradient_middle1=GetGradient(x_arr[saved_smallest],y_arr[saved_smallest],x_arr[saved_middle],y_arr[saved_middle]); //Gradient of main line
  c_middle1=GetGroundC(x_arr[saved_smallest],y_arr[saved_smallest],gradient_middle1);

  gradient_middle2=GetGradient(x_arr[saved_largest],y_arr[saved_largest],x_arr[saved_middle],y_arr[saved_middle]);
  c_middle2=GetGroundC(x_arr[saved_largest],y_arr[saved_largest],gradient_middle2);

  gradient_largest=GetGradient(x_arr[saved_largest],y_arr[saved_largest],x_arr[saved_smallest],y_arr[saved_smallest]);
  c_largest=GetGroundC(x_arr[saved_smallest],y_arr[saved_smallest],gradient_largest);



  int smallest_y=stickyTo(smallest,NODE_SIZE);
  int middle_y=stickyTo(y_arr[saved_middle],NODE_SIZE);
  int largest_y=stickyTo(largest,NODE_SIZE);

  int x1,x2;
  for (y=smallest_y;y<middle_y;y+=NODE_SIZE) {//small to middle
    x_1=GetX(y,gradient_middle1,c_middle1);
    x_2=GetX(y,gradient_largest,c_largest);
    x_1=stickyTo(x_1,NODE_SIZE);
    x_2=stickyTo(x_2,NODE_SIZE);
    x1=min(x_1,x_2);
    x2=max(x_1,x_2);
    for (x=x1;x<x2;x+=NODE_SIZE) {
      k=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (k!=-1) {
        if (Ground[gid]->type==1) {
          NodeGrid[k]->node_water=TRUE;
        } else if (Ground[gid]->type==3) {
          NodeGrid[k]->node_no_rain=TRUE;
        }
      }
    }
  }
  for (y=middle_y;y<largest_y;y+=NODE_SIZE) {//middle to largest
    x_1=GetX(y,gradient_middle2,c_middle2);
    x_2=GetX(y,gradient_largest,c_largest);
    x_1=stickyTo(x_1,NODE_SIZE);
    x_2=stickyTo(x_2,NODE_SIZE);
    x1=min(x_1,x_2);
    x2=max(x_1,x_2);
    for (x=x1;x<x2;x+=NODE_SIZE) {
      k=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (k!=-1) {
        if (Ground[gid]->type==1) {
          NodeGrid[k]->node_water=TRUE;
        } else if (Ground[gid]->type==3) {
          NodeGrid[k]->node_no_rain=TRUE;
        }
      }
    }
  }
  }
}


void SetNodeGridAttributes(int i)
{
  int lg_grid_id=0,node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (-1<Ground[i]->gradient && Ground[i]->gradient<1) { // y=mx+c
    for (x=Ground[i]->x1;x<Ground[i]->x2;x++) {
      lg_y=x*Ground[i]->gradient+Ground[i]->c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(lg_grid_id,i);
      if (!Ground[i]->is_ghost) { //Not a ghost
        node_grid_id=GetGridId(x,lg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
        NodeGrid[node_grid_id]->node_solid=TRUE;
        if (i<GROUND_NUM) {
          NodeGrid[node_grid_id]->non_web=TRUE;
        }
      }
    }
  } else { // x=(y-c)/m
    if (Ground[i]->y1<Ground[i]->y2) {
      min=Ground[i]->y1;
      max=Ground[i]->y2;
    } else {
      min=Ground[i]->y2;
      max=Ground[i]->y1;
    }
    for (y=min;y<max;y++) {
      lg_x=(y-Ground[i]->c)/Ground[i]->gradient;
      lg_grid_id=GetGridId(lg_x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(lg_grid_id,i);
      if (!Ground[i]->is_ghost) {
        node_grid_id=GetGridId(lg_x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
        NodeGrid[node_grid_id]->node_solid=TRUE;
        if (i<GROUND_NUM) {
          NodeGrid[node_grid_id]->non_web=TRUE;
        }
      }
    }
  }
  if (Ground[i]->type==3 || Ground[i]->type==1) {//triangle
    SetNodeGridAttributes2(i);
  }
  if (Ground[i]->type==3 || Ground[i]->type==1) { //water trifill
    TriFillNodeGridType(i);
  }
}



void InitNodeGridAttributes()
{
  for (int i=0;i<GROUND_NUM;i++) {
    SetNodeGridAttributes(i);
  }
  InitNodeShade();
}




void DrawGrids(HDC hdc,int player_cam_move_x,int player_cam_move_y)
{
  for (int y=0;y<=MAP_HEIGHT;y+=GRID_SIZE) { // ---->
    GrLine(hdc,0+player_cam_move_x,y+player_cam_move_y,MAP_WIDTH+player_cam_move_x,y+player_cam_move_y,BLACK);
  }

  for (int x=0;x<=MAP_WIDTH;x+=GRID_SIZE) {  // |
    GrLine(hdc,x+player_cam_move_x,0+player_cam_move_y,x+player_cam_move_x,MAP_HEIGHT+player_cam_move_y,BLACK);
  }
}



void DrawNodeGrids(HDC hdc)
{
  for (int i=0;i<MAP_NODE_NUM;i++) {
    if (NodeGrid[i]->node_no_rain) //solid trifill
      GrCircle(hdc,NodeGrid[i]->x1+player.cam_x,NodeGrid[i]->y1+player.cam_y,3,YELLOW,-1);
    if (NodeGrid[i]->node_no_shade)
      GrCircle(hdc,NodeGrid[i]->x1+player.cam_x,NodeGrid[i]->y1+player.cam_y,3,BLUE,-1);
    /*if (NodeGrid[i]->tmp_wet)
      GrCircle(hdc,NodeGrid[i]->x1+player.cam_x,NodeGrid[i]->y1+player.cam_y,3,BLUE,-1);*/
  }
}




//Shadows
struct photon
{
  bool in_solid,
       passed_solid;
  int dist_travelled;
  double rise,run;
} photon;


void DrawShadows2(HDC hdcSrc, HDC hdcDest, int x,int y,bool t)
{
    int c=Highlight(IsInvertedBackground(),BLACK,WHITE);
    int color = GetPixel(hdcSrc, x, y);
    /*if (color == CLR_INVALID) {
        printf("Failed to get pixel color\n");
    } else {
        //printf("Pixel color at (%d, %d): R=%d, G=%d, B=%d\n", x, y, GetRValue(color), GetGValue(color), GetBValue(color));
        printf("Pixel color at (%d, %d): %d\n", x, y, color);
    }*/
    //printf("Pixel color at (%d, %d): %d\n", x, y, color);
    if (color!=MYCOLOR1) { //<------ color that shows that it is NOT in non-solid, it is a solid
      //GrCircle(hdc,x,y,1,LTGRAY,-1);
      photon.in_solid=TRUE;
    }
    if (photon.in_solid && color!=MYCOLOR1) {
      photon.dist_travelled++;
    }

    if (photon.in_solid && color==MYCOLOR1) { //RGB(121,121,121)
      photon.in_solid=FALSE;
      photon.passed_solid=TRUE;
      photon.dist_travelled=0;
    }

    if ((t && photon.passed_solid) || (!t && photon.passed_solid && photon.in_solid)) {
      if (photon.in_solid) {
        photon.dist_travelled++;
      }
      if (photon.in_solid) {
        if (x%4==0) { //even
          if (y%4==0) {
            GrCircle(hdcDest,x,y,1,c,-1);
          }
        } else { //odd
          if (x%2==0) {
            if (y%4!=0 && y%2==0) {
              GrCircle(hdcDest,x,y,1,c,-1);
            }
          }
        }
      } else {
        GrCircle(hdcDest,x,y,1,c,-1);
      } 
      if (photon.dist_travelled>20) {
        photon.passed_solid=FALSE;
        photon.dist_travelled=0;
      }
    }
}






void CreatePlatformShadowBitmap(HDC hdc)
{
   //pass through first surface, hit second surface
/*

struct photon
{
    bool passed_solid;
    double x,y;
}



            photons
              |             |
              |       v v v v v v v v
              |     v ------------- <---------solids
              v v v /       |       \     |
            -------/ -----------------\   V
              |             |  no shad- --\____
              |             |  ows cuz no triangle below
              |             |
              v             v      _____________
above-----> ______________________/|||||||||   <--- shadow dist on triangle below
blocks      |||||||||||||||||||||||---------
are         -----------------------
non-solids

*/

    photon.rise = 20;
    photon.run = 5;

    // Set mapping mode so that +ve y axis is upwards
    HDC hMemSrc = CreateCompatibleDC(hdc);
    HDC hMemDest = CreateCompatibleDC(hdc);

    // Create a compatible bitmap for the shadow shader
    map_platforms_shadow_shader = CreateCrunchyBitmap(MAP_WIDTH, MAP_HEIGHT);

    // Select the bitmaps into the device contexts
    SelectObject(hMemSrc, map_platforms_sprite_mask);
    SelectObject(hMemDest, map_platforms_shadow_shader);

    int c=Highlight(IsInvertedBackground(),LTGRAY,LTR2LTGRAY/*LTRYELLOW*/);
    //GrRect(hMemDest,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,c);
    GrRect(hMemDest,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,c);


    double gradient=(double)photon.rise / photon.run;
    double start_x=0, x=0, y=0;

  while (start_x<MAP_WIDTH) {
    if (y>=MAP_HEIGHT) { //solid detected, move to next start_x nodegrid
      start_x++;
      x=start_x;
      y=0;
      photon.dist_travelled=0;
      photon.passed_solid=FALSE;
      photon.in_solid=FALSE;

    } else { //keep going down.
      DrawShadows2(hdc,hMemDest,x,y,TRUE);
      y++;
      x=start_x+GetX(y,gradient,0);
    }
  }
  photon.dist_travelled=0;
  photon.passed_solid=FALSE;
  photon.in_solid=FALSE;

  start_x=0,x=0,y=MAP_HEIGHT;
  while (start_x<MAP_WIDTH) {
    if (y<=0) { //solid detected, move to next start_x nodegrid
      start_x++;
      x=start_x;
      y=MAP_HEIGHT;
      photon.dist_travelled=0;
      photon.passed_solid=FALSE;
      photon.in_solid=FALSE;

    } else { //keep going up.
      DrawShadows2(hdc,hMemDest,x,y,FALSE);
      y--;
      x=start_x+GetX(y,gradient,0);
    }
  }
  photon.dist_travelled=0;
  photon.passed_solid=FALSE;
  photon.in_solid=FALSE;

  DeleteDC(hMemDest);
  DeleteDC(hMemSrc);
}


