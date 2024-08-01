
//global variables -- in struct now :)
struct MapEditor
{
  int cursor_x;
  int cursor_y;

  //overal
  int selected_option; //0:ground, 1:player, 2:enemy, 3:enemy_type, 4:background&pallette


  //Ground 
  int selected_ground_option; //0:ground_id, 1:type, 2:color, 3:is_ghost
  int selected_ground_id; //0 -> GROUND_NUM
  int selected_ground_pivot; //0:x1y1 ,1:x2y2, 2:x3y3 


  bool is_ground_txt_typing;
  int typing_ground_txt_pos;
  wchar_t typing_ground_txt[512]; //same ant as ground
  //wchar_t typing_ground_txt2[512]; //same ant as ground //inbetween txt

} MapEditor;


typedef struct MEEnemy
{
  double x,y;
  int type;
} AMEEnemy;


AMEEnemy *createMEEnemy()
{
  AMEEnemy *toReturn = malloc(sizeof(AMEEnemy));
  return toReturn;
}


void freeMEEnemy(AMEEnemy *myEnemy)
{
  if (myEnemy)
    free(myEnemy);
}

AMEEnemy **MEEnemy;



typedef struct MEEnemySprite
{
  HBITMAP sprite_1;
} AMEEnemySprite;


AMEEnemySprite *createMEEnemySprite()
{
  AMEEnemySprite *toReturn = malloc(sizeof(AMEEnemySprite));
  return toReturn;
}


void freeMEEnemySprite(AMEEnemySprite *myEnemySprite)
{
  if (myEnemySprite)
    free(myEnemySprite);
}
AMEEnemySprite** MEEnemySprite;


void CleanUpMEEnemySprites()
{
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    if (MEEnemySprite[i]->sprite_1!=NULL) {
      DeleteObject(MEEnemySprite[i]->sprite_1);
      MEEnemySprite[i]->sprite_1=NULL;
    }
  }
}


/*Set Enemy Type will be in the stack to be consistent with Saved Enemy Type being in the stack*/



