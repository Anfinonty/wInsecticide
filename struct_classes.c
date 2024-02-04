//classes for game

struct Ground 
{
  bool is_ghost, //Can be colided or not collided
       within_render_distance, //Is the ground within the player's Render Distance Grid'
       already_in_grid[VGRID_NUM]; //Is the Ground occupying the Grid it is in

  int health, //For Webs, health of ground
      color, //Color of ground, RGB()
      type, //Regular, Text ground or TriFill 
      saved_pos_in_grid[VGRID_NUM];


  double x1,y1,x2,y2,x3,y3, //3 Axes of ground
         sprite_x1,sprite_y1,sprite_x2,sprite_y2,sprite_x3,sprite_y3,//graphics 3 Axes of ground
         length, // = sqrt(x^2 + y^2)) // Babylonian Trigonometry Theorem for Calculating the Hypothenuse
         gradient,// y of ground / x of ground
         c, //The C in y-mx_c
         height_from_player_x,//= gradient * player_x + c
         angle; // = ACos of (x/length)

  char *text; //for type 1 which is Text_ground

} Ground[GROUND_NUM+MAX_WEB_NUM];


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
  bool rst_speed_break;
  bool attack;
  bool blocking;
  bool print_valid_web;
  bool valid_web;
  bool attack_rst;
  bool destroy_ground;
  bool uppercut;

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
  double knockback_speed;
  int knockback_speed_multiplier;
  double attack_strength;
  int knockback_strength;

    //Render
  int rendered_grid_num;
  int render_grids[RDGRID_NUM];

  int rendered_vgrid_num;
  int render_vgrids[VRDGRID_NUM];
 
  int rendered_enemy_num;
  int render_enemies[ENEMY_NUM];

  int rendered_ground_num;
  int render_grounds[GROUND_NUM+MAX_WEB_NUM];

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

  int block_recharge_timer_max;
  int block_recharge_timer;
  int block_cooldown_max;
  int block_cooldown;
  double block_health_max;
  double block_health;

  int left_click_hold_timer;
  int right_click_hold_timer;
  int attack_timer;
  int sleep_timer;
  int hit_cooldown_timer;
  int block_timer;

  double health;


  int max_web_num;

  int bullet_shot;

  int placed_web_pos;
  int placed_web_num;

  int web_being_shot;
  int web_storage[MAX_WEB_NUM];

  int cdweb_pos;
  int cdweb_player_pos;
  int cdweb_num;
  int cdwebs[MAX_WEB_NUM];

  int destroyed_web_pos;


  double pivot_x;
  double pivot_y;
  double pivot_angle;
  double pivot_length;
  bool is_swinging;
  double potential_energy;
  double counter_potential_energy;
  double launch_angle;
  double fling_distance;
  bool fling_left;
  int previous_web_placed;




  bool is_rebounding;
  double angle_of_incidence;
  double angle_of_reflection;
  bool rebound_above;
  bool rebound_below;

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
} player;


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
  int x,y;
} RDGrid[RDGRID_NUM];



struct VRenderDistanceGrid
{
  int x,y;
} VRDGrid[VRDGRID_NUM];


struct Grid 
{
  bool within_render_distance;
  int 
    enemy_occupy_num, //how many enemies are occupying this grid
    enemy_occupy[ENEMY_NUM], //id of enemies occupying this grid
    x1,y1,x2,y2; //grid axes
} Grid[GRID_NUM];


struct VGrid
{
  bool within_render_distance;
  int
    max_ground_num, //how many grounds are occupying this grid
    ground_ids[MAX_GROUNDS_WITHIN_GRID], //id of grounds that are occupying this grid
    x1,y1,x2,y2; //grid axes
} VGrid[VGRID_NUM];


