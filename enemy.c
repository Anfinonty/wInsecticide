
#define STRAIGHT_COST 10
#define DIAGONAL_COST 14

int CalculateDistanceCost(int enemy_id,int a, int b)
{
  int x_dist=0,y_dist=0,picked=0,remaining=0;
  x_dist=Enemy[enemy_id]->node_x[a]-Enemy[enemy_id]->node_x[b];
  y_dist=Enemy[enemy_id]->node_y[a]-Enemy[enemy_id]->node_y[b];
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
  int i=0,saved_i=-1,open_node_id=0,id=Enemy[enemy_id]->start_node,smallest=INT_MAX;
  for (i=0;i<Enemy[enemy_id]->open_nodes_num;i++) {//open nodes num is the key to the optimization
    open_node_id=Enemy[enemy_id]->open_nodes[i];
    if (Enemy[enemy_id]->node_fcost[open_node_id]<smallest && Enemy[enemy_id]->node_open[open_node_id]) {
      smallest=Enemy[enemy_id]->node_fcost[open_node_id];
      id=open_node_id;
      saved_i=i;
    }
  }
  if (saved_i!=-1) {
    for (i=saved_i;i<Enemy[enemy_id]->open_nodes_num;i++) {//shifts array left from open_node's pos
      Enemy[enemy_id]->open_nodes[i]=Enemy[enemy_id]->open_nodes[i+1];
    }
    Enemy[enemy_id]->open_nodes_num--;//dynamically scales: open_node becomes closed
  }
  return id;
}

void InitEnemyPathfinding(int enemy_id,double target_x,double target_y)
{
  int i=0,j=0,k=0,j2=0,x=0,y=0,
      start_node_x=0,start_node_y=0,
      node_id=0,
      current_x=Enemy[enemy_id]->x,
      current_y=Enemy[enemy_id]->y;
 //Init enemy fixed
  Enemy[enemy_id]->node_num=MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE;//Enemy[enemy_id]->follow_range*Enemy[enemy_id]->follow_range;
  Enemy[enemy_id]->sprite_timer=0;
  Enemy[enemy_id]->search_timer=0;
  Enemy[enemy_id]->idle_timer=0;
  Enemy[enemy_id]->idling=FALSE;  
  Enemy[enemy_id]->found_target=FALSE;
  Enemy[enemy_id]->trace_target=FALSE;
  Enemy[enemy_id]->move_to_target=FALSE;
  Enemy[enemy_id]->path_nodes_num=0;
  Enemy[enemy_id]->open_nodes_num=0;//reset to 0
 //set open path nodes to false
  for (i=0;i<MAX_NODE_NUM;i++) {
    Enemy[enemy_id]->path_nodes[i]=0;
  }
  Enemy[enemy_id]->x=current_x;
  Enemy[enemy_id]->y=current_y;
 //Set Enemy Pathfinding Nodes
 //X
  start_node_x=x=
    (Enemy[enemy_id]->x/NODE_SIZE*NODE_SIZE)
   -NODE_SIZE*(MAX_FOLLOW_RANGE/2);
 //Y
  start_node_y=y=
    (Enemy[enemy_id]->y/NODE_SIZE*NODE_SIZE)
   -NODE_SIZE*(MAX_FOLLOW_RANGE/2); 
  while (x%NODE_SIZE!=0) {
   start_node_x++;
   x++;
  }
  while (y%NODE_SIZE!=0) {
   start_node_y++;
   y++;
  }
  Enemy[enemy_id]->species1_solid_num=0;
  for (i=0;i<Enemy[enemy_id]->node_num;i++) {//maximum path finding node num
    Enemy[enemy_id]->node_back[i]=
      Enemy[enemy_id]->node_open[i]=
      Enemy[enemy_id]->node_closed[i]=FALSE;
    if (i<Enemy[enemy_id]->node_num) {
      Enemy[enemy_id]->node_x[i]=x;
      Enemy[enemy_id]->node_y[i]=y;
      node_id=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM); //all nodes      
      if (node_id!=-1) {
        switch (Enemy[enemy_id]->species) {
	      case 0://standard
          case 2:
          case 4:
            Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_solid;
            if (!Enemy[enemy_id]->node_solid[i])
              Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_water;
	        break;
	      case 1://inverse
            if (Enemy[enemy_id]->target_player || Enemy[enemy_id]->on_ground_id==-1 /*|| Enemy[enemy_id]->is_in_ground_edge*/) {
              Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_solid;
              if (!Enemy[enemy_id]->node_solid[i]) //cannot dive into water
                Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_water;
            } else {
              Enemy[enemy_id]->node_solid[i]=!NodeGrid[node_id]->node_solid;
              if (!Enemy[enemy_id]->node_solid[i])
                Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_water;
	          if (!Enemy[enemy_id]->node_solid[i]) {
                Enemy[enemy_id]->enemy_species1_solids[Enemy[enemy_id]->species1_solid_num]=i;
	            Enemy[enemy_id]->species1_solid_num++;
              }
            }
            break;
          case 3:
            /*if (!Enemy[enemy_id]->target_player || (Enemy[enemy_id]->on_ground_id!=-1 && !Enemy[enemy_id]->is_in_ground_edge)) {
              Enemy[enemy_id]->node_solid[i]=!NodeGrid[node_id]->node_solid;
	          if (!Enemy[enemy_id]->node_solid[i]) {
                Enemy[enemy_id]->enemy_species1_solids[Enemy[enemy_id]->species1_solid_num]=i;
	            Enemy[enemy_id]->species1_solid_num++;
              }
            } else{
              Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_solid;
            }*/
            if (Enemy[enemy_id]->target_player || Enemy[enemy_id]->on_ground_id==-1 /*|| Enemy[enemy_id]->is_in_ground_edge*/) {
              Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_solid;
            } else {
              Enemy[enemy_id]->node_solid[i]=!NodeGrid[node_id]->node_solid;
	          if (!Enemy[enemy_id]->node_solid[i]) {
                Enemy[enemy_id]->enemy_species1_solids[Enemy[enemy_id]->species1_solid_num]=i;
	            Enemy[enemy_id]->species1_solid_num++;
              }
            }
	        break;
            }
      } else {//out of bounds
        Enemy[enemy_id]->node_solid[i]=TRUE;
      }
    } else {
      Enemy[enemy_id]->node_x[i]=start_node_x;
      Enemy[enemy_id]->node_y[i]=start_node_y;
    }
    Enemy[enemy_id]->node_hcost[i]=0;
    Enemy[enemy_id]->node_gcost[i]=INT_MAX;
    Enemy[enemy_id]->node_fcost[i]=Enemy[enemy_id]->node_gcost[i]+Enemy[enemy_id]->node_hcost[i];
    Enemy[enemy_id]->node_parent[i]=-1;
    x+=NODE_SIZE;
    if (x>start_node_x-NODE_SIZE+MAX_FOLLOW_RANGE*NODE_SIZE) {
      x=start_node_x;
      y+=NODE_SIZE;
    }
  }
  if (Enemy[enemy_id]->species==1 || Enemy[enemy_id]->species==3) {//species 1
    for (i=0;i<Enemy[enemy_id]->species1_solid_num;i++) {
      k=Enemy[enemy_id]->enemy_species1_solids[i];
      x=Enemy[enemy_id]->node_x[k]-Enemy[enemy_id]->node_x[0];
      for (j2=0;j2<5;j2++) {
        if (j2<3) {
          x+=((j2-2)*NODE_SIZE);
    	} else {
          x+=((j2-1)*NODE_SIZE);
    	}
        for (j=0;j<8;j++) {
          y=Enemy[enemy_id]->node_y[k]-Enemy[enemy_id]->node_y[0];
          y+=((j-3)*NODE_SIZE);
          node_id=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[enemy_id]->node_num);
          if (node_id!=-1) {
	        Enemy[enemy_id]->node_solid[node_id]=FALSE;
	      }
        }
      }
    }
    for (i=0;i<Enemy[enemy_id]->species1_solid_num;i++) {
      k=Enemy[enemy_id]->enemy_species1_solids[i];
      for (j=0;j<2;j++) {
        x=Enemy[enemy_id]->node_x[k]-Enemy[enemy_id]->node_x[0];
        y=Enemy[enemy_id]->node_y[k]-Enemy[enemy_id]->node_y[0]+NODE_SIZE*j;
        node_id=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[enemy_id]->node_num);
        if (node_id!=-1) {
          Enemy[enemy_id]->node_solid[node_id]=TRUE;
        }
      }
    } 
  }
  //Set Start Node
  x=current_x-Enemy[enemy_id]->node_x[0];
  y=current_y-Enemy[enemy_id]->node_y[0];
  Enemy[enemy_id]->start_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[enemy_id]->node_num);
  for (i=0;i<MAX_NODE_NUM;i++) {//reset open_nodes array
    Enemy[enemy_id]->open_nodes[i]=Enemy[enemy_id]->start_node;
  }
  //Set Target
  x=target_x-Enemy[enemy_id]->node_x[0];
  y=target_y-Enemy[enemy_id]->node_y[0];
  Enemy[enemy_id]->end_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[enemy_id]->node_num);
  //Initiate start
  Enemy[enemy_id]->current_node=Enemy[enemy_id]->start_node;
  Enemy[enemy_id]->node_gcost[Enemy[enemy_id]->start_node]=0;
  Enemy[enemy_id]->node_hcost[Enemy[enemy_id]->start_node]=CalculateDistanceCost(0,Enemy[enemy_id]->start_node,Enemy[enemy_id]->end_node);
  Enemy[enemy_id]->node_fcost[Enemy[enemy_id]->start_node]=Enemy[enemy_id]->node_gcost[Enemy[enemy_id]->start_node]+Enemy[enemy_id]->node_hcost[Enemy[enemy_id]->start_node];
  Enemy[enemy_id]->node_open[Enemy[enemy_id]->start_node]=TRUE;
}

