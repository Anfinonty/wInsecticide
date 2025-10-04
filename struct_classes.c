//classes for game
//==================GROUND LINE==================
typedef struct GroundLine
{
  bool is_ghost; //Can be colided or not collided
  bool within_render_distance; //Is the ground within the player's Render Distance Grid'


  int health; //For Webs, health of ground
  int color_id;
  int color; //Color of ground, RGB()
  int type; //Regular, Text ground or TriFill 
  int font_size;

  //int fire_ground_id;

  float x1;
  float y1;
  float x2;
  float y2;
  float x3;
  float y3; //3 Axes of ground
  float length; // = sqrt(x^2 + y^2)) // Babylonian Trigonometry Theorem for Calculating the Hypothenuse
  float gradient;// y of ground / x of ground
  float c; //The C in y-mx_c
  //float height_from_player_x;//= gradient * player_x + c
  float angle; // = ACos of (x/length)
  wchar_t text[512]; //for type 1 which is Text_ground
  bool already_in_grid[MAX_VGRID_NUM];
  int saved_pos_in_grid[MAX_VGRID_NUM];
} AGround;

//https://stackoverflow.com/questions/48222554/c-creating-a-struct-array-beyond-certain-size-causes-a-crash

AGround *createGround()
{
  AGround *toReturn = malloc(sizeof(AGround));
  return toReturn;
}


void freeGround(AGround *myGround)
{
  if (myGround)
    free(myGround);
}

AGround **Ground;



#define GROUND_FIRE_SMOKE_NUM   5//20
typedef struct GroundLineFire
{
  int ground_id; //child of Ground, manage so it doesnt crash
  int rng_i;
  int old_x3;
  int old_y3;
  int current_tx;
  int current_ty;
  int rand_tip_x;
  int rand_tip_y;
  int tick;
  int smoke_rng_i;
  int smoke_x[GROUND_FIRE_SMOKE_NUM];
  int smoke_y[GROUND_FIRE_SMOKE_NUM];
  int smoke_height[GROUND_FIRE_SMOKE_NUM];
} AGroundFire;


AGroundFire *createGroundFire()
{
  AGroundFire *toReturn = malloc(sizeof(AGroundFire));
  return toReturn;
}


void freeGroundFire(AGroundFire *myGroundFire)
{
  if (myGroundFire)
    free(myGroundFire);
}


void InitGroundFire();
void FireGroundAct();

AGroundFire** GroundFire;



float GetLineTargetAngle(int Ground_id,float x,float y);
float GetLineTargetHeight(int Ground_id,float E,float x,float y);
float GetLineTargetHeightII(int Ground_id,float E,float x,float y);
void SetGround(int i);
void InitGround(bool is_max);
void InitGround2();
int GetOnGroundId(float x,float y,float min_range_1,float min_range_2);
int GetOnGroundIdPlayer(float x,float y,float min_range_1,float min_range_2);
int GetOnGroundIdE(float x,float y,float min_range_1,float min_range_2,int enemy_id);
void DestroyGround(int i);


void DrawWebs(HDC hdc);
void DrawGround(HDC hdc);
void DrawGroundText(HDC hdc);
void DrawGroundTriFill(HDC hdc);
void DrawWaterTriFill(HDC hdc);
void Draw1Ground(HDC hdc,int i,int x, int y);
void Draw1GroundText(HDC hdc,int i,int x, int y);
void Draw1GroundTextureTriFill(HDC hdc,HDC hdc2,int i,int x, int y);
void Draw1GroundTriFill(HDC hdc,int i,int x, int y);
void Draw1WaterTriFill(HDC hdc, int i,int x,int y);

//gameplay only
int *rendered_water_ground;
//int *rendered_fire_ground;





//For map editor
struct RenderDistanceGrid
{
  int x;
  int y;
} RDGrid[RDGRID_NUM]; //9*9


//map editor only
int rendered_ground_num;
int *render_grounds;



