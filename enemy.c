
#define STRAIGHT_COST 10
#define DIAGONAL_COST 14

int CalculateDistanceCost(int enemy_id,int a, int b)
{
  int pfi=Enemy[enemy_id]->pathfinding_id;
  if (pfi!=-1) {
  int x_dist=0,y_dist=0,picked=0,remaining=0;
  x_dist=EnemyPathfinding[pfi]->node_x[a]-EnemyPathfinding[pfi]->node_x[b];
  y_dist=EnemyPathfinding[pfi]->node_y[a]-EnemyPathfinding[pfi]->node_y[b];
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
  } else {
    return 0;
  }
}

int smallest_f_cost(int enemy_id)
{
  int pfi=Enemy[enemy_id]->pathfinding_id;
  if (pfi!=-1) {
  int i=0,saved_i=-1,open_node_id=0,id=EnemyPathfinding[pfi]->start_node,smallest=INT_MAX;
  for (i=0;i<EnemyPathfinding[pfi]->open_nodes_num;i++) {//open nodes num is the key to the optimization
    open_node_id=EnemyPathfinding[pfi]->open_nodes[i];
    if (EnemyPathfinding[pfi]->node_fcost[open_node_id]<smallest && EnemyPathfinding[pfi]->node_open[open_node_id]) {
      smallest=EnemyPathfinding[pfi]->node_fcost[open_node_id];
      id=open_node_id;
      saved_i=i;
    }
  }
  if (saved_i!=-1) {
    for (i=saved_i;i<EnemyPathfinding[pfi]->open_nodes_num;i++) {//shifts array left from open_node's pos
      EnemyPathfinding[pfi]->open_nodes[i]=EnemyPathfinding[pfi]->open_nodes[i+1];
    }
    EnemyPathfinding[pfi]->open_nodes_num--;//dynamically scales: open_node becomes closed
  }
  return id;
  } else {
    return 0;
  }
}

void InitEnemyPathfinding(int enemy_id,double target_x,double target_y)
{
  int i=0,j=0,k=0,j2=0,x=0,y=0,
      start_node_x=0,start_node_y=0,
      node_id=0,tmp_node_id=0,
      current_x=Enemy[enemy_id]->x,
      current_y=Enemy[enemy_id]->y;
 //Init enemy fixed
  Enemy[enemy_id]->sprite_timer=0;
  Enemy[enemy_id]->search_timer=0;
  Enemy[enemy_id]->idle_timer=0;
  Enemy[enemy_id]->idling=FALSE;  
  Enemy[enemy_id]->found_target=FALSE;
  Enemy[enemy_id]->trace_target=FALSE;
  Enemy[enemy_id]->move_to_target=FALSE;
  int pfi=Enemy[enemy_id]->pathfinding_id;
  if (pfi!=-1) {
  EnemyPathfinding[pfi]->node_num=MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE;//Enemy[enemy_id]->follow_range*Enemy[enemy_id]->follow_range;
  EnemyPathfinding[pfi]->path_nodes_num=0;
  EnemyPathfinding[pfi]->open_nodes_num=0;//reset to 0
 //set open path nodes to false
  for (i=0;i<MAX_NODE_NUM;i++) {
    EnemyPathfinding[pfi]->path_nodes[i]=0;
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
  EnemyPathfinding[pfi]->species1_solid_num=0;
  for (i=0;i<EnemyPathfinding[pfi]->node_num;i++) {//maximum path finding node num
    EnemyPathfinding[pfi]->node_back[i]=
      EnemyPathfinding[pfi]->node_open[i]=
      EnemyPathfinding[pfi]->node_closed[i]=FALSE;
    if (i<EnemyPathfinding[pfi]->node_num) {
      EnemyPathfinding[pfi]->node_x[i]=x;
      EnemyPathfinding[pfi]->node_y[i]=y;
      node_id=GetGridId(x,y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM); //all nodes      
      if (node_id!=-1) {
        switch (Enemy[enemy_id]->species) {
	      case 0://standard
          case 2:
          case 4:
            EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_solid;
            if (!EnemyPathfinding[pfi]->node_solid[i])
              EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_water;
            if (!EnemyPathfinding[pfi]->node_solid[i])
              EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_fire; //fire check
	        break;

	      case 1:
          case 3:
            if ((Enemy[enemy_id]->force_fall || Enemy[enemy_id]->flying_timer>0) || Enemy[enemy_id]->target_player || Enemy[enemy_id]->on_ground_id==-1) {
              EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_solid;
              if (!EnemyPathfinding[pfi]->node_solid[i] && Enemy[enemy_id]->species==1) //cannot dive into water
                EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_water;
              if (!EnemyPathfinding[pfi]->node_solid[i])
                EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_fire; //fire check
            } else { //inverse solid
              EnemyPathfinding[pfi]->node_solid[i]=!NodeGrid[node_id]->node_solid;
              if (!EnemyPathfinding[pfi]->node_solid[i] && Enemy[enemy_id]->species==1)
                EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_water;
              if (!EnemyPathfinding[pfi]->node_solid[i])
                EnemyPathfinding[pfi]->node_solid[i]=NodeGrid[node_id]->node_fire; //fire check
	          if (!EnemyPathfinding[pfi]->node_solid[i]) { //if TRUE solids
                EnemyPathfinding[pfi]->enemy_species1_solids[EnemyPathfinding[pfi]->species1_solid_num]=i;
	            EnemyPathfinding[pfi]->species1_solid_num++;
              }
            }
            break;
          }
      } else {//out of bounds
        EnemyPathfinding[pfi]->node_solid[i]=TRUE;
      }
    } else {
      EnemyPathfinding[pfi]->node_x[i]=start_node_x;
      EnemyPathfinding[pfi]->node_y[i]=start_node_y;
    }
    EnemyPathfinding[pfi]->node_hcost[i]=0;
    EnemyPathfinding[pfi]->node_gcost[i]=INT_MAX;
    EnemyPathfinding[pfi]->node_fcost[i]=EnemyPathfinding[pfi]->node_gcost[i]+EnemyPathfinding[pfi]->node_hcost[i];
    EnemyPathfinding[pfi]->node_parent[i]=-1;
    x+=NODE_SIZE;
    if (x>start_node_x-NODE_SIZE+MAX_FOLLOW_RANGE*NODE_SIZE) {
      x=start_node_x;
      y+=NODE_SIZE;
    }
  }
  if (Enemy[enemy_id]->species==1 || Enemy[enemy_id]->species==3) {//species 1
    for (i=0;i<EnemyPathfinding[pfi]->species1_solid_num;i++) {
      k=EnemyPathfinding[pfi]->enemy_species1_solids[i];
      x=EnemyPathfinding[pfi]->node_x[k]-EnemyPathfinding[pfi]->node_x[0];
      for (j2=0;j2<5;j2++) {
        if (j2<3) { //0:-2, 1:-1 ,2:0
          x+=((j2-2)*NODE_SIZE);
    	} else {//3:2, 4:3, 
          x+=((j2-1)*NODE_SIZE);
    	}
        x+=((j2-2)*NODE_SIZE);
        for (j=0;j<8;j++) { //adjacent solid nodes, turn into non-solids
          y=EnemyPathfinding[pfi]->node_y[k]-EnemyPathfinding[pfi]->node_y[0];
          y+=((j-3)*NODE_SIZE);
          node_id=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[pfi]->node_num);
          if (node_id!=-1) {
	        EnemyPathfinding[pfi]->node_solid[node_id]=FALSE;
	      }
        }
      }
    }
    for (i=0;i<EnemyPathfinding[pfi]->species1_solid_num;i++) { //turn back to solids
      k=EnemyPathfinding[pfi]->enemy_species1_solids[i];
      for (j=0;j<2;j++) {
        x=EnemyPathfinding[pfi]->node_x[k]-EnemyPathfinding[pfi]->node_x[0];
        y=EnemyPathfinding[pfi]->node_y[k]-EnemyPathfinding[pfi]->node_y[0]+NODE_SIZE*j;
        node_id=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[pfi]->node_num);
        if (node_id!=-1) {
          EnemyPathfinding[pfi]->node_solid[node_id]=TRUE;
        }
      }
    } 
  }
  //Set Start Node
  x=current_x-EnemyPathfinding[pfi]->node_x[0];
  y=current_y-EnemyPathfinding[pfi]->node_y[0];
  EnemyPathfinding[pfi]->start_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[pfi]->node_num);
  for (i=0;i<MAX_NODE_NUM;i++) {//reset open_nodes array
    EnemyPathfinding[pfi]->open_nodes[i]=EnemyPathfinding[pfi]->start_node;
  }
  //Set Target
  x=target_x-EnemyPathfinding[pfi]->node_x[0];
  y=target_y-EnemyPathfinding[pfi]->node_y[0];
  EnemyPathfinding[pfi]->end_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[pfi]->node_num);
  //Initiate start
  EnemyPathfinding[pfi]->current_node=EnemyPathfinding[pfi]->start_node;
  EnemyPathfinding[pfi]->node_gcost[EnemyPathfinding[pfi]->start_node]=0;
  EnemyPathfinding[pfi]->node_hcost[EnemyPathfinding[pfi]->start_node]=CalculateDistanceCost(0,EnemyPathfinding[pfi]->start_node,EnemyPathfinding[pfi]->end_node);
  EnemyPathfinding[pfi]->node_fcost[EnemyPathfinding[pfi]->start_node]=EnemyPathfinding[pfi]->node_gcost[EnemyPathfinding[pfi]->start_node]+EnemyPathfinding[pfi]->node_hcost[EnemyPathfinding[pfi]->start_node];
  EnemyPathfinding[pfi]->node_open[EnemyPathfinding[pfi]->start_node]=TRUE;
  }
}

