
//Background
void DrawBackground(HDC hdc) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255))); //RAVE
  switch (map_background) {
    case 0:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE);
      break;
    case 1:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,NOTSRCCOPY,FALSE);
      GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);
      break;
    default:
      GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,custom_map_background_color);
      break;
  }
}


void DrawPlatforms(HDC hDC)
{ //Dynamically scale with window size 
  //Draw platforms bitmap mask
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2,
                map_platforms_sprite_mask,SRCAND,FALSE);
  //Draw platforms paint
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2,
                map_platforms_sprite,SRCPAINT,FALSE);
}


void DrawCursor(HDC hDC)
{
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite,SRCAND,FALSE);
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite_mask,SRCPAINT,FALSE);
  GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_cache,FALSE);
  GrCircle(hDC,mouse_x,mouse_y,1,WHITE,-1);
}





void DrawPlayingMusic(HDC hdc,int x,int y,int c, int c4)
{
  if (!stop_playing_song) {

    if (song_num>0) {
      wchar_t txt[32+256];
      swprintf(txt,32+256,L"%c%d/%d%c: %s",171,song_rand_num+1,song_num,187,song_names[song_rand_num]);
      //%c 187

      GrPrintW(hdc,x,y,txt,"",c,16,FALSE,yes_unifont);
      GrPrintW(hdc,x+1,y+1,txt,"",c4,16,FALSE,yes_unifont);

      

      char txt2[72];
      //char txt2_1[2046];
      //char txt2_2[256];
      /*for (int j=0;j<128;j++) {
        sprintf(txt2_1,"%s %d:%c ",txt2_1,'z'+j,'z'+j);
      }*/ //max 256
      //note %c 134 is a cross

      switch (song_mode) {
        case 0:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [SHIFT_M: /%c]",171,187,177,187,171);
          break;
        case 1:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [SHIFT_M: /?]",171,187,177,171);
          break;
        case 2:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /?] [SHIFT_M: X]",171,187,177);
          break;
      }
      GrPrint(hdc,x,y+16,txt2,c);   
      GrPrint(hdc,x+1,y+1+16,txt2,c4);
    }
  } else {
    GrPrint(hdc,x,y,"Press Shift + M to Enable Songs",c);
    GrPrint(hdc,x+1,y+1,"Press Shift + M to Enable Songs",c4);
  }
}


