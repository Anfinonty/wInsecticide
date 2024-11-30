//classes for game
typedef struct GroundLine
{
  bool is_ghost; //Can be colided or not collided
  bool within_render_distance; //Is the ground within the player's Render Distance Grid'


  int health; //For Webs, health of ground
  int color_id;
  int color; //Color of ground, RGB()
  int type; //Regular, Text ground or TriFill 
  int font_size;

  double x1;
  double y1;
  double x2;
  double y2;
  double x3;
  double y3; //3 Axes of ground
  //double sprite_x1,sprite_y1,sprite_x2,sprite_y2,sprite_x3,sprite_y3,//graphics 3 Axes of ground
  double length; // = sqrt(x^2 + y^2)) // Babylonian Trigonometry Theorem for Calculating the Hypothenuse
  double gradient;// y of ground / x of ground
  double c; //The C in y-mx_c
  //double height_from_player_x;//= gradient * player_x + c
  double angle; // = ACos of (x/length)

  wchar_t text[512]; //for type 1 which is Text_ground

//  int already_in_grid_id[MAX_GRID_IN_GROUND];
//  bool already_in_grid[MAX_GRID_IN_GROUND]; //MAX VGRID NUM
//  int saved_pos_in_grid_id[MAX_GRID_IN_GROUND];
//  int saved_pos_in_grid[MAX_GRID_IN_GROUND]; //MAX VGRID NUM
  bool* already_in_grid; //MAX VGRID NUM
  int* saved_pos_in_grid; //MAX VGRID NUM
} AGround;
//} Ground[GROUND_NUM+MAX_WEB_NUM];
//struct GroundLine Ground[MAX_GROUND_NUM+MAX_WEB_NUM];

//https://stackoverflow.com/questions/48222554/c-creating-a-struct-array-beyond-certain-size-causes-a-crash
AGround *createGround(int vgrid_num)
{
  AGround *toReturn = malloc(sizeof(AGround));
  if (vgrid_num>MAX_VGRID_NUM) {
    vgrid_num=MAX_VGRID_NUM;
  }
  toReturn->already_in_grid = malloc(vgrid_num*sizeof(bool));
  toReturn->saved_pos_in_grid = malloc(vgrid_num*sizeof(int));
  return toReturn;
}


void freeGround(AGround *myGround)
{
  if (myGround->already_in_grid != NULL)
    free(myGround->already_in_grid);    
  if (myGround->saved_pos_in_grid != NULL)
    free(myGround->saved_pos_in_grid);
  if (myGround)
    free(myGround);
}

AGround **Ground;



void InitGround(bool is_max);
void InitGround2();
double GetLineTargetAngle(int Ground_id,double x,double y);
double GetLineTargetHeight(int Ground_id,double E,double x,double y);
void SetGround(int i);
int GetOnGroundId(double x,double y,double min_range_1,double min_range_2);
void DestroyGround(int i);

void DrawWebs(HDC hdc);
void DrawGround(HDC hdc);
void DrawGroundText(HDC hdc);
void DrawGroundTriFill(HDC hdc);



//For map editor
struct RenderDistanceGrid
{
  int x;
  int y;
} RDGrid[RDGRID_NUM]; //9*9


//int rendered_grid_num;
//int render_grids[RDGRID_NUM];
int rendered_ground_num;
int *render_grounds;
//int render_grounds[MAX_GROUND_NUM];




struct player 
{
  bool is_rebounding;
  bool is_swinging;
  //bool hiding;
  bool left_click;
  bool right_click;
  bool rst_left_click;
  bool rst_right_click;

  bool rst_down;
  bool rst_left;
  bool rst_right;
  bool rst_up;

  bool rst_arrow_down;
  bool rst_arrow_left;
  bool rst_arrow_right;
  bool rst_arrow_up;

  bool rst_right_click_snd;

  //bool rst_key_sprint;
  bool last_left;
  bool jump;
  bool current_above;
  bool current_below;
  bool previous_above;
  bool previous_below;
  bool print_current_above;
  bool print_current_below;
  bool time_breaker;
  bool attack;
  bool blocking;
  bool print_valid_web;
  bool valid_web;
  bool attack_rst;
  bool destroy_ground;
  bool uppercut;
  bool flag_revert_palette;
  bool on_a_ground;
  bool low_jump;

