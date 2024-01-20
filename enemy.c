
#define STRAIGHT_COST 10
#define DIAGONAL_COST 14

int CalculateDistanceCost(int enemy_id,int a, int b)
{
  int x_dist=0,y_dist=0,picked=0,remaining=0;
  x_dist=Enemy[enemy_id].node_x[a]-Enemy[enemy_id].node_x[b];
  y_dist=Enemy[enemy_id].node_y[a]-Enemy[enemy_id].node_y[b];
  if (x_dist<0) {
    x_dist*=-1;
  }
  if (y_dist<0) {
    y_dist*=-1;
  }
  remaining=sqrt(pow(x_dist-y_dist,2));
  if (x_dist<y_dist) {
    picked=x_dist;
  } else {
    picked=y_dist;
  }
  return (DIAGONAL_COST * picked + STRAIGHT_COST * remaining)/100;
}

int smallest_f_cost(int enemy_id)
{
  int i=0,saved_i=-1,open_node_id=0,id=Enemy[enemy_id].start_node,smallest=INT_MAX;
  for (i=0;i<Enemy[enemy_id].open_nodes_num;i++) {//open nodes num is the key to the optimization
    open_node_id=Enemy[enemy_id].open_nodes[i];
    if (Enemy[enemy_id].node_fcost[open_node_id]<smallest && Enemy[enemy_id].node_open[open_node_id]) {
      smallest=Enemy[enemy_id].node_fcost[open_node_id];
      id=open_node_id;
      saved_i=i;
    }
  }
  if (saved_i!=-1) {
    for (i=saved_i;i<Enemy[enemy_id].open_nodes_num;i++) {//shifts array left from open_node's pos
      Enemy[enemy_id].open_nodes[i]=Enemy[enemy_id].open_nodes[i+1];
    }
    Enemy[enemy_id].open_nodes_num--;//dynamically scales: open_node becomes closed
  }
  return id;
}

void InitEnemyPathfinding(int enemy_id,double target_x,double target_y)
{
  int i=0,j=0,k=0,j2=0,x=0,y=0,
      start_node_x=0,start_node_y=0,
      node_id=0,
      current_x=Enemy[enemy_id].x,
      current_y=Enemy[enemy_id].y;
 //Init enemy fixed
  Enemy[enemy_id].node_num=Enemy[enemy_id].follow_range*Enemy[enemy_id].follow_range;
  Enemy[enemy_id].sprite_timer=0;
  Enemy[enemy_id].search_timer=0;
  Enemy[enemy_id].idle_timer=0;
  Enemy[enemy_id].idling=FALSE;  
  Enemy[enemy_id].found_target=FALSE;
  Enemy[enemy_id].trace_target=FALSE;
  Enemy[enemy_id].move_to_target=FALSE;
  Enemy[enemy_id].path_nodes_num=0;
  Enemy[enemy_id].open_nodes_num=0;//reset to 0
 //set open path nodes to false
  for (i=0;i<MAX_NODE_NUM/2;i++) {
    Enemy[enemy_id].path_nodes[i]=0;
  }
  Enemy[enemy_id].x=current_x;
  Enemy[enemy_id].y=current_y;
 //Set Enemy Pathfinding Nodes
 //X
  start_node_x=x=
    (Enemy[enemy_id].x/NODE_SIZE*NODE_SIZE)
   -NODE_SIZE*(Enemy[enemy_id].follow_range/2);
 //Y
  start_node_y=y=
    (Enemy[enemy_id].y/NODE_SIZE*NODE_SIZE)
   -NODE_SIZE*(Enemy[enemy_id].follow_range/2); 
  while (x%NODE_SIZE!=0) {
   start_node_x++;
   x++;
  }
  while (y%NODE_SIZE!=0) {
   start_node_y++;
   y++;
  }
  int enemy_species1_solids[MAX_NODE_NUM],
      species1_solid_num=0;
  for (i=0;i<Enemy[enemy_id].node_num;i++) {//maximum path finding node num
    Enemy[enemy_id].node_back[i]=
      Enemy[enemy_id].node_open[i]=
      Enemy[enemy_id].node_closed[i]=FALSE;
    if (i<Enemy[enemy_id].node_num) {
      Enemy[enemy_id].node_x[i]=x;
      Enemy[enemy_id].node_y[i]=y;
      node_id=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM); //all nodes      
      if (node_id!=-1) {
        switch (Enemy[enemy_id].species) {
	  case 0://standard
            Enemy[enemy_id].node_solid[i]=NodeGrid[node_id].node_solid;
	    break;
	  case 1://inverse
            Enemy[enemy_id].node_solid[i]=!NodeGrid[node_id].node_solid;
	    if (!Enemy[enemy_id].node_solid[i]) {
              enemy_species1_solids[species1_solid_num]=i;
	      species1_solid_num++;
            }
	    break;
        }
      } else {//out of bounds
        Enemy[enemy_id].node_solid[i]=TRUE;
      }
    } else {
      Enemy[enemy_id].node_x[i]=start_node_x;
      Enemy[enemy_id].node_y[i]=start_node_y;
    }
    Enemy[enemy_id].node_hcost[i]=0;
    Enemy[enemy_id].node_gcost[i]=INT_MAX;
    Enemy[enemy_id].node_fcost[i]=Enemy[enemy_id].node_gcost[i]+Enemy[enemy_id].node_hcost[i];
    Enemy[enemy_id].node_parent[i]=-1;
    x+=NODE_SIZE;
    if (x>start_node_x-NODE_SIZE+Enemy[enemy_id].follow_range*NODE_SIZE) {
      x=start_node_x;
      y+=NODE_SIZE;
    }
  }
  if (Enemy[enemy_id].species==1) {//species 1
    for (i=0;i<species1_solid_num;i++) {
      k=enemy_species1_solids[i];
      x=Enemy[enemy_id].node_x[k]-Enemy[enemy_id].node_x[0];
      for (j2=0;j2<5;j2++) {
        if (j2<3) {
          x+=((j2-2)*NODE_SIZE);
	} else {
          x+=((j2-1)*NODE_SIZE);
	}
        for (j=0;j<8;j++) {
          y=Enemy[enemy_id].node_y[k]-Enemy[enemy_id].node_y[0];
          y+=((j-3)*NODE_SIZE);
          node_id=GetGridId(x,y,Enemy[enemy_id].follow_range*NODE_SIZE,NODE_SIZE,Enemy[enemy_id].node_num);
          if (node_id!=-1) {
	    Enemy[enemy_id].node_solid[node_id]=FALSE;
	  }
       }
      }
    }
    for (i=0;i<species1_solid_num;i++) {
      k=enemy_species1_solids[i];
      for (j=0;j<2;j++) {
        x=Enemy[enemy_id].node_x[k]-Enemy[enemy_id].node_x[0];
        y=Enemy[enemy_id].node_y[k]-Enemy[enemy_id].node_y[0]+NODE_SIZE*j;
        node_id=GetGridId(x,y,Enemy[enemy_id].follow_range*NODE_SIZE,NODE_SIZE,Enemy[enemy_id].node_num);
        if (node_id!=-1) {
          Enemy[enemy_id].node_solid[node_id]=TRUE;
        }
      }
    } 
  }
  //Set Start Node
  x=current_x-Enemy[enemy_id].node_x[0];
  y=current_y-Enemy[enemy_id].node_y[0];
  Enemy[enemy_id].start_node=GetGridId(x,y,Enemy[enemy_id].follow_range*NODE_SIZE,NODE_SIZE,Enemy[enemy_id].node_num);
  for (i=0;i<MAX_NODE_NUM/2;i++) {//reset open_nodes array
    Enemy[enemy_id].open_nodes[i]=Enemy[enemy_id].start_node;
  }
  //Set Target
  x=target_x-Enemy[enemy_id].node_x[0];
  y=target_y-Enemy[enemy_id].node_y[0];
  Enemy[enemy_id].end_node=GetGridId(x,y,Enemy[enemy_id].follow_range*NODE_SIZE,NODE_SIZE,Enemy[enemy_id].node_num);
  //Initiate start
  Enemy[enemy_id].current_node=Enemy[enemy_id].start_node;
  Enemy[enemy_id].node_gcost[Enemy[enemy_id].start_node]=0;
  Enemy[enemy_id].node_hcost[Enemy[enemy_id].start_node]=CalculateDistanceCost(0,Enemy[enemy_id].start_node,Enemy[enemy_id].end_node);
  Enemy[enemy_id].node_fcost[Enemy[enemy_id].start_node]=Enemy[enemy_id].node_gcost[Enemy[enemy_id].start_node]+Enemy[enemy_id].node_hcost[Enemy[enemy_id].start_node];
  Enemy[enemy_id].node_open[Enemy[enemy_id].start_node]=TRUE;
}