//==================PLAYER==================
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
  bool rst_key_1;
  bool rst_right_click_snd;

  bool flag_death;

  bool last_left;
  bool jump;
  bool current_above;
  bool current_below;
  bool previous_above;
  bool previous_below;
  bool print_current_above;
  bool print_current_below;
  bool above_ground_edge;
  bool below_ground_edge;
  bool time_breaker;
  bool attack;
  bool blocking;
  bool print_valid_web;
  bool valid_web;
  bool attack_rst;
  bool destroy_ground;
  bool uppercut;
  bool flag_revert_palette;
  bool flag_noir_palette;
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

  bool web_burned;

  int type;

  int bullet_num;
  int play_gun_snd;
  int knives_per_throw;
  int invalid_shoot_timer;
  int dmg_taken_timer;

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
  int death_timer;
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
  int time_breaker_deplete_cooldown;
  int time_breaker_units_max;
  int time_breaker_cooldown;
  int time_breaker_cooldown_max;
  int time_breaker_recharge_timer;
  int time_breaker_recharge_timer_max;
  int time_breaker_units_tick;
  int time_breaker_units_tick_max;
  int spin_timer;
  int in_water_timer;

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
  int on_ground_id_u2;
  int saved_ground_id;
  int walk_cycle;
  int player_jump_height;
  int knockback_speed_multiplier;
  int knockback_strength;
  int time_breaker_tick;
  int decceleration_timer;
  //int fast_duration;
  int shoot_knife_duration;
  int hurt_snd_timer;

  int on_ground_edge_id;
  int saved_on_ground_edge_id;
  int pivot_on_ground_id;
  int show_exp_timer;
  int show_block_health_timer;
  int show_health_timer;

  int current_blur_sprite;

  //int rain_wet_timer;
  //int visible_rain_wet_timer;
  int exp;

  int sprite_angle;


  int rng_i;
  int death_bullet_rng_i;
  int bullet_rng_i;
  int sniper_bullet_rng_i;

  int seed;

  int blur_timer[PLAYER_BLUR_NUM];
  int blur_x[PLAYER_BLUR_NUM];
  int blur_y[PLAYER_BLUR_NUM];
  int blur_sprite_x[PLAYER_BLUR_NUM];
  int blur_sprite_y[PLAYER_BLUR_NUM];

  float knockback_speed;
  float attack_strength;
  float saved_x;
  float saved_y;
  float x;
  float y;
  float below_x1;
  float below_y1;
  float below_x2;
  float below_y2;
  float above_x;
  float above_y;
  float above_head_x;
  float above_head_y;
  float above_x1;
  float above_y1;
  float above_x2;
  float above_y2;
//  float sprite_angle;
  float angle;
  float edge_angle;
  float saved_angle;
//  float saved_sprite_angle;
//  float saved_block_sprite_angle;
//  float saved_attack_sprite_angle;
  float player_grav;
  float cam_x;
  float cam_y;
  float cam_move_x;
  float cam_move_y;
  float cam_mouse_move_x;
  float cam_mouse_move_y;
  int cam_limiter_x;
  int cam_limiter_y;
  float sprite_x;
  float sprite_y;
  float claws_x;
  float claws_y;
  float claws_attack_x;
  float claws_attack_y;

  float block_health_max;
  float block_health;


  float health;
  float pivot_x;
  float pivot_y;
  float pivot_angle;
  float pivot_length;
  float jump_angle;
  float jump_angle2;
  float fling_distance;
  float angle_of_incidence;
  float angle_of_reflection;

  float mouse_angle;
  float mouse_dist;


  int web_storage[MAX_WEB_NUM];
  int cdwebs[MAX_WEB_NUM];
  int bullet[PLAYER_BULLET_NUM];
} player;


struct PlayerFlingWeb {
  //player shot web attributes
  float x[PLAYER_FLING_WEB_NUM];
  float y[PLAYER_FLING_WEB_NUM];
  float sprite_x[PLAYER_FLING_WEB_NUM];
  float sprite_y[PLAYER_FLING_WEB_NUM];
} player_fling_web;