//Map Editor Init()
//
void SetMENodeGridAttributes2(int i)
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
      if (y1<Ground[i]->y3) { //y1 is lower than y3
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



void SetMENodeGridAttributes(int i)
{
  int lg_grid_id=0,node_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (-1<Ground[i]->gradient && Ground[i]->gradient<1) { // y=mx+c
    for (x=Ground[i]->x1;x<Ground[i]->x2;x++) {
      lg_y=x*Ground[i]->gradient+Ground[i]->c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      SetGridLineArray(lg_grid_id,i);
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
    }
  }
  if (Ground[i]->type==3) {//triangle
    SetMENodeGridAttributes2(i);
  }
}





void InitRDGrid()
{
  int i=0,j=0,k=0,on_grid_id=0,column=0,row=0,
      start_x=0,start_y=0;

  /*for (i=0;i<rendered_grid_num;i++) {
    VGrid[render_grids[i]]->within_render_distance=FALSE; //all rendered-grids are no-longer within render distance
    render_grids[i]=-1;  //unrender all rendered grids
  }*/
  for (i=0;i<rendered_ground_num;i++) {
    Ground[render_grounds[i]]->within_render_distance=FALSE; //All rendered-grounds are no-longer within render distance
    render_grounds[i]=-1; //unrender all rendered grounds
  }

  rendered_ground_num=0;
  //rendered_grid_num=0;

  //Begin rendering
  start_x=player.cam_move_x-(RENDER_DIST/2*VGRID_SIZE); //Top left corner of render distance grids to bottom right corner
  start_y=player.cam_move_y-(RENDER_DIST/2*VGRID_SIZE);
  //"What happens when you lose everything? You just start again. Start all over again" - Maximo Park
  for (i=0;i<RDGRID_NUM;i++) { //all render distance grids from top-left to bottom-right
    RDGrid[i].x=start_x+column*VGRID_SIZE;
    RDGrid[i].y=start_y+row*VGRID_SIZE;
    if (0<RDGrid[i].x && RDGrid[i].x<MAP_WIDTH && //render distance grid is within range
        0<RDGrid[i].y && RDGrid[i].y<MAP_HEIGHT) {

      //vgrid
      on_grid_id=GetGridId(RDGrid[i].x,RDGrid[i].y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);//get grid id based on renderdistance grid axes
      //VGrid[on_grid_id]->within_render_distance=TRUE; //append grid to render_grids array
      //render_grids[rendered_grid_num]=on_grid_id;//cannot be i (what if out of bounds)
      //rendered_grid_num++; //count amount of grids rendered

      //grounds
      for (j=0;j<VGrid[on_grid_id]->max_ground_num;j++) { //fetch all grounds that are occupying the grid
	    k=VGrid[on_grid_id]->ground_ids[j]; //ground_id in grid
 	    if (!Ground[k]->within_render_distance) { //ground is now within render distance
	      Ground[k]->within_render_distance=TRUE;
          render_grounds[rendered_ground_num]=k; //append ground to render_grounds array
	      rendered_ground_num++; //count number of rendered grounds
	    } 
      }

    }
    column++; //Next column
    if (column>=RENDER_DIST) { //if the column is beyond the render distance
      row++; //move to the next row
      column=0; //go back to first column
    }
  } 
}


bool YesInitRDGrid()
{
  if (GRID_SIZE*2<player.cam_move_x && player.cam_move_x<MAP_WIDTH-GRID_SIZE*2) {
    if (player.cam_move_x<RDGrid[0].x+GRID_SIZE*2 || player.cam_move_x>RDGrid[RENDER_DIST-1].x-GRID_SIZE*2) {
      return TRUE;
    }
  }
  if (GRID_SIZE*2<player.cam_move_y && player.cam_move_y<MAP_HEIGHT-GRID_SIZE*2) {
    if (player.cam_move_y<RDGrid[0].y+GRID_SIZE*2 || player.cam_move_y>RDGrid[RDGRID_NUM-1].y-GRID_SIZE*2) {
      return TRUE;
    }
  }
  return FALSE;
}



void InitMENodeGridAttributes()
{
  for (int i=0;i<GROUND_NUM;i++) {
    SetMENodeGridAttributes(i);
  }
}



void DestroyMEGround2(int i)
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
        UnSetGridLineArray(lg_grid_id,i);
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
        UnSetGridLineArray(lg_grid_id,i);
      }
    }
  }
}