  bool is_on_ground_edge;
  //bool is_on_ground_edge2;
  bool is_on_left_ground_edge;
  bool is_on_right_ground_edge;
 
  bool in_water;
  bool phase_web;
  bool phase_web2;
  bool hiding;
  bool in_web_corner;
  bool key_b4_corner_is_left;
  bool key_b4_corner_is_right;
  //Render
  //int rendered_vgrid_num;
  //int rendered_enemy_num;

 // int rendered_ground_num;
  int play_gun_snd;
  int knives_per_throw;
  int invalid_shoot_timer;

  int block_recharge_timer_max;
  int block_recharge_timer;
  int block_cooldown_max;
  int block_cooldown;

  int left_click_hold_timer;
  int right_click_hold_timer;
  int attack_timer;
  int sleep_timer;
  int hit_cooldown_timer;
  int block_timer;
  int cdweb_pos;
  int cdweb_player_pos;
  int cdweb_num;
  int destroyed_web_pos;
  int max_web_num;
  int bullet_shot;
  int placed_web_pos;
  int placed_web_num;
  int web_being_shot;

  int previous_web_placed;
  int time_breaker_units;
  int time_breaker_units_max;
  int time_breaker_cooldown;
  int time_breaker_cooldown_max;
  int time_breaker_recharge_timer;
  int time_breaker_recharge_timer_max;
  int time_breaker_units_tick;
  int time_breaker_units_tick_max;
  int spin_timer;

  int bullet_shot_num;
  int color;
  int load_color;

  int grav;
  int jump_height;
  int sprite_timer;
  int in_air_timer;
  int below_ground_edge_timer;
  int speed;
  int on_ground_timer;
  int on_ground_id;
  //int on_ground_id_u1;
  int on_ground_id_u2;
  //int on_ground_id_d1;
  //int on_ground_id_d2;
  int saved_ground_id;
  int walk_cycle;
  int player_jump_height;
  //int key_jump_timer;
  int knockback_speed_multiplier;
  int knockback_strength;
  int time_breaker_tick;
  int decceleration_timer;
  int seed;
  int fast_duration;
  int shoot_knife_duration;

  int on_ground_edge_id;
  int saved_on_ground_edge_id;
  int pivot_on_ground_id;
  int show_exp_timer;
  int show_block_health_timer;
  int show_health_timer;

  int current_blur_sprite;

  int rain_wet_timer;
  int visible_rain_wet_timer;
  int exp;

  int blur_timer[PLAYER_BLUR_NUM];
  int blur_x[PLAYER_BLUR_NUM];
  int blur_y[PLAYER_BLUR_NUM];
  int blur_sprite_x[PLAYER_BLUR_NUM];
  int blur_sprite_y[PLAYER_BLUR_NUM];

  //=== JUST A DEMO===
  /*int sprite_width;
  int sprite_height;
  int sprite_minx;
  int sprite_miny;
  int sprite_maxx;
  int sprite_maxy;
  int current_draw_row;*/

//1
//Find size of rotated bitmap from rotating SpriteA
//2
//Create Empty bitmap (cache_rot) SpriteB
//3 
//place each line of rotation from SpriteA to SpriteB
//4
//SpriteC = SpriteB

  //int render_enemies[21];
  //int render_grounds[342];
  //int *render_enemies;
  //int *render_grounds;
  double knockback_speed;
  double attack_strength;
  double saved_x;
  double saved_y;
  double x;
  double y;
  double below_x1;
  double below_y1;
  double below_x2;
  double below_y2;
  double above_x;
  double above_y;
  double above_x1;
  double above_y1;
  double above_x2;
  double above_y2;
  double sprite_angle;
  double angle;
  double saved_angle;
  double saved_sprite_angle;
  double saved_block_sprite_angle;
  double saved_attack_sprite_angle;
  double player_grav;

