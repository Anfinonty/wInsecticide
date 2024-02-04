
#define GROUND_NUM 42
#define ENEMY_NUM 6
#define MAP_WIDTH 1920
#define MAP_HEIGHT 1760
float saved_ground_x1[GROUND_NUM]={938,579,786,1390,577,799,1225,957,1251,1218,1009,939,576,780,574,793,1225,1391,3,939,3,3,1543,939,1009,936,958,886,947,720,677,813,707,676,1315,1291,1372,1289,1371,1292,709,674};
float saved_ground_y1[GROUND_NUM]={963,895,583,675,893,1236,1306,895,873,1000,1012,964,898,582,892,1235,1306,676,6,962,2,1756,1088,962,1010,961,1261,1252,1116,713,901,979,962,898,992,1054,1106,1055,1107,1056,961,900};
float saved_ground_x2[GROUND_NUM]={956,786,1388,1547,799,1223,1548,1250,1287,1286,1219,1010,785,1396,798,1225,1543,1917,1917,1249,575,1225,1917,1250,1219,1287,1045,955,1020,920,855,854,814,708,1426,1316,1428,1373,1427,1369,811,853};
float saved_ground_y2[GROUND_NUM]={896,584,675,1096,1237,1306,1082,873,926,929,1000,1012,583,674,1240,1306,1087,4,7,873,896,1305,5,873,1000,927,1277,1261,1123,787,925,928,979,961,1064,992,1065,1106,1064,1103,979,925};
float saved_ground_x3[GROUND_NUM]={15,15,15,15,15,15,15,15,15,15,15,15,2,2,6,6,1915,1544,1391,1286,6,1917,1918,957,1287,1008,1020,948,956,15,15,15,15,15,15,15,15,15,1314,1316,676,813};
float saved_ground_y3[GROUND_NUM]={15,15,15,15,15,15,15,15,15,15,15,15,3,3,1755,1755,1753,1087,678,928,1755,1754,1756,896,926,1011,1124,1115,1266,15,15,15,15,15,15,15,15,15,992,995,898,979};
bool saved_ground_is_ghost[GROUND_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,1,1,1,1};
int saved_ground_color[GROUND_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,7};
int saved_ground_type[GROUND_NUM]={0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,2,0,0,0,0,0,0,0,0,3,3,3,3};
char *saved_ground_text[GROUND_NUM]={"","","","","","","","","","","","","","","","","","","","","","","","","","","","","","Hello World","","","","","","","","","","","",""};
int saved_enemy_type[ENEMY_NUM]={0,1,1,1,1,1};
float saved_enemy_x[ENEMY_NUM]={1115,840,1096,969,1056,1263};
float saved_enemy_y[ENEMY_NUM]={870,1219,1263,622,637,956};
float saved_enemy_type_speed[ENEMY_TYPE_NUM]={1.0,1.0,0.1,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
float saved_enemy_type_bullet_speed[ENEMY_TYPE_NUM]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};
int saved_enemy_type_species[ENEMY_TYPE_NUM]={1,1,0,0,0,0,0,0,0,0};
int saved_enemy_type_follow_range[ENEMY_TYPE_NUM]={100,100,100,1,1,1,1,1,1,1};
int saved_enemy_type_unchase_range[ENEMY_TYPE_NUM]={90,0,0,0,0,0,1,1,1,0};
int saved_enemy_type_chase_range[ENEMY_TYPE_NUM]={1,0,0,0,0,0,100,100,100,0};
int saved_enemy_type_color[ENEMY_TYPE_NUM]={15,0,11,0,0,0,0,0,0,0};
int saved_enemy_type_speed_multiplier[ENEMY_TYPE_NUM]={2,1,4,1,1,1,1,1,1,1};
int saved_enemy_type_health[ENEMY_TYPE_NUM]={40,8,10,1,1,1,1,1,1,1};
int saved_enemy_type_shoot_at_player_range[ENEMY_TYPE_NUM]={100,100,100,1,1,1,1,1,1,1};
int saved_enemy_type_aim_rand[ENEMY_TYPE_NUM]={75,0,640,0,0,0,0,0,0,0};
int saved_enemy_type_bullet_cooldown[ENEMY_TYPE_NUM]={1,15,5,500,500,500,500,500,500,500};
int saved_enemy_type_bullet_fire_cooldown[ENEMY_TYPE_NUM]={200,115,20,0,0,0,0,0,0,0};
int saved_enemy_type_bullet_fire_at_once[ENEMY_TYPE_NUM]={1,1,10,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_length[ENEMY_TYPE_NUM]={1,5,15,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_damage[ENEMY_TYPE_NUM]={1,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_speed_multiplier[ENEMY_TYPE_NUM]={2,1,1,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_range[ENEMY_TYPE_NUM]={100,100,100,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_color[ENEMY_TYPE_NUM]={11,15,11,1,1,1,1,1,1,1};
int saved_enemy_type_bullet_graphics_type[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_snd_dur[ENEMY_TYPE_NUM]={15,10,30,1,1,1,1,1,1,1};
int saved_enemy_type_snd_pitch[ENEMY_TYPE_NUM]={65,65,0,1,1,1,1,1,1,1};
int saved_enemy_type_snd_rand[ENEMY_TYPE_NUM]={5,5,126,1,1,1,1,1,1,1};
int saved_enemy_type_death_snd_dur[ENEMY_TYPE_NUM]={20,30,30,30,30,30,30,30,30,30};
int saved_enemy_type_death_snd_pitch[ENEMY_TYPE_NUM]={0,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_death_snd_rand[ENEMY_TYPE_NUM]={126,126,126,126,126,126,126,126,126,126};
int saved_enemy_type_time_breaker_rare[ENEMY_TYPE_NUM]={600,0,0,0,0,0,0,0,0,0};
int saved_enemy_type_time_breaker_length[ENEMY_TYPE_NUM]={15000,0,0,0,0,0,0,0,0,0};
bool saved_enemy_type_time_breaker_immune[ENEMY_TYPE_NUM]={1,0,0,0,0,0,0,0,0,0};
float saved_player_x=961,saved_player_y=1253;
int map_background=2;
int custom_map_background_color=DKGRAY;
bool is_inverted=FALSE;
int gamemode=1;
float saved_fb_x1=0.0000,saved_fb_y1=0.0000,saved_fb_x2=0.0000,saved_fb_y2=0.0000;
char *goal_text="Slay the White Crawler";
float version=1.501;
