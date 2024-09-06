

//Background
void DrawBackground(HDC hdc) {
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(253, 2, 139));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(173, 216, 230));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(8,39,245));
//  GrRect(hwnd,hdc,ps,0,0,GR_WIDTH,GR_HEIGHT,RGB(RandNum(0,255),RandNum(0,255),RandNum(0,255))); //RAVE
  switch (map_background) {
    case 0:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
      break;
    case 1:
      DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,NOTSRCCOPY,FALSE,FALSE);
      //GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);
//      DrawSprite(hdc, GR_WIDTH-128,128,&draw_moon_sprite,FALSE);
      DrawSprite(hdc, GR_WIDTH-128,128,&draw_moon_sprite,FALSE);
      break;
    default:
      if (map_background_sprite==NULL) {
        GrRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,custom_map_background_color);
      } else {
        if (IsInvertedBackground()) {
          DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,NOTSRCCOPY,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);
        }
      }
      break;
  }
}



/*void DrawForeground(HDC hdc) {
  if (IsInvertedBackground()) {
    //GrGlassRect(hdc,player.sprite_x-32,player.sprite_y-32,64,64,draw_color_arr[player_load_pupil_color],128);
    GrGlassCircle(hdc,player.sprite_x,player.sprite_y,32,LTGREEN,128);
  }
}*/


void DrawWaterShader(HDC hdc) 
{
  if (player.in_water) {
    if (!IsInvertedBackground()) {
      GrGlassRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,BLUE,128);
    } else {
      GrGlassRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,LTR2YELLOW,128);
    }
  }
}


void DrawPlatforms(HDC hDC)
{ //Dynamically scale with window size 
  //Draw platforms bitmap mask
  int extra_h=0;
  if (hide_taskbar)
    extra_h=8*4;
  DrawBitmap(hDC,
                 0,
                 0,
                 player.x-player.cam_mouse_move_x-player.cam_move_x-GR_WIDTH/2,
                 player.y-player.cam_mouse_move_y-player.cam_move_y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+extra_h,
                 map_platforms_sprite_mask,SRCAND,FALSE,FALSE);
  //Draw platforms paint
  DrawBitmap(hDC,
                 0,
                 0,                    
                 player.x-player.cam_mouse_move_x-player.cam_move_x-GR_WIDTH/2,
                 player.y-player.cam_mouse_move_y-player.cam_move_y-GR_HEIGHT/2,
                 GR_WIDTH,
                 GR_HEIGHT+extra_h,
                 map_platforms_sprite,SRCPAINT,FALSE,FALSE);
}


void DrawCursor(HDC hDC)
{
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite,SRCAND,FALSE);
  //DrawBitmap(hDC,mouse_x,mouse_y,0,0,64,64,mouse_cursor_sprite_mask,SRCPAINT,FALSE);
  if (!(player.speed>24 && frame_tick%2==0)) {
    if (player.health>PLAYER_LOW_HEALTH) {
      //GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_cache,FALSE);
      //GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_iris_cache,FALSE);
      DrawSprite(hDC,mouse_x,mouse_y,&draw_mouse_cursor_sprite,FALSE);
      DrawSprite(hDC,mouse_x,mouse_y,&draw_mouse_cursor_sprite_iris,FALSE);
    } else {
      //GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_cache2,FALSE);
      //GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_iris_cache2,FALSE);
      DrawSprite(hDC,mouse_x,mouse_y,&draw_mouse_cursor_sprite2,FALSE);
      DrawSprite(hDC,mouse_x,mouse_y,&draw_mouse_cursor_sprite_iris2,FALSE);
    }
  }
  if (player.health>PLAYER_LOW_HEALTH) {
    DrawSprite(hDC,mouse_x,mouse_y,&draw_mouse_cursor_sprite_pupil,FALSE);
    //GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_pupil_cache,FALSE);
  } else {
    DrawSprite(hDC,mouse_x,mouse_y,&draw_mouse_cursor_sprite_pupil2,FALSE);
    //GrSprite(hDC,mouse_x,mouse_y,mouse_cursor_sprite_pupil_cache2,FALSE);
  }

  /*if (!IsInvertedBackground())
    GrCircle(hDC,mouse_x,mouse_y,1,WHITE,-1);
  else
    GrCircle(hDC,mouse_x,mouse_y,1,BLACK,-1);*/
}




