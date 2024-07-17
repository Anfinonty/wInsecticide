int GROUND_NUM;
int ENEMY_NUM;
int MAP_WIDTH;
int MAP_HEIGHT;
int GRID_NUM;	
int VGRID_NUM;	
int MAP_NODE_NUM;


//GROUND_NUM
/*double saved_ground_x1[MAX_GROUND_NUM];
double saved_ground_y1[MAX_GROUND_NUM];
double saved_ground_x2[MAX_GROUND_NUM];
double saved_ground_y2[MAX_GROUND_NUM];
double saved_ground_x3[MAX_GROUND_NUM];
double saved_ground_y3[MAX_GROUND_NUM];
bool saved_ground_is_ghost[MAX_GROUND_NUM];
int saved_ground_color[MAX_GROUND_NUM];
int saved_ground_type[MAX_GROUND_NUM];
wchar_t saved_ground_text[MAX_GROUND_NUM][512];*/ //charsize 512


//saved ground attributes pointers
bool *saved_ground_is_ghost;
int *saved_ground_color;
int *saved_ground_type;
double *saved_ground_x1;
double *saved_ground_y1;
double *saved_ground_x2;
double *saved_ground_y2;
double *saved_ground_x3;
double *saved_ground_y3;
wchar_t **saved_ground_text;



//ENEMY_NUM
int saved_enemy_type[MAX_ENEMY_NUM];
double saved_enemy_x[MAX_ENEMY_NUM];
double saved_enemy_y[MAX_ENEMY_NUM];


//Enemy Type (fixed value of 10)
double saved_enemy_type_speed[ENEMY_TYPE_NUM];
double saved_enemy_type_bullet_speed[ENEMY_TYPE_NUM];
int saved_enemy_type_species[ENEMY_TYPE_NUM];
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
bool saved_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM];



//Sound called in the Stack
#define SPAM_SFX_NUM    3
#define KEY_SFX_NUM     6
#define CHANNEL_SFX_NUM 2

wavSoundEffect spamSoundEffect[SPAM_SFX_NUM];
wavSoundEffect keySoundEffect[KEY_SFX_NUM];
wavSoundEffect channelSoundEffect[CHANNEL_SFX_NUM];
wavSoundEffect songAudio;

wavSoundEffectCache spamSoundEffectCache[SPAM_SFX_NUM];
wavSoundEffectCache keySoundEffectCache[KEY_SFX_NUM];
wavSoundEffectCache channelSoundEffectCache[CHANNEL_SFX_NUM];




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
bool is_inverted;

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
      swprintf(indir,256,L"%s/%s",dirname,dir->d_name);
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