bool IsSpeedBreaking();
void Click();
void move_x(float x);
void move_y(float y);
void CameraInit(float x,float y);
void InitPlayerCamera(float target_x,float target_y);
void PlayerPlaceWeb();
void PlayerActPlaceWeb(int bm_x1,int bm_y1,int bm_x2,int bm_y2);
void InitPlayerSpritesAll();
void InitPlayerFlingWeb();
void InitPlayer();
void RegainWeb(int web_id);
void PlayerRevive();
void PlayerActGroundEdgeMovement();
void PlayerActXMovement(int grav_speed);
void PlayerOnGroundAction(int speed, int grav, int height_from_player_x);
void PlayerActFlingWeb(int speed);
void PlayerActSwinging(int grav_speed);
void PlayerActGravityMovement(int grav_speed,int speed);
void PlayerActFlingMovement(int grav_speed);
void PlayerActLoadWeaponSnd();
void PlayerActReboundActions(int grav_speed,int speed);
void PlayerActMouseMovement();
void PlayerActMouseClick();
void PlayerActDestroyGround();
void PlayerActDecceleration();
void PlayerAct();
void PlayerSndAct();
void PlayerCameraShake();

void InitPlayerSpritesObjColor(HDC hdc,HDC hdc2);
void InitPlayerCursorColor(HDC hdc,HDC hdc2);
void DrawPlayer(HDC hdc,HDC hdc2,int ptype);