void DestroyMEGround(int i)
{
  int lg_grid_id=0,x=0,y=0,min=0,max=0;
  double lg_x=0,lg_y=0;
  if (-1<Ground[i]->gradient && Ground[i]->gradient<1) {
    for (x=Ground[i]->x1;x<Ground[i]->x2;x++) {
      lg_y=x*Ground[i]->gradient+Ground[i]->c;
      lg_grid_id=GetGridId(x,lg_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
      UnSetGridLineArray(lg_grid_id,i);
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
    }
  }
  if (Ground[i]->type==3) {//triangle ground
    DestroyMEGround2(i);
  }

  Ground[i]->health=-1;
  //Ground[i]->x1=Ground[i]->y1=Ground[i]->x2=Ground[i]->y2=Ground[i]->x3=Ground[i]->y3=-20;
}




void InitMapEditorEnemy()
{
  for (int i=0;i<ENEMY_NUM;i++) {
    MEEnemy[i]->x=saved_enemy_x[i];
    MEEnemy[i]->y=saved_enemy_y[i];
    MEEnemy[i]->type=saved_enemy_type[i];
  }



  //Load 10 Sprites
  //init set enemy type
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    set_enemy_type_speed[i]=saved_enemy_type_speed[i];
    set_enemy_type_bullet_speed[i]=saved_enemy_type_bullet_speed[i];
    set_enemy_type_species[i]=saved_enemy_type_species[i];
    set_enemy_type_follow_range[i]=saved_enemy_type_follow_range[i];
    set_enemy_type_unchase_range[i]=saved_enemy_type_unchase_range[i];
    set_enemy_type_chase_range[i]=saved_enemy_type_chase_range[i];
    set_enemy_type_color[i]=saved_enemy_type_color[i];
    set_enemy_type_speed_multiplier[i]=saved_enemy_type_speed_multiplier[i];
    set_enemy_type_health[i]=saved_enemy_type_health[i];
    set_enemy_type_shoot_at_player_range[i]=saved_enemy_type_shoot_at_player_range[i];
    set_enemy_type_aim_rand[i]=saved_enemy_type_aim_rand[i];
    set_enemy_type_bullet_cooldown[i]=saved_enemy_type_bullet_cooldown[i];
    set_enemy_type_bullet_fire_cooldown[i]=saved_enemy_type_bullet_fire_cooldown[i];
    set_enemy_type_bullet_fire_at_once[i]=saved_enemy_type_bullet_fire_at_once[i];
    set_enemy_type_bullet_length[i]=saved_enemy_type_bullet_length[i];
    set_enemy_type_bullet_damage[i]=saved_enemy_type_bullet_damage[i];
    set_enemy_type_bullet_speed_multiplier[i]=saved_enemy_type_bullet_speed_multiplier[i];
    set_enemy_type_bullet_range[i]=saved_enemy_type_bullet_range[i];
    set_enemy_type_bullet_color[i]=saved_enemy_type_bullet_color[i];
    set_enemy_type_bullet_graphics_type[i]=saved_enemy_type_bullet_graphics_type[i];
    set_enemy_type_time_breaker_rare[i]=saved_enemy_type_time_breaker_rare[i];
    set_enemy_type_time_breaker_length[i]=saved_enemy_type_time_breaker_length[i];
    set_enemy_type_time_breaker_immune[i]=saved_enemy_type_time_breaker_immune[i];
  }


  //Generate Sprites for 10 enemy types
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    if (set_enemy_type_species[i]==0) {
      MEEnemySprite[i]->sprite_1=RotateSprite(NULL, enemy1_sprite_1,0,LTGREEN,BLACK,color_arr[set_enemy_type_color[i]],-1);
    } else {
      MEEnemySprite[i]->sprite_1=RotateSprite(NULL, enemy2_sprite_1,0,LTGREEN,BLACK,color_arr[set_enemy_type_color[i]],-1);
    }
  }
}




void InitMapEditorPlayer()
{
  //
  InitPlayer(); //but i only use player.x and player.y

//  alloc_enemy_once=TRUE;
  player_load_color=player_color;
  player_load_iris_color=player_iris_color;
  player_load_pupil_color=player_pupil_color;
  player_bullet_color=WHITE;

  if (IsInvertedBackground()) { //invert player color if background is inverted
    player_load_color=COLORS_NUM-player_color-1;
    player_load_iris_color=COLORS_NUM-player_iris_color-1;
    player_load_pupil_color=COLORS_NUM-player_pupil_color-1;

    player_bullet_color=BLACK;
  }

  player.cam_move_x=0;
  player.cam_move_y=0;

  player.cam_x=-player.x;
  player.cam_y=-player.y;
}



