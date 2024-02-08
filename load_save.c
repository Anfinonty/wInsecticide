int GROUND_NUM;
int ENEMY_NUM;
int MAP_WIDTH;
int MAP_HEIGHT;
int GRID_NUM;	
int VGRID_NUM;	
int MAP_NODE_NUM;


//GROUND_NUM
double* saved_ground_x1;
double* saved_ground_y1;
double* saved_ground_x2;
double* saved_ground_y2;
double* saved_ground_x3;
double* saved_ground_y3;
bool* saved_ground_is_ghost;
int* saved_ground_color;
int* saved_ground_type;
char **saved_ground_text;

//ENEMY_NUM
int *saved_enemy_type;
double* saved_enemy_x;
double* saved_enemy_y;


//Enemy Type
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

void LoadSave(char *saves_name)
{
  int row=0;
  int column=0;
  int int_val=0;
  int int_saved_val=0;
  
  //for string char
  int char_pos=0;
  char txt[256];
  //char *print_txt;
  bool writing_txt;


  FILE *fptr;
  fptr = fopen(saves_name,"r");
  int c; //each character

  //init
  writing_txt=FALSE;
  for (int i=0;i<256;i++) //init txt array
    txt[i] = 0;


  while ((c=fgetc(fptr))!=EOF) {
    if (row<=3 || row>=40) { //first 4 rows
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

            //Malloc Saved Grounds
            saved_ground_x1=(double*)malloc(GROUND_NUM*sizeof(double));
            saved_ground_y1=(double*)malloc(GROUND_NUM*sizeof(double));
            saved_ground_x2=(double*)malloc(GROUND_NUM*sizeof(double));
            saved_ground_y2=(double*)malloc(GROUND_NUM*sizeof(double));
            saved_ground_x3=(double*)malloc(GROUND_NUM*sizeof(double));
            saved_ground_y3=(double*)malloc(GROUND_NUM*sizeof(double));
            saved_ground_is_ghost=(bool*)malloc(GROUND_NUM*sizeof(bool));
            saved_ground_color=(int*)malloc(GROUND_NUM*sizeof(int));
            saved_ground_type=(int*)malloc(GROUND_NUM*sizeof(int));
            saved_ground_text=(char**)malloc(GROUND_NUM*sizeof(char*));
            break;
          case 1:
            ENEMY_NUM=int_saved_val;

            //Malloc Saved Enemies
            saved_enemy_type=(int*)malloc(ENEMY_NUM*sizeof(int));
            saved_enemy_x=(double*)malloc(ENEMY_NUM*sizeof(double));
            saved_enemy_y=(double*)malloc(ENEMY_NUM*sizeof(double));

            break;
          case 2:
            MAP_WIDTH=int_saved_val;
            break;
          case 3:
            MAP_HEIGHT=int_saved_val;
            GRID_NUM=(MAP_WIDTH/GRID_SIZE) * (MAP_HEIGHT/GRID_SIZE);
            VGRID_NUM=(MAP_WIDTH/VGRID_SIZE) * (MAP_HEIGHT/VGRID_SIZE);
            MAP_NODE_NUM=(MAP_WIDTH/NODE_SIZE) * (MAP_HEIGHT/NODE_SIZE);
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
            int_val=c-'0'; //ascii convert to num
            int_saved_val*=10; //move digit to left
            int_saved_val+=int_val; //append number digit to right side
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
              /*if (row>=17 && row<=17+ENEMY_F64_ATTRIBUTES_NUM) {
                enemy_type_double_pointer[row-17][column]=int_saved_val;//double
              } else if (row>17+ENEMY_F64_ATTRIBUTES_NUM && row<17+ENEMY_F64_ATTRIBUTES_NUM+ENEMY_I64_ATTRIBUTES_NUM) {
                enemy_type_int_pointer[row-(17+ENEMY_F64_ATTRIBUTES_NUM+1)][column]=int_saved_val;//int
              } else { //39
                saved_enemy_type_time_breaker_immune[column]=int_saved_val;//bool
              }*/
              switch (row) {
                case 17: saved_enemy_type_speed[column]=(double)int_saved_val;break;    //NOTE: can be in decimals (0,0.1,0.2...1), subject to change
                case 18: saved_enemy_type_bullet_speed[column]=(double)int_saved_val;break; //NOTE: can be in decimals 0,0.1,0.2...1), subject to change
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
            printf("%d,",int_saved_val);//save value to arr
            column++;
            int_val=int_saved_val=0;//restart values
          }



        } else {//txt characters only (row==13)
          if (c=='"') {
            if (!writing_txt)
              writing_txt=TRUE;
            else //end of string
              writing_txt=FALSE;
              //print_txt=txt;
              //printf(":::%s:::\n",print_txt);//save text
              saved_ground_text[column]=txt;//print_txt;
              column++;
              for (int i=0;i<256;i++) //clear out txt
                txt[i]=0;
              char_pos=0;//restart value
          } else {
            if (writing_txt) {
              txt[char_pos]=c;
              char_pos++; //next char
            } 
          }
        }
        //End of characters only



      } else { //semi colon ;
        printf(";\n");
        column=int_val=int_saved_val=0;//restart values
        row++;
      }
    }
  }
  printf("\n\n\n===Level Loaded===\n\n\n");
  fclose(fptr);
}




























