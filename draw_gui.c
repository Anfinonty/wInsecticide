

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
  int extra_h=0;
  if (hide_taskbar)
    extra_h=8*4;
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2+extra_h,
                map_platforms_sprite_mask,SRCAND,FALSE);
  //Draw platforms paint
  DrawBitmap(hDC,player.cam_move_x+player.cam_x+player.x-GR_WIDTH/2,
                 player.cam_move_y+player.cam_y+player.y-GR_HEIGHT/2,
                 player.x-GR_WIDTH/2,
                 player.y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+player.grav*2+extra_h,
                map_platforms_sprite,SRCPAINT,FALSE);
}


void DrawCursor(HDC hDC)
{
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite,SRCAND,FALSE);
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite_mask,SRCPAINT,FALSE);
  if (!(player.speed>24 && frame_tick%2==0)) {
    if (player.health>PLAYER_LOW_HEALTH) {
      GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_cache,FALSE);
    } else {
      GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_cache2,FALSE);
    }
  }
  if (!IsInvertedBackground())
    GrCircle(hDC,mouse_x,mouse_y,1,WHITE,-1);
  else
    GrCircle(hDC,mouse_x,mouse_y,1,BLACK,-1);
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
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /?] [/X :[N%cSHIFT%cM]: /%c]",171,187,177,171,187,187);
          break;
        case 1:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [/? :[N%cSHIFT%cM]: /%c]",171,187,177,187,171,187,171);
          break;
        case 2:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [/%c :[N%cSHIFT%cM]: /X]",171,187,177,171,187,171,187);
          break;
      }
      GrPrint(hdc,x,y+16,txt2,c);   
      GrPrint(hdc,x+1,y+1+16,txt2,c4);
    }
  } else {
    char txt2[72];
    sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /X] [/%c :[N%cSHIFT%cM]: /?]",171,187,177,171,171,187);
    GrPrint(hdc,x,y+16,txt2,c);   
    GrPrint(hdc,x+1,y+1+16,txt2,c4);
  }

  /*char txt3[6];
  sprintf(txt3,"%d",song_mode);
  GrPrint(hdc,x,y+32,txt3,c);
  GrPrint(hdc,x+1,y+32+1,txt3,c4);*/
}


