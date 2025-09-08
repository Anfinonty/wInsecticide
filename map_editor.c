
//Map Editor global variables -- in struct now :)
struct MapEditor
{
  bool init_palettes;
  bool load_ptextures;
  bool alter_ptexture;
  bool alter_ptexture_color;

  int rd_min_x;
  int rd_max_x;
  int rd_min_y;
  int rd_max_y;

  int cursor_x;
  int cursor_y;

  int sticky_level;
  
  //overall
  int selected_option; //0:ground, 1:player, 2:enemy, 3:enemy_type, 4:background, 5:textures editor

  //typing search
  bool is_typing_search;
  int typing_search_txt_pos;
  int typing_search_id;

  //enemy palette change
  int flag_enemy_palette_i;

  //===== Ground ===== 
  int selected_ground_option; //0:ground_id, 1:type, 2:color, 3:is_ghost
  int selected_ground_id; //0 -> GROUND_NUM
  int selected_ground_pivot; //0:x1y1 ,1:x2y2, 2:x3y3 


  bool is_ground_txt_typing;
  bool is_ground_txt_typing_loaded;
  int typing_ground_txt_pos;
  wchar_t typing_ground_txt[512]; //same ant as ground
  //wchar_t typing_ground_txt2[512]; //same ant as ground //inbetween txt



  //===== Enemy ===== 
  int selected_enemy_option;
  int selected_enemy_id;


  //===== Enemy Type ===== (0->9)
  int selected_enemy_type_id;
  int selected_enemy_type_option;


  //pick color
  int pick_color;

  //==== background ambient ====
  int selected_lvl_ambient_option;




  //set options for lvl ambient
  int set_lvl_ambient_val[9];



  //=== platform texture ====
  int selected_ptexture_option;
  int selected_ptexture_id;
  //int selected_ptexture_type;
  //int selected_ptexture_color_id;


  //Shooting bullet
  bool demo_enemy_spriteisleft;
  int demo_enemy_spritecooldown;  
  double demo_enemy_spritex;
  double demo_enemy_spritey;

  int bullet_cooldown;
  int bullet_fire_cooldown;
  int bullet_length;

  int bullet_head_x[MAX_BULLET_PER_FIRE];
  int bullet_head_y[MAX_BULLET_PER_FIRE];

  int clipboard_ground_id;
  int clipboard_ground_color_id;
  int clipboard_enemy_id;
  int clipboard_enemy_type_id;

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
  RGBQUAD enemyPalette[256];
  HBITMAP sprite_1;
  DRAWSPRITE draw_sprite_1;
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
      FreeDrawSprite(&MEEnemySprite[i]->draw_sprite_1);
    }
  }
}


/*Set Enemy Type will be in the stack to be consistent with Saved Enemy Type being in the stack*/

#define ENEMY_TYPE_INT_ATTR_NUM 20
#define ENEMY_TYPE_DOUBLE_ATTR_NUM 2
#define ENEMY_TYPE_BOOL_ATTR_NUM 1

int enemy_int_attr_min[ENEMY_TYPE_INT_ATTR_NUM]=
{
0, //species
15, //follow range
0, //unchase range
0, //chase range
0, //color
1, //speed multiplier*
1, //health
1, //shoot at player range
0, //aim random
1, //bullet cool down
0, //bullet fire cool down
1, //bullet fire at once
1, //bullet length
1, //bullet damage,
1, //bullet speed multiplier*
1, //bullet range
0, //bullet color
0, //bullet graphics type
0, //time breaker rare
0 //time breaker length
};

int enemy_int_attr_max[ENEMY_TYPE_INT_ATTR_NUM]=
{
8, // species
MAX_FOLLOW_RANGE+1, //follow range
MAX_FOLLOW_RANGE+1, //unchase range
MAX_FOLLOW_RANGE+1, //chase range
16, //color
21, //speed multiplier*
1001, //health
MAX_FOLLOW_RANGE+1, //shoot at player range
640+1, //aim random
501, //bullet cooldown
501, //bullet fire cooldown
MAX_BULLET_PER_FIRE+1, //bullet fire at once
501, //bullet length
101, //bullet damage
21, //bullet speed multiplier*
MAX_FOLLOW_RANGE+1, //bullet range
16, //bullet color
11, //bullet_graphics_type
10001, //time breaker rare
20001 //time breaker length
};


