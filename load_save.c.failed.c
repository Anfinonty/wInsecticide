//Save Level Attribute Row Values
#define S_PRELUDE_ROW_NUM  4


#define S_PLAYER_FLOAT_NUM   2

#define S_LEVEL_ATTR_NUM  4 //deprecate 47,48 //6 

#define S_TEXTURE_INT_NUM  3



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
  ground_bool_pointer[0]=saved_ground_is_ghost;
}


int *saved_ground_color;
int *saved_ground_type;
int *saved_ground_text_size;
#define S_GROUND_INT_NUM 2
int *ground_int_pointer[S_GROUND_INT_NUM];
void InitGroundIntPointer()
{
  ground_int_pointer[0]=saved_ground_color;
  ground_int_pointer[1]=saved_ground_type;
  ground_int_pointer[2]=saved_ground_text_size;
}


wchar_t **saved_ground_text;
#define S_GROUND_UTF_NUM 1
//UNIQUE, NO POINTER


//===Enemy===
int *saved_enemy_type;
#define S_ENEMY_INT_NUM    1
int *enemy_int_pointer[S_ENEMY_INT_NUM];
void InitEnemyIntPointer()
{
  enemy_int_pointer[0]=saved_enemy_type;
}

float *saved_enemy_x;
float *saved_enemy_y;
#define S_ENEMY_FLOAT_NUM    2
float *enemy_float_pointer[S_ENEMY_FLOAT_NUM];
void InitEnemyFloatPointer()
{
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




//Level Player Attributes 
float saved_player_x;
float saved_player_y;



//Level Background Attributes
int map_background;
int custom_map_background_color;
int custom_map_background_color_i;
bool is_moon;
bool is_shadows=FALSE;
int map_weather=0;
int rain_sound_duration=0;
float shadow_grad_rise=2;
float shadow_grad_run=1;
float rain_grad_rise=1;
float rain_grad_run=1;


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


int SAVE_ATTR_ROW_NUM[13]={
  S_PRELUDE_ROW_NUM,
  S_GROUND_FLOAT_NUM, //
  S_GROUND_INT_NUM,
  S_GROUND_FLOAT_NUM, //
  S_GROUND_UTF_NUM,
  S_ENEMY_INT_NUM, //
  S_ENEMY_FLOAT_NUM,
  S_ENEMY_TYPE_FLOAT_NUM, //
  S_ENEMY_TYPE_INT_NUM,
  S_ENEMY_TYPE_BOOL_NUM, //
  S_PLAYER_FLOAT_NUM,
  S_LEVEL_ATTR_NUM, //
  S_TEXTURE_INT_NUM
};


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


  //int current_lim_i=0,next_lim_i=0,low=0,lim=0;
  //low = SAVE_ATTR_ROW_NUM[current_lim_i];
  //next_lim_i=current_lim_i+1;
  //if (next_lim_i<13) {
    //lim = low + SAVE_ATTR_ROW_NUM[next_lim_i];
  // } else {
    //lim=low;
  //
  //if (row==lim) {
    //current_lim_i++;
  //}
  int current_lim_i=0,next_lim_i=0,low=0,lim=0;
  low = SAVE_ATTR_ROW_NUM[current_lim_i];
  next_lim_i=current_lim_i+1;
  lim = low + SAVE_ATTR_ROW_NUM[next_lim_i];

  while ((c=fgetwc(fptr))!=WEOF) {
    if (row<=3 || (row>=40 && row!=46 && row!=48 && !(row>=49 && row<=51))) { //first 4 rows
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
            InitGroundBoolPointer();
            InitGroundIntPointer();
            InitEnemyIntPointer();
            InitEnemyFloatPointer();


            if (spawn_objects) {
              Ground = calloc((GROUND_NUM+MAX_WEB_NUM),sizeof(AGround*));
              VGrid = calloc(VGRID_NUM,sizeof(struct AVGrid*));
              NodeGrid = calloc(MAP_NODE_NUM,sizeof(ANode*));
              Enemy = calloc(ENEMY_NUM,sizeof(AEnemy*));
              //EnemySprite = calloc(ENEMY_NUM,sizeof(AEnemySprite*));
            

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
            } 

            break;
          case 40:
            saved_player_x=(float)int_saved_val;
            break;
          case 41:
            saved_player_y=(float)int_saved_val;
            break;
          case 42:
            map_background=int_saved_val;
            break;
          case 43:
            custom_map_background_color_i=int_saved_val;
            custom_map_background_color=rgbPaint[int_saved_val];//color_arr[int_saved_val];
            break;
          case 44:
            is_moon=(bool)int_saved_val;
            break;
          case 45: //is_raining
            map_weather=(int)int_saved_val;
            break;
          case 47: //is_shadow
            is_shadows=(bool)int_saved_val;
            break;
        }
        column=int_val=int_saved_val=0;//restart values
        row++;


        if (row>=lim)
          current_lim_i++;
        low = SAVE_ATTR_ROW_NUM[current_lim_i];
        next_lim_i=current_lim_i+1;
        if (next_lim_i<13) {
          lim = low + SAVE_ATTR_ROW_NUM[next_lim_i];
        } else {
          lim=low;
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
          
            if (row>=low && row<lim) { //Ground Float Segment
              ground_float_pointer[row-low][column]=(float)int_saved_val;

            } else if (row>=low && row<lim) { //Ground Bool Segmemt
              ground_bool_pointer[row-low][column]=(bool)int_saved_val;

            } else if (row>=low && row<lim) { //Ground Int Segment
              ground_int_pointer[row-low][column]=int_saved_val;

            } else if (row>=low && row<lim) { //Enemy Int Segment
              enemy_int_pointer[row-low][column]=int_saved_val;

            } else if (row>=low && row<lim) { //Enemy Float Segment
              enemy_float_pointer[row-low][column]=(float)int_saved_val;

            } else if (row>=low && row<lim) { //EnemyType Float Segment
              enemy_type_float_pointer[row-low][column]=(float)int_saved_val;

            } else if (row>=low && row<lim) { //EnemyType Int Segment
              enemy_type_int_pointer[row-low][column]=int_saved_val;

            } else if (row>=low && row<lim) { //EnemyType Bool Segment
              enemy_type_bool_pointer[row-low][column]=(bool)int_saved_val;
            }

            switch (row) {
              case 46: 
                if (column==0) {
                  rain_grad_rise=int_saved_val;
                } else {
                  rain_grad_run=int_saved_val;
                  if (rain_grad_run==0) {
                    rain_grad_run=1;
                  }
                }
                break;
              case 48: 
                if (column==0) {
                  shadow_grad_rise=int_saved_val;
                } else {
                  shadow_grad_run=int_saved_val;
                  if (shadow_grad_run==0) {
                    shadow_grad_run=1;
                  }
                }
                break;
              case 49: GamePlatformTextures[column].type=int_saved_val;break;
              case 50: GamePlatformTextures[column].solid_value=int_saved_val;break;
              case 51: GamePlatformTextures[column].color_id=int_saved_val;break;
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

        if (row>=lim)
          current_lim_i++;
        low = SAVE_ATTR_ROW_NUM[current_lim_i];
        next_lim_i=current_lim_i+1;
        if (next_lim_i<13) {
          lim = low + SAVE_ATTR_ROW_NUM[next_lim_i];
        } else {
          lim=low;
        }
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
      wav_out_volume=0.5000; //scrapped
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


