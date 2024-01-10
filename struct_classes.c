//classes for game

struct Ground 
{
  bool is_ghost, //Can be colided or not collided
       within_render_distance, //Is the ground within the player's Render Distance Grid'
       already_in_grid[GRID_NUM]; //Is the Ground occupying the Grid it is in

  int health, //For Webs, health of ground
      color, //Color of ground, RGB()
      type, //Regular, Text ground or TriFill 
      saved_pos_in_grid[GRID_NUM];


  double x1,y1,x2,y2,x3,y3, //3 Axes of ground
         sprite_x1,sprite_y1,sprite_x2,sprite_y2,sprite_x3,sprite_y3,//graphics 3 Axes of ground
         length, // = sqrt(x^2 + y^2)) // Babylonian Trigonometry Theorem for Calculating the Hypothenuse
         gradient,// y of ground / x of ground
         c, //The C in y-mx_c
         height_from_player_x,//= gradient * player_x + c
         angle; // = ACos of (x/length)

  char *text; //for type 1 which is Text_ground

} Ground[GROUND_NUM];


struct player 
{
  bool  rst_down,
        rst_left,
        rst_right,
        rst_up,
        rst_key_sprint,
        last_left,
        jump,
        current_above,
        current_below,
        previous_above,
        previous_below;


  int grav,
    jump_height,
    sprite_timer,
    in_air_timer, 
    speed, 
    on_ground_timer, 
    on_ground_id, 
    saved_ground_id,
    walk_cycle,
    player_jump_height,
    key_jump_timer,

    //Render
    rendered_grid_num,
    render_grids[RDGRID_NUM],
 
   //rendered_enemy_num,
    //render_enemies[ENEMY_NUM],

    rendered_ground_num,
    render_grounds[GROUND_NUM];

  double saved_x,
         saved_y,
         x,
         y,
         sprite_angle,
         angle,
         player_grav;

  double cam_x,cam_y;
    //cam_move_x=0,
    //cam_move_y=0,
    //background_cam_move_x=0,
    //background_cam_move_y=0;

//sprites
  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_jump;
} player;


struct RenderDistanceGrid
{
  int x,y;
} RDGrid[RDGRID_NUM];



struct Grid 
{
  bool within_render_distance;
  int max_ground_num, //how many grounds are occupying this grid
    ground_ids[MAX_GROUNDS_WITHIN_GRID], //id of grounds that are occupying this grid
    //enemy_occupy_num, //how many enemies are occupying this grid
    //enemy_occupy[ENEMY_NUM], //id of enemies occupying this grid
    x1,y1,x2,y2; //grid axes
} Grid[GRID_NUM];


struct Node
{
  bool node_solid;
  int x1,y1,x2,y2;
} NodeGrid[MAP_NODE_NUM];


/*struct Enemy
{
  //Line Of Sight bullet
  //bool LOS_left,LOS_shot;
  //double LOS_x,LOS_y,LOS_angle,LOS_target_x,LOS_target_y;
  //Bullet
  /*int bullet_cooldown_max,
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
      bullet_shot_arr[BULLET_NUM*1];*/
  //double bullet_speed;


  //Grid interaction
  /*int grid_queue[GRID_NUM],
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
      time_breaker_length;*/
     //snd
      /*snd_dur,
      snd_dur_max,
      snd_pitch,
      snd_rand,
      death_snd_dur_max,
      death_snd_pitch,
      death_snd_rand;


  /*double shoot_target_x,shoot_target_y,
      x,y,angle,dist_from_player,
      //sprite_x,
      //sprite_y,
      speed;*/
//msprite
  /*bool msprite_last_left[MULTI_SPRITE_NUM];
  double msprite_x[MULTI_SPRITE_NUM],
      msprite_y[MULTI_SPRITE_NUM],
      msprite_angle[MULTI_SPRITE_NUM]; 
  int msprite_hold_timer,
      msprite_hold_timer_max,
      current_sm,
      appear_timer[MULTI_SPRITE_NUM],
      msprite_in_air_timer[MULTI_SPRITE_NUM],
      msprite_timer[MULTI_SPRITE_NUM];*/


  //Pathfinding attributes
/*  int current_node,
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
} Enemy[ENEMY_NUM];*/


