//Save Level Attribute Row Values
#define S_PRELUDE_ROW_NUM  4


//Game Values
int GROUND_NUM;
int WATER_GROUND_NUM=0;
int FIRE_GROUND_NUM=0;

int ENEMY_NUM;
int LARGE_ENEMY_TYPE_NUM;
int LARGER_ENEMY_TYPE_NUM;
int PF_ENEMY_NUM;
int MAP_WIDTH;
int MAP_HEIGHT;
int GRID_NUM;	
int VGRID_NUM;	

int PLATFORM_GRID_NUM;
//int FOREGROUND_GRID_NUM;
//int SHADOW_GRID_NUM;

int RD_DYN_WIDTH;
int RD_DYN_HEIGHT;
int RDGRID_DYN_NUM;
int MAP_NODE_NUM;


//====Ground====

float *saved_ground_x1;
float *saved_ground_y1;
float *saved_ground_x2;
float *saved_ground_y2;
float *saved_ground_x3;
float *saved_ground_y3;
#define S_GROUND_FLOAT_NUM 6
float *ground_float_pointer[S_GROUND_FLOAT_NUM];
void InitGroundFloatPointer()
{
  for (int i=0;i<S_GROUND_FLOAT_NUM;i++) {
    ground_float_pointer[i]=NULL;
  }

  ground_float_pointer[0]=saved_ground_x1;
  ground_float_pointer[1]=saved_ground_y1;
  ground_float_pointer[2]=saved_ground_x2;
  ground_float_pointer[3]=saved_ground_y2;
  ground_float_pointer[4]=saved_ground_x3;
  ground_float_pointer[5]=saved_ground_y3;
}

bool *saved_ground_is_ghost;
#define S_GROUND_BOOL_NUM 1
bool *ground_bool_pointer[S_GROUND_BOOL_NUM];
void InitGroundBoolPointer()
{
  for (int i=0;i<S_GROUND_BOOL_NUM;i++) {
    ground_bool_pointer[i]=NULL;
  }
  ground_bool_pointer[0]=saved_ground_is_ghost;
}


int *saved_ground_color;
int *saved_ground_type;
#define S_GROUND_INT_NUM 2
int *ground_int_pointer[S_GROUND_INT_NUM];
void InitGroundIntPointer()
{
  for (int i=0;i<S_GROUND_INT_NUM;i++) {
    ground_int_pointer[i]=NULL;
  }
  ground_int_pointer[0]=saved_ground_color;
  ground_int_pointer[1]=saved_ground_type;
}


int *saved_ground_text_size; //shared with UTF
wchar_t **saved_ground_text;
#define S_GROUND_UTF_NUM 1
//UNIQUE, NO POINTER


//===Enemy===
int *saved_enemy_type;
#define S_ENEMY_INT_NUM    1
int *enemy_int_pointer[S_ENEMY_INT_NUM];
void InitEnemyIntPointer()
{
  for (int i=0;i<S_ENEMY_INT_NUM;i++) {
    enemy_int_pointer[i]=NULL;
  }
  enemy_int_pointer[0]=saved_enemy_type;
}

float *saved_enemy_x;
float *saved_enemy_y;
#define S_ENEMY_FLOAT_NUM    2
float *enemy_float_pointer[S_ENEMY_FLOAT_NUM];
void InitEnemyFloatPointer()
{
  for (int i=0;i<S_ENEMY_FLOAT_NUM;i++) {
    enemy_float_pointer[i]=NULL;
  }
  enemy_float_pointer[0]=saved_enemy_x;
  enemy_float_pointer[1]=saved_enemy_y;
}

//====Enemy Type (fixed value of 10)====
//Load EnemyType Float Segment
float saved_enemy_type_speed[ENEMY_TYPE_NUM];
float saved_enemy_type_bullet_speed[ENEMY_TYPE_NUM];
#define S_ENEMY_TYPE_FLOAT_NUM 2
float *enemy_type_float_pointer[S_ENEMY_TYPE_FLOAT_NUM]=
{
  saved_enemy_type_speed,
  saved_enemy_type_bullet_speed
};