void DrawMainMenu(HDC hdc)
{

  //draw bkgrnd
  DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE);

  //Draw Moon Phase
  GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);

  //Moon Pos
  int mcalendar_l=64;
  int mcalendar_x=GR_WIDTH-mcalendar_l*2;
  int mcalendar_y=GR_HEIGHT-mcalendar_l*2-32;
  double moon_angle=0; 
  //Space Clock
  //Draw blue marbel
  GrCircle(hdc,mcalendar_x,mcalendar_y,10,LTBLUE,LTBLUE);
  GrRect(hdc,mcalendar_x,mcalendar_y,3,4,LTGREEN);
  GrRect(hdc,mcalendar_x-3,mcalendar_y,9,4,LTGREEN);
  GrRect(hdc,mcalendar_x+4,mcalendar_y,5,2,LTGREEN);
  GrRect(hdc,mcalendar_x-4,mcalendar_y-4,9,9,LTGREEN);
  GrCircle(hdc,mcalendar_x-4,mcalendar_y,2,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x+2,mcalendar_y+5,3,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x-2,mcalendar_y-3,3,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x-3,mcalendar_y-5,2,LTGREEN,LTGREEN);
  GrCircle(hdc,mcalendar_x-3,mcalendar_y+5,2,LTGREEN,LTGREEN);

  if (lunar_day<27) //0 to 26
    moon_angle=(-2*M_PI/27 * lunar_day ) - moon_angle_shift;
  else
    moon_angle=-moon_angle_shift;

  for (int i=0;i<27;i++) {
    double tmp_angle=-2*M_PI/27 * i - moon_angle_shift;
    if (i>1 && i<26) {
      GrCircle(hdc,mcalendar_x + mcalendar_l*cos(tmp_angle), mcalendar_y + mcalendar_l*sin(tmp_angle),5,BLACK,DKGRAY);
    } else {
      if (i==1 || i==26) { //Cresent Moon
        GrCircle(hdc,mcalendar_x + mcalendar_l*cos(tmp_angle), mcalendar_y + mcalendar_l*sin(tmp_angle),6,BLACK,CYAN);
      } else {
        GrCircle(hdc,mcalendar_x + mcalendar_l*cos(tmp_angle), mcalendar_y + mcalendar_l*sin(tmp_angle),7,BLACK,LTCYAN);
      }
    }
  }
  GrCircle(hdc,mcalendar_x + mcalendar_l*cos(moon_angle), mcalendar_y + mcalendar_l*sin(moon_angle),5,WHITE,WHITE);

  //Sun Pos
  GrCircle(hdc,mcalendar_x + mcalendar_l*cos(-solar_angle_day), mcalendar_y + mcalendar_l*sin(-solar_angle_day),5,YELLOW,YELLOW);



  //Write Hijri Dates
  wchar_t time_row1[16];
  char s_hijri_row1[128];
  wchar_t s_hijri_row2[128];
  char l_hijri_row1[128];
  wchar_t l_hijri_row2[128];

  int num_char='*';
  if (solar_month==1 && (solar_day>=12 && solar_day<=19)) {
    num_char=134;
  }

      swprintf(time_row1,16,L"[%d:%02d:%02d]",current_hour,current_min,current_sec);

      sprintf(s_hijri_row1,"=:: Solar Hijri ::= %c",num_char);

      swprintf(s_hijri_row2,128,L":: %s //%d. %s(%d) .%d",
solar_days_txt[solar_day_of_week],
solar_day,
solar_months_txt[solar_month-1],
solar_month,
solar_year
);


  //( <| <|)  O  (|> |> ) @
  if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","  )");
  } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s"," |)");
  } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","<|)");
  } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(O)");
  } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(|>");
  } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(|");
  } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s","(");
  } else {
    sprintf(l_hijri_row1,"(:: Lunar Hijri ::) %s"," @");
  }


  swprintf(l_hijri_row2,128,L":: %s //%d. %s(%d) .%d",
lunar_days_txt[lunar_day_of_week],
lunar_day,
lunar_months_txt[lunar_month-1],
lunar_month,
lunar_year
);

  GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-64,time_row1,"",WHITE,16,FALSE,yes_unifont);
  GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-32,L"",s_hijri_row1,WHITE,16,TRUE,yes_unifont);
  GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-16,s_hijri_row2,"",WHITE,16,FALSE,yes_unifont);
  GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y+16,L"",l_hijri_row1,WHITE,16,TRUE,yes_unifont);
  GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y+32,l_hijri_row2,"",WHITE,16,FALSE,yes_unifont);


  char C[1];
  sprintf(C,"%c",134);
  GrPrintW(hdc,GR_WIDTH-8*18,8*23+10,L"",C,WHITE,16,TRUE,FALSE);
  GrPrintW(hdc,GR_WIDTH-8*18-8*2,8*23+12,L"",C,WHITE,16,TRUE,FALSE);
  GrPrintW(hdc,GR_WIDTH-8*18+8*2,8*23+12,L"",C,WHITE,16,TRUE,FALSE);
  

  GrLine(hdc,GR_WIDTH-8*17-4,8*25+10,GR_WIDTH-8*17-4-8*8,8*25+12,WHITE);
  GrLine(hdc,GR_WIDTH-8*17-4,8*25+10,GR_WIDTH-8*17-4+8*8,8*25+12,WHITE);


  GrPrintW(hdc,30,10,L"អាពីងស៊ីរុយ - Welcome to the wInsecticide Menu!","",WHITE,16,FALSE,yes_unifont);

  int max_lvl_rows=10;
  char page_num[32];
  sprintf(page_num,"[%d/%d]",(level_chosen/max_lvl_rows)+1,(level_num/max_lvl_rows)+1);
  GrPrint(hdc,30,10+32,page_num,WHITE);


  int lvls_y=10+16*4;
  int lvl_i=0;

  for (int i=0;i<max_lvl_rows;i++) { //Print Levels
     lvl_i=i+10*(level_chosen/max_lvl_rows);
     GrPrint(hdc,30,lvls_y+16*i,"-_________",WHITE);
     if (lvl_i<level_num) {
       GrPrintW(hdc,30+8*11,lvls_y+16*i,level_names[lvl_i],"",WHITE,16,FALSE,yes_unifont);
     }
  }

  //Draw Level Selector
  GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows),"   [ENTER]",WHITE);
  GrPrint(hdc,30+66,2+lvls_y+16*(level_chosen%max_lvl_rows),"   *",WHITE);
  GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows)-16,"        /\\",WHITE);
  GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows)+16,"        \\/",WHITE);



  GrPrint(hdc,30,10+16*16,"[SHIFT_ESC]: Exit",WHITE);
  GrPrintW(hdc,30,10+16*17,L"[SHIFT] + 'L': Unifont [ពេលរាត្រី]","",WHITE,16,FALSE,yes_unifont);
  GrPrint(hdc,30,10+16*18,"Player Color: [LEFT] <    > [RIGHT]",WHITE);

  if (player_color!=0) {
    GrRect(hdc,30+8*18-2,10+32+16*16,16,16,BLACK);
  } else {
    GrRect(hdc,30+8*18-2,10+32+16*16,16,16,WHITE);
  }
  if (player_color>-1 && player_color<COLORS_NUM) {
    GrRect(hdc,30+8*18,10+32+16*16+2,12,12,draw_color_arr[player_color]);
  }

  DrawPlayingMusic(hdc,30,10+32+16*19,BLACK,WHITE);


}