struct Node
{
  bool node_solid,non_web;
  int x1,y1,x2,y2;
} NodeGrid[MAP_NODE_NUM];



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
  bool shot,left,near_miss;
  int color,
      speed_multiplier,
      damage,
      from_enemy_id,
      graphics_type,
      saved_pos;
  double sprite_x,sprite_y,start_x,start_y,
      x,y,angle,speed,range;

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













struct Enemy
{
  //Line Of Sight bullet
  bool LOS_left,LOS_shot;
  double LOS_x,LOS_y,LOS_angle,LOS_target_x,LOS_target_y;
  //Bullet
  int bullet_cooldown_max,
      bullet_cooldown,
      bullet_fire_at_once_max,
      bullet_fire_cooldown_max,
      bullet_fire_cooldown,
      bullet_length_max,
      bullet_length,
      bullet_damage,
      bullet_speed_multiplier,
      bullet_range,
      bullet_color,
      bullet_shot_num,
      bullet_graphics_type,
      bullet_head_x[MAX_BULLET_PER_FIRE],
      bullet_head_y[MAX_BULLET_PER_FIRE],
      bullet_shot_arr[BULLET_NUM*1];
  double bullet_speed;


  //Grid interaction
  int grid_queue[GRID_NUM],
      saved_grid_id;


  //Enemy Node Attributes
  bool node_solid[MAX_NODE_NUM],
       node_open[MAX_NODE_NUM],
       node_closed[MAX_NODE_NUM],
       node_back[MAX_NODE_NUM];
  int node_num,
      node_x[MAX_NODE_NUM],
      node_y[MAX_NODE_NUM],
      node_parent[MAX_NODE_NUM];
  double node_gcost[MAX_NODE_NUM],
      node_hcost[MAX_NODE_NUM],
      node_fcost[MAX_NODE_NUM];


  //Attacked
  bool knockback_left;
  double knockback_angle;
  int knockback_timer;


  //Enemy Attributes
  bool saw_player, //:Enemy seen the player, player was in line of sight seconds ago.
       last_left, //:Enemy was going to the left
       target_player, //:Enemy is actively chasing the player
       ignore_player, //:Enemy is not chasing the player due to constraints or in cooldown
       idling, //:Enemy is standing still
       found_target, //:Enemy has found the target node to travel to
       search_target, //:Enemy is searching for a target node to travel to
       trace_target, //:Enemy is currently drawing a path to the target node to travel to
       move_to_target, //:Emeny is currently moving towards the target node to travel to
       player_at_left, //:player is on the left side of the enemy
       player_at_right, //:player is on the right side of the enemy
       player_at_above, //:player is above the enemy
       player_at_below, //:player is below the enemy
       within_render_distance, //:Enemy is within the render distance
       above_ground, //:For crawlers: crawler is above the ground
       below_ground, //:For crawlers: crawler is upside down on a ground
       in_chase_range, //:player is within enemy's chase range
       in_unchase_range, //:player is within enemy's unchase range
       time_breaker_immune; //:Enemy is ummune to the player's time breaker effect


  int type,
      in_air_timer,
      species,
      follow_range,
      chase_range,
      unchase_range,
      shoot_at_player_range,
      aim_rand,
      color,
      speed_multiplier,
      health,
      max_health,
      time_breaker_rare,
      time_breaker_length,
      on_ground_id;

  double shoot_target_x,shoot_target_y,
      x,y,angle,saved_angle,sprite_angle,dist_from_player,
      sprite_x,
      sprite_y,
      speed;
  bool flip_sprite;

  //Pathfinding attributes
  int current_node,
      node_neighbour[8],
      start_node,
      end_node,
     //array of path nodes after target is found
      path_nodes_num,
      path_nodes[MAX_NODE_NUM/2],
     //open nodes
      open_nodes_num,
      open_nodes[MAX_NODE_NUM/2],
     //timers
      sprite_timer,
      search_timer,
      idle_timer,
      forgor_timer;

  int seed;

  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_3;
} Enemy[ENEMY_NUM];

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