void DrawMainMenu(HDC hdc)
{

  //draw bkgrnd
  DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE);

  //Draw Moon Phase
  GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);
  int help_y=GR_HEIGHT-128;
  if (!hide_taskbar) { //task bar is shown
    help_y-=8*4; //go up abit
  }

  //Moon Pos
  int mcalendar_l=64;
  int mcalendar_x=GR_WIDTH-mcalendar_l*2;
  int mcalendar_y=GR_HEIGHT-mcalendar_l*2-56;
  if (!hide_taskbar) { //task bar is shown
    mcalendar_y-=8*3; //go up abit
  }
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

  int main_menu_y=0;
  if (hide_taskbar)
    main_menu_y=15;
  GrPrintW(hdc,30,main_menu_y+10,L"អាពីងស៊ីរុយ - Welcome to the wInsecticide Menu!","",WHITE,16,FALSE,yes_unifont);







  int max_lvl_rows=10;
  char page_num[32];
  //Print Loaded Levels to Choose
  switch (main_menu_chosen) {
    case 0:
      sprintf(page_num,"Levels - [%d/%d]",(level_chosen/max_lvl_rows)+1,(level_num/max_lvl_rows)+1);
      GrPrint(hdc,30,main_menu_y+10+32,page_num,WHITE);

      int lvls_y=10+16*4;
      int lvl_i=0;

      if (hide_taskbar)
        lvls_y+=main_menu_y;

      for (int i=0;i<max_lvl_rows;i++) { //Print Levels
         lvl_i=i+10*(level_chosen/max_lvl_rows);
         //GrPrint(hdc,30,lvls_y+16*i,"-_________",WHITE);
         GrPrint(hdc,20,lvls_y+16*i,"-",WHITE);
         if (lvl_i<level_num) {
           //GrPrintW(hdc,30+8*11,lvls_y+16*i,level_names[lvl_i],"",WHITE,16,FALSE,yes_unifont);
           if (level_chosen!=lvl_i) {
             GrPrintW(hdc,30,lvls_y+16*i,level_names[lvl_i],"",WHITE,16,FALSE,yes_unifont);
           } else {
             GrPrint(hdc,20,lvls_y+16*i,"-",LTGREEN);
             GrPrintW(hdc,30,lvls_y+16*i,level_names[lvl_i],"",LTGREEN,16,FALSE,yes_unifont);
           }
         } else {
           GrPrint(hdc,20,lvls_y+16*i,"__________",WHITE);
         }
      }

      //Draw Level Selector
      //GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows),"   [ENTER]",WHITE);
      GrPrint(hdc,20,2+lvls_y+16*(level_chosen%max_lvl_rows),"*",LTGREEN);
      //GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows)-16,"        /\\",WHITE);
      //GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows)+16,"        \\/",WHITE);
      //DrawPlayingMusic(hdc,30,10+16*16,BLACK,WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*16,"'1': Go to Options.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*17,"'2': Create New Level.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*18,"'3': Edit Selected Level.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*19,"'4': Build Selected Level.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*21,"[SHIFT_ESC]: Exit.",WHITE);
      break;


    case 1:
      GrPrint(hdc,30,main_menu_y+10+16*2,"Options:",WHITE);
      //GrPrint(hdc,30,main_menu_y+10+16*4,"[SHIFT_ESC]: Exit",WHITE);
      GrPrintW(hdc,30,main_menu_y+10+16*4,L"[SHIFT] + 'L': Unifont [ពេលរាត្រី]","",WHITE,16,FALSE,yes_unifont);
      GrPrint(hdc,30,main_menu_y+10+16*5,"[SHIFT] + 'T': Toggle Borders",WHITE);

 
      int c=WHITE,soptions_y=16*8;
      if (hide_taskbar)
        soptions_y+=main_menu_y;
      if (option_choose==0) {
        c=LTGREEN;
      }
      GrPrint(hdc,30,10+soptions_y,"Player Color:",c);
      GrPrint(hdc,30+13*8,10+soptions_y,"<    >",c);
      //Draw Square
      if (player_color!=0) {
        GrRect(hdc,30+8*14,10+soptions_y,16,16,BLACK);
      } else {
        GrRect(hdc,30+8*14,10+soptions_y,16,16,WHITE);
      }
      if (player_color>-1 && player_color<COLORS_NUM) {
        GrRect(hdc,30+8*14+2,10+soptions_y+2,12,12,draw_color_arr[player_color]);
      }


      c=WHITE;
      if (option_choose==1) {
        c=LTGREEN;
      }
      GrPrint(hdc,30,10+soptions_y+16,"Audio:",c);
      if (game_audio) {
        GrPrint(hdc,30+13*8,10+soptions_y+16,"<ON>",c);
      } else {
        GrPrint(hdc,30+13*8,10+soptions_y+16,"<OFF>",c);
      }


      c=WHITE;
      if (option_choose==2) {
        c=LTGREEN;
      }
      GrPrint(hdc,30,10+soptions_y+16*2,"Camera Shake:",c);
      if (game_cam_shake) {
        GrPrint(hdc,30+13*8,10+soptions_y+16*2,"<ON>",c);
      } else {
        GrPrint(hdc,30+13*8,10+soptions_y+16*2,"<OFF>",c);
      }

      GrPrint(hdc,20,10+soptions_y+16*option_choose,"*",LTGREEN);


      GrPrint(hdc,30,main_menu_y+10+16*14,"'1': Go back to Main Menu.",WHITE);
      break;





    case 2:
      GrPrint(hdc,30,main_menu_y+10+16*2,"Create New Level",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*4,"[Enter]: Create Level",WHITE);

      GrPrintW(hdc,30,main_menu_y+10+16*5,L"[SHIFT] + 'L': Unifont [ពេលរាត្រី]","",WHITE,16,FALSE,yes_unifont);
     // GrPrint(hdc,30,main_menu_y+10+16*6,"[SHIFT] + 'T': Toggle Borders",WHITE);

 
      GrPrintW(hdc,30,main_menu_y+10+16*8,L"Level Name:","",WHITE,16,FALSE,yes_unifont);
      GrPrintW(hdc,30+13*8,main_menu_y+10+16*8,global_wchar,"",WHITE,16,FALSE,yes_unifont);


      GrPrint(hdc,30,main_menu_y+10+16*9,"Ground Amount:",WHITE);



      GrPrint(hdc,30,main_menu_y+10+16*10,"Enemies Amount:",WHITE);




      GrPrint(hdc,30,main_menu_y+10+16*11,"Level Width (Px):",WHITE);




      GrPrint(hdc,30,main_menu_y+10+16*12,"Level Height (Px):",WHITE);


      /*int c=WHITE,soptions_y=16*9;
      if (hide_taskbar)
        soptions_y+=main_menu_y;
      if (option_choose==0) {
        c=LTGREEN;
      }
      GrPrint(hdc,30,10+soptions_y,"Player Color:",c);
      GrPrint(hdc,30+13*8,10+soptions_y,"<    >",c);
      //Draw Square
      if (player_color!=0) {
        GrRect(hdc,30+8*14,10+soptions_y,16,16,BLACK);
      } else {
        GrRect(hdc,30+8*14,10+soptions_y,16,16,WHITE);
      }
      if (player_color>-1 && player_color<COLORS_NUM) {
        GrRect(hdc,30+8*14+2,10+soptions_y+2,12,12,draw_color_arr[player_color]);
      }


      c=WHITE;
      if (option_choose==1) {
        c=LTGREEN;
      }
      GrPrint(hdc,30,10+soptions_y+16,"Audio:",c);
      if (game_audio) {
        GrPrint(hdc,30+13*8,10+soptions_y+16,"<ON>",c);
      } else {
        GrPrint(hdc,30+13*8,10+soptions_y+16,"<OFF>",c);
      }


      c=WHITE;
      if (option_choose==2) {
        c=LTGREEN;
      }
      GrPrint(hdc,30,10+soptions_y+16*2,"Camera Shake:",c);
      if (game_cam_shake) {
        GrPrint(hdc,30+13*8,10+soptions_y+16*2,"<ON>",c);
      } else {
        GrPrint(hdc,30+13*8,10+soptions_y+16*2,"<OFF>",c);
      }

      GrPrint(hdc,20,10+soptions_y+16*option_choose,"*",LTGREEN);*/


      GrPrint(hdc,30,main_menu_y+10+16*14,"[SHIFT_ESC]: Go back to Main Menu.",WHITE);      
      break;
  }
  DrawPlayingMusic(hdc,16+4,help_y+48,BLACK,WHITE);
}