//Load EnemyType Int Segment
int saved_enemy_type_species[ENEMY_TYPE_NUM];
int saved_enemy_type_follow_range[ENEMY_TYPE_NUM]; //
int saved_enemy_type_unchase_range[ENEMY_TYPE_NUM];
int saved_enemy_type_chase_range[ENEMY_TYPE_NUM]; //
int saved_enemy_type_color[ENEMY_TYPE_NUM];
int saved_enemy_type_speed_multiplier[ENEMY_TYPE_NUM]; //
int saved_enemy_type_health[ENEMY_TYPE_NUM];
int saved_enemy_type_shoot_at_player_range[ENEMY_TYPE_NUM]; //
int saved_enemy_type_aim_rand[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_cooldown[ENEMY_TYPE_NUM]; //
//
int saved_enemy_type_bullet_fire_cooldown[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_fire_at_once[ENEMY_TYPE_NUM]; //
int saved_enemy_type_bullet_length[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_damage[ENEMY_TYPE_NUM]; //
int saved_enemy_type_bullet_speed_multiplier[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_range[ENEMY_TYPE_NUM]; //
int saved_enemy_type_bullet_color[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_graphics_type[ENEMY_TYPE_NUM]; //
int saved_enemy_type_time_breaker_rare[ENEMY_TYPE_NUM];
int saved_enemy_type_time_breaker_length[ENEMY_TYPE_NUM]; //
//
#define S_ENEMY_TYPE_INT_NUM 20
int *enemy_type_int_pointer[S_ENEMY_TYPE_INT_NUM]=
{
  saved_enemy_type_species, 
  saved_enemy_type_follow_range, //
  saved_enemy_type_unchase_range, 
  saved_enemy_type_chase_range,  //
  saved_enemy_type_color, 
  saved_enemy_type_speed_multiplier, // 
  saved_enemy_type_health, 
  saved_enemy_type_shoot_at_player_range, // 
  saved_enemy_type_aim_rand, 
  saved_enemy_type_bullet_cooldown, //
//
  saved_enemy_type_bullet_fire_cooldown, 
  saved_enemy_type_bullet_fire_at_once, //
  saved_enemy_type_bullet_length,
  saved_enemy_type_bullet_damage, //
  saved_enemy_type_bullet_speed_multiplier, 
  saved_enemy_type_bullet_range, //
  saved_enemy_type_bullet_color, 
  saved_enemy_type_bullet_graphics_type, //
  saved_enemy_type_time_breaker_rare, 
  saved_enemy_type_time_breaker_length //
};

//Load EnemyType Bool Segment
bool saved_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];
#define S_ENEMY_TYPE_BOOL_NUM 1
bool *enemy_type_bool_pointer[S_ENEMY_TYPE_BOOL_NUM]=
{
  saved_enemy_type_time_breaker_immune
};



//EnemyType Saved But Game Attributes
int saved_large_enemy_type_species[ENEMY_TYPE_NUM];
int saved_larger_enemy_type_species[ENEMY_TYPE_NUM];
//
int saved_enemy_type_rot_sprite_id[ENEMY_TYPE_NUM];
int saved_enemy_type_rot_xsprite_id[ENEMY_TYPE_NUM];
//
bool saved_large_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];
bool saved_larger_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];






//Set Enemy Float Segment
float set_enemy_type_speed[ENEMY_TYPE_NUM];
float set_enemy_type_bullet_speed[ENEMY_TYPE_NUM];
//Set Enemy Type Int Segment
int set_enemy_type_species[ENEMY_TYPE_NUM];
int set_enemy_type_follow_range[ENEMY_TYPE_NUM];
int set_enemy_type_unchase_range[ENEMY_TYPE_NUM];
int set_enemy_type_chase_range[ENEMY_TYPE_NUM];
int set_enemy_type_color[ENEMY_TYPE_NUM];
int set_enemy_type_speed_multiplier[ENEMY_TYPE_NUM];
int set_enemy_type_health[ENEMY_TYPE_NUM];
int set_enemy_type_shoot_at_player_range[ENEMY_TYPE_NUM];
int set_enemy_type_aim_rand[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_cooldown[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_fire_cooldown[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_fire_at_once[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_length[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_damage[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_speed_multiplier[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_range[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_color[ENEMY_TYPE_NUM];
int set_enemy_type_bullet_graphics_type[ENEMY_TYPE_NUM];
int set_enemy_type_time_breaker_rare[ENEMY_TYPE_NUM];
int set_enemy_type_time_breaker_length[ENEMY_TYPE_NUM];
//Set Enemy Bool Segment
bool set_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];

float *set_enemy_type_float_pointer[S_ENEMY_TYPE_FLOAT_NUM]=
{
  set_enemy_type_speed,
  set_enemy_type_bullet_speed
};

//
int *set_enemy_type_int_pointer[S_ENEMY_TYPE_INT_NUM]=
{
  set_enemy_type_species, 
  set_enemy_type_follow_range, //
  set_enemy_type_unchase_range, 
  set_enemy_type_chase_range,  //
  set_enemy_type_color, 
  set_enemy_type_speed_multiplier, // 
  set_enemy_type_health, 
  set_enemy_type_shoot_at_player_range, // 
  set_enemy_type_aim_rand, 
  set_enemy_type_bullet_cooldown, //
//
  set_enemy_type_bullet_fire_cooldown, 
  set_enemy_type_bullet_fire_at_once, //
  set_enemy_type_bullet_length,

  //more enemy bullet attributes for bullet patterns here
  //...,
  //set_enemy_type_bullet_next_angle //per bullet length, 

  set_enemy_type_bullet_damage, //
  set_enemy_type_bullet_speed_multiplier, 
  set_enemy_type_bullet_range, //
  set_enemy_type_bullet_color, 
  set_enemy_type_bullet_graphics_type, //
  set_enemy_type_time_breaker_rare, 
  set_enemy_type_time_breaker_length //
};

bool *set_enemy_type_bool_pointer[S_ENEMY_TYPE_BOOL_NUM]=
{
  set_enemy_type_time_breaker_immune
  //set_enemy_aim_player //enemy aiming at player or not
  //set_enemy_bullet_angle_oscillate //after shoot bullet:: ?angle=-angle,,:angle=(-angle)+angle (bullet_shot_num%2==1)
};



//Level Player Attributes 
float saved_player_x;
float saved_player_y;
#define S_PLAYER_FLOAT_NUM   2
float *player_float_pointer[S_PLAYER_FLOAT_NUM]=
{
  &saved_player_x,
  &saved_player_y
};


//Level Background Attributes
//they are stored in this exact order
int map_background; //0 day, //1 night-> night palette, //2 no background,
int custom_map_background_color,custom_map_background_color_i;
int is_moon;

//int map_moon_phase=0;
/*
  float map_moon_angle;
  float map_stars_angle;
  float map_sun_angle;
*/

int map_weather=0;
float rain_grad_rise=1;
float rain_grad_run=1;
#define S_LEVEL_ATTR_NUM    6

void *level_attributes[S_LEVEL_ATTR_NUM]=
{
  &map_background,
  &custom_map_background_color_i,
  &is_moon,
  &map_weather,
  &rain_grad_rise,
  &rain_grad_run
};

//0 == bool
//1 == int
//2 == float
char map_attr_var_type[S_LEVEL_ATTR_NUM]=
{
  1,
  1,
  1,
  1,
  2,
  2,
};

//See in struct_classes.c
//#define S_TEXTURE_INT_NUM  3
/*
  int type;
  int solid_value; //0-255
  int color_id;
*/

#define LVL_LOAD_PHASE_NUM   12
int load_lvl_jumpmap[LVL_LOAD_PHASE_NUM]=
{
  S_PRELUDE_ROW_NUM,
  S_GROUND_FLOAT_NUM,
  S_GROUND_BOOL_NUM,
  S_GROUND_INT_NUM,
  S_GROUND_UTF_NUM,
  S_ENEMY_INT_NUM,
  S_ENEMY_FLOAT_NUM,
  S_ENEMY_TYPE_FLOAT_NUM,
  S_ENEMY_TYPE_INT_NUM,
  S_ENEMY_TYPE_BOOL_NUM,
  S_PLAYER_FLOAT_NUM,
  S_LEVEL_ATTR_NUM,
  //S_TEXTURE_INT_NUM
};

int level_num=0;
wchar_t level_names[2000][256];



/*
saves
|__LevelName (folder) <--- store level name
|__LevelName (folder)
|__LevelName (folder)
|__LevelName (folder)
|__LevelName (folder)
    |__level.txt
    |__scores
    |__song
    |__images
*/
void GetSavesInDir(const wchar_t *dirname)
{
  for (int i=0;i<level_num;i++) {
    level_names[i][0]='\0';
  }
  level_num=0;

  _WDIR *d;
  struct _wdirent *dir; //https://castor-project.org/doc/castor3_0/struct__wdirent.html
  d = _wopendir(dirname);//
  if (d) {
    while ((dir=_wreaddir(d))!=NULL) {
      wchar_t indir[256];
      swprintf(indir,256,L"%ls/%ls",dirname,dir->d_name);
      if (PathIsDirectory(indir) && wcscmp(dir->d_name,L".")!=0 && wcscmp(dir->d_name,L"..")!=0) { //folder, check for songs in folder
        wcsncpy(level_names[level_num],dir->d_name,256);
        level_num++;
        if (level_num>=2000) {
          break;
        }
      }
    }
    _wclosedir(d);
  }
}



bool LoadSave(wchar_t *saves_name, bool spawn_objects)
{
  int row=0;
  int column=0;
  int set_txt_size=0;
  int int_val=0;
  int int_saved_val=0;
  float float_val;
  float float_saved_val;
  
  //for string char
  int char_utf16=0;
  int char_pos=0;
  wchar_t txt[512];
  bool writing_txt;
  bool deci;
  bool is_negative_val=FALSE;
  bool is_calc_utf16=FALSE;

  FILE *fptr;
  fptr = _wfopen(saves_name,L"r");

  if (!fptr) { //file notfound
    return FALSE;
  }
  int c; //each character

  //init
  writing_txt=FALSE;
  deci=FALSE;
  for (int i=0;i<512;i++) //init txt array
    txt[i] = 0;


  int current_lvl_load_phase_i=0;
  int low_=0;
  int lim_=load_lvl_jumpmap[current_lvl_load_phase_i];

  while ((c=fgetwc(fptr))!=WEOF) {
    if (row<=3 /*|| (row>=40 && row!=46 && row!=48 && !(row>=49 && row<=51))*/) { //first 4 rows
      if (c!=';') {//not yet a semicolon
        if (c>='0' && c<='9') { //numerical chars only
          int_val=c-'0'; //ascii convert to num
          int_saved_val*=10; //move digit to left
          int_saved_val+=int_val; //append number digit to right side
        }
      } else {//semi colon ;
        switch (row) {//save value
          case 0:
            GROUND_NUM=int_saved_val;
            WATER_GROUND_NUM=0;
            FIRE_GROUND_NUM=0;
            break;
          case 1:
            ENEMY_NUM=int_saved_val;
            break;
          case 2:
            MAP_WIDTH=int_saved_val;
            break;
          case 3:
            MAP_HEIGHT=int_saved_val;
            GRID_NUM=(MAP_WIDTH/GRID_SIZE) * (MAP_HEIGHT/GRID_SIZE);
            VGRID_NUM=(MAP_WIDTH/VGRID_SIZE) * (MAP_HEIGHT/VGRID_SIZE);
            MAP_NODE_NUM=(MAP_WIDTH/NODE_SIZE) * (MAP_HEIGHT/NODE_SIZE);
            //malloc attributes


            saved_ground_is_ghost=calloc(GROUND_NUM,sizeof(bool));
            saved_ground_color=calloc(GROUND_NUM,sizeof(int));
            saved_ground_type=calloc(GROUND_NUM,sizeof(int));

            saved_enemy_type=calloc(ENEMY_NUM,sizeof(int));
            saved_ground_text_size=calloc(GROUND_NUM,sizeof(int));
            saved_enemy_x=calloc(ENEMY_NUM,sizeof(float));
            saved_enemy_y=calloc(ENEMY_NUM,sizeof(float));


            saved_ground_x1=calloc(GROUND_NUM,sizeof(float));
            saved_ground_y1=calloc(GROUND_NUM,sizeof(float));
            saved_ground_x2=calloc(GROUND_NUM,sizeof(float));
            saved_ground_y2=calloc(GROUND_NUM,sizeof(float));
            saved_ground_x3=calloc(GROUND_NUM,sizeof(float));
            saved_ground_y3=calloc(GROUND_NUM,sizeof(float));
            saved_ground_text=calloc(GROUND_NUM,sizeof(wchar_t*));
            for (int i=0;i<GROUND_NUM;i++) {
              saved_ground_text[i]=(wchar_t*)malloc(512*sizeof(wchar_t));
            }

            InitGroundFloatPointer();
            InitGroundIntPointer();
            InitGroundBoolPointer();

            InitEnemyIntPointer(); 
            InitEnemyFloatPointer();


            if (spawn_objects) {
              Ground = calloc((GROUND_NUM+MAX_WEB_NUM),sizeof(AGround*));
              VGrid = calloc(VGRID_NUM,sizeof(struct AVGrid*));
              NodeGrid = calloc(MAP_NODE_NUM,sizeof(ANode*));
              Enemy = calloc(ENEMY_NUM,sizeof(AEnemy*));

              for (int i=0;i<(GROUND_NUM+MAX_WEB_NUM);i++) {
                AGround *newGround = createGround();
                Ground[i] = newGround;
              }

              for (int i=0;i<VGRID_NUM;i++) {
                AVGrid *newVGrid = createVGrid();
                VGrid[i] = newVGrid;
              }

              for (int i=0;i<MAP_NODE_NUM;i++) {
                ANode *newNode = createNode();
                NodeGrid[i] = newNode;
              }

              for (int i=0;i<ENEMY_NUM;i++) {
                AEnemy *newEnemy = createEnemy();
                Enemy[i] = newEnemy;
              }

            } /*else {
              printf("objects not spawned\n");
            }*/
            break;
        }
        column=int_val=int_saved_val=0;//restart values
        row++;

        //checkphase on next row
        if (row>=lim_) {
          if (current_lvl_load_phase_i<LVL_LOAD_PHASE_NUM) {
            low_=lim_;
            current_lvl_load_phase_i++;
            lim_+=load_lvl_jumpmap[current_lvl_load_phase_i];
          }
        }

      }
    } else { //after first 4 rows
      if (c!=';') {//not yet a semicolon
        if (row!=13) {
          if (c>='0' && c<='9') { //numerical chars only
            if (row!=17 && row!=18) {
              int_val=(int)c-'0'; //ascii convert to num
              int_saved_val*=10; //move digit to left
              int_saved_val+=int_val; //append number digit to right side
            } else {
              if (deci) {
                float_val=(float)(c-'0')*0.1;
                float_saved_val+=float_val;
                deci=FALSE;
              } else {
                float_saved_val=c-'0';
                deci=TRUE;
              }
            }
          } else if (c=='-') {
            is_negative_val=TRUE;
          } else if (c==',') {//comma value
            if (is_negative_val) {
              int_saved_val=-abs(int_saved_val);
              is_negative_val=FALSE;
            }

            //====== SET MULTI-VALUES ======
            switch (current_lvl_load_phase_i) { //jumpmapping, start from 1 as 0 is before row 4
              case 1:ground_float_pointer[row-low_][column]=(float)int_saved_val;break; //Ground Float Segment
              case 2:ground_bool_pointer[row-low_][column]=(bool)int_saved_val;break; //Ground Bool Segmnet
              case 3:ground_int_pointer[row-low_][column]=(int)int_saved_val;break; //Ground Int Segment
              case 4: break; //UTF GROUND IS SKIPPED
              case 5:enemy_int_pointer[row-low_][column]=int_saved_val;break; //Enemy Int Segment
              case 6: enemy_float_pointer[row-low_][column]=(float)int_saved_val;break; //EnemyType Float Segment
              case 7: enemy_type_float_pointer[row-low_][column]=float_saved_val;break; //!!true float!! //Enemy Float Segment
              case 8: enemy_type_int_pointer[row-low_][column]=int_saved_val;break;  //EnemyType Int Segment
              case 9: enemy_type_bool_pointer[row-low_][column]=(bool)int_saved_val;break; //EnemyType Bool Segment
              case 10: *player_float_pointer[row-low_]=(float)int_saved_val; break; //Saved Player Float Segment
              case 11:  //Saved Map Attributes Segment
                switch (map_attr_var_type[row-low_]) {
                  case 0:*(bool*)level_attributes[row-low_]=(bool)int_saved_val;break;
                  case 1:*(int*)level_attributes[row-low_]=int_saved_val;break;
                  case 2:*(float*)level_attributes[row-low_]=(float)int_saved_val;break;
                }
                break;
              case 12:  //Game Platform Textures Segment
                switch (row) {
                  case 48: GamePlatformTextures[column].type=int_saved_val;break;
                  case 49: GamePlatformTextures[column].solid_value=int_saved_val;break;
                  case 50: GamePlatformTextures[column].color_id=int_saved_val;break;
                }
                break;
            }


            //printf("%d,",int_saved_val);//save value to arr
            column++;
            is_negative_val=FALSE;
            int_val=int_saved_val=0;//restart values
          }//end of comma value



        } else {//txt characters only (row==13)
          if (c=='"') {
            if (!writing_txt) {
              writing_txt=TRUE;
            } else {//end of string
              writing_txt=FALSE;
              wcsncpy(saved_ground_text[column],txt,512);
              //if (set_txt_size!=0)
                saved_ground_text_size[column]=set_txt_size;
              //else 
                //saved_ground_text_size[column]=16; //default text size
              column++;
              for (int i=0;i<512;i++) //init txt array
                txt[i] = 0;
              char_pos=0;//restart value
              set_txt_size=0;
            }
          } else { //not float quotations
            if (writing_txt) {
              if (!is_calc_utf16) {
                if (c!='{') {
                  txt[char_pos]=c;
                  char_pos++; //next char
                } else { //c=='{'
                  is_calc_utf16=TRUE;
                }
              } else { //calculating utf16
                if (c=='}') {//end of utf16 calc
                  txt[char_pos]=char_utf16;
                  char_pos++;
                  is_calc_utf16=FALSE;//unflag
                  char_utf16=0;//reset value
                } else if (c>='0' && c<='9') { //theres a 'u' inbetween, im not doing anything to that for now
                  char_utf16=char_utf16*16+(c-'0'); //shift digit to left
                } else if (c>='A' && c<='F') {
                  char_utf16=char_utf16*16+(c-'A'+10); //shift digit to left
                }
              }
            } else { //non writing txt, check for size 
              if (c>='0' && c<='9') {
                set_txt_size*=10; //shift digit to left
                set_txt_size+=c-'0';
              }
            }
          }
        }
        //End of characters only
      } else { //semi colon ;
        column=int_val=int_saved_val=0;//restart values
        is_negative_val=FALSE;
        row++;

        //checkphase on next row
        if (row>=lim_) {
          if (current_lvl_load_phase_i<LVL_LOAD_PHASE_NUM) {
            //printf("low_:%d,lim_%d\n",low_,lim_);
            low_=lim_;
            current_lvl_load_phase_i++;
            lim_+=load_lvl_jumpmap[current_lvl_load_phase_i];
          }
        }
        //printf(";\n");
      }
    }
  }
  fclose(fptr);
  return TRUE;
}





void LoadOptions()
{
  if (!FileExists(L"saves/options.txt")) {
      player_color=0;
      player_iris_color=4;
      player_pupil_color=12;
      game_cam_shake=1;
      game_audio=1;
      game_volume=0.2000;
      wav_out_volume=0.5000; //scrapped, later change to music volume
      yes_unifont=1;
      game_shadow=0;
      hide_taskbar=0;
      resolution_choose=1;
      show_fps=0;
      show_hijiri=0;
      is_khmer=0;
      game_hard=0;
      free_will=0;
      return;
  }


  int c,row=0; //each character
  int int_val=0,int_saved_val=0;
  float float_val=0,float_saved_val=0;
  bool deci=FALSE;
  FILE *fptr;
  fptr = _wfopen(L"saves/options.txt",L"r");

  while ((c=fgetwc(fptr))!=WEOF) {
    if (c!=';') {
      if (!(row==5 || row==6)) { //other rows are int/bool values
        if (c>='0' && c<='9') { //numerical chars only
          int_val=c-'0'; //ascii convert to num
          int_saved_val*=10; //move digit to left
          int_saved_val+=int_val; //append number digit to right side
        }
      } else { //row 5 and 6 is float values
        if (c>='0' && c<='9') {
          if (deci) { //post '.'
            float_val=(float)(c-'0')*0.1;
            float_saved_val+=float_val;
          } else { //pre '.'
            float_val=c-'0';
            float_saved_val*=10;
            float_saved_val+=float_val;
          }
        } else if (c=='.') {
          deci=TRUE;
        }
      }
    } else { //assign +  goto next row
      switch (row) {
        case 0:
          player_color=int_saved_val;
          break;
        case 1:
          player_iris_color=int_saved_val;
          break;
        case 2:
          player_pupil_color=int_saved_val;
          break;
        case 3:
          game_cam_shake=int_saved_val;
          break;
        case 4:
          game_audio=int_saved_val;
          break;
        case 5:
          game_volume=float_saved_val;
          break;
        case 6:
          wav_out_volume=float_saved_val;
          break;
        case 7:
          yes_unifont=int_saved_val;
          break;
        case 8:
          game_shadow=int_saved_val;
          break;
        case 9:
          hide_taskbar=int_saved_val;
          break;
        case 10:
          resolution_choose=int_saved_val;
          break;
        case 11:
          show_fps=int_saved_val;
          break;
        case 12:
          show_hijiri=int_saved_val;
          break;
        case 13:
          is_khmer=int_saved_val;
          break;
        case 14:
          game_hard=int_saved_val;
          break;
        case 15:
          free_will=int_saved_val;
          break;
      }
      //printf("row:%d,option:%d,%5.4f\n",row,int_saved_val,float_saved_val);
      deci=FALSE;
      int_saved_val=0;
      float_saved_val=0;
      row++;
    }
  }
  flag_difficulty_change=TRUE;
  fclose(fptr);
}