bool display_controls=FALSE;
void DrawUI(HDC hdc) {
  int c;
  int c4;
  if (!IsInvertedBackground()) {
    c=WHITE;
    c4=BLACK;
  } else {
    c=BLACK;
    c4=WHITE;
  }

//======Print Game Score Text
  //GrRect(hdc,0,8+32,8*40,16*5,c4);

  char gametimetxt[32];
  double print_time_ms=(double)game_timer/1000;
  if (!game_over) {
    sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
    GrPrint(hdc,16+4,8+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+6,8+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+4,10+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+6,10+48,gametimetxt,YELLOW);
    GrPrint(hdc,16+5,9+48,gametimetxt,BROWN);
  } else { //game is over
    if (game_timer<int_best_score) { //New Score :D
      sprintf(gametimetxt,"New Best! :D %5.3f",print_time_ms);
      GrPrint(hdc,16+5,9+48,gametimetxt,LTPURPLE);
    } else {
      sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
      GrPrint(hdc,16+4,8+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+6,8+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+4,10+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+6,10+48,gametimetxt,YELLOW);
      GrPrint(hdc,16+5,9+48,gametimetxt,BROWN);
    }
  }

  char gamebesttimetxt[32];
  double best_time=double_best_score;
  sprintf(gamebesttimetxt,"Best Time: %5.3f",best_time);
  GrPrint(hdc,16+4,8+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+6,8+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+4,10+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+6,10+64,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+5,9+64,gamebesttimetxt,BROWN);

  char enemykills[32];
  int printenemykills=ENEMY_NUM-enemy_kills;
  int printenemynum=ENEMY_NUM;
  sprintf(enemykills,"Enemies Left: %d/%d",printenemykills,printenemynum);
  GrPrint(hdc,16+4,8+80,enemykills,YELLOW);
  GrPrint(hdc,16+6,8+80,enemykills,YELLOW);
  GrPrint(hdc,16+4,10+80,enemykills,YELLOW);
  GrPrint(hdc,16+6,10+80,enemykills,YELLOW);
  GrPrint(hdc,16+5,9+80,enemykills,BROWN);
//========================


  DrawPlayingMusic(hdc,16+4,8,c,c4);



//=========Draw Player UI
  int i=0,j=0;
  int c2,c3;

  if (!IsInvertedBackground()) {
    c2=RED;
    c3=BLACK;
  } else {
    c2=LTCYAN;
    c3=CYAN;
  }      
  //draw player block health
  for (i=0;i<player.block_health;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,5,BLACK,BLACK);
  }

  //draw player health
  for (i=0;i<player.health;i++) {
    j=i/10; //new row of hearts
    if (i<20) {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,4,c2,c3);
    } else {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+50+8*j,3,c,YELLOW);
    }
  }

  //GrLine(hdc,GR_WIDTH/2,0,GR_WIDTH/2,GR_HEIGHT,BLACK);
  //draw player speed
  //hehehehe
  for (i=0;i<player.speed;i++) {
    double speed_angle=i*0.1;
    double speed_dist=64;
    double angle_limit=M_PI_4+M_PI_2;
    if (speed_angle>angle_limit) {
      int speed_times=speed_angle/angle_limit;
      speed_angle-=angle_limit*speed_times;
      speed_dist=64+8*speed_times;
      if (!IsInvertedBackground()) {
        c2=LTCYAN;
      } else {
        c2=RED;
      }      
    } else {
      if (!IsInvertedBackground()) {
        if (i<5) c2=GREEN;
        else if (i<10) c2=LTGREEN;
        else c2=RED;
      } else {
        if (i<5) c2=DKGRAY;
        else if (i<10) c2=LTPURPLE;
        else c2=LTCYAN;
      }
    }
      GrCircle(hdc,
        player.sprite_x-speed_dist*cos(speed_angle),
        player.sprite_y-speed_dist*sin(speed_angle),
        3,c2,c2
      );
  }


  //draw player web left
  for (i=0;i<player.max_web_num-player.placed_web_num;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+64+8*j,player.sprite_y+8*(i%10)-(11*8)/2,3,LTCYAN,CYAN);
  }



  int c5;
  if (!player.time_breaker) {
    c5=PURPLE;
  } else {
    c5=LTPURPLE;
  }
  //draw player time breaker
  for (i=0;i<player.time_breaker_units;i++) {
    double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*i*2;
    GrCircle(hdc,
      player.sprite_x-32*cos(tb_angle),
      player.sprite_y-32*sin(tb_angle),
      2,c5,c5);
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    int frame_t=frame_tick%20;
    for (i=0;i<3;i++) {
      double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*(frame_t-i)*2;
      GrCircle(hdc,
        player.sprite_x-32*cos(tb_angle),
        player.sprite_y-32*sin(tb_angle),
        3,LTPURPLE,LTPURPLE);
    }
  }


  if (player.on_ground_id==-1) {
    GrLine(hdc,player.sprite_x+cos(player.angle_of_incidence)*50,player.sprite_y+sin(player.angle_of_incidence)*50,player.sprite_x+cos(player.angle_of_incidence)*60,player.sprite_y+sin(player.angle_of_incidence)*60,LTGREEN);
    //GrLine(hdc,player.sprite_x+cos(player.angle_of_reflection)*50,player.sprite_y+sin(player.angle_of_reflection)*50,player.sprite_x+cos(player.angle_of_reflection)*60,player.sprite_y+sin(player.angle_of_reflection)*60,YELLOW);
  }
  //GrLine(hdc,player.sprite_x+cos(player.angle_of_reflection)*50,player.sprite_y+sin(player.angle_of_reflection)*50,player.sprite_x+cos(player.angle_of_reflection)*60,player.sprite_y+sin(player.angle_of_reflection)*60,YELLOW);