void EnemyPathFinding(int enemy_id)
{
  int x=0,y=0,i=0,neighbour_id=0,TGCost=0;
  bool allow_act=FALSE;
  if (!Enemy[enemy_id].found_target) {//target not found
    Enemy[enemy_id].current_node=smallest_f_cost(enemy_id);
    if (Enemy[enemy_id].node_x[Enemy[enemy_id].current_node]==Enemy[enemy_id].node_x[Enemy[enemy_id].end_node] &&
        Enemy[enemy_id].node_y[Enemy[enemy_id].current_node]==Enemy[enemy_id].node_y[Enemy[enemy_id].end_node]) {
      Enemy[enemy_id].current_node=Enemy[enemy_id].end_node;
      Enemy[enemy_id].found_target=TRUE;
      Enemy[enemy_id].path_nodes[0]=Enemy[enemy_id].current_node;
      Enemy[enemy_id].path_nodes_num=1;
    }
    Enemy[enemy_id].node_open[Enemy[enemy_id].current_node]=FALSE;
    Enemy[enemy_id].node_closed[Enemy[enemy_id].current_node]=TRUE;
   //set neighbours
    for (i=0;i<8;i++) {
      x=Enemy[enemy_id].node_x[Enemy[enemy_id].current_node]-Enemy[enemy_id].node_x[0];
      y=Enemy[enemy_id].node_y[Enemy[enemy_id].current_node]-Enemy[enemy_id].node_y[0];
      if (i<=2) {
	x-=NODE_SIZE; //case 1: left
        switch (i) {
          case 0:
	    y-=NODE_SIZE;
            break;
          case 2:
	    y+=NODE_SIZE;
            break;
        }
      } else if (3<=i<=5) {
        x+=NODE_SIZE; // case 4: right
        switch (i) {
          case 3:
	    y-=NODE_SIZE;
            break;
          case 5:
	    y+=NODE_SIZE;
            break;
        }
      } else {
        switch (i) {
          case 6: //above
            y-=NODE_SIZE;
            break;
          case 7: //below
            y+=NODE_SIZE;
            break;
        }
      }
      if (0<x && x<NODE_SIZE*Enemy[enemy_id].follow_range
          && 0<y && y<NODE_SIZE*Enemy[enemy_id].follow_range) {//node is within range
        Enemy[enemy_id].node_neighbour[i]=GetGridId(x,y,Enemy[enemy_id].follow_range*NODE_SIZE,NODE_SIZE,Enemy[enemy_id].node_num);
      } else { //not within range
        Enemy[enemy_id].node_neighbour[i]=Enemy[enemy_id].start_node;
      }
    }
    for (i=0;i<8;i++) {
      neighbour_id=Enemy[enemy_id].node_neighbour[i];
      allow_act=FALSE;
      if (Enemy[enemy_id].current_node!=Enemy[enemy_id].start_node) {//no going through node corners
	if (Enemy[enemy_id].node_solid[Enemy[enemy_id].node_neighbour[1]] || Enemy[enemy_id].node_solid[Enemy[enemy_id].node_neighbour[4]]) { 
          if (Enemy[enemy_id].node_solid[Enemy[enemy_id].node_neighbour[6]] || Enemy[enemy_id].node_solid[Enemy[enemy_id].node_neighbour[7]]) {
	    allow_act=TRUE;
	  }
	}
      }
      if (Enemy[enemy_id].node_solid[neighbour_id] || allow_act) {//corner or solid
        Enemy[enemy_id].node_closed[neighbour_id]=TRUE;
      }
      if (!Enemy[enemy_id].node_closed[neighbour_id] &&
	  !Enemy[enemy_id].node_solid[neighbour_id]) {
        TGCost=Enemy[enemy_id].node_gcost[Enemy[enemy_id].current_node]+CalculateDistanceCost(enemy_id,Enemy[enemy_id].current_node,neighbour_id);
        if (TGCost<Enemy[enemy_id].node_gcost[neighbour_id]) {
          Enemy[enemy_id].node_parent[neighbour_id]=Enemy[enemy_id].current_node;
          Enemy[enemy_id].node_gcost[neighbour_id]=TGCost;
          Enemy[enemy_id].node_hcost[neighbour_id]=CalculateDistanceCost(enemy_id,neighbour_id,Enemy[enemy_id].end_node);
          Enemy[enemy_id].node_fcost[neighbour_id]=Enemy[enemy_id].node_gcost[neighbour_id]+Enemy[enemy_id].node_hcost[neighbour_id];
          if (!Enemy[enemy_id].node_open[neighbour_id]) {//not open
            Enemy[enemy_id].node_open[neighbour_id]=TRUE; //open it
            Enemy[enemy_id].open_nodes[Enemy[enemy_id].open_nodes_num]=neighbour_id;//enemy_open_nodes[self_open_nodes_num] = neighbor_id
            Enemy[enemy_id].open_nodes_num++;//this is the optimization
	  }
        }
      }
    }
  } else if (!Enemy[enemy_id].trace_target) {//target has been found (Enemy[enemy_id].found_target),trace back
    Enemy[enemy_id].search_timer=0;
    if (Enemy[enemy_id].start_node==Enemy[enemy_id].end_node) {
      Enemy[enemy_id].idling=TRUE;
    } else {
      if (Enemy[enemy_id].node_x[Enemy[enemy_id].current_node]==Enemy[enemy_id].node_x[Enemy[enemy_id].start_node] &&
          Enemy[enemy_id].node_y[Enemy[enemy_id].current_node]==Enemy[enemy_id].node_y[Enemy[enemy_id].start_node]) {
        Enemy[enemy_id].path_nodes_num--;
        Enemy[enemy_id].search_target=FALSE;
        Enemy[enemy_id].trace_target=TRUE;
        Enemy[enemy_id].move_to_target=TRUE;
      } else {
        Enemy[enemy_id].node_back[Enemy[enemy_id].current_node]=TRUE;
        Enemy[enemy_id].path_nodes[Enemy[enemy_id].path_nodes_num]
         =Enemy[enemy_id].current_node
         =Enemy[enemy_id].node_parent[Enemy[enemy_id].current_node];
        Enemy[enemy_id].path_nodes_num++;
      }
    }
  }
}