void InitMapEditor(HDC hdc)
{
  rendered_ground_num=0;

  MapEditor.selected_option=0;

  MapEditor.selected_ground_option=0;
  MapEditor.selected_ground_id=0;
  MapEditor.selected_ground_pivot=0;
  MapEditor.selected_ground_option=0;


  MapEditor.is_ground_txt_typing=FALSE;
  MapEditor.typing_ground_txt_pos=0;
  for (int i=0;i<512;i++)
    MapEditor.typing_ground_txt[i]='\0';
  //swprintf(MapEditor.typing_ground_txt,512,L"%s",Ground[MapEditor.selected_ground_id]->text);



  //for (int i=0;i<MAX_GROUND_NUM;i++)
    //render_grounds[i]=-1;
  render_grounds=calloc(GROUND_NUM,sizeof(int));
  for (int i=0;i<GROUND_NUM;i++)
    render_grounds[i]=-1;

  //Create Class Objects()
  Ground = calloc(GROUND_NUM,sizeof(AGround*));
  VGrid = calloc(VGRID_NUM,sizeof(struct AVGrid*));
  MEEnemy = calloc(ENEMY_NUM,sizeof(AMEEnemy*));
  MEEnemySprite = calloc(ENEMY_TYPE_NUM,sizeof(AMEEnemySprite*));

  for (int i=0;i<GROUND_NUM;i++) {
    AGround *newGround = createGround(VGRID_NUM);
    Ground[i] = newGround;//malloc(sizeof(struct GroundLine));
  }
    

  for (int i=0;i<VGRID_NUM;i++) {
    AVGrid *newVGrid = createVGrid();
    VGrid[i] = newVGrid;
  }

  //Special --> for enemy only
  for (int i=0;i<ENEMY_NUM;i++) {
    AMEEnemy *newMEEnemy = createMEEnemy();
    MEEnemy[i] = newMEEnemy;
  }

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    AMEEnemySprite *newMEEnemySprite = createMEEnemySprite();
    MEEnemySprite[i] = newMEEnemySprite;
  }


  //Init
  InitGrid();
  InitGround(FALSE);
  InitMENodeGridAttributes();
  InitMapEditorEnemy();
  InitMapEditorPlayer();
  InitRDGrid();


  BitmapPalette(hdc,map_platforms_sprite,rgbColorsDefault);

  if (!run_after_once) {
    run_after_once=TRUE;
  } else {
    PlaySound(NULL, NULL, SND_ASYNC);
  }
}






void InitLevelMapEditor(HWND hwnd, HDC hdc)
{
  wchar_t txt[128];
  swprintf(txt,128,L"saves/%s/level.txt",level_names[level_chosen]);
  LoadSave(txt,FALSE); //load saves
  srand(time(NULL));
  timeBeginPeriod(1);





  InitMapEditor(hdc);

  //Load Player Cosmetics
  DeleteObject(player.sprite_1);

  player.sprite_1 = RotateSprite(NULL, player.osprite_1,0,-1,LTRED,draw_color_arr[player_load_iris_color],-1);

  //Load Player Sprites
  player.sprite_1_cache = RotateSprite(NULL, player.sprite_1,player.sprite_angle,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  //moon sprite
  DeleteObject(moon_sprite_cache);
  HBITMAP tmp_moon_sprite=CopyCrunchyBitmap(moon_sprite,NOTSRCCOPY);
  moon_sprite_cache=RotateSprite(NULL, tmp_moon_sprite,0,PURPLE,BLACK,BLACK,-1);
  DeleteObject(tmp_moon_sprite);

  DeleteObject(mouse_cursor_sprite_cache);
  DeleteObject(mouse_cursor_sprite_cache2);
  DeleteObject(mouse_cursor_sprite_iris_cache);
  DeleteObject(mouse_cursor_sprite_iris_cache2);
  DeleteObject(mouse_cursor_sprite_pupil_cache);
  DeleteObject(mouse_cursor_sprite_pupil_cache2);

  mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);
  mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_load_color],-1);

  mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,draw_color_arr[player_load_iris_color]);
  mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,draw_color_arr[player_load_iris_color]);

  mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,draw_color_arr[player_load_pupil_color]);
  mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,draw_color_arr[player_load_pupil_color]);

  main_menu_chosen=4;
  in_map_editor=TRUE;
  level_loaded=TRUE;

  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;
}


//Logic
void MEmove_x(double x)
{
  player.cam_x-=x;
  //limiter x
  if (player.cam_move_x<0)
    player.cam_x=-5;
  else if (player.cam_move_x>MAP_WIDTH)
    player.cam_x=-MAP_WIDTH+5;
}

void MEmove_y(double y)
{
  player.cam_y-=y;
  //limiter y
  if (player.cam_move_y<0)
    player.cam_y=-5;
  else if (player.cam_move_y>MAP_HEIGHT)
    player.cam_y=-MAP_HEIGHT+5;
}