void EnemyPathFinding(int enemy_id)
{
  int x=0,y=0,i=0,neighbour_id=0,TGCost=0;
  bool allow_act=FALSE;
  if (!Enemy[enemy_id]->found_target) {//target not found
    Enemy[enemy_id]->current_node=smallest_f_cost(enemy_id);
    if (Enemy[enemy_id]->node_x[Enemy[enemy_id]->current_node]==Enemy[enemy_id]->node_x[Enemy[enemy_id]->end_node] &&
        Enemy[enemy_id]->node_y[Enemy[enemy_id]->current_node]==Enemy[enemy_id]->node_y[Enemy[enemy_id]->end_node]) {
      Enemy[enemy_id]->current_node=Enemy[enemy_id]->end_node;
      Enemy[enemy_id]->found_target=TRUE;
      Enemy[enemy_id]->path_nodes[0]=Enemy[enemy_id]->current_node;
      Enemy[enemy_id]->path_nodes_num=1;
    }
    Enemy[enemy_id]->node_open[Enemy[enemy_id]->current_node]=FALSE;
    Enemy[enemy_id]->node_closed[Enemy[enemy_id]->current_node]=TRUE;
   //set neighbours
    for (i=0;i<8;i++) {
      x=Enemy[enemy_id]->node_x[Enemy[enemy_id]->current_node]-Enemy[enemy_id]->node_x[0];
      y=Enemy[enemy_id]->node_y[Enemy[enemy_id]->current_node]-Enemy[enemy_id]->node_y[0];
      if (i<=2) {
    	x-=NODE_SIZE; //case 1: left
        switch (i) {
          case 0:y-=NODE_SIZE;break;
          case 2:y+=NODE_SIZE;break;
        }
      } else if (3<=i && i<=5) {
        x+=NODE_SIZE; // case 4: right
        switch (i) {
          case 3:y-=NODE_SIZE;break;
          case 5:y+=NODE_SIZE;break;
        }
      } else {
        switch (i) {
          case 6:y-=NODE_SIZE;break;//above
          case 7:y+=NODE_SIZE;break;//below
        }
      }
      if (0<x && x<NODE_SIZE*MAX_FOLLOW_RANGE
          && 0<y && y<NODE_SIZE*MAX_FOLLOW_RANGE) {//node is within range
        Enemy[enemy_id]->node_neighbour[i]=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[enemy_id]->node_num);
      } else { //not within range
        Enemy[enemy_id]->node_neighbour[i]=Enemy[enemy_id]->start_node;
      }
    }
    for (i=0;i<8;i++) {
      neighbour_id=Enemy[enemy_id]->node_neighbour[i];
      allow_act=FALSE;
      if (Enemy[enemy_id]->current_node!=Enemy[enemy_id]->start_node) {//no going through node corners
	if (Enemy[enemy_id]->node_solid[Enemy[enemy_id]->node_neighbour[1]] || Enemy[enemy_id]->node_solid[Enemy[enemy_id]->node_neighbour[4]]) { 
          if (Enemy[enemy_id]->node_solid[Enemy[enemy_id]->node_neighbour[6]] || Enemy[enemy_id]->node_solid[Enemy[enemy_id]->node_neighbour[7]]) {
	    allow_act=TRUE;
	  }
	}
      }
      if (Enemy[enemy_id]->node_solid[neighbour_id] || allow_act) {//corner or solid
        Enemy[enemy_id]->node_closed[neighbour_id]=TRUE;
      }
      if (!Enemy[enemy_id]->node_closed[neighbour_id] &&
	  !Enemy[enemy_id]->node_solid[neighbour_id]) {
        TGCost=Enemy[enemy_id]->node_gcost[Enemy[enemy_id]->current_node]+CalculateDistanceCost(enemy_id,Enemy[enemy_id]->current_node,neighbour_id);
        if (TGCost<Enemy[enemy_id]->node_gcost[neighbour_id]) {
          Enemy[enemy_id]->node_parent[neighbour_id]=Enemy[enemy_id]->current_node;
          Enemy[enemy_id]->node_gcost[neighbour_id]=TGCost;
          Enemy[enemy_id]->node_hcost[neighbour_id]=CalculateDistanceCost(enemy_id,neighbour_id,Enemy[enemy_id]->end_node);
          Enemy[enemy_id]->node_fcost[neighbour_id]=Enemy[enemy_id]->node_gcost[neighbour_id]+Enemy[enemy_id]->node_hcost[neighbour_id];
          if (!Enemy[enemy_id]->node_open[neighbour_id]) {//not open
            Enemy[enemy_id]->node_open[neighbour_id]=TRUE; //open it
            Enemy[enemy_id]->open_nodes[Enemy[enemy_id]->open_nodes_num]=neighbour_id;//enemy_open_nodes[self_open_nodes_num] = neighbor_id
            Enemy[enemy_id]->open_nodes_num++;//this is the optimization
	  }
        }
      }
    }
  } else if (!Enemy[enemy_id]->trace_target) {//target has been found (Enemy[enemy_id]->found_target),trace back
    Enemy[enemy_id]->search_timer=0;
    if (Enemy[enemy_id]->start_node==Enemy[enemy_id]->end_node) {
      Enemy[enemy_id]->idling=TRUE;
    } else {
      if (Enemy[enemy_id]->node_x[Enemy[enemy_id]->current_node]==Enemy[enemy_id]->node_x[Enemy[enemy_id]->start_node] &&
          Enemy[enemy_id]->node_y[Enemy[enemy_id]->current_node]==Enemy[enemy_id]->node_y[Enemy[enemy_id]->start_node]) {
        Enemy[enemy_id]->path_nodes_num--;
        Enemy[enemy_id]->search_target=FALSE;
        Enemy[enemy_id]->trace_target=TRUE;
        Enemy[enemy_id]->move_to_target=TRUE;
      } else {
        Enemy[enemy_id]->node_back[Enemy[enemy_id]->current_node]=TRUE;
        Enemy[enemy_id]->path_nodes[Enemy[enemy_id]->path_nodes_num]
         =Enemy[enemy_id]->current_node
         =Enemy[enemy_id]->node_parent[Enemy[enemy_id]->current_node];
        Enemy[enemy_id]->path_nodes_num++;
      }
    }
  }
}

void EnemySpriteOnGroundId(int enemy_id,int ground_id)
{
    double ground_entity_angle=GetLineTargetAngle(ground_id,Enemy[enemy_id]->x,Enemy[enemy_id]->y);
    double height_from_ground=GetLineTargetHeight(ground_id,ground_entity_angle,Enemy[enemy_id]->x,Enemy[enemy_id]->y);
    double edge_1_dist=-1;
    double edge_2_dist=-1;
    double edge_angle=-1;

    //GROUND EDGE
    int ground_edge_id=-1;
    if (Enemy[enemy_id]->is_in_ground_edge) {
      ground_edge_id=Enemy[enemy_id]->saved_ground_id;
    } else {
      ground_edge_id=ground_id;
    }
    if (ground_edge_id!=-1) {
      edge_1_dist=GetDistance(Enemy[enemy_id]->x,Enemy[enemy_id]->y,Ground[ground_edge_id]->x1,Ground[ground_edge_id]->y1);
      edge_2_dist=GetDistance(Enemy[enemy_id]->x,Enemy[enemy_id]->y,Ground[ground_edge_id]->x2,Ground[ground_edge_id]->y2);
      if (edge_1_dist<=34 || edge_2_dist<=34) {
        if (!Enemy[enemy_id]->is_in_ground_edge) {
          if (ground_id!=-1) {
            Enemy[enemy_id]->saved_ground_id=ground_id;
            ground_edge_id=ground_id;
          }
          Enemy[enemy_id]->is_in_ground_edge=TRUE;
        }
        if (edge_1_dist<=34 && edge_1_dist>=0 && ground_edge_id!=-1) {
          edge_angle=GetCosAngle(Enemy[enemy_id]->x-Ground[ground_edge_id]->x1,edge_1_dist);
          ///Enemy[enemy_id]->x+=cos(edge_angle);
          //Enemy[enemy_id]->y+=sin(edge_angle);
          Enemy[enemy_id]->angle=edge_angle;
          if (Enemy[enemy_id]->y<Ground[ground_edge_id]->y1) {
            Enemy[enemy_id]->above_ground_edge=TRUE;
            Enemy[enemy_id]->below_ground_edge=FALSE;
          } else {
            Enemy[enemy_id]->above_ground_edge=FALSE;
            Enemy[enemy_id]->below_ground_edge=TRUE;
            if(Enemy[enemy_id]->last_left) {
              Enemy[enemy_id]->flip_sprite=FALSE;
            } else {
              Enemy[enemy_id]->flip_sprite=TRUE;
            }

          }
        } else if (edge_2_dist<=34 && edge_2_dist>=0 && ground_edge_id!=-1) {
          edge_angle=GetCosAngle(Enemy[enemy_id]->x-Ground[ground_edge_id]->x2,edge_2_dist);
          //Enemy[enemy_id]->x+=cos(edge_angle);
          //Enemy[enemy_id]->y+=sin(edge_angle);
          Enemy[enemy_id]->angle=edge_angle;
          if (Enemy[enemy_id]->y<Ground[ground_edge_id]->y2) {
            Enemy[enemy_id]->above_ground_edge=TRUE;
            Enemy[enemy_id]->below_ground_edge=FALSE;
          } else {
            Enemy[enemy_id]->above_ground_edge=FALSE;
            Enemy[enemy_id]->below_ground_edge=TRUE;
            if(Enemy[enemy_id]->last_left) {
              Enemy[enemy_id]->flip_sprite=FALSE;
            } else {
              Enemy[enemy_id]->flip_sprite=TRUE;
            }
          }
        }
      } else {
        Enemy[enemy_id]->in_air_timer=2;
        Enemy[enemy_id]->saved_ground_id=-1;
        Enemy[enemy_id]->is_in_ground_edge=FALSE;
      }
    } else {
      Enemy[enemy_id]->in_air_timer=2;
      Enemy[enemy_id]->saved_ground_id=-1;
      Enemy[enemy_id]->is_in_ground_edge=FALSE;
    }
    

    //FOR VISUALS
    if (!Enemy[enemy_id]->is_in_ground_edge && abs(height_from_ground)<=41) {
      if (ground_id!=-1) {
        Enemy[enemy_id]->in_air_timer=0;
        if (height_from_ground>0) {    //species 1 above ground (positive)
          Enemy[enemy_id]->angle=Ground[ground_id]->angle;
          Enemy[enemy_id]->above_ground=TRUE;
          Enemy[enemy_id]->below_ground=FALSE;
          Enemy[enemy_id]->flip_sprite=FALSE;
        } else {    //species 1 below ground
          Enemy[enemy_id]->angle=-Ground[ground_id]->angle;//-M_PI;
          Enemy[enemy_id]->above_ground=FALSE;
          Enemy[enemy_id]->below_ground=TRUE;
          if(Enemy[enemy_id]->last_left) {
            Enemy[enemy_id]->flip_sprite=FALSE;
          } else {
            Enemy[enemy_id]->flip_sprite=TRUE;
          }
        }
      } 
    }

}



/*void EnemyReboundFromGround(int enemy_id,int ground_id,bool is_rebound)
{
    //rebound movement
    if (is_rebound) {
      /*if (Enemy[enemy_id]->in_node_grid_id!=-1 && Enemy[enemy_id]->in_node_grid_id<MAP_NODE_NUM) {
        if (NodeGrid[Enemy[enemy_id]->in_node_grid_id]->node_solid) {
          Enemy[enemy_id]->knockback_timer=0;
        }
      }*/

      /*if (abs(height_from_ground)<=27) { //actual rebounding
        int tmp_grnd=ground_id; //rebound enemy froun ground
        if (tmp_grnd!=-1) {
          if (Enemy[enemy_id]->above_ground) {
            Enemy[enemy_id]->x-=cos(Ground[tmp_grnd]->angle+M_PI_2);
            Enemy[enemy_id]->y-=sin(Ground[tmp_grnd]->angle+M_PI_2);
          } else if (Enemy[enemy_id]->below_ground){
            Enemy[enemy_id]->x-=cos(Ground[tmp_grnd]->angle-M_PI_2);
            Enemy[enemy_id]->y-=sin(Ground[tmp_grnd]->angle-M_PI_2);      
          }
        }
      }
    }*/
//}


void LargeEnemySpriteTimer(int enemy_id)
{
  if (Enemy[enemy_id]->species==1 || Enemy[enemy_id]->species==3) {
    Enemy[enemy_id]->sprite_timer++;
    if (Enemy[enemy_id]->sprite_timer>16) {
      Enemy[enemy_id]->sprite_timer=0;
    }
  }
}


void LargeEnemyGravity(int enemy_id)
{
  int tmp_=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y+2,30,29);
  int node_grid_id1=Enemy[enemy_id]->in_node_grid_id;//GetGridId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
  int node_grid_id2=GetGridId(Enemy[enemy_id]->x,Enemy[enemy_id]->y+NODE_SIZE,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
  bool is_in_solid1=FALSE;
  bool is_in_solid2=FALSE;
  if (node_grid_id1!=-1) {
    is_in_solid1=NodeGrid[node_grid_id1]->node_solid;
  }
  if (node_grid_id2!=-1) {
    is_in_solid2=NodeGrid[node_grid_id2]->node_solid;
  }
  if (Enemy[enemy_id]->on_ground_id==-1 && tmp_==-1) {//not on ground
    if (!is_in_solid1 && !is_in_solid2) {
      if (!Enemy[enemy_id]->in_water) {
        Enemy[enemy_id]->y+=1; //falling down
      } else {
        Enemy[enemy_id]->y-=1; //floating up
      }
    }
    Enemy[enemy_id]->in_air_timer=2;
    //Enemy[enemy_id]->above_ground=FALSE;
    //Enemy[enemy_id]->below_ground=FALSE;
  } 
  //Enemy[enemy_id]->previous_above_ground=Enemy[enemy_id]->above_ground;
  //Enemy[enemy_id]->previous_below_ground=Enemy[enemy_id]->below_ground;
}