#define HELP_TEXT_ARR_NUM1   17
char help_txt_arr1[HELP_TEXT_ARR_NUM1][64]=
{
  "Controls:",
  "'W' - Jump from Surface",
  "'S'_- Block while on Ground",
  " |__- Hold while in air for Ricochet-Shuriken Attack",
  "'A' - Move Left (Clockwise)",
  "'D' - Move Right (Anti-Clockwise)",
  "'Q' - Pick Up Web Standing On",
  "'Z' - Time Breaker Ability",
  "'E'_- Hold with Attack for Uppercut",
  " |__- Hold with Movement to Break Jump or Flinging",
  " |__- Hold with Jump to Jump Once",
  "'2' - Change Web-Kunai per Throw [ 1/ 3/ 5(1)/ 15(3)]",
  "'3' - Toggle Low Jump",
  "[Space] or 'C' - Increase Reaction Time",
  "[Left Click] or '1' - Throw Web-Kunai & Stop Web-Sling",
  "[Right Click] - Sling Web",
  "[Enter] - Restart Level"
};



#define HELP_TEXT_ARR_NUM2   9
char help_txt_arr2[HELP_TEXT_ARR_NUM2][64]=
{
  "Controls While Swinging:",
  "'W' - Decrease Web Length - Move towards Pivot",
  "'S' - Increase Web Length - Move Away from Pivot",
  "'A' - Swing Clockwise",
  "'D' - Swing Anti-Clockwise",
  "'E' - Hold for no flinging after Web Placement",
  " |__- Hold for opposite lower quadrant swing",
  "[Left Click] or '1' - Swing without Web Placement",
  "[Right Click] - Swing with Web Placement"
};