void MapEditorAct()
{
  //movement x,y
  if (!MapEditor.is_ground_txt_typing) {//only move when not typing
    if (player.rst_left) {
      MEmove_x(-5);
    }

    if (player.rst_right) {
      MEmove_x(5);
    }

    if (player.rst_up) {
      MEmove_y(-5);
    } 

    if (player.rst_down) {
      MEmove_y(5);
    }
  }

  //map editor cursor
  MapEditor.cursor_x=mouse_x+player.cam_move_x-GR_WIDTH/2;
  MapEditor.cursor_y=mouse_y+player.cam_move_y-GR_HEIGHT/2;

  //lvl maker movement limiter
  player.cam_move_x=-player.cam_x;
  player.cam_move_y=-player.cam_y;

  if (level_loaded) {
    if (YesInitRDGrid())
      InitRDGrid();


    Click();
    if (player.right_click_hold_timer==62) {
      if (Ground[MapEditor.selected_ground_id]->type==3) { //trifill
        MapEditor.selected_ground_pivot=LimitValue(MapEditor.selected_ground_pivot+1,0,3);
      } else {
        MapEditor.selected_ground_pivot=LimitValue(MapEditor.selected_ground_pivot+1,0,2);
      }
    }
    if (player.attack_rst) { //release mouse
      player.attack_rst=FALSE;
      if (!IsOutOfBounds(MapEditor.cursor_x,MapEditor.cursor_y,1,MAP_WIDTH,MAP_HEIGHT)) {
        DestroyMEGround(MapEditor.selected_ground_id);
        switch (MapEditor.selected_ground_pivot) {
          case 0:          
            Ground[MapEditor.selected_ground_id]->x1=MapEditor.cursor_x;
            Ground[MapEditor.selected_ground_id]->y1=MapEditor.cursor_y;
            break;
          case 1:
            Ground[MapEditor.selected_ground_id]->x2=MapEditor.cursor_x;
            Ground[MapEditor.selected_ground_id]->y2=MapEditor.cursor_y;
            break;
          case 2:
            Ground[MapEditor.selected_ground_id]->x3=MapEditor.cursor_x;
            Ground[MapEditor.selected_ground_id]->y3=MapEditor.cursor_y;
            break;
        }

        //swap when axis overtake
        int i=MapEditor.selected_ground_id;
        int tmp_saved_ground_x1=Ground[i]->x1;
        int tmp_saved_ground_y1=Ground[i]->y1;
        if (Ground[i]->x2<=Ground[i]->x1) {//x1 is less than x2, swap
          Ground[i]->x1=Ground[i]->x2;
          Ground[i]->y1=Ground[i]->y2;
          Ground[i]->x2=tmp_saved_ground_x1;
          Ground[i]->y2=tmp_saved_ground_y1;
          if (MapEditor.selected_ground_pivot==1)//2nd pivot, right pivot
            MapEditor.selected_ground_pivot=0;
          else
            MapEditor.selected_ground_pivot=1;
        }
        if (Ground[i]->x3==Ground[i]->x1 || Ground[i]->x3==Ground[i]->x2) {
	      Ground[i]->x3++;
        }


        if (Ground[i]->type==3) {//trifill
	      if (Ground[i]->y1==Ground[i]->y2) {
	        Ground[i]->y2++;
          }
        }
        if (Ground[i]->y3==Ground[i]->y1) {
	      Ground[i]->y3+=2;
        }
        if (Ground[i]->y3==Ground[i]->y2) {
	      Ground[i]->y3+=2;
        }



        SetGround(MapEditor.selected_ground_id);
        SetMENodeGridAttributes(MapEditor.selected_ground_id);
        InitRDGrid();
      }
    }
  }
}