void EnemySpecies1Gravity(int enemy_id)
{
  int enemy_on_ground_id=0;
//  enemy_on_ground_id=GetOnGroundId(Enemy[enemy_id].x,Enemy[enemy_id].y,30,29,FALSE);    //Get Ground id
  enemy_on_ground_id=GetOnGroundId(Enemy[enemy_id].x,Enemy[enemy_id].y,32,31,FALSE);    //Get Ground id
  if (enemy_on_ground_id==-1) {//not on ground
    Enemy[enemy_id].y+=1; //falling down
    Enemy[enemy_id].in_air_timer=2;
    Enemy[enemy_id].angle=0;
    Enemy[enemy_id].saved_angle=0;
    Enemy[enemy_id].above_ground=
      Enemy[enemy_id].below_ground=FALSE;
      Enemy[enemy_id].flip_sprite=FALSE;
  } else {//on ground
    double ground_entity_angle=GetLineTargetAngle(enemy_on_ground_id,Enemy[enemy_id].x,Enemy[enemy_id].y);
    double height_from_ground=GetLineTargetHeight(enemy_on_ground_id,ground_entity_angle,Enemy[enemy_id].x,Enemy[enemy_id].y);
    Enemy[enemy_id].in_air_timer=0;
    if (height_from_ground>0) {    //species 1 above ground (positive)
      Enemy[enemy_id].angle=Ground[enemy_on_ground_id].angle;
      Enemy[enemy_id].above_ground=TRUE;
      Enemy[enemy_id].below_ground=FALSE;
      Enemy[enemy_id].flip_sprite=FALSE;
    } else {    //species 1 below ground
      Enemy[enemy_id].angle=-Ground[enemy_on_ground_id].angle-M_PI;
      Enemy[enemy_id].above_ground=FALSE;
      Enemy[enemy_id].below_ground=TRUE;
      if(Enemy[enemy_id].last_left) {
        Enemy[enemy_id].flip_sprite=FALSE;
      } else {
        Enemy[enemy_id].flip_sprite=TRUE;
      }
      if (player.print_current_above && Enemy[enemy_id].saw_player) {
      	if (player.y-60<Enemy[enemy_id].y && Enemy[enemy_id].y<player.y-30 && !Enemy[enemy_id].idling) {//enemy above
	      if (!Enemy[enemy_id].ignore_player && RandNum(0,5,Enemy[enemy_id].seed)==1) {
            Enemy[enemy_id].y+=0.25;
	      }
        }
      }
    }
    if (!Enemy[enemy_id].last_left) {
      Enemy[enemy_id].angle*=-1;
    }
  }
}

void EnemyMove(int enemy_id)
{
  int path_node_arr_id=Enemy[enemy_id].path_nodes_num-1,
      path_node_id=Enemy[enemy_id].path_nodes[path_node_arr_id];
  double path_node_center_x=Enemy[enemy_id].node_x[path_node_id]+NODE_SIZE/2,
      path_node_center_y=Enemy[enemy_id].node_y[path_node_id]+NODE_SIZE/2;
  if (Enemy[enemy_id].species==1) {
    Enemy[enemy_id].sprite_timer++;
    if (Enemy[enemy_id].sprite_timer>16) {
      Enemy[enemy_id].sprite_timer=0;
    }
  }
  if (Enemy[enemy_id].x<path_node_center_x) {
    Enemy[enemy_id].last_left=FALSE;
    Enemy[enemy_id].x+=Enemy[enemy_id].speed;
  } else {
    Enemy[enemy_id].last_left=TRUE;
    Enemy[enemy_id].x-=Enemy[enemy_id].speed;  
  }
  switch (Enemy[enemy_id].species) {
    case 0://fly
      if (Enemy[enemy_id].y<path_node_center_y) {
        Enemy[enemy_id].y+=Enemy[enemy_id].speed;
      } else {
        Enemy[enemy_id].y-=Enemy[enemy_id].speed;
      }
      break;
    case 1://mite
      if (Enemy[enemy_id].y>path_node_center_y) {
        Enemy[enemy_id].y-=2;//Go against gravity
      } else {
        Enemy[enemy_id].y+=Enemy[enemy_id].speed;
      }
      break;
  }
  if (path_node_center_y-1<=Enemy[enemy_id].y && Enemy[enemy_id].y<=path_node_center_y+1 &&
      path_node_center_x-1<=Enemy[enemy_id].x && Enemy[enemy_id].x<=path_node_center_x+1) {
    Enemy[enemy_id].path_nodes_num--;
    if (path_node_arr_id<=0) { //all nodes followed
      Enemy[enemy_id].idling=TRUE;
      Enemy[enemy_id].move_to_target=FALSE;
    }
  }
}

void EnemyTargetPlayer(int i)
{
  int target_node=0;
  double target_x=0,target_y=0;
  Enemy[i].idling=FALSE;
  Enemy[i].search_target=TRUE;
  Enemy[i].idle_timer=0;
  Enemy[i].search_timer=0;
  if (player.print_current_above || player.print_current_below) {
    target_x=player.above_x;
    target_y=player.above_y;
  } else {
    target_x=player.x;
    target_y=player.y;
  }
  target_node=GetGridId(target_x-Enemy[i].node_x[0],
			target_y-Enemy[i].node_y[0],
                        Enemy[i].follow_range*NODE_SIZE,
			NODE_SIZE,
			Enemy[i].node_num);
  if (Enemy[i].species==1 &&
      Enemy[i].node_solid[target_node]) {
    target_x=player.above_x2;
    target_y=player.above_y2;
    target_node=GetGridId(target_x-Enemy[i].node_x[0],
			  target_y-Enemy[i].node_y[0],
                          Enemy[i].follow_range*NODE_SIZE,
			  NODE_SIZE,
			  Enemy[i].node_num);      
  }
  if (Enemy[i].node_solid[target_node] ||//Total ignore player (player is hidden)
      Enemy[i].in_unchase_range //dont chase player if its in unchase range
    ) {
    if (Enemy[i].species==0 && Enemy[i].node_solid[target_node]) {
      Enemy[i].ignore_player=TRUE; //dont ignore if player is within chase range
    }
    Enemy[i].target_player=FALSE;
    Enemy[i].idling=FALSE;
    Enemy[i].search_target=TRUE;
    Enemy[i].idle_timer=0;
    target_x=Enemy[i].x+RandNum(-Enemy[i].follow_range/4*NODE_SIZE,abs(Enemy[i].follow_range/4*NODE_SIZE),Enemy[i].seed);
    target_y=Enemy[i].y+RandNum(-Enemy[i].follow_range/4*NODE_SIZE,abs(Enemy[i].follow_range/4*NODE_SIZE),Enemy[i].seed);
    target_node=GetGridId(target_x-Enemy[i].node_x[0],
			target_y-Enemy[i].node_y[0],
                        Enemy[i].follow_range*NODE_SIZE,
			NODE_SIZE,
			Enemy[i].node_num);
  } else {
    Enemy[i].target_player=TRUE;
    Enemy[i].ignore_player=FALSE;
  }
  InitEnemyPathfinding(i,target_x,target_y);
}

