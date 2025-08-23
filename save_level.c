

void SaveNewCreatedLvl(const wchar_t* create_lvl_name_)
{
  //Create Subfolders
  wchar_t create_lvl_name_sub1[48];
  swprintf(create_lvl_name_sub1,48,L"%ls/images",create_lvl_name_);
  _wmkdir(create_lvl_name_sub1);
  wchar_t create_lvl_name_sub2[48];
  swprintf(create_lvl_name_sub2,48,L"%ls/scores",create_lvl_name_);
  _wmkdir(create_lvl_name_sub2);
  wchar_t create_lvl_name_sub3[48];
  swprintf(create_lvl_name_sub3,48,L"%ls/song",create_lvl_name_);
  _wmkdir(create_lvl_name_sub3);


  FILE *fptr;
  wchar_t create_lvl_name[64];
  swprintf(create_lvl_name,64,L"saves/%ls/level.txt",typing_lvl_name);
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

  fprintf(fptr,"0;\n"); //
  fprintf(fptr,"200,200;\n"); // 

  fprintf(fptr,"0;\n"); //
  fprintf(fptr,"200,200;\n"); // 

  fclose(fptr);
}


/*void SaveLvlBmpSegmentation2(HWND hwnd,HDC hdc)
{
  wchar_t water_bmp_seg_save[64];
  swprintf(water_bmp_seg_save,64,L"saves/%ls/seg_foreground",level_names[level_chosen]);
  wchar_t seg_save_seg[72];

  PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
  hdc=BeginPaint(hwnd, &ps);
  HDC hdc1;
  HDC hdc2;
  HBITMAP tmp_bitmap;

  //Foreground, water
  if (map_water_platforms_sprite!=NULL) {
      hdc1=CreateCompatibleDC(hdc);
      RemoveFolderRecursive(water_bmp_seg_save);
      _wmkdir(water_bmp_seg_save);
      hdc1=CreateCompatibleDC(hdc);
      SelectObject(hdc1,map_water_platforms_sprite);
      for (int i=0;i<VGRID_NUM;i++) {
        if (VGrid[i]->has_water) {
          tmp_bitmap=CreateCrunchyBitmap(VGRID_SIZE,VGRID_SIZE);
          hdc2 = CreateCompatibleDC(hdc1); // Create a compatible DC      
          SelectObject(hdc2,tmp_bitmap);
          BitBlt(hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, hdc1, VGrid[i]->x1, VGrid[i]->y1, SRCCOPY); // Copy from hdc1 to hdc2

          swprintf(seg_save_seg,72,L"%ls/%d.bmp",water_bmp_seg_save,i);
          SaveBitmapToFile2(tmp_bitmap,rgbColorsDefault, seg_save_seg);
          DeleteDC(hdc2);
          DeleteObject(tmp_bitmap);
          loading_numerator++;
          loading_percentage=(loading_numerator+1)/(loading_denominator+1)*100;
          printf("=== %1.0f%% - [%1.0f/%1.0f]\r",loading_percentage,loading_numerator,loading_denominator);
        }
      }
      DeleteDC(hdc1);
      DeleteObject(map_water_platforms_sprite);
  }
  EndPaint(hwnd, &ps);
}*/