void EnemyMove(int enemy_id)
{
  int path_node_arr_id=Enemy[enemy_id]->path_nodes_num-1,
      path_node_id=Enemy[enemy_id]->path_nodes[path_node_arr_id];
  double path_node_center_x=-1;
  double path_node_center_y=-1;
  if (path_node_id!=-1) {
    path_node_center_x=Enemy[enemy_id]->node_x[path_node_id]+NODE_SIZE/2;
    path_node_center_y=Enemy[enemy_id]->node_y[path_node_id]+NODE_SIZE/2;
  }



  /*if (raining) { //move ment interruption due to rain
    int enemy_on_node_grid_id=GetGridId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);

        //Raining
    if (enemy_on_node_grid_id!=-1 && !Enemy[enemy_id]->ignore_player) {
      if (NodeGrid[enemy_on_node_grid_id]->node_no_shade) {
        Enemy[enemy_id]->ignore_player=TRUE;
        Enemy[enemy_id]->speed_multiplier=Enemy[enemy_id]->ospeed_multiplier*2;
        Enemy[enemy_id]->idling=TRUE;
        Enemy[enemy_id]->move_to_target=FALSE;
      }
    }
  }*/
      if (Enemy[enemy_id]->x<path_node_center_x) {
        //if (Enemy[enemy_id]->species==0) {
          Enemy[enemy_id]->last_left=FALSE;
        /*} else {
          if (Enemy[enemy_id]->sprite_flip_timer>0) {
            Enemy[enemy_id]->sprite_flip_timer--;
          } else {
            Enemy[enemy_id]->last_left=FALSE;
            Enemy[enemy_id]->sprite_flip_timer=50;
          }
        }*/
        Enemy[enemy_id]->x+=Enemy[enemy_id]->speed;
      } else {
        Enemy[enemy_id]->last_left=TRUE;
        Enemy[enemy_id]->x-=Enemy[enemy_id]->speed;  
      }


    if (Enemy[enemy_id]->y<path_node_center_y) {
      Enemy[enemy_id]->y+=Enemy[enemy_id]->speed;
    } else {
      Enemy[enemy_id]->y-=Enemy[enemy_id]->speed;
    }



  if (path_node_center_y-1<=Enemy[enemy_id]->y && Enemy[enemy_id]->y<=path_node_center_y+1 &&
      path_node_center_x-1<=Enemy[enemy_id]->x && Enemy[enemy_id]->x<=path_node_center_x+1) {
    Enemy[enemy_id]->path_nodes_num--;
    if (path_node_arr_id<=0) { //all nodes followed
      Enemy[enemy_id]->idling=TRUE;
      Enemy[enemy_id]->move_to_target=FALSE;
    }
  }
}

void EnemyTargetPlayer(int i)
{
  int target_node=0;
  double target_x=0,target_y=0;
  Enemy[i]->idling=FALSE;
  Enemy[i]->search_target=TRUE;
  Enemy[i]->idle_timer=0;
  Enemy[i]->search_timer=0;
  if (player.print_current_above || player.print_current_below) {
    target_x=player.above_x;
    target_y=player.above_y;
  } else {
    target_x=player.x;
    target_y=player.y;
  }
  target_node=GetGridId(target_x-Enemy[i]->node_x[0],
			target_y-Enemy[i]->node_y[0],
                        MAX_FOLLOW_RANGE*NODE_SIZE,
			NODE_SIZE,
			Enemy[i]->node_num);
  if ((Enemy[i]->species==1 || Enemy[i]->species==3) &&
      Enemy[i]->node_solid[target_node]) {
    target_x=player.above_x2;
    target_y=player.above_y2;
    target_node=GetGridId(target_x-Enemy[i]->node_x[0],
			  target_y-Enemy[i]->node_y[0],
                          MAX_FOLLOW_RANGE*NODE_SIZE,
			  NODE_SIZE,
			  Enemy[i]->node_num);      
  }
  if (Enemy[i]->node_solid[target_node] ||//Total ignore player (player is hidden)
      Enemy[i]->in_unchase_range //dont chase player if its in unchase range
    ) {
    if ((Enemy[i]->species==0 || Enemy[i]->species==2 ||  Enemy[i]->species==4) && Enemy[i]->node_solid[target_node]) {
      Enemy[i]->ignore_player=TRUE; //dont ignore if player is within chase range
    }
    Enemy[i]->target_player=FALSE;
    Enemy[i]->idling=FALSE;
    Enemy[i]->search_target=TRUE;
    Enemy[i]->idle_timer=0;
    target_x=Enemy[i]->x+RandNum(-MAX_FOLLOW_RANGE/4*NODE_SIZE,abs(MAX_FOLLOW_RANGE/4*NODE_SIZE),Enemy[i]->seed);
    target_y=Enemy[i]->y+RandNum(-MAX_FOLLOW_RANGE/4*NODE_SIZE,abs(MAX_FOLLOW_RANGE/4*NODE_SIZE),Enemy[i]->seed);
    target_node=GetGridId(target_x-Enemy[i]->node_x[0],
			target_y-Enemy[i]->node_y[0],
                        MAX_FOLLOW_RANGE*NODE_SIZE,
			NODE_SIZE,
			Enemy[i]->node_num);
  } else {
    Enemy[i]->target_player=TRUE;
    Enemy[i]->ignore_player=FALSE;
  }
  InitEnemyPathfinding(i,target_x,target_y);
}

void EnemyLOSAct(int i)
{
  int los_on_ground_id=0,j=0;
  double x1=0,x2=0,y1=0,y2=0,bullet_gradient=0;
  bool allow_act=FALSE;
  for (j=0;j<10;j++) {//LOS Speed
    if (Enemy[i]->LOS_shot) {
      if (Enemy[i]->LOS_left) {
        Enemy[i]->LOS_x-=cos(Enemy[i]->LOS_angle);
        Enemy[i]->LOS_y-=sin(Enemy[i]->LOS_angle);
      } else {
        Enemy[i]->LOS_x+=cos(Enemy[i]->LOS_angle);
        Enemy[i]->LOS_y+=sin(Enemy[i]->LOS_angle);
      }
      los_on_ground_id=GetOnGroundId(Enemy[i]->LOS_x,Enemy[i]->LOS_y,0.5,0.5);
      if (Enemy[i]->LOS_target_x-1<=Enemy[i]->LOS_x && Enemy[i]->LOS_x<=Enemy[i]->LOS_target_x+1 &&
          Enemy[i]->LOS_target_y-1<=Enemy[i]->LOS_y && Enemy[i]->LOS_y<=Enemy[i]->LOS_target_y+1) {
        Enemy[i]->saw_player=TRUE;
        Enemy[i]->LOS_shot=FALSE;
      } else {
	    allow_act=FALSE;
        if (los_on_ground_id<GROUND_NUM && los_on_ground_id!=-1) {
          if (!Ground[los_on_ground_id]->is_ghost) {
	        allow_act=TRUE;
	      }
        }
	    if (IsOutOfBounds(Enemy[i]->LOS_x,Enemy[i]->LOS_y,5,MAP_WIDTH,MAP_HEIGHT) || allow_act //hit solid
	     ){
          Enemy[i]->saw_player=FALSE;
          Enemy[i]->target_player=FALSE;
          Enemy[i]->LOS_shot=FALSE;
	    }
      }
    } else {
      Enemy[i]->LOS_x=Enemy[i]->x;
      Enemy[i]->LOS_y=Enemy[i]->y;
      Enemy[i]->LOS_target_x=player.x;
      Enemy[i]->LOS_target_y=player.y;
      if (player.x<Enemy[i]->x) {
        x1=Enemy[i]->LOS_target_x;
        x2=Enemy[i]->LOS_x;
        y1=Enemy[i]->LOS_target_y;
        y2=Enemy[i]->LOS_y;
        Enemy[i]->LOS_left=TRUE;
      } else {
        x2=Enemy[i]->LOS_target_x;
        x1=Enemy[i]->LOS_x;
        y2=Enemy[i]->LOS_target_y;
        y1=Enemy[i]->LOS_y;
        Enemy[i]->LOS_left=FALSE;
      }
      bullet_gradient=GetGradient(x1,y1,x2,y2);
      Enemy[i]->LOS_angle=GetCosAngle(x2-x1,GetDistance(x1,y1,x2,y2));//cos(angle) = adjacent/hypothenuse
      if (bullet_gradient<=0) {
        Enemy[i]->LOS_angle=-Enemy[i]->LOS_angle;
      }
      Enemy[i]->LOS_shot=TRUE;
    }
  }
}


void EnemyKnockbackMove(int i,int ground_id) //now with bouncing
{
  if (IsOutOfBounds(Enemy[i]->x,Enemy[i]->y,5,MAP_WIDTH,MAP_HEIGHT)) {
    Enemy[i]->knockback_timer=0;
  } else {
    if (ground_id!=-1 /*&& ground_id!=Enemy[i]->on_ground_id*/) {
      Enemy[i]->knockback_angle=GetBounceAngle(Enemy[i]->knockback_angle,Ground[ground_id]->angle);
      Enemy[i]->player_knockback=FALSE;
    }

    double kb_x=cos(Enemy[i]->knockback_angle)*player.knockback_speed;
    double kb_y=sin(Enemy[i]->knockback_angle)*player.knockback_speed;      
    if (Enemy[i]->in_water) {
      kb_x/=2;
      kb_y/=2;
    }
    if (!Enemy[i]->player_knockback) {
      Enemy[i]->x+=kb_x;
      Enemy[i]->y+=kb_y;
    } else {
      if (Enemy[i]->knockback_left) {
        Enemy[i]->x-=kb_x;
        Enemy[i]->y-=kb_y;
      } else {
        Enemy[i]->x+=kb_x;
        Enemy[i]->y+=kb_y;      
      }
    }
  }
  if (Enemy[i]->knockback_timer<=0) {
    Enemy[i]->knockback_angle=0;
    Enemy[i]->knockback_left=FALSE;
    Enemy[i]->player_knockback=FALSE;
  }
}


void EnemySndAct(int i)
{
  if (Enemy[i]->play_death_snd && !back_to_menu) {
    /*wchar_t sndid[16];
    swprintf(sndid,16,L"bk_%d",i);
    PlaySnd(L"snd/clang_death.wav",sndid);*/
    //PlayMemSnd(cdeath_mem_audio_cache,cdeath_mem_audio_filesize,cdeath_mem_audio_duration,0);
    PlayMemSnd(&channelSoundEffect[1],&channelSoundEffectCache[1],TRUE,0); 
    Enemy[i]->play_death_snd=FALSE;
  }
}

int EnemyActDazzle(int i, int slash_t) 
{
  Enemy[i]->saw_player=FALSE;
  Enemy[i]->ignore_player=TRUE;
  Enemy[i]->target_player=FALSE;
  if (!Enemy[i]->move_to_target) {
  }
  return slash_t;
}