void CleanupMapEditorAll()
{
    level_loaded=FALSE;


    //for (int i=0;i<rendered_ground_num;i++)
      //render_grounds[i]=-1;
    free(render_grounds);

    //CleanUpPlayer(); //clean up all sprites
    if (player.sprite_1_cache!=NULL) { //clean up 1 sprite
      DeleteObject(player.sprite_1_cache);
    }

    CleanUpMEEnemySprites();
    CleanupPlayerAttributes();
    save_level[0]='\0';
    call_help_timer=0;


    //free saved enemy
    //free saved grounds pointer & Ground

    free(saved_ground_is_ghost);
    free(saved_ground_color);
    free(saved_ground_type);


    free(saved_enemy_type);
    free(saved_enemy_x);
    free(saved_enemy_y);


    free(saved_ground_x1);
    free(saved_ground_y1);
    free(saved_ground_x2);
    free(saved_ground_y2);
    free(saved_ground_x3);
    free(saved_ground_y3);

    rendered_ground_num=0;
    for (int i=0;i<GROUND_NUM;i++) {
      free(saved_ground_text[i]);
    }
    free(saved_ground_text);



    for (int i=0;i<GROUND_NUM;i++) {//free objects
      freeGround(Ground[i]);
    }

    for (int i=0;i<VGRID_NUM;i++) {
      freeVGrid(VGrid[i]); //free actual obj
    }


    for (int i=0;i<ENEMY_NUM;i++) {
      freeMEEnemy(MEEnemy[i]);
    }

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      freeMEEnemySprite(MEEnemySprite[i]);
    }


    free(Ground); //free pointer to pointers
    free(VGrid); //free pointer to pointers
    free(MEEnemy);
    free(MEEnemySprite);
    //printf("===All pointers freed\n");


    LoadMainMenuBackground();

    DeleteObject(moon_sprite_cache);
    moon_sprite_cache=RotateSprite(NULL, moon_sprite,0,LTGREEN,BLACK,BLACK,-1);

    DeleteObject(mouse_cursor_sprite_cache);
    DeleteObject(mouse_cursor_sprite_cache2);
    DeleteObject(mouse_cursor_sprite_iris_cache);
    DeleteObject(mouse_cursor_sprite_iris_cache2);
    DeleteObject(mouse_cursor_sprite_pupil_cache);
    DeleteObject(mouse_cursor_sprite_pupil_cache2);


    mouse_cursor_sprite_cache=RotateSprite(NULL, mouse_cursor_sprite,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);
    mouse_cursor_sprite_cache2=RotateSprite(NULL, mouse_cursor_sprite2,0,LTGREEN,BLACK,draw_color_arr[player_color],-1);


    mouse_cursor_sprite_iris_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTBLUE,draw_color_arr[player_iris_color]);
    mouse_cursor_sprite_iris_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTBLUE,draw_color_arr[player_iris_color]);


    mouse_cursor_sprite_pupil_cache=RotateSpriteExclude(NULL, mouse_cursor_sprite,0,LTRED,draw_color_arr[player_pupil_color]);
    mouse_cursor_sprite_pupil_cache2=RotateSpriteExclude(NULL, mouse_cursor_sprite2,0,LTRED,draw_color_arr[player_pupil_color]);


    main_menu_chosen=0;
    back_to_menu=FALSE;
    run_after_once=FALSE;
    //clean_up_sound=TRUE;
    in_map_editor=FALSE;
}