void SaveLvlBmpSegmentation1(HWND hwnd,HDC hdc,const wchar_t* lvl_name)
{
  //wchar_t bmp_seg_save[64];
  //swprintf(bmp_seg_save,64,L"saves/%ls/seg_platforms",level_names[level_chosen]);
  wchar_t shadow_bmp_seg_save[64];
  // swprintf(shadow_bmp_seg_save,64,L"saves/%ls/seg_shadow",level_names[level_chosen]);
  swprintf(shadow_bmp_seg_save,64,L"saves/%ls/seg_shadow",lvl_name);

  //RemoveFolderRecursive(bmp_seg_save);
  //_wmkdir(bmp_seg_save);
  wchar_t seg_save_seg[72];


  PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
  if (hwnd!=NULL) {
    hdc=BeginPaint(hwnd, &ps);
  }
  HDC hdc1;
  HDC hdc2;
  HBITMAP tmp_bitmap;


  //normal platforms
  /*hdc1=CreateCompatibleDC(hdc);
  SelectObject(hdc1,map_platforms_sprite);
  for (int i=0;i<VGRID_NUM;i++) {
    if (VGrid[i]->max_ground_num>0 && (!VGrid[i]->has_water || VGrid[i]->not_just_water)) {
      tmp_bitmap=CreateCrunchyBitmap(VGRID_SIZE,VGRID_SIZE);
      hdc2 = CreateCompatibleDC(hdc1); // Create a compatible DC      
      SelectObject(hdc2,tmp_bitmap);
      BitBlt(hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, hdc1, VGrid[i]->x1, VGrid[i]->y1, SRCCOPY); // Copy from hdc1 to hdc2

      swprintf(seg_save_seg,72,L"%ls/%d.bmp",bmp_seg_save,i);
      SaveBitmapToFile2(tmp_bitmap,rgbColorsDefault, seg_save_seg);
      DeleteDC(hdc2);
      DeleteObject(tmp_bitmap);
      loading_numerator++;
      loading_percentage=(loading_numerator+1)/(loading_denominator+1)*100;
      printf("=== %1.0f%% - [%1.0f/%1.0f]\r",loading_percentage,loading_numerator,loading_denominator);
    }
  }
  DeleteDC(hdc1);
  DeleteObject(map_platforms_sprite);*/



  //shadow platforms, water excluded from shadow
  if ((hwnd==NULL) || 
        (MapEditor.set_lvl_ambient_val[6]==1 && 
        (MapEditor.set_lvl_ambient_val[7]!=shadow_grad_rise || MapEditor.set_lvl_ambient_val[8]!=shadow_grad_run))) {
      hdc1=CreateCompatibleDC(hdc);
      RemoveFolderRecursive(shadow_bmp_seg_save);
      _wmkdir(shadow_bmp_seg_save);
      SelectObject(hdc1,map_platforms_shadow_shader);
      for (int i=0;i<VGRID_NUM;i++) {
        if (VGrid[i]->has_shadow) {
          tmp_bitmap=CreateCrunchyBitmap(VGRID_SIZE,VGRID_SIZE);
          hdc2 = CreateCompatibleDC(hdc1); // Create a compatible DC      
          SelectObject(hdc2,tmp_bitmap);
          BitBlt(hdc2, 0, 0, VGRID_SIZE, VGRID_SIZE, hdc1, VGrid[i]->x1, VGrid[i]->y1, SRCCOPY); // Copy from hdc1 to hdc2

          swprintf(seg_save_seg,72,L"%ls/%d.bmp",shadow_bmp_seg_save,i);
          SaveBitmapToFile2(tmp_bitmap,rgbColorsDefault, seg_save_seg);
          DeleteDC(hdc2);
          DeleteObject(tmp_bitmap);
          if (hwnd!=NULL) {
            loading_numerator++;
            loading_percentage=(loading_numerator+1)/(loading_denominator+1)*100;
            printf("=== %1.0f%% - [%1.0f/%1.0f]\r",loading_percentage,loading_numerator,loading_denominator);
          }
        }
      }
      DeleteDC(hdc1);
      DeleteObject(map_platforms_shadow_shader);
  }
  if (hwnd!=NULL) {
    EndPaint(hwnd, &ps);
  }
}