void DrawPlayingMusic(HDC hdc,int x,int y,int c, int c4)
{
  if (!stop_playing_song) {

    if (song_num>0) {
      wchar_t txt[32+256];
      if (time_song_end==-1 && current_song_time==-1) {
        call_help_timer=0;
        if (showoff%30<5 || showoff%30>25) {
          swprintf(txt,32+256,L"%c%d/%d%c: %s  [.  ]",171,song_rand_num+1,song_num,187,song_names[song_rand_num]);
        } else if ((showoff%30>4 && showoff%30<10) || showoff%30>19){
          swprintf(txt,32+256,L"%c%d/%d%c: %s  [.. ]",171,song_rand_num+1,song_num,187,song_names[song_rand_num]);
        } else {
          swprintf(txt,32+256,L"%c%d/%d%c: %s  [...]",171,song_rand_num+1,song_num,187,song_names[song_rand_num]);
        }

      } else {
        int songtimediff;
        if (current_song_time!=-1)
          songtimediff=(int)((time_song_end-current_song_time)/1000);
        else
          songtimediff=(int)((songAudio.duration)/1000);
        int min=songtimediff/60;
        int seconds=songtimediff%60;
        swprintf(txt,32+256,L"%c%d/%d%c: %s  [%d:%02d]",171,song_rand_num+1,song_num,187,song_names[song_rand_num],min,seconds);
      }
      //%c 187
      //wchar_t txt[32+256];
      //swprintf(txt,32+256,L"%c%d/%d%c: %s  []",171,song_rand_num+1,song_num,187,song_names[song_rand_num]);
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

  /*char txt3[32];
  sprintf(txt3,"%d/%d",(int)current_song_duration,(int)song_duration);
  GrPrint(hdc,x,y+32,txt3,c);*/
  
  /*char txt3[6];
  sprintf(txt3,"%d",song_mode);
  GrPrint(hdc,x,y+32,txt3,c);
  GrPrint(hdc,x+1,y+32+1,txt3,c4);*/
}


void DrawMainMenu(HDC hdc)
{

  //draw bkgrnd
  DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);


  //Draw Moon Phase
  //GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);
  //DrawSprite(hdc, GR_WIDTH-128,128,&draw_moon_sprite,FALSE);
  DrawSprite(hdc, GR_WIDTH-128,128,&draw_moon_sprite,FALSE);

  DrawBitmap(hdc,GR_WIDTH/2-352/2,
                 -32,
                 0,
                 0,
                 352,
                 256,
                title_sprite_mask,SRCAND,FALSE,FALSE);
  //Draw platforms paint
  DrawBitmap(hdc,GR_WIDTH/2-352/2,
                 -32,
                 0,
                 0,
                 352,
                 256,
                title_sprite,SRCPAINT,FALSE,FALSE);


  int help_y=GR_HEIGHT-128;
  if (!hide_taskbar) { //task bar is shown
    help_y-=8*4; //go up abit
  }

  //Moon Pos
  int mcalendar_l=64;
  int mcalendar_x=GR_WIDTH-mcalendar_l*2;
  int mcalendar_y=GR_HEIGHT-mcalendar_l*2-40+8;//56;
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

  if (main_menu_chosen==-1 || (GR_WIDTH>=800 && GR_HEIGHT>=600)) {
    GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-64,time_row1,"",WHITE,16,FALSE,yes_unifont);
    GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-32,L"",s_hijri_row1,WHITE,16,TRUE,yes_unifont);
    GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y-16,s_hijri_row2,"",WHITE,16,FALSE,yes_unifont);
    GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y+16,L"",l_hijri_row1,WHITE,16,TRUE,yes_unifont);
    GrPrintW(hdc,mcalendar_x-mcalendar_l*6,mcalendar_y+32,l_hijri_row2,"",WHITE,16,FALSE,yes_unifont);
  }

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
  //GrPrintW(hdc,30,main_menu_y+10,L"អាពីងស៊ីរុយ - Welcome to the wInsecticide Menu!","",WHITE,16,FALSE,yes_unifont);







  //int max_lvl_rows=15;
  int max_lvl_rows=10;
  char page_num[32];
  //Print Loaded Levels to Choose
  switch (main_menu_chosen) {
    case -1: { //Main menu
/*      GrPrintA(hdc,30,main_menu_y,L"\n\
\n\
\n\
'Go to Options.\n\
'Create New Level.\n\
'Edit Selected Level.\n\
'Build Selected Level.\n\
[SHIFT_ESC]: Exit."
        ,WHITE);*/
      int c;
      c=Highlight((select_main_menu==0),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*2,"Levels.",c);

      c=Highlight((select_main_menu==1),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*3,"Options.",c);

      /*c=Highlight((select_main_menu==2),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*4,"Create New Level.",c);

      c=Highlight((select_main_menu==3),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*5,"Edit Selected Level.",c);

      c=Highlight((select_main_menu==4),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*6,"Build Selected Level.",c);*/


      if (select_main_menu<2)
        GrPrint(hdc,20,main_menu_y+10+16*2+16*select_main_menu,"*",LTGREEN);

      c=Highlight((select_main_menu==2),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*10,"[SHIFT]+'Q':  Exit.",c);
    }
      break;
     

    //Level chooser
    case 0:
    {
      int current_page=(level_chosen/max_lvl_rows)+1;
      double max_page=ceil(((double)(level_num)/max_lvl_rows));
      sprintf(page_num,"Levels - [%d/%1.0f]",current_page,max_page);
      GrPrint(hdc,30,main_menu_y+10+32,page_num,WHITE);

      int lvls_y=10+16*4+4;
      int lvl_i=0;

      if (hide_taskbar)
        lvls_y+=main_menu_y;


      if (current_page>1) {
        GrPrint(hdc,20,lvls_y-16,"- ...",WHITE);
      }
      for (int i=0;i<max_lvl_rows;i++) { //Print Levels
         lvl_i=i+max_lvl_rows*(level_chosen/max_lvl_rows);
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
      if (current_page<max_page && max_page>1) {
        GrPrint(hdc,20,lvls_y+16*max_lvl_rows,"- ...",WHITE);
      }


      //Draw Level Selector
      //GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows),"   [ENTER]",WHITE);
      GrPrint(hdc,20,2+lvls_y+16*(level_chosen%max_lvl_rows),"*",LTGREEN);
      //GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows)-16,"        /\\",WHITE);
      //GrPrint(hdc,30,lvls_y+16*(level_chosen%max_lvl_rows)+16,"        \\/",WHITE);
      //DrawPlayingMusic(hdc,30,10+16*16,BLACK,WHITE);
      //GrPrint(hdc,30,main_menu_y+10+16*16,"'1': Go to Options.",WHITE);
      //GrPrint(hdc,30,main_menu_y+10+16*17,"'2': Create New Level.",WHITE);
      //GrPrint(hdc,30,main_menu_y+10+16*18,"'3': Edit Selected Level.",WHITE);
      //GrPrint(hdc,30,main_menu_y+10+16*19,"'4': Build Selected Level.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*16,"'1': Create New Level.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*17,"'2': Change Selected Level Limits.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*18,"'3': Build Selected Level.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*19,"[SHIFT]+[BACKSPACE]: Delete Map.",WHITE);
      GrPrint(hdc,30,main_menu_y+10+16*21,"[SHIFT_ESC]: Back.",WHITE);
      }
      break;


    //Game Options
    case 1:
      GrPrint(hdc,30,main_menu_y+10+16*2,"Options:",WHITE);
      //GrPrint(hdc,30,main_menu_y+10+16*4,"[SHIFT_ESC]: Exit",WHITE);
      //GrPrintW(hdc,30,main_menu_y+10+16*4,L"[SHIFT] + 'L': Unifont [ពេលរាត្រី]","",WHITE,16,FALSE,yes_unifont);
      //GrPrint(hdc,30,main_menu_y+10+16*5,"[SHIFT] + 'T': Toggle Borders",WHITE);

 
      int c,soptions_y=16*4;
      if (hide_taskbar)
        soptions_y+=main_menu_y;

      //Graphics
      c=Highlight((option_choose==0),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y,"Player Color:",c);
      GrPrint(hdc,30+20*8,10+soptions_y,"<    >",c);
      //Draw Square
      if (player_color!=0) {
        GrRect(hdc,30+8*21,10+soptions_y,16,16,BLACK);
      } else {
        GrRect(hdc,30+8*21,10+soptions_y,16,16,WHITE);
      }
      if (player_color>-1 && player_color<COLORS_NUM) {
        GrRect(hdc,30+8*21+2,10+soptions_y+2,12,12,draw_color_arr[player_color]);
      }



      c=Highlight((option_choose==1),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16,"Iris:",c);
      GrPrint(hdc,30+20*8,10+soptions_y+16,"<    >",c);
      //Draw Square
      if (player_iris_color!=0) {
        GrRect(hdc,30+8*21,10+soptions_y+16,16,16,BLACK);
      } else {
        GrRect(hdc,30+8*21,10+soptions_y+16,16,16,WHITE);
      }
      if (player_iris_color>-1 && player_color<COLORS_NUM) {
        GrRect(hdc,30+8*21+2,10+soptions_y+16+2,12,12,draw_color_arr[player_iris_color]);
      }


      c=Highlight((option_choose==2),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*2,"Pupil:",c);
      GrPrint(hdc,30+20*8,10+soptions_y+16*2,"<    >",c);
      //Draw Square
      if (player_pupil_color!=0) {
        GrRect(hdc,30+8*21,10+soptions_y+16*2,16,16,BLACK);
      } else {
        GrRect(hdc,30+8*21,10+soptions_y+16*2,16,16,WHITE);
      }
      if (player_pupil_color>-1 && player_color<COLORS_NUM) {
        GrRect(hdc,30+8*21+2,10+soptions_y+2+16*2,12,12,draw_color_arr[player_pupil_color]);
      }


      //Game Options

      c=Highlight((option_choose==3),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*3,"Camera Shake:",c);
      if (game_cam_shake) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*3,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*3,"<OFF>",c);
      }


      c=Highlight((option_choose==4),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*5,"Audio:",c);
      if (game_audio) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*5,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*5,"<OFF>",c);
      }


      //Sound=============
      c=Highlight((option_choose==5),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*6,"Sound Effects Volume:",c);
      char print_volume[8];
      sprintf(print_volume,"<%1.0f%>",game_volume*100);
      GrPrint(hdc,30+20*8,10+soptions_y+16*6,print_volume,c);



      //c=Highlight((option_choose==6),WHITE,LTGREEN);
      //GrPrint(hdc,30,10+soptions_y+16*7,"Encoded Music Volume:",c);
      //char print_song_volume[8];
      //sprintf(print_song_volume,"<%1.0f%%>",song_volume*100);
      //GrPrint(hdc,30+20*8,10+soptions_y+16*7,print_song_volume,c);



      c=Highlight((option_choose==6),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*7,"Raw Wav Volume:",c);
      char print_wav_out_volume[7];
      sprintf(print_wav_out_volume,"<%1.0f%%>",wav_out_volume*100);
      GrPrint(hdc,30+20*8,10+soptions_y+16*7,print_wav_out_volume,c);


      c=Highlight((option_choose==7),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*9,"Unifont:",c);
      if (yes_unifont) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*9,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*9,"<OFF>",c);
      }



      //Misc============
      c=Highlight((option_choose==8),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*10,"Window Borders:",c);
      if (!hide_taskbar) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*10,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*10,"<OFF>",c);
      }



      c=Highlight((option_choose==9),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*11,"Toggle Resolution:",c);
      switch (resolution_choose) {
        case 0:
          GrPrint(hdc,30+20*8,10+soptions_y+16*11,"<640x480>",c);
          break;
        case 1:
          GrPrint(hdc,30+20*8,10+soptions_y+16*11,"<800x600>",c);
          break;
        case 2: {
          char print_screen_size[20];
          sprintf(print_screen_size,"<%dx%d> (MAX)",SCREEN_WIDTH,SCREEN_HEIGHT);
          GrPrint(hdc,30+20*8,10+soptions_y+16*11,print_screen_size,c);
          }
          break;
      } 

      c=Highlight((option_choose==10),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*12,"Window Align Position:",c);
      GrPrint(hdc,30+20*8,10+soptions_y+16*12,"<Corner || Middle>",c);



      c=Highlight((option_choose==11),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*13,"Show FPS:",c);
      if (show_fps) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<OFF>",c);
      }



      //===========================

      int add_option_choose=0;
      if (option_choose>6) {
        add_option_choose=16*2;
      } else if (option_choose>3) {
        add_option_choose=16;
      }
      GrPrint(hdc,20,10+soptions_y+16*option_choose+add_option_choose,"*",LTGREEN);


      if (hide_taskbar)
        GrPrint(hdc,30,main_menu_y+10+16*22,"[SHIFT_ESC]: Back.",WHITE);
      else
        GrPrint(hdc,30,main_menu_y+10+16*21,"[SHIFT_ESC]: Back.",WHITE);
      break;





    case 2: //Create New Level 
    case 3:
      if (main_menu_chosen==2)
        GrPrint(hdc,30,main_menu_y+10+16*2,"Create New Level",WHITE);
      else
        GrPrint(hdc,30,main_menu_y+10+16*2,"Edit Level Limits",WHITE);
     // GrPrintW(hdc,30,main_menu_y+10+16*5,L"[SHIFT] + 'L': Unifont [ពេលរាត្រី]","",WHITE,16,FALSE,yes_unifont);
     // GrPrint(hdc,30,main_menu_y+10+16*6,"[SHIFT] + 'T': Toggle Borders",WHITE);

      int create_lvl_y=main_menu_y+10+16*4;
      c=Highlight((create_lvl_option_choose==0),WHITE,LTGREEN);
      GrPrintW(hdc,30,create_lvl_y,L"Level Name:","",c,16,FALSE,yes_unifont);
      if (typing_lvl_name_pos>0)
        GrPrintW(hdc,30+20*8,create_lvl_y,typing_lvl_name,"",c,16,FALSE,yes_unifont);


      c=Highlight((create_lvl_option_choose==1),WHITE,LTGREEN);
      GrPrint(hdc,30,create_lvl_y+16,"Ground Amount:",c);
      char print_set_ground_amount[8];
      sprintf(print_set_ground_amount,"<%d>",set_ground_amount);
      GrPrint(hdc,30+20*8,create_lvl_y+16,print_set_ground_amount,c);
      

      c=Highlight((create_lvl_option_choose==2),WHITE,LTGREEN);
      GrPrint(hdc,30,create_lvl_y+16*2,"Enemies Amount:",c);
      char print_set_enemy_amount[8];
      sprintf(print_set_enemy_amount,"<%d>",set_enemy_amount);
      GrPrint(hdc,30+20*8,create_lvl_y+16*2,print_set_enemy_amount,c);


      c=Highlight((create_lvl_option_choose==3),WHITE,LTGREEN);
      GrPrint(hdc,30,create_lvl_y+16*3,"Level Width (Px):",c);
      char print_set_map_width_amount[16];
      sprintf(print_set_map_width_amount,"<%d>",set_map_width_amount);
      GrPrint(hdc,30+20*8,create_lvl_y+16*3,print_set_map_width_amount,c);


      c=Highlight((create_lvl_option_choose==4),WHITE,LTGREEN);
      GrPrint(hdc,30,create_lvl_y+16*4,"Level Height (Px):",c);
      char print_set_map_height_amount[16];
      sprintf(print_set_map_height_amount,"<%d>",set_map_height_amount);
      GrPrint(hdc,30+20*8,create_lvl_y+16*4,print_set_map_height_amount,c);

      GrPrint(hdc,20,create_lvl_y+16*create_lvl_option_choose,"*",LTGREEN);

      int map_ratio1=set_map_width_amount/160;
      int map_ratio2=set_map_height_amount/160;
      int calc_grid_value=map_ratio1*map_ratio2;
      char print_calc_grid_value[32];
      sprintf(print_calc_grid_value,"%d (Must be less than 4800)",map_ratio1*map_ratio2);

      c=Highlight(calc_grid_value>MAX_VGRID_NUM,WHITE,LTRED);
      GrPrint(hdc,30,create_lvl_y+16*7,"Grid Amount:",c);
      GrPrint(hdc,30+20*8,create_lvl_y+16*7,print_calc_grid_value,c);


      char print_calc_ratio_value[32];

      GrPrint(hdc,30,create_lvl_y+16*8,"Level Grids Ratio:",WHITE);
      sprintf(print_calc_ratio_value,"%d:%d",map_ratio1,map_ratio2);
      GrPrint(hdc,30+20*8,create_lvl_y+16*8,print_calc_ratio_value,WHITE);


      if (main_menu_chosen==2)
        GrPrint(hdc,30,main_menu_y+10+16*17,"[Enter]: Create Level",WHITE);
      else
        GrPrint(hdc,30,main_menu_y+10+16*17,"[Enter]: Change Level Limits",WHITE);


      GrPrint(hdc,30,main_menu_y+10+16*18,"[SHIFT_ESC]: Back.",WHITE);      
      break;
  }
  DrawPlayingMusic(hdc,16+4,help_y+48,BLACK,WHITE);

  if (show_fps) {
    char fpstxt[10];
    sprintf(fpstxt,"FPS: %d / %d",saved_showoff,FPS);
    int FPS_x=GR_WIDTH-strlen(fpstxt)*8-14;
    if (hide_taskbar)
      GrPrint(hdc,FPS_x,main_menu_y+10+16,fpstxt,WHITE);
    else
      GrPrint(hdc,FPS_x,main_menu_y+10+32,fpstxt,WHITE);
  }
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
  "'E'_- Hold with Attack for Uppercut or Check HP",
  " |__- Hold with Movement to Break Jump or Flinging",
  " |__- Hold with Jump to Jump Once",
  "'2' - Change Web-Kunai per Throw [ 1/ 3/ 5(1)/ 15(3)]",
  "'3' - Toggle Low Jump",
  "[Space] or 'C' - Increase Reaction Time",
  "[Left Click] or '1' - Throw Web-Kunai & Stop Web-Sling",
  "[Right Click] or '4' - Sling Web",
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
  "[Right Click] or '4' - Swing with Web Placement"
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

  if (call_help_timer<5000) {
    DrawPlayingMusic(hdc,16+4,help_y+48,c,c4);
  }

  if (show_fps) {
    char fpstxt[10];
    sprintf(fpstxt,"FPS: %d / %d",saved_showoff,FPS);
    int FPS_x=GR_WIDTH-strlen(fpstxt)*8-14;
    GrPrint(hdc,FPS_x,10+32,fpstxt,c);
  }