//======== UI =========

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


  int help_y=GR_HEIGHT-128;
  if (!hide_taskbar) { //task bar is shown
    help_y-=8*4; //go up abit
  }

//======Print Game Score Text
  //GrRect(hdc,0,8+32,8*40,16*5,c4);

  char gametimetxt[32];
  double print_time_ms=(double)game_timer/1000;
  if (!game_over) {
    sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
    int digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
    GrPrint(hdc,digit_num-4,8+16,gametimetxt,YELLOW);
    GrPrint(hdc,digit_num-6,8+16,gametimetxt,YELLOW);
    GrPrint(hdc,digit_num-4,10+16,gametimetxt,YELLOW);
    GrPrint(hdc,digit_num-6,10+16,gametimetxt,YELLOW);
    GrPrint(hdc,digit_num-5,9+16,gametimetxt,BROWN);
  } else { //game is over
    if (game_timer<int_best_score) { //New Score :D
      sprintf(gametimetxt,"New Best! :D %5.3f",print_time_ms);
      int digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
      GrPrint(hdc,digit_num+5,9+16,gametimetxt,LTPURPLE);
    } else {
      sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
      int digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
      GrPrint(hdc,digit_num-4,8+16,gametimetxt,YELLOW);
      GrPrint(hdc,digit_num-6,8+16,gametimetxt,YELLOW);
      GrPrint(hdc,digit_num-4,10+16,gametimetxt,YELLOW);
      GrPrint(hdc,digit_num-6,10+16,gametimetxt,YELLOW);
      GrPrint(hdc,digit_num-5,9+16,gametimetxt,BROWN);
    }
  }

  char gamebesttimetxt[32];
  double best_time=double_best_score;
  sprintf(gamebesttimetxt,"Best: %5.3f",best_time);
  GrPrint(hdc,16+4,8+16,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+6,8+16,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+4,10+16,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+6,10+16,gamebesttimetxt,YELLOW);
  GrPrint(hdc,16+5,9+16,gamebesttimetxt,BROWN);

  /*int printenemykills=ENEMY_NUM-enemy_kills;
  int printenemynum=ENEMY_NUM;
  sprintf(enemykills,"Enemies Left: %d/%d",printenemykills,printenemynum);
  int digit_num=GR_WIDTH/2-(strlen(enemykills)*8)/2-4;
  GrPrint(hdc,digit_num-4,8+16,enemykills,YELLOW);
  GrPrint(hdc,digit_num-6,8+16,enemykills,YELLOW);
  GrPrint(hdc,digit_num-4,10+16,enemykills,YELLOW);
  GrPrint(hdc,digit_num-6,10+16,enemykills,YELLOW);
  GrPrint(hdc,digit_num-5,9+16,enemykills,BROWN);*/


  char enemykills[2];
  int printenemykills=ENEMY_NUM-enemy_kills;

  int digit_num3=0;
  if (printenemykills>0) {
    int digit_num1=GR_WIDTH/2-(12*8)/2+4+4;
    GrPrint(hdc,digit_num1-4,8+16,"Enemies Left:",YELLOW);
    GrPrint(hdc,digit_num1-6,8+16,"Enemies Left:",YELLOW);
    GrPrint(hdc,digit_num1-4,10+16,"Enemies Left:",YELLOW);
    GrPrint(hdc,digit_num1-6,10+16,"Enemies Left:",YELLOW);
    GrPrint(hdc,digit_num1-5,9+16,"Enemies Left:",BROWN);

    sprintf(enemykills,"%d",printenemykills);
  } else {
    digit_num3=-16;
    if (frame_tick<FPS/2)
      sprintf(enemykills,"GAME OVER");
    else
      sprintf(enemykills," ");
  }

  int digit_num2=GR_WIDTH/2-(strlen(enemykills)*8)/2+4;
  if (printenemykills<=0)
    digit_num2-=4;
  GrPrint(hdc,digit_num2-4,8+32+digit_num3,enemykills,YELLOW);
  GrPrint(hdc,digit_num2-6,8+32+digit_num3,enemykills,YELLOW);
  GrPrint(hdc,digit_num2-4,10+32+digit_num3,enemykills,YELLOW);
  GrPrint(hdc,digit_num2-6,10+32+digit_num3,enemykills,YELLOW);
  GrPrint(hdc,digit_num2-5,9+32+digit_num3,enemykills,BROWN);