void SaveLvlBmp(HWND hwnd,HDC hdc,const wchar_t* lvl_name)
{  
  //tri fill all triangles to be segmented
  for (int i=0;i<GROUND_NUM;i++) {
    TriFillGridType(i,0);
  }
  for (int i=0;i<GROUND_NUM;i++) {
    TriFillGridType(i,1);
  }
  //printf("triangles_filled\n");



    //FOREGROUND_GRID_NUM=0;
    PLATFORM_GRID_NUM=0;
    SHADOW_GRID_NUM=0;
    if (hwnd!=NULL) {
      loading_numerator=0;
      loading_denominator=0;
    }

    /*for (int i=0;i<VGRID_NUM;i++) {
      if (VGrid[i]->has_water) {
        FOREGROUND_GRID_NUM++;
      }
    }*/

    for (int i=0;i<VGRID_NUM;i++) {
      if (VGrid[i]->max_ground_num>0 && (!VGrid[i]->has_water || VGrid[i]->not_just_water)) {
        PLATFORM_GRID_NUM++;
      }
    }

    for (int i=0;i<VGRID_NUM;i++) {
      if (VGrid[i]->has_shadow) {
        SHADOW_GRID_NUM++;
      }
    }

    if (hwnd!=NULL) {
      loading_denominator=PLATFORM_GRID_NUM/*+FOREGROUND_GRID_NUM*/+SHADOW_GRID_NUM;
    }



 // wchar_t bmp_save[64];
 // swprintf(bmp_save,64,L"saves/%ls/map.bmp",level_names[level_chosen]);
 // wchar_t water_bmp_save[64];
//  swprintf(water_bmp_save,64,L"saves/%ls/map_water.bmp",level_names[level_chosen]);
//  swprintf(water_bmp_save,64,L"saves/%ls/map_foreground.bmp",level_names[level_chosen]);
//  wchar_t bmp_save_shadow[64];
//  swprintf(bmp_save_shadow,64,L"saves/%ls/map_shadow.bmp",level_names[level_chosen]);

  PAINTSTRUCT ps; //Suggestion Credit: https://git.xslendi.xyz
  if (hwnd!=NULL) {
    hdc=BeginPaint(hwnd, &ps);
  }
  HDC hdc2=CreateCompatibleDC(hdc);
  //HDC hdc3=CreateCompatibleDC(hdc);

  //normal platforms
  //int max_size=max(MAP_WIDTH,MAP_HEIGHT);
  map_platforms_sprite=CreateCrunchyBitmap(MAP_WIDTH,MAP_HEIGHT);
  SelectObject(hdc2,map_platforms_sprite);
  GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,MYCOLOR1); //Create Background with set color over platforms
  DrawGroundTriFill(hdc2);
  DrawGround(hdc2);
  DrawGroundText(hdc2);



  //shadow platforms, water excluded from shadow
  COLORREF shadowc=LTGRAY;
  if (hwnd!=NULL) {
  if (MapEditor.set_lvl_ambient_val[6]==1 && 
        (MapEditor.set_lvl_ambient_val[7]!=shadow_grad_rise || MapEditor.set_lvl_ambient_val[8]!=shadow_grad_run)) {
    if (MapEditor.set_lvl_ambient_val[0]==1 || (MapEditor.set_lvl_ambient_val[0]==2 && MapEditor.set_lvl_ambient_val[1]>127)) {
      shadowc=RGB(8,8,8);//DKRDKGRAY;
    }
    //SaveBitmapToFile2(map_platforms_shadow_shader,rgbColorsDefault, bmp_save_shadow);

    CreatePlatformShadowBitmap2(hdc2, map_platforms_sprite, MapEditor.set_lvl_ambient_val[7],
                                    MapEditor.set_lvl_ambient_val[8],
                                    shadowc);

  }
  } else {
    if (map_background==1 || (map_background==2 && custom_map_background_color_i>127)) {
      shadowc=RGB(8,8,8);//DKRDKGRAY;
    }

    CreatePlatformShadowBitmap2(hdc2, map_platforms_sprite, shadow_grad_rise,
                                    shadow_grad_run,
                                    shadowc);
  }

  //DeleteDC(hdc3);
  DeleteDC(hdc2);
  SaveLvlBmpSegmentation1(hwnd,hdc,lvl_name);


  /*hdc2=CreateCompatibleDC(hdc);
  map_water_platforms_sprite=CreateCrunchyBitmap(MAP_WIDTH,MAP_HEIGHT);
  SelectObject(hdc2,map_water_platforms_sprite);
  GrRect(hdc2,0,0,MAP_WIDTH+1,MAP_HEIGHT+1,MYCOLOR1); //Create Background with set  color over platforms
  DrawWaterTriFill(hdc2);
  DeleteDC(hdc2);

  SaveLvlBmpSegmentation2(hwnd,hdc);*/

  //water platforms

  if (hwnd!=NULL) {
    EndPaint(hwnd, &ps);
  }
  //SaveBitmapToFile2(map_platforms_sprite,rgbColorsDefault, bmp_save);
  //SaveBitmapToFile2(map_water_platforms_sprite,rgbColorsDefault, water_bmp_save);

  DeleteObject(map_platforms_sprite);
  //DeleteObject(map_water_platforms_sprite);
}