//=========Draw Player UI
  int i=0,j=0;
  int c2,c3,ca;

  c2 = Highlight(IsInvertedBackground(),LTRED,LTCYAN);
  ca = Highlight(IsInvertedBackground(),RED,CYAN);
//  c3 = Highlight(IsInvertedBackground(),BLACK,CYAN);
  c3 = Highlight(IsInvertedBackground(),LTGRAY,DKGRAY);

  double percentage=player.health/DEFAULT_PLAYER_HEALTH;
  double health_length=64*percentage;

  double bpercentage=player.block_health/player.block_health_max;
  double bhealth_length=150*bpercentage;

  //Draw Block Health
  if (player.show_block_health_timer>0 || (player.health<=PLAYER_LOW_HEALTH+1)) {
    //if (bhealth_length>150)
      //bhealth_length=150;
    //GrRect(hdc,player.sprite_x-33,player.sprite_y-60,66,12,c4);
    //GrRect(hdc,player.sprite_x-bhealth_length/2,player.sprite_y-58,bhealth_length,8,c3);
//    GrRect(hdc,player.sprite_x-33,player.sprite_y-50-3,66,8,c4);
//    GrRect(hdc,player.sprite_x-32,player.sprite_y-50-2,bhealth_length,6,c3);
    GrRect(hdc,player.sprite_x-76,player.sprite_y-50-3,152,8,c4);
    GrRect(hdc,player.sprite_x-75,player.sprite_y-50-2,bhealth_length,6,c3);
  }

  //Draw Player Health
  if (player.show_health_timer>0 || (player.health<=PLAYER_LOW_HEALTH+1)) {
    if (health_length>150) {
      health_length=150;
    }

    if (player.health>PLAYER_LOW_HEALTH+1) {
      GrRect(hdc,player.sprite_x-health_length/2,player.sprite_y-42,health_length,4,c2);
    } else {
      health_length*=4;
      if (frame_tick%15<7) {
        GrRect(hdc,player.sprite_x-health_length/2,player.sprite_y-42,health_length,4,ca);
      } else {
        GrRect(hdc,player.sprite_x-health_length/2,player.sprite_y-42,health_length,4,c2);
      }
    }


    if (health_length>=150) {
      //print health numbers
      char txt[16];
      int print_health=player.health;
      sprintf(txt,"%d",print_health);
      int sprite_x_health=(int)player.sprite_x-strlen(txt)*8/2;
      GrPrint(hdc,sprite_x_health,player.sprite_y-48,txt,c);
    }
  }



  //draw player block health
  /*for (i=0;i<player.block_health;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,5,BLACK,BLACK);
  }*/

  //draw player health
  /*for (i=0;i<player.health;i++) {
    j=i/10; //new row of hearts
    if (i<DEFAULT_PLAYER_HEALTH) {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,4,c2,c3);
    } else {
      GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+50+8*j,3,c,YELLOW);
    }
  }*/

  //draw player block health
  /*for (i=0;i<player.block_health;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+8*(i%10)-(10*8)/2+4,player.sprite_y+48+8*j,1,c,c);
  }*/

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
      //player.sprite_x-speed_dist*cos(speed_angle),
      //player.sprite_y-speed_dist*sin(speed_angle),
      mouse_x-speed_dist*cos(speed_angle),
      mouse_y-speed_dist*sin(speed_angle),
      3,c2,c2
    );
  }


  //draw player web left
  /*for (i=0;i<player.max_web_num-player.placed_web_num;i++) {
    j=i/10; //new row
    GrCircle(hdc,player.sprite_x+64+8*j,player.sprite_y+8*(i%10)-(11*8)/2,3,LTCYAN,CYAN);
  }*/
  int b_dmg_m=1;
  if (player.knives_per_throw==1) {
    b_dmg_m=3;
  }
  int c5=LTGREEN;
  if (!IsInvertedBackground()) {
    if (player.speed>10) {
      if (player.knives_per_throw==1) {
        b_dmg_m=6;
      } else {
        b_dmg_m=2;
      }
      c5=LTRED;
    }
    if (player.speed>24) {
      if (player.knives_per_throw==1) {
        b_dmg_m=12;
      } else {
        b_dmg_m=4;
      }
      c5=LTCYAN;
    }
  } else {
    c5=LTPURPLE;
    if (player.speed>10) {
      if (player.knives_per_throw==1) {
        b_dmg_m=6;
      } else {
        b_dmg_m=2;
      }
      c5=LTCYAN;
    }
    if (player.speed>24) {
      if (player.knives_per_throw==1) {
        b_dmg_m=12;
      } else {
        b_dmg_m=4;
      }
      c5=RED;
    }
  }


  const int right_cursor_len=68;

  char shuriken_dmg_txt[42];
  int print_shuriken_dmg=player.attack_strength;
  if (!display_controls)
    sprintf(shuriken_dmg_txt,"<:-%d>",print_shuriken_dmg);
  else
    sprintf(shuriken_dmg_txt,"<:-%d> Melee / Ricochet-Shuriken:-DMG",print_shuriken_dmg);

  //GrPrint(hdc,player.sprite_x+48-1,player.sprite_y-48-1,shuriken_dmg_txt,c2);
  //GrPrint(hdc,player.sprite_x+48,player.sprite_y-48,shuriken_dmg_txt,c5);
  GrPrint(hdc,mouse_x+right_cursor_len-1,mouse_y-48-1,shuriken_dmg_txt,c2);
  GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-48,shuriken_dmg_txt,c5);
  

  char bullet_num_txt[48];
  int print_snipe_dmg=4+player.attack_strength*2;
  if (!display_controls)
    sprintf(bullet_num_txt,"[%d]:-%d",player.max_web_num-player.placed_web_num,print_snipe_dmg);
  else
    sprintf(bullet_num_txt,"[%d]:-%d Web-Slings Remaining:-DMG",player.max_web_num-player.placed_web_num,print_snipe_dmg);

  char bullet_num_txt_[8];
  sprintf(bullet_num_txt_,"[%d]:",player.max_web_num-player.placed_web_num);

