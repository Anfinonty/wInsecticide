//classes for game
//note: double first, bool last


struct GroundLine
{
  bool is_ghost; //Can be colided or not collided
  bool within_render_distance; //Is the ground within the player's Render Distance Grid'


  int health; //For Webs, health of ground
  int color; //Color of ground, RGB()
  int type; //Regular, Text ground or TriFill 
      //saved_pos_in_grid[VGRID_NUM];
      //*saved_pos_in_grid;

       //already_in_grid[VGRID_NUM]; //Is the Ground occupying the Grid it is in
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
  double height_from_player_x;//= gradient * player_x + c
  double angle; // = ACos of (x/length)


  char text[256]; //for type 1 which is Text_ground

//} Ground[GROUND_NUM+MAX_WEB_NUM];
  bool already_in_grid[MAX_VGRID_NUM]; //MAX VGRID NUM
  int saved_pos_in_grid[MAX_VGRID_NUM]; //MAX VGRID NUM

};
struct GroundLine* Ground;


void InitGround();
void InitGround2();
double GetLineTargetAngle(int Ground_id,double x,double y);
double GetLineTargetHeight(int Ground_id,double E,double x,double y);
void SetGround(int i);
int GetOnGroundId(double x,double y,double min_range_1,double min_range_2,bool is_player);
void DestroyGround(int i);

void DrawWebs(HDC hdc);
void DrawGround(HDC hdc);
void DrawGroundText(HDC hdc);
void DrawGroundTriFill(HDC hdc);







struct player 
{
  bool fling_left;
  bool is_rebounding;
  bool rebound_above;
  bool rebound_below;
  bool is_swinging;
  bool hiding;
  bool left_click;
  bool right_click;
  bool rst_left_click;
  bool rst_right_click;
  bool rst_down;
  bool rst_left;
  bool rst_right;
  bool rst_up;
  bool rst_key_sprint;
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

    //Render
  int rendered_grid_num;
  int rendered_vgrid_num;
   //int render_grounds[GROUND_NUM+MAX_WEB_NUM];

  int rendered_enemy_num;
  //int render_enemies[ENEMY_NUM];
  int rendered_ground_num;
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


  int grav;
  int jump_height;
  int sprite_timer;
  int in_air_timer;
  int speed;
  int on_ground_timer;
  int on_ground_id;
  int saved_ground_id;
  int walk_cycle;
  int player_jump_height;
  int key_jump_timer;
  int knockback_speed_multiplier;
  int knockback_strength;


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
  double above_x;
  double above_y;
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
  double sprite_x;
  double sprite_y;
  double claws_x;
  double claws_y;

  double block_health_max;
  double block_health;


  double health;
  double pivot_x;
  double pivot_y;
  double pivot_angle;
  double pivot_length;
  double potential_energy;
  double counter_potential_energy;
  double launch_angle;
  double fling_distance;
  double angle_of_incidence;
  double angle_of_reflection;


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


  int web_storage[MAX_WEB_NUM];
  int cdwebs[MAX_WEB_NUM];
  int render_grids[RDGRID_NUM];
  int render_vgrids[VRDGRID_NUM];
} player;


int *player_render_enemies;
int *player_render_grounds;

//int player_render_enemies[21];
//int player_render_grounds[342];


void InitPlayer();
bool IsInvertedBackground();
bool IsSpeedBreaking();
bool IsCollideCrawler(double x1,double y1,double x2,double y2,double gradient,double c);
void Click();
void move_x(double x);
void move_y(double y);
void InitRDGrid();
void InitVRDGrid();
void CameraInit(double x,double y);
void InitPlayerCamera();
bool NearCrawler();
void CleanUpPlayer();
bool YesInitRDGrid();
bool YesInitVRDGrid();
void RegainWeb(int web_id);
void PlayerAct();
void PlayerPlaceWeb();
void PlayerCameraShake();
void DrawPlayer(HDC hdc);











struct RenderDistanceGrid
{
  int x;
  int y;
} RDGrid[RDGRID_NUM];


struct VRenderDistanceGrid
{
  int x;
  int y;
} VRDGrid[VRDGRID_NUM];


struct grid 
{
  bool within_render_distance;
    //enemy_occupy[ENEMY_NUM], //id of enemies occupying this grid
    //*enemy_occupy, //id of enemies occupying this grid
  int x1;
  int y1;
  int x2;
  int y2; //grid axes
  int enemy_occupy_num; //how many enemies are occupying this grid
  int enemy_occupy[MAX_ENEMY_NUM]; //MAX ENEMY NUM
};
//} Grid[GRID_NUM];
struct grid* Grid;