void SaveMELvl(HWND hwnd,HDC hdc)
{
    
    FILE *fptr;
    wchar_t create_lvl_name[64];
    swprintf(create_lvl_name,64,L"saves/%ls/level.txt",level_names[level_chosen]);


    //create trash folder
    _wmkdir(L"trash");


    wchar_t trash_lvl_name[64];
    swprintf(trash_lvl_name,64,L"trash/%ls-level.txt",level_names[level_chosen]);
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
      Ground[i]->color=rgbPaint[Ground[i]->color_id]; //for saving
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
      //ground size
      //if (Ground[i]->font_size!=16) { //only set font size if font_size is NOT 16
        fprintf(fptr,"%d",Ground[i]->font_size);
      //}
      fprintf(fptr,"\"");
      for (int j=0;j<lstrlenW(Ground[i]->text);j++) {
        fprintf(fptr,"{u%X}",Ground[i]->text[j]);
      }
      fprintf(fptr,"\",");
    }
    fprintf(fptr,";\n");



    //Enemy tyoe
    for (int i=0;i<ENEMY_NUM;i++) {
      fprintf(fptr,"%d,",MEEnemy[i]->type);
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
    for (int i=0;i<ENEMY_TYPE_NUM;i++) { //double type var
      fprintf(fptr,"%3.1f,",set_enemy_type_speed[i]);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<ENEMY_TYPE_NUM;i++) { //double type var
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

    for (int i=0;i<ENEMY_TYPE_NUM;i++) { //bool type var
      fprintf(fptr,"%d,",set_enemy_type_time_breaker_immune[i]);
    }
    fprintf(fptr,";\n");



    fprintf(fptr,"%1.0f;\n",player.x); //player x
    fprintf(fptr,"%1.0f;\n",player.y); //player y

    fprintf(fptr,"%d;\n",MapEditor.set_lvl_ambient_val[0]/*map_background*/); //
    fprintf(fptr,"%d;\n",MapEditor.set_lvl_ambient_val[1]/*custom_map_background_color_i*/); // 
    fprintf(fptr,"%d;\n",MapEditor.set_lvl_ambient_val[2]/*is_moon*/); //


    fprintf(fptr,"%d;\n",MapEditor.set_lvl_ambient_val[3]); // 
    fprintf(fptr,"%d,%d,;\n",MapEditor.set_lvl_ambient_val[4],MapEditor.set_lvl_ambient_val[5]); //
    fprintf(fptr,"%d;\n",MapEditor.set_lvl_ambient_val[6]); //
    fprintf(fptr,"%d,%d,;\n",MapEditor.set_lvl_ambient_val[7],MapEditor.set_lvl_ambient_val[8]); //


    for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
      fprintf(fptr,"%d,",GamePlatformTextures[i].type);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
      fprintf(fptr,"%d,",GamePlatformTextures[i].solid_value);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
      fprintf(fptr,"%d,",GamePlatformTextures[i].color_id);
    }
    fprintf(fptr,";\n");


    fclose(fptr);

   printf("\n=== Saving Map Id: %d ===\n\n",level_chosen);

   SaveLvlBmp(hwnd,hdc,level_names[level_chosen]);
   level_loading=FALSE;
   printf("\n\n=== Save Complete ===\n",level_chosen);

}