void LoadSave(wchar_t *saves_name)
{
  int row=0;
  int column=0;
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
  bool is_calc_utf16=FALSE;

  FILE *fptr;
  fptr = _wfopen(saves_name,L"r");
  int c; //each character

  //init
  writing_txt=FALSE;
  deci=FALSE;
  for (int i=0;i<512;i++) //init txt array
    txt[i] = 0;


  while ((c=fgetwc(fptr))!=WEOF) {
    //wprintf(L"%c",c);
    if (row<=3 || row>=40) { //first 4 rows
      if (c!=';') {//not yet a semicolon
        if (c>='0' && c<='9') { //numerical chars only
          int_val=c-'0'; //ascii convert to num
          int_saved_val*=10; //move digit to left
          int_saved_val+=int_val; //append number digit to right side
          //printf(int_val);
        }
      } else {//semi colon ;
        switch (row) {//save value
          case 0:
            GROUND_NUM=int_saved_val;
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

            //printf("pt:%d\n",sizeof(struct GroundLine*));
            //printf("act:%d\n",sizeof(struct GroundLine));
            Ground = calloc((GROUND_NUM+MAX_WEB_NUM),sizeof(AGround*));
            VGrid = calloc(VGRID_NUM,sizeof(struct AVGrid*));
            NodeGrid = calloc(MAP_NODE_NUM,sizeof(ANode*));
            Enemy = calloc(ENEMY_NUM,sizeof(AEnemy*));
            EnemySprite = calloc(ENEMY_NUM,sizeof(AEnemySprite*));

            //printf("Pointers Made==\n");
            

            for (int i=0;i<(GROUND_NUM+MAX_WEB_NUM);i++) {
              //struct GroundLine* buf = malloc(sizeof(struct GroundLine));
              //printf("buf:%d\n",i);
              AGround *newGround = createGround(VGRID_NUM);
              Ground[i] = newGround;//malloc(sizeof(struct GroundLine));
            }
            

            for (int i=0;i<VGRID_NUM;i++) {
              AVGrid *newVGrid = createVGrid();
              VGrid[i] = newVGrid;
            }
            //printf("Alloc Grids==\n");


            for (int i=0;i<MAP_NODE_NUM;i++) {
              ANode *newNode = createNode();
              NodeGrid[i] = newNode;
            }
            //printf("Alloc Nodes==\n");


            for (int i=0;i<ENEMY_NUM;i++) {
              AEnemy *newEnemy = createEnemy();
              Enemy[i] = newEnemy;
            }


            for (int i=0;i<ENEMY_NUM;i++) {
              AEnemySprite *newEnemySprite = createEnemySprite();
              EnemySprite[i] = newEnemySprite;
            }

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
            custom_map_background_color=color_arr[int_saved_val];
            break;
          case 44:
            is_inverted=(bool)int_saved_val;
            break;
        }
        //printf("\n-%d: %d",row, int_saved_val);
        column=int_val=int_saved_val=0;//restart values
        row++;
      }
    } else { //after first 4 rows
      if (c!=';') {//not yet a semicolon
        if (row!=13) {
          if (c>='0' && c<='9') { //numerical chars only
            if (row!=17 && row!=18) {
              int_val=(int)c-'0'; //ascii convert to num
              //wprintf(L"%c",c);
              int_saved_val*=10; //move digit to left
              int_saved_val+=int_val; //append number digit to right side
            } else {
              if (deci) {
                double_val=(double)(c-'0')*0.1;
                double_saved_val+=double_val;
                //printf("\n-%3.2f",double_saved_val);
                deci=FALSE;
              } else {
                double_saved_val=c-'0';
                deci=TRUE;
              }
            }
          } else if (c==',') {//comma value
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
                case 17: saved_enemy_type_speed[column]=double_saved_val;break;    //NOTE: can be in decimals (0,0.1,0.2...1), subject to change
                case 18: saved_enemy_type_bullet_speed[column]=double_saved_val;break; //NOTE: can be in decimals 0,0.1,0.2...1), subject to change
                case 19: saved_enemy_type_species[column]=int_saved_val;break;
                case 20: saved_enemy_type_follow_range[column]=int_saved_val;break;
                case 21: saved_enemy_type_unchase_range[column]=int_saved_val;break;
                case 22: saved_enemy_type_chase_range[column]=int_saved_val;break;
                case 23: saved_enemy_type_color[column]=int_saved_val;break;
                case 24: saved_enemy_type_speed_multiplier[column]=int_saved_val;break;
                case 25:  saved_enemy_type_health[column]=int_saved_val;break;
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
              }
            }
            //printf("%d,",int_saved_val);//save value to arr
            column++;
            int_val=int_saved_val=0;//restart values
          }



        } else {//txt characters only (row==13)
          if (c=='"') {
            if (!writing_txt) {
              writing_txt=TRUE;
            } else {//end of string
              writing_txt=FALSE;
              //saved_ground_text[column]=txt;
              wcsncpy(saved_ground_text[column],txt,512);
              //printf("%d@@@%s\n",column,saved_ground_text[column]);                
              column++;
              txt[0]='\0';
              char_pos=0;//restart value
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
            } 
          }
        }
        //End of characters only



      } else { //semi colon ;
        //printf(";\n");
        column=int_val=int_saved_val=0;//restart values
        row++;
      }
    }
  }
  fclose(fptr);
  //printf("\n\n\n===Level Loaded===\n\n\n");
}