void EnemyLOSAct(int i)
{
  int los_on_ground_id=0,j=0;
  double x1=0,x2=0,y1=0,y2=0,bullet_gradient=0,bullet_c=0;
  bool allow_act=FALSE;
  for (j=0;j<10;j++) {//LOS Speed
    if (Enemy[i].LOS_shot) {
      if (Enemy[i].LOS_left) {
        Enemy[i].LOS_x-=cos(Enemy[i].LOS_angle);
        Enemy[i].LOS_y-=sin(Enemy[i].LOS_angle);
      } else {
        Enemy[i].LOS_x+=cos(Enemy[i].LOS_angle);
        Enemy[i].LOS_y+=sin(Enemy[i].LOS_angle);
      }
      los_on_ground_id=GetOnGroundId(Enemy[i].LOS_x,Enemy[i].LOS_y,0.5,0.5,FALSE);
      if (Enemy[i].LOS_target_x-1<=Enemy[i].LOS_x && Enemy[i].LOS_x<=Enemy[i].LOS_target_x+1 &&
          Enemy[i].LOS_target_y-1<=Enemy[i].LOS_y && Enemy[i].LOS_y<=Enemy[i].LOS_target_y+1) {
        Enemy[i].saw_player=TRUE;
        Enemy[i].LOS_shot=FALSE;
      } else {
	allow_act=FALSE;
        if (!Ground[los_on_ground_id].is_ghost) {
	  if (los_on_ground_id<GROUND_NUM && los_on_ground_id!=-1) {
	    allow_act=TRUE;
	  }
        }
	if (IsOutOfBounds(Enemy[i].LOS_x,Enemy[i].LOS_y,5,MAP_WIDTH,MAP_HEIGHT) || allow_act //hit solid
	   ){
          Enemy[i].saw_player=FALSE;
          Enemy[i].target_player=FALSE;
          Enemy[i].LOS_shot=FALSE;
	}
      }
    } else {
      Enemy[i].LOS_x=Enemy[i].x;
      Enemy[i].LOS_y=Enemy[i].y;
      Enemy[i].LOS_target_x=player.x;
      Enemy[i].LOS_target_y=player.y;
      if (player.x<Enemy[i].x) {
        x1=Enemy[i].LOS_target_x;
        x2=Enemy[i].LOS_x;
        y1=Enemy[i].LOS_target_y;
        y2=Enemy[i].LOS_y;
        Enemy[i].LOS_left=TRUE;
      } else {
        x2=Enemy[i].LOS_target_x;
        x1=Enemy[i].LOS_x;
        y2=Enemy[i].LOS_target_y;
        y1=Enemy[i].LOS_y;
        Enemy[i].LOS_left=FALSE;
      }
      bullet_gradient=GetGradient(x1,y1,x2,y2);
      bullet_c=GetGroundC(x1,y1,bullet_gradient);
      Enemy[i].LOS_angle=GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));//cos(angle) = adjacent/hypothenuse
      if (bullet_gradient<=0) {
        Enemy[i].LOS_angle=-Enemy[i].LOS_angle;
      }
      Enemy[i].LOS_shot=TRUE;
    }
  }
}

/*void EnemyKnockbackMove(int i)
{
  bool allow_act=FALSE;
  switch (Enemy[i].species) {
    case 0:
      if (GetOnGroundId(Enemy[i].x,Enemy[i].y,5,4,FALSE)!=-1) {
	allow_act=TRUE;
      }
      break;
    case 1:
      if (GetOnGroundId(Enemy[i].x,Enemy[i].y,10,9,FALSE)!=-1) {
	allow_act=TRUE;
      }
      break;
  }
// ^^ condition
  if (allow_act || IsOutOfBounds(Enemy[i].x,Enemy[i].y,5,MAP_WIDTH,MAP_HEIGHT)) {
    Enemy[i].knockback_timer=0;
  } else {
    if (Enemy[i].knockback_left) {
      Enemy[i].x-=cos(Enemy[i].knockback_angle)*player_knockback_speed;
      Enemy[i].y-=sin(Enemy[i].knockback_angle)*player_knockback_speed;
    } else {
      Enemy[i].x+=cos(Enemy[i].knockback_angle)*player_knockback_speed;
      Enemy[i].y+=sin(Enemy[i].knockback_angle)*player_knockback_speed;      
    }
  }
  if (Enemy[i].knockback_timer<=0) {
    Enemy[i].knockback_angle=0;
    Enemy[i].knockback_left=FALSE;
  }
}*/