void EnemyActWebStuck(int i)
{
    int nx,ny,sub_tmp_ngid;
    //toggle state web_stuck/unstuck
    if (Enemy[i]->flag_web_stuck) {
      Enemy[i]->flag_web_stuck=FALSE;
      if (!Enemy[i]->web_stuck) {
        Enemy[i]->web_stuck=TRUE;
        Enemy[i]->speed_multiplier=1;
        Enemy[i]->speed=0.01;
      }
    }

  if (Enemy[i]->flag_web_unstuck) {
    Enemy[i]->flag_web_unstuck=FALSE;
    if (Enemy[i]->web_stuck) {
      Enemy[i]->web_stuck=FALSE;
      Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
      Enemy[i]->speed=Enemy[i]->ospeed;
    }
  }


    if (Enemy[i]->species==0 || Enemy[i]->species==2 || Enemy[i]->species==4) {
      if (!Enemy[i]->web_stuck) {
        nx=Enemy[i]->x-NODE_SIZE+GetXFromId(Enemy[i]->current_ngid_n,3)*NODE_SIZE; //     1 2 3   
        ny=Enemy[i]->y-NODE_SIZE+GetYFromId(Enemy[i]->current_ngid_n,3)*NODE_SIZE; //     4 5 6  
      } else {
        nx=Enemy[i]->x-NODE_SIZE*2+GetXFromId(Enemy[i]->current_ngid_n,5)*NODE_SIZE; //   1 2 3 4 5  
        ny=Enemy[i]->y-NODE_SIZE*2+GetYFromId(Enemy[i]->current_ngid_n,5)*NODE_SIZE; //   6 7 8 9 A 
      }
    } else if (Enemy[i]->species==1 || Enemy[i]->species==3) {
      nx=Enemy[i]->x-NODE_SIZE*3+GetXFromId(Enemy[i]->current_ngid_n,7)*NODE_SIZE; //0 1 2 3 4 5 6 
      ny=Enemy[i]->y-NODE_SIZE*3+GetYFromId(Enemy[i]->current_ngid_n,7)*NODE_SIZE; //7 8 9 A B C D
    }
    sub_tmp_ngid=GetGridId(nx,ny,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
    if (sub_tmp_ngid!=-1) {
       if (!NodeGrid[sub_tmp_ngid]->non_web && NodeGrid[sub_tmp_ngid]->node_solid) { //web_detected
         Enemy[i]->flag_web_stuck=TRUE;
         Enemy[i]->current_ngid_n=0;
       }
    }
    //unstuck process
    bool allow_act=FALSE;
    if (Enemy[i]->species==0 || Enemy[i]->species==2 || Enemy[i]->species==4) {
      if (!Enemy[i]->web_stuck) {
        if (Enemy[i]->current_ngid_n==8) {
          allow_act=TRUE;
        }
      } else {
        if (Enemy[i]->current_ngid_n==24) {
          allow_act=TRUE;
        }
      }
    } else if (Enemy[i]->species==1 || Enemy[i]->species==3) {
      if (Enemy[i]->current_ngid_n==48) {
        allow_act=TRUE;
      }
    }
    if (allow_act) {
      if (Enemy[i]->web_stuck && !Enemy[i]->flag_web_stuck) {
        Enemy[i]->flag_web_unstuck=TRUE;
      }
      Enemy[i]->current_ngid_n=0;
    } else {
      Enemy[i]->current_ngid_n++;
    }
}


void EnemyAct(int i)
{
  int dice=0,
	j=0,
	slash_time=1,
	slash_time_i=0,
	target_node=0,
	knock_max=0;
  double target_x=0,target_y=0;
  bool allow_act=FALSE;
  //Attack
  bool deduct_health=FALSE;
  bool force_search=FALSE;



  Enemy[i]->dist_from_player=GetDistance(player.x,player.y,Enemy[i]->x,Enemy[i]->y);
  int dist_unit=GR_WIDTH/2+VGRID_SIZE*2;
  if (GR_WIDTH<GR_HEIGHT)
    dist_unit=GR_HEIGHT/2+VGRID_SIZE*2;

  if (Enemy[i]->dist_from_player<=dist_unit)
    Enemy[i]->within_render_distance=TRUE;
  else
    Enemy[i]->within_render_distance=FALSE;


  for (j=0;j<Enemy[i]->bullet_shot_num;j++) {
    BulletAct(Enemy[i]->bullet_shot_arr[j]);
  }

  if (Enemy[i]->health>0) {


    //Enemy bullet
    EnemyLOSAct(i);//Shoot line of sight bullet


    if (Enemy[i]->damage_taken_timer>0) {
      Enemy[i]->damage_taken_timer--;
    }

    for (int k=0;k<player.bullet_shot_num;k++) {
      int bk=player.bullet[k];
      double dist_from_bullet0=GetDistance(Bullet[bk].x,Bullet[bk].y,Enemy[i]->x,Enemy[i]->y);
      if (dist_from_bullet0<=NODE_SIZE*10) {
        Enemy[i]->force_search=TRUE;
      }
    //^^ condition
      //player bullet
      if (Enemy[i]->species==4 && !Enemy[i]->web_stuck) {
        if (!player.time_breaker) {
          if (dist_from_bullet0<=NODE_SIZE*3) {
            slash_time=EnemyActDazzle(i,1000);
            force_search=TRUE;
          }
        } else {
          if (dist_from_bullet0<=NODE_SIZE*4) {
            slash_time=EnemyActDazzle(i,1500);
            force_search=TRUE;
          }
        }
      }


      if (dist_from_bullet0<=NODE_SIZE*4) {
        switch (Enemy[i]->species) {
      	  case 0:
          case 2:
          case 4: //fly
          {
            if (dist_from_bullet0<=NODE_SIZE*2) {
              Enemy[i]->damage_taken_timer=256;
              Enemy[i]->health-=Bullet[bk].damage;
              if (game_audio) {
                PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
              }
              if (!Enemy[i]->in_water) {
                Enemy[i]->knockback_timer=player.knockback_strength;
              } else {
                Enemy[i]->knockback_timer=player.knockback_strength/2;
              }
              Enemy[i]->knockback_angle=Bullet[bk].angle;
              Enemy[i]->player_knockback=FALSE;
              if (Bullet[bk].graphics_type!=6)
                Bullet[bk].angle=RandAngle(0,360,player.seed);
            }
            }
	        break;
         case 1://crawl
         case 3:
            Enemy[i]->damage_taken_timer=256;
            Enemy[i]->health-=Bullet[bk].damage;
            if (game_audio) {
              PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
            }
            Enemy[i]->knockback_timer=player.knockback_strength;
            Enemy[i]->knockback_angle=Bullet[bk].angle;
            Enemy[i]->player_knockback=FALSE;
            StopBullet(bk,FALSE); //Stop the web
	        for (int m=Bullet[bk].saved_pos;m<player.bullet_shot_num-1;m++) { //shift to left in player bullet shot arr from bullet shot
	          player.bullet[m]=player.bullet[m+1];
              Bullet[player.bullet[m]].saved_pos--;
            }
            Bullet[bk].saved_pos=-1;
	        player.bullet[player.bullet_shot_num-1]=-1; //remove bullet from arr
            player.bullet_shot_num--;
            break; 
        }
      }
    } //end of enemy-playerbullet interactions

    //web swinging affects enemies
    if (player.is_swinging) {
      for (int k=0;k<PLAYER_FLING_WEB_NUM;k++) {
        //knockback
        double dist_from_web_string=GetDistance(player_fling_web.x[k],player_fling_web.y[k],Enemy[i]->x,Enemy[i]->y);
        if (dist_from_web_string<NODE_SIZE*2) {
          Enemy[i]->knockback_timer=player.knockback_strength;
          Enemy[i]->knockback_angle=player.angle_of_incidence;//player.pivot_angle+M_PI_2;
        }  
      }
    }


    //sniper web bullet-enemy interaction
    double dist_from_bullet=GetDistance(Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y,Enemy[i]->x,Enemy[i]->y);
    if (Enemy[i]->species==4 && !Enemy[i]->web_stuck) {
      if (dist_from_bullet<=NODE_SIZE*8) {
        slash_time=EnemyActDazzle(i,1000);
        force_search=TRUE;
      }
    }


    if (dist_from_bullet<=NODE_SIZE*4) {
      switch (Enemy[i]->species) {
	    case 0://fly
        case 2:
        case 4:


          if (dist_from_bullet<=NODE_SIZE*2) {
            if (game_audio) {
              PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
            }
            Enemy[i]->damage_taken_timer=256;
            Enemy[i]->health-=Bullet[player.bullet_shot].damage;
            Enemy[i]->knockback_timer=player.knockback_strength;
            Enemy[i]->knockback_angle=Bullet[player.bullet_shot].angle;            
            Enemy[i]->player_knockback=FALSE;
          }
          break;
        case 1://crawl
        case 3:
          if (game_audio) {
            PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
          }
          Enemy[i]->damage_taken_timer=256;
          Enemy[i]->health-=Bullet[player.bullet_shot].damage;
          Enemy[i]->knockback_timer=player.knockback_strength;
          Enemy[i]->knockback_angle=Bullet[player.bullet_shot].angle;
          Enemy[i]->player_knockback=FALSE;

          StopBullet(player.bullet_shot,TRUE); //Stop the web
          PlayerPlaceWeb(); //Web related
          player.web_being_shot=-1;
          player.bullet_shot=-1;
          break;
      }
    }

   //dazzle if fly close to player
      if (Enemy[i]->species==4 && !Enemy[i]->web_stuck) {
        if (Enemy[i]->dist_from_player<=NODE_SIZE*3) {
          slash_time=EnemyActDazzle(i,1000);
          force_search=TRUE;
        }
      } 
    
   //Enemy knockback & attacked
    allow_act=FALSE;
    double distance_from_player_claws=GetDistance(Enemy[i]->x,Enemy[i]->y,player.claws_attack_x,player.claws_attack_y);
    switch (Enemy[i]->species) {
      case 0://fly
      case 2:
      case 4:
	    if (!(Enemy[i]->time_breaker_length>0)) {
	      if (distance_from_player_claws<=NODE_SIZE*2) {
	        allow_act=TRUE;
	      }
	    } else {
	      if (distance_from_player_claws<=NODE_SIZE*4) {
	        allow_act=TRUE;
	      }
	    }
	    break;
      case 1://crawl
      case 3:
	    if (distance_from_player_claws<=NODE_SIZE*4) {
	      allow_act=TRUE;
	    }
	    break;
    }
    //^^ condition 
    if (allow_act) {
      allow_act=FALSE;
      switch (Enemy[i]->species) {
        case 0:
        case 2:
        case 4:
          if (player.attack_timer>34) {
            allow_act=TRUE;
          }
	      break;
	    case 1:
        case 3:
          if (player.attack_timer>38)
            allow_act=TRUE;
	      break;
	  }
    }
	// ^^ condition
    if (allow_act) {  //player meelee
      allow_act=FALSE;
      deduct_health=TRUE;
      Enemy[i]->player_knockback=TRUE;
      Enemy[i]->knockback_timer=player.knockback_strength;

      if (!player.uppercut /*&& !player.rst_up && !player.rst_down*/) {//normal
        if (player.on_ground_id==-1) {
          Enemy[i]->knockback_angle=0;//player.angle;
        } else {
          Enemy[i]->knockback_angle=player.angle;
        }

        if (player.rst_down) {
          if (!player.last_left) {//drag enemy to player
            Enemy[i]->knockback_angle+=M_PI_2;
          } else {
            Enemy[i]->knockback_angle+=-M_PI_2;
          }
        }
      } else /*if (player.uppercut)*/ {//uppercut
        if (player.print_current_above) {
          if (!player.last_left) {
            Enemy[i]->knockback_angle=player.jump_angle+M_PI;//player.angle-M_PI/2;
          } else {
            Enemy[i]->knockback_angle=player.jump_angle;//player.angle+M_PI/2;
          }
        } else if (player.print_current_below) {
          if (!player.last_left) {
            Enemy[i]->knockback_angle=-player.jump_angle+M_PI_2;//player.angle+M_PI/2;
          } else {
            Enemy[i]->knockback_angle=-player.jump_angle-M_PI_2;//player.angle-M_PI/2;
          }
        }
      }

      if (player.on_ground_id==-1) {
        if (player.last_left) {
          Enemy[i]->knockback_left=TRUE;
        } else {
          Enemy[i]->knockback_left=FALSE;
        }
      } else {
        if (player.print_current_above) {
          if (player.last_left) {
            Enemy[i]->knockback_left=TRUE;
          }
        } else if (player.print_current_below) {
          if (!player.last_left) {
            Enemy[i]->knockback_left=TRUE;
          }
        }
      }
    }


    //Player Spinning Attack
    if (player.block_timer>1 && !player.on_a_ground) {
    //knockback from rebounding
      if (Enemy[i]->dist_from_player<=NODE_SIZE*2){
      
        Enemy[i]->player_knockback=FALSE;
        Enemy[i]->knockback_angle=player.angle_of_incidence;
        Enemy[i]->knockback_timer=player.knockback_strength*2;
        deduct_health=FALSE;
        switch (Enemy[i]->species) {
          case 0:
          case 2:
          case 4:
            if (player.speed>5) {
              deduct_health=TRUE;
            } else if (game_audio) {
              PlaySound(spamSoundEffectCache[6].audio,NULL, SND_MEMORY | SND_ASYNC);            
            }
            break;
          case 1:
          case 3:
            if (player.speed>10) {
              deduct_health=TRUE;
            } else if (game_audio) {
              PlaySound(spamSoundEffectCache[6].audio,NULL, SND_MEMORY | SND_ASYNC);
            }
            break;
        }
      }
    }

    //Deduct health
    if (deduct_health) {
      deduct_health=FALSE;
      Enemy[i]->damage_taken_timer=256;
      Enemy[i]->health-=player.attack_strength;
      if (game_audio) {
        PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
      }
    }


    //Enemy Immediate Death Action
    if (Enemy[i]->health<=0) {
      for (j=0;j<Enemy[i]->bullet_shot_num;j++) { //reset bullets
        StopBullet(Enemy[i]->bullet_shot_arr[j],FALSE);
      }
      int rand_bullet_shot_num;/*=8+RandNum(1,10,Enemy[i]->seed);
      if (Enemy[i]->species==1 || Enemy[i]->species==3) {
        rand_bullet_shot_num=25+RandNum(30,40,Enemy[i]->seed);        
      }*/
      switch (Enemy[i]->species) {
        case 0: rand_bullet_shot_num=20+RandNum(10,20,Enemy[i]->seed); break;
        case 1: rand_bullet_shot_num=25+RandNum(30,40,Enemy[i]->seed); break;
        case 2: rand_bullet_shot_num=8+RandNum(1,10,Enemy[i]->seed); break;
        case 3: rand_bullet_shot_num=25+RandNum(50,60,Enemy[i]->seed); break;
        case 4: rand_bullet_shot_num=12+RandNum(1,10,Enemy[i]->seed); break;
      }
      for (int n=0;n<rand_bullet_shot_num;n++) {
         int rand_range=NODE_SIZE*3+NODE_SIZE*RandNum(1,5,Enemy[i]->seed);
        ShootBullet(current_bullet_id,
            Enemy[i]->bullet_shot_num,
            Enemy[i]->color,
            10, //graphics type
            rand_range, // range
            0.1, //speed
            1+RandNum(1,10,Enemy[i]->seed*player.speed), //speed multiuplier
            0, //damage
            i, //enemy id
            Enemy[i]->x,
            Enemy[i]->y,
            Enemy[i]->x,
            Enemy[i]->y,
            Enemy[i]->x+RandNum(-50,50,Enemy[i]->seed),
            Enemy[i]->y+RandNum(-30,30,Enemy[i]->seed),
            0 //off angle
        );
        Enemy[i]->bullet_shot_arr[Enemy[i]->bullet_shot_num]=current_bullet_id;
        Enemy[i]->bullet_shot_num++;
        
      //after shooting
        current_bullet_id++;
        if (current_bullet_id>=SHOOT_BULLET_NUM-1) {
          current_bullet_id=0;
        }
      }
    }
    int tmp_ngid;
    int tmp_gid_;
    //Knockback 
    if (Enemy[i]->knockback_timer>0 && !Enemy[i]->move_to_target) {
      if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
        Enemy[i]->knockback_timer--;
        if (Enemy[i]->knockback_timer>20) {
          knock_max=player.knockback_speed_multiplier*2;
        } else {
          knock_max=player.knockback_speed_multiplier;
        }
        if (Enemy[i]->web_stuck) {
          knock_max=0;
          Enemy[i]->knockback_timer=0;
        }
        for (j=0;j<knock_max;j++) {
          Enemy[i]->in_node_grid_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);      
          tmp_gid_=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,5,4); 
          if (tmp_gid_==-1) {
            for (int w=0;w<8;w++) { //NOTE; resource intensive BUT only do it while being knockedback, prevents clipping through walls
              switch (w) {
                case 0:tmp_gid_=GetOnGroundId(Enemy[i]->x+NODE_SIZE,Enemy[i]->y,5,4); break;
                case 1:tmp_gid_=GetOnGroundId(Enemy[i]->x-NODE_SIZE,Enemy[i]->y,5,4); break;

                case 2:tmp_gid_=GetOnGroundId(Enemy[i]->x+NODE_SIZE,Enemy[i]->y-NODE_SIZE,5,4); break;
                case 3:tmp_gid_=GetOnGroundId(Enemy[i]->x+NODE_SIZE,Enemy[i]->y+NODE_SIZE,5,4); break;

                case 4:tmp_gid_=GetOnGroundId(Enemy[i]->x-NODE_SIZE,Enemy[i]->y-NODE_SIZE,5,4); break;
                case 5:tmp_gid_=GetOnGroundId(Enemy[i]->x-NODE_SIZE,Enemy[i]->y+NODE_SIZE,5,4); break;

                case 6:tmp_gid_=GetOnGroundId(Enemy[i]->x,Enemy[i]->y+NODE_SIZE,5,4); break;
                case 7:tmp_gid_=GetOnGroundId(Enemy[i]->x,Enemy[i]->y-NODE_SIZE,5,4); break;
              }
              if (tmp_gid_!=-1) {
                break;
              }
            }
          }
          EnemyKnockbackMove(i,tmp_gid_);
        }
      }
    }

    //Pathfinding and movement only act when within render distance
    if ((!player.time_breaker || Enemy[i]->time_breaker_immune || Enemy[i]->species==4) && Enemy[i]->within_render_distance) {

      //timebreaker enemy
      if (Enemy[i]->time_breaker_length>0 && !player.time_breaker) {
        dice=RandNum(0,Enemy[i]->time_breaker_rare,Enemy[i]->seed);
        if (dice==1) {
          slash_time=Enemy[i]->time_breaker_length;
        }
      }

      if (Enemy[i]->species==1 || Enemy[i]->species==3) {
        if (Enemy[i]->knockback_timer>0 || Enemy[i]->on_ground_id==-1 || Enemy[i]->in_water) {
          LargeEnemySpriteTimer(i);
        }
      }

      if (Enemy[i]->web_stuck) {
        slash_time=1;
      }


      //toggle staate in water
      Enemy[i]->in_node_grid_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);      
      tmp_ngid=Enemy[i]->in_node_grid_id;
      if (tmp_ngid!=-1) {
        if (NodeGrid[tmp_ngid]->node_water) {
          if (!Enemy[i]->in_water) {
            if (Enemy[i]->species!=3) {
              Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier/4;
              if (Enemy[i]->speed_multiplier<=1) {
                Enemy[i]->speed_multiplier=1;
              }
            } else {
              Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier*2;
            }
          }
          Enemy[i]->in_water=TRUE;
          Enemy[i]->sprite_in_water=TRUE;
        } else {
          if (Enemy[i]->in_water) {
            Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
          } 
          Enemy[i]->in_water=FALSE;
        }
      } else {
        if (Enemy[i]->in_water) {
          Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
        } 
        Enemy[i]->in_water=FALSE;
      }
    
        if (!Enemy[i]->in_water && Enemy[i]->sprite_in_water) {
          for (int u=0;u<2;u++) {
            switch (u) {
              case 0:tmp_ngid=GetGridId(Enemy[i]->x,Enemy[i]->y+NODE_SIZE,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);break;
              case 1:tmp_ngid=GetGridId(Enemy[i]->x,Enemy[i]->y+NODE_SIZE*2,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);break;
            }
            if (tmp_ngid!=-1) {
              if (!NodeGrid[tmp_ngid]->node_water) {
                Enemy[i]->sprite_in_water=FALSE;
                break;
              }
            }
          }
        }
    
      for (slash_time_i=0;slash_time_i<slash_time;slash_time_i++) {
          Enemy[i]->in_node_grid_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);      
          Enemy[i]->on_ground_id=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,33,32);
          if ((Enemy[i]->species==1 || Enemy[i]->species==3) && Enemy[i]->on_ground_id!=-1) {//<------ source of major error
            EnemySpriteOnGroundId(i,Enemy[i]->on_ground_id);
          }
          if (Enemy[i]->on_ground_id==-1) {
            Enemy[i]->is_in_ground_edge=FALSE;
          }

      //check state web stuck
        EnemyActWebStuck(i);

        //Prevent reaching border
        if (Enemy[i]->x<NODE_SIZE*2) {
          Enemy[i]->x++;
        }
        if (Enemy[i]->y<NODE_SIZE*2) {
          Enemy[i]->y++;
        }
        if (Enemy[i]->x>MAP_WIDTH-NODE_SIZE*2) {
          Enemy[i]->x--;
        }
        if (Enemy[i]->y>MAP_HEIGHT-NODE_SIZE*2) {
          Enemy[i]->y--;
        }

        if ((Enemy[i]->species==1 || Enemy[i]->species==3) && !Enemy[i]->move_to_target && !Enemy[i]->target_player) {//Species 1 gravity
          if (slash_time_i==0) {
            LargeEnemyGravity(i);
            if (Enemy[i]->in_air_timer>0) {
              Enemy[i]->in_air_timer--;
            }
          }
        }
        if (Enemy[i]->species==4 && !Enemy[i]->web_stuck) {
          if (!player.time_breaker) {
            if (slash_time>1) {
              Enemy[i]->speed_multiplier=1;
              Enemy[i]->speed=0.05;
            } else {
              if (slash_time_i==0) {
                Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
                Enemy[i]->speed=Enemy[i]->ospeed;
              }
            }
          } else {
            if (slash_time>1) {
              Enemy[i]->speed_multiplier=1;
              Enemy[i]->speed=0.025;
            } else {
              if (slash_time_i==0) {
                Enemy[i]->speed_multiplier=1;
                Enemy[i]->speed=0.05;
              }
            }
          }
        }
        /*int above_player_node1=GetGridId(
                player.above_x-Enemy[i]->node_x[0],
                player.above_y-Enemy[i]->node_y[0],
                Enemy[i]->follow_range*NODE_SIZE,
                NODE_SIZE,Enemy[i]->node_num);*/

        //int enemy_on_node_grid_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);

        //Raining
        /*if (raining && !Enemy[i]->move_to_target) {
          if (enemy_on_node_grid_id!=-1) {
            if (NodeGrid[enemy_on_node_grid_id]->node_no_shade) {
              Enemy[i]->ignore_player=TRUE;
              force_search=TRUE;
              Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier*2;
            } else {
              if (Enemy[i]->speed_multiplier!=Enemy[i]->ospeed_multiplier)
                Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
            }
          }
        }*/
        /*if (Enemy[i]->is_in_ground_edge) {
          force_search=TRUE;
        }*/
        if (Enemy[i]->force_search) {
          force_search=TRUE;
          Enemy[i]->force_search=FALSE;
        }

        if ((!Enemy[i]->ignore_player /*&&  //not ignore & within range
             Enemy[i]->dist_from_player<Enemy[i]->follow_range/2*NODE_SIZE*/
            )
           ) {
          if (Enemy[i]->bullet_fire_cooldown<=0) {
	        if (Enemy[i]->bullet_length==0) {
	          for (j=0;j<Enemy[i]->bullet_fire_at_once_max;j++) {//shoot several bullets at once
	            Enemy[i]->bullet_head_x[j]=player.x+RandNum(-Enemy[i]->aim_rand,Enemy[i]->aim_rand,Enemy[i]->seed);
	            Enemy[i]->bullet_head_y[j]=player.y+RandNum(-Enemy[i]->aim_rand,Enemy[i]->aim_rand,Enemy[i]->seed);
	          }
	        }

            if (Enemy[i]->bullet_cooldown<=0) {
	          if (Enemy[i]->bullet_shot_num<500) {//shot less than 500 bullets
                int bspeed_m=Enemy[i]->bullet_speed_multiplier,
                    bdmg=Enemy[i]->bullet_damage;
                double bspeed=Enemy[i]->bullet_speed,
                       brange=Enemy[i]->bullet_range;
                if (Enemy[i]->web_stuck) {
                  bspeed/=3;
                  brange/=3;             
                  bspeed_m=bspeed_m/3+1;
                  bdmg/=3;
                }
	            for (j=0;j<Enemy[i]->bullet_fire_at_once_max;j++) {//several bullets at once
		          if (Enemy[i]->saw_player) {
                    if (Enemy[i]->dist_from_player<Enemy[i]->shoot_at_player_range/2*NODE_SIZE) {
    		          Enemy[i]->shoot_target_x=Enemy[i]->bullet_head_x[j];
        		      Enemy[i]->shoot_target_y=Enemy[i]->bullet_head_y[j];
                      ShootBullet(current_bullet_id,
		                Enemy[i]->bullet_shot_num,
		                Enemy[i]->bullet_color,
		                Enemy[i]->bullet_graphics_type,
		                brange,
		                bspeed,
		                bspeed_m,
		                bdmg,
		                i,
		                Enemy[i]->x,
		                Enemy[i]->y,
		                Enemy[i]->x,
		                Enemy[i]->y,
		                Enemy[i]->shoot_target_x,
		                Enemy[i]->shoot_target_y,
                        0
                    );
                    /*if (Enemy[i]->shoot_target_x<Enemy[i]->x) {
                       Enemy[i]->last_left=TRUE;
                    } else {
                       Enemy[i]->last_left=FALSE;
                    }*/
                    Enemy[i]->bullet_shot_arr[Enemy[i]->bullet_shot_num]=current_bullet_id;
                    Enemy[i]->bullet_shot_num++;
                  //after shooting
                    current_bullet_id++;
                    if (current_bullet_id>=SHOOT_BULLET_NUM-1) {
                      current_bullet_id=0;
                    }
                  }
	            }
	          } //end of for
	        }
	        Enemy[i]->bullet_cooldown=Enemy[i]->bullet_cooldown_max;
	        Enemy[i]->bullet_length++;
	        if (Enemy[i]->bullet_length>=Enemy[i]->bullet_length_max) {
	          Enemy[i]->bullet_fire_cooldown=Enemy[i]->bullet_fire_cooldown_max;
	          Enemy[i]->bullet_length=0;
	        }
	      } else {
            Enemy[i]->bullet_cooldown--;
          }
        } else {
	      Enemy[i]->bullet_fire_cooldown--;
        }
      }


    //pathfinding
      Enemy[i]->forgor_timer++;
      if (Enemy[i]->forgor_timer>300) {
        Enemy[i]->forgor_timer=0;
        Enemy[i]->ignore_player=FALSE;
      }

    //Movement
    if (Enemy[i]->idling) { //idling
      /*if (Enemy[i]->species==4) {
        Enemy[i]->idle_timer=29;
      }*/
      Enemy[i]->idle_timer++;
	  allow_act=FALSE;
      if (Enemy[i]->saw_player && !Enemy[i]->ignore_player) {//not ignoring
        if (!Enemy[i]->target_player && //not targetting
             Enemy[i]->dist_from_player/NODE_SIZE<Enemy[i]->follow_range/2-2) {
          allow_act=TRUE;
        }
      }
      //^^ condition
      if (allow_act) {
        EnemyTargetPlayer(i); //target player
      } else if (Enemy[i]->idle_timer>30 || force_search) {//idling over
        Enemy[i]->target_player=FALSE;
        dice=RandNum(0,5,Enemy[i]->seed);
        Enemy[i]->idle_timer=0;
        if (dice==1 || force_search) { //Start searching
	    //total ignore player (still hostile)
          if (force_search) {
            force_search=FALSE;
          }
          target_x=Enemy[i]->x+RandNum(-MAX_FOLLOW_RANGE/3*NODE_SIZE,abs(MAX_FOLLOW_RANGE/3*NODE_SIZE),Enemy[i]->seed);
          target_y=Enemy[i]->y+RandNum(-MAX_FOLLOW_RANGE/3*NODE_SIZE,abs(MAX_FOLLOW_RANGE/3*NODE_SIZE),Enemy[i]->seed/2);
          target_node=GetGridId(target_x-Enemy[i]->node_x[0],
		                        target_y-Enemy[i]->node_y[0],
                                MAX_FOLLOW_RANGE*NODE_SIZE,
		                        NODE_SIZE,
    	                        Enemy[i]->node_num);
          if (!Enemy[i]->node_solid[target_node]/* && (!raining ||
              (raining && !NodeGrid[enemy_on_node_grid_id]->node_no_shade))*/
             ) {
            Enemy[i]->idling=FALSE;
            Enemy[i]->search_target=TRUE;
            InitEnemyPathfinding(i,target_x,target_y);
          } else {
            Enemy[i]->idling=TRUE;
          }
        } else { //not searching
          Enemy[i]->idling=TRUE;
        }
      }
    } else if (Enemy[i]->search_target) {
      //Enemy[i]->msprite_hold_timer_max=0;
      EnemyPathFinding(i);
      Enemy[i]->search_timer++;
      if (Enemy[i]->search_timer>500) {//stop searching
        if (Enemy[i]->dist_from_player/NODE_SIZE<Enemy[i]->follow_range/2-2) { //within distance
          Enemy[i]->ignore_player=TRUE;
        }
        Enemy[i]->idling=TRUE;
        Enemy[i]->search_target=FALSE;
        Enemy[i]->search_timer=0;
      }
    } else if (Enemy[i]->move_to_target) {
      for (j=0;j<Enemy[i]->speed_multiplier;j++) {
        LargeEnemySpriteTimer(i);
        EnemyMove(i);
      }
      if (!Enemy[i]->ignore_player && Enemy[i]->saw_player) { //chasing player
        /*if (Enemy[i]->species==0 /*&&
            NodeGrid[GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)]->node_solid)
        {   //become blind when inside a solid
          Enemy[i]->saw_player=FALSE;
          Enemy[i]->idling=TRUE;
          Enemy[i]->move_to_target=FALSE;
        }*/
        if (!Enemy[i]->idling) {
          if (player.x<Enemy[i]->x) {
            Enemy[i]->player_at_left=TRUE;
          } else if (Enemy[i]->x<=player.x) {
            Enemy[i]->player_at_right=TRUE;
          }
          if (Enemy[i]->player_at_left && Enemy[i]->player_at_right) {//Change of X
            EnemyTargetPlayer(i);
            Enemy[i]->player_at_left=FALSE;
            Enemy[i]->player_at_right=FALSE;
          }
          if (player.y<Enemy[i]->y) {//Change of Y
            Enemy[i]->player_at_above=TRUE;
          } else if (Enemy[i]->y<=player.y) {
            Enemy[i]->player_at_below=TRUE;
          }
          if (Enemy[i]->player_at_above && Enemy[i]->player_at_below) {
            EnemyTargetPlayer(i);
            Enemy[i]->player_at_above=FALSE;
            Enemy[i]->player_at_below=FALSE;
          }
          if (Enemy[i]->chase_range/2*NODE_SIZE<Enemy[i]->dist_from_player && Enemy[i]->dist_from_player<Enemy[i]->unchase_range/2*NODE_SIZE //in unchase range
      	         ) //not in chase range) 
          {
            Enemy[i]->in_unchase_range=TRUE;
          } else {
            Enemy[i]->in_chase_range=TRUE;
          }
          if (Enemy[i]->in_unchase_range && Enemy[i]->in_chase_range) {
            EnemyTargetPlayer(i);
            Enemy[i]->in_chase_range=FALSE;
            Enemy[i]->in_unchase_range=FALSE;
              }
            }
          }
        }//end of slash_time
      //other
        if (!Enemy[i]->web_stuck) {
          if (Enemy[i]->species==0 || Enemy[i]->species==2 || (Enemy[i]->species==4 && (!player.time_breaker || Enemy[i]->time_breaker_immune))) {
            Enemy[i]->sprite_timer++;
            if (Enemy[i]->sprite_timer>3) {
              Enemy[i]->sprite_timer=0;
            }
          }
        }

      }
    }//end of tbt
  } else {//end of health
    //Shoot death bullets
    if (Enemy[i]->damage_taken_timer>0) {
      Enemy[i]->damage_taken_timer--;
    }
    if (Enemy[i]->death_timer<1000)
      Enemy[i]->death_timer++;
  }

}