  double cam_x;
  double cam_y;
  double cam_move_x;
  double cam_move_y;
  double cam_mouse_move_x;
  double cam_mouse_move_y;
  double sprite_x;
  double sprite_y;
  double claws_x;
  double claws_y;
  double claws_attack_x;
  double claws_attack_y;
  //double cam_edge_move_x;
  //double cam_edge_move_y;

  double block_health_max;
  double block_health;


  double health;
  double pivot_x;
  double pivot_y;
  double pivot_angle;
  double pivot_length;
  double jump_angle;
  double jump_angle2;
  double fling_distance;
  double angle_of_incidence;
  double angle_of_reflection;

  double mouse_angle;
  double mouse_dist;


  int web_storage[MAX_WEB_NUM];
  int cdwebs[MAX_WEB_NUM];
  //int render_vgrids[VRDGRID_NUM];
  int bullet[PLAYER_BULLET_NUM];


//
  HBITMAP osprite_1;
  HBITMAP osprite_2;
  HBITMAP osprite_jump;
  HBITMAP oattack_sprite_1;
  HBITMAP oattack_sprite_2;
  HBITMAP oattack_sprite_3;
  HBITMAP oattack_sprite_4;
  HBITMAP oblock_sprite_1;
  HBITMAP oblock_sprite_2;
  HBITMAP oblock_sprite_3;
  HBITMAP ospin_sprite;



//sprites
  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_jump;
  HBITMAP attack_sprite_1;
  HBITMAP attack_sprite_2;
  HBITMAP attack_sprite_3;
  HBITMAP attack_sprite_4;
  HBITMAP block_sprite_1;
  HBITMAP block_sprite_2;
  HBITMAP block_sprite_3;
  HBITMAP spin_sprite;


//sprite cache to be displayed
  HBITMAP sprite_1_cache;
  HBITMAP sprite_2_cache;
  HBITMAP sprite_jump_cache;
  HBITMAP attack_sprite_1_cache;
  HBITMAP attack_sprite_2_cache;
  HBITMAP attack_sprite_3_cache;
  HBITMAP attack_sprite_4_cache;
  HBITMAP block_sprite_1_cache;
  HBITMAP block_sprite_2_cache;
  HBITMAP block_sprite_3_cache;
  HBITMAP spin_sprite_1_cache;
  HBITMAP spin_sprite_2_cache;
  HBITMAP spin_sprite_3_cache;
  HBITMAP spin_sprite_4_cache;

  //blur sprite caches
  HBITMAP blur_sprite_jump_cache;
  HBITMAP spin_blur_sprite_1_cache;
  HBITMAP spin_blur_sprite_2_cache;
  HBITMAP spin_blur_sprite_3_cache;
  HBITMAP spin_blur_sprite_4_cache;


//sprite to be drawn
  DRAWSPRITE draw_sprite_1;
  DRAWSPRITE draw_sprite_2;

  DRAWSPRITE draw_sprite_jump;

  DRAWSPRITE draw_attack_sprite_1;
  DRAWSPRITE draw_attack_sprite_2;
  DRAWSPRITE draw_attack_sprite_3;
  DRAWSPRITE draw_attack_sprite_4;

  DRAWSPRITE draw_block_sprite_1;
  DRAWSPRITE draw_block_sprite_2;
  DRAWSPRITE draw_block_sprite_3;

  DRAWSPRITE draw_spin_sprite_1;
  DRAWSPRITE draw_spin_sprite_2;
  DRAWSPRITE draw_spin_sprite_3;
  DRAWSPRITE draw_spin_sprite_4;


  //blur sprites
  DRAWSPRITE draw_blur_sprite_jump;
  DRAWSPRITE draw_spin_blur_sprite_1;
  DRAWSPRITE draw_spin_blur_sprite_2;
  DRAWSPRITE draw_spin_blur_sprite_3;
  DRAWSPRITE draw_spin_blur_sprite_4;
} player;


struct PlayerFlingWeb {
  //player shot web attrobutes
  //int length;
  //int on_ground_id[64];
  //double angle[64];
  double x[PLAYER_FLING_WEB_NUM];
  double y[PLAYER_FLING_WEB_NUM];
  double sprite_x[PLAYER_FLING_WEB_NUM];
  double sprite_y[PLAYER_FLING_WEB_NUM];
} player_fling_web;

