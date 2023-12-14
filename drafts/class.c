
//FinishBox
double fb_x1=0,fb_x2=0,fb_y1=0,fb_y2=0;

//Player
int web_being_shot=-1,
    player_color=BLACK,
    player_bullet_shot=-1,
    //Dynamic attributes
    player_speed_breaker_cooldown_max=0,
    player_speed_breaker_cooldown=0,
    player_speed_breaker_units=0,
    player_speed_breaker_max=0,
    player_speed_breaker_recharge_cooldown_max=0,
    player_speed_breaker_recharge_cooldown=0,
    player_jump_height=0,
    //Attack
    player_knockback_strength=0,
    player_attack_strength=0,
    //block
    player_block_recharge_timer_max=125, //(251) every 1.5/2 secs, ++ player_block_health
    player_block_recharge_timer=0,
    player_block_cooldown_max=251, //(501)start recharge only after 3 secs after blocking
    player_block_cooldown=0,
    player_block_timer=0,
    //HP
    player_health=1,
    //Build range
    player_build_range=0,
    //Web
    player_max_web_num=0,
    player_web_health=0,
    player_speed=0,
  //Game function attributes
    //noise
    player_snd_dur=0,
    player_snd_pitch=0,
    player_snd_rand=0,
    //soul
    enemy_kills=0,
    souls_taken=0,
    animate_regen_timer=0,
    regen_type=0,
    //other
    sleep_timer=_DEFAULT_SLEEP_TIMER,
    grav=2,
    player_web_type=1,
    player_lock_web_type=1,
    player_in_air_timer=0,
    player_attack_timer=0,
    player_in_air_cooldown=0,
    on_ground_id=-1,
    on_ground_timer=0,
    key_jump_timer=0,
    walk_cycle=0,
    saved_ground_id=-1,
    player_knockback_speed_multiplier=0,
    //web
    placed_web_num=0,
    destroyed_web_pos=0,
    placed_web_pos=0,
    player_web_storage[MAX_WEB_NUM],
    //cdweb
    cdwebs[MAX_WEB_NUM],
    cdweb_num=0,
    cdweb_pos=0,
    cdweb_player_pos=0,
    //Debug
    //print_on_ground_id=0,
    //Render
    rendered_grid_num=0,
    render_grids[RDGRID_NUM],
    rendered_enemy_num=0,
    render_enemies[ENEMY_NUM],
    rendered_ground_num=0,
    render_grounds[GROUND_NUM+MAX_WEB_NUM];
    //player shot bullet
    //player_shot_bullet_arr[MAX_WEB_NUM];

double //Game function attributes
    player_block_health_max=DEFAULT_PLAYER_BLOCK_HEALTH_MAX,
    player_block_health=DEFAULT_PLAYER_BLOCK_HEALTH_MAX,
    player_cur_x=0,
    player_cur_y=0,
    jump_height=0,
    player_grav=0.5,
    player_claws_x=0,
    player_claws_y=0,
    player_knockback_speed=0,
    player_x=0,
    player_y=0,
    player_sprite_x=GR_WIDTH/2,
    player_sprite_y=GR_HEIGHT/2,
    player_start_x=0,
    player_start_y=0,
    player_angle=0,
    above_player_x=0,
    above_player_y=0,
    above_player_x2=0,
    above_player_y2=0,
    background_cam_move_x=0,
    background_cam_move_y=0,
    //Draw
    player_E=0,
    player_E_x=0,
    player_E_y=0,
    player_sprite_E=0,

    edge_x=0,edge_y=0;


bool //Game function attributes
     player_hiding=FALSE,
     player_blocking=FALSE,
     build_mode=FALSE,
     uppercut=FALSE,
     player_attack=FALSE,
     destroy_ground=FALSE,
     valid_web=FALSE,
     print_valid_web=FALSE,
     jump=FALSE,
     last_left=FALSE,
     change_view=FALSE,
     rst_key_sprint=FALSE,
     rst_speed_break=FALSE,
     rst_left=FALSE,
     rst_right=FALSE,
     rst_up=FALSE,
     rst_down=FALSE,
     player_attack_rst=FALSE,
     print_current_above=FALSE,
     print_current_below=FALSE,
     current_above=FALSE,
     current_below=FALSE,
     previous_above=FALSE,
     previous_below=FALSE;

//Game
int restart_timer=0,
    bg_cam_fall_cooldown=0,
//the bravery tyrrant
    the_bravery_tyrant_timer=0,
    the_bravery_tyrant_max=501,
//combo ender
    player_hit_cooldown_timer=0,
    player_hit_cooldown_timer_max=100,
//combos
    combo_timer[COMBO_NUM]={0,0,0},
    combo_streak[COMBO_NUM]={0,0,0},
//
    enemy_hit_global_timer=0,
//game rating
    player_speed_breaker_recharge_minus=0,
    game_rating_timer_max=201,
    game_rating_timer=0,
//
    player_combo_min_dodge[4]={5,15,25,35},
    player_combo_min_hit[4]={5,15,25,30},
    player_combo_min_kill[4]={3,6,9,12},
    *player_combo_min_arr[COMBO_NUM]={
	player_combo_min_dodge,
	player_combo_min_hit,
	player_combo_min_kill
    },
//
    player_sbrm_reward_dodge[3]={
      0,
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN/2,
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-10
    },
    player_sbrm_reward_hit[3]={
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN/10,
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN/4,
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN/3
    },
    player_sbrm_reward_kill[3]={
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN/3,
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN/2,
      DEFAULT_PLAYER_SPEED_BREAKER_RECHARGE_COOLDOWN-20
    },
    *player_sbrm_reward[COMBO_NUM]={
	player_sbrm_reward_dodge,
	player_sbrm_reward_hit,
	player_sbrm_reward_kill
    },