void SetEnemyByType(int i,int type)
{
  Enemy[i]->angle=0;
  Enemy[i]->angle=0;
  Enemy[i]->type=type;
  Enemy[i]->species=saved_enemy_type_species[type];
  Enemy[i]->max_health=Enemy[i]->health=saved_enemy_type_health[type];
  Enemy[i]->follow_range=saved_enemy_type_follow_range[type];
  Enemy[i]->chase_range=saved_enemy_type_chase_range[type];
  Enemy[i]->unchase_range=saved_enemy_type_unchase_range[type];
  if (Enemy[i]->unchase_range>Enemy[i]->follow_range ||
      Enemy[i]->chase_range>Enemy[i]->unchase_range) {
    Enemy[i]->unchase_range=0;
    Enemy[i]->chase_range=0;
  }
  Enemy[i]->color=rgbPaint[saved_enemy_type_color[type]];
  Enemy[i]->ospeed=saved_enemy_type_speed[type];
  Enemy[i]->speed=saved_enemy_type_speed[type];
  Enemy[i]->ospeed_multiplier=saved_enemy_type_speed_multiplier[type];
  Enemy[i]->speed_multiplier=saved_enemy_type_speed_multiplier[type];
//bool
  Enemy[i]->time_breaker_immune=saved_enemy_type_time_breaker_immune[type];
//shoot
  Enemy[i]->bullet_fire_cooldown_max=
  Enemy[i]->bullet_fire_cooldown=saved_enemy_type_bullet_fire_cooldown[type];
  Enemy[i]->bullet_fire_at_once_max=saved_enemy_type_bullet_fire_at_once[type];
  Enemy[i]->bullet_length_max=saved_enemy_type_bullet_length[type];
  Enemy[i]->bullet_length=0;
  Enemy[i]->shoot_at_player_range=saved_enemy_type_shoot_at_player_range[type];
  Enemy[i]->aim_rand=saved_enemy_type_aim_rand[type];
//bullet
  Enemy[i]->bullet_damage=saved_enemy_type_bullet_damage[type],
  Enemy[i]->bullet_cooldown_max=Enemy[i]->bullet_cooldown=saved_enemy_type_bullet_cooldown[type];
  Enemy[i]->bullet_speed=saved_enemy_type_bullet_speed[type];
  Enemy[i]->bullet_speed_multiplier=saved_enemy_type_bullet_speed_multiplier[type];
  Enemy[i]->bullet_range=saved_enemy_type_bullet_range[type];
  Enemy[i]->bullet_color=rgbPaint[saved_enemy_type_bullet_color[type]];
  Enemy[i]->bullet_graphics_type=saved_enemy_type_bullet_graphics_type[type];
  //time breaker
  Enemy[i]->time_breaker_rare=saved_enemy_type_time_breaker_rare[type];
  Enemy[i]->time_breaker_length=saved_enemy_type_time_breaker_length[type];
}