void InitPlayerFlingWeb();

//int player_render_enemies[MAX_ENEMY_NUM];


void InitPlayer();
//bool IsInvertedBackground();
bool IsSpeedBreaking();
bool IsCollideCrawler(double x1,double y1,double x2,double y2,double gradient,double c);
void Click();
void move_x(double x);
void move_y(double y);
void InitRDGrid();
void InitVRDGrid();
void CameraInit(double x,double y);
void InitPlayerCamera();
//void PlayerBulletLimitAct();
//bool NearCrawler();
void CleanUpPlayer();
bool YesInitRDGrid();
bool YesInitVRDGrid();
void RegainWeb(int web_id);
void PlayerAct();
void PlayerPlaceWeb();
void PlayerCameraShake();
void DrawPlayer(HDC hdc);



typedef struct vgrid
{
  bool within_render_distance;
  int x1;
  int y1;
  int x2;
  int y2; //grid axes
  int max_ground_num; //how many grounds are occupying this grid
  int ground_ids[MAX_GROUNDS_WITHIN_GRID]; //id of grounds that are occupying this grid
} AVGrid;


AVGrid *createVGrid()
{
  AVGrid *toReturn = malloc(sizeof(AVGrid));
  return toReturn;
}


void freeVGrid(AVGrid *myVGrid)
{
  if (myVGrid)
    free(myVGrid);
}


AVGrid **VGrid;





typedef struct node
{
  bool node_solid;
  bool node_water;
  bool tmp_wet;
  bool node_no_rain;
  bool node_no_shade;
  bool non_web;
  int x1;
  int y1;
  int x2;
  int y2;
} ANode;


ANode *createNode()
{
  ANode *toReturn = malloc(sizeof(ANode));
  return toReturn;
}


void freeNode(ANode *myNode)
{
  if (myNode)
    free(myNode);
}



ANode **NodeGrid;



int GetGridId(int x,int y,int width, int size,int max);
void SetGridLineArray(int grid_id,int ground_id);
void UnSetGridLineArray(int grid_id,int ground_id);
void InitGrid();
void DrawGrid(HDC hdc);
void InitNodeGrid();
bool IsCollideSolid(double x1,double y1,double x2,double y2,double gradient,double c);
bool IsHasNeighbours(int nx,int ny);
void SetNodeGridAttributes2(int i);
void SetNodeGridAttributes(int i);
void InitNodeGridAttributes();














int current_bullet_id=0;
struct Bullet
{
  bool shot;
  bool near_miss;
  bool playsnd;
  int color;
  int speed_multiplier;
  int from_enemy_id;
  int graphics_type;
  int saved_pos;
  int saved_ground_id;
  int saved_node_grid_id;
  double damage;
  double sprite_x;
  double sprite_y;
  double start_x;
  double start_y;
  double x;
  double y;
  double angle;
  double speed;
  double range;
  double start_range;

} Bullet[BULLET_NUM];

void InitBullet();
void ShootBullet(
  int bullet_id,
  int saved_pos,
  int color,
  int graphics_type,
  int range,
  double speed,
  int speed_multiplier,
  double damage,
  int enemy_id,
  double start_x,
  double start_y,
  double source_x,
  double source_y,
  double target_x,
  double target_y,
  double off_angle);

bool HitPlayer(int bullet_id,int r1,int r2);
void StopBullet(int bullet_id,bool is_player);
void BulletAct(int bullet_id);
void DrawBullet2(HDC hdc,int i,double x,double y,int color);
void DrawBullet(HDC hdc,int i);













