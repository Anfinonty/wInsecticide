//classes for game

struct Ground 
{
  bool within_render_distance,already_in_grid[GRID_NUM];
  int saved_pos_in_grid[GRID_NUM];
  double x1,y1,x2,y2,angle,length,gradient,c,height_from_player_x;
} Ground[GROUND_NUM];


struct player 
{
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
    //WIDTH,
    //HEIGHT,
    player_jump_height,
    key_jump_timer,

    //Render
    rendered_grid_num,
    render_grids[RDGRID_NUM],
    //rendered_enemy_num,
    //render_enemies[ENEMY_NUM],
    rendered_ground_num,
    render_grounds[GROUND_NUM];

  double saved_x,saved_y,x,y,sprite_angle,angle,player_grav;

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


void CameraInit(double x,double y)
{
  while (x<GR_WIDTH/2) {
    player.cam_x++;
    x++;
  }
  while (x>GR_WIDTH/2) {
    player.cam_x--;
    x--;
  }
  while (y<GR_HEIGHT/2) {
    player.cam_y++;
    y++;
  }
  while (y>GR_HEIGHT/2) {
    player.cam_y--;
    y--;
  }  
}


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