void EnemyAct(int i)
{
  int dice=0,
	j=0,
	slash_time=1,
	slash_time_i=0,
	current_grid_id=0,
	target_node=0,
	saved_grid_id=0,
	knock_max=0;
  double target_x=0,target_y=0;
  bool allow_act=FALSE,allow_act_1=FALSE;
  //timebreaker enemy
  //if (Enemy[i].time_breaker_immune/* && !the_bravery_tyrant*/) {
    dice=RandNum(0,Enemy[i].time_breaker_rare,Enemy[i].seed);
    if (dice==1) {
      slash_time=Enemy[i].time_breaker_length;
      /*if (Enemy[i].saw_player) {
        Enemy[i].snd_dur=5;
      }*/
    }
  //}
  //Sound
  /*if (sound_on && Enemy[i].snd_dur>0) {
    if (dice!=1) {
      Enemy[i].snd_dur=PlaySound(Enemy[i].snd_dur,Enemy[i].snd_pitch,Enemy[i].snd_rand);
    } else {
      Enemy[i].snd_dur=PlaySound(Enemy[i].snd_dur,100,20);
    }
  }*/
  //Attack
  for (j=0;j<Enemy[i].bullet_shot_num;j++) {
    BulletAct(Enemy[i].bullet_shot_arr[j]);
  }
  Enemy[i].dist_from_player=GetDistance(player.x,player.y,Enemy[i].x,Enemy[i].y);
  //grid interaction
  current_grid_id=GetGridId(Enemy[i].x,Enemy[i].y,MAP_WIDTH,GRID_SIZE,GRID_NUM);
  saved_grid_id=Enemy[i].saved_grid_id;
  if (current_grid_id!=saved_grid_id) {//current grid isnt the same as previous grid
    //set new (always put at the edge of array)
    Grid[current_grid_id].enemy_occupy[Grid[current_grid_id].enemy_occupy_num]=i;
    Enemy[i].grid_queue[current_grid_id]=Grid[current_grid_id].enemy_occupy_num;
    Grid[current_grid_id].enemy_occupy_num++;
    //set previous grid when moving onto new grid
    if (saved_grid_id!=-1) {
      //For every enemy in the previous grid, the Enemy's grid_queue is subtracted by 1
      for (j=Enemy[i].grid_queue[saved_grid_id]+1;j<Grid[saved_grid_id].enemy_occupy_num;j++) {
	    Enemy[Grid[saved_grid_id].enemy_occupy[j]].grid_queue[saved_grid_id]--;
      }
      //enemy's saved grid queue id until the end
      for (j=Enemy[i].grid_queue[saved_grid_id];j<Grid[saved_grid_id].enemy_occupy_num-1;j++) {
        Grid[saved_grid_id].enemy_occupy[j]=Grid[saved_grid_id].enemy_occupy[j+1];//current pos = next pos
      }
      Grid[saved_grid_id].enemy_occupy[Grid[saved_grid_id].enemy_occupy_num-1]=-1;//last char =-1
      Grid[saved_grid_id].enemy_occupy_num--; //enemy occupying grid --;
      Enemy[i].grid_queue[saved_grid_id]=-1; //current enemy's grid queue at previous grid set to -1
    }
    Enemy[i].saved_grid_id=current_grid_id;//will become previous grid after it moves onto next
  }
  bool deduct_health=FALSE;
  if (Enemy[i].health>0) {
    //Enemy knockback & attacked
    //Player bullet
    /*double dist_from_bullet=GetDistance(Bullet[player_bullet_shot].x,Bullet[player_bullet_shot].y,Enemy[i].x,Enemy[i].y);
    switch (Enemy[i].species) {
      case 0://fly
	if (!Enemy[i].time_breaker_immune) {
	  if (GetDistance(Enemy[i].x,Enemy[i].y,player_claws_x,player_claws_y)<=NODE_SIZE*2) {
	    allow_act=TRUE;
	  }
	} else {
	  if (GetDistance(Enemy[i].x,Enemy[i].y,player_claws_x,player_claws_y)<=NODE_SIZE*4) {
	    allow_act=TRUE;
	  }
	}
	break;
      case 1://crawl
	if (GetDistance(Enemy[i].x,Enemy[i].y,player_claws_x,player_claws_y)<=NODE_SIZE*4) {
	  allow_act=TRUE;
	}
	break;
    }
    //^^ condition
    if (allow_act || dist_from_bullet<=NODE_SIZE+NODE_SIZE/4) {
    //player bullet
      if (dist_from_bullet<=NODE_SIZE+NODE_SIZE/4) {
        switch (Enemy[i].species) {
	  case 0://fly
            deduct_health=TRUE;
            Enemy[i].knockback_timer=player_knockback_strength;
            Enemy[i].knockback_angle=Bullet[player_bullet_shot].angle;
            if (Bullet[player_bullet_shot].left) {
              Enemy[i].knockback_left=TRUE;
            } else {
              Enemy[i].knockback_left=FALSE;
            }
	    break;
	  case 1://crawl
	    deduct_health=TRUE;
            StopBullet(player_bullet_shot,TRUE); //Stop the web
            PlayerPlaceWeb; //Web related
	    web_being_shot=-1;
	    player_bullet_shot=-1;
	    break;
        }
      } else {
	if (!the_bravery_tyrant) {
	  if (Enemy[i].knockback_timer==0) {
	    allow_act=TRUE;
	  }
	} else {
	  if (!Enemy[i].time_breaker_immune) {
	    allow_act=TRUE;
	  } else if (Enemy[i].knockback_timer==0) {
	    allow_act=TRUE;
	  }
	}*/
    /*if (allow_act && Enemy[i].saw_player) {
	  switch (Enemy[i].species) {
	    case 0:
	      if (player_attack_timer>34) {
		    allow_act_1=TRUE;
	      }
	      break;
	    case 1:
	      if (player_attack_timer<=39) {
		    if (player_attack_timer>34) {
		      if (Enemy[i].above_ground && player.print_current_below) {
  		        allow_act_1=TRUE;
		      } else if (Enemy[i].below_ground) {
		        if (player.print_current_above || on_ground_id==-1) {
		          allow_act_1=TRUE;
		        }
		      }
		    }
	      } else {
		    allow_act_1=TRUE;
	      }
	      break;
	  }
    }*/
	// ^^ condition
	/*if (allow_act && allow_act_1) {  //player meelee
	  allow_act=allow_act_1=FALSE;
          deduct_health=TRUE;
          Enemy[i].knockback_timer=player_knockback_strength;
	  if (!uppercut && !rst_up && !rst_down) {//normal
            Enemy[i].knockback_angle=player_angle;
	  } else if (uppercut) {//uppercut
            if (!last_left) {
              Enemy[i].knockback_angle=player_angle-M_PI/2;
	    } else {
              Enemy[i].knockback_angle=player_angle+M_PI/2;
	    }
	  } else { //drag enemy down
            if (!last_left) {//drag enemy to player
              Enemy[i].knockback_angle=player_angle+M_PI/2;
	    } else {
              Enemy[i].knockback_angle=player_angle-M_PI/2;
	    }
          }
          if (player.print_current_above || on_ground_id==-1) {
            if (last_left) {
              Enemy[i].knockback_left=TRUE;
            }
          } else if (player.print_current_below) {
            if (!last_left) {
              Enemy[i].knockback_left=TRUE;
            }
          }
        }
      }*/
      //Deduct health
      /*if (deduct_health) {
      // Player hit combo
	enemy_hit_global_timer=5;
        if (player_hit_cooldown_timer==0 && !the_bravery_tyrant) {
	  combo_timer[0]=PLAYER_COMBO_TIME_LIMIT,
          combo_timer[1]=501;//PLAYER_COMBO_TIME_LIMIT;
	  combo_streak[1]+=player_attack_strength;
	  combo_timer[2]=751;//kill streak
        }
      //
        Enemy[i].health-=player_attack_strength;
        if ((Enemy[i].species==1 && Enemy[i].in_air_timer>0/*GetOnGroundId(Enemy[i].x,Enemy[i].y,10,9,FALSE)==-1*///)) {
        /*  Enemy[i].health-=player_attack_strength*2;
        }
        if (Enemy[i].health<=0) {//killed
          if (sound_on) {
            Enemy[i].snd_dur=Enemy[i].death_snd_dur_max;
            Enemy[i].snd_pitch=Enemy[i].death_snd_pitch;
            Enemy[i].snd_rand=Enemy[i].death_snd_rand;
          }
        //Kill Combo
          if (player_hit_cooldown_timer==0 && !the_bravery_tyrant) {
	    combo_timer[2]=751;//killstreak
	    combo_streak[2]++;
	  }
	//
          enemy_kills++;
          souls_taken++;
        } else {
          Enemy[i].snd_dur=Enemy[i].snd_dur_max;
        }
      }
    }
    if (Enemy[i].knockback_timer>0) {
      if (!the_bravery_tyrant || Enemy[i].time_breaker_immune) {
        Enemy[i].knockback_timer--;
        if (Enemy[i].knockback_timer>20) {
          knock_max=player_knockback_speed_multiplier*2;
        } else {
          knock_max=player_knockback_speed_multiplier;
        }
        for (j=0;j<knock_max;j++) {
          EnemyKnockbackMove(i);
        }
      }
    }*/
    EnemyLOSAct(i);//Shoot line of sight bullet
    //if (/*!the_bravery_tyrant ||*/ Enemy[i].time_breaker_immune) {
      for (slash_time_i=0;slash_time_i<slash_time;slash_time_i++) {
        //Prevent reaching border
        if (Enemy[i].x<NODE_SIZE*2) {
          Enemy[i].x++;
        }
        if (Enemy[i].y<NODE_SIZE*2) {
          Enemy[i].y++;
        }
        if (Enemy[i].x>MAP_WIDTH-NODE_SIZE*2) {
          Enemy[i].x--;
        }
        if (Enemy[i].y>MAP_HEIGHT-NODE_SIZE*2) {
          Enemy[i].y--;
        }

        if (Enemy[i].species==1) {//Species 1 gravity
          EnemySpecies1Gravity(i);
          if (Enemy[i].in_air_timer>0) {
            Enemy[i].in_air_timer--;
          }
        }
        int above_player_node1=GetGridId(
                player.above_x-Enemy[i].node_x[0],
                player.above_y-Enemy[i].node_y[0],
                Enemy[i].follow_range*NODE_SIZE,
                NODE_SIZE,Enemy[i].node_num);
        //Crawler
        if ((!Enemy[i].ignore_player &&  //not ignore & within range
             Enemy[i].dist_from_player<Enemy[i].follow_range/2*NODE_SIZE
            )
           ) {
          if (Enemy[i].bullet_fire_cooldown<=0) {
	    if (Enemy[i].bullet_length==0) {
	      for (j=0;j<Enemy[i].bullet_fire_at_once_max;j++) {//shoot several bullets at once
	        Enemy[i].bullet_head_x[j]=player.x+RandNum(-Enemy[i].aim_rand,Enemy[i].aim_rand,Enemy[i].seed);
	        Enemy[i].bullet_head_y[j]=player.y+RandNum(-Enemy[i].aim_rand,Enemy[i].aim_rand,Enemy[i].seed);
	      }
	    }

        if (Enemy[i].bullet_cooldown<=0) {
	      if (Enemy[i].bullet_shot_num<500) {//shot less than 500 bullets
	        for (j=0;j<Enemy[i].bullet_fire_at_once_max;j++) {//several bullets at once
		  allow_act=FALSE;
		  if (Enemy[i].saw_player) {
		    switch (Enemy[i].species) {
		      case 0:
			if (!Enemy[i].node_solid[above_player_node1] && !player.hiding) {
			  allow_act=TRUE;
			}
			break;
		      case 1:
			if (Enemy[i].in_air_timer==0 || slash_time>1) {
			  allow_act=TRUE;
			}
			break;
		    }
		  }
		  if (allow_act) {
                    if (Enemy[i].dist_from_player<Enemy[i].shoot_at_player_range/2*NODE_SIZE) {
		      Enemy[i].shoot_target_x=Enemy[i].bullet_head_x[j];
		      Enemy[i].shoot_target_y=Enemy[i].bullet_head_y[j];
                      ShootBullet(current_bullet_id,
		        Enemy[i].bullet_shot_num,
		        Enemy[i].bullet_color,
		        Enemy[i].bullet_graphics_type,
		        Enemy[i].bullet_range,
		        Enemy[i].bullet_speed,
		        Enemy[i].bullet_speed_multiplier,
		        Enemy[i].bullet_damage,
		        i,
		        Enemy[i].x,
		        Enemy[i].y,
		        Enemy[i].x,
		        Enemy[i].y,
		        Enemy[i].shoot_target_x,
		        Enemy[i].shoot_target_y);
                      Enemy[i].bullet_shot_arr[Enemy[i].bullet_shot_num]=current_bullet_id;
                      Enemy[i].bullet_shot_num++;
                  //after shooting
                      current_bullet_id++;
                      if (current_bullet_id>=BULLET_NUM-1) {
                        current_bullet_id=0;
                      }
                    }
	          }
	        } //end of for
	      }
	          Enemy[i].bullet_cooldown=Enemy[i].bullet_cooldown_max;
	          Enemy[i].bullet_length++;
	          if (Enemy[i].bullet_length>=Enemy[i].bullet_length_max) {
	            Enemy[i].bullet_fire_cooldown=Enemy[i].bullet_fire_cooldown_max;
	            Enemy[i].bullet_length=0;
	          }
	        } else {
              Enemy[i].bullet_cooldown--;
            }
          } else {
	        Enemy[i].bullet_fire_cooldown--;
          }
        }
      //pathfinding
        Enemy[i].forgor_timer++;
        if (Enemy[i].forgor_timer>300) {
          Enemy[i].forgor_timer=0;
          Enemy[i].ignore_player=FALSE;
        }
      //Movement
        if (Enemy[i].idling) { //idling
          Enemy[i].idle_timer++;
    	  allow_act=FALSE;
          if (Enemy[i].saw_player && !Enemy[i].ignore_player) {//not ignoring
	        if (!Enemy[i].target_player && //not targetting
                Enemy[i].dist_from_player/NODE_SIZE<Enemy[i].follow_range/2-2) {
	          allow_act=TRUE;
	        }
          }
          //^^ condition
          if (allow_act) {
            EnemyTargetPlayer(i); //target player
          } else if (Enemy[i].idle_timer>100) {//idling over
            Enemy[i].target_player=FALSE;
            dice=RandNum(0,5,i);
            Enemy[i].idle_timer=0;
            if (dice==1) { //Start searching
	    //total ignore player (still hostile)
              target_x=Enemy[i].x+RandNum(-Enemy[i].follow_range/4*NODE_SIZE,abs(Enemy[i].follow_range/4*NODE_SIZE),Enemy[i].seed);
              target_y=Enemy[i].y+RandNum(-Enemy[i].follow_range/4*NODE_SIZE,abs(Enemy[i].follow_range/4*NODE_SIZE),Enemy[i].seed);
              target_node=GetGridId(target_x-Enemy[i].node_x[0],
			                        target_y-Enemy[i].node_y[0],
                                    Enemy[i].follow_range*NODE_SIZE,
			                        NODE_SIZE,
			                        Enemy[i].node_num);
	          if (!Enemy[i].node_solid[target_node]) {
                Enemy[i].idling=FALSE;
                Enemy[i].search_target=TRUE;
                InitEnemyPathfinding(i,target_x,target_y);	    
	          } else {
                Enemy[i].idling=TRUE;
	          }
            } else { //not searching
              Enemy[i].idling=TRUE;
            }
          }
        } else if (Enemy[i].search_target) {
          //Enemy[i].msprite_hold_timer_max=0;
          EnemyPathFinding(i);
          Enemy[i].search_timer++;
          if (Enemy[i].search_timer>500) {//stop searching
            if (Enemy[i].dist_from_player/NODE_SIZE<Enemy[i].follow_range/2-2) { //within distance
              Enemy[i].ignore_player=TRUE;
            }
            Enemy[i].idling=TRUE;
            Enemy[i].search_target=FALSE;
            Enemy[i].search_timer=0;
          }
        } else if (Enemy[i].move_to_target) {
          for (j=0;j<Enemy[i].speed_multiplier;j++) {
            EnemyMove(i);
          }
          if (!Enemy[i].ignore_player && Enemy[i].saw_player) { //chasing player
            if (Enemy[i].species==0 &&
                NodeGrid[GetGridId(Enemy[i].x,Enemy[i].y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)].node_solid)
            {   //become blind when inside a solid
	          Enemy[i].saw_player=FALSE;
              Enemy[i].idling=TRUE;
              Enemy[i].move_to_target=FALSE;
            }
            if (!Enemy[i].idling) {
              if (player.x<Enemy[i].x) {
                Enemy[i].player_at_left=TRUE;
              } else if (Enemy[i].x<=player.x) {
                Enemy[i].player_at_right=TRUE;
              }
              if (Enemy[i].player_at_left && Enemy[i].player_at_right) {//Change of X
                EnemyTargetPlayer(i);
                Enemy[i].player_at_left=FALSE;
                Enemy[i].player_at_right=FALSE;
              }
              if (player.y<Enemy[i].y) {//Change of Y
                Enemy[i].player_at_above=TRUE;
              } else if (Enemy[i].y<=player.y) {
                Enemy[i].player_at_below=TRUE;
              }
              if (Enemy[i].player_at_above && Enemy[i].player_at_below) {
                EnemyTargetPlayer(i);
                Enemy[i].player_at_above=FALSE;
                Enemy[i].player_at_below=FALSE;
              }
	      if (Enemy[i].chase_range/2*NODE_SIZE<Enemy[i].dist_from_player && Enemy[i].dist_from_player<Enemy[i].unchase_range/2*NODE_SIZE //in unchase range
      	         ) //not in chase range) 
	      {
	        Enemy[i].in_unchase_range=TRUE;
	      } else {
	        Enemy[i].in_chase_range=TRUE;
              }
	      if (Enemy[i].in_unchase_range && Enemy[i].in_chase_range) {
                EnemyTargetPlayer(i);
	        Enemy[i].in_chase_range=FALSE;
	        Enemy[i].in_unchase_range=FALSE;
              }
            }
          }
        }
      //}//end of slash_time
  //other
      /*if (fade_enemy) {
        int k=0;
	    allow_act=FALSE;
        Enemy[i].msprite_hold_timer++;
        if (Enemy[i].msprite_hold_timer>Enemy[i].msprite_hold_timer_max) {
          if (Enemy[i].speed_multiplier>=2) {//fast
            if (Enemy[i].move_to_target) {
	      switch (Enemy[i].species) {
		case 0:
	      	  Enemy[i].msprite_hold_timer_max=2;
		  break;
		case 1:
	      	  Enemy[i].msprite_hold_timer_max=3;
		  break;
	      }
            } else {
	      Enemy[i].msprite_hold_timer_max=1;
            }
          }*/
	  /*if (Enemy[i].saw_player) {
	    if (Enemy[i].speed_multiplier>=2 && Enemy[i].move_to_target) {
	      allow_act=TRUE;
	    } else if (YesLongFade2 || player_in_air_cooldown>0) {
	      allow_act=TRUE;
	    }
	  }*/
          /*if (allow_act) {
            Enemy[i].appear_timer[Enemy[i].current_sm]=50;
          }
          Enemy[i].msprite_x[Enemy[i].current_sm]=Enemy[i].sprite_x;
          Enemy[i].msprite_y[Enemy[i].current_sm]=Enemy[i].sprite_y;
          Enemy[i].msprite_timer[Enemy[i].current_sm]=Enemy[i].sprite_timer;
          Enemy[i].msprite_last_left[Enemy[i].current_sm]=Enemy[i].last_left;
          Enemy[i].msprite_angle[Enemy[i].current_sm]=Enemy[i].angle;
          Enemy[i].msprite_in_air_timer[Enemy[i].current_sm]=Enemy[i].in_air_timer;
        }
        for (k=0;k<MULTI_SPRITE_NUM;k++) {
          if (Enemy[i].appear_timer[k]>0 && !the_bravery_tyrant) {
            Enemy[i].appear_timer[k]--;
          }
        }
      }*/
      if (Enemy[i].species==0) {
        Enemy[i].sprite_timer++;
        if (Enemy[i].sprite_timer>3) {
          Enemy[i].sprite_timer=0;
        }
      }

    }//end of tbt
  }//end of health
  /*if (fade_enemy) {//fade enemy
    if (Enemy[i].msprite_hold_timer>Enemy[i].msprite_hold_timer_max) {
      Enemy[i].msprite_hold_timer=0;
      Enemy[i].current_sm++;
      if (Enemy[i].current_sm>=MULTI_SPRITE_NUM-1) {
        Enemy[i].current_sm=0;
      }
    }
  }*/
  Enemy[i].sprite_x=Enemy[i].x+player.cam_x+player.cam_move_x;
  Enemy[i].sprite_y=Enemy[i].y+player.cam_y+player.cam_move_y;

}