//
    performance_grade_arr[COMBO_NUM]={-1,-1,-1},
    performance_type=-1,
    performance_txt_timer=0,
//
    units_since_sb_max=0;

if (BigResolution) {
  the_bravery_tyrant_max=651;
}


// psm
#define PSM_NUM		100
int   psmht_max=5,current_psm=0,psm_hold_timer=0,cpsm_max=0;
class PlayerSpriteMulti
{
  int walk_cycle,attack_timer,appear_timer,block_timer;
  bool is_jump,is_left;
  double angle,x,y,sprite_x,sprite_y;
} PSM[PSM_NUM];


//Songs
#define SONG_NUM 2
int song_type=0;
double song_tempo[SONG_NUM]={5.100,5.100},
    song_stacatto[SONG_NUM]={0.880,0.835};

U8 **performance_grade_txt[3]={"Good"," Great"," Awesome"},
   **performance_type_txt[3]={"Dodging ","Fighting"," Kill Streak"},
   **over_performance_txt_arr0[3]={ "BLOOD SUGAR"," ADRENALINE JUNKIE"," DAREDEVIL"},
   **over_performance_txt_arr1[3]={" LIKE KNIVES"," APEX PREDATOR","BESERKER"},
   **over_performance_txt_arr2[3]={" INSECTICIDE"," INSECTICIDE"," INSECTICIDE"},
   **over_performance_txt_arr[COMBO_NUM]={
     over_performance_txt_arr0,
     over_performance_txt_arr1,
     over_performance_txt_arr2
   },
   *over_performance_txt="";

double current_time1=0,current_time2=0,tta=0,t0=0,tdiff=0;
bool the_bravery_tyrant=FALSE,
     is_draw_grid=FALSE,
     game_over=FALSE,
     beep_once=FALSE,
     win=FALSE,
     celebrate=FALSE,
     game_help=FALSE;


class RenderDistanceGrid
{
  int x,y;
} RDGrid[RDGRID_NUM];


class Grid
{
  bool within_render_distance;
  int max_ground_num,
      ground_ids[MAX_GROUNDS_WITHIN_GRID],
      enemy_occupy_num,
      enemy_occupy[ENEMY_NUM],
      x1,
      y1,
      x2,
      y2;
} Grid[GRID_NUM];

class NodeGrid//Subject to change
{
  bool node_solid;
  int x1,y1,x2,y2;
} NodeGrid[MAP_NODE_NUM];

#define TOTAL_GROUNDS	GROUND_NUM+MAX_WEB_NUM
#define	MULTI_SPRITE_NUM	4

int current_gsm=0;
int gsm_hold_timer=0;
class GroundLine
{
  bool is_ghost,
       within_render_distance,
       already_in_grid[GRID_NUM];//occupies multiple grids
  int health,
      color,
      type,
      appear_timer[MULTI_SPRITE_NUM],
      saved_pos_in_grid[GRID_NUM];
  double msprite_x1[MULTI_SPRITE_NUM],
      msprite_y1[MULTI_SPRITE_NUM],
      msprite_x2[MULTI_SPRITE_NUM],
      msprite_y2[MULTI_SPRITE_NUM],
      sprite_x1,
      sprite_y1,
      sprite_x2,
      sprite_y2,
      sprite_x3,
      sprite_y3,
      x1,
      y1,
      x2,
      y2,
      x3,
      y3,
      gradient,
      c,
      angle,
      height_from_player_x;
  U8 *text;
} Ground[TOTAL_GROUNDS];

int current_bullet_id=0;
class Bullet
{
  bool shot,left,near_miss;
  int color,
      speed_multiplier,
      damage,
      from_enemy_id,
      graphics_type,
      saved_pos;
  double sprite_x,sprite_y,start_x,start_y,
      x,y,E,speed,range;
  //msprite
  int msprite_hold_timer,
      msprite_hold_timer_max,
      current_sm,
      appear_timer[MULTI_SPRITE_NUM];
  double msprite_x[MULTI_SPRITE_NUM],
      msprite_y[MULTI_SPRITE_NUM];
} Bullet[BULLET_NUM];


class Enemy
{
  //Line Of Sight bullet
  bool LOS_left,LOS_shot;
  double LOS_x,LOS_y,LOS_E,LOS_target_x,LOS_target_y;
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
  double knockback_E;
  int knockback_timer;
  //Enemy Attributes
  bool saw_player,
       last_left,
       target_player,
       ignore_player,
       idling,
       found_target,
       search_target,
       trace_target,
       move_to_target,
       player_at_left,
       player_at_right,
       player_at_above,
       player_at_below,
       within_render_distance,
       above_ground,
       below_ground,
       in_chase_range,
       in_unchase_range,
       tyrant_immune;
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
      tbt_rare,
      tbt_length,
     //snd
      snd_dur,
      snd_dur_max,
      snd_pitch,
      snd_rand,
      death_snd_dur_max,
      death_snd_pitch,
      death_snd_rand;
  double shoot_target_x,shoot_target_y,
      x,y,E,dist_from_player,
      sprite_x,
      sprite_y,
      speed;
//msprite
  bool msprite_last_left[MULTI_SPRITE_NUM];
  double msprite_x[MULTI_SPRITE_NUM],
      msprite_y[MULTI_SPRITE_NUM],
      msprite_E[MULTI_SPRITE_NUM]; 
  int msprite_hold_timer,
      msprite_hold_timer_max,
      current_sm,
      appear_timer[MULTI_SPRITE_NUM],
      msprite_in_air_timer[MULTI_SPRITE_NUM],
      msprite_timer[MULTI_SPRITE_NUM];
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
} Enemy[ENEMY_NUM];
