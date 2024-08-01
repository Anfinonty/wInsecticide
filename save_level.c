

void SaveNewCreatedLvl(const wchar_t* create_lvl_name_)
{
  //Create Subfolders
  wchar_t create_lvl_name_sub1[48];
  swprintf(create_lvl_name_sub1,48,L"%s/images",create_lvl_name_);
  _wmkdir(create_lvl_name_sub1);
  wchar_t create_lvl_name_sub2[48];
  swprintf(create_lvl_name_sub2,48,L"%s/scores",create_lvl_name_);
  _wmkdir(create_lvl_name_sub2);
  wchar_t create_lvl_name_sub3[48];
  swprintf(create_lvl_name_sub3,48,L"%s/song",create_lvl_name_);
  _wmkdir(create_lvl_name_sub3);


  FILE *fptr;
  wchar_t create_lvl_name[64];
  swprintf(create_lvl_name,64,L"saves/%s/level.txt",typing_lvl_name);
  fptr = _wfopen(create_lvl_name,L"w");


  //beginning attributes
  fprintf(fptr,"%d;\n",set_ground_amount);
  fprintf(fptr,"%d;\n",set_enemy_amount);
  fprintf(fptr,"%d;\n",set_map_width_amount);
  fprintf(fptr,"%d;\n",set_map_height_amount);


  //GROUND
  for (int j=0;j<6;j++) { //x1,y1,x2,y2,x3,y3
    for (int i=0;i<set_ground_amount;i++) {
      if (i==0) {//first ground
        switch (j) {
          case 0://x1
            fprintf(fptr,"2,");
            break;
          case 1://y1
            fprintf(fptr,"450,");
            break;
          case 2://x2
            fprintf(fptr,"638,");
            break;
          case 3://y2
            fprintf(fptr,"412,");
            break;
          default: //x3,y3
            fprintf(fptr,"15,");
            break;
        }
      } else {
        if (j>=0 && j<=1) {
          fprintf(fptr,"5,");
        } else if (j>=2 && j<=3) {
          fprintf(fptr,"10,");
        } else if (j>=4 && j<=5) {
          fprintf(fptr,"15,");
        }
      }
    }
    fprintf(fptr,";\n");
  }

  for (int j=0;j<3;j++) {
    for (int i=0;i<set_ground_amount;i++) {//is_ghost,color,type
      fprintf(fptr,"0,");
    }
    fprintf(fptr,";\n");
  } 

  for (int i=0;i<set_ground_amount;i++) {//text
    fprintf(fptr,"\"\",");
  }
  fprintf(fptr,";\n");



  //Enemy
  for (int i=0;i<set_enemy_amount;i++) { //type
    fprintf(fptr,"0,");
  }
  fprintf(fptr,";\n");


  for (int i=0;i<set_enemy_amount;i++) { //x
    fprintf(fptr,"483,");
  }
  fprintf(fptr,";\n");


  for (int i=0;i<set_enemy_amount;i++) { //y
    fprintf(fptr,"306,");
  }
  fprintf(fptr,";\n");

  //enemy Type
  for (int j=0;j<2;j++) {
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"1.0,");
    }
    fprintf(fptr,";\n");
  }

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    fprintf(fptr,"0,");
  }
  fprintf(fptr,";\n");

  for (int j=0;j<2;j++) {
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"1,");
    }
    fprintf(fptr,";\n");
  }

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    fprintf(fptr,"100,");
  }
  fprintf(fptr,";\n");

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    fprintf(fptr,"0,");
  }
  fprintf(fptr,";\n");

  for (int j=0;j<3;j++) {
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"1,");
    }
    fprintf(fptr,";\n");
  }

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    fprintf(fptr,"0,");
  }
  fprintf(fptr,";\n");

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    fprintf(fptr,"500,");
  }
  fprintf(fptr,";\n");

  for (int i=0;i<ENEMY_TYPE_NUM;i++) {
    fprintf(fptr,"0,");
  }
  fprintf(fptr,";\n");

  for (int j=0;j<6;j++) {
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"1,");
    }
    fprintf(fptr,";\n");
  }

  for (int j=0;j<4;j++) {
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"0,");
    }
    fprintf(fptr,";\n");
  }


  fprintf(fptr,"228;\n"); //player x
  fprintf(fptr,"400;\n"); //player y

  fprintf(fptr,"2;\n");
  fprintf(fptr,"15;\n");
  fprintf(fptr,"0;\n");


  fclose(fptr);
}