void SetEnemyByType(int i,int type)
{
  Enemy[i].angle=0;
  Enemy[i].type=type;
  Enemy[i].species=saved_enemy_type_species[type];
  Enemy[i].max_health=Enemy[i].health=saved_enemy_type_health[type];
  Enemy[i].follow_range=saved_enemy_type_follow_range[type];
  Enemy[i].chase_range=saved_enemy_type_chase_range[type];
  Enemy[i].unchase_range=saved_enemy_type_unchase_range[type];
  if (Enemy[i].unchase_range>Enemy[i].follow_range ||
      Enemy[i].chase_range>Enemy[i].unchase_range) {
    Enemy[i].unchase_range=0;
    Enemy[i].chase_range=0;
  }
  Enemy[i].color=color_arr[saved_enemy_type_color[type]];
  Enemy[i].speed=saved_enemy_type_speed[type];
  Enemy[i].speed_multiplier=saved_enemy_type_speed_multiplier[type];
//bool
  Enemy[i].time_breaker_immune=saved_enemy_type_time_breaker_immune[type];
//shoot
  Enemy[i].bullet_fire_cooldown_max=
  Enemy[i].bullet_fire_cooldown=saved_enemy_type_bullet_fire_cooldown[type];
  Enemy[i].bullet_fire_at_once_max=saved_enemy_type_bullet_fire_at_once[type];
  Enemy[i].bullet_length_max=saved_enemy_type_bullet_length[type];
  Enemy[i].bullet_length=0;
  Enemy[i].shoot_at_player_range=saved_enemy_type_shoot_at_player_range[type];
  Enemy[i].aim_rand=saved_enemy_type_aim_rand[type];
//bullet
  Enemy[i].bullet_damage=saved_enemy_type_bullet_damage[type],
  Enemy[i].bullet_cooldown_max=Enemy[i].bullet_cooldown=saved_enemy_type_bullet_cooldown[type];
  Enemy[i].bullet_speed=saved_enemy_type_bullet_speed[type];
  Enemy[i].bullet_speed_multiplier=saved_enemy_type_bullet_speed_multiplier[type];
  Enemy[i].bullet_range=saved_enemy_type_bullet_range[type];
  Enemy[i].bullet_color=color_arr[saved_enemy_type_bullet_color[type]];
  Enemy[i].bullet_graphics_type=saved_enemy_type_bullet_graphics_type[type];
  //eney snd
  //Enemy[i].snd_dur_max=saved_enemy_type_snd_dur[type];
  //Enemy[i].snd_pitch=saved_enemy_type_snd_pitch[type];
  //Enemy[i].snd_rand=saved_enemy_type_snd_rand[type];
  //enemy death snd
  //Enemy[i].death_snd_dur_max=saved_enemy_type_death_snd_dur[type];
  //Enemy[i].death_snd_pitch=saved_enemy_type_death_snd_pitch[type];
  //Enemy[i].death_snd_rand=saved_enemy_type_death_snd_rand[type];
  //time breaker
  Enemy[i].time_breaker_rare=saved_enemy_type_time_breaker_rare[type];
  Enemy[i].time_breaker_length=saved_enemy_type_time_breaker_length[type];
  //misc
  //Enemy[i].msprite_hold_timer_max=1;
}

