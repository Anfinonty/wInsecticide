
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
int SHADOW_GRID_NUM;

int RD_DYN_WIDTH;
int RD_DYN_HEIGHT;
int RDGRID_DYN_NUM;

int MAP_NODE_NUM;



//Dynamic Arrays holding saved values
bool *saved_ground_is_ghost;
int *saved_ground_color;
int *saved_ground_type;
int *saved_ground_text_size;

int *saved_enemy_type;//[MAX_ENEMY_NUM];
double *saved_enemy_x;//[MAX_ENEMY_NUM];
double *saved_enemy_y;//[MAX_ENEMY_NUM];


double *saved_ground_x1;
double *saved_ground_y1;
double *saved_ground_x2;
double *saved_ground_y2;
double *saved_ground_x3;
double *saved_ground_y3;
wchar_t **saved_ground_text;



//Enemy Type (fixed value of 10)
double saved_enemy_type_speed[ENEMY_TYPE_NUM];
double saved_enemy_type_bullet_speed[ENEMY_TYPE_NUM];
int saved_enemy_type_species[ENEMY_TYPE_NUM];
int saved_large_enemy_type_species[ENEMY_TYPE_NUM];
int saved_larger_enemy_type_species[ENEMY_TYPE_NUM];
int saved_enemy_type_follow_range[ENEMY_TYPE_NUM];
int saved_enemy_type_unchase_range[ENEMY_TYPE_NUM];
int saved_enemy_type_chase_range[ENEMY_TYPE_NUM];
int saved_enemy_type_color[ENEMY_TYPE_NUM];
int saved_enemy_type_speed_multiplier[ENEMY_TYPE_NUM];
int saved_enemy_type_health[ENEMY_TYPE_NUM];
int saved_enemy_type_shoot_at_player_range[ENEMY_TYPE_NUM];
int saved_enemy_type_aim_rand[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_cooldown[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_fire_cooldown[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_fire_at_once[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_length[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_damage[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_speed_multiplier[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_range[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_color[ENEMY_TYPE_NUM];
int saved_enemy_type_bullet_graphics_type[ENEMY_TYPE_NUM];
int saved_enemy_type_time_breaker_rare[ENEMY_TYPE_NUM];
int saved_enemy_type_time_breaker_length[ENEMY_TYPE_NUM];
int saved_enemy_type_rot_sprite_id[ENEMY_TYPE_NUM];
int saved_enemy_type_rot_xsprite_id[ENEMY_TYPE_NUM];
bool saved_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];
bool saved_large_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];
bool saved_larger_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];




double set_enemy_type_speed[ENEMY_TYPE_NUM];
double set_enemy_type_bullet_speed[ENEMY_TYPE_NUM];
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
bool set_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];


/*double *enemy_type_double_pointer[ENEMY_F64_ATTRIBUTES_NUM]=
{
  saved_enemy_type_speed,
  saved_enemy_type_bullet_speed
};


int* enemy_type_int_pointer[ENEMY_I64_ATTRIBUTES_NUM]=
{
  saved_enemy_type_species,
  saved_enemy_type_follow_range,
  saved_enemy_type_unchase_range,
  saved_enemy_type_chase_range,
  saved_enemy_type_color,
  saved_enemy_type_speed_multiplier,
  saved_enemy_type_health,
  saved_enemy_type_shoot_at_player_range,
  saved_enemy_type_aim_rand,
  saved_enemy_type_bullet_cooldown,
  saved_enemy_type_bullet_fire_cooldown,
  saved_enemy_type_bullet_fire_at_once,
  saved_enemy_type_bullet_length,
  saved_enemy_type_bullet_damage,
  saved_enemy_type_bullet_speed_multiplier,
  saved_enemy_type_bullet_range,
  saved_enemy_type_bullet_color,
  saved_enemy_type_bullet_graphics_type,
  saved_enemy_type_time_breaker_rare,
  saved_enemy_type_time_breaker_length
};*/

double saved_player_x;
double saved_player_y;
int map_background;
int custom_map_background_color;
int custom_map_background_color_i;
bool is_moon;

bool is_shadows=FALSE;
int map_weather=0;

int rain_sound_duration=0;
double shadow_grad_rise=2;
double shadow_grad_run=1;
double rain_grad_rise=1;
double rain_grad_run=1;


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
  double double_val;
  double double_saved_val;
  
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
            saved_enemy_x=calloc(ENEMY_NUM,sizeof(double));
            saved_enemy_y=calloc(ENEMY_NUM,sizeof(double));


            saved_ground_x1=calloc(GROUND_NUM,sizeof(double));
            saved_ground_y1=calloc(GROUND_NUM,sizeof(double));
            saved_ground_x2=calloc(GROUND_NUM,sizeof(double));
            saved_ground_y2=calloc(GROUND_NUM,sizeof(double));
            saved_ground_x3=calloc(GROUND_NUM,sizeof(double));
            saved_ground_y3=calloc(GROUND_NUM,sizeof(double));
            saved_ground_text=calloc(GROUND_NUM,sizeof(wchar_t*));
            for (int i=0;i<GROUND_NUM;i++) {
              saved_ground_text[i]=(wchar_t*)malloc(512*sizeof(wchar_t));
            }



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
              /*for (int i=0;i<ENEMY_NUM;i++) {
                AEnemySprite *newEnemySprite = createEnemySprite();
                EnemySprite[i] = newEnemySprite;
              }*/
            } /*else {
              printf("objects not spawned\n");
            }*/

            break;
          case 40:
            saved_player_x=(double)int_saved_val;
            break;
          case 41:
            saved_player_y=(double)int_saved_val;
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
                double_val=(double)(c-'0')*0.1;
                double_saved_val+=double_val;
                deci=FALSE;
              } else {
                double_saved_val=c-'0';
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
            //ground
            if (row>=4 && row<=12) {
              switch (row) {
                case 4: saved_ground_x1[column]=(double)int_saved_val; break;
                case 5: saved_ground_y1[column]=(double)int_saved_val; break;
                case 6: saved_ground_x2[column]=(double)int_saved_val; break;
                case 7: saved_ground_y2[column]=(double)int_saved_val; break;
                case 8: saved_ground_x3[column]=(double)int_saved_val; break;
                case 9: saved_ground_y3[column]=(double)int_saved_val; break;
                case 10: saved_ground_is_ghost[column]=(bool)int_saved_val; break;
                case 11: saved_ground_color[column]=int_saved_val; break;
                case 12: saved_ground_type[column]=int_saved_val; break;
              }
            } else if (row>=14 && row<=16) {
            //enemy
              switch (row) {
                case 14:saved_enemy_type[column]=int_saved_val; //int
                case 15:saved_enemy_x[column]=(double)int_saved_val; //double
                case 16:saved_enemy_y[column]=(double)int_saved_val; //double
              }
            } else { //17
            //enemy type
              switch (row) {
                case 17: saved_enemy_type_speed[column]=double_saved_val;break;    //NOTE: can be in decimals 
                case 18: saved_enemy_type_bullet_speed[column]=double_saved_val;break; //NOTE: can be in decimals 
                case 19: saved_enemy_type_species[column]=int_saved_val;break;
                case 20: saved_enemy_type_follow_range[column]=int_saved_val;break;
                case 21: saved_enemy_type_unchase_range[column]=int_saved_val;break;
                case 22: saved_enemy_type_chase_range[column]=int_saved_val;break;
                case 23: saved_enemy_type_color[column]=int_saved_val;break;
                case 24: saved_enemy_type_speed_multiplier[column]=int_saved_val;break;
                case 25: saved_enemy_type_health[column]=int_saved_val;break;
                case 26: saved_enemy_type_shoot_at_player_range[column]=int_saved_val;break;
                case 27: saved_enemy_type_aim_rand[column]=int_saved_val;break;
                case 28: saved_enemy_type_bullet_cooldown[column]=int_saved_val;break;
                case 29: saved_enemy_type_bullet_fire_cooldown[column]=int_saved_val;break;
                case 30: saved_enemy_type_bullet_fire_at_once[column]=int_saved_val;break;
                case 31: saved_enemy_type_bullet_length[column]=int_saved_val;break;
                case 32: saved_enemy_type_bullet_damage[column]=int_saved_val;break;
                case 33: saved_enemy_type_bullet_speed_multiplier[column]=int_saved_val;break;
                case 34: saved_enemy_type_bullet_range[column]=int_saved_val;break;
                case 35: saved_enemy_type_bullet_color[column]=int_saved_val;break;
                case 36: saved_enemy_type_bullet_graphics_type[column]=int_saved_val;break;
                case 37: saved_enemy_type_time_breaker_rare[column]=int_saved_val;break;
                case 38: saved_enemy_type_time_breaker_length[column]=int_saved_val;break;
                case 39: saved_enemy_type_time_breaker_immune[column]=int_saved_val;break;
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
          } else { //not double quotations
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
  double double_val=0,double_saved_val=0;
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
      } else { //row 5 and 6 is double values
        if (c>='0' && c<='9') {
          if (deci) { //post '.'
            double_val=(double)(c-'0')*0.1;
            double_saved_val+=double_val;
          } else { //pre '.'
            double_val=c-'0';
            double_saved_val*=10;
            double_saved_val+=double_val;
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
          game_volume=double_saved_val;
          break;
        case 6:
          wav_out_volume=double_saved_val;
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
      //printf("row:%d,option:%d,%5.4f\n",row,int_saved_val,double_saved_val);
      deci=FALSE;
      int_saved_val=0;
      double_saved_val=0;
      row++;
    }
  }
  flag_difficulty_change=TRUE;
  fclose(fptr);
}