void SaveMELvl()
{
    FILE *fptr;
    wchar_t create_lvl_name[64];
    swprintf(create_lvl_name,64,L"saves/%s/level.txt",level_names[level_chosen]);


    //create trash folder
    _wmkdir(L"trash");


    wchar_t trash_lvl_name[64];
    swprintf(trash_lvl_name,64,L"trash/%s-level.txt",level_names[level_chosen]);
    //copy old file to trash
    myCopyFile(trash_lvl_name,create_lvl_name);



    fptr = _wfopen(create_lvl_name,L"w");


    //beginning attributes
    fprintf(fptr,"%d;\n",GROUND_NUM);
    fprintf(fptr,"%d;\n",ENEMY_NUM);
    fprintf(fptr,"%d;\n",MAP_WIDTH);
    fprintf(fptr,"%d;\n",MAP_HEIGHT);

    //GROUND X1,Y1
    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%1.0f,",Ground[i]->x1);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%1.0f,",Ground[i]->y1);
    }
    fprintf(fptr,";\n");


    //GROUND X2,Y2
    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%1.0f,",Ground[i]->x2);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%1.0f,",Ground[i]->y2);
    }
    fprintf(fptr,";\n");



    //GROUND X3,Y3
    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%1.0f,",Ground[i]->x3);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%1.0f,",Ground[i]->y3);
    }
    fprintf(fptr,";\n");



    //GROUND is_ghost
    for (int i=0;i<GROUND_NUM;i++) {
      if (Ground[i]->is_ghost)
        fprintf(fptr,"1,");
      else
        fprintf(fptr,"0,");
    }
    fprintf(fptr,";\n");


    //GROUND color
    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%d,",Ground[i]->color_id);
    }
    fprintf(fptr,";\n");


    //GROUND type
    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"%d,",Ground[i]->type);
    }
    fprintf(fptr,";\n");



    //GROUND text
    for (int i=0;i<GROUND_NUM;i++) {
      fprintf(fptr,"\"");
      for (int j=0;j<lstrlenW(Ground[i]->text);j++) {
        fprintf(fptr,"{u%X}",Ground[i]->text[j]);
      }
      fprintf(fptr,"\",");
    }
    fprintf(fptr,";\n");



    //Enemy tyoe
    for (int i=0;i<ENEMY_NUM;i++) {
      fprintf(fptr,"%1.0f,",MEEnemy[i]->type);
    }
    fprintf(fptr,";\n");


    //Enemy X
    for (int i=0;i<ENEMY_NUM;i++) {
      fprintf(fptr,"%1.0f,",MEEnemy[i]->x);
    }
    fprintf(fptr,";\n");


    //Enemy Y
    for (int i=0;i<ENEMY_NUM;i++) {
      fprintf(fptr,"%1.0f,",MEEnemy[i]->y);
    }
    fprintf(fptr,";\n");





    //ENEMY TYPE
    //enemy Type,, NOTE: ENEMY_TYPE NUM DOES NOT CHANGE
    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%3.1f,",set_enemy_type_speed[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%3.1f,",set_enemy_type_bullet_speed[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_species[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_follow_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_unchase_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_chase_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_color[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_speed_multiplier[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_health[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_shoot_at_player_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_aim_rand[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_cooldown[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_fire_cooldown[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_fire_at_once[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_length[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_damage[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_speed_multiplier[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_color[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_bullet_graphics_type[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_time_breaker_rare[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_time_breaker_length[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",set_enemy_type_time_breaker_immune[i]);
    }
    fprintf(fptr,";\n");



    fprintf(fptr,"%1.0f;\n",player.x); //player x
    fprintf(fptr,"%1.0f;\n",player.y); //player y

    fprintf(fptr,"%d;\n",map_background); //
    fprintf(fptr,"%d;\n",custom_map_background_color_i); // 
    fprintf(fptr,"%d;\n",is_inverted); //


    fclose(fptr);
}



//
//
//
//Adjusted Lvl Min Max

void SaveNewLimitAdjustedLvl()
{
    FILE *fptr;
    wchar_t create_lvl_name[64];
    swprintf(create_lvl_name,64,L"saves/%s/level.txt",typing_lvl_name);


    //create trash folder
    _wmkdir(L"trash");


    wchar_t trash_lvl_name[64];
    swprintf(trash_lvl_name,64,L"trash/%s-level.txt",typing_lvl_name);
    //copy old file to trash
    myCopyFile(trash_lvl_name,create_lvl_name);



    fptr = _wfopen(create_lvl_name,L"w");


    //beginning attributes
    fprintf(fptr,"%d;\n",set_ground_amount);
    fprintf(fptr,"%d;\n",set_enemy_amount);
    fprintf(fptr,"%d;\n",set_map_width_amount);
    fprintf(fptr,"%d;\n",set_map_height_amount);

    //GROUND X1,Y1
    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        if (saved_ground_x1[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_x1[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"5,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        if (saved_ground_x1[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_x1[i]);
      }
    }
    fprintf(fptr,";\n");




    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        if (saved_ground_y1[i]>set_map_height_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_height_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_y1[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"5,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        if (saved_ground_y1[i]>set_map_height_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_height_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_y1[i]);
      }
    }
    fprintf(fptr,";\n");




    //GROUND X2,Y2
    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        if (saved_ground_x2[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_x2[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"10,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        if (saved_ground_x2[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_x2[i]);
      }
    }
    fprintf(fptr,";\n");




    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        if (saved_ground_y2[i]>set_map_height_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_height_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_y2[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"10,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        if (saved_ground_y2[i]>set_map_height_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_height_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_y2[i]);
      }
    }
    fprintf(fptr,";\n");



    //GROUND X3,Y3
    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        if (saved_ground_x3[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_x3[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"15,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        if (saved_ground_x3[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_x3[i]);
      }
    }
    fprintf(fptr,";\n");




    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        fprintf(fptr,"%1.0f,",saved_ground_y3[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"15,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        if (saved_ground_y3[i]>set_map_height_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_height_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_ground_y3[i]);
      }
    }
    fprintf(fptr,";\n");



    //GROUND is_ghost
    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        fprintf(fptr,"%d,",saved_ground_is_ghost[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"0,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        fprintf(fptr,"%d,",saved_ground_is_ghost[i]);
      }
    }
    fprintf(fptr,";\n");






    //GROUND color
    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        fprintf(fptr,"%d,",saved_ground_color[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"0,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        fprintf(fptr,"%d,",saved_ground_color[i]);
      }
    }
    fprintf(fptr,";\n");






    //GROUND type
    if (set_ground_amount>GROUND_NUM) { // add new grounds
      for (int i=0;i<GROUND_NUM;i++) {
        fprintf(fptr,"%d,",saved_ground_type[i]);
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"0,");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        fprintf(fptr,"%d,",saved_ground_type[i]);
      }
    }
    fprintf(fptr,";\n");



    //GROUND text
    if (set_ground_amount>GROUND_NUM) { // text
      for (int i=0;i<GROUND_NUM;i++) {
        fprintf(fptr,"\"");
        for (int j=0;j<lstrlenW(saved_ground_text[i]);j++) {
          fprintf(fptr,"{u%X}",saved_ground_text[i][j]);
        }
        fprintf(fptr,"\",");
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"\"\",");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        fprintf(fptr,"\"");
        for (int j=0;j<lstrlenW(saved_ground_text[i]);j++) {
          fprintf(fptr,"{u%X}",saved_ground_text[i][j]);
        }
        fprintf(fptr,"\",");
      }
    }
    fprintf(fptr,";\n");



    //Enemy
    if (set_enemy_amount>ENEMY_NUM) { // type enemy
      for (int i=0;i<ENEMY_NUM;i++) {
        fprintf(fptr,"%d,",saved_enemy_type[i]);
      }
      for (int i=ENEMY_NUM;i<set_enemy_amount;i++) { //type
        fprintf(fptr,"0,");
      }
    } else {
      for (int i=0;i<set_enemy_amount;i++) {
        fprintf(fptr,"%d,",saved_enemy_type[i]);
      }
    }
    fprintf(fptr,";\n");


    if (set_enemy_amount>ENEMY_NUM) { // x enemy
      for (int i=0;i<ENEMY_NUM;i++) {
        if (saved_enemy_x[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_enemy_x[i]);
      }
      for (int i=ENEMY_NUM;i<set_enemy_amount;i++) {
        fprintf(fptr,"483,");
      }
    } else {
      for (int i=0;i<set_enemy_amount;i++) { //x
        if (saved_enemy_x[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_enemy_x[i]);
      }
    }
    fprintf(fptr,";\n");


    if (set_enemy_amount>ENEMY_NUM) { // y enemy
      for (int i=0;i<ENEMY_NUM;i++) {
        if (saved_enemy_y[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_enemy_y[i]);
      }
      for (int i=ENEMY_NUM;i<set_enemy_amount;i++) { //y
        fprintf(fptr,"306,");
      }
    } else {
      for (int i=0;i<set_enemy_amount;i++) { //y
        if (saved_enemy_y[i]>set_map_width_amount) //over new map limit, adjust
          fprintf(fptr,"%1.0f,",set_map_width_amount-2);
        else //within new map limit, remain the same
          fprintf(fptr,"%1.0f,",saved_enemy_y[i]);
      }
    }
    fprintf(fptr,";\n");





    //ENEMY TYPE
    //enemy Type,, NOTE: ENEMY_TYPE NUM DOES NOT CHANGE

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%3.1f,",saved_enemy_type_speed[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%3.1f,",saved_enemy_type_bullet_speed[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_species[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_follow_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_unchase_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_chase_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_color[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_speed_multiplier[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_health[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_shoot_at_player_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_aim_rand[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_cooldown[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_fire_cooldown[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_fire_at_once[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_length[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_damage[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_speed_multiplier[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_range[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_color[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_bullet_graphics_type[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_time_breaker_rare[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_time_breaker_length[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) {
      fprintf(fptr,"%d,",saved_enemy_type_time_breaker_immune[i]);
    }
    fprintf(fptr,";\n");



    fprintf(fptr,"%1.0f;\n",saved_player_x); //player x
    fprintf(fptr,"%1.0f;\n",saved_player_y); //player y

    fprintf(fptr,"%d;\n",map_background); //
    fprintf(fptr,"%d;\n",custom_map_background_color_i); // 
    fprintf(fptr,"%d;\n",is_inverted); //


    fclose(fptr);
}