//
//
//
//Adjusted Lvl Min Max

void SaveNewLimitAdjustedLvl(HWND hwnd, HDC hdc)
{
    FILE *fptr;
    wchar_t create_lvl_name[64];
    swprintf(create_lvl_name,64,L"saves/%ls/level.txt",typing_lvl_name);


    //create trash folder
    _wmkdir(L"trash");


    wchar_t trash_lvl_name[64];
    swprintf(trash_lvl_name,64,L"trash/%ls-level.txt",typing_lvl_name);
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
        fprintf(fptr,"%d",saved_ground_text_size[i]);
        fprintf(fptr,"\"");
        for (int j=0;j<lstrlenW(saved_ground_text[i]);j++) {
          fprintf(fptr,"{u%X}",saved_ground_text[i][j]);
        }
        fprintf(fptr,"\",");
      }
      for (int i=GROUND_NUM;i<set_ground_amount;i++) {
        fprintf(fptr,"16\"\",");
      }
    } else {
      for (int i=0;i<set_ground_amount;i++) {
        fprintf(fptr,"%d",saved_ground_text_size[i]);
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
    fprintf(fptr,"%d;\n",is_moon); //

    fprintf(fptr,"%d;\n",map_weather); //
    fprintf(fptr,"%d,%d;\n",rain_grad_rise,rain_grad_run); // 

    fprintf(fptr,"%d;\n",is_shadows); //
    fprintf(fptr,"%d,%d;\n",shadow_grad_rise,shadow_grad_run); // 

    for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
      fprintf(fptr,"%d,",GamePlatformTextures[i].type);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
      fprintf(fptr,"%d,",GamePlatformTextures[i].solid_value);
    }
    fprintf(fptr,";\n");

    for (int i=0;i<PLATFORM_TEXTURES_NUM;i++) {
      fprintf(fptr,"%d,",GamePlatformTextures[i].color_id);
    }
    fprintf(fptr,";\n");

    fclose(fptr);

}




void SaveOptions()
{
  FILE *fptr;
  wchar_t create_save_name[64];
  swprintf(create_save_name,64,L"saves/options.txt");
  fptr = _wfopen(create_save_name,L"w+");
  //write options here
  fprintf(fptr,"%d;\n",player_color);//player color
  fprintf(fptr,"%d;\n",player_iris_color);//play iris color
  fprintf(fptr,"%d;\n",player_pupil_color);  //pupil color
  fprintf(fptr,"%d;\n",game_cam_shake);  //camera shake
  fprintf(fptr,"%d;\n",game_audio);    //audio
  fprintf(fptr,"%5.4f;\n",game_volume);  //sfx
  fprintf(fptr,"%5.4f;\n",wav_out_volume); //raw wav out volume
  fprintf(fptr,"%d;\n",yes_unifont);      //unifont
  fprintf(fptr,"%d;\n",game_shadow);        //shadows
  fprintf(fptr,"%d;\n",hide_taskbar);        //window borders
  fprintf(fptr,"%d;\n",resolution_choose);  //saved resolution
  fprintf(fptr,"%d;\n",show_fps);  //show fps
  fprintf(fptr,"%d;\n",show_hijiri);  //show hijri
  fprintf(fptr,"%d;\n",is_khmer);  //is_khmer
  fprintf(fptr,"%d;\n",game_hard);  //game difficulty
  fprintf(fptr,"%d;\n",free_will);  //free will
  
  //close file
  fclose(fptr);
}