struct vgrid
{
  bool within_render_distance;
  int x1;
  int y1;
  int x2;
  int y2; //grid axes
  int max_ground_num; //how many grounds are occupying this grid
  int ground_ids[MAX_GROUNDS_WITHIN_GRID]; //id of grounds that are occupying this grid
};
//} VGrid[VGRID_NUM];
struct vgrid* VGrid;


struct node
{
  bool node_solid;
  bool non_web;
  int x1;
  int y1;
  int x2;
  int y2;
};
//} NodeGrid[MAP_NODE_NUM];
struct node* NodeGrid;


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
  bool left,near_miss;
  int color;
  int speed_multiplier;
  int damage;
  int from_enemy_id;
  int graphics_type;
  int saved_pos;
  double sprite_x;
  double sprite_y;
  double start_x;
  double start_y;
  double x;
  double y;
  double angle;
  double speed;
  double range;

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
  int damage,
  int enemy_id,
  double start_x,
  double start_y,
  double source_x,
  double source_y,
  double target_x,
  double target_y);

bool HitPlayer(int bullet_id);
void StopBullet(int bullet_id,bool is_player);
void BulletAct(int bullet_id);
void DrawBullet2(HDC hdc,int i,double x,double y,int color);
void DrawBullet(HDC hdc,int i);













struct enemy
{
  //Line Of Sight bullet


  //Enemy Attributes
  //Enemy Node Attributes
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
  bool in_chase_range; //:player is within enemy's chase range
  bool in_unchase_range; //:player is within enemy's unchase range
  bool time_breaker_immune; //:Enemy is ummune to the player's time breaker effect
  bool LOS_left;
  bool LOS_shot;
  bool flip_sprite;

  //Attacked
  bool knockback_left;

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
     //array of path nodes after target is found
  int path_nodes_num;
     //open nodes
  int open_nodes_num;
     //timers
  int sprite_timer;
  int search_timer;
  int idle_timer;
  int forgor_timer;

  int seed;
  int sprite_width;
  int sprite_height;
  int sprite_minx;
  int sprite_miny;
  int sprite_maxx;
  int sprite_maxy;
  int current_draw_row;


  int type;
  int in_air_timer;
  int species;
  int follow_range;
  int chase_range;
  int unchase_range;
  int shoot_at_player_range;
  int aim_rand;
  int color;
  int speed_multiplier;
  int health;
  int max_health;
  int time_breaker_rare;
  int time_breaker_length;
  int on_ground_id;
  int knockback_timer;
  //Grid interaction
  int //grid_queue[GRID_NUM],
      saved_grid_id;
  int node_num;


  int node_neighbour[8];

  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_3;

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
  int bullet_head_y[MAX_BULLET_PER_FIRE];
  int bullet_head_x[MAX_BULLET_PER_FIRE];
  int grid_queue[MAX_GRID_NUM];
  bool node_solid[MAX_NODE_NUM];
  bool node_open[MAX_NODE_NUM];
  bool node_closed[MAX_NODE_NUM];
  bool node_back[MAX_NODE_NUM];
  int path_nodes[MAX_NODE_NUM/2];
  int open_nodes[MAX_NODE_NUM/2];
  int node_x[MAX_NODE_NUM];
  int node_y[MAX_NODE_NUM];
  int node_parent[MAX_NODE_NUM];
  double node_gcost[MAX_NODE_NUM];
  double node_hcost[MAX_NODE_NUM];
  double node_fcost[MAX_NODE_NUM];
  int bullet_shot_arr[BULLET_NUM];
}; 
//} Enemy[ENEMY_NUM];
struct enemy* Enemy;


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



int mouse_x;
int mouse_y;
HBITMAP mouse_cursor_sprite;
HBITMAP mouse_cursor_sprite_cache;



HBITMAP enemy1_sprite_1;
HBITMAP enemy1_sprite_2;
HBITMAP enemy2_sprite_1;
HBITMAP enemy2_sprite_2;
HBITMAP enemy2_sprite_3;


HBITMAP map_background_sprite;

HBITMAP map_platforms_sprite;
HBITMAP map_platforms_sprite_mask;





//functions