void CleanUpEnemy()
{
  for (int i=0;i<ENEMY_NUM;i++) {
    DeleteObject(Enemy[i].sprite_1);
    DeleteObject(Enemy[i].sprite_2);
    DeleteObject(Enemy[i].sprite_3);
  }
}

void InitEnemy()
{
  int i=0,j=0,x=0,y=0;
  //CleanUpEnemy();
  for (i=0;i<ENEMY_NUM;i++) {
    Enemy[i].seed=0;
    Enemy[i].dist_from_player=999;
    Enemy[i].x=saved_enemy_x[i];
    Enemy[i].y=saved_enemy_y[i];
    Enemy[i].sprite_x=
    Enemy[i].sprite_y=-20;
    /*Enemy[i].current_sm=0;
    Enemy[i].msprite_hold_timer=0;
    for (j=0;j<MULTI_SPRITE_NUM;j++) {
      Enemy[i].msprite_x[j]=
      Enemy[i].msprite_y[j]=-20;
      Enemy[i].appear_timer[j]=0;
      Enemy[i].msprite_timer[j]=0;
      Enemy[i].msprite_last_left[j]=FALSE;
      Enemy[i].msprite_angle[j]=0;
      Enemy[i].msprite_in_air_timer[j]=0;
    }*/
    SetEnemyByType(i,saved_enemy_type[i]);
    if (Enemy[i].x<5) {
      Enemy[i].x=25;
    }
    if (Enemy[i].y<5) {
      Enemy[i].y=25;
    }
    Enemy[i].above_ground=
    Enemy[i].below_ground=FALSE;
    Enemy[i].in_air_timer=0;
    Enemy[i].angle=0;
    Enemy[i].sprite_angle=0;
    Enemy[i].saved_angle=0;
  //bullet
    Enemy[i].bullet_shot_num=0;
    for (j=0;j<BULLET_NUM*1;j++) {
      Enemy[i].bullet_shot_arr[j]=-1;
    }
    for (j=0;j<MAX_BULLET_PER_FIRE;j++) {
      Enemy[i].bullet_head_x[j]=0;
      Enemy[i].bullet_head_y[j]=0;
    }
    //Enemy[i].knockback_left=FALSE;
    //Enemy[i].knockback_angle=0;
    //Enemy[i].knockback_timer=0;
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
    Enemy[i].saved_grid_id=-1;
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
    x=Enemy[i].x-Enemy[i].node_x[0];
    y=Enemy[i].y-Enemy[i].node_y[0];
    Enemy[i].start_node=GetGridId(x,y,Enemy[i].follow_range*NODE_SIZE,NODE_SIZE,Enemy[i].node_num);
    Enemy[i].end_node=GetGridId(x,y,Enemy[i].follow_range*NODE_SIZE,NODE_SIZE,Enemy[i].node_num);
    Enemy[i].open_nodes_num=0;
    for (j=0;j<MAX_NODE_NUM/2;j++) {
      Enemy[i].open_nodes[j]=Enemy[i].start_node;
    }
    for (j=0;j<GRID_NUM;j++) {
      Enemy[i].grid_queue[j]=-1;
    }
    EnemyAct(i);
  }
}