//==================VISUAL GRID==================
typedef struct vgrid
{
  bool within_render_distance;
  bool has_shadow;
  bool has_water;
  bool not_just_water;
  bool checked;
  int draw_shadow_seg_id;
  int draw_platform_seg_id;
  int draw_foreground_seg_id;
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
  bool node_fire;
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
void InitColorPlatformTextures(HDC hdc, HDC hdc2,int i);
void DrawCreateTiles(HDC hdc,HDC hdc2);
void InitGrid();
void InitRDGrid();
void InitNodeGrid();
void InitNodeShade();
bool IsHasNeighbours(int nx,int ny);
void SetNodeGridAttributes2(int i);
void SetNodeGridAttributes(int i);
void TriFillNodeGridType(int gid);
void InitNodeGridAttributes();
void DrawNodeGrids(HDC hdc);
void DrawShadows2(HDC hdcSrc,HDC hdcDest,int x,int y,bool t);
void CreatePlatformShadowBitmap(HDC hdc,float rise,float run,int color);
HBITMAP GenerateColoredTexture(int loaded_ptexture_id, int solid_value,int color_id);
//void DrawGrid(HDC hdc);














int current_bullet_id=0;
struct Bullet
{
  bool shot;
  bool near_miss;
  bool in_water; 
  bool playsnd;
  bool is_left;

  int color;
  int speed_multiplier;
  int ospeed_multiplier;
  int from_enemy_id;
  int graphics_type;
  int saved_pos;
  int saved_ground_id;
  int saved_node_grid_id;
  int bounce_timer;

  int rng_i;

  float oangle;
  float oscilating_angle;
  float oscilating_angle_max;
  float oscilating_angle_delta;

  float damage;
  float sprite_x;
  float sprite_y;
  float start_x;
  float start_y;
  float x;
  float y;
  float angle;
  float speed;
  float ospeed;
  float range;
  float start_range;

} Bullet[BULLET_NUM];

void InitBullet();
void ShootBullet(
  int bullet_id,
  int saved_pos,
  int color,
  int graphics_type,
  int range,
  float speed,
  int speed_multiplier,
  float damage,
  int enemy_id,
  float start_x,
  float start_y,
  float source_x,
  float source_y,
  float target_x,
  float target_y,
  float off_angle);
void BulletDamagePlayerAct(int bullet_id);

bool HitPlayer(int bullet_id,int r1,int r2);
void StopBullet(int bullet_id,bool is_player);
//bool RainBulletTransitNodeGrid(int bullet_id);
void RainBulletTransitNodeGrid(int bullet_id);
void EnemyBulletAct(int bullet_id,int enemy_id);
void MapEditorBulletAct(int bullet_id);
void PlayerBulletAct(int bullet_id,int enemy_id);
void ResetBulletRain();
void InitBulletRain();
void RainBulletAct(int bullet_id);

void BulletAct(int bullet_id);
void BulletSndAct(int i);
void RainAct();

void DrawBullet2(HDC hdc,HDC hdc2,int i,float x,float y,int color);
void DrawBullet(HDC hdc,HDC hdc2,int i);
void DrawRain(HDC hdc,HDC hdc2);











//==================ENEMY==================
typedef struct enemy
{
  //Enemy Attributes
  //Enemy Node Attributes

  //bool being_drawn;
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
  bool above_ground_edge; //:For crawlers: crawler is above the ground
  bool below_ground_edge; //:For crawlers: crawler is upside down on a ground
  //bool previous_above_ground; //:For crawlers: crawler is above the ground
  //bool previous_below_ground; //:For crawlers: crawler is upside down on a ground
  bool in_chase_range; //:player is within enemy's chase range
  bool in_unchase_range; //:player is within enemy's unchase range
  bool time_breaker_immune; //:Enemy is immune to the player's time breaker effect
  bool LOS_left; //:line of sight bullet, is it being shot in the left
  bool LOS_shot; //:is line of sight bullet being shot
  bool flip_sprite; //:is sprite required to be flipped?, condition: large sprite + its upside down
  bool play_death_snd; //:play death sound when killed
  bool is_ground_rebounding; //:is it currently rebounding from a ground it is on
  bool is_in_ground_edge; //:is it currently on the edge of a ground
  bool flag_web_unstuck; //:is it not stuck in the web _screen flag
  bool flag_web_stuck; //: is it currently stuck in the web _screen flag
  bool web_stuck; //: is it currently stuck in the web
  bool is_clockwize; //: for ants, is it the cloockwize type ant
  bool true_dead; //: for cockroach__ is it truly dead in hard mode
  bool draw_falling; //: for cockroach adn toe_biter__ is it falling and forced to be drawn its flying sprite

  //Attacked
  bool knockback_left;
  bool player_knockback;
  bool saved_above_ground;
  bool saved_below_ground;
  bool is_in_left_ground_edge;
  bool is_in_right_ground_edge;
  bool force_fall;
  bool try_revive_player;

  //movement
  bool flag_last_left;
  bool flag_last_right;

  bool flag_flip_sprite;
  bool flag_unflip_sprite;

  //bool flag_above_ground;
  //bool flag_below_ground;

  //force search
  bool force_search;

  bool in_water;
  bool sprite_in_water;
  int in_node_grid_id;
  int current_ngid_n;
  int flying_timer;

  int rotated_sprite_id;
  int rotated_xsprite_id;
  int current_rot_sprite_angle_id;
  //int current_rot_sprite_edge_angle_id;
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

  int mleft_streak;
  int mright_streak;


 //timers
  int last_seen_timer;
  int sprite_flip_timer;
  int sprite_timer;
  int search_timer;
  int idle_timer;
  int forgor_timer;
  int render_distance_timer;
  //int in_ground_edge_timer;

  //suffocate in solid trifills
  int suffocate_timer;
  int current_suffocate_ngid_n;

  int type; //0 to 10
  int sprite_type; //0 to 10
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
  int on_ground_edge_id;
  int knockback_timer;
  int damage_taken_timer;
  int on_ground_timer;
  int hurt_snd_timer;
  int in_water_timer;
  int pathfinding_id;

  int rng_i; //rng index doesnt interfere others
  int flying_rng_i;
  int target_rng_i;
  int target_player_rng_i;
  int bullet_rng_i;
  int dazzle_rng_i;
  int timebreaker_rng_i;
  int bullet_aim_rand_rng_i;
  int bullet_stuck_rng_i;
  int idle_rng_i;

  int seed;

  int sprite_x;
  int sprite_y;

  int antannae_timer;

  float health;
  float max_health;
  float LOS_x;
  float LOS_y;
  float LOS_angle;
  float LOS_target_x;
  float LOS_target_y;
  float bullet_speed;
  float shoot_target_x;
  float shoot_target_y;
  float x;
  float y;
  float angle;
  float sprite_angle;
  float dist_from_player;
  float speed;
  float ospeed;
  float knockback_angle;


  //6715632

  int bullet_head_y[MAX_BULLET_PER_FIRE];
  int bullet_head_x[MAX_BULLET_PER_FIRE];




  int bullet_shot_arr[ENEMY_BULLET_NUM];

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

typedef struct EnemyPathfinding
{
  int enemy_id;
  //Pathfinding attributes
  int current_node;
  int start_node;
  int end_node;

  int species1_solid_num;
     //array of path nodes after target is found
  int path_nodes_num;
     //open nodes
  int open_nodes_num;
  int node_num;


  int node_neighbour[8];
  bool node_solid[MAX_NODE_NUM];
  bool node_open[MAX_NODE_NUM];
  bool node_closed[MAX_NODE_NUM];
  bool node_back[MAX_NODE_NUM];
  int path_nodes[MAX_NODE_NUM];
  int open_nodes[MAX_NODE_NUM];
  int enemy_species1_solids[MAX_NODE_NUM];
  int node_x[MAX_NODE_NUM];
  int node_y[MAX_NODE_NUM];
  int node_parent[MAX_NODE_NUM];
  int node_gcost[MAX_NODE_NUM];
  int node_hcost[MAX_NODE_NUM];
  int node_fcost[MAX_NODE_NUM];
} AEnemyPathfinding;



AEnemyPathfinding *createEnemyPathfinding()
{
  AEnemyPathfinding *toReturn = malloc(sizeof(AEnemyPathfinding));
  return toReturn;
}


void freeEnemyPathfinding(AEnemyPathfinding *myEnemyPathfinding)
{
  if (myEnemyPathfinding)
    free(myEnemyPathfinding);
}


AEnemyPathfinding **EnemyPathfinding;


int CalculateDistanceCost(int enemy_id,int a, int b);
int smallest_f_cost(int enemy_id);
void InitEnemyPathfinding(int enemy_id,float target_x,float target_y);
void EnemyPathFinding(int enemy_id);
void EnemySpriteOnGroundId(int enemy_id,int ground_id);
void LargeEnemySpriteTimer(int enemy_id);
void EnemyGravity(int enemy_id,int gr);
void EnemyMove(int enemy_id);
void EnemyTargetPlayer(int i);
void EnemyLOSAct(int i);
void EnemyKnockbackMove(int i,int ground_id);
void EnemySndAct(int i);
int EnemyActDazzle(int i, int slash_t);
void EnemyActWebStuck(int i);
void EnemyAntActOnGround(int i,float height_from_e_x,int ground_id,bool clockwise);
void EnemyAntActOnGroundEdge(int i,bool clockwise);
void EnemyAntGravity(int enemy_id);
void EnemyAntActGravity(int i,int slash_time_i);
void EnemyAntAct(int i,int slash_time_i);
bool InsectBites(int i,int dmg,bool is_mosquito);
void EnemyDeductMaxHealth(int i);
void EnemyAct(int i);
void SetEnemyByType(int i,int type);
void InitEnemySpritesObjColor(HDC hdc,HDC hdc2);
void InitEnemySpritesObjColorNoir(HDC hdc,HDC hdc2);
void InitEnemySpritesObj();
void InitPFEnemyObj();
void InitEnemy();
void DrawEnemy(HDC hdc,HDC hdc2);

//==============PLAYER SPRITES======================
#define M_PI_16     M_PI/16
#define ROTATED_SPRITE_NUM  32 //fear not, 1 variant is very small, 8 bit

#define M_PI_32     M_PI/32
#define PLAYER_ROTATED_SPRITE_NUM  64

struct LoadPlayerSprite
{
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

  //HBITMAP sprite_bee_1;
  //HBITMAP sprite_bee_2;
  //HBITMAP sprite_bee_aero_1;
  //HBITMAP sprite_bee_aero_2;
} LoadPlayerSprite;


struct PlayerSprite //used in
{
  RGBQUAD PlayerPalette[256]; //2 colors are changed, LTGREEN and LTREED 
  RGBQUAD PlayerBlurPalette[256]; //2 colors are changed, LTGREEN and LTREED 
  DRAWSPRITE sprite_jump;
  DRAWSPRITE blur_sprite_jump;
  DRAWSPRITE spin_sprite[4];
  DRAWSPRITE blur_spin_sprite[4];

  DRAWSPRITE dithered_spin_sprite[4];


  //DRAWSPRITE sprite_bee_1;
  //DRAWSPRITE sprite_bee_2;
  //DRAWSPRITE sprite_bee_aero_1;
  //DRAWSPRITE sprite_bee_aero_2;

  DRAWSPRITE sprite_1[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE sprite_2[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE attack_sprite_1[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE attack_sprite_2[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE attack_sprite_3[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE attack_sprite_4[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE block_sprite_1[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE block_sprite_2[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE block_sprite_3[PLAYER_ROTATED_SPRITE_NUM];

  //Dithered Sprite while in water (im left with no other choice!!)
  DRAWSPRITE dithered_sprite_jump;
  //DRAWSPRITE dithered_sprite_bee_1;
  //DRAWSPRITE dithered_sprite_bee_2;
  //DRAWSPRITE dithered_sprite_bee_aero_1;
  //DRAWSPRITE dithered_sprite_bee_aero_2;

  DRAWSPRITE dithered_sprite_1[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_sprite_2[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_attack_sprite_1[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_attack_sprite_2[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_attack_sprite_3[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_attack_sprite_4[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_block_sprite_1[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_block_sprite_2[PLAYER_ROTATED_SPRITE_NUM];
  DRAWSPRITE dithered_block_sprite_3[PLAYER_ROTATED_SPRITE_NUM];
} PlayerSprite[1];










//==================ENEMY SPRITES==================
//static, saves memory


//loaded
struct LoadEnemyFlySprite
{
  HBITMAP prelude_tmp_sprite1;
  HBITMAP prelude_tmp_sprite2;
  DRAWSPRITE draw_fly_sprite_1;
  DRAWSPRITE draw_fly_sprite_2;
  DRAWSPRITE draw_dithered_fly_sprite_1;
  DRAWSPRITE draw_dithered_fly_sprite_2;
} LoadEnemyFlySprite[5]; //mosquito, flying-termite, fly, cockroach, toe biter



struct LoadEnemyRotatedSprite
{
  HBITMAP prelude_tmp_sprite1[ROTATED_SPRITE_NUM]; //tmp sprite rotated 1
  HBITMAP prelude_tmp_sprite2[ROTATED_SPRITE_NUM]; //tmp sprite rotated 2
  DRAWSPRITE draw_rotated_sprite1[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_rotated_sprite2[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_dithered_rotated_sprite1[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_dithered_rotated_sprite2[ROTATED_SPRITE_NUM];
} LoadEnemyRotatedSprite[3]; //cockroach, toe biter and ant



struct LoadEnemyRotatedSpriteXtra
{
  HBITMAP prelude_tmp_sprite[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_rotated_sprite[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_dithered_rotated_sprite[ROTATED_SPRITE_NUM];
} XLoadEnemyRotatedSprite[1]; //toe biter






//inspired while playing angry birds on Ethihide airlines to Abu Dhabi
//ingame
struct EnemyTypeSprite //on the fly
{
  //int enemy_type_id;
  RGBQUAD enemyPalette[256];
  RGBQUAD enemyPaletteNoir[256];
  DRAWSPRITE draw_fly_sprite_1; //right/left-facing-movesprite-1,right left determined by last_left/is_left/flip_sprite
  DRAWSPRITE draw_fly_sprite_2; //right/left-facing-movesprite-2
  DRAWSPRITE draw_dithered_fly_sprite_1;
  DRAWSPRITE draw_dithered_fly_sprite_2;
} EnemyTypeSprite[ENEMY_TYPE_NUM];



struct EnemyRotatedSprite
{
  int type;
  DRAWSPRITE draw_rotated_sprite1[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_rotated_sprite2[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_dithered_rotated_sprite1[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_dithered_rotated_sprite2[ROTATED_SPRITE_NUM];
} EnemyRotatedSprite[ENEMY_TYPE_NUM];


struct EnemyRotatedSpriteXtra
{
  int type;
  DRAWSPRITE draw_rotated_sprite[ROTATED_SPRITE_NUM];
  DRAWSPRITE draw_dithered_rotated_sprite[ROTATED_SPRITE_NUM];
} XEnemyRotatedSprite[ENEMY_TYPE_NUM];












//============================TILE MAPS=========================

typedef struct TileMap
{
  int x;
  int y;
  HBITMAP sprite_paint;
  HBITMAP sprite_mask;
} ATileMap;

ATileMap *createTileMap()
{
  ATileMap *toReturn = malloc(sizeof(ATileMap));
  return toReturn;
}


void freeTileMap(ATileMap *myTileMap)
{
  if (myTileMap->sprite_paint!=NULL)
    DeleteObject(myTileMap->sprite_paint);
  if (myTileMap->sprite_mask!=NULL)
    DeleteObject(myTileMap->sprite_mask);
  if (myTileMap)
    free(myTileMap);
}


ATileMap **TileMapPlatform;
//ATileMap **TileMapForeground;



typedef struct TileMapPaint
{
  int x;
  int y;
  HBITMAP sprite_paint;
} ATileMapPaint;

ATileMapPaint *createTileMapPaint()
{
  ATileMapPaint *toReturn = malloc(sizeof(ATileMapPaint));
  return toReturn;
}



void freeTileMapPaint(ATileMapPaint *myTileMapPaint)
{
  if (myTileMapPaint->sprite_paint!=NULL)
    DeleteObject(myTileMapPaint->sprite_paint);
  if (myTileMapPaint)
    free(myTileMapPaint);
}


ATileMapPaint **TileMapShadow;


//$#$#$#$#$#$#$#$#$CLEAN UP#$#$#$#$#$#$
void CleanUpGround();
//void CleanUpPlayerSprites(); //clean up all sprites
void CleanupPlayerAttributes();
void CleanUpEnemySprites();
void CleanUpRotatedSprites();
void CleanUpGrid();
void CleanupAll(bool btm);



#define SSTAR_NUM   3
struct shooting_star
{
  //int start_time;
  int cooldown;
  int lifetime;
  float x;
  float y;
  float speed;
  float angle;
} SStar[SSTAR_NUM];



struct color_chooser
{
  bool is_choosing_color;
  int color_id;
  int color_id_choosing;
} color_chooser;


#define STAR_NUM    627
#define MAX_STAR_X  900    
#define MAX_STAR_Y  600
struct star
{
  int size;
  int timer;
  float x;
  float y;
} Star[STAR_NUM];

void SetStar(int i,float x,float y);
void DrawStar(HDC hdc,int x, int y, int size, float angl,int color);
void DrawStars(HDC hdc);

//===========OTHER GROUNDS TEXTURE=============
#define PLATFORM_TEXTURES_NUM 25

HBITMAP LoadedPlatformTextures[PLATFORM_TEXTURES_NUM];

struct GamePlatformTextures
{
  int type;
  int solid_value; //0-255
  int color_id;
  //RGBQUAD palette[256];

  HBITMAP palette_sprite;
} GamePlatformTextures[PLATFORM_TEXTURES_NUM];

//================================




//===========WATER TEXTURE=============
int global_water_texture_timer=0;
int global_water_texture_id=0;
HBITMAP texture_water[9];
BYTE *ptexture_water[9];



//================================




//mouse 
int mouse_x;
int mouse_y;
int true_mouse_x;
int true_mouse_y;
RGBQUAD player_cursor_palette[256];

HBITMAP player_cursor[16]; //4 left and 4 right, open or closed, loaded
HBITMAP player_cursor_cache[16];
DRAWSPRITE draw_player_cursor[16];

/*HBITMAP player_cursorbee[5]; //4 left and 4 right, open or closed, loaded
HBITMAP player_cursorbee_cache[5];
DRAWSPRITE draw_player_cursorbee[5];*/


//enemy global sprites
//mosquito
HBITMAP enemy1_sprite_1;
HBITMAP enemy1_sprite_2;

//cockroach flying
HBITMAP enemy2_sprite_1;
HBITMAP enemy2_sprite_2;
//cockroach walking
HBITMAP enemy2_sprite_3;
HBITMAP enemy2_sprite_4;


//small termite
HBITMAP enemy3_sprite_1;
HBITMAP enemy3_sprite_2;


//កន្ទាទូក walking
HBITMAP enemy4_sprite_1;
HBITMAP enemy4_sprite_2;
HBITMAP enemy4_sprite_1_0;
//កន្ទាទូក flying
HBITMAP enemy4_sprite_3;
HBITMAP enemy4_sprite_4;


//fly 
HBITMAP enemy5_sprite_1;
HBITMAP enemy5_sprite_2;

//Ant Walking
HBITMAP enemy6_sprite_1;
HBITMAP enemy6_sprite_2;






//map platforms for loading in
HBITMAP map_platforms_sprite;
HBITMAP map_water_platforms_sprite;
HBITMAP map_platforms_shadow_shader;



//map background drawn
HBITMAP map_background_sprite;
HBITMAP screen;
HBITMAP screen_mirror;
BYTE *publicScreenPixels,*publicScreenMirrorPixels;// *publicScreenModPixels;

//moon, for calendar only
HBITMAP moon_cartoon_sprite;
HBITMAP moon_cartoon_sprite_cache;
DRAWSPRITE draw_moon_cartoon_sprite;


//background moon
int current_moon_phase_id;
HBITMAP moon_sprite[8];
HBITMAP moon_sprite_cache[8];
DRAWSPRITE draw_moon_sprite[8];

HBITMAP mirror_moon_sprite_cache[8];
DRAWSPRITE draw_mirror_moon_sprite[8];

HBITMAP snowflake_sprite;
HBITMAP snowflake_sprite_cache;
DRAWSPRITE draw_snowflake_sprite;


//Shadow Tile
HBITMAP dkrdkgray_shadow_tile;
HBITMAP ltgray_shadow_tile;



//main menu title
HBITMAP title_sprite[2];
HBITMAP title_sprite_mask[2];
HBITMAP title_small_sprite[2];
HBITMAP title_small_sprite_mask[2];

//Khmer main menu
HBITMAP intro_msg;
HBITMAP intro_msg_mask;


HBITMAP mm0_kh[4];
HBITMAP mm0_kh_mask[4];
HBITMAP mm0_kh_green[4];
HBITMAP mm0_kh_white[4];

HBITMAP mm2_kh[16];
HBITMAP mm2_kh_mask[16];
HBITMAP mm2_kh_green[16];
HBITMAP mm2_kh_white[16];

HBITMAP mm0_kh_hijri[2];
HBITMAP mm0_kh_hijri_mask[2];

HBITMAP kh_goback;
HBITMAP kh_goback_mask;

HBITMAP kh_cornmid;
HBITMAP kh_cornmid_mask;
HBITMAP kh_cornmid_green;
HBITMAP kh_cornmid_white;


HBITMAP kh_bool[2]; //on/off
HBITMAP kh_bool_green[2];
HBITMAP kh_bool_white[2];
HBITMAP kh_bool_mask[2];

HBITMAP kh_difficulty[2]; //easy/hard
HBITMAP kh_difficulty_green[2];
HBITMAP kh_difficulty_white[2];
HBITMAP kh_difficulty_mask[2];


HBITMAP kh_pressanykey;
HBITMAP kh_pressanykey_mask;

//Khmer ingame, while playing game
HBITMAP ga0_khhard[4];
HBITMAP ga0_khhard_mask[4];
HBITMAP ga0_kh[5];
HBITMAP ga0_kh_mask[5];


HBITMAP _bb;

//raindrop
int sc_raindrop_num_=0;
int sc_raindrop_id_1=0;
int sc_raindrop_id_2=0;
#define SC_RAINDROP_NUM  240
struct screenraindrop
{
  //bool once;
  int lifetime,olifetime;
  float oy,x,y,speed;
} sc_raindrop[SC_RAINDROP_NUM];




//Sound
//Sound called in the Stack
#define SPAM_SFX_NUM    9
#define KEY_SFX_NUM     7
#define CHANNEL_SFX_NUM 13

wavSoundEffect spamSoundEffect[SPAM_SFX_NUM];
wavSoundEffect keySoundEffect[KEY_SFX_NUM];
wavSoundEffect channelSoundEffect[CHANNEL_SFX_NUM];

wavSoundEffectCache spamSoundEffectCache[SPAM_SFX_NUM];
wavSoundEffectCache keySoundEffectCache[KEY_SFX_NUM];
wavSoundEffectCache channelSoundEffectCache[CHANNEL_SFX_NUM];


//Global blend function for transparency
BLENDFUNCTION gblendFunction;
BLENDFUNCTION waterBlendFunction;
BLENDFUNCTION underwaterBlendFunction;


void SaveLvlBmp(HWND hwnd,HDC hdc,const wchar_t *lvl_name);



//rng values
int weather_rng_i=0;
int misc_rng_i=0;
int sstar_rng_i=0;
int star_rng_i=0;


float enemy_rotated_angle_arr[ROTATED_SPRITE_NUM];
