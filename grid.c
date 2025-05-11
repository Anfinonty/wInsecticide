
int GetGridId(int x,int y,int width, int size,int max)
{
  int id=-1,row=0,column=0;
  row=y/size;
  column=x/size;
  id=row*(width/size)+column;
  if (-1<id && id<max && 0<=x && x<width) {
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



void InitGridTiles(const wchar_t* lvl_name)
{
  bool has_ground=FALSE;
  bool has_water=FALSE;
  int j,k;
  bool yes_shadow=FALSE;
  wchar_t seg_name[72];
  if (is_shadows && game_shadow) {
    yes_shadow=TRUE;
  }
  FOREGROUND_GRID_NUM=0;
  PLATFORM_GRID_NUM=0;
  SHADOW_GRID_NUM=0;

  for (int i=0;i<VGRID_NUM;i++) {      
    for (int j=0;j<VGrid[i]->max_ground_num;j++) {
      k=VGrid[i]->ground_ids[j];
      if (Ground[k]->type==1) {
        has_water=TRUE;
      } else {
        has_ground=TRUE;
      }
      if (has_water && has_ground) {
        break;
      }
    }


    if (has_ground) {
      VGrid[i]->draw_platform_seg_id=PLATFORM_GRID_NUM;
      PLATFORM_GRID_NUM++;
    } else {
      VGrid[i]->draw_platform_seg_id=-1;
    }
    has_ground=FALSE;


    if (has_water) {
      VGrid[i]->draw_foreground_seg_id=FOREGROUND_GRID_NUM;
      FOREGROUND_GRID_NUM++;
    } else {
      VGrid[i]->draw_foreground_seg_id=-1;
    }
    has_water=FALSE;



    if (yes_shadow) {
      swprintf(seg_name,72,L"saves/%s/seg_shadow/%d.bmp", lvl_name,i);
      if (FileExists(seg_name)) {
        VGrid[i]->has_shadow=TRUE;
        VGrid[i]->draw_shadow_seg_id=SHADOW_GRID_NUM;
        SHADOW_GRID_NUM++;
      } else {
        VGrid[i]->draw_shadow_seg_id=-1;
      }
    }    
  }
}


int loading_tile_grid_prog=0;
void DrawCreateTiles(HDC hdc)
{
  HBITMAP oldbitmap;
  oldbitmap=CreateBitmap(0,0, 1, 1, NULL);
  bool yes_shadow=FALSE;
  wchar_t seg_name[72];  
  int tmp_id;

  if (loading_tile_grid_prog==1) {
    if (is_shadows && game_shadow) {
      yes_shadow=TRUE;
    }
    if (yes_shadow) {
      wchar_t lvl_shadow_name[72];
      if (!back_to_menu)
        swprintf(lvl_shadow_name,72,L"%s",level_names[level_chosen]);    
      else
        swprintf(lvl_shadow_name,72,L"mm_demo2");

      wchar_t lvl_shadow_folder_name[128];
      swprintf(lvl_shadow_folder_name,128,L"saves/%s/seg_shadow",lvl_shadow_name);

      if (!DirExists(lvl_shadow_folder_name)) {
        flag_display_long_loading=TRUE;
      }
    }
    loading_tile_grid_prog=2;
    //Draw "Loading shadows. This will take a while..."
  } else if (loading_tile_grid_prog==2) {
    if (is_shadows && game_shadow) {
      yes_shadow=TRUE;
    }
    if (yes_shadow) {
      wchar_t lvl_shadow_name[72];
      if (!back_to_menu)
        swprintf(lvl_shadow_name,72,L"%s",level_names[level_chosen]);    
      else
        swprintf(lvl_shadow_name,72,L"mm_demo2");

      wchar_t lvl_shadow_folder_name[128];
      swprintf(lvl_shadow_folder_name,128,L"saves/%s/seg_shadow",lvl_shadow_name);

      if (!DirExists(lvl_shadow_folder_name)) {
        SaveLvlBmp(NULL,hdc,lvl_shadow_name);
        InitGridTiles(lvl_shadow_name);
        loading_numerator=0;
        loading_denominator=SHADOW_GRID_NUM+PLATFORM_GRID_NUM+FOREGROUND_GRID_NUM;
        flag_display_long_loading=FALSE;
      }
    }

    loading_tile_grid_prog=3;
  } else if (loading_tile_grid_prog==3) {
    TileMapPlatform = calloc(PLATFORM_GRID_NUM,sizeof(ATileMap*));
    for (int i=0;i<PLATFORM_GRID_NUM;i++) {
      ATileMap *newTileMap = createTileMap();
      TileMapPlatform[i] = newTileMap;
    }
    for (int i=0;i<VGRID_NUM;i++) {
      if (VGrid[i]->draw_platform_seg_id!=-1) {
          tmp_id=VGrid[i]->draw_platform_seg_id;
          if (tmp_id!=-1) { //0, 1, 2 , ...
            TileMapPlatform[tmp_id]->x=VGrid[i]->x1;
            TileMapPlatform[tmp_id]->y=VGrid[i]->y1;

            TileMapPlatform[tmp_id]->sprite_paint=CreateCrunchyBitmap(VGRID_SIZE,VGRID_SIZE);
            SelectObject(hdc,TileMapPlatform[tmp_id]->sprite_paint);
            GrRect(hdc,0,0,VGRID_SIZE+1,VGRID_SIZE+1,MYCOLOR1);

            for (int s=0;s<3;s++) {
              for (int k=0;k<VGrid[i]->max_ground_num;k++) {
                int l = VGrid[i]->ground_ids[k];
                switch (s) {
                  case 0: //draw ground trifill
                    Draw1GroundTriFill(hdc,l,VGrid[i]->x1,VGrid[i]->y1);
                    break;
                  case 1: //draw ground line
                    Draw1Ground(hdc,l,VGrid[i]->x1,VGrid[i]->y1);
                    break;
                  case 2: //draw text
                    Draw1GroundText(hdc,l,VGrid[i]->x1,VGrid[i]->y1);
                    break;
                }
              }
            }
            SelectObject(hdc,oldbitmap);
            TileMapPlatform[tmp_id]->sprite_mask=CreateBitmapMask(TileMapPlatform[tmp_id]->sprite_paint,MYCOLOR1,NULL); //create mask
            loading_numerator++;
          }
      }
    }
    loading_tile_grid_prog=4;

  } else if (loading_tile_grid_prog==4) {
    if (FOREGROUND_GRID_NUM>0) {

      TileMapForeground = calloc(FOREGROUND_GRID_NUM,sizeof(ATileMap*));
      for (int i=0;i<FOREGROUND_GRID_NUM;i++) {
        ATileMap *newTileMap = createTileMap();
        TileMapForeground[i] = newTileMap;
      }

      for (int i=0;i<VGRID_NUM;i++) {
        if (VGrid[i]->draw_foreground_seg_id!=-1) {
          tmp_id=VGrid[i]->draw_foreground_seg_id;
          if (tmp_id!=-1) { //0, 1, 2 , ...
            TileMapForeground[tmp_id]->x=VGrid[i]->x1;
            TileMapForeground[tmp_id]->y=VGrid[i]->y1;

            TileMapForeground[tmp_id]->sprite_paint=CreateCrunchyBitmap(VGRID_SIZE,VGRID_SIZE);
            SelectObject(hdc,TileMapForeground[tmp_id]->sprite_paint);

            GrRect(hdc,0,0,VGRID_SIZE+1,VGRID_SIZE+1,MYCOLOR1);
            for (int k=0;k<VGrid[i]->max_ground_num;k++) {
              int l = VGrid[i]->ground_ids[k];
              Draw1WaterTriFill(hdc,l,VGrid[i]->x1,VGrid[i]->y1);
            }
            SelectObject(hdc,oldbitmap);
            TileMapForeground[tmp_id]->sprite_mask=CreateBitmapMask(TileMapForeground[tmp_id]->sprite_paint,MYCOLOR1,NULL); //create mask
            loading_numerator++;
          }
        }
      }
      loading_tile_grid_prog=5;
    } else {
      loading_tile_grid_prog=5;
    }
  } else if (loading_tile_grid_prog==5) {
    if (is_shadows && game_shadow) {
      yes_shadow=TRUE;
    }
    if (yes_shadow) {
      TileMapShadow = calloc(SHADOW_GRID_NUM,sizeof(ATileMapPaint*));
      for (int i=0;i<SHADOW_GRID_NUM;i++) {
        ATileMapPaint *newTileMapPaint = createTileMapPaint();
        TileMapShadow[i] = newTileMapPaint;
      }

      wchar_t lvl_name[128];
      if (!back_to_menu)
        swprintf(lvl_name,128,L"%s",level_names[level_chosen]);    
      else
        swprintf(lvl_name,128,L"mm_demo2");    


      //HBITMAP tmp_bitmap;
      wchar_t seg_name[72];
      int tms_id=-1;
      for (int i=0;i<VGRID_NUM;i++) {
        if (VGrid[i]->has_shadow) {
          tms_id=VGrid[i]->draw_shadow_seg_id;
          if (tms_id!=-1) { //0, 1, 2 , ...
            swprintf(seg_name,72,L"saves/%s/seg_shadow/%d.bmp", lvl_name,i);
            TileMapShadow[tms_id]->x=VGrid[i]->x1;
            TileMapShadow[tms_id]->y=VGrid[i]->y1;
            TileMapShadow[tms_id]->sprite_paint=//CopyCrunchyBitmap(tmp_bitmap,SRCCOPY);
                LoadRLE8CompressedBitmap(seg_name);
            loading_numerator++;
          }
        }
      }
    }

    flag_begin_drawing_tiles=FALSE;
    time_begin=current_timestamp();
    OLD_GR_WIDTH=0;
    OLD_GR_HEIGHT=0;


    loading_tile_grid_prog=0;
    level_loading=FALSE;
    level_loaded=TRUE; //start game
    if (back_to_menu) {
      back_to_menu=FALSE;
    }
  }
  SelectObject(hdc,oldbitmap);
  DeleteObject(oldbitmap);
}



//bool igto1=FALSE;
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

    //for map editor use only
    VGrid[i]->has_shadow=FALSE;
    VGrid[i]->has_water=FALSE;
    VGrid[i]->not_just_water=FALSE;
    VGrid[i]->checked=FALSE;

    x+=VGRID_SIZE;
    if (x>MAP_WIDTH-VGRID_SIZE) {
      x=0;
      y+=VGRID_SIZE;
    }
  }

}


