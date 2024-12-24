

void CleanUpEnemy()
{
  int i=0,j=0;
  for (i=0;i<ENEMY_NUM;i++) {
    //printf("Enemy: %d\n",i);
    Enemy[i]->species=-1;
    Enemy[i]->type=0;
    Enemy[i]->angle=0;
    Enemy[i]->saved_angle=0;
    Enemy[i]->sprite_angle=0;

  //Enemy[i]->being_drawn=TRUE;
  Enemy[i]->health=0;
  Enemy[i]->play_death_snd=FALSE;
  Enemy[i]->max_health=0;
  Enemy[i]->follow_range=0;
  Enemy[i]->chase_range=0;
  Enemy[i]->unchase_range=0;
  Enemy[i]->unchase_range=0;
  Enemy[i]->chase_range=0;
  Enemy[i]->color=0;
  Enemy[i]->speed=0;
  Enemy[i]->speed_multiplier=0;
//bool
  Enemy[i]->time_breaker_immune=0;
//shoot
  Enemy[i]->bullet_fire_cooldown_max=
  Enemy[i]->bullet_fire_cooldown=0;
  Enemy[i]->bullet_fire_at_once_max=0;
  Enemy[i]->bullet_length_max=0;
  Enemy[i]->bullet_length=0;
  Enemy[i]->shoot_at_player_range=0;
  Enemy[i]->aim_rand=0;
//bullet
  Enemy[i]->bullet_damage=0,
  Enemy[i]->bullet_cooldown_max=0;
  Enemy[i]->bullet_speed=0;
  Enemy[i]->bullet_speed_multiplier=0;
  Enemy[i]->bullet_range=0;
  Enemy[i]->bullet_color=0;
  Enemy[i]->bullet_graphics_type=0;
  //time breaker
  Enemy[i]->time_breaker_rare=0;
  Enemy[i]->time_breaker_length=0;

    Enemy[i]->on_ground_id=-1;
    Enemy[i]->seed=0;
    Enemy[i]->dist_from_player=999;
    Enemy[i]->x=-20;
    Enemy[i]->y=-20;
    Enemy[i]->sprite_x=
    Enemy[i]->sprite_y=-20;
    Enemy[i]->above_ground=
    Enemy[i]->below_ground=FALSE;
    Enemy[i]->in_air_timer=0;
    Enemy[i]->angle=0;
    Enemy[i]->sprite_angle=0;
    Enemy[i]->saved_angle=-9999;
  //bullet
    Enemy[i]->bullet_shot_num=0;
    for (j=0;j<BULLET_NUM*1;j++) {
      Enemy[i]->bullet_shot_arr[j]=-1;
    }
    for (j=0;j<MAX_BULLET_PER_FIRE;j++) {
      Enemy[i]->bullet_head_x[j]=0;
      Enemy[i]->bullet_head_y[j]=0;
    }
    Enemy[i]->knockback_left=FALSE;
    Enemy[i]->player_knockback=FALSE;
    Enemy[i]->knockback_angle=0;
    Enemy[i]->knockback_timer=0;
    Enemy[i]->shoot_target_x=0;
    Enemy[i]->shoot_target_y=0;
  //LOS
    Enemy[i]->LOS_left=FALSE;
    Enemy[i]->LOS_shot=FALSE;
    Enemy[i]->LOS_x=-20;
    Enemy[i]->LOS_y=-20;
    Enemy[i]->LOS_angle=0;
    Enemy[i]->LOS_target_x=-20;
    Enemy[i]->LOS_target_y=-20;
  //init default bool
    //player.rendered_enemy_num=0;
    Enemy[i]->target_player=FALSE;
    Enemy[i]->saw_player=FALSE;
    Enemy[i]->within_render_distance=FALSE;
    Enemy[i]->idling=TRUE;
    Enemy[i]->last_left=FALSE;
    Enemy[i]->last_left=FALSE;
    Enemy[i]->found_target=FALSE;
    Enemy[i]->search_target=FALSE;
    Enemy[i]->trace_target=FALSE;
    Enemy[i]->move_to_target=FALSE;
    Enemy[i]->ignore_player=TRUE;
  //init default int 
    //Enemy[i]->snd_dur=0;
    Enemy[i]->path_nodes_num=0;
    Enemy[i]->sprite_timer=0;
    Enemy[i]->idle_timer=0;
    Enemy[i]->search_timer=0;
    Enemy[i]->forgor_timer=100;
    Enemy[i]->player_at_left=FALSE;
    Enemy[i]->player_at_right=FALSE;
    Enemy[i]->player_at_above=FALSE;
    Enemy[i]->player_at_below=FALSE;
    Enemy[i]->in_unchase_range=FALSE;
    Enemy[i]->in_chase_range=FALSE;
  //init once
    for (j=0;j<MAX_NODE_NUM;j++) {
      Enemy[i]->node_solid[j]=
        Enemy[i]->node_back[j]=
        Enemy[i]->node_open[j]=
        Enemy[i]->node_closed[j]=FALSE;
      Enemy[i]->node_x[j]=(Enemy[i]->x/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(Enemy[i]->follow_range/2);
      Enemy[i]->node_y[j]=(Enemy[i]->y/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(Enemy[i]->follow_range/2);
      Enemy[i]->node_hcost[j]=0;
      Enemy[i]->node_gcost[j]=INT_MAX;
      Enemy[i]->node_fcost[j]=Enemy[i]->node_gcost[j]+Enemy[i]->node_hcost[j];
      Enemy[i]->node_parent[j]=-1;
    }
    Enemy[i]->start_node=-20;//GetGridId(x,y,Enemy[i]->follow_range*NODE_SIZE,NODE_SIZE,Enemy[i]->node_num);
    Enemy[i]->end_node=-20;//GetGridId(x,y,Enemy[i]->follow_range*NODE_SIZE,NODE_SIZE,Enemy[i]->node_num);
    Enemy[i]->open_nodes_num=0;
    for (j=0;j<MAX_NODE_NUM/2;j++) {
      Enemy[i]->open_nodes[j]=Enemy[i]->start_node;
    }
  }
}



void CleanUpNodeGrid()
{
  for (int i=0;i<MAP_NODE_NUM;i++) {
    NodeGrid[i]->node_solid=FALSE;
    NodeGrid[i]->non_web=FALSE;
    NodeGrid[i]->x1=-20;
    NodeGrid[i]->y1=-20;
    NodeGrid[i]->x2=-20;
    NodeGrid[i]->y2=-20;
  }
}





void CleanUpGrid()
{
  //int i=0,j=0;
  /*for (i=0;i<VGRID_NUM;i++) {
    VGrid[i]->within_render_distance=FALSE;
    VGrid[i]->max_ground_num=0;
    
    for (j=0;j<MAX_GROUNDS_WITHIN_GRID;j++) {
      VGrid[i]->ground_ids[j]=-1;
    }
    VGrid[i]->x1=-20;
    VGrid[i]->y1=-20;
    VGrid[i]->x2=-20;
    VGrid[i]->y2=-20;
  }*/
  bool yes_shadow=FALSE;
  if (is_shadows && game_shadow) {
    yes_shadow=TRUE;
  }
  if (PLATFORM_GRID_NUM>0) {
    for (int i=0;i<PLATFORM_GRID_NUM;i++) {
      /*if (TileMapPlatform[i]->sprite_paint!=NULL) {
        DeleteObject(TileMapPlatform[i]->sprite_paint);
      }
      if (TileMapPlatform[i]->sprite_mask!=NULL) {
        DeleteObject(TileMapPlatform[i]->sprite_mask);
      }*/
      FreeDrawSprite(&TileMapPlatform[i]->draw_tile);
    }
  }

  if (FOREGROUND_GRID_NUM>0) {
    for (int i=0;i<FOREGROUND_GRID_NUM;i++) {
      /*if (TileMapForeground[i]->sprite_paint!=NULL) {
        DeleteObject(TileMapForeground[i]->sprite_paint);
      } 
      if (TileMapForeground[i]->sprite_mask!=NULL) {
        DeleteObject(TileMapForeground[i]->sprite_mask);
      }*/
      FreeDrawSprite(&TileMapForeground[i]->draw_tile);
    }
  }

  if (yes_shadow) { //IMPORTANT: refrain from freeing empty pointers or mem leak and then crash
      for (int i=0;i<SHADOW_GRID_NUM;i++) {
        if (TileMapShadow[i]->sprite_paint!=NULL) {
          DeleteObject(TileMapShadow[i]->sprite_paint);
        }
      }
  }

  //printf("freed:%d,%d,%d\n",PLATFORM_GRID_NUM,FOREGROUND_GRID_NUM,SHADOW_GRID_NUM);
}





void CleanUpGround()
{
  for (int i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {
    for (int j=0;j<VGRID_NUM;j++) {
      Ground[i]->already_in_grid[j]=FALSE;
      Ground[i]->saved_pos_in_grid[j]=-1;
    }
    Ground[i]->x1=-20;
    Ground[i]->y1=-20;
    Ground[i]->x2=-20;
    Ground[i]->y2=-20;
    Ground[i]->x3=-20;
    Ground[i]->y3=-20;
    Ground[i]->color=0;
    Ground[i]->text[0]='\0';
    Ground[i]->is_ghost=FALSE;
    Ground[i]->health=-1;
    //Ground[i]->height_from_player_x=0;
    Ground[i]->within_render_distance=FALSE;
    Ground[i]->angle=0;
  }
}


void CleanupPlayerAttributes()
{
  player.is_rebounding=
  player.is_swinging=
//  player.hiding=
  player.left_click=
  player.right_click=
  player.rst_left_click=
  player.rst_right_click=
  player.rst_down=
  player.rst_left=
  player.rst_right=
  player.rst_up=
  //player.rst_key_sprint=
  player.last_left=
  player.jump=
  player.current_above=
  player.current_below=
  player.previous_above=
  player.previous_below=
  player.print_current_above=
  player.print_current_below=
  player.time_breaker=
  player.attack=
  player.blocking=
  player.print_valid_web=
  player.valid_web=
  player.attack_rst=
  player.destroy_ground=
  player.uppercut=
  player.flag_revert_palette=
  player.low_jump=
  player.on_a_ground=FALSE;

  player.knives_per_throw=0;

  player.block_recharge_timer_max=
  player.block_recharge_timer=
  player.block_cooldown_max=
  player.block_cooldown=

  player.left_click_hold_timer=
  player.right_click_hold_timer=
  player.attack_timer=
  player.sleep_timer=
  player.hit_cooldown_timer=
  player.block_timer=
  player.cdweb_pos=
  player.cdweb_player_pos=
  player.cdweb_num=
  player.destroyed_web_pos=
  player.max_web_num=
  player.bullet_shot=
  player.placed_web_pos=
  player.placed_web_num=
  player.web_being_shot=

  player.previous_web_placed=
  player.time_breaker_units=
  player.time_breaker_units_max=
  player.time_breaker_cooldown=
  player.time_breaker_cooldown_max=
  player.time_breaker_recharge_timer=
  player.time_breaker_recharge_timer_max=
  player.time_breaker_units_tick=
  player.time_breaker_units_tick_max=
  player.spin_timer=

  player.bullet_shot_num=
  player.color=
  player.load_color=

  player.grav=
  player.jump_height=
  player.sprite_timer=
  player.in_air_timer=
  player.speed=
  player.on_ground_timer=
  player.on_ground_id=
  player.saved_ground_id=
  player.walk_cycle=
  player.player_jump_height=
//  player.key_jump_timer=
  player.knockback_speed_multiplier=
  player.knockback_strength=
  player.time_breaker_tick=
  player.seed=0;

  player.knockback_speed=
  player.attack_strength=
  player.saved_x=
  player.saved_y=
  player.x=
  player.y=
  player.above_x=
  player.above_y=
  player.above_x2=
  player.above_y2=
  player.sprite_angle=
  player.angle=
  player.saved_angle=
  player.saved_sprite_angle=
  player.saved_block_sprite_angle=
  player.saved_attack_sprite_angle=
  player.player_grav=

  player.cam_x=
  player.cam_y=
  player.cam_move_x=
  player.cam_move_y=
  player.sprite_x=
  player.sprite_y=
  player.claws_x=
  player.claws_y=

  player.block_health_max=
  player.block_health=


  player.health=20;
  player.pivot_x=
  player.pivot_y=
  player.pivot_angle=
  player.pivot_length=
  player.jump_angle=
  player.jump_angle2=
  player.fling_distance=
  player.angle_of_incidence=
  player.angle_of_reflection=0.0;
}



void CleanupAll()
{
    level_loaded=FALSE;
    //InitBullet(BULLET_NUM);
    CleanUpPlayer(); //clean up all sprites
    CleanUpEnemySprites();
    CleanUpRotatedSprites();
    CleanUpGrid();
    //CleanUpNodeGrid();
    //CleanUpEnemy();
    //CleanUpGround();
    CleanupPlayerAttributes();
    save_level[0]='\0';
    call_help_timer=0;


    //free saved enemy
    //free saved grounds pointer & Ground

    free(saved_ground_is_ghost);
    free(saved_ground_color);
    free(saved_ground_type);
    free(saved_ground_text_size);


    free(saved_enemy_type);
    free(saved_enemy_x);
    free(saved_enemy_y);


    free(saved_ground_x1);
    free(saved_ground_y1);
    free(saved_ground_x2);
    free(saved_ground_y2);
    free(saved_ground_x3);
    free(saved_ground_y3);
    for (int i=0;i<GROUND_NUM;i++) {
      free(saved_ground_text[i]);
    }
    free(saved_ground_text);



    for (int i=0;i<GROUND_NUM+MAX_WEB_NUM;i++) {//free objects
      freeGround(Ground[i]);
    }


    for (int i=0;i<MAP_NODE_NUM;i++) {
      freeNode(NodeGrid[i]); //free actual obj
    }


    for (int i=0;i<VGRID_NUM;i++) {
      freeVGrid(VGrid[i]); //free actual obj
    }

    

     for (int i=0;i<PLATFORM_GRID_NUM;i++) {
       freeTileMap(TileMapPlatform[i]);
     }

      for (int i=0;i<FOREGROUND_GRID_NUM;i++) {
        freeTileMap(TileMapForeground[i]);
      }

      for (int i=0;i<SHADOW_GRID_NUM;i++) {
        freeTileMapPaint(TileMapShadow[i]);
      }


    for (int i=0;i<ENEMY_NUM;i++) {
      freeEnemy(Enemy[i]);
    }

    /*for (int i=0;i<ENEMY_NUM;i++) {
      freeEnemySprite(EnemySprite[i]);
    }*/
    //printf("===All objects freed\n");

    free(Ground); //free pointer to pointers
    free(NodeGrid); //free pointer to pointers
    free(VGrid); //free pointer to pointers
    if (SHADOW_GRID_NUM>0) {
      free(TileMapShadow);
    }
    if (PLATFORM_GRID_NUM>0) {
      free(TileMapPlatform);
    }
    if (FOREGROUND_GRID_NUM>0) {
      free(TileMapForeground);
    }
    free(Enemy);

    //free(EnemySprite);
    //printf("===All pointers freed\n");


    /*DeleteObject(map_platforms_sprite_mask);
    DeleteObject(map_platforms_sprite); //delete sprites

    DeleteObject(map_water_platforms_sprite_mask);
    DeleteObject(map_water_platforms_sprite);
    if (has_water) {
      has_water=FALSE;
    }

    DeleteObject(map_platforms_shadow_shader);*/

    DeleteObject(map_background_sprite);
    //LoadMainMenuBackground();

    back_to_menu=FALSE;
    run_after_once=FALSE;
    clean_up_sound=TRUE;
    in_main_menu=TRUE;
    run_once_only=FALSE;
}
