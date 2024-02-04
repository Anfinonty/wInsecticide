
#define GROUND_NUM 1
#define ENEMY_NUM 2
#define MAP_WIDTH 800
#define MAP_HEIGHT 640
double saved_ground_x1[GROUND_NUM]={2};
double saved_ground_y1[GROUND_NUM]={450};
double saved_ground_x2[GROUND_NUM]={797};
double saved_ground_y2[GROUND_NUM]={412};
double saved_ground_x3[GROUND_NUM]={16};
double saved_ground_y3[GROUND_NUM]={15};
bool saved_ground_is_ghost[GROUND_NUM]={0};
int saved_ground_color[GROUND_NUM]={0};
int saved_ground_type[GROUND_NUM]={0};
char *saved_ground_text[GROUND_NUM]={""};
int saved_enemy_type[ENEMY_NUM]={0,0};
double saved_enemy_x[ENEMY_NUM]={483,537};
double saved_enemy_y[ENEMY_NUM]={306,330};
double saved_enemy_type_speed[ENEMY_TYPE_NUM]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
double saved_enemy_type_bullet_speed[ENEMY_TYPE_NUM]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
int saved_enemy_type_species[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_follow_range[ENEMY_TYPE_NUM]={100,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_unchase_range[ENEMY_TYPE_NUM]={40,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_chase_range[ENEMY_TYPE_NUM]={0,100,100,100,100,100,100,100,100,100};
int saved_enemy_type_color[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_speed_multiplier[ENEMY_TYPE_NUM]={1,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_health[ENEMY_TYPE_NUM]={1,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_shoot_at_player_range[ENEMY_TYPE_NUM]={100,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_aim_rand[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_bullet_cooldown[ENEMY_TYPE_NUM]={1,500,500,500,500,500,500,500,500,500};
int saved_enemy_type_bullet_fire_cooldown[ENEMY_TYPE_NUM]={100,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_bullet_fire_at_once[ENEMY_TYPE_NUM]={1,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_length[ENEMY_TYPE_NUM]={1,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_damage[ENEMY_TYPE_NUM]={1,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_speed_multiplier[ENEMY_TYPE_NUM]={2,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_range[ENEMY_TYPE_NUM]={100,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_color[ENEMY_TYPE_NUM]={0,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_graphics_type[ENEMY_TYPE_NUM]={4,0,0,0,0,0,0,0,0,0};
bool saved_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_time_breaker_rare[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_time_breaker_length[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
double saved_player_x=228,saved_player_y=420;
int map_background=2;
int custom_map_background_color=WHITE;
bool is_inverted=1;
int gamemode=0;
double saved_fb_x1=0.0000,saved_fb_y1=0.0000,saved_fb_x2=0.0000,saved_fb_y2=0.0000;
char *goal_text="";
double version=1.501;