//========================


  DrawPlayingMusic(hdc,16+4,help_y+48,c,c4);



//=========Draw Player UI
  int i=0,j=0;
  int c2,c3;

  if (!IsInvertedBackground()) {
    c2=LTRED;
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
    if (i<DEFAULT_PLAYER_HEALTH) {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,4,c2,c3);
    } else {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+50+8*j,3,c,YELLOW);
    }
  }

  //draw player block health
  for (i=0;i<player.block_health;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,1,c,c);
  }

  //GrLine(hdc,GR_WIDTH/2,0,GR_WIDTH/2,GR_HEIGHT,BLACK);
  //===--- Draw player speed ---===
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
  /*for (i=0;i<player.max_web_num-player.placed_web_num;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+64+8*j,player.sprite_y+8*(i%10)-(11*8)/2,3,LTCYAN,CYAN);
  }*/
  int b_dmg_m=1;
  int c5=LTGREEN;
  if (!IsInvertedBackground()) {
    if (player.speed>10) {
      b_dmg_m=2;
      c5=LTRED;
    }
    if (player.speed>24) {
      b_dmg_m=4;
      c5=LTCYAN;
    }
  } else {
    c5=LTPURPLE;
    if (player.speed>10) {
      b_dmg_m=2;
      c5=LTCYAN;
    }
    if (player.speed>24) {
      b_dmg_m=4;
      c5=RED;
    }
  }



  char shuriken_dmg_txt[40];
  int print_shuriken_dmg=player.attack_strength;
  if (!display_controls)
    sprintf(shuriken_dmg_txt,"<:-%d>",print_shuriken_dmg);
  else
    sprintf(shuriken_dmg_txt,"<:-%d> Ricochet-Shuriken:-DMG",print_shuriken_dmg);

  GrPrint(hdc,player.sprite_x+48-1,player.sprite_y-48-1,shuriken_dmg_txt,c2);
  GrPrint(hdc,player.sprite_x+48,player.sprite_y-48,shuriken_dmg_txt,c5);
  

  char bullet_num_txt[48];
  int print_snipe_dmg=4+player.attack_strength*2;
  if (!display_controls)
    sprintf(bullet_num_txt,"[%d]:-%d",player.max_web_num-player.placed_web_num,print_snipe_dmg);
  else
    sprintf(bullet_num_txt,"[%d]:-%d Web-Slings Remaining:-DMG",player.max_web_num-player.placed_web_num,print_snipe_dmg);

  char bullet_num_txt_[8];
  sprintf(bullet_num_txt_,"[%d]:",player.max_web_num-player.placed_web_num);

  GrPrint(hdc,player.sprite_x+48,player.sprite_y-32,bullet_num_txt,c5);
  GrPrint(hdc,player.sprite_x+48,player.sprite_y-32,bullet_num_txt_,LTCYAN);




  char bullet_num_txt2[48];
  char bullet_num_txt2_[8];
  if (!display_controls)
    sprintf(bullet_num_txt2,"[%d/%d]:-%d",PLAYER_BULLET_NUM-player.bullet_shot_num,player.knives_per_throw,b_dmg_m);
  else
    sprintf(bullet_num_txt2,"[%d/%d]:-%d Web-Kunai/throw:-DMG",PLAYER_BULLET_NUM-player.bullet_shot_num,player.knives_per_throw,b_dmg_m);

  sprintf(bullet_num_txt2_,"[%d/%d]",PLAYER_BULLET_NUM-player.bullet_shot_num,player.knives_per_throw);

  if (player.knives_per_throw<5) {
    GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2,c5);
    GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2_,c);
  } else {
    if (player.knives_per_throw>5)
      GrPrint(hdc,player.sprite_x+48-1,player.sprite_y-16-1,bullet_num_txt2,CYAN);
    GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2,c5);
    GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2_,LTCYAN);
  }



  //draw perfect block
  if (player.block_timer>0) {
    GrPrint(hdc,player.sprite_x+48,player.sprite_y,"*",c);
    if (player.block_timer<=23) {
      GrPrint(hdc,player.sprite_x+48,player.sprite_y,"*&",c);
    }
  }
  if (display_controls) {
    GrPrint(hdc,player.sprite_x+48,player.sprite_y,   "      *& = Perfect-Block || * = Block",c);
    GrPrint(hdc,player.sprite_x+48,player.sprite_y+16,"      Block = ++Speed",c);
  }


  if (player.uppercut) {
    GrPrint(hdc,player.sprite_x+48,player.sprite_y+16,"e",c);
  }

  if (player.low_jump) {
    GrPrint(hdc,player.sprite_x+48,player.sprite_y+32,"3",LTGREEN);
  }

  //draw perfect block -- too confusing
  /*if (player.on_ground_id==-1 && player.block_timer>0 && player.block_timer<=23) {
    GrCircle(hdc,player.sprite_x,player.sprite_y,1,LTCYAN,-1);
  }*/



  //===-- Draw Timebreaker Circle ---===

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

  if (display_controls) {
    if (!player.is_swinging) { //swinging
      //GrRect(hdc,16,GR_HEIGHT-128-16*(HELP_TEXT_ARR_NUM1+2),8*42,128+16*22,c4);
      for (int i=0;i<HELP_TEXT_ARR_NUM1;i++) {
        GrPrint(hdc,16+4,help_y-16*(i+2),help_txt_arr1[HELP_TEXT_ARR_NUM1-1-i],c4);
        GrPrint(hdc,16+5,help_y+1-16*(i+2),help_txt_arr1[HELP_TEXT_ARR_NUM1-1-i],c);
      }
    } else { //not swinging
      //GrRect(hdc,16,GR_HEIGHT-128-16*(HELP_TEXT_ARR_NUM2+2),8*42,128+16*22,c4);
      for (int i=0;i<HELP_TEXT_ARR_NUM2;i++) {
        GrPrint(hdc,16+4,help_y-16*(i+2),help_txt_arr2[HELP_TEXT_ARR_NUM2-1-i],c4);
        GrPrint(hdc,16+5,help_y+1-16*(i+2),help_txt_arr2[HELP_TEXT_ARR_NUM2-1-i],c);
      }
    }
    GrPrint(hdc,16+4,help_y-16,"[SHIFT_ESC] to Quit",c4);
    GrPrint(hdc,16+5,help_y+1-16,"[SHIFT_ESC] to Quit",c);
  } 

  GrPrint(hdc,16+4,help_y+16,"Press '*' for Controls Help",c4);
  GrPrint(hdc,16+5,help_y+17,"Press '*' for Controls Help",c);
}