void EnemyPathFinding(int enemy_id)
{
  int x=0,y=0,i=0,neighbour_id=0,TGCost=0;
  bool allow_act=FALSE;
  int pfi=Enemy[enemy_id]->pathfinding_id;
  if (pfi!=-1) {
  if (!Enemy[enemy_id]->found_target) {//target not found
    EnemyPathfinding[pfi]->current_node=smallest_f_cost(enemy_id);
    if (EnemyPathfinding[pfi]->node_x[EnemyPathfinding[pfi]->current_node]==EnemyPathfinding[pfi]->node_x[EnemyPathfinding[pfi]->end_node] &&
        EnemyPathfinding[pfi]->node_y[EnemyPathfinding[pfi]->current_node]==EnemyPathfinding[pfi]->node_y[EnemyPathfinding[pfi]->end_node]) {
      EnemyPathfinding[pfi]->current_node=EnemyPathfinding[pfi]->end_node;
      Enemy[enemy_id]->found_target=TRUE;
      EnemyPathfinding[pfi]->path_nodes[0]=EnemyPathfinding[pfi]->current_node;
      EnemyPathfinding[pfi]->path_nodes_num=1;
    }
    EnemyPathfinding[pfi]->node_open[EnemyPathfinding[pfi]->current_node]=FALSE;
    EnemyPathfinding[pfi]->node_closed[EnemyPathfinding[pfi]->current_node]=TRUE;
   //set neighbours
    for (i=0;i<8;i++) {
      x=EnemyPathfinding[pfi]->node_x[EnemyPathfinding[pfi]->current_node]-EnemyPathfinding[pfi]->node_x[0];
      y=EnemyPathfinding[pfi]->node_y[EnemyPathfinding[pfi]->current_node]-EnemyPathfinding[pfi]->node_y[0];
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
        EnemyPathfinding[pfi]->node_neighbour[i]=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[pfi]->node_num);
      } else { //not within range
        EnemyPathfinding[pfi]->node_neighbour[i]=EnemyPathfinding[pfi]->start_node;
      }
    }
    for (i=0;i<8;i++) {
      neighbour_id=EnemyPathfinding[pfi]->node_neighbour[i];
      allow_act=FALSE;
      if (EnemyPathfinding[pfi]->current_node!=EnemyPathfinding[pfi]->start_node) {//no going through node corners
	if (EnemyPathfinding[pfi]->node_solid[EnemyPathfinding[pfi]->node_neighbour[1]] || EnemyPathfinding[pfi]->node_solid[EnemyPathfinding[pfi]->node_neighbour[4]]) { 
          if (EnemyPathfinding[pfi]->node_solid[EnemyPathfinding[pfi]->node_neighbour[6]] || EnemyPathfinding[pfi]->node_solid[EnemyPathfinding[pfi]->node_neighbour[7]]) {
	    allow_act=TRUE;
	  }
	}
      }
      if (EnemyPathfinding[pfi]->node_solid[neighbour_id] || allow_act) {//corner or solid
        EnemyPathfinding[pfi]->node_closed[neighbour_id]=TRUE;
      }
      if (!EnemyPathfinding[pfi]->node_closed[neighbour_id] &&
	  !EnemyPathfinding[pfi]->node_solid[neighbour_id]) {
        TGCost=EnemyPathfinding[pfi]->node_gcost[EnemyPathfinding[pfi]->current_node]+CalculateDistanceCost(enemy_id,EnemyPathfinding[pfi]->current_node,neighbour_id);
        if (TGCost<EnemyPathfinding[pfi]->node_gcost[neighbour_id]) {
          EnemyPathfinding[pfi]->node_parent[neighbour_id]=EnemyPathfinding[pfi]->current_node;
          EnemyPathfinding[pfi]->node_gcost[neighbour_id]=TGCost;
          EnemyPathfinding[pfi]->node_hcost[neighbour_id]=CalculateDistanceCost(enemy_id,neighbour_id,EnemyPathfinding[pfi]->end_node);
          EnemyPathfinding[pfi]->node_fcost[neighbour_id]=EnemyPathfinding[pfi]->node_gcost[neighbour_id]+EnemyPathfinding[pfi]->node_hcost[neighbour_id];
          if (!EnemyPathfinding[pfi]->node_open[neighbour_id]) {//not open
            EnemyPathfinding[pfi]->node_open[neighbour_id]=TRUE; //open it
            EnemyPathfinding[pfi]->open_nodes[EnemyPathfinding[pfi]->open_nodes_num]=neighbour_id;//enemy_open_nodes[self_open_nodes_num] = neighbor_id
            EnemyPathfinding[pfi]->open_nodes_num++;//this is the optimization
	  }
        }
      }
    }
  } else if (!Enemy[enemy_id]->trace_target) {//target has been found (Enemy[enemy_id]->found_target),trace back
    Enemy[enemy_id]->search_timer=0;
    if (EnemyPathfinding[pfi]->start_node==EnemyPathfinding[pfi]->end_node) {
      Enemy[enemy_id]->idling=TRUE;
    } else {
      if (EnemyPathfinding[pfi]->node_x[EnemyPathfinding[pfi]->current_node]==EnemyPathfinding[pfi]->node_x[EnemyPathfinding[pfi]->start_node] &&
          EnemyPathfinding[pfi]->node_y[EnemyPathfinding[pfi]->current_node]==EnemyPathfinding[pfi]->node_y[EnemyPathfinding[pfi]->start_node]) {
        EnemyPathfinding[pfi]->path_nodes_num--;
        Enemy[enemy_id]->search_target=FALSE;
        Enemy[enemy_id]->trace_target=TRUE;
        Enemy[enemy_id]->move_to_target=TRUE;
      } else {
        EnemyPathfinding[pfi]->node_back[EnemyPathfinding[pfi]->current_node]=TRUE;
        EnemyPathfinding[pfi]->path_nodes[EnemyPathfinding[pfi]->path_nodes_num]
         =EnemyPathfinding[pfi]->current_node
         =EnemyPathfinding[pfi]->node_parent[EnemyPathfinding[pfi]->current_node];
        EnemyPathfinding[pfi]->path_nodes_num++;
      }
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
    double distl=34;

    if (Enemy[enemy_id]->species==1 && Enemy[enemy_id]->health<=0) {
      if  (height_from_ground<0) {
        Enemy[enemy_id]->force_fall=TRUE;
        Enemy[enemy_id]->draw_falling=FALSE;
      }
      height_from_ground=-abs(height_from_ground);
    }

    Enemy[enemy_id]->draw_falling=FALSE;

    //GROUND EDGE
    int ground_edge_id=-1;
    if (Enemy[enemy_id]->is_in_ground_edge) {
      ground_edge_id=Enemy[enemy_id]->saved_ground_id;
    } else {
      ground_edge_id=ground_id;
    }
    //EnemyAntActOnGroundEdge(enemy_id,TRUE);
    if (ground_edge_id!=-1) {
      edge_1_dist=GetDistance(Enemy[enemy_id]->x,Enemy[enemy_id]->y,Ground[ground_edge_id]->x1,Ground[ground_edge_id]->y1);
      edge_2_dist=GetDistance(Enemy[enemy_id]->x,Enemy[enemy_id]->y,Ground[ground_edge_id]->x2,Ground[ground_edge_id]->y2);
      if (edge_1_dist<=distl || edge_2_dist<=distl) {
        if (!Enemy[enemy_id]->is_in_ground_edge) {
          if (ground_id!=-1) {
            Enemy[enemy_id]->saved_ground_id=ground_id;
            Enemy[enemy_id]->on_ground_edge_id=ground_id;
            ground_edge_id=ground_id;
          }
          Enemy[enemy_id]->is_in_ground_edge=TRUE;
        }
        if (edge_1_dist<=distl && edge_1_dist>=0 && ground_edge_id!=-1) {
          ///Enemy[enemy_id]->x+=cos(edge_angle);
          //Enemy[enemy_id]->y+=sin(edge_angle);
          //Enemy[enemy_id]->draw_falling=FALSE;
          edge_angle=GetCosAngle(Enemy[enemy_id]->x-Ground[ground_edge_id]->x1,edge_1_dist);
          if (Enemy[enemy_id]->health>0) {
            Enemy[enemy_id]->angle=edge_angle;
          }
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
        } else if (edge_2_dist<=distl && edge_2_dist>=0 && ground_edge_id!=-1) {
          //Enemy[enemy_id]->x+=cos(edge_angle);
          //Enemy[enemy_id]->y+=sin(edge_angle);
          //Enemy[enemy_id]->draw_falling=FALSE;
          edge_angle=GetCosAngle(Enemy[enemy_id]->x-Ground[ground_edge_id]->x2,edge_2_dist);
          if (Enemy[enemy_id]->health>0) {
            Enemy[enemy_id]->angle=edge_angle;
          }
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
        if (!(Enemy[enemy_id]->species>=5 && Enemy[enemy_id]->species<=7)) { 
          if (Enemy[enemy_id]->in_air_timer<500) {
            Enemy[enemy_id]->in_air_timer+=2;//=2;
          }
          Enemy[enemy_id]->saved_ground_id=-1;
          Enemy[enemy_id]->on_ground_edge_id=-1;
          Enemy[enemy_id]->is_in_ground_edge=FALSE;
        }
      }
    } else {
      if (!(Enemy[enemy_id]->species>=5 && Enemy[enemy_id]->species<=7)) { 
        if (Enemy[enemy_id]->in_air_timer<500) {
          Enemy[enemy_id]->in_air_timer+=2;//=2;
        }
        Enemy[enemy_id]->saved_ground_id=-1;
        Enemy[enemy_id]->on_ground_edge_id=-1;
        Enemy[enemy_id]->is_in_ground_edge=FALSE;
      }
    }
    
    if (ground_id>=GROUND_NUM && Enemy[enemy_id]->health>0) { //eating      
      Ground[ground_id]->health-=(1+Enemy[enemy_id]->bullet_damage/2);
    }

    //FOR VISUALS
    if (!Enemy[enemy_id]->is_in_ground_edge && abs(height_from_ground)<=41) {
      if (ground_id!=-1) {
        Enemy[enemy_id]->in_air_timer=0;
        Enemy[enemy_id]->flying_timer=0;
        Enemy[enemy_id]->force_fall=FALSE;
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




void LargeEnemySpriteTimer(int enemy_id)
{
  if (Enemy[enemy_id]->species==1 || Enemy[enemy_id]->species==3) {
    Enemy[enemy_id]->sprite_timer++;
    if (Enemy[enemy_id]->sprite_timer>16) {
      Enemy[enemy_id]->sprite_timer=0;
    }
  }
}


void EnemyGravity(int enemy_id,int gr)
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
  if ((Enemy[enemy_id]->on_ground_id==-1 && tmp_==-1) || Enemy[enemy_id]->force_fall) {//not on ground
    if (!is_in_solid1 && !is_in_solid2) {
      if (!Enemy[enemy_id]->in_water) {
        Enemy[enemy_id]->y+=1; //falling down
        if (Enemy[enemy_id]->y>=MAP_HEIGHT-4) {
          Enemy[enemy_id]->true_dead=TRUE;
          Enemy[enemy_id]->health--;
        }
        if (Enemy[enemy_id]->health>0) {
          Enemy[enemy_id]->draw_falling=TRUE;
        }

        if (Enemy[enemy_id]->flying_timer==0 && Enemy[enemy_id]->health>0) { 
          if (Enemy[enemy_id]->species==1) {
              if (!(abs(RandNum(0,500,&Enemy[enemy_id]->flying_rng_i,Enemy[enemy_id]->seed))<40)/*90*/) { //small chance cockroach falls while flying
                Enemy[enemy_id]->flying_timer=5;
                Enemy[enemy_id]->draw_falling=FALSE;
              } else {
                Enemy[enemy_id]->force_fall=TRUE;
                Enemy[enemy_id]->draw_falling=FALSE;
              }
          } else if (Enemy[enemy_id]->species==3) {
              if (abs(RandNum(0,100,&Enemy[enemy_id]->flying_rng_i,Enemy[enemy_id]->seed))==5) {
                Enemy[enemy_id]->flying_timer=300;
                Enemy[enemy_id]->draw_falling=FALSE;
              } 
          }
        }


      } else if (gr==0 && Enemy[enemy_id]->species!=3) {
        Enemy[enemy_id]->flying_timer=0;
        Enemy[enemy_id]->force_fall=FALSE;
        Enemy[enemy_id]->y-=1; //floating up
      }
    }
    if (Enemy[enemy_id]->in_air_timer<50) {
      Enemy[enemy_id]->in_air_timer+=2;//2;
    }
  } 
}


void EnemyMove(int enemy_id)
{
  int pfi=Enemy[enemy_id]->pathfinding_id;
  if (pfi!=-1) {
  int path_node_arr_id=EnemyPathfinding[pfi]->path_nodes_num-1,
      path_node_id=EnemyPathfinding[pfi]->path_nodes[path_node_arr_id];
  double path_node_center_x=-1;
  double path_node_center_y=-1;
  if (path_node_id!=-1) {
    path_node_center_x=EnemyPathfinding[pfi]->node_x[path_node_id]+NODE_SIZE/2;
    path_node_center_y=EnemyPathfinding[pfi]->node_y[path_node_id]+NODE_SIZE/2;
  }

  int gid=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,2,2);


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
      Enemy[enemy_id]->draw_falling=FALSE;
      Enemy[enemy_id]->y-=Enemy[enemy_id]->speed;
    }



  if (path_node_center_y-1<=Enemy[enemy_id]->y && Enemy[enemy_id]->y<=path_node_center_y+1 &&
      path_node_center_x-1<=Enemy[enemy_id]->x && Enemy[enemy_id]->x<=path_node_center_x+1) {
    EnemyPathfinding[pfi]->path_nodes_num--;
    if (path_node_arr_id<=0 || gid!=-1) { //all nodes followed
      Enemy[enemy_id]->idling=TRUE;
      Enemy[enemy_id]->move_to_target=FALSE;
    }
  }
  }
}

void EnemyTargetPlayer(int i)
{
  int target_node=0;
  double target_x=0,target_y=0;
  int pfi=Enemy[i]->pathfinding_id;
  if (pfi!=-1) {
  Enemy[i]->idling=FALSE;
  Enemy[i]->search_target=TRUE;
  Enemy[i]->idle_timer=0;
  Enemy[i]->search_timer=0;
  if (player.print_current_above || player.print_current_below || player.above_ground_edge || player.below_ground_edge) {
    target_x=player.above_head_x;
    target_y=player.above_head_y;
  } else {
    target_x=player.x;
    target_y=player.y;
  }
  target_node=GetGridId(target_x-EnemyPathfinding[pfi]->node_x[0],
			target_y-EnemyPathfinding[pfi]->node_y[0],
                        MAX_FOLLOW_RANGE*NODE_SIZE,
			NODE_SIZE,
			EnemyPathfinding[pfi]->node_num);
  if ((Enemy[i]->species==1 || Enemy[i]->species==3) &&
      EnemyPathfinding[pfi]->node_solid[target_node]) {
    //target_x=player.above_x2;
    //target_y=player.above_y2;
    target_x=player.above_head_x;
    target_y=player.above_head_y;
    target_node=GetGridId(target_x-EnemyPathfinding[pfi]->node_x[0],
			  target_y-EnemyPathfinding[pfi]->node_y[0],
                          MAX_FOLLOW_RANGE*NODE_SIZE,
			  NODE_SIZE,
			  EnemyPathfinding[pfi]->node_num);      
  }
  if (EnemyPathfinding[pfi]->node_solid[target_node] ||//Total ignore player (player is hidden)
      Enemy[i]->in_unchase_range //dont chase player if its in unchase range
    ) {
    if ((Enemy[i]->species==0 || Enemy[i]->species==2 ||  Enemy[i]->species==4) && EnemyPathfinding[pfi]->node_solid[target_node]) {
      Enemy[i]->ignore_player=TRUE; //dont ignore if player is within chase range
    }
    Enemy[i]->target_player=FALSE;
    Enemy[i]->idling=FALSE;
    Enemy[i]->search_target=TRUE;
    Enemy[i]->idle_timer=0;
    int seed1=Enemy[i]->seed;
    int seed2=Enemy[i]->seed*2;
    if (!free_will) {seed1=seed2=-1;}
    target_x=Enemy[i]->x+RandNum(-MAX_FOLLOW_RANGE/4*NODE_SIZE,abs(MAX_FOLLOW_RANGE/4*NODE_SIZE),&Enemy[i]->target_player_rng_i,seed1);
    target_y=Enemy[i]->y+RandNum(-MAX_FOLLOW_RANGE/4*NODE_SIZE,abs(MAX_FOLLOW_RANGE/4*NODE_SIZE),&Enemy[i]->target_player_rng_i,seed2);
    target_node=GetGridId(target_x-EnemyPathfinding[pfi]->node_x[0],
			target_y-EnemyPathfinding[pfi]->node_y[0],
                        MAX_FOLLOW_RANGE*NODE_SIZE,
			NODE_SIZE,
			EnemyPathfinding[pfi]->node_num);
  } else {
    Enemy[i]->target_player=TRUE;
    Enemy[i]->ignore_player=FALSE;
  }
  /*if (Enemy[i]->species==1 || Enemy[i]->species==3) {
    int rand_dice=RandNum(0,100,&Enemy[i]->rng_i);
    if (rand_dice<40) {
      Enemy[i]->flying_timer=100;
    }
  }*/
  InitEnemyPathfinding(i,target_x,target_y);
  }
}

void EnemyLOSAct(int i)
{
  int los_on_ground_id=0,j=0;
  double x1=0,x2=0,y1=0,y2=0,bullet_gradient=0;
  bool allow_act=FALSE;
  for (j=0;j</*10*/15;j++) {//LOS Speed
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
        Enemy[i]->last_seen_timer=200;
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
  if (IsOutOfBounds(Enemy[i]->x,Enemy[i]->y,5,MAP_WIDTH,MAP_HEIGHT) /*|| Enemy[i]->is_in_ground_edge*/) {
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
    if (Enemy[i]->species==1) { //knock cockroach off
      Enemy[i]->force_fall=TRUE;
      Enemy[i]->draw_falling=FALSE;
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
  if (Enemy[i]->hurt_snd_timer>0)
    Enemy[i]->hurt_snd_timer--;

  if (Enemy[i]->play_death_snd && !back_to_menu) {
    PlayMemSnd(&channelSoundEffect[1],&channelSoundEffectCache[1],TRUE,0); 
    Enemy[i]->play_death_snd=FALSE;
  }
}



int EnemyActDazzle(int i, int slash_t) 
{
  Enemy[i]->saw_player=FALSE;
  Enemy[i]->ignore_player=TRUE;
  Enemy[i]->target_player=FALSE;
  /*if (!Enemy[i]->move_to_target) {
  }*/
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


    if (Enemy[i]->species==0 || Enemy[i]->species==2 || Enemy[i]->species==4) { //3*3
      if (!Enemy[i]->web_stuck) {
        nx=Enemy[i]->x-NODE_SIZE+GetXFromId(Enemy[i]->current_ngid_n,3)*NODE_SIZE; //     1 2 3   
        ny=Enemy[i]->y-NODE_SIZE+GetYFromId(Enemy[i]->current_ngid_n,3)*NODE_SIZE; //     4 5 6  
        if (Enemy[i]->on_ground_id>=GROUND_NUM) {
          Enemy[i]->flag_web_stuck=TRUE;
        }
      } else { //5*5
        nx=Enemy[i]->x-NODE_SIZE*2+GetXFromId(Enemy[i]->current_ngid_n,5)*NODE_SIZE; //   1 2 3 4 5  
        ny=Enemy[i]->y-NODE_SIZE*2+GetYFromId(Enemy[i]->current_ngid_n,5)*NODE_SIZE; //   6 7 8 9 A 
      }
    } else if ((Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
      nx=Enemy[i]->x;   
      ny=Enemy[i]->y;
      if (Enemy[i]->on_ground_id>=GROUND_NUM) {
        Enemy[i]->flag_web_stuck=TRUE;
      }
    } else if (Enemy[i]->species==1 || Enemy[i]->species==3) { //7*7
      nx=Enemy[i]->x-NODE_SIZE*3+GetXFromId(Enemy[i]->current_ngid_n,7)*NODE_SIZE; //0 1 2 3 4 5 6 
      ny=Enemy[i]->y-NODE_SIZE*3+GetYFromId(Enemy[i]->current_ngid_n,7)*NODE_SIZE; //7 8 9 A B C D
      if (Enemy[i]->on_ground_id>=GROUND_NUM) {
        Enemy[i]->flag_web_stuck=TRUE;
      }
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
    } else if (Enemy[i]->species>=5 && Enemy[i]->species<=7) {
      if (Enemy[i]->current_ngid_n==1) {
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



void EnemyActSuffocate(int i)
{
  int nx,ny,sub_tmp_ngid;
  if (Enemy[i]->suffocate_timer<450) {
    //for (int j=0;j<9;j++) {
    nx=Enemy[i]->x-NODE_SIZE+GetXFromId(/*j*/Enemy[i]->current_suffocate_ngid_n,3)*NODE_SIZE; //     1 2 3   
    ny=Enemy[i]->y-NODE_SIZE+GetYFromId(/*j*/Enemy[i]->current_suffocate_ngid_n,3)*NODE_SIZE; //     4 5 6  
    sub_tmp_ngid=GetGridId(nx,ny,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);
    if (sub_tmp_ngid!=-1) {
      if (NodeGrid[sub_tmp_ngid]->node_solid && NodeGrid[sub_tmp_ngid]->non_web) { //solid
        Enemy[i]->current_suffocate_ngid_n++;
        if (/*j==8*/Enemy[i]->current_suffocate_ngid_n>=9) {
          Enemy[i]->current_suffocate_ngid_n=0; //reset search surrounding
          //Enemy[i]->suffocate_timer++;
        }
        Enemy[i]->suffocate_timer++;
      } else { //non-solid
        //Enemy[i]->current_suffocate_ngid_n=0; //reset search surrounding
        Enemy[i]->suffocate_timer=0;
        /*if (Enemy[i]->suffocate_timer<0) {
          Enemy[i]->suffocate_timer=0;
        }*/
        //break;
      }
    }
    //}
  } /*else {
    Enemy[i]->health--;
    if (Enemy[i]->health<=0) {
      Enemy[i]->true_dead=TRUE;
    }
  }*/
}



void EnemyAntActOnGround(int i,double height_from_e_x,int ground_id,bool clockwise)
{ 
  if (ground_id!=-1) {
    Enemy[i]->angle=Ground[ground_id]->angle;
    if (-10<height_from_e_x && height_from_e_x<10) {
       if (ground_id>=GROUND_NUM) {
         if (Ground[ground_id]->health<=0) {//completely destroy web at 0 health (can be regained after '4')
              DestroyGround(ground_id); 
              player.cdwebs[player.cdweb_pos]=ground_id;
              player.cdweb_pos++;
              if (player.cdweb_pos>=player.max_web_num) {
                player.cdweb_pos=0;
              }
              player.cdweb_num++;
         } else {
           Ground[ground_id]->health-=Enemy[i]->bullet_damage;
         }
       }
    }
    if (0<height_from_e_x && height_from_e_x<10) {
       Enemy[i]->in_air_timer=0;
       Enemy[i]->flying_timer=0;
       Enemy[i]->force_fall=FALSE;
       Enemy[i]->draw_falling=FALSE;
       Enemy[i]->on_ground_timer=20;
       Enemy[i]->sprite_angle=Ground[ground_id]->angle;
       if (clockwise) {
         Enemy[i]->last_left=FALSE;
       } else {
         Enemy[i]->last_left=TRUE;
       }
       if(Enemy[i]->last_left) {
         Enemy[i]->flip_sprite=FALSE;
       } else {
         Enemy[i]->flip_sprite=TRUE;
       }
       Enemy[i]->above_ground=TRUE;
       Enemy[i]->below_ground=FALSE;
      if (height_from_e_x<5)  {
        Enemy[i]->x+=(-cos(Ground[ground_id]->angle+M_PI_2)*2);
        Enemy[i]->y+=(-sin(Ground[ground_id]->angle+M_PI_2)*2);
      } else {
        if (clockwise) {
          Enemy[i]->x+=cos(Ground[ground_id]->angle)*Enemy[i]->speed;
          Enemy[i]->y+=sin(Ground[ground_id]->angle)*Enemy[i]->speed;
        } else {
          Enemy[i]->x+=-cos(Ground[ground_id]->angle)*Enemy[i]->speed;
          Enemy[i]->y+=-sin(Ground[ground_id]->angle)*Enemy[i]->speed;
        }
      }
    } else if (-10<height_from_e_x && height_from_e_x<=0) {
       Enemy[i]->in_air_timer=0;
       Enemy[i]->flying_timer=0;
       Enemy[i]->force_fall=FALSE;
       Enemy[i]->draw_falling=FALSE;
       Enemy[i]->on_ground_timer=20;
       Enemy[i]->sprite_angle=-Ground[ground_id]->angle;
       if (clockwise) {
         Enemy[i]->last_left=TRUE;
       } else {
         Enemy[i]->last_left=FALSE;
       }
       if(Enemy[i]->last_left) {
         Enemy[i]->flip_sprite=FALSE;
       } else {
         Enemy[i]->flip_sprite=TRUE;
       }
       Enemy[i]->above_ground=FALSE;
       Enemy[i]->below_ground=TRUE;

      if (height_from_e_x>-5)  {
        Enemy[i]->x+=(-cos(Ground[ground_id]->angle-M_PI_2)*2); //go outwards 
        Enemy[i]->y+=(-sin(Ground[ground_id]->angle-M_PI_2)*2);
      } else {
        if (clockwise) {
          Enemy[i]->x+=-cos(Ground[ground_id]->angle)*Enemy[i]->speed;
          Enemy[i]->y+=-sin(Ground[ground_id]->angle)*Enemy[i]->speed;
        } else {
          Enemy[i]->x+=cos(Ground[ground_id]->angle)*Enemy[i]->speed;
          Enemy[i]->y+=sin(Ground[ground_id]->angle)*Enemy[i]->speed;
        }
      }
    } else {
      Enemy[i]->above_ground=FALSE;
      Enemy[i]->below_ground=FALSE;
    }
  }
}



void EnemyAntActOnGroundEdge(int i,bool clockwise)
{

  double edge_dist1;
  double edge_dist2;
  double edge_angle;
  double distl=20;
  if (!Enemy[i]->is_in_ground_edge) {
        if (Enemy[i]->on_ground_id!=-1) {
          edge_dist1=GetDistance(Enemy[i]->x,Enemy[i]->y,Ground[Enemy[i]->on_ground_id]->x1,Ground[Enemy[i]->on_ground_id]->y1); //left edge
          edge_dist2=GetDistance(Enemy[i]->x,Enemy[i]->y,Ground[Enemy[i]->on_ground_id]->x2,Ground[Enemy[i]->on_ground_id]->y2); //right edge
          if (edge_dist1<distl || edge_dist2<distl) {
            if (edge_dist1<distl) {
              Enemy[i]->is_in_left_ground_edge=TRUE;
              Enemy[i]->is_in_right_ground_edge=FALSE;
            } else if (edge_dist2<distl) {
              Enemy[i]->is_in_left_ground_edge=FALSE;
              Enemy[i]->is_in_right_ground_edge=TRUE;
            }
            Enemy[i]->on_ground_edge_id=Enemy[i]->on_ground_id;
            Enemy[i]->is_in_ground_edge=TRUE;
          }
        }
      } else { //on ground edge id present
        Enemy[i]->on_ground_timer=20;
        if (Enemy[i]->is_in_left_ground_edge) {
          edge_dist1=GetDistance(Enemy[i]->x,Enemy[i]->y,Ground[Enemy[i]->on_ground_edge_id]->x1,Ground[Enemy[i]->on_ground_edge_id]->y1); //left edge
          edge_angle=GetCosAngle(Enemy[i]->x-Ground[Enemy[i]->on_ground_edge_id]->x1,edge_dist1);
          Enemy[i]->sprite_angle=edge_angle;
          if (Enemy[i]->y<Ground[Enemy[i]->on_ground_edge_id]->y1) { //above pivot
              Enemy[i]->above_ground_edge=TRUE;
              Enemy[i]->below_ground_edge=FALSE;
              if (clockwise) { //clockwize
                Enemy[i]->x+=(cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=FALSE;
              } else { //anticlockwize
                Enemy[i]->x+=(-cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(-sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=TRUE;
              }

              if(Enemy[i]->last_left) {
                Enemy[i]->flip_sprite=FALSE;
              } else {
                Enemy[i]->flip_sprite=TRUE;
              }

          } else { //below pivot
              Enemy[i]->above_ground_edge=FALSE;
              Enemy[i]->below_ground_edge=TRUE;
              if (clockwise) { //clockwize
                Enemy[i]->x+=(-cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=TRUE;
              } else { //anticlockwize
                Enemy[i]->x+=(cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(-sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=FALSE;
              }

              if(Enemy[i]->last_left) {
                Enemy[i]->flip_sprite=FALSE;
              } else {
                Enemy[i]->flip_sprite=TRUE;
              }

          }
          if (edge_dist1>distl+1) {
            Enemy[i]->on_ground_edge_id=-1;
            Enemy[i]->is_in_ground_edge=FALSE;
            Enemy[i]->is_in_left_ground_edge=FALSE;
            Enemy[i]->is_in_right_ground_edge=FALSE;
            Enemy[i]->above_ground_edge=FALSE;
            Enemy[i]->below_ground_edge=FALSE;
            if (Enemy[i]->species==7) {
              if (RandNum(0,30,&Enemy[i]->rng_i,Enemy[i]->seed)==1) {
                Enemy[i]->is_clockwize=!Enemy[i]->is_clockwize;
              }
            }
          }
        } else if (Enemy[i]->is_in_right_ground_edge) {
          edge_dist2=GetDistance(Enemy[i]->x,Enemy[i]->y,Ground[Enemy[i]->on_ground_edge_id]->x2,Ground[Enemy[i]->on_ground_edge_id]->y2); //right edge
          edge_angle=GetCosAngle(Enemy[i]->x-Ground[Enemy[i]->on_ground_edge_id]->x2,edge_dist2);
          Enemy[i]->sprite_angle=edge_angle;
          if (Enemy[i]->y<Ground[Enemy[i]->on_ground_edge_id]->y2) { //above pivot
              Enemy[i]->above_ground_edge=TRUE;
              Enemy[i]->below_ground_edge=FALSE;
              if (clockwise) { //clockwize
                Enemy[i]->x+=(cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=FALSE;
              } else { //anticlockwize
                Enemy[i]->x+=(-cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(-sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=TRUE;
              }

              if(Enemy[i]->last_left) {
                Enemy[i]->flip_sprite=FALSE;
              } else {
                Enemy[i]->flip_sprite=TRUE;
              }

          } else { //below pivot
              Enemy[i]->above_ground_edge=FALSE;
              Enemy[i]->below_ground_edge=TRUE;
              if (clockwise) { //clockwize
                Enemy[i]->x+=(-cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=TRUE;
              } else { //anticlockwize
                Enemy[i]->x+=(cos(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->y+=(-sin(-edge_angle+M_PI_2)*0.5);
                Enemy[i]->last_left=FALSE;
              }

              if(Enemy[i]->last_left) {
                Enemy[i]->flip_sprite=FALSE;
              } else {
                Enemy[i]->flip_sprite=TRUE;
              }

          }
          if (edge_dist2>distl+1) {
            Enemy[i]->on_ground_edge_id=-1;
            Enemy[i]->is_in_ground_edge=FALSE;
            Enemy[i]->is_in_left_ground_edge=FALSE;
            Enemy[i]->is_in_right_ground_edge=FALSE;
            Enemy[i]->above_ground_edge=FALSE;
            Enemy[i]->below_ground_edge=FALSE;
            if (Enemy[i]->species==7) {
            if (RandNum(0,30,&Enemy[i]->rng_i,Enemy[i]->seed)==1) {
              Enemy[i]->is_clockwize=!Enemy[i]->is_clockwize;
            }
            }
          }
        }
  }
  

  if (Enemy[i]->on_ground_id!=-1 && Enemy[i]->on_ground_id!=Enemy[i]->on_ground_edge_id) {
    Enemy[i]->on_ground_edge_id=Enemy[i]->on_ground_id;
    Enemy[i]->is_in_ground_edge=FALSE;
    Enemy[i]->is_in_left_ground_edge=FALSE;
    Enemy[i]->is_in_right_ground_edge=FALSE;
    Enemy[i]->above_ground_edge=FALSE;
    Enemy[i]->below_ground_edge=FALSE;
    if (Enemy[i]->species==7) {
    if (RandNum(0,30,&Enemy[i]->rng_i,Enemy[i]->seed)==1) {
      Enemy[i]->is_clockwize=!Enemy[i]->is_clockwize;
    }
    }
  }
}



void EnemyAntGravity(int enemy_id)
{
  if (Enemy[enemy_id]->on_ground_id==-1) {//not on ground
    if (!Enemy[enemy_id]->in_water) {
      if (Enemy[enemy_id]->y<MAP_HEIGHT-3) {
        Enemy[enemy_id]->y+=1; //falling down
      } else {
        Enemy[enemy_id]->true_dead=TRUE;
        Enemy[enemy_id]->health--;
      }
    } else {
      if (Enemy[enemy_id]->y>3) {
        Enemy[enemy_id]->y-=1; //floating up
      }
    }
  } 
}

void EnemyAntActGravity(int i,int slash_time_i)
{
  if (Enemy[i]->on_ground_id==-1 && !Enemy[i]->is_in_ground_edge) {
    Enemy[i]->above_ground=FALSE;
    Enemy[i]->below_ground=FALSE;
    if (slash_time_i==0 && Enemy[i]->knockback_timer==0) {
      EnemyAntGravity(i);
      if (!Enemy[i]->in_water) {
        if (Enemy[i]->in_air_timer<500) {
          Enemy[i]->in_air_timer++;
        }
      }
    }
  } 
}



void EnemyAntAct(int i,int slash_time_i)
{

  int speed=Enemy[i]->speed_multiplier;
  int grav=1;
  if (!Enemy[i]->within_render_distance && Enemy[i]->render_distance_timer<=0) {
    grav=1;
    speed=1;
  } else {
    grav=1+Enemy[i]->in_air_timer/5;
    if (grav>10) {
      grav=10;
    }
    if (Enemy[i]->is_in_ground_edge) {
      speed=/*5*/2*Enemy[i]->speed_multiplier;
    }
  }

  if (Enemy[i]->render_distance_timer>0) {
    Enemy[i]->render_distance_timer--;
  }

  //Ground ACtion
  for (int g=0;g<grav;g++) {
  for (int s=0;s<speed;s++) {
    //Gravity Action
    Enemy[i]->on_ground_id=GetOnGroundIdE(Enemy[i]->x,Enemy[i]->y,5,4,i);
    if (s==0) {
      EnemyAntActGravity(i,slash_time_i);
    }
    if (Enemy[i]->knockback_timer==0) { 
        if (Enemy[i]->in_water) {
          Enemy[i]->on_ground_id=-1;
        }
        if (Enemy[i]->in_water && Enemy[i]->on_ground_timer==0 && !Enemy[i]->is_in_ground_edge) {
            if (Enemy[i]->species==5) {
              if (Enemy[i]->x<MAP_WIDTH-NODE_SIZE) {
                Enemy[i]->x+=Enemy[i]->speed;
              }
            } else if (Enemy[i]->species==6) {
              if (Enemy[i]->x>NODE_SIZE) {
                Enemy[i]->x-=Enemy[i]->speed;
              }
            } else {
              if (Enemy[i]->is_clockwize) {
                if (Enemy[i]->x<MAP_WIDTH-NODE_SIZE) {
                  Enemy[i]->x+=Enemy[i]->speed;
                }
              } else {
                if (Enemy[i]->x>NODE_SIZE) {
                  Enemy[i]->x-=Enemy[i]->speed;
                }
              }
            }
        }

        if (Enemy[i]->on_ground_id!=-1) {
          double ground_entity_E=GetLineTargetAngle(Enemy[i]->on_ground_id,Enemy[i]->x,Enemy[i]->y);
          double height_from_e_x=GetLineTargetHeight(Enemy[i]->on_ground_id,ground_entity_E,Enemy[i]->x,Enemy[i]->y);

          if  ((Ground[Enemy[i]->on_ground_id]->x1-10<Enemy[i]->x &&  Enemy[i]->x<Ground[Enemy[i]->on_ground_id]->x2+10) &&
              ((Ground[Enemy[i]->on_ground_id]->y1-10<Enemy[i]->y && Enemy[i]->y<Ground[Enemy[i]->on_ground_id]->y2+10) ||
               (Ground[Enemy[i]->on_ground_id]->y2-10<Enemy[i]->y && Enemy[i]->y<Ground[Enemy[i]->on_ground_id]->y1+10)))
          {
            if (Enemy[i]->species==5) {
              EnemyAntActOnGround(i,height_from_e_x,Enemy[i]->on_ground_id,TRUE);
            } else if (Enemy[i]->species==6) {
              EnemyAntActOnGround(i,height_from_e_x,Enemy[i]->on_ground_id,FALSE);
            } else {
              EnemyAntActOnGround(i,height_from_e_x,Enemy[i]->on_ground_id,Enemy[i]->is_clockwize);
            }
          }
        } else {
          Enemy[i]->saved_ground_id=-1;
        }

        Enemy[i]->saved_ground_id=Enemy[i]->on_ground_id;
        if (Enemy[i]->species==5) {
          EnemyAntActOnGroundEdge(i,TRUE);
        } else if (Enemy[i]->species==6) {
          EnemyAntActOnGroundEdge(i,FALSE);
        } else {
          EnemyAntActOnGroundEdge(i,Enemy[i]->is_clockwize);
        }
      }
    }
  }
  if (Enemy[i]->on_ground_timer>0) {
    Enemy[i]->on_ground_timer--;
  } else {
    if (!Enemy[i]->in_water && Enemy[i]->species==7) {
      if (RandNum(0,30,&Enemy[i]->rng_i,Enemy[i]->seed)==0) {
        Enemy[i]->is_clockwize=!Enemy[i]->is_clockwize;
      }
    }
    Enemy[i]->above_ground=FALSE;
    Enemy[i]->below_ground=FALSE;
    Enemy[i]->on_ground_id=-1;
    Enemy[i]->saved_ground_id=-1;
  }
}

bool InsectBites(int i,int dmg,bool is_mosquito)
{

  if (player.block_timer>23 || player.block_timer==0 || player.block_health<=0) {
      Enemy[i]->bullet_cooldown=Enemy[i]->bullet_cooldown_max;
      if (Enemy[i]->bullet_fire_cooldown<=0) {
         Enemy[i]->bullet_fire_cooldown=Enemy[i]->bullet_fire_cooldown_max;
         player.show_block_health_timer=HP_SHOW_TIMER_NUM;
         player.show_health_timer=HP_SHOW_TIMER_NUM;
        if (player.block_timer==0 || player.block_health<=0) {
             if (is_mosquito) {
               Enemy[i]->damage_taken_timer=256;
             }
             if (game_audio && player.health>0 && player.hurt_snd_timer==0) {
               PlaySound(spamSoundEffectCache[5].audio, NULL, SND_MEMORY | SND_ASYNC); //hurt snd
               player.hurt_snd_timer=HIT_PLAYER_SND_COOLDOWN_DURATION;
             }
             if (player.health>PLAYER_LOW_HEALTH+1) { //usual response
               //player.health-=dmg;
               player.block_health-=1+dmg/2;
               if (player.block_health<=0) {
                 player.block_health=0;
               }
               double dmg_delta=dmg;
               if (player.block_health>10) { 
                 player.health-=dmg/(player.block_health/16+1);
                 dmg_delta=dmg/(player.block_health/16+1);
               } else {
                 player.health-=dmg;
               }

               if (is_mosquito) {
                 Enemy[i]->health+=dmg_delta;
                 if (Enemy[i]->health>=Enemy[i]->max_health) {
                   Enemy[i]->max_health+=dmg_delta;
                 }
               }
             } else { //Player when low health
               if (player.health<PLAYER_LOW_HEALTH) {
                 player.health-=0.1;
               } else {
                 player.health-=1;
               }
             }
         } else {
           if (game_audio && player.health>0 && player.hurt_snd_timer==0) {
             PlaySound(spamSoundEffectCache[3].audio, NULL, SND_MEMORY | SND_ASYNC); //hurt snd
             player.hurt_snd_timer=HIT_PLAYER_SND_COOLDOWN_DURATION;
           }
           player.block_health-=dmg;
         }

        
        if (player.speed>5 && player.type==0) {
          player.speed--;
        } else { //penalty only at low speed
          if (player.time_breaker_units>1) {
            player.invalid_shoot_timer=9;
            player.time_breaker_units=1;
          }
        }

      } else {
        Enemy[i]->bullet_fire_cooldown--;
      }
    return TRUE;
  } else {//perfect block , 23 or less than
    if (game_audio && player.health>0 && player.hurt_snd_timer==0) {
      PlaySound(spamSoundEffectCache[4].audio, NULL, SND_MEMORY | SND_ASYNC); //block perfect
      player.hurt_snd_timer=HIT_PLAYER_SND_COOLDOWN_DURATION;
    }
    return FALSE;    
  }
}

void EnemyDeductMaxHealth(int i)
{

  if (Enemy[i]->health<=0) {
    if (game_hard) {
      Enemy[i]->max_health/=3;
      if (Enemy[i]->max_health<=1) {
        Enemy[i]->true_dead=TRUE;
      }
    } else {
      Enemy[i]->true_dead=TRUE;
    }
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

  if (Enemy[i]->dist_from_player<=dist_unit) {
    Enemy[i]->within_render_distance=TRUE;
    Enemy[i]->render_distance_timer=100;
  } else {
    Enemy[i]->within_render_distance=FALSE;
  }

  for (j=0;j<Enemy[i]->bullet_shot_num;j++) {
    BulletAct(Enemy[i]->bullet_shot_arr[j]);
  }

  //cockroach quirk, small chance to revive after presume dead
  /*if (Enemy[i]->species==1 && Enemy[i]->health<=0 && !Enemy[i]->true_dead) {
  }*/

  if (Enemy[i]->health>0 || !Enemy[i]->true_dead) {


    //Enemy bullet
    EnemyLOSAct(i);//Shoot line of sight bullet


    if (Enemy[i]->damage_taken_timer>0) {
      Enemy[i]->damage_taken_timer--;
    }

    //in water
    if (Enemy[i]->in_water) {
      Enemy[i]->in_water_timer=25;
    } else if (Enemy[i]->in_water_timer>0){
      Enemy[i]->in_water_timer--;
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


      /*if (Enemy[i]->species==1 && Enemy[i]->health<=0 && !Enemy[i]->true_dead && dist_from_bullet0<=NODE_SIZE*7 && Enemy[i]->saw_player/*&& !game_hard) {
        int crevdice=RandNum(0,1000,&Enemy[i]->rng_i);
        if (crevdice<10) {
          slash_time=EnemyActDazzle(i,1800);
          force_search=TRUE;
          Enemy[i]->health=Enemy[i]->max_health;
        }
      }*/


      if (dist_from_bullet0<=NODE_SIZE*4 && (player.health>0 || player.death_timer>198)) {
        Enemy[i]->last_seen_timer=200;
        switch (Enemy[i]->species) {
      	  case 0:case 2:case 4:case 5:case 6:case 7:
          {
            if (dist_from_bullet0<=NODE_SIZE*2) {
              Enemy[i]->damage_taken_timer=256;
              Enemy[i]->health-=Bullet[bk].damage;
              if (player.health<=0) {
                if (Enemy[i]->health<=0 && !Enemy[i]->try_revive_player) {
                  Enemy[i]->try_revive_player=TRUE;
                  Enemy[i]->true_dead=TRUE;
                  int revdice=abs(RandNum(0,100,&player.rng_i,player.seed));
                  if (revdice<8) {
                    player.x=Enemy[i]->x;
                    player.y=Enemy[i]->y;
                    PlayerRevive();
                  }
                }
              }
              if (game_audio && Enemy[i]->hurt_snd_timer==0) {
                PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
                Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
              }
              if (!Enemy[i]->in_water) {
                Enemy[i]->knockback_timer=player.knockback_strength;
              } else {
                Enemy[i]->knockback_timer=player.knockback_strength/2;
              }
              Enemy[i]->knockback_angle=Bullet[bk].angle;
              Enemy[i]->player_knockback=FALSE;
              if (Bullet[bk].graphics_type!=6)
                Bullet[bk].angle=RandAngle(0,360,&player.rng_i,player.seed*bk);
            }
            }
	        break;
         case 1://crawl
         case 3:
            Enemy[i]->damage_taken_timer=256;
            if (Enemy[i]->health>0) {
              Enemy[i]->health-=Bullet[bk].damage;
              EnemyDeductMaxHealth(i);                
            }
            if (Enemy[i]->health<=0 && !Enemy[i]->true_dead) {
              if (Bullet[bk].graphics_type==6) {
                Enemy[i]->true_dead=TRUE;
              }
            }
              if (player.health<=0) {
                if (Enemy[i]->health<=0 && !Enemy[i]->try_revive_player) {
                  Enemy[i]->true_dead=TRUE;
                  Enemy[i]->try_revive_player=TRUE;
                  int revdice=abs(RandNum(0,100,&player.rng_i,player.seed));
                  if (revdice<8) {
                    player.x=Enemy[i]->x;
                    player.y=Enemy[i]->y;
                    PlayerRevive();
                  }
                }
              }

              if (game_audio && Enemy[i]->hurt_snd_timer==0) {
                PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
                Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
              }
            //if ((Enemy[i]->on_ground_id==-1 && (Enemy[i]->health<=0 || RandNum(0,100,&Enemy[i]->rng_i)<20)) || Enemy[i]->on_ground_id!=-1) {
              Enemy[i]->knockback_timer=player.knockback_strength;
            //}
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
    if (player.is_swinging && player.health>0) {
      for (int k=0;k<PLAYER_FLING_WEB_NUM;k++) {
        //knockback
        double dist_from_web_string=GetDistance(player_fling_web.x[k],player_fling_web.y[k],Enemy[i]->x,Enemy[i]->y);
        if (dist_from_web_string<NODE_SIZE*2 && Enemy[i]->on_ground_id==-1 && !Enemy[i]->web_stuck) {
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


    if (dist_from_bullet<=NODE_SIZE*4 && player.health>0) {
      Enemy[i]->last_seen_timer=200;
      switch (Enemy[i]->species) {
	    case 0:case 2:case 4:case 5:case 6:case 7:
          if (dist_from_bullet<=NODE_SIZE*2) {
            if (game_audio && Enemy[i]->hurt_snd_timer==0) {
              PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
              Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
            }
            Enemy[i]->damage_taken_timer=256;
            Enemy[i]->health-=Bullet[player.bullet_shot].damage;
            Enemy[i]->knockback_timer=player.knockback_strength;
            Enemy[i]->knockback_angle=Bullet[player.bullet_shot].angle;            
            Enemy[i]->player_knockback=FALSE;
          }
          break;
        case 1:case 3:
          if (game_audio && Enemy[i]->hurt_snd_timer==0) {
            PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
            Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
          }
          Enemy[i]->damage_taken_timer=256;
          if (Enemy[i]->health>0) {
            Enemy[i]->health-=Bullet[player.bullet_shot].damage;
            EnemyDeductMaxHealth(i);
          }
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
        if (game_hard && Enemy[i]->dist_from_player<=NODE_SIZE*5) {
          Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
          Enemy[i]->speed=Enemy[i]->ospeed;
        }
        if (Enemy[i]->dist_from_player<=NODE_SIZE*3) {
          slash_time=EnemyActDazzle(i,1000);
          force_search=TRUE;
        }
      } 

  //cockroach play dead and instant attack surprise when close
  if (Enemy[i]->species==1 && Enemy[i]->health<=0 && !Enemy[i]->true_dead /*&& !game_hard*/) {
        if (!Enemy[i]->saw_player && Enemy[i]->last_seen_timer==0) { //not seen by player
          int crevdice=RandNum(0,1000,&Enemy[i]->dazzle_rng_i,Enemy[i]->seed);
          if (crevdice<10) {
            slash_time=EnemyActDazzle(i,2000);
            force_search=TRUE;
            Enemy[i]->health=Enemy[i]->max_health;
            Enemy[i]->flying_timer=100;
          }
        } else { //seeing player
          int crevdice;
          if (Enemy[i]->dist_from_player<=NODE_SIZE*5) {
            crevdice=RandNum(0,100,&Enemy[i]->dazzle_rng_i,Enemy[i]->seed);
          } else if (Enemy[i]->dist_from_player<=NODE_SIZE*15 || Enemy[i]->saw_player) {
            crevdice=RandNum(0,4000,&Enemy[i]->dazzle_rng_i,Enemy[i]->seed);
          }
          if (crevdice<5) {
            slash_time=EnemyActDazzle(i,17);
            force_search=TRUE;
            Enemy[i]->health=Enemy[i]->max_health;
            Enemy[i]->flying_timer=100;
          }
       }
  }


   bool insect_bite=FALSE;
   //mosquito quirk 
     if (Enemy[i]->species==0 && !Enemy[i]->web_stuck && (!player.time_breaker || Enemy[i]->time_breaker_immune)) {
        if (Enemy[i]->dist_from_player<=NODE_SIZE*2) {
          insect_bite=InsectBites(i,Enemy[i]->bullet_damage,TRUE);
        }
     }

   //ant quirk 
    if ((Enemy[i]->species>=5 && Enemy[i]->species<=7) && !Enemy[i]->web_stuck && (!player.time_breaker || Enemy[i]->time_breaker_immune)) {
        if (Enemy[i]->dist_from_player<=NODE_SIZE*2) {
          insect_bite=InsectBites(i,Enemy[i]->bullet_damage,FALSE); 
        }
     }

    //toe BITER quirk
    if ((Enemy[i]->species==3) && !Enemy[i]->web_stuck && (!player.time_breaker || Enemy[i]->time_breaker_immune)) {
        if (Enemy[i]->dist_from_player<=NODE_SIZE*3) {          
          insect_bite=InsectBites(i,Enemy[i]->bullet_damage,FALSE); 
        }
     }

    if (Enemy[i]->last_seen_timer>0  && (!player.time_breaker || Enemy[i]->time_breaker_immune)) {
      Enemy[i]->last_seen_timer--;
    }

   //Enemy knockback & attacked
    allow_act=FALSE;
    double distance_from_player_claws=GetDistance(Enemy[i]->x,Enemy[i]->y,player.claws_attack_x,player.claws_attack_y);
    switch (Enemy[i]->species) {
      case 0:case 2:case 4:case 5:case 6:case 7:
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
      case 1:case 3:
	    if (distance_from_player_claws<=NODE_SIZE*4) {
	      allow_act=TRUE;
	    }
	    break;
    }
    //^^ condition 
    if (allow_act) {
      allow_act=FALSE;
      switch (Enemy[i]->species) {
        case 0:case 2:case 4:case 5:case 6:case 7:
          if (player.attack_timer>34) {
            allow_act=TRUE;
          }
	      break;
	    case 1:case 3:
          if (player.attack_timer>38)
            allow_act=TRUE;
	      break;
	  }
    }
	// ^^ condition
    if (allow_act && (Enemy[i]->saw_player || Enemy[i]->last_seen_timer>0)) {  //player meelee
      allow_act=FALSE;
      deduct_health=TRUE;
      if (Enemy[i]->species==1 && Enemy[i]->health<=0) {
        Enemy[i]->true_dead=TRUE;
      }

      if (Enemy[i]->health>0) {
        Enemy[i]->player_knockback=TRUE;
        Enemy[i]->knockback_timer=player.knockback_strength;
      }
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
    if (player.block_timer>1 && (/*(Enemy[i]->species>=5 && Enemy[i]->species<=7) ||*/ !player.on_a_ground) && player.health>0 && (Enemy[i]->saw_player || Enemy[i]->last_seen_timer>0)) {
    //knockback from rebounding
      if (Enemy[i]->dist_from_player<=NODE_SIZE*2 || 
            (player.block_timer>0 && player.block_timer<=23 && Enemy[i]->dist_from_player<=player.block_timer*2)){
      
        Enemy[i]->player_knockback=FALSE;
        Enemy[i]->knockback_angle=player.angle_of_incidence;
        Enemy[i]->knockback_timer=player.knockback_strength*2;
        deduct_health=FALSE;
        switch (Enemy[i]->species) {
          case 0:case 2:case 4:
          case 5:case 6:case 7:
            if ((player.speed>0 && Enemy[i]->species>=5) || player.speed>5) {
              deduct_health=TRUE;
            } else if (game_audio && Enemy[i]->hurt_snd_timer==0) {
              if (!insect_bite) {
                PlaySound(spamSoundEffectCache[6].audio,NULL, SND_MEMORY | SND_ASYNC);            
                Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
              }
            }
            break;
          case 1:case 3:
            if (Enemy[i]->species==1 && Enemy[i]->health<=0) {
              if (player.speed>0) {
                Enemy[i]->true_dead=TRUE;                
              }
            }
            if (player.speed>10) {
              deduct_health=TRUE;
            } else if (game_audio && Enemy[i]->hurt_snd_timer==0) {
              PlaySound(spamSoundEffectCache[6].audio,NULL, SND_MEMORY | SND_ASYNC);
              Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
            }
            break;
        }
      }
    }

    if (player.speed>24) {
      switch (Enemy[i]->species) {
	    case 0:case 2:case 4:case 5:case 6:case 7:
	      if (distance_from_player_claws<=NODE_SIZE*4) {
            deduct_health=TRUE;
          }
          break;
        case 1:case 3:
	      if (distance_from_player_claws<=NODE_SIZE*4) {
            deduct_health=TRUE;
          }
          break;
      }
    }

    //Deduct health
    if (deduct_health) {
      deduct_health=FALSE;
      Enemy[i]->damage_taken_timer=256;
      if (Enemy[i]->health>0) {
        Enemy[i]->health-=player.attack_strength;
        if (Enemy[i]->species==1) {
          EnemyDeductMaxHealth(i);
        }
      }
      if (game_audio && !insect_bite && Enemy[i]->hurt_snd_timer==0) {
        PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
        Enemy[i]->hurt_snd_timer=HIT_SND_COOLDOWN_DURATION;
      }
    }


    //Enemy Immediate Death Action
    if (Enemy[i]->health<=0) {
      if (Enemy[i]->species!=1 || (Enemy[i]->species==1 && Enemy[i]->true_dead)) {
          Enemy[i]->true_dead=TRUE;
          for (j=0;j<Enemy[i]->bullet_shot_num;j++) { //reset bullets
            StopBullet(Enemy[i]->bullet_shot_arr[j],FALSE);
          }
          int rand_bullet_shot_num;/*=8+RandNum(1,10,&Enemy[i]->rng_i);
          if (Enemy[i]->species==1 || Enemy[i]->species==3) {
            rand_bullet_shot_num=25+RandNum(30,40,&Enemy[i]->rng_i);        
          }*/
          switch (Enemy[i]->species) {
            case 0: rand_bullet_shot_num=Enemy[i]->max_health/2+RandNum(5,10,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 1: rand_bullet_shot_num=6+RandNum(10,15,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 2: rand_bullet_shot_num=2+RandNum(1,5,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 3: rand_bullet_shot_num=6+RandNum(15,20,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 4: rand_bullet_shot_num=6+RandNum(1,5,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 5: rand_bullet_shot_num=1+RandNum(5,10,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 6: rand_bullet_shot_num=1+RandNum(5,10,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
            case 7: rand_bullet_shot_num=1+RandNum(5,10,&Enemy[i]->bullet_rng_i,Enemy[i]->seed); break;
          }
          for (int n=0;n<rand_bullet_shot_num;n++) {
            int seed=Enemy[i]->seed*n;
            if (!free_will) {seed=-1;}
            int rand_range=NODE_SIZE*3+NODE_SIZE*RandNum(1,5,&Enemy[i]->bullet_rng_i,seed);
            ShootBullet(current_bullet_id,
                Enemy[i]->bullet_shot_num,
                Enemy[i]->color,
                -8, //graphics type
                rand_range, // range
                0.1, //speed
                1+RandNum(1,10,&Enemy[i]->bullet_rng_i,seed), //speed multiuplier
                0, //damage
                i, //enemy id
                Enemy[i]->x,
                Enemy[i]->y,
                Enemy[i]->x,
                Enemy[i]->y,
                Enemy[i]->x+RandNum(-50,50,&Enemy[i]->bullet_rng_i,seed),
                Enemy[i]->y+RandNum(-30,30,&Enemy[i]->bullet_rng_i,seed),
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
      /*} else {
        //
      }*/
    }
    int tmp_ngid;
    int tmp_gid_;
    //Knockback 
    if (Enemy[i]->knockback_timer>0 /*&& (!Enemy[i]->move_to_target || (Enemy[i]->species>=5 && Enemy[i]->species<=7))*/) {
      if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
        //if (Enemy[i]->on_ground_id==-1) {
          Enemy[i]->knockback_timer--;
        //}
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
          //tmp_gid_=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,2,2); //squishy, but risk of clipping, i dont care
          tmp_gid_=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,3,2); //squishy, but risk of clipping, i dont care
            //Enemy[i]->on_ground_id;//
          //Enemy[i]->on_ground_id=GetOnGroundIdE(Enemy[i]->x,Enemy[i]->y,2,2,i);
          EnemyKnockbackMove(i,tmp_gid_);
        }
      }
    }

    //Pathfinding and movement only act when within render distance
    if ((!player.time_breaker || Enemy[i]->time_breaker_immune || Enemy[i]->species==4) && (Enemy[i]->within_render_distance || (Enemy[i]->species>=5 && Enemy[i]->species<=7))) {
      //timebreaker enemy
      if (Enemy[i]->time_breaker_length>0 && !player.time_breaker) {
        dice=RandNum(0,Enemy[i]->time_breaker_rare,&Enemy[i]->timebreaker_rng_i,Enemy[i]->seed);
        if (dice==1) {
          slash_time=Enemy[i]->time_breaker_length;
        }
      }

      if (Enemy[i]->species==1 || Enemy[i]->species==3) {
        if (Enemy[i]->health>0) {
          if (Enemy[i]->knockback_timer>0 || Enemy[i]->on_ground_id==-1 || Enemy[i]->in_water) {
            LargeEnemySpriteTimer(i);
          }
        } /*else {
          if (Enemy[i]->sprite_timer!=0) {
            LargeEnemySpriteTimer(i);
          }
        }*/
      }

      if (Enemy[i]->web_stuck) {
        slash_time=1;
      }


      //toggle staate in water
      Enemy[i]->in_node_grid_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);      
      tmp_ngid=Enemy[i]->in_node_grid_id;
      if (tmp_ngid!=-1) {
        if (NodeGrid[tmp_ngid]->node_fire || Enemy[i]->suffocate_timer>=450) {
          Enemy[i]->damage_taken_timer=256;
          Enemy[i]->health-=1;
          if (Enemy[i]->species==1 && Enemy[i]->health<=0) {
            Enemy[i]->true_dead=TRUE;
          }
        }
        if (NodeGrid[tmp_ngid]->node_water) {
          if (!Enemy[i]->in_water) {
            if (Enemy[i]->species!=3) {
              Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier/4;
              Enemy[i]->speed=Enemy[i]->ospeed/4;
              if (Enemy[i]->speed_multiplier<=1) {
                Enemy[i]->speed_multiplier=1;
              }
            } else {
              Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier*2;
            }
          }
          Enemy[i]->in_water=TRUE;
          Enemy[i]->flying_timer=0;
          Enemy[i]->force_fall=FALSE;
          Enemy[i]->draw_falling=FALSE;
          Enemy[i]->sprite_in_water=TRUE;
        } else { //leave water
          if (Enemy[i]->in_water) {
            Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
            Enemy[i]->speed=Enemy[i]->ospeed;
          } 
          Enemy[i]->in_water=FALSE;
        }
      } else { //leave water
        if (Enemy[i]->in_water) {
          Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
          Enemy[i]->speed=Enemy[i]->ospeed;
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
          if (!(Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
            Enemy[i]->on_ground_id=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,33,32);
          }
          if (Enemy[i]->is_in_ground_edge) {
            Enemy[i]->draw_falling=FALSE;
          }
          if ((Enemy[i]->species==1 || Enemy[i]->species==3) && Enemy[i]->on_ground_id!=-1) {//<------ source of major error
            EnemySpriteOnGroundId(i,Enemy[i]->on_ground_id);
          }
          if (Enemy[i]->on_ground_id==-1 && !(Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
            Enemy[i]->is_in_ground_edge=FALSE;
          }

      //check state web stuck
        EnemyActWebStuck(i);

      //check state suffocate
        if (!Enemy[i]->is_in_ground_edge) {
          EnemyActSuffocate(i);
        }

        //Prevent reaching border
        if (Enemy[i]->x<NODE_SIZE*2) {
          Enemy[i]->x+=2;
          Enemy[i]->idling=TRUE;
          Enemy[i]->move_to_target=FALSE;
        }
        if (Enemy[i]->y<NODE_SIZE*2) {
          Enemy[i]->y+=2;
          Enemy[i]->idling=TRUE;
          Enemy[i]->move_to_target=FALSE;
        }
        if (Enemy[i]->x>MAP_WIDTH-NODE_SIZE*2) {
          Enemy[i]->x-=2;
          Enemy[i]->idling=TRUE;
          Enemy[i]->move_to_target=FALSE;
        }
        if (Enemy[i]->y>MAP_HEIGHT-NODE_SIZE*2) {
          Enemy[i]->y-=2;
          Enemy[i]->idling=TRUE;
          Enemy[i]->move_to_target=FALSE;
        }


        if ((Enemy[i]->species==1 || Enemy[i]->species==3)) {
            if ((Enemy[i]->health<=0 || (!Enemy[i]->move_to_target && !Enemy[i]->target_player))) {//Species 1 gravity
              if (slash_time_i==0) {
                if (Enemy[i]->health<=0) { //dead roach
                  Enemy[i]->force_fall=TRUE;
                  Enemy[i]->draw_falling=FALSE;
                  Enemy[i]->flying_timer=0;
                }
                if (Enemy[i]->health<=0 || (Enemy[i]->knockback_timer==0 && (Enemy[i]->flying_timer==0 || Enemy[i]->force_fall))) {//not flying, fall
                  for (int gr=0;gr<1+Enemy[i]->in_air_timer/5;gr++) {
                    EnemyGravity(i,gr);
                  }
                }
              }
            }

            if (Enemy[i]->flying_timer>0) {
              Enemy[i]->flying_timer--;
              if (Enemy[i]->flying_timer>=2 && Enemy[i]->flying_timer<=4 && Enemy[i]->species==3) {
                Enemy[i]->flying_timer=300;
                Enemy[i]->draw_falling=FALSE;
              }
            } else {
              if (Enemy[i]->health>0 /*&& Enemy[i]->move_to_target*/) {
                Enemy[i]->draw_falling=FALSE;
                Enemy[i]->flying_timer=0;
              }
            }

        }




        if (Enemy[i]->species==1 || Enemy[i]->species==3) {
            if (Enemy[i]->in_air_timer>0) {
              Enemy[i]->in_air_timer--;
            }
            if (Enemy[i]->in_air_timer==0) {
              Enemy[i]->draw_falling=FALSE;
            }
        }

        if (Enemy[i]->species==4 && !Enemy[i]->web_stuck) {
          if (!player.time_breaker) {
            if (slash_time>1) { //dazzle movement
              Enemy[i]->speed_multiplier=1;
              Enemy[i]->speed=0.05;
            } else {
              if (slash_time_i==0) {
                Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
                Enemy[i]->speed=Enemy[i]->ospeed;
              }
            }
          } else {
            if (slash_time>1) { //dazzle movement
              Enemy[i]->speed_multiplier=1;
              Enemy[i]->speed=0.025;
            } else {
              if (slash_time_i==0) {
                if (!game_hard) {
                  Enemy[i]->speed_multiplier=1;
                  Enemy[i]->speed=0.05;
                } else {
                  Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
                  Enemy[i]->speed=Enemy[i]->ospeed;
                }
              }
            }
          }
        }

        if (Enemy[i]->species>=5 && Enemy[i]->species<=7) {
          EnemyAntAct(i,slash_time_i);
        }

        /*int above_player_node1=GetGridId(
                player.above_x-EnemyPathfinding[pfi]->node_x[0],
                player.above_y-EnemyPathfinding[pfi]->node_y[0],
                Enemy[i]->follow_range*NODE_SIZE,
                NODE_SIZE,EnemyPathfinding[pfi]->node_num);*/

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


        //ensure doesnt get too close to ground, absolute;
        int tbgid=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,2,2);
        if (tbgid!=-1) {
            double ground_entity_E=GetLineTargetAngle(tbgid,Enemy[i]->x,Enemy[i]->y);
            double height_from_e_x=GetLineTargetHeight(tbgid,ground_entity_E,Enemy[i]->x,Enemy[i]->y);
            EnemyAntActOnGround(i,height_from_e_x,tbgid,Enemy[i]->last_left);
            Enemy[i]->player_knockback=FALSE;          
            Enemy[i]->idling=TRUE;
            Enemy[i]->move_to_target=FALSE;
            //Enemy[i]->knockback_timer=0;
        }

        if (Enemy[i]->force_search/* || game_hard*/) {
          force_search=TRUE;
          Enemy[i]->force_search=FALSE;
        }

        if (Enemy[i]->health>0 && ((player.health>0 && !Enemy[i]->ignore_player) || Enemy[i]->web_stuck)  //not ignore & within range
             /*Enemy[i]->dist_from_player<Enemy[i]->follow_range/2*NODE_SIZE*/
           ) {
          if (Enemy[i]->bullet_fire_cooldown<=0) {
	        if (Enemy[i]->bullet_length==0) {
	          for (j=0;j<Enemy[i]->bullet_fire_at_once_max;j++) {//shoot several bullets at once
                int seed1=Enemy[i]->seed*j;
                int seed2=Enemy[i]->seed*2*j;
                if (!free_will) {seed1=seed2=-1;}
	            Enemy[i]->bullet_head_x[j]=player.x+RandNum(-Enemy[i]->aim_rand,Enemy[i]->aim_rand,&Enemy[i]->bullet_aim_rand_rng_i,seed1);
	            Enemy[i]->bullet_head_y[j]=player.y+RandNum(-Enemy[i]->aim_rand,Enemy[i]->aim_rand,&Enemy[i]->bullet_aim_rand_rng_i,seed2);
	          }
	        }

            if (Enemy[i]->bullet_cooldown<=0) {
	          if (Enemy[i]->bullet_shot_num<500) {//shot less than 500 bullets
                int bspeed_m=Enemy[i]->bullet_speed_multiplier,
                    bdmg=Enemy[i]->bullet_damage,btype=Enemy[i]->bullet_graphics_type;
                double bspeed=Enemy[i]->bullet_speed,
                       brange=Enemy[i]->bullet_range;
                if (Enemy[i]->web_stuck) { //shoot webstuck bullets
                  bspeed/=3;
                  brange=NODE_SIZE+NODE_SIZE/2;
                  bspeed_m=bspeed_m/3+1;
                  
                  //bdmg/=3;
                  btype=-1; //specific type only damages webs, doesnt react to bullets nor player
	              for (j=0;j<Enemy[i]->bullet_fire_at_once_max;j++) {//shoot several bullets at once
                    int seed1=Enemy[i]->seed*j;
                    int seed2=Enemy[i]->seed*2*j;
                    if (!free_will) {seed1=seed2=-1;}
	                Enemy[i]->bullet_head_x[j]=Enemy[i]->x+RandNum(-1000,1000,&Enemy[i]->bullet_stuck_rng_i,seed1);
	                Enemy[i]->bullet_head_y[j]=Enemy[i]->y+RandNum(-1000,1000,&Enemy[i]->bullet_stuck_rng_i,seed2);
                  }
                }
	            for (j=0;j<Enemy[i]->bullet_fire_at_once_max;j++) {//several bullets at once
		          if (Enemy[i]->saw_player || Enemy[i]->web_stuck) {
                    if (Enemy[i]->dist_from_player<Enemy[i]->shoot_at_player_range/2*NODE_SIZE || Enemy[i]->web_stuck) {
    		          Enemy[i]->shoot_target_x=Enemy[i]->bullet_head_x[j];
        		      Enemy[i]->shoot_target_y=Enemy[i]->bullet_head_y[j];
                      ShootBullet(current_bullet_id,
		                Enemy[i]->bullet_shot_num,
		                Enemy[i]->bullet_color,
                        btype,
		                //Enemy[i]->bullet_graphics_type,
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
            if (!game_hard) {
              Enemy[i]->bullet_cooldown--;
            } else {
	          Enemy[i]->bullet_cooldown-=3;
            }
          }
        } else {
          if (!game_hard) {
	        Enemy[i]->bullet_fire_cooldown--;
          } else {
	        Enemy[i]->bullet_fire_cooldown-=3;
          }
        }
      }


    int pfi=Enemy[i]->pathfinding_id;
   //pathfinding
    if (!(Enemy[i]->species>=5 && Enemy[i]->species<=7) && pfi!=-1) { //not for ants
 


     Enemy[i]->forgor_timer++;
      if (Enemy[i]->forgor_timer>300) {
        Enemy[i]->forgor_timer=0;
        Enemy[i]->ignore_player=FALSE;
      }

    //Movement
      if (Enemy[i]->idling) { //idling
        Enemy[i]->idle_timer++;
        //if (game_hard) { //removed due to cpu overclock risk
          //Enemy[i]->idle_timer+=3;
        //}
	    allow_act=FALSE;
        if (Enemy[i]->saw_player && !Enemy[i]->ignore_player && player.health>0) {//not ignoring
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
          //if (game_hard) { //commented out due to risk of cpu overclock
            //dice=1;
          //} else {
            dice=RandNum(0,5,&Enemy[i]->idle_rng_i,Enemy[i]->seed);
          //}
          Enemy[i]->idle_timer=0;
          if (dice==1 || force_search) { //Start searching
	      //total ignore player (still hostile)
            if (force_search) {
              force_search=FALSE;
            }
            int seed1=Enemy[i]->seed;
            int seed2=Enemy[i]->seed*2;
            if (!free_will) {seed1=seed2=-1;}
            target_x=Enemy[i]->x+RandNum(-MAX_FOLLOW_RANGE/3*NODE_SIZE,abs(MAX_FOLLOW_RANGE/3*NODE_SIZE),&Enemy[i]->target_rng_i,seed1);
            target_y=Enemy[i]->y+RandNum(-MAX_FOLLOW_RANGE/3*NODE_SIZE,abs(MAX_FOLLOW_RANGE/3*NODE_SIZE),&Enemy[i]->target_rng_i,seed2);
            target_node=GetGridId(target_x-EnemyPathfinding[pfi]->node_x[0],
		                        target_y-EnemyPathfinding[pfi]->node_y[0],
                                MAX_FOLLOW_RANGE*NODE_SIZE,
		                        NODE_SIZE,
    	                        EnemyPathfinding[pfi]->node_num);
            if (!EnemyPathfinding[pfi]->node_solid[target_node]/* && (!raining ||
              (raining && !NodeGrid[enemy_on_node_grid_id]->node_no_shade))*/
             ) {
              Enemy[i]->idling=FALSE;
              Enemy[i]->search_target=TRUE;
              /*if (Enemy[i]->species==1 || Enemy[i]->species==3) {
                int rand_dice=RandNum(0,100,&Enemy[i]->rng_i);
                if (rand_dice<40) {
                  Enemy[i]->flying_timer=100;
                }
              }*/
              InitEnemyPathfinding(i,target_x,target_y);
            } else {
              Enemy[i]->idling=TRUE;
            }
          } else { //not searching
            Enemy[i]->idling=TRUE;
          }
        }
      } else if (Enemy[i]->search_target) {
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
        if (Enemy[i]->knockback_timer==0 && Enemy[i]->health>0) {
          for (j=0;j<Enemy[i]->speed_multiplier;j++) {
            LargeEnemySpriteTimer(i);
            EnemyMove(i);
          }
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
                if (Enemy[i]->player_at_left && Enemy[i]->player_at_right && player.health>0) {//Change of X
                  EnemyTargetPlayer(i);
                  Enemy[i]->player_at_left=FALSE;
                  Enemy[i]->player_at_right=FALSE;
                }
                if (player.y<Enemy[i]->y) {//Change of Y
                  Enemy[i]->player_at_above=TRUE;
                } else if (Enemy[i]->y<=player.y) {
                  Enemy[i]->player_at_below=TRUE;
                }
                if (Enemy[i]->player_at_above && Enemy[i]->player_at_below && player.health>0) {
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
                if (Enemy[i]->in_unchase_range && Enemy[i]->in_chase_range && player.health>0) {
                  EnemyTargetPlayer(i);
                  Enemy[i]->in_chase_range=FALSE;
                  Enemy[i]->in_unchase_range=FALSE;
                }
              }
            }
          }
        }//end of slash_time
      //other
        if (!Enemy[i]->web_stuck) {
          if (Enemy[i]->species==0 || Enemy[i]->species==2 || (Enemy[i]->species==4 && (!player.time_breaker || Enemy[i]->time_breaker_immune)) || (Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
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
  if (!game_hard) {
    Enemy[i]->ospeed_multiplier=saved_enemy_type_speed_multiplier[type];
  } else {
    Enemy[i]->ospeed_multiplier=saved_enemy_type_speed_multiplier[type]+saved_enemy_type_speed_multiplier[type]/2;
  }
  Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
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
  if (!game_hard) {
    Enemy[i]->bullet_damage=saved_enemy_type_bullet_damage[type];
  } else {
    Enemy[i]->bullet_damage=saved_enemy_type_bullet_damage[type]*4;
  }
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
      FreeDrawSprite(&EnemyRotatedSprite[i].draw_dithered_rotated_sprite1[j]);
      FreeDrawSprite(&EnemyRotatedSprite[i].draw_dithered_rotated_sprite2[j]);
    }
  }

  for (int i=0;i<LARGER_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      FreeDrawSprite(&XEnemyRotatedSprite[i].draw_rotated_sprite[j]);
      FreeDrawSprite(&XEnemyRotatedSprite[i].draw_dithered_rotated_sprite[j]);
    }
  }
}



void CleanUpEnemySprites()
{
  //manual cleaning because static
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    FreeDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_1);
    FreeDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_2);
    FreeDrawSprite(&EnemyTypeSprite[i].draw_dithered_fly_sprite_1);
    FreeDrawSprite(&EnemyTypeSprite[i].draw_dithered_fly_sprite_2);
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
    //rotated sprites
    if (saved_enemy_type_species[i]==1 || saved_enemy_type_species[i]==3 || (saved_enemy_type_species[i]>=5 && saved_enemy_type_species[i]<=7)) {
      saved_enemy_type_rot_sprite_id[i]=LARGE_ENEMY_TYPE_NUM;
      EnemyRotatedSprite[LARGE_ENEMY_TYPE_NUM].type=i;
      saved_large_enemy_type_species[LARGE_ENEMY_TYPE_NUM]=saved_enemy_type_species[i];
      if (saved_enemy_type_time_breaker_immune[i]) {
        saved_large_enemy_type_time_breaker_immune[LARGE_ENEMY_TYPE_NUM]=TRUE;
      } else {
        saved_large_enemy_type_time_breaker_immune[LARGE_ENEMY_TYPE_NUM]=FALSE;
      }
      LARGE_ENEMY_TYPE_NUM++;
      if (saved_enemy_type_species[i]==3) { //xtra rotated rsprite
        saved_enemy_type_rot_xsprite_id[i]=LARGER_ENEMY_TYPE_NUM;
        XEnemyRotatedSprite[LARGER_ENEMY_TYPE_NUM].type=i;
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
    } else { //non-rotated sprite
      saved_enemy_type_rot_sprite_id[i]=-1;
      saved_enemy_type_rot_xsprite_id[i]=-1;
      saved_large_enemy_type_time_breaker_immune[i]=FALSE;
      saved_larger_enemy_type_time_breaker_immune[i]=FALSE;
    }
  }

  for (int i=0;i<ENEMY_NUM;i++) {
    Enemy[i]->rotated_sprite_id=saved_enemy_type_rot_sprite_id[Enemy[i]->type];
    Enemy[i]->rotated_xsprite_id=saved_enemy_type_rot_xsprite_id[Enemy[i]->type];
  }
}

void InitEnemySpritesObjColor(HDC hdc,HDC hdc2)
{
  int species_i=0;
  int si;
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
   // species_i=saved_enemy_type_species[i];
    //if (species_i!=1 && species_i!=3) {
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_fly_sprite_1.sprite_paint,EnemyTypeSprite[i].enemyPalette);
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_fly_sprite_2.sprite_paint,EnemyTypeSprite[i].enemyPalette);
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_dithered_fly_sprite_1.sprite_paint,EnemyTypeSprite[i].enemyPalette);
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_dithered_fly_sprite_2.sprite_paint,EnemyTypeSprite[i].enemyPalette);
    //}
  }

  for (int i=0;i<LARGE_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      species_i=saved_large_enemy_type_species[i]; 
      switch (species_i) {
        case 1:si=0;break;
        case 3:si=1;break;
        case 5:case 6:case 7:si=2;break;
      }
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPalette);
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPalette);
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_dithered_rotated_sprite1[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPalette);
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_dithered_rotated_sprite2[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPalette);
    }
  }

  for (int i=0;i<LARGER_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      species_i=saved_larger_enemy_type_species[i];
      switch (species_i) {
        case 3:
          BitmapPalette(hdc,hdc2,XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_paint,EnemyTypeSprite[XEnemyRotatedSprite[i].type].enemyPalette);
          BitmapPalette(hdc,hdc2,XEnemyRotatedSprite[i].draw_dithered_rotated_sprite[j].sprite_paint,EnemyTypeSprite[XEnemyRotatedSprite[i].type].enemyPalette);
          break;
      }
    }
  }
}


void InitEnemySpritesObjColorNoir(HDC hdc,HDC hdc2)
{
  int species_i=0;
  int si;
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    if (!saved_enemy_type_time_breaker_immune[i]) {
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_fly_sprite_1.sprite_paint,EnemyTypeSprite[i].enemyPaletteNoir);
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_fly_sprite_2.sprite_paint,EnemyTypeSprite[i].enemyPaletteNoir);
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_dithered_fly_sprite_1.sprite_paint,EnemyTypeSprite[i].enemyPaletteNoir);
      BitmapPalette(hdc,hdc2,EnemyTypeSprite[i].draw_dithered_fly_sprite_2.sprite_paint,EnemyTypeSprite[i].enemyPaletteNoir);
    }
  }
  for (int i=0;i<LARGE_ENEMY_TYPE_NUM;i++) {
    if (!saved_enemy_type_time_breaker_immune[EnemyRotatedSprite[i].type]) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      species_i=saved_large_enemy_type_species[i]; 
      switch (species_i) {
        case 1:si=0;break;
        case 3:si=1;break;
        case 5:case 6:case 7:si=2;break;
      }
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPaletteNoir);
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPaletteNoir);
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_dithered_rotated_sprite1[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPaletteNoir);
      BitmapPalette(hdc,hdc2,EnemyRotatedSprite[i].draw_dithered_rotated_sprite2[j].sprite_paint,EnemyTypeSprite[EnemyRotatedSprite[i].type].enemyPaletteNoir);
    }
    }
  }

  for (int i=0;i<LARGER_ENEMY_TYPE_NUM;i++) {
    if (!saved_enemy_type_time_breaker_immune[XEnemyRotatedSprite[i].type]) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      species_i=saved_larger_enemy_type_species[i];
      switch (species_i) {
        case 3:
          BitmapPalette(hdc,hdc2,XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_paint,EnemyTypeSprite[XEnemyRotatedSprite[i].type].enemyPaletteNoir);
          BitmapPalette(hdc,hdc2,XEnemyRotatedSprite[i].draw_dithered_rotated_sprite[j].sprite_paint,EnemyTypeSprite[XEnemyRotatedSprite[i].type].enemyPaletteNoir);
          break;
      }
    }
    }
  }
}



void InitEnemySpritesObj()
{
  int species_i=0;
  double angle_rn=0;
  HBITMAP tmp_sprite1[ENEMY_TYPE_NUM],
          tmp_sprite2[ENEMY_TYPE_NUM]; //temporary
  for (int i=0;i<ENEMY_TYPE_NUM;i++) { //init small flysprites
    species_i=saved_enemy_type_species[i];
    switch (species_i) {
      case 0:
        tmp_sprite1[i]=CopyCrunchyBitmap(enemy1_sprite_1,SRCCOPY);
        tmp_sprite2[i]=CopyCrunchyBitmap(enemy1_sprite_2,SRCCOPY);
        break;
      case 1:
        tmp_sprite1[i]=CopyCrunchyBitmap(enemy2_sprite_3,SRCCOPY);
        tmp_sprite2[i]=CopyCrunchyBitmap(enemy2_sprite_4,SRCCOPY);
        break;
      case 2:
        tmp_sprite1[i]=CopyCrunchyBitmap(enemy3_sprite_1,SRCCOPY);
        tmp_sprite2[i]=CopyCrunchyBitmap(enemy3_sprite_2,SRCCOPY);
        break;
      case 3:
        tmp_sprite1[i]=CopyCrunchyBitmap(enemy4_sprite_3,SRCCOPY);
        tmp_sprite2[i]=CopyCrunchyBitmap(enemy4_sprite_4,SRCCOPY);
        break;
      case 4:
        tmp_sprite1[i]=CopyCrunchyBitmap(enemy5_sprite_1,SRCCOPY);
        tmp_sprite2[i]=CopyCrunchyBitmap(enemy5_sprite_2,SRCCOPY);
        break;
    }
    ReplaceBitmapColor(tmp_sprite1[i],LTBLUE,LTGREEN); //8 due to pureblack reserved for mask
    ReplaceBitmapColor(tmp_sprite2[i],LTBLUE,LTGREEN);
    ReplaceBitmapColor2(tmp_sprite1[i],LTGREEN,BLACK,8,LTGREEN); //8 due to pureblack reserved for mask
    ReplaceBitmapColor2(tmp_sprite2[i],LTGREEN,BLACK,8,LTGREEN);
    GenerateDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_1,tmp_sprite1[i]);
    GenerateDrawSprite(&EnemyTypeSprite[i].draw_fly_sprite_2,tmp_sprite2[i]);

    //create ditehred sprite
    DitherBitmapColor(tmp_sprite1[i],-1,BLACK);
    DitherBitmapColor(tmp_sprite2[i],-1,BLACK);
    GenerateDrawSprite(&EnemyTypeSprite[i].draw_dithered_fly_sprite_1,tmp_sprite1[i]);
    GenerateDrawSprite(&EnemyTypeSprite[i].draw_dithered_fly_sprite_2,tmp_sprite2[i]);

    loading_numerator++;
  }

  //mass chunks cleanup
  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    DeleteObject(tmp_sprite1[i]);
    DeleteObject(tmp_sprite2[i]);
  }

  //set flysprite palettes
  for (int i=0;i<ENEMY_TYPE_NUM;i++) { //init small flysprites
    CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,rgbColorsDefault,167,rgbPaint[saved_enemy_type_color[i]]); //set normal palette
    //if (map_background==0 || map_background==2) {
      //CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,151,RGB(16,16,16)); //set outline color
    //} else {
    CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,151,BLACK); //set outline color ltblue to ltgrey
    //}
    if (free_will) {
      CopyReplaceColorPalette(EnemyTypeSprite[i].enemyPalette,EnemyTypeSprite[i].enemyPalette,199,DKRLTGREEN); //set freewill to normal palette
    }
    CopyReplaceColorPaletteNoir(EnemyTypeSprite[i].enemyPaletteNoir,rgbColorsDefault,167,rgbPaint_i[saved_enemy_type_color[i]]); //set noir palette
    //}
  }


  //init rotated sprites
  int si;
  for (int i=0;i<LARGE_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      //angle_rn=M_PI_2-M_PI_16*j;
      species_i=saved_large_enemy_type_species[i]; 
      switch (species_i) {
        case 1:si=0;break;
        case 3:si=1;break;
        case 5:case 6:case 7:si=2;break;
      }
      EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_rotated_sprite1[j].sprite_mask,SRCCOPY);
      EnemyRotatedSprite[i].draw_rotated_sprite1[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_rotated_sprite1[j].sprite_paint,SRCCOPY);
      EnemyRotatedSprite[i].draw_dithered_rotated_sprite1[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_dithered_rotated_sprite1[j].sprite_mask,SRCCOPY);
      EnemyRotatedSprite[i].draw_dithered_rotated_sprite1[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_dithered_rotated_sprite1[j].sprite_paint,SRCCOPY);
      loading_numerator++;
      EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_rotated_sprite2[j].sprite_mask,SRCCOPY);
      EnemyRotatedSprite[i].draw_rotated_sprite2[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_rotated_sprite2[j].sprite_paint,SRCCOPY);
      EnemyRotatedSprite[i].draw_dithered_rotated_sprite2[j].sprite_mask=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_dithered_rotated_sprite2[j].sprite_mask,SRCCOPY);
      EnemyRotatedSprite[i].draw_dithered_rotated_sprite2[j].sprite_paint=CopyCrunchyBitmap(LoadEnemyRotatedSprite[si].draw_dithered_rotated_sprite2[j].sprite_paint,SRCCOPY);
      loading_numerator++;
    }
  }

  for (int i=0;i<LARGER_ENEMY_TYPE_NUM;i++) {
    for (int j=0;j<ROTATED_SPRITE_NUM;j++) {
      //angle_rn=M_PI_2-M_PI_16*j;
      species_i=saved_larger_enemy_type_species[i];
      switch (species_i) {
        case 3:
          XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_mask=CopyCrunchyBitmap(XLoadEnemyRotatedSprite[0].draw_rotated_sprite[j].sprite_mask,SRCCOPY);
          XEnemyRotatedSprite[i].draw_rotated_sprite[j].sprite_paint=CopyCrunchyBitmap(XLoadEnemyRotatedSprite[0].draw_rotated_sprite[j].sprite_paint,SRCCOPY);
          XEnemyRotatedSprite[i].draw_dithered_rotated_sprite[j].sprite_mask=CopyCrunchyBitmap(XLoadEnemyRotatedSprite[0].draw_dithered_rotated_sprite[j].sprite_mask,SRCCOPY);
          XEnemyRotatedSprite[i].draw_dithered_rotated_sprite[j].sprite_paint=CopyCrunchyBitmap(XLoadEnemyRotatedSprite[0].draw_dithered_rotated_sprite[j].sprite_paint,SRCCOPY);
          loading_numerator++;
          break;
      }
    }
  }
}

void InitEnemyPathfindingNodes()
{
  int x=0,y=0,ei=0;
  for (int i=0;i<PF_ENEMY_NUM;i++) {
    EnemyPathfinding[i]->path_nodes_num=0;
    ei=EnemyPathfinding[i]->enemy_id;
    for (int j=0;j<MAX_NODE_NUM;j++) {
      EnemyPathfinding[i]->enemy_species1_solids[j]=-1;
      EnemyPathfinding[i]->node_solid[j]=
        EnemyPathfinding[i]->node_back[j]=
        EnemyPathfinding[i]->node_open[j]=
        EnemyPathfinding[i]->node_closed[j]=FALSE;
      EnemyPathfinding[i]->node_x[j]=(Enemy[ei]->x/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(MAX_FOLLOW_RANGE/2);
      EnemyPathfinding[i]->node_y[j]=(Enemy[ei]->y/NODE_SIZE*NODE_SIZE)-NODE_SIZE*(MAX_FOLLOW_RANGE/2);
      EnemyPathfinding[i]->node_hcost[j]=0;
      EnemyPathfinding[i]->node_gcost[j]=INT_MAX;
      EnemyPathfinding[i]->node_fcost[j]=EnemyPathfinding[i]->node_gcost[j]+EnemyPathfinding[i]->node_hcost[j];
      EnemyPathfinding[i]->node_parent[j]=-1;
    }
    x=Enemy[ei]->x-EnemyPathfinding[i]->node_x[0];
    y=Enemy[ei]->y-EnemyPathfinding[i]->node_y[0];
    EnemyPathfinding[i]->start_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[i]->node_num);
    EnemyPathfinding[i]->end_node=GetGridId(x,y,MAX_FOLLOW_RANGE*NODE_SIZE,NODE_SIZE,EnemyPathfinding[i]->node_num);
    EnemyPathfinding[i]->open_nodes_num=0;
    for (int j=0;j<MAX_NODE_NUM;j++) {
      EnemyPathfinding[i]->open_nodes[j]=EnemyPathfinding[i]->start_node;
    }
  }
}


void InitPFEnemyObj()
{
  int pfi;
  PF_ENEMY_NUM=0;
  for (int i=0;i<ENEMY_NUM;i++) {
    if (!(Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
      Enemy[i]->pathfinding_id=PF_ENEMY_NUM;
      PF_ENEMY_NUM++;
    } else {
      Enemy[i]->pathfinding_id=-1;
    }
  }
  EnemyPathfinding = calloc(PF_ENEMY_NUM,sizeof(AEnemy*));

  for (int i=0;i<PF_ENEMY_NUM;i++) {
    AEnemyPathfinding *newEnemyPathfinding = createEnemyPathfinding();
    EnemyPathfinding[i] = newEnemyPathfinding;
  }
  for (int i=0;i<ENEMY_NUM;i++) {
    pfi=Enemy[i]->pathfinding_id;
    if (pfi!=-1) {
      EnemyPathfinding[pfi]->enemy_id=i;
    }
  }
}

//bool once=TRUE;
void InitEnemy()
{
  int i=0,j=0;
  for (i=0;i<ENEMY_NUM;i++) {
    int rng_index=i%4;
    int rng_index_start=0;
    switch (rng_index) { //rng variations based on enemy id
      case 1: //1/4
        rng_index_start=RAND_NUM/4;
        break;
      case 2: //2/4
        rng_index_start=RAND_NUM/2;
        break;
      case 3: //3/4
        rng_index_start=RAND_NUM/2+RAND_NUM/4;
        break;
      default://4/4
        rng_index_start=0;
        break;
    }

    Enemy[i]->rng_i=
    Enemy[i]->flying_rng_i=
    Enemy[i]->target_rng_i=
    Enemy[i]->target_player_rng_i=
    Enemy[i]->bullet_rng_i=
    Enemy[i]->dazzle_rng_i=
    Enemy[i]->timebreaker_rng_i=
    Enemy[i]->bullet_aim_rand_rng_i=
    Enemy[i]->bullet_stuck_rng_i=
    Enemy[i]->idle_rng_i=
    Enemy[i]->target_rng_i=rng_index_start;


    Enemy[i]->on_ground_id=-1;
    Enemy[i]->on_ground_edge_id=-1;
    Enemy[i]->saved_ground_id=-1;
    Enemy[i]->current_ngid_n=0;
    Enemy[i]->current_suffocate_ngid_n=0;
    Enemy[i]->suffocate_timer=0;
    Enemy[i]->in_water_timer=0;
    Enemy[i]->hurt_snd_timer=0;
    Enemy[i]->current_rot_sprite_angle_id=0;//-1;
    Enemy[i]->dist_from_player=999;
    Enemy[i]->flying_timer=0;
    Enemy[i]->force_fall=FALSE;
    Enemy[i]->x=saved_enemy_x[i];
    Enemy[i]->y=saved_enemy_y[i];
    Enemy[i]->sprite_x=
    Enemy[i]->sprite_y=-20;
    Enemy[i]->true_dead=FALSE;
    Enemy[i]->is_ground_rebounding=FALSE;
    Enemy[i]->is_in_ground_edge=FALSE;
    Enemy[i]->is_in_left_ground_edge=FALSE;
    Enemy[i]->is_in_right_ground_edge=FALSE;
    Enemy[i]->above_ground_edge=FALSE;
    Enemy[i]->below_ground_edge=FALSE;
    Enemy[i]->force_search=FALSE;
    Enemy[i]->sprite_flip_timer=0;
    Enemy[i]->render_distance_timer=0;
    Enemy[i]->try_revive_player=FALSE;
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
    Enemy[i]->on_ground_timer=0;
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
    Enemy[i]->draw_falling=FALSE;
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
    if (Enemy[i]->species>=5 && Enemy[i]->species<=7) {
      Enemy[i]->move_to_target=TRUE;
      Enemy[i]->ignore_player=FALSE;
    }
    Enemy[i]->is_clockwize=FALSE;
    Enemy[i]->play_death_snd=FALSE;
  //init default int 
    Enemy[i]->last_seen_timer=0;
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
    if (frame_tick==-8) { //initiate on start of app
      player.flag_revert_palette=TRUE;
      player.time_breaker_tick=-1;
    }

  /*funny++;
  if (funny>31)
    funny=0;*/

  for (i=0;i<ENEMY_NUM;i++) {  
    if (free_will)
      Enemy[i]->seed=rand();
    else
      Enemy[i]->seed=-1;

    Enemy[i]->sprite_x=(int)Enemy[i]->x+px+cx1+cx2;
    Enemy[i]->sprite_y=(int)Enemy[i]->y+py+cy1+cy2;
    if (Enemy[i]->species==1 || Enemy[i]->species==3 || (Enemy[i]->species>=5 && Enemy[i]->species<=7)) {//rotate sprite


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
      if (Enemy[i]->is_in_ground_edge && Enemy[i]->health>0) {
        int le_angle;
        double le_enemy_angle=Enemy[i]->angle;
        if ((Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
          le_enemy_angle=Enemy[i]->sprite_angle;
        }
        if (Enemy[i]->above_ground_edge) {
          if (!Enemy[i]->last_left) {
            le_angle=16-(le_enemy_angle)/(M_PI_16);
          } else {
            le_angle=(le_enemy_angle)/(M_PI_16);
          } 
        } else if (Enemy[i]->below_ground_edge) {
          if (Enemy[i]->last_left) {
            le_angle=16+(le_enemy_angle)/(M_PI_16);
          } else {
            le_angle=32-(le_enemy_angle)/(M_PI_16);
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
        double le_enemy_angle=Enemy[i]->angle;
        if ((Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
          le_enemy_angle=Enemy[i]->sprite_angle;
        }
        if (Enemy[i]->sprite_angle>0) { //Slope ++ \/
            if (Enemy[i]->above_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=8+le_enemy_angle/(M_PI_16);
              } else {
                le_angle=8-le_enemy_angle/(M_PI_16);
              }
            } else if (Enemy[i]->below_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=24+(le_enemy_angle)/(M_PI_16);
              } else {
                le_angle=24-(le_enemy_angle)/(M_PI_16);
              }
           }
        } else { //Slope -- /
            if (Enemy[i]->above_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=8+le_enemy_angle/(M_PI_16);
              } else {
                le_angle=(M_PI_2-le_enemy_angle)/(M_PI_16);
              }
            } else if (Enemy[i]->below_ground) {
              if (!Enemy[i]->last_left) {
                le_angle=16+(M_PI_2+le_enemy_angle)/(M_PI_16);
              } else {
                le_angle=32-(M_PI_2+le_enemy_angle)/(M_PI_16);
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

    for (k=0;k<Enemy[i]->bullet_shot_num;k++) {
      DrawBullet(hdc,hdc2,Enemy[i]->bullet_shot_arr[k]);
    }

    //Drawing operations, enemy is living
    if (Enemy[i]->health>0 || !Enemy[i]->true_dead) { //enemy is alive


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
          if (!player.time_breaker && player.type==0) {
            player.speed+=2;
          }
        }
      }

      Enemy[i]->play_death_snd=TRUE;
      Enemy[i]->health=-99999;
      if (in_main_menu && game_audio) {
        PlayMemSnd(&channelSoundEffect[1],&channelSoundEffectCache[1],TRUE,0); 
      }
    }


    //enemy Display on screen operations
    /*char debug_txt[16];
    sprintf(debug_txt,"%d",Enemy[i]->current_rot_sprite_angle_id);
    GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y-64,debug_txt,WHITE);*/

//    char debug_txt[64];
//    sprintf(debug_txt,"%d,%d,[%5.4f||%5.4f] || -%d",Enemy[i]->on_ground_id,Enemy[i]->saved_ground_id,print_me/*Enemy[i]->angle*/,Enemy[i]->max_edge_angle,Enemy[i]->is_in_ground_edge);
//    sprintf(debug_txt,"%d,%d,[%5.4f| || -%d",Enemy[i]->on_ground_id,Enemy[i]->saved_ground_id/*,print_me,Enemy[i]->angle*/,Enemy[i]->max_edge_angle,Enemy[i]->is_in_ground_edge);
//    GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y-64,debug_txt,WHITE);

    bool allow_act=FALSE;
    if (!game_hard) { //enemy visibility
      allow_act=Enemy[i]->within_render_distance;
    } else {
      allow_act=(player.health<=0 || (Enemy[i]->within_render_distance && (Enemy[i]->web_stuck || Enemy[i]->last_seen_timer>0)));
    }

    if (allow_act) {
    
      int etype=Enemy[i]->type;
      if (Enemy[i]->health>0 || !Enemy[i]->true_dead) {
      switch (Enemy[i]->species) {
        case 0:case 2:case 4:
          if (etype>-1 && etype<ENEMY_TYPE_NUM) {
            if (Enemy[i]->sprite_timer%2==0) {
              /*if (Enemy[i]->sprite_timer%4==0) {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y+1,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
              } else {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
              }*/
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,
                Enemy[i]->sprite_timer%4==0
                  ? Enemy[i]->sprite_y+1
                  : Enemy[i]->sprite_y,
                Enemy[i]->in_water_timer==0
                  ? &EnemyTypeSprite[etype].draw_fly_sprite_1
                  : &EnemyTypeSprite[etype].draw_dithered_fly_sprite_1,
                Enemy[i]->last_left);              
            } else {
              /*if (Enemy[i]->sprite_timer%3==0) {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y-2,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
              } else {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y-1,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
              }*/
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,
                Enemy[i]->sprite_timer%3==0
                  ? Enemy[i]->sprite_y-2
                  : Enemy[i]->sprite_y-1,
                Enemy[i]->in_water_timer==0
                  ? &EnemyTypeSprite[etype].draw_fly_sprite_2
                  : &EnemyTypeSprite[etype].draw_dithered_fly_sprite_2,
                Enemy[i]->last_left);
            }
          }
          break;

        case 1: 
        case 3: //កន្ទាទូក
        case 5: //ant
        case 6:
        case 7:
        {
          //DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[Enemy[i]->rotated_sprite_id].draw_rotated_sprite1[funny],Enemy[i]->last_left);
          /*if (Enemy[i]->species==3) {
            DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&XEnemyRotatedSprite[Enemy[i]->rotated_xsprite_id].draw_rotated_sprite[funny],Enemy[i]->last_left);
          }*/
          //char printfunny[6];
          //sprintf(printfunny,"@%d",funny);
          //GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y+48,printfunny,WHITE);
          int rsid=Enemy[i]->rotated_sprite_id;
          if (rsid!=-1 && etype>-1 && etype<ENEMY_TYPE_NUM) {
          if (((!Enemy[i]->is_in_ground_edge && Enemy[i]->draw_falling && Enemy[i]->health>0 && Enemy[i]->species==1) ||
               (Enemy[i]->sprite_in_water && !Enemy[i]->web_stuck)
              )) {
            int swim_rot_id=7;
            if (Enemy[i]->species==3) {
              swim_rot_id=9;
            }
            if (Enemy[i]->health<=0) {
              swim_rot_id=24;
            }
            /*if (Enemy[i]->sprite_timer%8==0) {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
              &EnemyRotatedSprite[rsid].draw_rotated_sprite1[swim_rot_id],Enemy[i]->last_left);
            } else {
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
              &EnemyRotatedSprite[rsid].draw_rotated_sprite2[swim_rot_id],Enemy[i]->last_left);
            }*/
            DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
              Enemy[i]->sprite_timer%8==0
                ? (Enemy[i]->in_water_timer==0
                  ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[swim_rot_id]
                  : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[swim_rot_id])
                : (Enemy[i]->in_water_timer==0
                  ? &EnemyRotatedSprite[rsid].draw_rotated_sprite2[swim_rot_id]
                  : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite2[swim_rot_id]),
              Enemy[i]->last_left);
          } else if (Enemy[i]->on_ground_id!=-1 || Enemy[i]->health<0) { //on a ground
            if (!Enemy[i]->is_in_ground_edge) {
              if (Enemy[i]->move_to_target || Enemy[i]->knockback_timer>0) { //moving to target and on ground
                bool flip_bool=FALSE;
                if (Enemy[i]->above_ground || Enemy[i]->below_ground) {
                   if (Enemy[i]->above_ground) {
                    flip_bool=Enemy[i]->last_left;
                  } else if (Enemy[i]->below_ground) {
                    flip_bool=Enemy[i]->flip_sprite;
                  }
                  /*if (Enemy[i]->sprite_timer%8==0) {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  } else {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  }*/
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                    Enemy[i]->sprite_timer%8==0
                      ? (Enemy[i]->in_water_timer==0
                        ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id]
                        : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id])
                      : (Enemy[i]->in_water_timer==0
                        ? &EnemyRotatedSprite[rsid].draw_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id]
                        : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id]),
                    flip_bool
                  );                
                } else {
                  if (!(Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
                    /*if (Enemy[i]->sprite_timer%2==0) { //fly sprite
                      DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
                    } else {
                      DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
                    }*/
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                      Enemy[i]->sprite_timer%2==0
                        ? (Enemy[i]->in_water_timer==0
                          ? &EnemyTypeSprite[etype].draw_fly_sprite_1
                          : &EnemyTypeSprite[etype].draw_dithered_fly_sprite_1)
                        :  (Enemy[i]->in_water_timer==0
                          ? &EnemyTypeSprite[etype].draw_fly_sprite_2
                          : &EnemyTypeSprite[etype].draw_dithered_fly_sprite_2),
                      Enemy[i]->last_left
                    );

                  } else {
                    /*if (Enemy[i]->sprite_timer%4==0) {
                      DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[6],Enemy[i]->last_left);
                    } else {
                      DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite2[6],Enemy[i]->last_left);
                    }*/                   
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                      Enemy[i]->sprite_timer%4==0
                        ? (Enemy[i]->in_water_timer==0
                          ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[6]
                          : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[6])
                        :  (Enemy[i]->in_water_timer==0
                          ? &EnemyRotatedSprite[rsid].draw_rotated_sprite2[6]
                          : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite2[6]),
                      Enemy[i]->last_left
                    );
                  }
                }
              } else { //not moving to target && on ground
                bool flip_bool=FALSE;
                if (Enemy[i]->above_ground) {
                  flip_bool=Enemy[i]->last_left;
                } else if (Enemy[i]->below_ground) {
                  flip_bool=Enemy[i]->flip_sprite;
                }

                if (Enemy[i]->species==3) {  //head shake toe biter
                  /*if (Enemy[i]->idle_timer%16<8) {
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  } else if (Enemy[i]->rotated_xsprite_id!=-1){
                    DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&XEnemyRotatedSprite[Enemy[i]->rotated_xsprite_id].draw_rotated_sprite[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  }*/
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                    Enemy[i]->idle_timer%16<8
                      ? (Enemy[i]->in_water_timer==0
                        ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id]
                        : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id])
                      : (Enemy[i]->in_water_timer==0
                        ? &XEnemyRotatedSprite[rsid].draw_rotated_sprite[Enemy[i]->current_rot_sprite_angle_id]
                        : &XEnemyRotatedSprite[rsid].draw_dithered_rotated_sprite[Enemy[i]->current_rot_sprite_angle_id]),
                    flip_bool
                  );                
                } else {
                  //DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                    Enemy[i]->in_water_timer==0
                      ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id]
                      : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],
                    flip_bool
                  );
                }
             }



            } else { //sprite on ground edge
              bool flip_bool=FALSE;
              if (Enemy[i]->above_ground_edge) {
                flip_bool=Enemy[i]->last_left;
              } else if (Enemy[i]->below_ground_edge) {
                flip_bool=Enemy[i]->flip_sprite;
              }
              /*if (Enemy[i]->sprite_timer%8==0) {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
              } else {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id],flip_bool);
              }*/
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                Enemy[i]->sprite_timer%8==0
                  ? (Enemy[i]->in_water_timer==0
                    ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id]
                    : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[Enemy[i]->current_rot_sprite_angle_id])
                  : (Enemy[i]->in_water_timer==0
                    ? &EnemyRotatedSprite[rsid].draw_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id]
                    : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite2[Enemy[i]->current_rot_sprite_angle_id]),
                flip_bool
              );                
            }



          } else { //sprite flying
            if (!(Enemy[i]->species>=5 && Enemy[i]->species<=7)) {
              if (Enemy[i]->sprite_timer%2==0) {
                /*if (Enemy[i]->sprite_timer%4==0) {
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y+1,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
                } else {
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyTypeSprite[etype].draw_fly_sprite_1,Enemy[i]->last_left);
                }*/
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,
                    Enemy[i]->sprite_timer%4==0
                      ? Enemy[i]->sprite_y+1
                      : Enemy[i]->sprite_y,
                    Enemy[i]->in_water_timer==0 
                      ? &EnemyTypeSprite[etype].draw_fly_sprite_1
                      : &EnemyTypeSprite[etype].draw_dithered_fly_sprite_1,
                    Enemy[i]->last_left
                );
              } else {
                /*if (Enemy[i]->sprite_timer%3==0) {
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y-2,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
                } else {
                  DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y-1,&EnemyTypeSprite[etype].draw_fly_sprite_2,Enemy[i]->last_left);
                }*/
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,
                    Enemy[i]->sprite_timer%3==0
                      ? Enemy[i]->sprite_y-2
                      : Enemy[i]->sprite_y-1,
                    Enemy[i]->in_water_timer==0 
                      ? &EnemyTypeSprite[etype].draw_fly_sprite_2
                      : &EnemyTypeSprite[etype].draw_dithered_fly_sprite_2,
                    Enemy[i]->last_left
                );
              }
            } else {
              /*if (Enemy[i]->sprite_timer%4==0) {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite1[6],Enemy[i]->last_left);
              } else {
                DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemyRotatedSprite[rsid].draw_rotated_sprite2[6],Enemy[i]->last_left);
              }*/                   
              DrawSprite(hdc,hdc2,Enemy[i]->sprite_x,Enemy[i]->sprite_y,
                Enemy[i]->sprite_timer%4==0
                  ? (Enemy[i]->in_water_timer==0
                    ? &EnemyRotatedSprite[rsid].draw_rotated_sprite1[6]
                    : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite1[6])
                  :  (Enemy[i]->in_water_timer==0
                    ? &EnemyRotatedSprite[rsid].draw_rotated_sprite2[6]
                    : &EnemyRotatedSprite[rsid].draw_dithered_rotated_sprite2[6]),
                Enemy[i]->last_left
              );
            }
          }
          }
          break;
        }
      }


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
          GrRect(hdc,Enemy[i]->sprite_x-health_length/2-2,
                    Enemy[i]->sprite_y-58-2,
                    health_length+4,
                    8,DKRLTGRAY);
          GrRect(hdc,Enemy[i]->sprite_x-health_length/2,
                    Enemy[i]->sprite_y-58-1,
                    health_length,
                    6,c);
        } else {
          GrRect(hdc,Enemy[i]->sprite_x-health_length/2-2,
                    Enemy[i]->sprite_y-26-2,
                    health_length+4,
                    8,DKRLTGRAY);
          GrRect(hdc,Enemy[i]->sprite_x-health_length/2,
                    Enemy[i]->sprite_y-26-1,
                    health_length,
                    6,c);
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
      }
    }
  }
}

