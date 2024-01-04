//classes for game

struct Ground {
  bool already_in_grid[GRID_NUM];
  int saved_pos_in_grid[GRID_NUM];
  double x1,y1,x2,y2,angle,length,gradient,c,height_from_player_x;
} Ground[GROUND_NUM];

struct player {
  bool  rst_down,
        rst_left,
        rst_right,
        rst_up,
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
    WIDTH,
    HEIGHT,
    player_jump_height,
    key_jump_timer;

  double x,y,sprite_angle,angle,player_grav;

//sprites
  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_jump;
} player;


struct Grid {
  int max_ground_num,
    ground_ids[MAX_GROUNDS_WITHIN_GRID],
    x1,y1,x2,y2;
} Grid[GRID_NUM];


struct Node
{
  bool node_solid;
  int x1,y1,x2,y2;
} NodeGrid[MAP_NODE_NUM];