//Graphics
void DrawMapEditorPlatforms(HDC hdc)
{
  //Draw type 3
  int c;
  int i;
  int player_cam_move_x=+player.cam_x+GR_WIDTH/2;
  int player_cam_move_y=+player.cam_y+GR_HEIGHT/2;
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
    if (Ground[i]->type==3) { 
	  c=color_arr[Ground[i]->color_id];
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
        if (!IsInvertedBackground()) {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,FALSE,0);
        } else {
	      DrawTriFill(hdc,c,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
				Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x2+player.cam_x+GR_WIDTH/2,
				Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
				Ground[i]->x3+player.cam_x+GR_WIDTH/2,
				Ground[i]->y3+player.cam_y+GR_HEIGHT/2,TRUE,HS_BDIAGONAL);
        }
      }
    }
    }
  }


  //Draw type 0
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
    if (Ground[i]->type==0) { //draw ground type 1
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
          
          GrLine(hdc,
                Ground[i]->x1+player.cam_x+GR_WIDTH/2,
                Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
                Ground[i]->x2+player.cam_x+GR_WIDTH/2,
                Ground[i]->y2+player.cam_y+GR_HEIGHT/2,
                color_arr[Ground[i]->color_id]);

      }
    }
    }
  }




  //Draw type 2 == Text
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
    if (Ground[i]->type==2) { 
      if (!IsOutOfBounds(Ground[i]->x1,Ground[i]->y1,1,MAP_WIDTH,MAP_HEIGHT) &&
          !IsOutOfBounds(Ground[i]->x2,Ground[i]->y2,1,MAP_WIDTH,MAP_HEIGHT)) {
	    GrPrintW(hdc,
            Ground[i]->x1+player.cam_x+GR_WIDTH/2,
            Ground[i]->y1+player.cam_y+GR_HEIGHT/2,
            Ground[i]->text,
            "",
            color_arr[Ground[i]->color_id],
            16,
            FALSE,
            yes_unifont);
      }
    }
    }
  }


  //Print ground details
  char print_ground_id[4];
  for (int k=0;k<rendered_ground_num;k++) {
    i=render_grounds[k];
    if (i!=-1) {
      c=Highlight((i==MapEditor.selected_ground_id),BLACK,LTPURPLE);
      sprintf(print_ground_id,"%d",i);
      GrPrint(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y-16,print_ground_id,c);
      if (i==MapEditor.selected_ground_id) {
        c=Highlight((MapEditor.selected_ground_pivot==0),BLACK,LTPURPLE);
        GrCircle(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y,6,c,-1);
        c=Highlight((MapEditor.selected_ground_pivot==1),BLACK,LTPURPLE);
        GrCircle(hdc,Ground[i]->x2+player_cam_move_x,Ground[i]->y2+player_cam_move_y,6,c,-1);
        if (Ground[i]->type==3) {
          c=Highlight((MapEditor.selected_ground_pivot==2),BLACK,LTPURPLE);
          GrCircle(hdc,Ground[i]->x3+player_cam_move_x,Ground[i]->y3+player_cam_move_y,6,c,-1);
        }

        switch (MapEditor.selected_ground_pivot) {
          case 0:
            GrCircle(hdc,Ground[i]->x1+player_cam_move_x,Ground[i]->y1+player_cam_move_y,8,LTPURPLE,-1);
            break;
          case 1:
            GrCircle(hdc,Ground[i]->x2+player_cam_move_x,Ground[i]->y2+player_cam_move_y,8,LTPURPLE,-1);
            break;
          case 2:
            GrCircle(hdc,Ground[i]->x3+player_cam_move_x,Ground[i]->y3+player_cam_move_y,8,LTPURPLE,-1);
            break;
        }
      }
    }
  }

}


void DrawMapEditorEnemy(HDC hdc)
{
  for (int i=0;i<ENEMY_NUM;i++) {
    char txt_i[4];
    int type=MEEnemy[i]->type;
    sprintf(txt_i,"%d",i);
    if (set_enemy_type_species[type]==0)
      GrPrint(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-32+GR_HEIGHT/2,txt_i,color_arr[set_enemy_type_color[type]]);
    else
      GrPrint(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y-64+GR_HEIGHT/2,txt_i,color_arr[set_enemy_type_color[type]]);


    GrSprite(hdc,MEEnemy[i]->x+player.cam_x+GR_WIDTH/2,MEEnemy[i]->y+player.cam_y+GR_HEIGHT/2,MEEnemySprite[type]->sprite_1,FALSE);
  }
}



void DrawMapEditorPlayer(HDC hdc)
{
  GrSprite(hdc,player.x+player.cam_x+GR_WIDTH/2,player.y+player.cam_y+GR_HEIGHT/2,player.sprite_1_cache,FALSE);
}