typedef struct enemy
{
  //Line Of Sight bullet


  //Enemy Attributes
  //Enemy Node Attributes

  bool being_drawn;
  bool saw_player; //:Enemy seen the player, player was in line of sight seconds ago.
  bool last_left; //:Enemy was going to the left
  bool target_player; //:Enemy is actively chasing the player
  bool ignore_player; //:Enemy is not chasing the player due to constraints or in cooldown
  bool idling; //:Enemy is standing still
  bool found_target; //:Enemy has found the target node to travel to
  bool search_target; //:Enemy is searching for a target node to travel to
  bool trace_target; //:Enemy is currently drawing a path to the target node to travel to
  bool move_to_target; //:Emeny is currently moving towards the target node to travel to
  bool player_at_left; //:player is on the left side of the enemy
  bool player_at_right; //:player is on the right side of the enemy
  bool player_at_above; //:player is above the enemy
  bool player_at_below; //:player is below the enemy
  bool within_render_distance; //:Enemy is within the render distance
  bool above_ground; //:For crawlers: crawler is above the ground
  bool below_ground; //:For crawlers: crawler is upside down on a ground
  bool previous_above_ground; //:For crawlers: crawler is above the ground
  bool previous_below_ground; //:For crawlers: crawler is upside down on a ground
  bool in_chase_range; //:player is within enemy's chase range
  bool in_unchase_range; //:player is within enemy's unchase range
  bool time_breaker_immune; //:Enemy is ummune to the player's time breaker effect
  bool LOS_left;
  bool LOS_shot;
  bool flip_sprite;
  bool play_death_snd;

  bool is_ground_rebounding;
  bool is_in_ground_edge;

  //Attacked
  bool knockback_left;
  bool player_knockback;

  //force search
  bool force_search;

  //Bullet
  int bullet_cooldown_max;
  int bullet_cooldown;
  int bullet_fire_at_once_max;
  int bullet_fire_cooldown_max;
  int bullet_fire_cooldown;
  int bullet_length_max;
  int bullet_length;
  int bullet_damage;
  int bullet_speed_multiplier;
  int bullet_range;
  int bullet_color;
  int bullet_shot_num;
  int bullet_graphics_type;

  //Pathfinding attributes
  int current_node;
  int start_node;
  int end_node;

  int species1_solid_num;
     //array of path nodes after target is found
  int path_nodes_num;
     //open nodes
  int open_nodes_num;
     //timers
  int sprite_timer;
  int search_timer;
  int idle_timer;
  int forgor_timer;

  int sprite_width;
  int sprite_height;
  int sprite_minx;
  int sprite_miny;
  int sprite_maxx;
  int sprite_maxy;
  int current_draw_row;

  int type;
  int in_air_timer;
  int death_timer;
  int species;
  int follow_range;
  int chase_range;
  int unchase_range;
  int shoot_at_player_range;
  int aim_rand;
  int color;
  int ospeed_multiplier;
  int speed_multiplier;
  int time_breaker_rare;
  int time_breaker_length;
  int saved_ground_id;
  int on_ground_id;
  int knockback_timer;
  int node_num;
  int seed;
  int damage_taken_timer;

  double health;
  double max_health;
  double LOS_x;
  double LOS_y;
  double LOS_angle;
  double LOS_target_x;
  double LOS_target_y;
  double bullet_speed;
  double shoot_target_x;
  double shoot_target_y;
  double x;
  double y;
  double angle;
  double saved_angle;
  double sprite_angle;
  double dist_from_player;
  double sprite_x;
  double sprite_y;
  double speed;
  double knockback_angle;
  double cosine;
  double sine;

  //double x_height_from_ground;
  //double x_height_from_prev_ground;

  int node_neighbour[8];

  //6715632

  int bullet_head_y[MAX_BULLET_PER_FIRE];
  int bullet_head_x[MAX_BULLET_PER_FIRE];


  bool node_solid[MAX_NODE_NUM];
  bool node_open[MAX_NODE_NUM];
  bool node_closed[MAX_NODE_NUM];
  bool node_back[MAX_NODE_NUM];


  int bullet_shot_arr[ENEMY_BULLET_NUM];

  int path_nodes[MAX_NODE_NUM/2];
  int open_nodes[MAX_NODE_NUM/2];
  int enemy_species1_solids[MAX_NODE_NUM];
  int node_x[MAX_NODE_NUM];
  int node_y[MAX_NODE_NUM];
  int node_parent[MAX_NODE_NUM];
  int node_gcost[MAX_NODE_NUM];
  int node_hcost[MAX_NODE_NUM];
  int node_fcost[MAX_NODE_NUM];
} AEnemy; 