/*
Controls:
W - Jump from Surface
A - Move Left (Anti-Clockwise)
S - Block or Spin
D - Move Right (Clockwise)
Z - Time Breaker Ability
C - Increase Reaction Time
Space - Sprint
Left Click - Attack and Stop Web Shooting
Right Click - Shoot web
Enter - Restart Level

*While Swinging
W - Decrease Web Length
A - Swing Anti-clockwise
S - Increase Web Length
D - Swing Clockwise
Left Click - Swing without Web Placement
Right Click - Swing with Wceb Placement
*/

  if (display_controls) {
  GrRect(hdc,16,GR_HEIGHT-128-16*26,8*42,128+16*22,c4);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*25,"Controls:",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*24,"'W' - Jump from Surface",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*23,"'A' - Move Left (Clockwise)",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*22,"'S' - Block / Spin / Drag Down Attack",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*21,"'D' - Move Right (Anti-Clockwise)",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*20,"'Q' - Pick Up Web",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*19,"'E' - Hold with Attack for Uppercut",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*18,"'Z' - Time Breaker Ability",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*17,"'C' - Increase Reaction Time",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*16,"'M' - Next Music",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*15,"[SHIFT] + 'M' - Change Song Playing Mode",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*14,"[Space] - Sprint",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*13,"[Left Click] or '1' - Attack and Stop Web Shooting",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*12,"[Right Click] - Shoot web",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*11,"[Enter] - Restart Level",c);

  GrPrint(hdc,16+4,GR_HEIGHT-128-16*9,"Controls While Swinging:",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*8,"'W' - Decrease Web Length",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*7,"'A' - Swing Clockwise",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*6,"'S' - Increase Web Length",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*5,"'D' - Swing Anti-Clockwise",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*4,"[Left Click] or '1' - Swing without Web Placement",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16*3,"[Right Click] - Swing with Web Placement",c);
  GrPrint(hdc,16+4,GR_HEIGHT-128-16,"[SHIFT_ESC] to Quit",c);
  } 

  GrPrint(hdc,16+4,GR_HEIGHT-128+16,"Press '*' for Controls Help",c4);
  GrPrint(hdc,16+5,GR_HEIGHT-128+17,"Press '*' for Controls Help",c);
}