int enemy_int_attr_delta[ENEMY_TYPE_INT_ATTR_NUM]=
{
1, // species
1, //follow range
1, //unchase range
1, //chase range
1, //color
1, //speed multiplier*
1, //health
1, //shoot at player range
1, //aim random
1, //bullet cooldown
1, //bullet fire cooldown
1, //bullet fire at once
1, //bullet length
1, //bullet damage
1, //bullet speed multiplier*
1, //bullet range
1, //bullet color
1, //bullet_graphics_type
5, //time breaker rare
5 //time breaker length
};

int *set_enemy_type_int_attr[ENEMY_TYPE_INT_ATTR_NUM]=
{
    set_enemy_type_species,
    set_enemy_type_follow_range,
    set_enemy_type_unchase_range,
    set_enemy_type_chase_range,
    set_enemy_type_color,
    set_enemy_type_speed_multiplier,
    set_enemy_type_health,
    set_enemy_type_shoot_at_player_range,
    set_enemy_type_aim_rand,
    set_enemy_type_bullet_cooldown,
    set_enemy_type_bullet_fire_cooldown,
    set_enemy_type_bullet_fire_at_once,
    set_enemy_type_bullet_length,
    set_enemy_type_bullet_damage,
    set_enemy_type_bullet_speed_multiplier,
    set_enemy_type_bullet_range,
    set_enemy_type_bullet_color,
    set_enemy_type_bullet_graphics_type,
    set_enemy_type_time_breaker_rare,
    set_enemy_type_time_breaker_length
};


double enemy_double_attr_min[ENEMY_TYPE_DOUBLE_ATTR_NUM]={0,0};
double enemy_double_attr_max[ENEMY_TYPE_DOUBLE_ATTR_NUM]={1.0,1.0};
double enemy_double_attr_delta[ENEMY_TYPE_DOUBLE_ATTR_NUM]={0.1,0.1};
double *set_enemy_type_double_attr[ENEMY_TYPE_DOUBLE_ATTR_NUM]={
  set_enemy_type_speed,
  set_enemy_type_bullet_speed
};


bool *set_enemy_type_bool_attr[ENEMY_TYPE_BOOL_ATTR_NUM]={
  set_enemy_type_time_breaker_immune
};


int melvlambience_min[9]={
0,0,0,0,1,-100,0,1,-100
};

int melvlambience_max[9]={
5,0,2,4,101,101,2,101,101
};


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
  if ((Ground[i]->type>=3 && Ground[i]->type<=8) || Ground[i]->type==1) {//triangle
    SetMENodeGridAttributes2(i);
  }
}





void InitMERDGrid()
{
  int i=0,j=0,k=0,gid=0,column=0,row=0,x=0,y=0,_x=0,_y=0,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y;

  for (i=0;i<rendered_ground_num;i++) {
    int k=render_grounds[i];
    if (k!=-1) {
      Ground[k]->within_render_distance=FALSE; //All rendered-grounds are no-longer within render distance
    }
    render_grounds[i]=-1; //unrender all rendered grounds
  }


  rendered_ground_num=0;

  //Begin rendering
  //"What happens when you lose everything? You just start again. Start all over again" - Maximo Park
  for (i=0;i<RDGRID_DYN_NUM;i++) { //all render distance grids from top-left to bottom-right
    _x=RDGrid[i].x+cx2-GR_WIDTH/2;
    _y=RDGrid[i].y+cy2-GR_HEIGHT/2;

    if (i==0) {
      MapEditor.rd_min_x=_x;
      MapEditor.rd_min_y=_y;
    } else if (i==RDGRID_DYN_NUM-1) {
      MapEditor.rd_max_x=_x;
      MapEditor.rd_max_y=_y;
    }
    gid=GetGridId(_x,_y,MAP_WIDTH,VGRID_SIZE,VGRID_NUM);
    if (gid!=-1 && _y>0 && _x>0 && _x<MAP_WIDTH && _y<MAP_HEIGHT) {
      x=GR_WIDTH/2+VGrid[gid]->x1-cx2;
      y=GR_HEIGHT/2+VGrid[gid]->y1-cy2;

      //grounds
      for (j=0;j<VGrid[gid]->max_ground_num;j++) { //fetch all grounds that are occupying the grid
	    k=VGrid[gid]->ground_ids[j]; //ground_id in grid
 	    if (!Ground[k]->within_render_distance) { //ground is now within render distance
	      Ground[k]->within_render_distance=TRUE;
          render_grounds[rendered_ground_num]=k; //append ground to render_grounds array
	      rendered_ground_num++; //count number of rendered grounds
	    } 
      }
    }
    column++; //Next column
    if (column>=RD_DYN_WIDTH) { //if the column is beyond the render distance
      row++; //move to the next row
      column=0; //go back to first column
    }
  }
  //Sort
  quicksort(render_grounds, 0, rendered_ground_num-1);
}