AEnemy *createEnemy()
{
  AEnemy *toReturn = malloc(sizeof(AEnemy));
  return toReturn;
}


void freeEnemy(AEnemy *myEnemy)
{
  if (myEnemy)
    free(myEnemy);
}


AEnemy **Enemy;



typedef struct enemy_sprites
{
  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_3;
  HBITMAP sprite_4;

  DRAWSPRITE draw_sprite_1;
  DRAWSPRITE draw_sprite_2;
  DRAWSPRITE draw_sprite_3;
  DRAWSPRITE draw_sprite_4;
}AEnemySprite;

AEnemySprite** EnemySprite;

AEnemySprite *createEnemySprite()
{
  AEnemySprite *toReturn = malloc(sizeof(AEnemySprite));
  return toReturn;
}


void freeEnemySprite(AEnemySprite *myEnemySprite)
{
  if (myEnemySprite)
    free(myEnemySprite);
}





struct color_chooser
{
  bool is_choosing_color;
  int color_id;
  int color_id_choosing;
} color_chooser;




int CalculateDistanceCost(int enemy_id,int a, int b);
int smallest_f_cost(int enemy_id);
void InitEnemyPathfinding(int enemy_id,double target_x,double target_y);
void EnemyPathFinding(int enemy_id);
void EnemySpecies1Gravity(int enemy_id);
void EnemyMove(int enemy_id);
void EnemyTargetPlayer(int i);
void EnemyAct(int i);
void SetEnemyByType(int i,int type);
void InitEnemy();
void DrawEnemy(HDC hdc);




//mouse 
int mouse_x;
int mouse_y;
HBITMAP player_cursor[16]; //4 left and 4 right, open or closed, loaded
HBITMAP player_cursor_body[16];
HBITMAP player_cursor_iris[16];
HBITMAP player_cursor_pupil[2];// 1 left  open or closed

DRAWSPRITE draw_player_cursor_body[16];
DRAWSPRITE draw_player_cursor_iris[16];
DRAWSPRITE draw_player_cursor_pupil[2];



//enemy global
HBITMAP enemy1_sprite_1;
HBITMAP enemy1_sprite_2;
HBITMAP enemy2_sprite_1;
HBITMAP enemy2_sprite_2;
HBITMAP enemy2_sprite_3;
HBITMAP enemy2_sprite_4;


//map background
HBITMAP map_background_sprite;


//map platforms
HBITMAP map_platforms_sprite;
HBITMAP map_platforms_sprite_mask;
HBITMAP map_platforms_shadow_shader;
//HBITMAP map_platforms_shadow_shader_mask;

//moon
HBITMAP moon_sprite;
HBITMAP moon_sprite_cache;
DRAWSPRITE draw_moon_sprite;

//main menu title
HBITMAP title_sprite;
HBITMAP title_sprite_mask;


HBITMAP title_small_sprite;
HBITMAP title_small_sprite_mask;

//Khmer main menu
HBITMAP mm0_kh[4];
HBITMAP mm0_kh_mask[4];
HBITMAP mm0_kh_green[4];
HBITMAP mm0_kh_white[4];

HBITMAP mm2_kh[7];
HBITMAP mm2_kh_mask[7];
HBITMAP mm2_kh_green[7];
HBITMAP mm2_kh_white[7];

HBITMAP mm0_kh_hijri[2];
HBITMAP mm0_kh_hijri_mask[2];


//Khmer ingame
HBITMAP ga0_kh[5];
HBITMAP ga0_kh_mask[5];

//Sound
//Sound called in the Stack
#define SPAM_SFX_NUM    9
#define KEY_SFX_NUM     7
#define CHANNEL_SFX_NUM 13

wavSoundEffect spamSoundEffect[SPAM_SFX_NUM];
wavSoundEffect keySoundEffect[KEY_SFX_NUM];
wavSoundEffect channelSoundEffect[CHANNEL_SFX_NUM];
wavSoundEffect songAudio;

wavSoundEffectCache spamSoundEffectCache[SPAM_SFX_NUM];
wavSoundEffectCache keySoundEffectCache[KEY_SFX_NUM];
wavSoundEffectCache channelSoundEffectCache[CHANNEL_SFX_NUM];