void DrawMapEditorUI(HDC hdc)
{
  //Print cursor Axis
  char axis_x[16];
  char axis_y[16];
  sprintf(axis_x,"cam_x:%1.0f",player.cam_move_x);
  sprintf(axis_y,"cam_y:%1.0f",player.cam_move_y);
  GrPrint(hdc,mouse_x,mouse_y+32+8,axis_x,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+32+9,axis_x,WHITE);

  GrPrint(hdc,mouse_x,mouse_y+50+8,axis_y,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+50+9,axis_y,WHITE);

  sprintf(axis_x,"cursor_x:%d",MapEditor.cursor_x);
  sprintf(axis_y,"cursor_y:%d",MapEditor.cursor_y);
  GrPrint(hdc,mouse_x,mouse_y+68+8,axis_x,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+68+9,axis_x,WHITE);
  GrPrint(hdc,mouse_x,mouse_y+82+8,axis_y,BLACK);
  //GrPrint(hdc,mouse_x+1,mouse_y+82+9,axis_y,WHITE);


  int c;
  if (level_loaded) {

    c = Highlight((MapEditor.selected_ground_option==0),BLACK,LTPURPLE);
    GrPrint(hdc,8,16,"GROUNDS:",c);
    char print_ground_id[8];
    sprintf(print_ground_id,"<%d>",MapEditor.selected_ground_id);
    GrPrint(hdc,8*11,16,print_ground_id,c);


    //type
    c = Highlight((MapEditor.selected_ground_option==1),BLACK,LTPURPLE);
    GrPrint(hdc,8,34,"Type:",c);
    char print_ground_type[8];
    sprintf(print_ground_id,"<%d>",Ground[MapEditor.selected_ground_id]->type);
    GrPrint(hdc,8*11,34,print_ground_id,c);

    //color
    c = Highlight((MapEditor.selected_ground_option==2),BLACK,LTPURPLE);
    GrPrint(hdc,8,52,"Color:",c);
    GrPrint(hdc,8*11,52,"<    >",c);

    if (Ground[MapEditor.selected_ground_id]->color_id!=0) {
      GrRect(hdc,8*12+1,52,16,16,BLACK);
    } else {
      GrRect(hdc,8*12+1,52,16,16,WHITE);
    }

    GrRect(hdc,8*12+2+1,52+2,12,12,draw_color_arr[Ground[MapEditor.selected_ground_id]->color_id]);

    //is_ghost
    c = Highlight((MapEditor.selected_ground_option==3),BLACK,LTPURPLE);
    GrPrint(hdc,8,70,"IsGhost:",c);
    if (Ground[MapEditor.selected_ground_id]->is_ghost) {
      GrPrint(hdc,8*11,70,"<TRUE>",c);
    } else {
      GrPrint(hdc,8*11,70,"<FALSE>",c);
    }



    wchar_t duplicate_txt_visual[513];
    if (Ground[MapEditor.selected_ground_id]->type==2) { //Ground text, show gui
      if (!MapEditor.is_ground_txt_typing) {
        GrPrint(hdc,8,86,"[Enter]: Begin Typing",GREEN);
      } else {
        GrRect(hdc,0,86+16*2,GR_WIDTH,GR_HEIGHT-86-16*2,BLACK);
        GrPrint(hdc,8,86+16*2,"[ESC]: Exit and Save.  [SHIFT_ESC]: Abort.  [BACKSPACE]: Backspace",GREEN);
      }
      
      if (frame_tick%FPS>10) {
        //swprintf(duplicate_txt_visual,MapEditor.typing_ground_txt_pos+2,L"%s*",MapEditor.typing_ground_txt);
        for (int i=0;i<MapEditor.typing_ground_txt_pos;i++) {
          duplicate_txt_visual[i]=MapEditor.typing_ground_txt[i];
        }
        duplicate_txt_visual[MapEditor.typing_ground_txt_pos]='_';
        for (int i=MapEditor.typing_ground_txt_pos+1;i<513;i++) {
          duplicate_txt_visual[i]=0;
        }
        GrPrintW(hdc,8,94+16*3,duplicate_txt_visual,"",GREEN,16,FALSE,yes_unifont);
      }
      GrPrintW(hdc,8,94+16*3,MapEditor.typing_ground_txt,"",WHITE,16,FALSE,yes_unifont);
    }

  }
}