void CleanUpRotatedSprites()
{
  //manual cleaning because static, NO MORE dynamic sprites!!
  for (int i=0;i<LARGE_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      FreeDrawSprite(&EnemyRotatedSprite[i].draw_rotated_sprite1[j]);
      FreeDrawSprite(&EnemyRotatedSprite[i].draw_rotated_sprite2[j]);
    }
  }

  for (int i=0;i<LARGER_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      FreeDrawSprite(&XEnemyRotatedSprite[i].draw_rotated_sprite[j]);
    }
  }
}

void CleanUpEnemySprites()
{
  //manual cleaning because static
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    FreeDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_1);
    FreeDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_2);
  }
}


void InitEnemySprites()
{
  LARGE_ENEMY_TYPE_NUM=0;
  LARGER_ENEMY_TYPE_NUM=0;
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    saved_large_enemy_type_species[i]=-1;
    saved_larger_enemy_type_species[i]=-1;
  }
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    if (saved_enemy_type_species[i]==1 || saved_enemy_type_species[i]==3) {
      saved_enemy_type_rot_sprite_id[i]=LARGE_ENEMY_TYPE_NUM;
      saved_large_enemy_type_species[LARGE_ENEMY_TYPE_NUM]=saved_enemy_type_species[i];
      if (saved_enemy_type_time_breaker_immune[i]) {
        saved_large_enemy_type_time_breaker_immune[LARGE_ENEMY_TYPE_NUM]=TRUE;
      } else {
        saved_large_enemy_type_time_breaker_immune[LARGE_ENEMY_TYPE_NUM]=FALSE;
      }
      LARGE_ENEMY_TYPE_NUM++;
      if (saved_enemy_type_species[i]==3) {
        saved_enemy_type_rot_xsprite_id[i]=LARGER_ENEMY_TYPE_NUM;
        saved_larger_enemy_type_species[LARGER_ENEMY_TYPE_NUM]=3;
        if (saved_enemy_type_time_breaker_immune[i]) {
          saved_larger_enemy_type_time_breaker_immune[LARGER_ENEMY_TYPE_NUM]=TRUE;
        } else {
          saved_larger_enemy_type_time_breaker_immune[LARGER_ENEMY_TYPE_NUM]=FALSE;
        }
        LARGER_ENEMY_TYPE_NUM++;
      } else {
        saved_larger_enemy_type_species[i]=-1;
        saved_enemy_type_rot_xsprite_id[i]=-1;
        saved_larger_enemy_type_time_breaker_immune[i]=FALSE;
      }
    } else {
      saved_enemy_type_rot_sprite_id[i]=-1;
      saved_enemy_type_rot_xsprite_id[i]=-1;
      saved_large_enemy_type_time_breaker_immune[i]=FALSE;
      saved_larger_enemy_type_time_breaker_immune[i]=FALSE;
    }
  }

  for (int i=0;i<ENEMY_NUM;i++) {
    Enemy[i]->rotated_sprite_id=saved_enemy_type_rot_sprite_id[Enemy[i]->type];
    Enemy[i]->rotated_xsprite_id=saved_enemy_type_rot_xsprite_id[Enemy[i]->type];
    //printf("Enemy[i]->rotated_sprite_id:%d\n",Enemy[i]->rotated_sprite_id);
  }
}