//  GrPrint(hdc,player.sprite_x+48,player.sprite_y-32,bullet_num_txt,c5);
//  GrPrint(hdc,player.sprite_x+48,player.sprite_y-32,bullet_num_txt_,LTCYAN);
  GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-32,bullet_num_txt,c5);
  GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-32,bullet_num_txt_,LTCYAN);



  char bullet_num_txt2[48];
  char bullet_num_txt2_[8];
  if (!display_controls)
    sprintf(bullet_num_txt2,"[%d/%d]:-%d",PLAYER_BULLET_NUM-player.bullet_shot_num,player.knives_per_throw,b_dmg_m);
  else
    sprintf(bullet_num_txt2,"[%d/%d]:-%d Web-Kunai/throw:-DMG",PLAYER_BULLET_NUM-player.bullet_shot_num,player.knives_per_throw,b_dmg_m);

  sprintf(bullet_num_txt2_,"[%d/%d]",PLAYER_BULLET_NUM-player.bullet_shot_num,player.knives_per_throw);

  if (player.knives_per_throw<5) {
    //GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2,c5);
    //GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2_,c);

    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-16,bullet_num_txt2,c5);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-16,bullet_num_txt2_,c);
  } else {
    /*if (player.knives_per_throw>5)
      GrPrint(hdc,player.sprite_x+48-1,player.sprite_y-16-1,bullet_num_txt2,CYAN);
    GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2,c5);
    GrPrint(hdc,player.sprite_x+48,player.sprite_y-16,bullet_num_txt2_,LTCYAN);*/
    if (player.knives_per_throw>5)
      GrPrint(hdc,mouse_x+right_cursor_len-1,mouse_y-16-1,bullet_num_txt2,CYAN);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-16,bullet_num_txt2,c5);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y-16,bullet_num_txt2_,LTCYAN);
  }



  //draw perfect block
  if (player.block_timer>0) {
    /*GrPrint(hdc,player.sprite_x+48,player.sprite_y,"*",c);
    if (player.block_timer<=23) {
      GrPrint(hdc,player.sprite_x+48,player.sprite_y,"*&",c);
    }*/
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y,"*",c);
    if (player.block_timer<=23) {
      GrPrint(hdc,mouse_x+right_cursor_len,mouse_y,"*&",c);
    }
  }
  if (display_controls) {
    //GrPrint(hdc,player.sprite_x+48,player.sprite_y,   "      *& = Perfect-Block || * = Block",c);
    //GrPrint(hdc,player.sprite_x+48,player.sprite_y+16,"      Block = ++Speed",c);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y,   "      *& = Perfect-Block || * = Block",c);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y+16,"      Block = ++Speed",c);
  }


  if (player.uppercut) {
    //GrPrint(hdc,player.sprite_x+48,player.sprite_y+16,"e",c);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y+16,"e",c);
  }

  if (player.low_jump) {
    //GrPrint(hdc,player.sprite_x+48,player.sprite_y+32,"3",LTGREEN);
    GrPrint(hdc,mouse_x+right_cursor_len,mouse_y+32,"3",LTGREEN);
  }

  //draw perfect block -- too confusing
  /*if (player.on_ground_id==-1 && player.block_timer>0 && player.block_timer<=23) {
    GrCircle(hdc,player.sprite_x,player.sprite_y,1,LTCYAN,-1);
  }*/




  //===-- Draw Timebreaker Circle ---===
  if (!IsInvertedBackground()) {
    c5 = Highlight(player.time_breaker,PURPLE,LTPURPLE);
  } else {
    c5 = Highlight(player.time_breaker,LTRGREEN,LTGREEN);
  }

  int c6 = Highlight(IsInvertedBackground(),LTPURPLE,LTRLTGREEN);

  //draw player time breaker
  const int tb_circle_r=8;
  for (i=0;i<player.time_breaker_units;i++) {
    double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*i*2;
    /*GrCircle(hdc,
      player.sprite_x-32*cos(tb_angle),
      player.sprite_y-32*sin(tb_angle),
      2,c5,c5);*/
    GrCircle(hdc,
      mouse_x-tb_circle_r*cos(tb_angle),
      mouse_y-tb_circle_r*sin(tb_angle),
      2,c5,c5);
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    int frame_t=frame_tick%20;
    for (i=0;i<3;i++) {
      double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*(frame_t-i)*2;
      /*GrCircle(hdc,
        player.sprite_x-32*cos(tb_angle),
        player.sprite_y-32*sin(tb_angle),
        3,LTPURPLE,LTPURPLE);*/
      GrCircle(hdc,
        mouse_x-tb_circle_r*cos(tb_angle),
        mouse_y-tb_circle_r*sin(tb_angle),
        3,c6,c6);
    }
  }


  //if (player.on_ground_id==-1) {
    //GrLine(hdc,player.sprite_x+cos(player.angle_of_incidence)*50,player.sprite_y+sin(player.angle_of_incidence)*50,player.sprite_x+cos(player.angle_of_incidence)*60,player.sprite_y+sin(player.angle_of_incidence)*60,LTGREEN);
    //GrLine(hdc,player.sprite_x+cos(player.angle_of_reflection)*50,player.sprite_y+sin(player.angle_of_reflection)*50,player.sprite_x+cos(player.angle_of_reflection)*60,player.sprite_y+sin(player.angle_of_reflection)*60,YELLOW);
  //}
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
    GrPrint(hdc,16+4,help_y+16,"Press '*' for Controls Help",c4);
    GrPrint(hdc,16+5,help_y+17,"Press '*' for Controls Help",c);
  } else if (call_help_timer<5000) {
    GrPrint(hdc,16+4,help_y+16,"Press '*' for Controls Help",c4);
    GrPrint(hdc,16+5,help_y+17,"Press '*' for Controls Help",c);
  }
}