void DrawEnemy(HDC hdc) {
  int i=0,j=0,k=0;
  for (j=0;j<player.rendered_enemy_num;j++) {  
    i=player.render_enemies[j];
    if (Enemy[i].health>0) {
      for (k=0;k<Enemy[i].bullet_shot_num;k++) {
        DrawBullet(hdc,Enemy[i].bullet_shot_arr[k]);
      }
    }

    if (Enemy[i].species==1) {
      if (Enemy[i].angle!=Enemy[i].saved_angle) {
        DeleteObject(Enemy[i].sprite_1);
        DeleteObject(Enemy[i].sprite_2);
        //DeleteObject(Enemy[i].sprite_3);
        
        Enemy[i].sprite_1=RotateSprite(hdc, enemy2_sprite_1,Enemy[i].angle,LTGREEN,Enemy[i].color);
        Enemy[i].sprite_2=RotateSprite(hdc, enemy2_sprite_2,Enemy[i].angle,LTGREEN,Enemy[i].color);
        //Enemy[i].sprite_3=RotateSprite(hdc, enemy2_sprite_3,Enemy[i].angle,LTGREEN,Enemy[i].color);
        Enemy[i].saved_angle=Enemy[i].angle;
      }
    }

    if (Enemy[i].saw_player) {
      if (Enemy[i].in_air_timer==0) {
        switch (Enemy[i].species) {
          case 0:
            if (Enemy[i].sprite_timer%2==0) {
              GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_1,Enemy[i].last_left);
            } else {
              GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_2,Enemy[i].last_left);
            }
            break;
          case 1:
            if (Enemy[i].above_ground) {
              if (Enemy[i].sprite_timer%8==0) {
                GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_1,Enemy[i].last_left);
              } else {
                GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_2,Enemy[i].last_left);
              }
            } else if (Enemy[i].below_ground) {
              if (Enemy[i].sprite_timer%8==0) {
                GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_1,Enemy[i].flip_sprite);
              } else {
                GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_2,Enemy[i].flip_sprite);
              }
            }
            break;
        }
      } else {
        switch (Enemy[i].species) {
          case 1: //placeholder sprites
            GrSprite(hdc,Enemy[i].sprite_x,Enemy[i].sprite_y,Enemy[i].sprite_3,Enemy[i].last_left);
            break;
        }
      }
    }
  }
}