void InitEnemySpritesObj()
{
  int species_i=0;
  double angle_rn=0;
  HBITMAP tmp_sprite1,tmp_sprite2;
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    species_i=saved_enemy_type_species[i];
    switch (species_i) {
      case 0:
        tmp_sprite1=RotateSprite(NULL, enemy1_sprite_1,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        tmp_sprite2=RotateSprite(NULL, enemy1_sprite_2,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        break;
      case 1:
        tmp_sprite1=RotateSprite(NULL, enemy2_sprite_3,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        tmp_sprite2=RotateSprite(NULL, enemy2_sprite_4,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        break;
      case 2:
        tmp_sprite1=RotateSprite(NULL, enemy3_sprite_1,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        tmp_sprite2=RotateSprite(NULL, enemy3_sprite_2,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        break;
      case 3:
        tmp_sprite1=RotateSprite(NULL, enemy4_sprite_3,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        tmp_sprite2=RotateSprite(NULL, enemy4_sprite_4,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        break;
      case 4:
        tmp_sprite1=RotateSprite(NULL, enemy5_sprite_1,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        tmp_sprite2=RotateSprite(NULL, enemy5_sprite_2,0,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[i]],-1);
        break;
    }

    GenerateDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_1,tmp_sprite1);
    GenerateDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_2,tmp_sprite2);
    DeleteObject(tmp_sprite2);
    DeleteObject(tmp_sprite1);

    loading_numerator++;
  }

  for (int i=0;i<LARGE_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      //angle_rn=M_PI_2-M_PI_16*j;
      species_i=saved_large_enemy_type_species[i]; 
      switch (species_i) {
        case 1:
          /*tmp_sprite1=RotateSprite(NULL,enemy2_sprite_1,angle_rn,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]],-1);
          loading_numerator++;
          tmp_sprite2=RotateSprite(NULL,enemy2_sprite_2,angle_rn,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]],-1);
          loading_numerator++;*/
          EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[0].draw_rotated_sprite1[j].sprite_mask,SRCCOPY);
          EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[0].draw_rotated_sprite1[j].sprite_paint,SRCCOPY);
          ReplaceColorSprite(EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint,
            LTGREEN,
            rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]]);
          loading_numerator++;
          EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[0].draw_rotated_sprite2[j].sprite_mask,SRCCOPY);
          EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[0].draw_rotated_sprite2[j].sprite_paint,SRCCOPY);
          ReplaceColorSprite(EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint,
            LTGREEN,
            rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]]);
          loading_numerator++;
          break;
        case 3:
          /*tmp_sprite1=RotateSprite(NULL,enemy4_sprite_1,angle_rn,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]],-1);
          loading_numerator++;
          tmp_sprite2=RotateSprite(NULL,enemy4_sprite_2,angle_rn,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]],-1);
          loading_numerator++;*/
          EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[1].draw_rotated_sprite1[j].sprite_mask,SRCCOPY);
          EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[1].draw_rotated_sprite1[j].sprite_paint,SRCCOPY);
          ReplaceColorSprite(EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint,
            LTGREEN,
            rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]]);
          loading_numerator++;
          EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[1].draw_rotated_sprite2[j].sprite_mask,SRCCOPY);
          EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[1].draw_rotated_sprite2[j].sprite_paint,SRCCOPY);
          ReplaceColorSprite(EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint,
            LTGREEN,
            rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]]);
          loading_numerator++;
          break;
      }

      //GenerateDrawSprite(&EnemyRotatedSprite[i].draw_rotated_sprite1[j],tmp_sprite1);
      //GenerateDrawSprite(&EnemyRotatedSprite[i].draw_rotated_sprite2[j],tmp_sprite2);
      //DeleteObject(tmp_sprite2);
      //DeleteObject(tmp_sprite1);
    }
  }

  for (int i=0;i<LARGER_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      angle_rn=M_PI_2-M_PI_16*j;
      species_i=saved_larger_enemy_type_species[i];
      switch (species_i) {
        case 3:
          /*tmp_sprite1=RotateSprite(NULL, enemy4_sprite_1_0,angle_rn,LTGREEN,BLACK,rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]],-1);
          loading_numerator++;
          GenerateDrawSprite(&XEnemyRotatedSprite[i].draw_rotated_sprite[j],tmp_sprite1);
          DeleteObject(tmp_sprite1);*/

          XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_mask=CopyCrunchyBitmap(XLoadEnemyRotatedSprite[0].draw_rotated_sprite[j].sprite_mask,SRCCOPY);
          XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_paint=CopyCrunchyBitmap(XLoadEnemyRotatedSprite[0].draw_rotated_sprite[j].sprite_paint,SRCCOPY);
          ReplaceColorSprite(XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_paint,
            LTGREEN,
            rgbPaint[saved_enemy_type_color[saved_enemy_type_rot_sprite_id[i]]]);
          loading_numerator++;
          //DeleteObject(tmp_sprite1);
          break;
      }
    }
  }
}


//bool once=TRUE;
void InitEnemy()
{
  int i=0,j=0,x=0,y=0;
  for (i=0;i<ENEMY_NUM;i++) {
    //printf("Enemy: %d\n",i);
    //Enemy[i]->being_drawn=TRUE;
    Enemy[i]->on_ground_id=-1;
    Enemy[i]->saved_ground_id=-1;
    Enemy[i]->current_ngid_n=0;
    Enemy[i]->seed=0;
    Enemy[i]->current_rot_sprite_angle_id=0;//-1;
    Enemy[i]->dist_from_player=999;
    Enemy[i]->x=saved_enemy_x[i];
    Enemy[i]->y=saved_enemy_y[i];
    Enemy[i]->sprite_x=
    Enemy[i]->sprite_y=-20;
    Enemy[i]->is_ground_rebounding=FALSE;
    Enemy[i]->is_in_ground_edge=FALSE;
    Enemy[i]->force_search=FALSE;
    Enemy[i]->sprite_flip_timer=0;
    SetEnemyByType(i,saved_enemy_type[i]);
    if (Enemy[i]->x<5) {
      Enemy[i]->x=25;
    }
    if (Enemy[i]->y<5) {
      Enemy[i]->y=25;
    }
   // Enemy[i]->previous_above_ground=
   // Enemy[i]->previous_below_ground=
    Enemy[i]->above_ground=
    Enemy[i]->below_ground=FALSE;
    Enemy[i]->in_air_timer=0;
    Enemy[i]->angle=0;
    Enemy[i]->angle=0;
    Enemy[i]->sprite_angle=0;
    //Enemy[i]->saved_angle=-9999;
    Enemy[i]->damage_taken_timer=0;
    Enemy[i]->death_timer=0;
  //bullet
    Enemy[i]->bullet_shot_num=0;
    for (j=0;j<ENEMY_BULLET_NUM;j++) {
      Enemy[i]->bullet_shot_arr[j]=-1;
    }
    //printf("initialized Bullets\n");
    for (j=0;j<MAX_BULLET_PER_FIRE;j++) {
      Enemy[i]->bullet_head_x[j]=0;
      Enemy[i]->bullet_head_y[j]=0;
    }
    Enemy[i]->player_knockback=FALSE;
    Enemy[i]->knockback_left=FALSE;
    Enemy[i]->knockback_angle=0;
    Enemy[i]->knockback_timer=0;
    Enemy[i]->shoot_target_x=0;
    Enemy[i]->shoot_target_y=0;
    Enemy[i]->in_node_grid_id=FALSE;
    Enemy[i]->sprite_in_water=FALSE;
    Enemy[i]->in_water=FALSE;
    Enemy[i]->web_stuck=FALSE;
    Enemy[i]->flag_web_unstuck=FALSE;
    Enemy[i]->flag_web_stuck=FALSE;

  //LOS
    Enemy[i]->LOS_left=FALSE;
    Enemy[i]->LOS_shot=FALSE;
    Enemy[i]->LOS_x=-20;
    Enemy[i]->LOS_y=-20;
    Enemy[i]->LOS_angle=0;
    Enemy[i]->LOS_target_x=-20;
    Enemy[i]->LOS_target_y=-20;
  //init default bool
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
    Enemy[i]->play_death_snd=FALSE;
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
      Enemy[i]->enemy_species1_solids[j]=-1;
      Enemy[i]->node_solid[j]=
        Enemy[i]->node_back[j]=
        Enemy[i]->node_open[j]=
        Enemy[i]->node_closed[j]=FALSE;
      Enemy[i]->node_x[j]=(Enemy[i]->x/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(MAX_FOLLOW_RANGE/2);
      Enemy[i]->node_y[j]=(Enemy[i]->y/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(MAX_FOLLOW_RANGE/2);
      Enemy[i]->node_hcost[j]=0;
      Enemy[i]->node_gcost[j]=INT_MAX;
      Enemy[i]->node_fcost[j]=Enemy[i]->node_gcost[j]+Enemy[i]->node_hcost[j];
      Enemy[i]->node_parent[j]=-1;
    }
    x=Enemy[i]->x-Enemy[i]->node_x[0];
    y=Enemy[i]->y-Enemy[i]->node_y[0];
    Enemy[i]->start_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[i]->node_num);
    Enemy[i]->end_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,Enemy[i]->node_num);
    Enemy[i]->open_nodes_num=0;
    for (j=0;j<MAX_NODE_NUM;j++) {
      Enemy[i]->open_nodes[j]=Enemy[i]->start_node;
    }
    EnemyAct(i);
  }
}