bool YesInitMERDGrid()
{
  if (GRID_SIZE*2<player.cam_move_x && player.cam_move_x<MAP_WIDTH-GRID_SIZE*2) {
    if (player.cam_move_x<MapEditor.rd_min_x+GRID_SIZE*2 || player.cam_move_x>MapEditor.rd_max_x-GRID_SIZE*2) {
      return TRUE;
    }
  }
  if (GRID_SIZE*2<player.cam_move_y && player.cam_move_y<MAP_HEIGHT-GRID_SIZE*2) {
    if (player.cam_move_y<MapEditor.rd_min_y+GRID_SIZE*2 || player.cam_move_y>MapEditor.rd_max_y-GRID_SIZE*2) {
      return TRUE;
    }
  }
  return FALSE;
}


void InitMENodeGridAttributes()
{
  for (int i=0;i<GROUND_NUM;i++) {
    SetMENodeGridAttributes(i);
    loading_numerator++;
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
  if ((Ground[i]->type>=3 && Ground[i]->type<=8) || Ground[i]->type==1) {//triangle ground
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
    loading_numerator++;
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
    switch (set_enemy_type_species[i]) {
      case 0:
        MEEnemySprite[i]->sprite_1=CopyCrunchyBitmap(enemy1_sprite_1,SRCCOPY);
        break;
      case 1:
        MEEnemySprite[i]->sprite_1=CopyCrunchyBitmap(enemy2_sprite_1,SRCCOPY);
        break;
      case 2:
        MEEnemySprite[i]->sprite_1=CopyCrunchyBitmap(enemy3_sprite_1,SRCCOPY);
        break;
      case 3:
        MEEnemySprite[i]->sprite_1=CopyCrunchyBitmap(enemy4_sprite_1,SRCCOPY);
        break;
      case 4:
        MEEnemySprite[i]->sprite_1=CopyCrunchyBitmap(enemy5_sprite_1,SRCCOPY);
        break;
      case 5:
      case 6:
      case 7:
        MEEnemySprite[i]->sprite_1=CopyCrunchyBitmap(enemy6_sprite_1,SRCCOPY);
        break;
    }

    ReplaceBitmapColor2(MEEnemySprite[i]->sprite_1,LTGREEN,BLACK,8,LTGREEN);
    loading_numerator++;
    GenerateDrawSprite(&MEEnemySprite[i]->draw_sprite_1,MEEnemySprite[i]->sprite_1);

    CopyReplaceColorPalette(MEEnemySprite[i]->enemyPalette,rgbColorsDefault,167,rgbPaint[set_enemy_type_color[i]]); //set normal palette
    CopyReplaceColorPalette(MEEnemySprite[i]->enemyPalette,MEEnemySprite[i]->enemyPalette,151,LTGRAY); //set outline color
    //BitmapPalette(hdc,hdc2,MEEnemySprite[i]->draw_sprite_1.sprite_paint,MEEnemySprite[i]->enemyPalette);
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
  player.cam_move_x=0;
  player.cam_move_y=0;

  player.cam_x=-player.x;
  player.cam_y=-player.y;
}



void InitMapEditor()
{
  rendered_ground_num=0;

  MapEditor.selected_option=0;


  MapEditor.sticky_level=0;


  MapEditor.typing_search_txt_pos=0;
  MapEditor.typing_search_id=0;
  MapEditor.is_typing_search=FALSE;


  MapEditor.selected_ground_id=0;
  MapEditor.selected_ground_pivot=0;
  MapEditor.selected_ground_option=0;

  MapEditor.selected_enemy_id=0;
  MapEditor.selected_enemy_option=0;


  MapEditor.selected_enemy_type_id=0;
  MapEditor.selected_enemy_type_option=0;

  MapEditor.selected_lvl_ambient_option=0;

  MapEditor.selected_ptexture_id=0;
  MapEditor.selected_ptexture_option=0;
  

  MapEditor.pick_color=-1;

  MapEditor.is_ground_txt_typing=FALSE;
  MapEditor.is_ground_txt_typing_loaded=FALSE;
  MapEditor.typing_ground_txt_pos=0;


  MapEditor.set_lvl_ambient_val[0]=map_background;
  MapEditor.set_lvl_ambient_val[1]=custom_map_background_color_i;
  MapEditor.set_lvl_ambient_val[2]=is_moon;
  MapEditor.set_lvl_ambient_val[3]=map_weather;
  MapEditor.set_lvl_ambient_val[4]=rain_grad_rise;
  MapEditor.set_lvl_ambient_val[5]=rain_grad_run;
  MapEditor.set_lvl_ambient_val[6]=is_shadows;
  MapEditor.set_lvl_ambient_val[7]=shadow_grad_rise;
  MapEditor.set_lvl_ambient_val[8]=shadow_grad_run;

  MapEditor.clipboard_ground_id=0;
  MapEditor.clipboard_ground_color_id=0;
  MapEditor.clipboard_enemy_id=0;
  MapEditor.clipboard_enemy_type_id=0;

  for (int i=0;i<512;i++) {
    MapEditor.typing_ground_txt[i]='\0';
  }
  //swprintf(MapEditor.typing_ground_txt,512,L"%ls",Ground[MapEditor.selected_ground_id]->text);



  //for (int i=0;i<MAX_GROUND_NUM;i++)
    //render_grounds[i]=-1;
  render_grounds=calloc(GROUND_NUM,sizeof(int));
  for (int i=0;i<GROUND_NUM;i++) {
    render_grounds[i]=-1;
  }
  //Create Class Objects()
  Ground = calloc(GROUND_NUM,sizeof(AGround*));
  VGrid = calloc(VGRID_NUM,sizeof(struct AVGrid*));
  MEEnemy = calloc(ENEMY_NUM,sizeof(AMEEnemy*));
  MEEnemySprite = calloc(ENEMY_TYPE_NUM,sizeof(AMEEnemySprite*));

  for (int i=0;i<GROUND_NUM;i++) {
    AGround *newGround = createGround();
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
  InitBullet(ENEMY_BULLET_NUM);

  player.cam_move_x=-player.cam_x;
  player.cam_move_y=-player.cam_y;
  InitMERDGrid();

  if (!run_after_once) {
    run_after_once=TRUE;
  } else {
    PlaySound(NULL, NULL, SND_ASYNC);
  }
}






void InitLevelMapEditor()
{
  wchar_t txt[128];
  loading_numerator=0;
  loading_denominator=0;
  swprintf(txt,128,L"saves/%ls/level.txt",level_names[level_chosen]);
  level_loading=TRUE;

  LoadSave(txt,FALSE); //load saves

  MapEditor.load_ptextures=TRUE;
  MapEditor.alter_ptexture=FALSE;
  MapEditor.alter_ptexture_color=FALSE;

  wcsncpy(typing_lvl_name,level_names[level_chosen],16);
  typing_lvl_name_pos=lstrlenW(typing_lvl_name);
  set_ground_amount=GROUND_NUM;
  set_enemy_amount=ENEMY_NUM;
  set_map_width_amount=MAP_WIDTH;
  set_map_height_amount=MAP_HEIGHT;
  loading_denominator=GROUND_NUM+ENEMY_NUM+ENEMY_TYPE_NUM;


  srand(time(NULL));
  timeBeginPeriod(1);
  in_map_editor=TRUE;

  InitMapEditor();

  main_menu_chosen=4;
  level_loaded=TRUE;
  level_loading=FALSE;


  //Unforeground and unfullscreen!!
  flag_hide_taskbar=TRUE;

  OLD_GR_WIDTH=0;
  OLD_GR_HEIGHT=0;

  MapEditor.flag_enemy_palette_i=-1;
  MapEditor.init_palettes=FALSE;

  MapEditor.demo_enemy_spriteisleft=FALSE;
  MapEditor.demo_enemy_spritecooldown=300;
  MapEditor.demo_enemy_spritex=0;
  MapEditor.demo_enemy_spritey=0;

  MapEditor.bullet_cooldown=0;
  MapEditor.bullet_fire_cooldown=0;
  MapEditor.bullet_length=0;
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
  MapEditor.cursor_x=stickyTo(mouse_x+player.cam_move_x-GR_WIDTH/2,MapEditor.sticky_level);
  MapEditor.cursor_y=stickyTo(mouse_y+player.cam_move_y-GR_HEIGHT/2,MapEditor.sticky_level);

  

  //lvl maker movement limiter
  player.cam_move_x=-player.cam_x;
  player.cam_move_y=-player.cam_y;

  if (level_loaded) {
    if (YesInitMERDGrid()) {
      InitRDGrid();
      InitMERDGrid();
    }


    Click();


    if (MapEditor.selected_option>0 && player.attack_rst) {
      player.attack_rst=FALSE;
    }

    switch (MapEditor.selected_option) {
      case 0:
        if (player.right_click_hold_timer==62) {
          if ((Ground[MapEditor.selected_ground_id]->type>=3 && Ground[MapEditor.selected_ground_id]->type<=8) || Ground[MapEditor.selected_ground_id]->type==1) { //trifill
            MapEditor.selected_ground_pivot=LimitValue(MapEditor.selected_ground_pivot+1,0,3);
          } else {
            MapEditor.selected_ground_pivot=LimitValue(MapEditor.selected_ground_pivot+1,0,2);
          }
        }    
        if (player.attack_rst || player.rst_left_click) { //release mouse
          if (!IsOutOfBounds(MapEditor.cursor_x,MapEditor.cursor_y,1,MAP_WIDTH,MAP_HEIGHT)) {
            if (player.attack_rst)
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


            if ((Ground[i]->type>=3 && Ground[i]->type<=8) || Ground[i]->type==1) {//trifill
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
            if (player.attack_rst) { //let go of left click
              SetGround(MapEditor.selected_ground_id);
              SetMENodeGridAttributes(MapEditor.selected_ground_id);
              InitMERDGrid();
              player.attack_rst=FALSE;
            }
          }
        }
        break;

      case 1: //click action, player axes
        if (!IsOutOfBounds(MapEditor.cursor_x,MapEditor.cursor_y,1,MAP_WIDTH,MAP_HEIGHT)) {
          if (player.rst_left_click) { //release mouse
            player.x=MapEditor.cursor_x;
            player.y=MapEditor.cursor_y;
          }
        }
        break;

      case 2: //click action, enemey axes and set type 0->9
        if (!IsOutOfBounds(MapEditor.cursor_x,MapEditor.cursor_y,1,MAP_WIDTH,MAP_HEIGHT)) {
          if (player.rst_left_click) { //release mouse
            MEEnemy[MapEditor.selected_enemy_id]->x=MapEditor.cursor_x;
            MEEnemy[MapEditor.selected_enemy_id]->y=MapEditor.cursor_y;
          }
        }
        break;


      case 3: { //Do bullet action when showing enemy type
        int q=MapEditor.selected_enemy_type_id;
        int slash_time=1;
        int dice=0;
        double unchase_dist=0;
        if (mouse_x-1<=MapEditor.demo_enemy_spritex && MapEditor.demo_enemy_spritex<=mouse_x+1 &&
            mouse_y-1<=MapEditor.demo_enemy_spritey && MapEditor.demo_enemy_spritey<=mouse_y+1) {
          MapEditor.demo_enemy_spritecooldown=300;
        }

        if (q>-1 && q<ENEMY_TYPE_NUM) {
          if (set_enemy_type_time_breaker_length[q]>0) {
            dice=RandNum(0,set_enemy_type_time_breaker_rare[q],&misc_rng_i,-1);
            if (dice==1) {
              slash_time=set_enemy_type_time_breaker_length[q];
            }
          }


          unchase_dist=GetDistance(mouse_x,mouse_y,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey);
          if ((unchase_dist<NODE_SIZE*set_enemy_type_unchase_range[q]/2 &&
               GetDistance(mouse_x,mouse_y,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey)>NODE_SIZE*set_enemy_type_chase_range[q]/2)) {
            MapEditor.demo_enemy_spritecooldown=300;
          }

          for (int slash_time_i=0;slash_time_i<slash_time;slash_time_i++) {
            if (MapEditor.demo_enemy_spritecooldown==0) {
              unchase_dist=GetDistance(mouse_x,mouse_y,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey);
              if ((GetDistance(mouse_x,mouse_y,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey)<NODE_SIZE*set_enemy_type_chase_range[q]/2 &&
                   unchase_dist<NODE_SIZE*set_enemy_type_unchase_range[q]/2) || 
                  (unchase_dist>=NODE_SIZE*set_enemy_type_unchase_range[q]/2 &&
                   GetDistance(mouse_x,mouse_y,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey)<NODE_SIZE*set_enemy_type_follow_range[q]/2)) {                    
                for (int t=0;t<set_enemy_type_speed_multiplier[q];t++) {
                  if (MapEditor.demo_enemy_spritex<mouse_x) {
                    MapEditor.demo_enemy_spritex+=set_enemy_type_speed[q];
                    if (set_enemy_type_species[q]!=5 && set_enemy_type_species[q]!=6) {
                      MapEditor.demo_enemy_spriteisleft=FALSE;
                    } else {
                      if (set_enemy_type_species[q]==5) {
                        MapEditor.demo_enemy_spriteisleft=FALSE;
                      } else {
                        MapEditor.demo_enemy_spriteisleft=TRUE;
                      }
                    }
                  } else {
                    MapEditor.demo_enemy_spritex-=set_enemy_type_speed[q];
                    if (set_enemy_type_species[q]!=5 && set_enemy_type_species[q]!=6) {
                      MapEditor.demo_enemy_spriteisleft=TRUE;
                    } else {
                      if (set_enemy_type_species[q]==5) {
                        MapEditor.demo_enemy_spriteisleft=FALSE;
                      } else {
                        MapEditor.demo_enemy_spriteisleft=TRUE;
                      }
                    }
                  }

                  if (MapEditor.demo_enemy_spritey<mouse_y) {
                    MapEditor.demo_enemy_spritey+=set_enemy_type_speed[q];
                  } else {
                    MapEditor.demo_enemy_spritey-=set_enemy_type_speed[q];
                  }
                }
              }
            } else {
              MapEditor.demo_enemy_spritecooldown--;
            }

            if (GetDistance(mouse_x,mouse_y,MapEditor.demo_enemy_spritex,MapEditor.demo_enemy_spritey)<=NODE_SIZE*set_enemy_type_shoot_at_player_range[q]/2) {
              if (MapEditor.bullet_fire_cooldown<=0) {
                if (MapEditor.bullet_length==0) {
                  for (int j=0;j<set_enemy_type_bullet_fire_at_once[q];j++) {//shoot several bullets at once
	                    //MapEditor.bullet_head_x[j]=8+RandNum(-set_enemy_type_aim_rand[q],set_enemy_type_aim_rand[q],player.seed);
	                    //MapEditor.bullet_head_y[j]=32+16*(ENEMY_TYPE_INT_ATTR_NUM+ENEMY_TYPE_DOUBLE_ATTR_NUM)+16*3+RandNum(-set_enemy_type_aim_rand[q],set_enemy_type_aim_rand[q],player.seed);
	                  MapEditor.bullet_head_x[j]=mouse_x+RandNum(-set_enemy_type_aim_rand[q],set_enemy_type_aim_rand[q],&misc_rng_i,-1);
	                  MapEditor.bullet_head_y[j]=mouse_y+RandNum(-set_enemy_type_aim_rand[q],set_enemy_type_aim_rand[q],&misc_rng_i,-1);
                  }
	            }



                if (MapEditor.bullet_cooldown<=0) {
                  for (int j=0;j<set_enemy_type_bullet_fire_at_once[q];j++) {//several bullets at once
	                double shoot_target_x= MapEditor.bullet_head_x[j];
    	  	        double shoot_target_y= MapEditor.bullet_head_y[j];
                    ShootBullet(current_bullet_id,
                        -1,//Enemy[i]->bullet_shot_num,
                        rgbPaint[set_enemy_type_bullet_color[q]],//Enemy[i]->bullet_color,
                        set_enemy_type_bullet_graphics_type[q],//Enemy[i]->bullet_graphics_type,
                        set_enemy_type_bullet_range[q],//Enemy[i]->bullet_range,
                        set_enemy_type_bullet_speed[q],//Enemy[i]->bullet_speed,
                        set_enemy_type_bullet_speed_multiplier[q],//Enemy[i]->bullet_speed_multiplier,
                        0,//Enemy[i]->bullet_damage,
                        -10,//i,
                        MapEditor.demo_enemy_spritex,//Enemy[i]->x,
                        MapEditor.demo_enemy_spritey,//Enemy[i]->y,
                        MapEditor.demo_enemy_spritex,//Enemy[i]->x,
                        MapEditor.demo_enemy_spritey,//Enemy[i]->y,
                        shoot_target_x,//Enemy[i]->shoot_target_x,
                        shoot_target_y,//Enemy[i]->shoot_target_y,
                        0
                    );

                //after shooting
                    current_bullet_id++;
                    if (current_bullet_id>ENEMY_BULLET_NUM-1) {                      
                      current_bullet_id=0;
                    }
	              } //end of for
	              MapEditor.bullet_cooldown=set_enemy_type_bullet_cooldown[q];
	              MapEditor.bullet_length++;
	              if (MapEditor.bullet_length>=set_enemy_type_bullet_length[q]) {
  	                MapEditor.bullet_fire_cooldown=set_enemy_type_bullet_fire_cooldown[q];
	                MapEditor.bullet_length=0;
	              }
	            } else {
                  MapEditor.bullet_cooldown--;
                }
              } else {
	            MapEditor.bullet_fire_cooldown--;
              }
            }
          }
        }
        for (int i=0;i<ENEMY_BULLET_NUM;i++) {
          BulletAct(i);
            //printf("%d: hello:\n",i);
        }
        }
        break;
    } //End of switch statement
  }
}


void CleanupMapEditorAll()
{
    level_loaded=FALSE;
    free(render_grounds);

    //CleanUpPlayer(); //clean up all sprites
    /*if (player.sprite_1_cache!=NULL) { //clean up 1 sprite
      DeleteObject(player.sprite_1_cache);
      FreeDrawSprite(&player.draw_sprite_1);
    }*/

    CleanUpMEEnemySprites();
    CleanupPlayerAttributes();
    save_level[0]='\0';
    call_help_timer=0;


    //free saved enemy
    //free saved grounds pointer & Ground

    free(saved_ground_is_ghost);
    free(saved_ground_color);
    free(saved_ground_type);
    free(saved_ground_text_size);


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


    DeleteObject(map_background_sprite);
    //LoadMainMenuBackground();
    main_menu_chosen=0;
    //back_to_menu=FALSE;
    run_after_once=FALSE;
    //clean_up_sound=TRUE;
    in_map_editor=FALSE;
    //printf("===All pointers freed, groundnum:%d,gridnum:%d,enemynum:%d\n",GROUND_NUM,VGRID_NUM,ENEMY_NUM);
}

