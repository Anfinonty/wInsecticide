
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
  Enemy[enemy_id]->node_num=MAX_NODE_NUM;//MAX_FOLLOW_RANGE*MAX_FOLLOW_RANGE;//Enemy[enemy_id]->follow_range*Enemy[enemy_id]->follow_range;
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
//  for (i=0;i<MAX_NODE_NUM/2;i++) {
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
            //Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_solid;
            if (GetOnGroundId(x,y,NODE_SIZE,NODE_SIZE-1)!=-1) {
              Enemy[enemy_id]->node_solid[i]=TRUE;
            } else {
              Enemy[enemy_id]->node_solid[i]=FALSE;
            }
	        break;
	      case 1://inverse
            if (!Enemy[enemy_id]->target_player || (Enemy[enemy_id]->on_ground_id!=-1 && !Enemy[enemy_id]->is_in_ground_edge)) {
              //Enemy[enemy_id]->node_solid[i]=!NodeGrid[node_id]->node_solid;
              if (GetOnGroundId(x,y,NODE_SIZE,NODE_SIZE-1)!=-1) {
                Enemy[enemy_id]->node_solid[i]=FALSE;
              } else {
                Enemy[enemy_id]->node_solid[i]=TRUE;
              }
	          if (!Enemy[enemy_id]->node_solid[i]) {
                Enemy[enemy_id]->enemy_species1_solids[Enemy[enemy_id]->species1_solid_num]=i;
	            Enemy[enemy_id]->species1_solid_num++;
              }
            } else{
              //Enemy[enemy_id]->node_solid[i]=NodeGrid[node_id]->node_solid;
              if (GetOnGroundId(x,y,NODE_SIZE,NODE_SIZE-1)!=-1) {
                Enemy[enemy_id]->node_solid[i]=TRUE;
              } else {
                Enemy[enemy_id]->node_solid[i]=FALSE;
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
  if (Enemy[enemy_id]->species==1) {//species 1
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
//  for (i=0;i<MAX_NODE_NUM/2;i++) {//reset open_nodes array
  for (i=0;i<MAX_NODE_NUM;i++) {
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
          case 0:
	    y-=NODE_SIZE;
            break;
          case 2:
	    y+=NODE_SIZE;
            break;
        }
      } else if (3<=i && i<=5) {
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

void EnemyReboundFromGround(int enemy_id,int ground_id,bool is_rebound)
{
    double ground_entity_angle=GetLineTargetAngle(ground_id,Enemy[enemy_id]->x,Enemy[enemy_id]->y);
    double height_from_ground=GetLineTargetHeight(ground_id,ground_entity_angle,Enemy[enemy_id]->x,Enemy[enemy_id]->y);
    double edge_1_dist=GetDistance(Enemy[enemy_id]->x,Enemy[enemy_id]->y,Ground[ground_id]->x1,Ground[ground_id]->y1);
    double edge_2_dist=GetDistance(Enemy[enemy_id]->x,Enemy[enemy_id]->y,Ground[ground_id]->x2,Ground[ground_id]->y2);

    //double ground_entity_angle2=GetLineTargetAngle(Enemy[enemy_id]->saved_ground_id,Enemy[enemy_id]->x,Enemy[enemy_id]->y);
    //double height_from_ground2=GetLineTargetHeight(Enemy[enemy_id]->saved_ground_id,ground_entity_angle2,Enemy[enemy_id]->x,Enemy[enemy_id]->y);

    Enemy[enemy_id]->in_air_timer=0;

    if (abs(height_from_ground)<=31) {
      if (height_from_ground>0) {    //species 1 above ground (positive)
        Enemy[enemy_id]->angle=Ground[ground_id]->angle;
        Enemy[enemy_id]->above_ground=TRUE;
        Enemy[enemy_id]->below_ground=FALSE;
        Enemy[enemy_id]->flip_sprite=FALSE;
      } else {    //species 1 below ground
        Enemy[enemy_id]->angle=-Ground[ground_id]->angle-M_PI;
        Enemy[enemy_id]->above_ground=FALSE;
        Enemy[enemy_id]->below_ground=TRUE;
        if(Enemy[enemy_id]->last_left) {
          Enemy[enemy_id]->flip_sprite=FALSE;
        } else {
          Enemy[enemy_id]->flip_sprite=TRUE;
        }
      //Drop down (Old Feature Depracated)
      /*if (player.print_current_above && Enemy[enemy_id]->saw_player) {
      	if (player.y>Enemy[enemy_id]->y-60 && player.y<Enemy[enemy_id]->y-30 && !Enemy[enemy_id]->idling) {//enemy above
	      if (!Enemy[enemy_id]->ignore_player && RandNum(0,5,Enemy[enemy_id]->seed)==1) {
            Enemy[enemy_id]->y+=0.25;
	      }
        }
      }*/
      }
    } /*else {
      Enemy[enemy_id]->on_ground_id=-1;
    }*/

    if (Enemy[enemy_id]->species==1) {
      if (edge_1_dist<=31 || edge_2_dist<=31) {
        //Enemy[enemy_id]->above_ground=
        //Enemy[enemy_id]->below_ground=FALSE;
        Enemy[enemy_id]->in_air_timer=2;
        Enemy[enemy_id]->is_in_ground_edge=TRUE;
      } else {
        Enemy[enemy_id]->is_in_ground_edge=FALSE;
      }
    }


    if (abs(height_from_ground)<=25 && is_rebound) {
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

}

void EnemySpecies1Gravity(int enemy_id)
{
//  Enemy[enemy_id]->on_ground_id=GetOnGroundIdE(Enemy[enemy_id]->x,Enemy[enemy_id]->y,30,29,enemy_id);    //Get Ground id

  Enemy[enemy_id]->on_ground_id=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,30,29);    //Get 
  if (Enemy[enemy_id]->on_ground_id==-1) {//not on ground
    Enemy[enemy_id]->y+=1; //falling down
    Enemy[enemy_id]->in_air_timer=2;
    Enemy[enemy_id]->above_ground=FALSE;
    Enemy[enemy_id]->below_ground=FALSE;
  } else {//on ground
    EnemyReboundFromGround(enemy_id,Enemy[enemy_id]->on_ground_id,TRUE);
  }
  Enemy[enemy_id]->previous_above_ground=Enemy[enemy_id]->above_ground;
  Enemy[enemy_id]->previous_below_ground=Enemy[enemy_id]->below_ground;
  Enemy[enemy_id]->saved_ground_id=Enemy[enemy_id]->on_ground_id;
}

void EnemyMove(int enemy_id)
{
  int path_node_arr_id=Enemy[enemy_id]->path_nodes_num-1,
      path_node_id=Enemy[enemy_id]->path_nodes[path_node_arr_id];
  double path_node_center_x=Enemy[enemy_id]->node_x[path_node_id]+NODE_SIZE/2,
      path_node_center_y=Enemy[enemy_id]->node_y[path_node_id]+NODE_SIZE/2;


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



  if (Enemy[enemy_id]->species==1) {
    Enemy[enemy_id]->sprite_timer++;
    if (Enemy[enemy_id]->sprite_timer>16) {
      Enemy[enemy_id]->sprite_timer=0;
    }
  }

  //bool allow_act=TRUE;
  //int tmp_node_id=GetGridId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM); //all nodes
  //if (tmp_node_id!=-1) {
  //  if (NodeGrid[tmp_node_id]->node_solid)
  //    allow_act=FALSE;
  //}

  int tmp_on_ground_id=-1;

  if (!Enemy[enemy_id]->is_ground_rebounding) {
    tmp_on_ground_id=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,5,4);    //Get Ground id
    if (tmp_on_ground_id!=-1) {
      Enemy[enemy_id]->is_ground_rebounding=TRUE;
    } else {
      if (Enemy[enemy_id]->x<path_node_center_x) {
          Enemy[enemy_id]->last_left=FALSE;
          Enemy[enemy_id]->x+=Enemy[enemy_id]->speed;
        } else {
          Enemy[enemy_id]->last_left=TRUE;
          Enemy[enemy_id]->x-=Enemy[enemy_id]->speed;  
        }
        switch (Enemy[enemy_id]->species) {
          case 0://fly
            if (Enemy[enemy_id]->y<path_node_center_y) {
              Enemy[enemy_id]->y+=Enemy[enemy_id]->speed;
            } else {
              Enemy[enemy_id]->y-=Enemy[enemy_id]->speed;
            }
            break;
          case 1://roach
            if (Enemy[enemy_id]->y<path_node_center_y) {
              Enemy[enemy_id]->y+=Enemy[enemy_id]->speed;
            } else {
              Enemy[enemy_id]->y-=Enemy[enemy_id]->speed;
            }
            break;
      }
    }
  } else {
    if (Enemy[enemy_id]->species==1) {
      tmp_on_ground_id=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,25,24);
    } else {
      //tmp_on_ground_id=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,5,4);
      tmp_on_ground_id=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,10,9);
    }
    //tmp_on_ground_id=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,10,9);
    if (tmp_on_ground_id==-1) {
      Enemy[enemy_id]->is_ground_rebounding=FALSE;
    } else {    
      EnemyReboundFromGround(enemy_id,tmp_on_ground_id,TRUE);
    }
  }

  if (Enemy[enemy_id]->species==1) {
    int tmp_on_ground_id2=GetOnGroundId(Enemy[enemy_id]->x,Enemy[enemy_id]->y,30,29);
    //if (tmp_on_ground_id2==-1) {
      //Enemy[enemy_id]->in_air_timer=2;
      //Enemy[enemy_id]->above_ground=FALSE;
      //Enemy[enemy_id]->below_ground=FALSE;
    //} else {
      //Enemy[enemy_id]->in_air_timer=0;
    //}
    //Enemy[enemy_id]->previous_above_ground=Enemy[enemy_id]->above_ground;
    //Enemy[enemy_id]->previous_below_ground=Enemy[enemy_id]->below_ground;
    //Enemy[enemy_id]->saved_ground_id=tmp_on_ground_id2;
     if (tmp_on_ground_id2!=-1) {
       Enemy[enemy_id]->on_ground_id=tmp_on_ground_id2;
       EnemyReboundFromGround(enemy_id,tmp_on_ground_id2,FALSE);
     } 
     if (Enemy[enemy_id]->on_ground_id==-1) {
       Enemy[enemy_id]->in_air_timer=2;
       Enemy[enemy_id]->on_ground_id=-1;
       Enemy[enemy_id]->above_ground=FALSE;
       Enemy[enemy_id]->below_ground=FALSE;
     }
   }

  if (Enemy[enemy_id]->on_ground_id==-1) {
    Enemy[enemy_id]->is_in_ground_edge=FALSE;
  }

  /*if (tmp_on_ground_id==-1) {
    //Enemy[enemy_id]->in_air_timer=2;
    //Enemy[enemy_id]->above_ground=FALSE;
    //Enemy[enemy_id]->below_ground=FALSE;

    
  } else {
    //Enemy[enemy_id]->idling=TRUE;
    //Enemy[enemy_id]->move_to_target=FALSE;
    //EnemyReboundFromGround(enemy_id,tmp_on_ground_id);
  }*/


  if (path_node_center_y-1<=Enemy[enemy_id]->y && Enemy[enemy_id]->y<=path_node_center_y+1 &&
      path_node_center_x-1<=Enemy[enemy_id]->x && Enemy[enemy_id]->x<=path_node_center_x+1) {
    Enemy[enemy_id]->path_nodes_num--;
    if (path_node_arr_id<=0) { //all nodes followed
      Enemy[enemy_id]->idling=TRUE;
      Enemy[enemy_id]->move_to_target=FALSE;
    }
  }
  /*Enemy[enemy_id]->previous_above_ground=Enemy[enemy_id]->above_ground;
  Enemy[enemy_id]->previous_below_ground=Enemy[enemy_id]->below_ground;
  Enemy[enemy_id]->saved_ground_id=tmp_on_ground_id;*///Enemy[enemy_id]->on_ground_id;
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
  if (Enemy[i]->species==1 &&
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
    if (Enemy[i]->species==0 && Enemy[i]->node_solid[target_node]) {
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


void EnemyKnockbackMove(int i)
{
  bool allow_act=FALSE;
  switch (Enemy[i]->species) {
    case 0:
      if (GetOnGroundId(Enemy[i]->x,Enemy[i]->y,14,13)!=-1) {
  	    allow_act=TRUE;
      }
      break;
    case 1:
      if (GetOnGroundId(Enemy[i]->x,Enemy[i]->y,24,23)!=-1) { //dont knockback enemy  //warning: can cause enemy to clip  through grounds
	    allow_act=TRUE;
      }
      break;
  }
  /*int tmp_node_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM); //all nodes
  if (tmp_node_id!=-1) {
    if (NodeGrid[tmp_node_id]->node_solid)
      allow_act=TRUE;
  }*/

  int tmp_ground_id=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,NODE_SIZE,NODE_SIZE-1); //all nodes
  if (tmp_ground_id!=-1) {
    allow_act=TRUE;
  }
// ^^ condition
  if (allow_act || IsOutOfBounds(Enemy[i]->x,Enemy[i]->y,5,MAP_WIDTH,MAP_HEIGHT)) {
    Enemy[i]->knockback_timer=0;
  } else {
    Enemy[i]->on_ground_id=-1;
    Enemy[i]->saved_ground_id=-1;
    Enemy[i]->angle=0;

    if (!Enemy[i]->player_knockback) {
      Enemy[i]->x+=cos(Enemy[i]->knockback_angle)*player.knockback_speed;
      Enemy[i]->y+=sin(Enemy[i]->knockback_angle)*player.knockback_speed;      
    } else {
      if (Enemy[i]->knockback_left) {
        Enemy[i]->x-=cos(Enemy[i]->knockback_angle)*player.knockback_speed;
        Enemy[i]->y-=sin(Enemy[i]->knockback_angle)*player.knockback_speed;      
      } else {
        Enemy[i]->x+=cos(Enemy[i]->knockback_angle)*player.knockback_speed;
        Enemy[i]->y+=sin(Enemy[i]->knockback_angle)*player.knockback_speed;      
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


  if (Enemy[i]->health>0) {


    //Enemy bullet
    for (j=0;j<Enemy[i]->bullet_shot_num;j++) {
      BulletAct(Enemy[i]->bullet_shot_arr[j]);
    }
    EnemyLOSAct(i);//Shoot line of sight bullet


    if (Enemy[i]->damage_taken_timer>0) {
      Enemy[i]->damage_taken_timer--;
    }

    for (int k=0;k<player.bullet_shot_num;k++) {
      int bk=player.bullet[k];
      double dist_from_bullet0=GetDistance(Bullet[bk].x,Bullet[bk].y,Enemy[i]->x,Enemy[i]->y);
    //^^ condition
      //player bullet
      if (dist_from_bullet0<=NODE_SIZE*4) {
        switch (Enemy[i]->species) {
      	  case 0://fly
            if (dist_from_bullet0<=NODE_SIZE*2) {
              Enemy[i]->damage_taken_timer=256;
              Enemy[i]->health-=Bullet[bk].damage;
              if (game_audio) {
                PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
                //PlaySound(L"snd/clang.wav", NULL, SND_FILENAME | SND_ASYNC);      
                //PlaySound(clang_audio_cache, NULL, SND_MEMORY | SND_ASYNC);
              }
              Enemy[i]->knockback_timer=player.knockback_strength;
              Enemy[i]->knockback_angle=Bullet[bk].angle;
              Enemy[i]->player_knockback=FALSE;
              if (Bullet[bk].graphics_type!=6)
                Bullet[bk].angle=RandAngle(0,360,player.seed);
              /*if (Bullet[bk].speed_multiplier<6) {
              }*/
            }
	        break;
         case 1://crawl
            Enemy[i]->damage_taken_timer=256;
            Enemy[i]->health-=Bullet[bk].damage;
            if (game_audio) {
              PlaySound(spamSoundEffectCache[2].audio,NULL, SND_MEMORY | SND_ASYNC);
              //PlaySound(L"snd/clang.wav", NULL, SND_FILENAME | SND_ASYNC);      
              //PlaySound(clang_audio_cache, NULL, SND_MEMORY | SND_ASYNC);
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
    } //end

    //sniper web bullet
    double dist_from_bullet=GetDistance(Bullet[player.bullet_shot].x,Bullet[player.bullet_shot].y,Enemy[i]->x,Enemy[i]->y);
    if (dist_from_bullet<=NODE_SIZE*4) {
      switch (Enemy[i]->species) {
	    case 0://fly
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

    
   //Enemy knockback & attacked
    allow_act=FALSE;
    double distance_from_player_claws=GetDistance(Enemy[i]->x,Enemy[i]->y,player.claws_x,player.claws_y);
    switch (Enemy[i]->species) {
      case 0://fly
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
	    if (distance_from_player_claws<=NODE_SIZE*4) {
	      allow_act=TRUE;
	    }
	    break;
    }
    //^^ condition
    /*if (allow_act) {
      //within player claws
      allow_act=FALSE;
	  if (!player.time_breaker) {
	    //if (Enemy[i]->knockback_timer==0) {
	      allow_act=TRUE;
	    //}
	  } else {
	    //if (!Enemy[i]->time_breaker_immune) {
	      allow_act=TRUE;
	    //} else if (Enemy[i]->knockback_timer==0) {
	      //allow_act=TRUE;
	    //}
	  }
    }*/
    //^^ condition 
    if (allow_act) {
      allow_act=FALSE;
      switch (Enemy[i]->species) {
        case 0:
          if (player.attack_timer>34) {
            allow_act=TRUE;
          }
	      break;
	    case 1:
	      /*if (player.attack_timer<=39) {
		    /*if (player.attack_timer>34) { //more damage to roach at underside when player is upside down
		      if (Enemy[i]->above_ground) {
                if (player.print_current_below && player.y<Enemy[i]->y-6) {
  		          allow_act=TRUE;
                }
		      } else if (Enemy[i]->below_ground) { //more damage to roach at underside when player isnot upside down
		        if ((player.print_current_above || player.on_ground_id==-1) && player.y>Enemy[i]->y+6) {
		          allow_act=TRUE;
		        }
		      }
		    }*/
	      //}
          if (player.attack_timer>38)
            allow_act=TRUE;
	      break;
	  }
    }
	// ^^ condition
    if (allow_act) {  //player meelee
      allow_act=FALSE;
      //mele attack (legacy feature)
      deduct_health=TRUE;
      Enemy[i]->player_knockback=TRUE;
      Enemy[i]->knockback_timer=player.knockback_strength;
      if (!player.uppercut && !player.rst_up && !player.rst_down) {//normal
        Enemy[i]->knockback_angle=player.angle;
      } else if (player.uppercut) {//uppercut
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
      } else { //drag enemy down
        if (!player.last_left) {//drag enemy to player
          Enemy[i]->knockback_angle=player.angle+M_PI_2;
        } else {
          Enemy[i]->knockback_angle=player.angle-M_PI_2;
        }
      }

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
            if (player.speed>5) deduct_health=TRUE;
            break;
          case 1:
            if (player.speed>10) deduct_health=TRUE;
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
        //PlaySound(clang_audio_cache, NULL, SND_MEMORY | SND_ASYNC);
        //PlaySound(L"snd/clang.wav", NULL, SND_FILENAME | SND_ASYNC);
      }
    }


    //Knockback 
    if (Enemy[i]->knockback_timer>0) {
      if (!player.time_breaker || Enemy[i]->time_breaker_immune) {
        Enemy[i]->knockback_timer--;
        if (Enemy[i]->knockback_timer>20) {
          knock_max=player.knockback_speed_multiplier*2;
        } else {
          knock_max=player.knockback_speed_multiplier;
        }
        for (j=0;j<knock_max;j++) {
          EnemyKnockbackMove(i);
        }
      }
    }

    //Pathfinding and movement only act when within render distance
    if ((!player.time_breaker || Enemy[i]->time_breaker_immune) && Enemy[i]->within_render_distance) {

      //timebreaker enemy
      if (Enemy[i]->time_breaker_length>0 && !player.time_breaker) {
        dice=RandNum(0,Enemy[i]->time_breaker_rare,Enemy[i]->seed);
        if (dice==1) {
          slash_time=Enemy[i]->time_breaker_length;
        }
      }


      for (slash_time_i=0;slash_time_i<slash_time;slash_time_i++) {
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

        if (Enemy[i]->species==1 && !Enemy[i]->move_to_target && !Enemy[i]->target_player) {//Species 1 gravity
          if (slash_time_i==0) {
            EnemySpecies1Gravity(i);
            if (Enemy[i]->in_air_timer>0) {
              Enemy[i]->in_air_timer--;
            }
          }
        }
        /*int above_player_node1=GetGridId(
                player.above_x-Enemy[i]->node_x[0],
                player.above_y-Enemy[i]->node_y[0],
                Enemy[i]->follow_range*NODE_SIZE,
                NODE_SIZE,Enemy[i]->node_num);*/

        int enemy_on_node_grid_id=GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM);

        //Raining
        if (raining && !Enemy[i]->move_to_target) {
          if (enemy_on_node_grid_id!=-1) {
            if (NodeGrid[enemy_on_node_grid_id]->node_no_shade || NodeGrid[enemy_on_node_grid_id]->tmp_wet) {
              Enemy[i]->ignore_player=TRUE;
              force_search=TRUE;
              Enemy[i]->idle_timer=31;
              Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier*2;
            } else {
              if (Enemy[i]->speed_multiplier!=Enemy[i]->ospeed_multiplier)
                Enemy[i]->speed_multiplier=Enemy[i]->ospeed_multiplier;
            }
          }
        }
        if (Enemy[i]->is_in_ground_edge) {
          force_search=TRUE;
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
	            for (j=0;j<Enemy[i]->bullet_fire_at_once_max;j++) {//several bullets at once
		          if (Enemy[i]->saw_player) {
                    if (Enemy[i]->dist_from_player<Enemy[i]->shoot_at_player_range/2*NODE_SIZE) {
    		          Enemy[i]->shoot_target_x=Enemy[i]->bullet_head_x[j];
        		      Enemy[i]->shoot_target_y=Enemy[i]->bullet_head_y[j];
                      ShootBullet(current_bullet_id,
		                Enemy[i]->bullet_shot_num,
		                Enemy[i]->bullet_color,
		                Enemy[i]->bullet_graphics_type,
		                Enemy[i]->bullet_range,
		                Enemy[i]->bullet_speed,
		                Enemy[i]->bullet_speed_multiplier,
		                Enemy[i]->bullet_damage,
		                i,
		                Enemy[i]->x,
		                Enemy[i]->y,
		                Enemy[i]->x,
		                Enemy[i]->y,
		                Enemy[i]->shoot_target_x,
		                Enemy[i]->shoot_target_y,
                        0
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
        EnemyMove(i);
      }
      if (!Enemy[i]->ignore_player && Enemy[i]->saw_player) { //chasing player
        int tmp_enemy_on_ground_id=GetOnGroundId(Enemy[i]->x,Enemy[i]->y,3,2);
        if (Enemy[i]->species==0 &&
            //NodeGrid[GetGridId(Enemy[i]->x,Enemy[i]->y,MAP_WIDTH,NODE_SIZE,MAP_NODE_NUM)]->node_solid)
            tmp_enemy_on_ground_id>=GROUND_NUM)
        {   //become blind when inside a solid
          Enemy[i]->saw_player=FALSE;
          Enemy[i]->idling=TRUE;
          Enemy[i]->move_to_target=FALSE;
        }
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
        if (Enemy[i]->species==0) {
          Enemy[i]->sprite_timer++;
          if (Enemy[i]->sprite_timer>3) {
            Enemy[i]->sprite_timer=0;
          }
        }


      }
    }//end of tbt
  }//end of health
  Enemy[i]->sprite_x=Enemy[i]->x+player.cam_x+player.cam_move_x+player.cam_mouse_move_x;
  Enemy[i]->sprite_y=Enemy[i]->y+player.cam_y+player.cam_move_y+ player.cam_mouse_move_y;

}

void SetEnemyByType(int i,int type)
{
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
  Enemy[i]->color=color_arr[saved_enemy_type_color[type]];
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
  Enemy[i]->bullet_color=color_arr[saved_enemy_type_bullet_color[type]];
  Enemy[i]->bullet_graphics_type=saved_enemy_type_bullet_graphics_type[type];
  //time breaker
  Enemy[i]->time_breaker_rare=saved_enemy_type_time_breaker_rare[type];
  Enemy[i]->time_breaker_length=saved_enemy_type_time_breaker_length[type];
}

void CleanUpEnemySprites()
{
  for (int i=0;i<ENEMY_NUM;i++) {
    if (EnemySprite[i]->sprite_1!=NULL) {
      DeleteObject(EnemySprite[i]->sprite_1);
      EnemySprite[i]->sprite_1=NULL;
    }
    if (EnemySprite[i]->sprite_2!=NULL) {
      DeleteObject(EnemySprite[i]->sprite_2);
      EnemySprite[i]->sprite_2=NULL;
    }
    if (EnemySprite[i]->sprite_3!=NULL) {
      DeleteObject(EnemySprite[i]->sprite_3);
      EnemySprite[i]->sprite_3=NULL;
    }

    FreeDrawSprite(&EnemySprite[i]->draw_sprite_1);
    FreeDrawSprite(&EnemySprite[i]->draw_sprite_2);
    FreeDrawSprite(&EnemySprite[i]->draw_sprite_3);
  }
}


void InitEnemySprites()
{
  CleanUpEnemySprites();
  for (int i=0;i<ENEMY_NUM;i++) {
    Enemy[i]->current_draw_row=-9999;    
    if (Enemy[i]->species==0) {
      if (EnemySprite[i]->sprite_1==NULL) {
        EnemySprite[i]->sprite_1=RotateSprite(NULL, enemy1_sprite_1,0,LTGREEN,BLACK,Enemy[i]->color,-1);
      }
      if (EnemySprite[i]->sprite_2==NULL) {
        EnemySprite[i]->sprite_2=RotateSprite(NULL, enemy1_sprite_2,0,LTGREEN,BLACK,Enemy[i]->color,-1);
      }
      EnemySprite[i]->sprite_3=NULL;
      
      GenerateDrawSprite(&EnemySprite[i]->draw_sprite_1,EnemySprite[i]->sprite_1);
      GenerateDrawSprite(&EnemySprite[i]->draw_sprite_2,EnemySprite[i]->sprite_2);
    } else {
      if (EnemySprite[i]->sprite_1==NULL) {
        EnemySprite[i]->sprite_1=RotateSprite(NULL, enemy2_sprite_1,0,LTGREEN,BLACK,Enemy[i]->color,-1);
      }
      if (EnemySprite[i]->sprite_2==NULL) {
        EnemySprite[i]->sprite_2=RotateSprite(NULL, enemy2_sprite_2,0,LTGREEN,BLACK,Enemy[i]->color,-1);
      }
      if (EnemySprite[i]->sprite_3==NULL) {
        EnemySprite[i]->sprite_3=RotateSprite(NULL, enemy2_sprite_3,0,LTGREEN,BLACK,Enemy[i]->color,-1);
      }
      GenerateDrawSprite(&EnemySprite[i]->draw_sprite_1,EnemySprite[i]->sprite_1);
      GenerateDrawSprite(&EnemySprite[i]->draw_sprite_2,EnemySprite[i]->sprite_2);
      GenerateDrawSprite(&EnemySprite[i]->draw_sprite_3,EnemySprite[i]->sprite_3);
    }
  }
}


//bool once=TRUE;
void InitEnemy()
{
  int i=0,j=0,x=0,y=0;
  for (i=0;i<ENEMY_NUM;i++) {
    //printf("Enemy: %d\n",i);
    Enemy[i]->being_drawn=TRUE;
    Enemy[i]->on_ground_id=-1;
    Enemy[i]->saved_ground_id=-1;
    Enemy[i]->seed=0;
    Enemy[i]->dist_from_player=999;
    Enemy[i]->x=saved_enemy_x[i];
    Enemy[i]->y=saved_enemy_y[i];
    Enemy[i]->sprite_x=
    Enemy[i]->sprite_y=-20;
    Enemy[i]->is_ground_rebounding=FALSE;
    Enemy[i]->is_in_ground_edge=FALSE;
    SetEnemyByType(i,saved_enemy_type[i]);
    if (Enemy[i]->x<5) {
      Enemy[i]->x=25;
    }
    if (Enemy[i]->y<5) {
      Enemy[i]->y=25;
    }
    Enemy[i]->previous_above_ground=
    Enemy[i]->previous_below_ground=
    Enemy[i]->above_ground=
    Enemy[i]->below_ground=FALSE;
    Enemy[i]->in_air_timer=0;
    Enemy[i]->angle=0;
    Enemy[i]->sprite_angle=0;
    Enemy[i]->saved_angle=-9999;
    Enemy[i]->damage_taken_timer=0;
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
    Enemy[i]->cosine=0;
    Enemy[i]->sine=0;

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
//    for (j=0;j<MAX_NODE_NUM/2;j++) {
    for (j=0;j<MAX_NODE_NUM;j++) {
      Enemy[i]->open_nodes[j]=Enemy[i]->start_node;
    }
    EnemyAct(i);
  }
}


//int cenemy=0;
void DrawEnemy(HDC hdc)
{
  int i=0,k=0,c=0,c2=0;
  //int min_enemy=0, max_enemy=100;
  if (frame_tick==-8) { //initiate on start of app
    for (i=0;i<ENEMY_NUM;i++) {
      if (Enemy[i]->species==1) {
        DeleteObject(EnemySprite[i]->sprite_3);
        FreeDrawSprite(&EnemySprite[i]->draw_sprite_3);
        EnemySprite[i]->sprite_3=RotateSprite(hdc, enemy2_sprite_3,0,LTGREEN,BLACK,Enemy[i]->color,-1);
        GenerateDrawSprite(&EnemySprite[i]->draw_sprite_3,EnemySprite[i]->sprite_3);
      }
    }
  }



  /*if (ENEMY_NUM>20) {
    switch (cenemy) {
      case 0:
        min_enemy=0;
        max_enemy=ENEMY_NUM/4;
        break;
      case 1:
        min_enemy=ENEMY_NUM/4;
        max_enemy=ENEMY_NUM/2;
        break;
      case 2:
        min_enemy=ENEMY_NUM/2;
        max_enemy=ENEMY_NUM/2+ENEMY_NUM/4;
        break;
      case 3:
        min_enemy=ENEMY_NUM/2+ENEMY_NUM/4;
        max_enemy=ENEMY_NUM/2;
        break;
    }
    cenemy=LimitValue(cenemy+1,0,4);
  } else {
    cenemy=0;
    min_enemy=0;
    max_enemy=ENEMY_NUM;
  }*/
  

  for (i=0;i<ENEMY_NUM;i++) {  
  //for (i=min_enemy;i<max_enemy;i++) {  
    if (Enemy[i]->species==1 && Enemy[i]->within_render_distance) {//rotate sprite
      if (Enemy[i]->on_ground_id!=-1) {
        Enemy[i]->sprite_angle=Enemy[i]->angle;
        if (!Enemy[i]->last_left) {
          Enemy[i]->sprite_angle*=-1;
        }
      }
    }


    //Drawing operations, enemy is living
    if (Enemy[i]->health>0) { //enemy is alive
      Enemy[i]->seed=rand();
      for (k=0;k<Enemy[i]->bullet_shot_num;k++) {
        DrawBullet(hdc,Enemy[i]->bullet_shot_arr[k]);
      }

      if (Enemy[i]->species==1 && Enemy[i]->within_render_distance) { //Cockroach sprite species1
        if (Enemy[i]->sprite_angle!=Enemy[i]->saved_angle) { // enemy is on ground
          Enemy[i]->being_drawn=TRUE;
          if (EnemySprite[i]->sprite_1!=NULL) { //delete old sprites
            DeleteObject(EnemySprite[i]->sprite_1);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_1);
          }
          if (EnemySprite[i]->sprite_2!=NULL) {
            DeleteObject(EnemySprite[i]->sprite_2);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_2);
          }

          SetRotatedSpriteSize(
            NULL,
            enemy2_sprite_1,
            Enemy[i]->sprite_angle,
            &Enemy[i]->sprite_minx,
            &Enemy[i]->sprite_miny,
            &Enemy[i]->sprite_maxx,
            &Enemy[i]->sprite_maxy,
            &Enemy[i]->sprite_width,
            &Enemy[i]->sprite_height,
            &Enemy[i]->cosine,
            &Enemy[i]->sine
          );

          EnemySprite[i]->sprite_1=CreateCrunchyBitmap(Enemy[i]->sprite_width,Enemy[i]->sprite_height); //create new sprite with new height and width
          EnemySprite[i]->sprite_2=CreateCrunchyBitmap(Enemy[i]->sprite_width,Enemy[i]->sprite_height);

          Enemy[i]->current_draw_row=Enemy[i]->sprite_miny;

          Enemy[i]->saved_angle=Enemy[i]->sprite_angle;
        } //else { // sprite_angle==saved angle

        //for (int k=0;k<2;k++) {
          if (Enemy[i]->current_draw_row>=Enemy[i]->sprite_miny && Enemy[i]->current_draw_row<=Enemy[i]->sprite_maxy) {
            RotateSpriteII(hdc, enemy2_sprite_1, EnemySprite[i]->sprite_1,Enemy[i]->cosine, Enemy[i]->sine, LTGREEN, Enemy[i]->color, -1, Enemy[i]->sprite_minx, Enemy[i]->sprite_miny, Enemy[i]->sprite_maxx, Enemy[i]->sprite_maxy, Enemy[i]->current_draw_row); 
            RotateSpriteII(hdc, enemy2_sprite_2, EnemySprite[i]->sprite_2,Enemy[i]->cosine, Enemy[i]->sine, LTGREEN, Enemy[i]->color, -1, Enemy[i]->sprite_minx, Enemy[i]->sprite_miny, Enemy[i]->sprite_maxx, Enemy[i]->sprite_maxy, Enemy[i]->current_draw_row);
            Enemy[i]->current_draw_row++;
            //printf("===Drawing!\n");
            if (Enemy[i]->current_draw_row>=Enemy[i]->sprite_maxy) {
              Enemy[i]->being_drawn=FALSE;
              Enemy[i]->current_draw_row=-9999;
              GenerateDrawSprite(&EnemySprite[i]->draw_sprite_1,EnemySprite[i]->sprite_1);
              GenerateDrawSprite(&EnemySprite[i]->draw_sprite_2,EnemySprite[i]->sprite_2);
              //printf("Finished drawing!\n");
            }
          }
        //}



      } else { //other species 0
        if (Enemy[i]->saved_angle==-9999) {
          if (EnemySprite[i]->sprite_1!=NULL) {
            DeleteObject(EnemySprite[i]->sprite_1);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_1);
          }
          if (EnemySprite[i]->sprite_2!=NULL) {
            DeleteObject(EnemySprite[i]->sprite_2);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_2);
          }

          EnemySprite[i]->sprite_1=RotateSprite(hdc, enemy1_sprite_1,Enemy[i]->sprite_angle,LTGREEN,BLACK,Enemy[i]->color,-1);
          EnemySprite[i]->sprite_2=RotateSprite(hdc, enemy1_sprite_2,Enemy[i]->sprite_angle,LTGREEN,BLACK,Enemy[i]->color,-1);
          GenerateDrawSprite(&EnemySprite[i]->draw_sprite_1,EnemySprite[i]->sprite_1);
          GenerateDrawSprite(&EnemySprite[i]->draw_sprite_2,EnemySprite[i]->sprite_2);
          Enemy[i]->saved_angle=0;
        }
      }

    //Enemy has died
    } else if (Enemy[i]->health>-1000 && Enemy[i]->health<=0){
      //Add to enemy kill count and buff player stats
      if (Enemy[i]->health>-500) {
        enemy_kills++;
        player.health+=2;
        player.block_health+=10;
        if (player.block_health>player.block_health_max) {
          player.block_health=player.block_health_max;
          player.health+=2;
        }
        player.show_health_timer=HP_SHOW_TIMER_NUM;
        player.show_block_health_timer=HP_SHOW_TIMER_NUM;
        if (player.max_web_num<MAX_WEB_NUM) {
          player.max_web_num++;
        }
        //Add to player stats after defeat
        if (!IsSpeedBreaking()) {
          if (player.time_breaker_units<player.time_breaker_units_max-2 && !player.time_breaker) {
            player.time_breaker_units+=2;
          }
        } else {
          player.speed+=2;
        }
      }



      if (Enemy[i]->species==1) {  //Cockroach sprite
        if (Enemy[i]->health>-500) {
          if (EnemySprite[i]->sprite_1!=NULL) {
            DeleteObject(EnemySprite[i]->sprite_1);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_1);
          }
          if (EnemySprite[i]->sprite_2!=NULL) {
            DeleteObject(EnemySprite[i]->sprite_2);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_2);
          }

          if (EnemySprite[i]->sprite_3!=NULL) {
            DeleteObject(EnemySprite[i]->sprite_3);
            FreeDrawSprite(&EnemySprite[i]->draw_sprite_3);
          }


          SetRotatedSpriteSize(
              NULL,
              enemy2_sprite_1,
              Enemy[i]->sprite_angle,
              &Enemy[i]->sprite_minx,
              &Enemy[i]->sprite_miny,
              &Enemy[i]->sprite_maxx,
              &Enemy[i]->sprite_maxy,
              &Enemy[i]->sprite_width,
              &Enemy[i]->sprite_height,
              &Enemy[i]->cosine,
              &Enemy[i]->sine
          );

          EnemySprite[i]->sprite_1=CreateCrunchyBitmap(Enemy[i]->sprite_width,Enemy[i]->sprite_height);
          EnemySprite[i]->sprite_2=CreateCrunchyBitmap(Enemy[i]->sprite_width,Enemy[i]->sprite_height);
          EnemySprite[i]->sprite_3=CreateCrunchyBitmap(Enemy[i]->sprite_width,Enemy[i]->sprite_height);

          Enemy[i]->current_draw_row=Enemy[i]->sprite_miny;
          Enemy[i]->play_death_snd=TRUE;
          Enemy[i]->health=-501;
        }


        if (Enemy[i]->current_draw_row>=Enemy[i]->sprite_miny && Enemy[i]->current_draw_row<=Enemy[i]->sprite_maxy && Enemy[i]->within_render_distance) {
          RotateSpriteII(hdc, enemy2_sprite_1, EnemySprite[i]->sprite_1,Enemy[i]->cosine,Enemy[i]->sine, LTGREEN, DKBLACK, TRANSPARENT, Enemy[i]->sprite_minx, Enemy[i]->sprite_miny, Enemy[i]->sprite_maxx, Enemy[i]->sprite_maxy,Enemy[i]->current_draw_row);
          RotateSpriteII(hdc, enemy2_sprite_2, EnemySprite[i]->sprite_2,Enemy[i]->cosine,Enemy[i]->sine, LTGREEN, DKBLACK, TRANSPARENT, Enemy[i]->sprite_minx, Enemy[i]->sprite_miny, Enemy[i]->sprite_maxx, Enemy[i]->sprite_maxy, Enemy[i]->current_draw_row);
          RotateSpriteII(hdc, enemy2_sprite_3, EnemySprite[i]->sprite_3,Enemy[i]->cosine,Enemy[i]->sine, LTGREEN, DKBLACK, TRANSPARENT, Enemy[i]->sprite_minx, Enemy[i]->sprite_miny, Enemy[i]->sprite_maxx, Enemy[i]->sprite_maxy, Enemy[i]->current_draw_row);
          Enemy[i]->current_draw_row++;
          if (Enemy[i]->current_draw_row>=Enemy[i]->sprite_maxy) {
            Enemy[i]->current_draw_row=-9999;
            Enemy[i]->health=-99999;
            GenerateDrawSprite(&EnemySprite[i]->draw_sprite_1,EnemySprite[i]->sprite_1);
            GenerateDrawSprite(&EnemySprite[i]->draw_sprite_2,EnemySprite[i]->sprite_2);
            GenerateDrawSprite(&EnemySprite[i]->draw_sprite_3,EnemySprite[i]->sprite_3);
          }
        }
      } else { //Fly sprite
        if (EnemySprite[i]->sprite_1!=NULL) {
          DeleteObject(EnemySprite[i]->sprite_1);
          FreeDrawSprite(&EnemySprite[i]->draw_sprite_1);
        }
        if (EnemySprite[i]->sprite_2!=NULL) {
          DeleteObject(EnemySprite[i]->sprite_2);
          FreeDrawSprite(&EnemySprite[i]->draw_sprite_2);
        }
        Enemy[i]->play_death_snd=TRUE;
        EnemySprite[i]->sprite_1=RotateSprite(hdc, enemy1_sprite_1,Enemy[i]->sprite_angle,LTGREEN,BLACK,DKBLACK,TRANSPARENT);
        EnemySprite[i]->sprite_2=RotateSprite(hdc, enemy1_sprite_2,Enemy[i]->sprite_angle,LTGREEN,BLACK,DKBLACK,TRANSPARENT);
        GenerateDrawSprite(&EnemySprite[i]->draw_sprite_1,EnemySprite[i]->sprite_1);
        GenerateDrawSprite(&EnemySprite[i]->draw_sprite_2,EnemySprite[i]->sprite_2);
        Enemy[i]->health=-99999;
      }
    }


    //enemy Display on screen operations
    if (/*Enemy[i]->saw_player &&*/ Enemy[i]->within_render_distance) {
      if (Enemy[i]->health>0 && Enemy[i]->damage_taken_timer>0) {
        //percentage 
        c = Highlight(IsInvertedBackground(),LTRED,LTCYAN);
        c2 = Highlight(IsInvertedBackground(),WHITE,BLACK);
        double percentage = Enemy[i]->health/Enemy[i]->max_health;
        double health_length_max=64;
        if (Enemy[i]->species==1) {
          health_length_max=100;
        }
        if (Enemy[i]->max_health>=150) {
          health_length_max=150;
        } else if (Enemy[i]->max_health>64) {
          health_length_max=Enemy[i]->max_health;
          if (Enemy[i]->species==1) {
            health_length_max=100;
          }
        }

        double health_length = health_length_max*percentage;
        if (Enemy[i]->species==1) {
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
          char txt[16];
          int print_health=Enemy[i]->health;
          if (print_health>1) {
            sprintf(txt,"%d",print_health);        
          } else {
            sprintf(txt,"1");
          }
          int sprite_x_health=(int)Enemy[i]->sprite_x-strlen(txt)*8/2;
          if (Enemy[i]->species==1) {
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


      //char mytxt[20];
      //sprintf(mytxt,"%d",Enemy[i]->on_ground_id);
      //GrPrint(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y-64,mytxt,LTGREEN);

      switch (Enemy[i]->species) {
        case 0:
          if (Enemy[i]->sprite_timer%2==0) {
            DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_1,Enemy[i]->last_left);
          } else {
            DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_2,Enemy[i]->last_left);
          }
         // if (Enemy[i]->health>0)
            //GrSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,uncanny,Enemy[i]->last_left);
          break;
        case 1: 
        {
          if (Enemy[i]->in_air_timer==0 && !Enemy[i]->being_drawn) {
            if (Enemy[i]->above_ground) {
              if (Enemy[i]->sprite_timer%8==0) {
                DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_1,Enemy[i]->last_left);
              } else {
                DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_2,Enemy[i]->last_left);
              }
            } else if (Enemy[i]->below_ground) {
              if (Enemy[i]->sprite_timer%8==0) {
                DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_1,Enemy[i]->flip_sprite);
              } else {
                DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_2,Enemy[i]->flip_sprite);
              }
            } else {
              DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_3,Enemy[i]->last_left);
            }
          } else {
            DrawSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,&EnemySprite[i]->draw_sprite_3,Enemy[i]->last_left);
          }
        //if (Enemy[i]->health>0)
          //GrSprite(hdc,Enemy[i]->sprite_x,Enemy[i]->sprite_y,uncanny,Enemy[i]->last_left);
          break;
        }
      }
    }
  }
}