//int cenemy=0;
//int funny=0;
void DrawEnemy(HDC hdc,HDC hdc2)
{
  int i=0,k=0,c=0,c2=0;
  int 
    px=player.cam_x,
    py=player.cam_y,
    cx1=player.cam_mouse_move_x,
    cy1=player.cam_mouse_move_y,
    cx2=player.cam_move_x,
    cy2=player.cam_move_y;
  //int min_enemy=0, max_enemy=100;
  if (frame_tick==-8) { //initiate on start of app
    player.flag_revert_palette=TRUE;
    player.time_breaker_tick=-1;
  }

  /*funny++;
  if (funny>63)
    funny=0;*/

  for (i=0;i<ENEMY_NUM;i++) {  
    Enemy[i]->sprite_x=(int)Enemy[i]->x+px+cx1+cx2;
    Enemy[i]->sprite_y=(int)Enemy[i]->y+py+cy1+cy2;
    if (Enemy[i]->species==1 || Enemy[i]->species==3) {//rotate sprite


      //sprite_angle_calculation;
/*
                                |->
       64/0         below  ^    |
   48        16     ground |--- + ---|   above ground       __________________
        32                      |    v                      ---\ angle
                              <-|                               -----\
    arr = sprite_angle
    63/0 = PI+PI/2
    16 = 0
    32 = PI/2
    48 = PI


    [48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63]
    [ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15]


    [16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31]
    [32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47]

   currently anticlockwise
*/        
     //WHILE ON GROUND EDGE
      if (Enemy[i]->is_in_ground_edge) {
        int le_angle;
        if (Enemy[i]->above_ground_edge) {
          if (!Enemy[i]->last_left) {
            le_angle=16-(Enemy[i]->angle)/(M_PI_16);
          } else {
            le_angle=(Enemy[i]->angle)/(M_PI_16);
          } 
        } else if (Enemy[i]->below_ground_edge) {
          if (Enemy[i]->last_left) {
            le_angle=16+(Enemy[i]->angle)/(M_PI_16);
          } else {
            le_angle=32-(Enemy[i]->angle)/(M_PI_16);
          } 
        }

        if (le_angle>31) {
          le_angle-=32;
        } 
        if (le_angle<0) {
          le_angle+=32;
        }
        if (le_angle>-1 && le_angle<32) {
          Enemy[i]->current_rot_sprite_angle_id=le_angle;
        }

     //ON GROUND ACTUAL
      } else if (Enemy[i]->on_ground_id!=-1) {
        int le_angle=8;
        Enemy[i]->sprite_angle=Enemy[i]->angle;
        if (Enemy[i]->sprite_angle>0) { //Slope ++ \/
            if (Enemy[i]->above_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=8+Enemy[i]->sprite_angle/(M_PI_16);
              } else {
                le_angle=8-Enemy[i]->sprite_angle/(M_PI_16);
              }
            } else if (Enemy[i]->below_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=24+(Enemy[i]->sprite_angle)/(M_PI_16);
              } else {
                le_angle=24-(Enemy[i]->sprite_angle)/(M_PI_16);
              }
           }
        } else { //Slope -- /
            if (Enemy[i]->above_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=8+Enemy[i]->sprite_angle/(M_PI_16);
              } else {
                le_angle=(M_PI_2-Enemy[i]->sprite_angle)/(M_PI_16);
              }
            } else if (Enemy[i]->below_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=16+(M_PI_2+Enemy[i]->sprite_angle)/(M_PI_16);
              } else {
                le_angle=32-(M_PI_2+Enemy[i]->sprite_angle)/(M_PI_16);
              }
            }
        }
        if (le_angle>31) {
          le_angle-=32;
        } 
        if (le_angle<0) {
          le_angle+=32;
        }
        if (le_angle>-1 && le_angle<32) {
          Enemy[i]->current_rot_sprite_angle_id=le_angle;
        }
      }

    }

    Enemy[i]->seed=rand();
    for (k=0;k<Enemy[i]->bullet_shot_num;k++) {
      DrawBullet(hdc,Enemy[i]->bullet_shot_arr[k]);
    }

    //Drawing operations, enemy is living
    if (Enemy[i]->health>0) { //enemy is alive


    //Enemy has died
    } else if (Enemy[i]->health>-1000 && Enemy[i]->health<=0){
      //Add to enemy kill count and buff player stats
      if (Enemy[i]->health>-500) {
        enemy_kills++;
        //Add to player stats after defeat
        if (!IsSpeedBreaking()) {
          if (player.time_breaker_units<player.time_breaker_units_max-2 && !player.time_breaker) {
            player.time_breaker_units+=2;
          }
        } else {
          player.speed+=2;
        }
      }

      Enemy[i]->play_death_snd=TRUE;
      Enemy[i]->health=-99999;
    }


    //enemy Display on screen operations
    /*char debug_txt[16];
    sprintf(debug_txt,"%d",Enemy[i]->current_rot_sprite_angle_id);
    GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y-64,debug_txt,WHITE);*/

    if (/*Enemy[i]->saw_player &&*/ Enemy[i]->within_render_distance) {
      if (Enemy[i]->health>0 && Enemy[i]->damage_taken_timer>0) {
        //percentage 
        c = LTRED;//Highlight(IsInvertedBackground(),LTRED,LTCYAN);
        c2 = WHITE;//Highlight(IsInvertedBackground(),WHITE,BLACK);
        double percentage = Enemy[i]->health/Enemy[i]->max_health;
        double health_length_max=64;
        if (Enemy[i]->species==1 || Enemy[i]->species==3) {
          health_length_max=100;
        }
        if (Enemy[i]->max_health>=150) {
          health_length_max=150;
        } else if (Enemy[i]->max_health>64) {
          health_length_max=Enemy[i]->max_health;
          if (Enemy[i]->species==1 || Enemy[i]->species==3) {
            health_length_max=100;
          }
        }

        double health_length = health_length_max*percentage;
        if (Enemy[i]->species==1 || Enemy[i]->species==3) {
          GrRect(hdc,Enemy[i]->sprite_x-health_length/2,
                    Enemy[i]->sprite_y-58,
                    health_length,
                    4,c);
        } else {
          GrRect(hdc,Enemy[i]->sprite_x-health_length/2,
                    Enemy[i]->sprite_y-26,
                    health_length,
                    4,c);
        }


        if (health_length>32) {
          int print_health=Enemy[i]->health;
          if (is_khmer) {
              wchar_t wtxt[16];
              if (print_health>1) {
                swprintf(wtxt,16,L"%d",print_health);        
              } else {
                swprintf(wtxt,16,L"1");
              }
              int sprite_x_health=(int)Enemy[i]->sprite_x-wcslen(wtxt)*12/2-3;
              if (Enemy[i]->species==1 || Enemy[i]->species==3) {
                if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
                  GrPrintW(hdc,sprite_x_health,Enemy[i]->sprite_y-64,ReplaceToKhmerNum(wtxt),"",c2,16,FALSE,yes_unifont);
                } else {
                  GrPrintW(hdc,sprite_x_health,Enemy[i]->sprite_y-64,ReplaceToKhmerNum(wtxt),"",LTGRAY,16,FALSE,yes_unifont);
                }
              } else {
                if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
                  GrPrintW(hdc,sprite_x_health,Enemy[i]->sprite_y-32,ReplaceToKhmerNum(wtxt),"",c2,16,FALSE,yes_unifont);
                } else {
                  GrPrintW(hdc,sprite_x_health,Enemy[i]->sprite_y-32,ReplaceToKhmerNum(wtxt),"",LTGRAY,16,FALSE,yes_unifont);
                }
              }
          } else {
          char txt[16];
          if (print_health>1) {
            sprintf(txt,"%d",print_health);        
          } else {
            sprintf(txt,"1");
          }
          int sprite_x_health=(int)Enemy[i]->sprite_x-strlen(txt)*8/2;
          if (Enemy[i]->species==1 || Enemy[i]->species==3) {
            if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
              GrPrint(hdc,sprite_x_health,Enemy[i]->sprite_y-64,txt,c2);
            } else {
              GrPrint(hdc,sprite_x_health,Enemy[i]->sprite_y-64,txt,LTGRAY);
            }
          } else {
            if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
              GrPrint(hdc,sprite_x_health,Enemy[i]->sprite_y-32,txt,c2);
            } else {
              GrPrint(hdc,sprite_x_health,Enemy[i]->sprite_y-32,txt,LTGRAY);
            }
          }
          }
        }
      }
    
      /*if (Enemy[i]->web_stuck) {
        GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y+16,"...",WHITE);
      }*/

      int etype=Enemy[i]->type;
      if (Enemy[i]->health>0) {
      switch (Enemy[i]->species) {
        case 0:
        case 2:
        case 4:
          if (etype>-1 && etype<ENEMY_TYPE_NUM) {
          if (Enemy[i]->sprite_timer%2==0) {
            if (Enemy[i]->sprite_timer%4==0) {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y+1,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
            } else {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
            }
          } else {
            if (Enemy[i]->sprite_timer%3==0) {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y-2,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
            } else {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y-1,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
            }
          }
          }
          break;

        case 1: 
        case 3: //កន្ទាទូក
        {
          //DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[Enemy[i]->rotated_sprite_id]->draw_rotated_sprite1[funny],Enemy[i]->last_left);
          //char printfunny[6];
          //sprintf(printfunny,"@%d",funny);
          //GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y+48,printfunny,WHITE);
          int rsid=Enemy[i]->rotated_sprite_id;
          if (rsid!=-1 && etype>-1 && etype<ENEMY_TYPE_NUM) {
          if (Enemy[i]->sprite_in_water && !Enemy[i]->web_stuck) {
            int swim_rot_id=7;
            if (Enemy[i]->species==3) {
              swim_rot_id=9;
            }
            if (Enemy[i]->sprite_timer%8==0) {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
              &EnemyRotatedSprite[rsid].draw_rotated_sprite1[swim_rot_id],Enemy[i]->last_left);
            } else {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
              &EnemyRotatedSprite[rsid].draw_rotated_sprite2[swim_rot_id],Enemy[i]->last_left);
            }
          } else if (Enemy[i]->on_ground_id!=-1) { //on a ground
            if (!Enemy[i]->is_in_ground_edge) {
              if (Enemy[i]->move_to_target || Enemy[i]->knockback_timer>0) { //moving to target and on ground
                bool flip_bool=FALSE;
                if (Enemy[i]->above_ground || Enemy[i]->below_ground) {
                   if (Enemy[i]->above_ground) {
                    flip_bool=Enemy[i]->last_left;
                  } else if (Enemy[i]->below_ground) {
                    flip_bool=Enemy[i]->flip_sprite;
                  }
                  if (Enemy[i]->sprite_timer%8==0) {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  } else {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  }
                } else {
                  if (Enemy[i]->sprite_timer%2==0) { //fly sprite
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
                  } else {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
                  }
                }
              } else { //not moving to target && on ground
                bool flip_bool=FALSE;
                if (Enemy[i]->above_ground) {
                  flip_bool=Enemy[i]->last_left;
                } else if (Enemy[i]->below_ground) {
                  flip_bool=Enemy[i]->flip_sprite;
                }

                if (Enemy[i]->species==3) { 
                  if (Enemy[i]->idle_timer%16<8) {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                    &EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  } else if (Enemy[i]->rotated_xsprite_id!=-1){
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                    &XEnemyRotatedSprite[Enemy[i]->rotated_xsprite_id].draw_rotated_sprite[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  }
                } else {
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                  &EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                }
             }



            } else { //sprite on ground edge
              bool flip_bool=FALSE;
              if (Enemy[i]->above_ground_edge) {
                flip_bool=Enemy[i]->last_left;
              } else if (Enemy[i]->below_ground_edge) {
                flip_bool=Enemy[i]->flip_sprite;
              }

              if (Enemy[i]->sprite_timer%8==0) {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
              } else {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
              }

            }



          } else { //sprite flying
            if (Enemy[i]->sprite_timer%2==0) {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
            } else {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
            }
          }
          }
          break;
        }
      }
      }
    }
  }
}

