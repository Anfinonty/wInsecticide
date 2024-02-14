

void CleanUpEnemy()
{
  int i=0,j=0;
  for (i=0;i<MAX_ENEMY_NUM;i++) {
    //printf("Enemy: %d\n",i);
    Enemy[i].species=-1;
    Enemy[i].type=0;
    Enemy[i].angle=0;
    Enemy[i].saved_angle=0;
    Enemy[i].sprite_angle=0;

  Enemy[i].being_drawn=TRUE;
  Enemy[i].health=0;
  Enemy[i].max_health=0;
  Enemy[i].follow_range=0;
  Enemy[i].chase_range=0;
  Enemy[i].unchase_range=0;
  Enemy[i].unchase_range=0;
  Enemy[i].chase_range=0;
  Enemy[i].color=0;
  Enemy[i].speed=0;
  Enemy[i].speed_multiplier=0;
//bool
  Enemy[i].time_breaker_immune=0;
//shoot
  Enemy[i].bullet_fire_cooldown_max=
  Enemy[i].bullet_fire_cooldown=0;
  Enemy[i].bullet_fire_at_once_max=0;
  Enemy[i].bullet_length_max=0;
  Enemy[i].bullet_length=0;
  Enemy[i].shoot_at_player_range=0;
  Enemy[i].aim_rand=0;
//bullet
  Enemy[i].bullet_damage=0,
  Enemy[i].bullet_cooldown_max=0;
  Enemy[i].bullet_speed=0;
  Enemy[i].bullet_speed_multiplier=0;
  Enemy[i].bullet_range=0;
  Enemy[i].bullet_color=0;
  Enemy[i].bullet_graphics_type=0;
  //time breaker
  Enemy[i].time_breaker_rare=0;
  Enemy[i].time_breaker_length=0;

    Enemy[i].on_ground_id=-1;
    Enemy[i].seed=0;
    Enemy[i].dist_from_player=999;
    Enemy[i].x=-20;
    Enemy[i].y=-20;
    Enemy[i].sprite_x=
    Enemy[i].sprite_y=-20;
    Enemy[i].above_ground=
    Enemy[i].below_ground=FALSE;
    Enemy[i].in_air_timer=0;
    Enemy[i].angle=0;
    Enemy[i].sprite_angle=0;
    Enemy[i].saved_angle=-9999;
  //bullet
    Enemy[i].bullet_shot_num=0;
    for (j=0;j<BULLET_NUM*1;j++) {
      Enemy[i].bullet_shot_arr[j]=-1;
    }
    for (j=0;j<MAX_BULLET_PER_FIRE;j++) {
      Enemy[i].bullet_head_x[j]=0;
      Enemy[i].bullet_head_y[j]=0;
    }
    Enemy[i].knockback_left=FALSE;
    Enemy[i].knockback_angle=0;
    Enemy[i].knockback_timer=0;
    Enemy[i].shoot_target_x=0;
    Enemy[i].shoot_target_y=0;
  //LOS
    Enemy[i].LOS_left=FALSE;
    Enemy[i].LOS_shot=FALSE;
    Enemy[i].LOS_x=-20;
    Enemy[i].LOS_y=-20;
    Enemy[i].LOS_angle=0;
    Enemy[i].LOS_target_x=-20;
    Enemy[i].LOS_target_y=-20;
  //init default bool
    player.rendered_enemy_num=0;
    Enemy[i].target_player=FALSE;
    Enemy[i].saw_player=FALSE;
    Enemy[i].within_render_distance=FALSE;
    Enemy[i].idling=TRUE;
    Enemy[i].last_left=FALSE;
    Enemy[i].last_left=FALSE;
    Enemy[i].found_target=FALSE;
    Enemy[i].search_target=FALSE;
    Enemy[i].trace_target=FALSE;
    Enemy[i].move_to_target=FALSE;
    Enemy[i].ignore_player=TRUE;
  //init default int 
    //Enemy[i].snd_dur=0;
    Enemy[i].path_nodes_num=0;
    Enemy[i].sprite_timer=0;
    Enemy[i].idle_timer=0;
    Enemy[i].search_timer=0;
    Enemy[i].forgor_timer=100;
    Enemy[i].player_at_left=FALSE;
    Enemy[i].player_at_right=FALSE;
    Enemy[i].player_at_above=FALSE;
    Enemy[i].player_at_below=FALSE;
    Enemy[i].in_unchase_range=FALSE;
    Enemy[i].in_chase_range=FALSE;
  //init once
    for (j=0;j<MAX_NODE_NUM;j++) {
      Enemy[i].node_solid[j]=
        Enemy[i].node_back[j]=
        Enemy[i].node_open[j]=
        Enemy[i].node_closed[j]=FALSE;
      Enemy[i].node_x[j]=(Enemy[i].x/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(Enemy[i].follow_range/2);
      Enemy[i].node_y[j]=(Enemy[i].y/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(Enemy[i].follow_range/2);
      Enemy[i].node_hcost[j]=0;
      Enemy[i].node_gcost[j]=INT_MAX;
      Enemy[i].node_fcost[j]=Enemy[i].node_gcost[j]+Enemy[i].node_hcost[j];
      Enemy[i].node_parent[j]=-1;
    }
    Enemy[i].start_node=-20;//GetGridId(x,y,Enemy[i].follow_range*NODE_SIZE,NODE_SIZE,Enemy[i].node_num);
    Enemy[i].end_node=-20;//GetGridId(x,y,Enemy[i].follow_range*NODE_SIZE,NODE_SIZE,Enemy[i].node_num);
    Enemy[i].open_nodes_num=0;
    for (j=0;j<MAX_NODE_NUM/2;j++) {
      Enemy[i].open_nodes[j]=Enemy[i].start_node;
    }
  }
}



void CleanUpNodeGrid()
{
  for (int i=0;i<MAX_MAP_NODE_NUM;i++) {
    NodeGrid[i].node_solid=FALSE;
    NodeGrid[i].non_web=FALSE;
    NodeGrid[i].x1=-20;
    NodeGrid[i].y1=-20;
    NodeGrid[i].x2=-20;
    NodeGrid[i].y2=-20;
  }
}





void CleanUpGrid()
{
  int i=0,j=0,x=0,y=0;
  for (i=0;i<MAX_VGRID_NUM;i++) {
    VGrid[i].within_render_distance=FALSE;
    VGrid[i].max_ground_num=0;
    
    for (j=0;j<MAX_GROUNDS_WITHIN_GRID;j++) {
      VGrid[i].ground_ids[j]=-1;
    }
    VGrid[i].x1=-20;
    VGrid[i].y1=-20;
    VGrid[i].x2=-20;
    VGrid[i].y2=-20;
  }
}





void CleanUpGround()
{
  for (int i=0;i<MAX_GROUND_NUM+MAX_WEB_NUM;i++) {
    for (int j=0;j<MAX_VGRID_NUM;j++) {
      Ground[i].already_in_grid[j]=FALSE;
      Ground[i].saved_pos_in_grid[j]=-1;
    }
    Ground[i].x1=-20;
    Ground[i].y1=-20;
    Ground[i].x2=-20;
    Ground[i].y2=-20;
    Ground[i].x3=-20;
    Ground[i].y3=-20;
    Ground[i].color=0;
    Ground[i].text[0]='\0';
    Ground[i].is_ghost=FALSE;
    Ground[i].health=-1;
    Ground[i].height_from_player_x=0;
    Ground[i].within_render_distance=FALSE;
    Ground[i].angle=0;
  }
}