void InitRDGrid()
{
  int i=0,j=0,k=0,on_grid_id=0,column=0,row=0,
      start_x=0,start_y=0;

  //Begin rendering
  start_x=0;//GR_WIDTH/2+player.cam_move_x-player.cam_mouse_move_x-(RENDER_DIST/2*VGRID_SIZE); //Top left corner of render distance grids to bottom right corner
  start_y=0;//GR_HEIGHT/2+player.cam_move_y-player.cam_mouse_move_y-(RENDER_DIST/2*VGRID_SIZE);

  RD_DYN_WIDTH=GR_WIDTH/VGRID_SIZE+2;
  RD_DYN_HEIGHT=GR_HEIGHT/VGRID_SIZE+2;
  if (RD_DYN_WIDTH>RENDER_WIDTH_MAX) {
    RD_DYN_WIDTH=RENDER_WIDTH_MAX;
  }
  if (RD_DYN_HEIGHT>RENDER_HEIGHT_MAX) {
    RD_DYN_HEIGHT=RENDER_HEIGHT_MAX;
  }

  RDGRID_DYN_NUM=RD_DYN_WIDTH*RD_DYN_HEIGHT;
  for (i=0;i<RDGRID_DYN_NUM;i++) { //all render distance grids from top-left to bottom-right
    RDGrid[i].x=start_x+column*VGRID_SIZE;
    RDGrid[i].y=start_y+row*VGRID_SIZE;
    column++; //Next column
    if (column>=RD_DYN_WIDTH) { //if the column is beyond the render distance
      row++; //move to the next row
      column=0; //go back to first column
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
    NodeGrid[i]->node_fire=FALSE;
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
  int lim=0,delta=0;;
  double gradient=rain_grad_rise/rain_grad_run; //y/x to down-right
  double start_x=0,x=0,y=0;

  if (gradient>0) {
    start_x=-MAP_HEIGHT;
    delta=NODE_SIZE;
  } else {
    start_x=MAP_WIDTH+MAP_HEIGHT;
    delta=-NODE_SIZE;
  }

  while (lim<MAP_WIDTH+MAP_HEIGHT) {
    on_node_grid_id=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
    if (on_node_grid_id!=-1) {
      if (NodeGrid[on_node_grid_id]->node_solid || NodeGrid[on_node_grid_id]->node_no_rain || y>=MAP_HEIGHT) { //solid detected, move to next start_x nodegrid
        start_x+=delta;
        x=start_x;
        y=0;
        lim++;
      } else { //keep going down.
        NodeGrid[on_node_grid_id]->node_no_shade=TRUE;
        y+=NODE_SIZE;
        x=start_x+GetX(y,gradient,0);
      }
    } else {
      y+=NODE_SIZE;
      if (y>=MAP_HEIGHT) {
        start_x+=delta;
        x=start_x;
        y=0;
        lim++;
      } else {
        x=start_x+GetX(y,gradient,0);
      }
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

//for saving level
void TriFillGridType(int gid, int part)
{
  if (Ground[gid]->type==1 || (Ground[gid]->type>=3 && Ground[gid]->type<=8)) {
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
      for (y=smallest_y;y<middle_y;y+=1/*NODE_SIZE*/) {//small to middle
        x_1=GetX(y,gradient_middle1,c_middle1);
        x_2=GetX(y,gradient_largest,c_largest);
        //x_1=stickyTo(x_1,NODE_SIZE);
        //x_2=stickyTo(x_2,NODE_SIZE);
        x1=min(x_1,x_2);
        x2=max(x_1,x_2);
        for (x=x1;x<x2;x+=1/*NODE_SIZE*/) {
          k=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
          if (k!=-1) {
            if (part==0) {
            SetGridLineArray(k,gid);
            } else {
            if (Ground[gid]->type==1) {
              VGrid[k]->has_water=TRUE;
              if (!VGrid[k]->checked) {
                for (int z=0;z<VGrid[k]->max_ground_num;z++) {
                  int z_=VGrid[k]->ground_ids[z];
                  if (z_!=-1) {
                    if (Ground[z_]->type!=1) {
                      VGrid[k]->not_just_water=TRUE;
                      break;
                    }
                  }
                }
                VGrid[k]->checked=TRUE;              
              }
            }
            }
          }
        }
      }
      for (y=middle_y;y<largest_y;y+=1/*NODE_SIZE*/) {//middle to largest
        x_1=GetX(y,gradient_middle2,c_middle2);
        x_2=GetX(y,gradient_largest,c_largest);
        //x_1=stickyTo(x_1,NODE_SIZE);
        //x_2=stickyTo(x_2,NODE_SIZE);
        x1=min(x_1,x_2);
        x2=max(x_1,x_2);
        for (x=x1;x<x2;x+=1/*NODE_SIZE*/) {
          k=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
          if (k!=-1) {
            if (part==0) {
            SetGridLineArray(k,gid);
            } else {
            if (Ground[gid]->type==1) {
              VGrid[k]->has_water=TRUE;
              if (!VGrid[k]->checked) {
                for (int z=0;z<VGrid[k]->max_ground_num;z++) {
                  int z_=VGrid[k]->ground_ids[z];
                  if (z_!=-1) {
                    if (Ground[z_]->type!=1) {
                      VGrid[k]->not_just_water=TRUE;
                      break;
                    }
                  }
                }
                VGrid[k]->checked=TRUE;              
              }
            }
            }
          }
        }
      }
    }
}




//trifill for level loading
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


  if (Ground[gid]->type==1 || (Ground[gid]->type>=3 && Ground[gid]->type<=8)) {
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
  int grid_id;
  for (y=smallest_y;y<middle_y;y+=NODE_SIZE) {//small to middle
    x_1=GetX(y,gradient_middle1,c_middle1);
    x_2=GetX(y,gradient_largest,c_largest);
    x_1=stickyTo(x_1,NODE_SIZE);
    x_2=stickyTo(x_2,NODE_SIZE);
    x1=min(x_1,x_2);
    x2=max(x_1,x_2);
    for (x=x1;x<x2;x+=NODE_SIZE) {
      //Add to existing ground_ids in grid
      grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(grid_id,gid);

      k=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (k!=-1) {
        if (Ground[gid]->type==1) { //trifill water
          NodeGrid[k]->node_water=TRUE;
          NodeGrid[k]->node_no_rain=TRUE;
        } else if (Ground[gid]->type==3 || Ground[gid]->type==5) { //trifill solid
          NodeGrid[k]->node_solid=TRUE;
          NodeGrid[k]->node_no_rain=TRUE;
          NodeGrid[k]->non_web=TRUE; //toggle node_solid to false
        } else if (Ground[gid]->type>=7 && Ground[gid]->type<=8) { //fire
          NodeGrid[k]->node_fire=TRUE;
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
      //Add to existing ground_ids in grid
      grid_id=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(grid_id,gid);

      k=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
      if (k!=-1) {
        if (Ground[gid]->type==1) { //water trifill
          NodeGrid[k]->node_water=TRUE;
          NodeGrid[k]->node_no_rain=TRUE;
        } else if (Ground[gid]->type==3 || Ground[gid]->type==5) {//solid trifill
          NodeGrid[k]->node_solid=TRUE;
          NodeGrid[k]->node_no_rain=TRUE;
          NodeGrid[k]->non_web=TRUE;
        } else if (Ground[gid]->type>=7 && Ground[gid]->type<=8) { //fire trifill
          NodeGrid[k]->node_fire=TRUE;
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
      /*for (int n=0;n<4;n++) {
        double llg_x,llg_y;
        switch (n) {
          case 0:llg_x=x-NODE_SIZE;llg_y=lg_y-NODE_SIZE;break;
          //case 1:llg_x=x;llg_y=lg_y-NODE_SIZE;break;
          case 1:llg_x=x+NODE_SIZE;llg_y=lg_y-NODE_SIZE;break;
          case 2:llg_x=x-NODE_SIZE;llg_y=lg_y+NODE_SIZE;break;
          //case 4:llg_x=x;llg_y=lg_y+NODE_SIZE;break;
          case 3:llg_x=x+NODE_SIZE;llg_y=lg_y+NODE_SIZE;break;
          //case 6:llg_x=x-NODE_SIZE;llg_y=lg_y;break;
          //case 7:llg_x=x+NODE_SIZE;llg_y=lg_y;break;
        }
          lg_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
          if (lg_grid_id!=-1) {
            SetGridLineArray(lg_grid_id,i);
          }
          if (!Ground[i]->is_ghost) { //Not a ghost
            node_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
            if (node_grid_id!=-1) {
              NodeGrid[node_grid_id]->node_solid=TRUE;
              if (i<GROUND_NUM) {
                NodeGrid[node_grid_id]->non_web=TRUE;
              }
            }
          }
        }*/


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
      for (int n=0;n<4;n++) {
          double llg_x,llg_y;
          switch (n) {
            case 0:llg_x=lg_x-NODE_SIZE;llg_y=y-NODE_SIZE;break;
            //case 1:llg_x=lg_x;llg_y=y-NODE_SIZE;break;
            case 1:llg_x=lg_x+NODE_SIZE;llg_y=y-NODE_SIZE;break;
            case 2:llg_x=lg_x-NODE_SIZE;llg_y=y+NODE_SIZE;break;
            //case 4:llg_x=lg_x;llg_y=y+NODE_SIZE;break;
            case 3:llg_x=lg_x+NODE_SIZE;llg_y=y+NODE_SIZE;break;
            //case 6:llg_x=lg_x-NODE_SIZE;llg_y=y;break;
            //case 7:llg_x=lg_x+NODE_SIZE;llg_y=y;break;
          }
          lg_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
          if (lg_grid_id!=-1) {
            SetGridLineArray(lg_grid_id,i);
          }
          if (!Ground[i]->is_ghost) {
            node_grid_id=GetGridId(llg_x,llg_y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
            if (node_grid_id!=-1) {
              NodeGrid[node_grid_id]->node_solid=TRUE;
              if (i<GROUND_NUM) {
                NodeGrid[node_grid_id]->non_web=TRUE;
              }
            }
          }
      }
    }
  }
  if ((Ground[i]->type>=3 && Ground[i]->type<=8) || Ground[i]->type==1) {//triangle
    SetNodeGridAttributes2(i);
  }
  if ((Ground[i]->type>=3 && Ground[i]->type<=8) || Ground[i]->type==1) { //water and fire, etc trifill
    TriFillNodeGridType(i);
  }
}



void InitNodeGridAttributes()
{
  for (int i=0;i<GROUND_NUM;i++) {
    SetNodeGridAttributes(i);
  }
  //render shade
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
  int x,y;
  double dist;
  if (level_loaded) {
  for (int i=0;i<MAP_NODE_NUM;i++) {
    x=NodeGrid[i]->x1+player.cam_x+player.cam_move_x;
    y=NodeGrid[i]->y1+player.cam_y+player.cam_move_y;
    //dist=GetDistance(player.x,player.y,x,y);
    //if (dist<GR_WIDTH) {
      if (NodeGrid[i]->node_no_rain) //solid trifill
        GrCircle(hdc,x,y,3,YELLOW,-1);
      if (NodeGrid[i]->node_no_shade)
        GrCircle(hdc,x,y,3,BLUE,-1);
      //if (NodeGrid[i]->node_solid)
        //GrCircle(hdc,x,y,3,LTRED,-1);
    //}
    /*if (NodeGrid[i]->tmp_wet)
      GrCircle(hdc,NodeGrid[i]->x1+player.cam_x,NodeGrid[i]->y1+player.cam_y,3,BLUE,-1);*/
  }
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



void DrawShadows3(HBITMAP destBitmap, HBITMAP sourceBitmap, BITMAPINFO* bmInfo,int x,int y,bool t)
{
    BITMAP sbitmap;
    GetObject(sourceBitmap, sizeof(BITMAP), &sbitmap);
    BITMAP dbitmap;
    GetObject(destBitmap, sizeof(BITMAP), &dbitmap);


    int adjustedSourceY = sbitmap.bmHeight - 1 - y;
    BYTE *sourcePixels = (BYTE *)sbitmap.bmBits;
    BYTE *rowSource = sourcePixels + (adjustedSourceY*sbitmap.bmWidthBytes);


    int adjustedDestY = dbitmap.bmHeight - 1 - y;
    BYTE *destPixels = (BYTE *)dbitmap.bmBits;
    BYTE *rowDest = destPixels + (adjustedDestY*dbitmap.bmWidthBytes);


    BYTE colorIndex = rowSource[x];
    COLORREF color = RGB(bmInfo->bmiColors[colorIndex].rgbRed,
                     bmInfo->bmiColors[colorIndex].rgbGreen,
                     bmInfo->bmiColors[colorIndex].rgbBlue);

    //GetAdjacent Dest X
    int vgid=GetGridId(x,y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (color!=MYCOLOR1) { //<------ color that shows that it is NOT in non-solid, it is a solid
      photon.in_solid=TRUE;
    }
    if (photon.in_solid && color!=MYCOLOR1) {
      photon.dist_travelled++;
    }

    if (photon.in_solid && color==MYCOLOR1) {
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
              rowDest[x]=0x00;
              if (x > 0) {rowDest[x - 1] = 0x00;}  // Set the left pixel (if within bounds)
              if (x < dbitmap.bmWidth - 1) {rowDest[x + 1] = 0x00;} // Set the right pixel (if within bounds)
              if (adjustedDestY > 0) {BYTE *rowAbove = destPixels + ((adjustedDestY - 1) * dbitmap.bmWidthBytes);rowAbove[x] = 0x00;}   // Set the above pixel (if within bounds)
              if (adjustedDestY < dbitmap.bmHeight - 1) {BYTE *rowBelow = destPixels + ((adjustedDestY + 1) * dbitmap.bmWidthBytes);rowBelow[x] = 0x00;} // Set the below pixel (if within bounds)
              if (vgid!=1) {
                VGrid[vgid]->has_shadow=TRUE;
              }
          }
        } else { //odd
          if (x%2==0) {
            if (y%4!=0 && y%2==0) {
                rowDest[x]=0x00;
                if (x > 0) {rowDest[x - 1] = 0x00;}  // Set the left pixel (if within bounds)
                if (x < dbitmap.bmWidth - 1) {rowDest[x + 1] = 0x00;} // Set the right pixel (if within bounds)
                if (adjustedDestY > 0) {BYTE *rowAbove = destPixels + ((adjustedDestY - 1) * dbitmap.bmWidthBytes);rowAbove[x] = 0x00;}   // Set the above pixel (if within bounds)
                if (adjustedDestY < dbitmap.bmHeight - 1) {BYTE *rowBelow = destPixels + ((adjustedDestY + 1) * dbitmap.bmWidthBytes);rowBelow[x] = 0x00;} // Set the below pixel (if within bounds)
                if (vgid!=1) {
                  VGrid[vgid]->has_shadow=TRUE;
                }
            }
          }
        }
      } else {
          rowDest[x]=0x00;
          if (x > 0) {rowDest[x - 1] = 0x00;}  // Set the left pixel (if within bounds)
          if (x < dbitmap.bmWidth - 1) {rowDest[x + 1] = 0x00;} // Set the right pixel (if within bounds)
          if (adjustedDestY > 0) {BYTE *rowAbove = destPixels + ((adjustedDestY - 1) * dbitmap.bmWidthBytes);rowAbove[x] = 0x00;}   // Set the above pixel (if within bounds)
          if (adjustedDestY < dbitmap.bmHeight - 1) {BYTE *rowBelow = destPixels + ((adjustedDestY + 1) * dbitmap.bmWidthBytes);rowBelow[x] = 0x00;} // Set the below pixel (if within bounds)
          if (vgid!=1) {
            VGrid[vgid]->has_shadow=TRUE;
          }
      } 
      if (photon.dist_travelled>20) {
        photon.passed_solid=FALSE;
        photon.dist_travelled=0;
      }
    }
}


void CreatePlatformShadowBitmap2(HDC hdc,HBITMAP hBitmap, double rise, double run, COLORREF clrBack)
{
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

  //Retrieve Bitmap Info
  BITMAP bmp;
  if (!GetObject(hBitmap,sizeof(BITMAP),&bmp)) { //assign 8bit bitmap to bmp
    return;
  }

  //HBITMAP tmp_8bitbitmap=CopyCrunchyBitmap(hBitmap,SRCCOPY);

  int width = bmp.bmWidth;
  int height = bmp.bmHeight;
  int bpp = 1;

  //Create DIB Section for Source Bitmap
  BITMAPINFO* bmInfo = (BITMAPINFO*)alloca(offsetof(BITMAPINFO, bmiColors[256]));
  bmInfo->bmiHeader.biSize = sizeof (bmInfo->bmiHeader);
  bmInfo->bmiHeader.biWidth = width;
  bmInfo->bmiHeader.biHeight = height;
  bmInfo->bmiHeader.biPlanes = 1;
  bmInfo->bmiHeader.biBitCount = 8;
  bmInfo->bmiHeader.biCompression = BI_RGB;
  bmInfo->bmiHeader.biSizeImage = 0;
  bmInfo->bmiHeader.biXPelsPerMeter = 14173;
  bmInfo->bmiHeader.biYPelsPerMeter = 14173;

  Init8BitRGBColorsDefault(bmInfo->bmiColors);
  bmInfo->bmiHeader.biClrUsed = 0;
  bmInfo->bmiHeader.biClrImportant = 0;


  //
  //
  //draw shadow over bitmap
    photon.rise = rise;
    photon.run = run;

    // Set mapping mode so that +ve y axis is upwards
    HDC hMemDest = CreateCompatibleDC(hdc);

    // Create a compatible bitmap for the shadow shader
    //int max_size=max(MAP_WIDTH,MAP_HEIGHT);
    map_platforms_shadow_shader = CreateCrunchyBitmap(MAP_WIDTH,MAP_HEIGHT);

    // Select the bitmaps into the device contexts
    SelectObject(hMemDest, map_platforms_shadow_shader);
    GrRect(hMemDest,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,clrBack);
    DeleteDC(hMemDest);


    double gradient=photon.rise / photon.run;
    double start_x=0, x=0, y=0, _start_x=0;


    photon.dist_travelled=0;
    photon.passed_solid=FALSE;
    photon.in_solid=FALSE;


    int lim=0;
    int delta=0;

    if (gradient>0) {
      _start_x=-MAP_HEIGHT;
      delta=1;
    } else { //negative gradient
      _start_x=MAP_WIDTH+MAP_HEIGHT;
      delta=-1;
    }

    start_x=_start_x;
    x=start_x;
    while (lim<MAP_WIDTH+MAP_HEIGHT) {
      if (y>=MAP_HEIGHT) { //solid detected, move to next start_x nodegrid
        start_x+=delta;
        x=start_x;
        y=0;
        photon.dist_travelled=0;
        photon.passed_solid=FALSE;
        photon.in_solid=FALSE;
        lim++;
      } else { //keep going down.
        if (x>=0 && y>=0 && x<MAP_WIDTH && y<MAP_HEIGHT) {
          DrawShadows3(map_platforms_shadow_shader,map_platforms_sprite,bmInfo,x,y,TRUE);
        }
        y++;
        x=start_x+GetX(y,gradient,0);
      }
    }
    photon.dist_travelled=0;
    photon.passed_solid=FALSE;
    photon.in_solid=FALSE;
    lim=0;
    start_x=_start_x;
    x=_start_x,y=MAP_HEIGHT;
    while (lim<MAP_WIDTH+MAP_HEIGHT) {
      if (y<=0) { //solid detected, move to next start_x nodegrid
        start_x+=delta;
        x=start_x;
        y=MAP_HEIGHT;
        photon.dist_travelled=0;
        photon.passed_solid=FALSE;
        photon.in_solid=FALSE;
        lim++;
      } else { //keep going up.
        if (x>=0 && y>=0 && x<MAP_WIDTH && y<MAP_HEIGHT) {
          DrawShadows3(map_platforms_shadow_shader,map_platforms_sprite,bmInfo,x,y,FALSE);
        }
        y--;
        x=start_x+GetX(y,gradient,0);
      }
    }
    photon.dist_travelled=0;
    photon.passed_solid=FALSE;
    photon.in_solid=FALSE;

    //cleanup
    free(bmInfo);
}


