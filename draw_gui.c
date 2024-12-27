

void DrawWaterShader(HDC hdc) 
{
  if (player.in_water) {
    //if (!IsInvertedBackground()) {
      GrGlassRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,BLUE,128);
    /*} else {
      GrGlassRect(hdc,0,0,GR_WIDTH,GR_HEIGHT,LTR2YELLOW,128);
    }*/
  }
}


#define SC_RAINDROP_NUM  20
struct screenraindrop
{
  int lifetime,olifetime;
  double oy,x,y,speed;
} sc_raindrop[SC_RAINDROP_NUM];


void InitScreenRainDrop()
{
  for (int i=0;i<SC_RAINDROP_NUM;i++) {
    sc_raindrop[i].x=RandNum(-20,GR_WIDTH+20,player.seed);
    sc_raindrop[i].oy=RandNum(-20,GR_HEIGHT+20,player.seed);
    sc_raindrop[i].y=sc_raindrop[i].oy;
    sc_raindrop[i].speed=RandNum(2,10,player.seed)*0.1;
    sc_raindrop[i].olifetime=RandNum(50,200,player.seed);
    sc_raindrop[i].lifetime=sc_raindrop[i].olifetime;
  }
}


void ScreenRainDropAct()
{
  for (int i=0;i<SC_RAINDROP_NUM;i++) {
    if (sc_raindrop[i].lifetime>0) {
      sc_raindrop[i].lifetime--;
      sc_raindrop[i].y+=sc_raindrop[i].speed;
      sc_raindrop[i].oy+=sc_raindrop[i].speed/3;
    } else {
      sc_raindrop[i].x=RandNum(-20,GR_WIDTH+20,player.seed);
      sc_raindrop[i].oy=RandNum(-20,GR_HEIGHT+20,player.seed);
      sc_raindrop[i].y=sc_raindrop[i].oy;
      sc_raindrop[i].speed=RandNum(2,10,player.seed)*0.1;
      sc_raindrop[i].olifetime=RandNum(50,200,player.seed);
      sc_raindrop[i].lifetime=sc_raindrop[i].olifetime;
    }
  } 
}



void DrawRainShader(HDC hdc)
{
  if (player.rain_wet_timer>0) {
    int c=BLACK;//Highlight(IsInvertedBackground(),BLACK,WHITE);
    BLENDFUNCTION blendFunction;
    blendFunction.BlendOp = AC_SRC_OVER;
    blendFunction.BlendFlags = 0;
    if (player.rain_wet_timer>60) {
      blendFunction.SourceConstantAlpha = 32;// Transparency level (0-255)
    } else {
      player.visible_rain_wet_timer=160;
      blendFunction.SourceConstantAlpha = 16;
    }
    blendFunction.AlphaFormat = 0;

    HDC hdcMem = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, GR_WIDTH, GR_HEIGHT);
    SelectObject(hdcMem, hBitmap);
    GrRect(hdcMem,0,0,GR_WIDTH,GR_HEIGHT,c);
    for (int i=0;i<SC_RAINDROP_NUM;i++) {
      GrLine(hdcMem,sc_raindrop[i].x,sc_raindrop[i].y,sc_raindrop[i].x,sc_raindrop[i].oy,LTGRAY); //trails of rain
      GrLine(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,sc_raindrop[i].x+1,sc_raindrop[i].oy,LTGRAY);

      GrRect(hdcMem,sc_raindrop[i].x-2,sc_raindrop[i].y-10,7,10,LTGRAY);
      GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,3,LTGRAY,LTGRAY);
      if (sc_raindrop[i].olifetime-sc_raindrop[i].lifetime<10)
        GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].oy,5,LTGRAY,LTGRAY);

    }  
    AlphaBlend(hdc, 0, 0, GR_WIDTH, GR_HEIGHT, hdcMem, 0, 0, GR_WIDTH, GR_HEIGHT, blendFunction);
  // Clean up
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
  }
}


void DrawRainShader2(HDC hdc)
{
  int c;
  if (custom_map_background_color_i>127 || map_background==1) {
    if (player.rain_wet_timer>60) {
      c=DKGRAY;
    } else {
      c=rgbPaint[119+16*2];
    }
  } else {
    if (player.rain_wet_timer>60) {
      c=LTGRAY;//Highlight(IsInvertedBackground(),LTGRAY,RGB(200,200,200));
    } else {
      c=RGB(125,125,125);//Highlight(IsInvertedBackground(),RGB(125,125,125),RGB(215,215,215));
    }
  }

  if (player.rain_wet_timer>0) {
    for (int i=0;i<SC_RAINDROP_NUM;i++) {
      GrLine(hdc,sc_raindrop[i].x,sc_raindrop[i].y,sc_raindrop[i].x,sc_raindrop[i].oy,c); //trails of rain
      GrLine(hdc,sc_raindrop[i].x+1,sc_raindrop[i].y,sc_raindrop[i].x+1,sc_raindrop[i].oy,c);

      GrCircle(hdc,sc_raindrop[i].x+1,sc_raindrop[i].y,3,c,-1);
      if (sc_raindrop[i].olifetime-sc_raindrop[i].lifetime<10)
        GrCircle(hdc,sc_raindrop[i].x+1,sc_raindrop[i].oy,5,c,-1);

    }  
  }
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


void DrawCrosses(HDC hdc,int x, int y)
{
  char C[1];
  sprintf(C,"%c",134);

  //x=GR_WIDTH-8*18
  //y=8*23
  GrPrintW(hdc,x,y+10,L"",C,WHITE,16,TRUE,FALSE);
  GrPrintW(hdc,x-8*2,y+12,L"",C,WHITE,16,TRUE,FALSE);
  GrPrintW(hdc,x+8*2,y+12,L"",C,WHITE,16,TRUE,FALSE);

  int x2=x+8-4;
  int y2=y+8*2;

  GrLine(hdc,x2,y2+10,x2-8*8,y2+12,WHITE);
  GrLine(hdc,x2,y2+10,x2+8*8,y2+12,WHITE);
}


void DrawPersianClock(HDC hdc)
{
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
  if (GR_WIDTH>=800) {
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
  } else {
    DrawSprite(hdc, mcalendar_x,mcalendar_y,&draw_moon_sprite,FALSE);
  }

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
  char l_hijri_row1_1[19];
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
  if (is_khmer) {
    sprintf(l_hijri_row1_1,"");
  } else {
    sprintf(l_hijri_row1_1,"(:: Lunar Hijri ::)");
  }

  if (lunar_day>=1 && lunar_day<=5) { //1, 2, 3, 4, 5
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1,"  )");
  } else if (lunar_day>=6 && lunar_day<=9) {// 6, 7, 8, 9
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1," |)");
  } else if (lunar_day>=10 && lunar_day<=12) {// 10, 11, 12,
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1,"<|)");
  } else if (lunar_day>=13 && lunar_day<=15) {//13, 14, 15
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1,"(O)");
  } else if (lunar_day>=16 && lunar_day<=18) {//16, 17, 18
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1,"(|>");
  } else if (lunar_day>=19 && lunar_day<=22) {//19, 20, 21, 22
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1,"(|");
  } else if (lunar_day>=23 && lunar_day<=26) {//23, 24, 25,26
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1,"(");
  } else {
    sprintf(l_hijri_row1,"%s %s",l_hijri_row1_1," @");
  }


  swprintf(l_hijri_row2,128,L":: %s //%d. %s(%d) .%d",
    lunar_days_txt[lunar_day_of_week],
    lunar_day,
    lunar_months_txt[lunar_month-1],
    lunar_month,
    lunar_year
  );

  if (main_menu_chosen==-1 || (GR_WIDTH>=800 && GR_HEIGHT>=600)) {
    GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24,mcalendar_y-64+4,time_row1,"",WHITE,16,FALSE,yes_unifont);

    if (is_khmer) {
      DrawBitmap(hdc,mcalendar_x-mcalendar_l*7-24,
                     mcalendar_y-36,
                     0,
                     0,
                     119,
                     27,
                    mm0_kh_hijri_mask[0],SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,mcalendar_x-mcalendar_l*7-24,
                     mcalendar_y-36,
                     0,
                     0,
                     119,
                     27,
                    mm0_kh_hijri[0],SRCPAINT,FALSE,FALSE);
      GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24+8*16,mcalendar_y-32+4,L"","*",WHITE,16,TRUE,yes_unifont);
    } else {
      GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24,mcalendar_y-32+4,L"",s_hijri_row1,WHITE,16,TRUE,yes_unifont);
    }

    GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24,mcalendar_y-16+4,s_hijri_row2,"",WHITE,16,FALSE,yes_unifont);

    if (is_khmer) {
      DrawBitmap(hdc,mcalendar_x-mcalendar_l*7-24,
                     mcalendar_y+8,
                     0,
                     0,
                     90,
                     27,
                    mm0_kh_hijri_mask[1],SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,mcalendar_x-mcalendar_l*7-24,
                     mcalendar_y+8,
                     0,
                     0,
                     90,
                     27,
                    mm0_kh_hijri[1],SRCPAINT,FALSE,FALSE);
      GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24+8*12,mcalendar_y+13,L"",l_hijri_row1,WHITE,16,TRUE,yes_unifont);
    } else {
      GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24,mcalendar_y+16,L"",l_hijri_row1,WHITE,16,TRUE,yes_unifont);
    }

    GrPrintW(hdc,mcalendar_x-mcalendar_l*7-24,mcalendar_y+32,l_hijri_row2,"",WHITE,16,FALSE,yes_unifont);
  }

  //x=GR_WIDTH-8*18
  //y=8*23
  //DrawCrosses(hdc,GR_WIDTH-8*18,8*23);

  /*char C[1];
  sprintf(C,"%c",134);
  GrPrintW(hdc,GR_WIDTH-8*18,8*23+10,L"",C,WHITE,16,TRUE,FALSE);
  GrPrintW(hdc,GR_WIDTH-8*18-8*2,8*23+12,L"",C,WHITE,16,TRUE,FALSE);
  GrPrintW(hdc,GR_WIDTH-8*18+8*2,8*23+12,L"",C,WHITE,16,TRUE,FALSE);
  

  GrLine(hdc,GR_WIDTH-8*17-4,8*25+10,GR_WIDTH-8*17-4-8*8,8*25+12,WHITE);
  GrLine(hdc,GR_WIDTH-8*17-4,8*25+10,GR_WIDTH-8*17-4+8*8,8*25+12,WHITE);)*/
}


void DrawTitle(HDC hdc)
{
  int title_x=GR_WIDTH/2-352/2+4;
  int title_y=-32;
  int title_x2=GR_WIDTH-352*3/5-24;
  int title_y2=-48;
  if (GR_HEIGHT>=600) {

  } else {
    title_y=-48;
  }
  if (main_menu_chosen==-1) {
      DrawBitmap(hdc,title_x,
                     title_y,
                     0,
                     0,
                     352,
                     256,
                    title_sprite_mask,SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,title_x,
                     title_y,
                     0,
                     0,
                     352,
                     256,
                    title_sprite,SRCPAINT,FALSE,FALSE);
  } else {
      DrawBitmap(hdc,title_x2,
                     title_y2,
                     0,
                     0,
                     352*3/5,
                     256*3/5,
                    title_small_sprite_mask,SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,title_x2,
                     title_y2,
                     0,
                     0,
                     352*2/3,
                     256*2/3,
                    title_small_sprite,SRCPAINT,FALSE,FALSE);

  }

}


void DrawMainMenu(HDC hdc)
{

  //draw bkgrnd
  //DrawBitmap(hdc,0,0,0,0,GR_WIDTH,GR_HEIGHT,map_background_sprite,SRCCOPY,FALSE,FALSE);

  DrawMovingAVI(hdc);
  //Draw Moon Phase
  //GrSprite(hdc, GR_WIDTH-128, 128, moon_sprite_cache,FALSE);
  if (GR_WIDTH>=800)
    DrawSprite(hdc, GR_WIDTH-128,128,&draw_moon_sprite,FALSE);


  DrawTitle(hdc);

  int help_y=GR_HEIGHT-128;
  if (!hide_taskbar) { //task bar is shown
    help_y-=8*4; //go up abit
  }

  DrawPersianClock(hdc);

  int main_menu_y=0;
  int main_menu_y2=0;
  if (hide_taskbar) {
    main_menu_y=15;
//    main_menu_y2=15;
  } else {
    main_menu_y=0;
//    main_menu_y2=35;
  }
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
      /*c=Highlight((select_main_menu==0),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*2,"Levels.",c);

      c=Highlight((select_main_menu==1),WHITE,LTGREEN);
      GrPrint(hdc,30,main_menu_y+10+16*3,"Options.",c);*/

      //GrLine(hdc,GR_WIDTH/2,0,GR_WIDTH/2,GR_HEIGHT,WHITE);
      if (is_khmer) {

        //Level
        DrawBitmap(hdc,GR_WIDTH/2-121/2,GR_HEIGHT/2-16*4-main_menu_y2-31/2-3,0,0,121,31,mm0_kh_mask[0],SRCAND,FALSE,FALSE);
        if (select_main_menu==0) {
          DrawBitmap(hdc,GR_WIDTH/2-121/2,GR_HEIGHT/2-16*4-main_menu_y2-31/2-3,0,0,121,31,mm0_kh_green[0],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,GR_WIDTH/2-121/2,GR_HEIGHT/2-16*4-main_menu_y2-31/2-3,0,0,121,31,mm0_kh_white[0],SRCPAINT,FALSE,FALSE);
        }
        
        //Options
        DrawBitmap(hdc,GR_WIDTH/2-114/2,GR_HEIGHT/2-16*2-main_menu_y2-36/2,0,0,145,36,mm0_kh_mask[1],SRCAND,FALSE,FALSE);
        if (select_main_menu==1) {
          DrawBitmap(hdc,GR_WIDTH/2-114/2,GR_HEIGHT/2-16*2-main_menu_y2-36/2,0,0,145,36,mm0_kh_green[1],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,GR_WIDTH/2-114/2,GR_HEIGHT/2-16*2-main_menu_y2-36/2,0,0,145,36,mm0_kh_white[1],SRCPAINT,FALSE,FALSE);
        }


        //Exit
        DrawBitmap(hdc,GR_WIDTH/2-82/2-2,GR_HEIGHT/2-main_menu_y2-36/2,0,0,145,36,mm0_kh_mask[2],SRCAND,FALSE,FALSE);
        if (select_main_menu==2) {
          DrawBitmap(hdc,GR_WIDTH/2-82/2-2,GR_HEIGHT/2-main_menu_y2-36/2,0,0,145,36,mm0_kh_green[2],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,GR_WIDTH/2-82/2-2,GR_HEIGHT/2-main_menu_y2-36/2,0,0,145,36,mm0_kh_white[2],SRCPAINT,FALSE,FALSE);
        }

        //Language
        c=Highlight((select_main_menu==3),WHITE,LTGREEN);
        GrPrint(hdc,GR_WIDTH/2-8*7/2-4,GR_HEIGHT/2-main_menu_y2+16,"ENGLISH",c);


      } else {
        c=Highlight((select_main_menu==0),WHITE,LTGREEN);
        GrPrint(hdc,GR_WIDTH/2-7*6/2-4,GR_HEIGHT/2-16*4-12-main_menu_y2,"LEVELS",c);

        c=Highlight((select_main_menu==1),WHITE,LTGREEN);
        GrPrint(hdc,GR_WIDTH/2-7*8/2-1,GR_HEIGHT/2-16*2-12-main_menu_y2,"OPTIONS",c);

        c=Highlight((select_main_menu==2),WHITE,LTGREEN);
        GrPrint(hdc,GR_WIDTH/2-8*4/2+2,GR_HEIGHT/2-12-main_menu_y2,"EXIT",c);


        DrawBitmap(hdc,GR_WIDTH/2-71/2,GR_HEIGHT/2-main_menu_y2+24-25/2,0,0,145,36,mm0_kh_mask[3],SRCAND,FALSE,FALSE);
        if (select_main_menu==3) {
          DrawBitmap(hdc,GR_WIDTH/2-71/2,GR_HEIGHT/2-main_menu_y2+24-25/2,0,0,145,36,mm0_kh_green[3],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,GR_WIDTH/2-71/2,GR_HEIGHT/2-main_menu_y2+24-25/2,0,0,145,36,mm0_kh_white[3],SRCPAINT,FALSE,FALSE);
        }
      }
      
      /*if (select_main_menu<2)
        GrPrint(hdc,20,main_menu_y+10+16*2+16*select_main_menu,"*",LTGREEN);*/

      //c=Highlight((select_main_menu==2),WHITE,LTGREEN);
      //GrPrint(hdc,30,main_menu_y+10+16*10,"[SHIFT]+'Q':  Exit.",c);
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
         GrPrint(hdc,20,lvls_y+16*i,"-",WHITE);
         if (lvl_i<level_num) {
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
      GrPrint(hdc,20,2+lvls_y+16*(level_chosen%max_lvl_rows),"*",LTGREEN);
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
      int c,soptions_y=16*4;
      if (hide_taskbar)
        soptions_y+=main_menu_y;

      int mm2y1=10+soptions_y-8;
      int mm2y2=10+soptions_y;
      int mm2y3;

      //Graphics
      c=Highlight((option_choose==0),WHITE,LTGREEN);
      if (is_khmer) {
        DrawBitmap(hdc,30,mm2y1,0,0,127,21,mm2_kh_mask[0],SRCAND,FALSE,FALSE);
        if (option_choose==0) {
          DrawBitmap(hdc,30,mm2y1,0,0,127,21,mm2_kh_green[0],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,30,mm2y1,0,0,127,21,mm2_kh_white[0],SRCPAINT,FALSE,FALSE);
        }
        mm2y3=mm2y1+4;
      } else {
        GrPrint(hdc,30,mm2y2,"Player Color:",c);
        mm2y3=mm2y2;
      }


      GrPrint(hdc,30+20*8,mm2y3,"[      ]",c);
      GrRect(hdc,30+8*21,mm2y3,16,16,WHITE);
      if (color_chooser.is_choosing_color && option_choose==0) {
        if (color_chooser.color_id_choosing<256 && color_chooser.color_id_choosing>-1)
          GrRect(hdc,30+8*21+2,mm2y3+2,12,12,rgbPaint[color_chooser.color_id_choosing]);
      } else {
        GrRect(hdc,30+8*21+2,mm2y3+2,12,12,rgbPaint[player_color]);
      }




      mm2y1=10+soptions_y+16-4;
      mm2y2=10+soptions_y+16;
      c=Highlight((option_choose==1),WHITE,LTGREEN);
      if (is_khmer) {
        DrawBitmap(hdc,30,mm2y1,0,0,115,27,mm2_kh_mask[1],SRCAND,FALSE,FALSE);
        if (option_choose==1) {
          DrawBitmap(hdc,30,mm2y1,0,0,115,27,mm2_kh_green[1],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,30,mm2y1,0,0,115,27,mm2_kh_white[1],SRCPAINT,FALSE,FALSE);
        }
        mm2y3=mm2y1+4;
      } else {
        GrPrint(hdc,30,mm2y2,"Iris:",c);
        mm2y3=mm2y2;
      }


      GrPrint(hdc,30+20*8,mm2y3,"[      ]",c);
      GrRect(hdc,30+8*21,mm2y3,16,16,WHITE);
      if (color_chooser.is_choosing_color && option_choose==1) {
        if (color_chooser.color_id_choosing<256 && color_chooser.color_id_choosing>-1)
          GrRect(hdc,30+8*21+2,mm2y3+2,12,12,rgbPaint[color_chooser.color_id_choosing]);
      } else {
        GrRect(hdc,30+8*21+2,mm2y3+2,12,12,rgbPaint[player_iris_color]);
      }



      mm2y1=10+soptions_y+16*2+2;
      mm2y2=10+soptions_y+16*2;
      c=Highlight((option_choose==2),WHITE,LTGREEN);
      if (is_khmer) {
        DrawBitmap(hdc,30,mm2y1,0,0,107,25,mm2_kh_mask[2],SRCAND,FALSE,FALSE);
        if (option_choose==2) {
          DrawBitmap(hdc,30,mm2y1,0,0,107,25,mm2_kh_green[2],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,30,mm2y1,0,0,107,25,mm2_kh_white[2],SRCPAINT,FALSE,FALSE);
        }
        mm2y3=mm2y1+2;
      } else {
        GrPrint(hdc,30,mm2y2,"Pupil:",c);
        mm2y3=mm2y2;
      }


      GrPrint(hdc,30+20*8,mm2y3,"[      ]",c);
      GrRect(hdc,30+8*21,mm2y3,16,16,WHITE);
      if (color_chooser.is_choosing_color && option_choose==2) {
        if (color_chooser.color_id_choosing<256 && color_chooser.color_id_choosing>-1)
          GrRect(hdc,30+8*21+2,mm2y3+2,12,12,rgbPaint[color_chooser.color_id_choosing]);
      } else {
        GrRect(hdc,30+8*21+2,mm2y3+2,12,12,rgbPaint[player_pupil_color]);
      }

      if (option_choose>=0 && option_choose<=2 && color_chooser.is_choosing_color) {
        DrawPaintSquare(hdc,30+220,mm2y2,color_chooser.color_id,color_chooser.color_id_choosing);
      }








      //Game Options
      mm2y1=10+soptions_y+16*3+4;
      mm2y2=10+soptions_y+16*3;
      c=Highlight((option_choose==3),WHITE,LTGREEN);
      if (is_khmer) {
        DrawBitmap(hdc,30,mm2y1,0,0,75,26,mm2_kh_mask[3],SRCAND,FALSE,FALSE);
        if (option_choose==3) {
          DrawBitmap(hdc,30,mm2y1,0,0,75,26,mm2_kh_green[3],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,30,mm2y1,0,0,75,26,mm2_kh_white[3],SRCPAINT,FALSE,FALSE);
        }
        mm2y3=mm2y1+2;
      } else {
        GrPrint(hdc,30,mm2y2,"Camera Shake:",c);
        mm2y3=mm2y2;
      }


      if (game_cam_shake) {
        GrPrint(hdc,30+20*8,mm2y3,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,mm2y3,"<OFF>",c);
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



      c=Highlight((option_choose==6),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*7,"Raw Wav Volume:",c);
      char print_wav_out_volume[7];
      sprintf(print_wav_out_volume,"<%1.0f%%>",wav_out_volume*100);
      GrPrint(hdc,30+20*8,10+soptions_y+16*7,print_wav_out_volume,c);



      //Graphics===============
      c=Highlight((option_choose==7),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*9,"Unifont:",c);
      if (yes_unifont) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*9,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*9,"<OFF>",c);
      }


      c=Highlight((option_choose==8),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*10,"Shadows:",c);
      if (game_shadow) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*10,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*10,"<OFF>",c);
      }




      //Misc============
      c=Highlight((option_choose==9),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*12,"Window Borders:",c);
      if (!hide_taskbar) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*12,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*12,"<OFF>",c);
      }



      c=Highlight((option_choose==10),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*13,"Resolution:",c);
      switch (resolution_choose) {
        case 0:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<640x480>",c);
          break;
        case 1:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<800x600>",c);
          break;
        case 2:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1024x768>",c);
          break;
        case 3:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1280x720>",c);
          break;
        case 4:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1280x800>",c);
          break;
        case 5:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1440x900>",c);
          break;
        case 6:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1280x1024>",c);
          break;
        case 7:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1680x1050>",c);
          break;
        case 8:
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<1920x1080> [MAX]",c);
          break;
      } 

      c=Highlight((option_choose==11),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*14,"Window Align Position:",c);
      GrPrint(hdc,30+20*8,10+soptions_y+16*14,"<Corner || Middle>",c);



      c=Highlight((option_choose==12),WHITE,LTGREEN);
      GrPrint(hdc,30,10+soptions_y+16*15,"Show FPS:",c);
      if (show_fps) {
        GrPrint(hdc,30+20*8,10+soptions_y+16*15,"<ON>",c);
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*15,"<OFF>",c);
      }



      //===========================

      int add_option_choose=0;
      if (option_choose>8) {
        add_option_choose=16*3;
      } else if (option_choose>6) {
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
void DrawUI(HDC hdc) 
{
  int c = WHITE;//Highlight(IsInvertedBackground(),WHITE,BLACK);
  int c4 = BLACK;//Highlight(IsInvertedBackground(),BLACK,WHITE);


  int help_y=GR_HEIGHT-128;
  if (!hide_taskbar) { //task bar is shown
    help_y-=8*4; //go up abit
  }

//======Print Game Score Text
  //GrRect(hdc,0,8+32,8*40,16*5,c4);
  int digit_num4=0;
  if (IsSpeedBreaking()) {
    digit_num4=32;
  }

  char gametimetxt[32];
  wchar_t wgametimetxt[32];
  double print_time_ms=(double)game_timer/1000;

  char gamebesttimetxt[32];
  wchar_t wgamebesttimetxt[32];
  double best_time=double_best_score;

  char enemykills[10];
  wchar_t wenemykills[10];
  int printenemykills=ENEMY_NUM-enemy_kills;

  int digit_num;
  int digit_num1;
  int digit_num2;
  int digit_num3=0;

  if (is_khmer) {
      int c_kh1=BROWN;//GREEN;
      int c_kh2=YELLOW;//LTGREEN;
    //Draw Best Score
      DrawBitmap(hdc,16+8,1+8+16+digit_num4,0,0,65,19,ga0_kh_mask[0],SRCAND,FALSE,FALSE);
      DrawBitmap(hdc,16+8,1+8+16+digit_num4,0,0,65,19,ga0_kh[0],SRCPAINT,FALSE,FALSE);


      swprintf(wgamebesttimetxt,32,L"%5.3f",best_time);
      GrPrintW(hdc,16+4,24+8+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+6,24+8+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+4,24+10+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+6,24+10+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+5,24+9+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh2,16,FALSE,yes_unifont);



    //Draw Current Time/Congrats
      if (!game_over) {
        DrawBitmap(hdc,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_kh_mask[1],SRCAND,FALSE,FALSE);
        DrawBitmap(hdc,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_kh[1],SRCPAINT,FALSE,FALSE);

        swprintf(wgametimetxt,32,L"%5.3f",print_time_ms);
        digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16-24;
        GrPrintW(hdc,digit_num-4,24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-6,24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-4,24+10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-6,24+10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-5,24+9+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh2,16,FALSE,yes_unifont);


      } else { //game is over
        if (game_timer<int_best_score) { //New Score :D
          DrawBitmap(hdc,GR_WIDTH-106-32,8+16+digit_num4,0,0,123,37,ga0_kh_mask[2],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,GR_WIDTH-106-32,8+16+digit_num4,0,0,123,37,ga0_kh[2],SRCPAINT,FALSE,FALSE);

          swprintf(wgametimetxt,32,L"%5.3f",print_time_ms);
          digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16-24;
          GrPrintW(hdc,digit_num-4,16+24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",PURPLE,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-6,16+24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",PURPLE,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-4,16+24+10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",PURPLE,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-6,16+24+10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",PURPLE,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-5,16+24+9+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",LTPURPLE,16,FALSE,yes_unifont);
        } else {
          DrawBitmap(hdc,GR_WIDTH-69-32,8+16+digit_num4,0,0,69,16,ga0_kh_mask[1],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,GR_WIDTH-69-32,8+16+digit_num4,0,0,69,16,ga0_kh[1],SRCPAINT,FALSE,FALSE);
          swprintf(wgametimetxt,32,L"%5.3f",print_time_ms);
          digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16-24;
          GrPrintW(hdc,digit_num-4,24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-6,24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-4,24+10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-6,24+10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-5,24+9+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh2,16,FALSE,yes_unifont);
        }
      }


    //Draw Enemy Num/game Over
      if (printenemykills>0) {
        digit_num1=GR_WIDTH/2-133/2+4;//-(12*8)/2+4+4;
        DrawBitmap(hdc,digit_num1,16+10+digit_num4-4,0,0,133,26,ga0_kh_mask[3],SRCAND,FALSE,FALSE);
        DrawBitmap(hdc,digit_num1,16+10+digit_num4-4,0,0,133,26,ga0_kh[3],SRCPAINT,FALSE,FALSE);

        //print enemy kills below
        swprintf(wenemykills,10,L"%d",printenemykills);
        digit_num2=GR_WIDTH/2-(wcslen(wenemykills)*12)/2+4;
        GrPrintW(hdc,digit_num2-4,16+8+24+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num2-6,16+8+24+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num2-4,16+10+24+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num2-6,16+10+24+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num2-5,16+9+24+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh2,16,FALSE,yes_unifont);
      } else { //GAME OVER
        digit_num3=-32;//16;
        if (frame_tick<FPS/2) {
          DrawBitmap(hdc,GR_WIDTH/2-52/2+4,16+10+32+digit_num3+digit_num4,0,0,52,22,ga0_kh_mask[4],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,GR_WIDTH/2-52/2+4,16+10+32+digit_num3+digit_num4,0,0,52,22,ga0_kh[4],SRCPAINT,FALSE,FALSE);
        }
      }

    




/*
      int c_kh1=GREEN;//Highlight(IsInvertedBackground(),GREEN,YELLOW);
      int c_kh2=LTGREEN;//Highlight(IsInvertedBackground(),LTGREEN,BROWN);
      if (!game_over) {
        swprintf(wgametimetxt,32,L"Time: %5.3f",print_time_ms);
        digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16;
        GrPrintW(hdc,digit_num-4,8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-6,8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-4,10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-6,10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num-5,9+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh2,16,FALSE,yes_unifont);
      } else { //game is over
        if (game_timer<int_best_score) { //New Score :D
          swprintf(wgametimetxt,32,L"New Best! :D %5.3f",print_time_ms);
          digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16;
          GrPrintW(hdc,digit_num+5,9+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",LTPURPLE,16,FALSE,yes_unifont);
        } else {
          swprintf(wgametimetxt,32,L"Time: %5.3f",print_time_ms);
          digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16;
          GrPrintW(hdc,digit_num-4,8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-6,8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-4,10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-6,10+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh1,16,FALSE,yes_unifont);
          GrPrintW(hdc,digit_num-5,9+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh2,16,FALSE,yes_unifont);
        }
      }

      swprintf(wgamebesttimetxt,32,L"Best: %5.3f",best_time);
      GrPrintW(hdc,16+4,8+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+6,8+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+4,10+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+6,10+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,16+5,9+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh2,16,FALSE,yes_unifont);

      if (printenemykills>0) {
        digit_num1=GR_WIDTH/2-(12*8)/2+4+4;
        GrPrintW(hdc,digit_num1-4,8+16+digit_num4,L"Enemies Left:","",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num1-6,8+16+digit_num4,L"Enemies Left:","",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num1-4,10+16+digit_num4,L"Enemies Left:","",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num1-6,10+16+digit_num4,L"Enemies Left:","",c_kh1,16,FALSE,yes_unifont);
        GrPrintW(hdc,digit_num1-5,9+16+digit_num4,L"Enemies Left:","",c_kh2,16,FALSE,yes_unifont);
        swprintf(wenemykills,10,L"%d",printenemykills);
      } else {
        digit_num3=-16;
        if (frame_tick<FPS/2)
          swprintf(wenemykills,10,L"GAME OVER");
        else
          swprintf(wenemykills,10,L" ");
      }

      digit_num2=GR_WIDTH/2-(wcslen(wenemykills)*8)/2+4;
      if (printenemykills<=0)
        digit_num2-=4;
      GrPrintW(hdc,digit_num2-4,8+32+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,digit_num2-6,8+32+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,digit_num2-4,10+32+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,digit_num2-6,10+32+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh1,16,FALSE,yes_unifont);
      GrPrintW(hdc,digit_num2-5,9+32+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh2,16,FALSE,yes_unifont);
*/
  } else { //not-khmer
      if (!game_over) {
        sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
        digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
        GrPrint(hdc,digit_num-4,8+16+digit_num4,gametimetxt,YELLOW);
        GrPrint(hdc,digit_num-6,8+16+digit_num4,gametimetxt,YELLOW);
        GrPrint(hdc,digit_num-4,10+16+digit_num4,gametimetxt,YELLOW);
        GrPrint(hdc,digit_num-6,10+16+digit_num4,gametimetxt,YELLOW);
        GrPrint(hdc,digit_num-5,9+16+digit_num4,gametimetxt,BROWN);
      } else { //game is over
        if (game_timer<int_best_score) { //New Score :D
          sprintf(gametimetxt,"New Best! :D %5.3f",print_time_ms);
          digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
          GrPrint(hdc,digit_num+5,9+16+digit_num4,gametimetxt,LTPURPLE);
        } else {
          sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
          digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
          GrPrint(hdc,digit_num-4,8+16+digit_num4,gametimetxt,YELLOW);
          GrPrint(hdc,digit_num-6,8+16+digit_num4,gametimetxt,YELLOW);
          GrPrint(hdc,digit_num-4,10+16+digit_num4,gametimetxt,YELLOW);
          GrPrint(hdc,digit_num-6,10+16+digit_num4,gametimetxt,YELLOW);
          GrPrint(hdc,digit_num-5,9+16+digit_num4,gametimetxt,BROWN);
        }
      }

      sprintf(gamebesttimetxt,"Best: %5.3f",best_time);
      GrPrint(hdc,16+4,8+16+digit_num4,gamebesttimetxt,YELLOW);
      GrPrint(hdc,16+6,8+16+digit_num4,gamebesttimetxt,YELLOW);
      GrPrint(hdc,16+4,10+16+digit_num4,gamebesttimetxt,YELLOW);
      GrPrint(hdc,16+6,10+16+digit_num4,gamebesttimetxt,YELLOW);
      GrPrint(hdc,16+5,9+16+digit_num4,gamebesttimetxt,BROWN);

      /*int printenemykills=ENEMY_NUM-enemy_kills;
      int printenemynum=ENEMY_NUM;
      sprintf(enemykills,"Enemies Left: %d/%d",printenemykills,printenemynum);
      int digit_num=GR_WIDTH/2-(strlen(enemykills)*8)/2-4;
      GrPrint(hdc,digit_num-4,8+16,enemykills,YELLOW);
      GrPrint(hdc,digit_num-6,8+16,enemykills,YELLOW);
      GrPrint(hdc,digit_num-4,10+16,enemykills,YELLOW);
      GrPrint(hdc,digit_num-6,10+16,enemykills,YELLOW);
      GrPrint(hdc,digit_num-5,9+16,enemykills,BROWN);*/

      digit_num3=0;
      if (printenemykills>0) {
        digit_num1=GR_WIDTH/2-(12*8)/2+4+4;
        GrPrint(hdc,digit_num1-4,8+16+digit_num4,"Enemies Left:",YELLOW);
        GrPrint(hdc,digit_num1-6,8+16+digit_num4,"Enemies Left:",YELLOW);
        GrPrint(hdc,digit_num1-4,10+16+digit_num4,"Enemies Left:",YELLOW);
        GrPrint(hdc,digit_num1-6,10+16+digit_num4,"Enemies Left:",YELLOW);
        GrPrint(hdc,digit_num1-5,9+16+digit_num4,"Enemies Left:",BROWN);
        sprintf(enemykills,"%d",printenemykills);
      } else {
        digit_num3=-16;
        if (frame_tick<FPS/2)
          sprintf(enemykills,"GAME OVER");
        else
          sprintf(enemykills," ");
      }

      digit_num2=GR_WIDTH/2-(strlen(enemykills)*8)/2+4;
      if (printenemykills<=0)
        digit_num2-=4;
      GrPrint(hdc,digit_num2-4,8+32+digit_num3+digit_num4,enemykills,YELLOW);
      GrPrint(hdc,digit_num2-6,8+32+digit_num3+digit_num4,enemykills,YELLOW);
      GrPrint(hdc,digit_num2-4,10+32+digit_num3+digit_num4,enemykills,YELLOW);
      GrPrint(hdc,digit_num2-6,10+32+digit_num3+digit_num4,enemykills,YELLOW);
      GrPrint(hdc,digit_num2-5,9+32+digit_num3+digit_num4,enemykills,BROWN);
  }




//========================

  if (call_help_timer<1000/*5000*/) {
    DrawPlayingMusic(hdc,16+4,help_y+48,c,c4);
  }

  if (show_fps) {
    char fpstxt[24];
    sprintf(fpstxt,"FPS: %d / %d",saved_showoff,FPS);
    int FPS_x=GR_WIDTH-strlen(fpstxt)*8-14;
    GrPrint(hdc,FPS_x,10+32,fpstxt,c);
  }

//=========Draw Player UI
  int i=0,j=0;
  int c2,c3,ca;

  c2 = LTRED;//Highlight(IsInvertedBackground(),LTRED,LTCYAN);
  ca = RED;//Highlight(IsInvertedBackground(),RED,CYAN);
  c3 = LTGRAY;//Highlight(IsInvertedBackground(),LTGRAY,DKGRAY);

  double percentage=player.health/DEFAULT_PLAYER_HEALTH;
  double health_length=64*percentage;

  double bpercentage=player.block_health/player.block_health_max;
  double bhealth_length=150*bpercentage;

  //Draw Block Health
  if (player.show_block_health_timer>0 || (player.health<=PLAYER_LOW_HEALTH+1)) {
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
      int print_health=player.health;
      if (is_khmer) {
        wchar_t txt[16];
        swprintf(txt,16,L"%d",print_health);
        int sprite_x_health=(int)player.sprite_x-wcslen(txt)*12/2;
        GrPrintW(hdc,sprite_x_health,player.sprite_y-48,ReplaceToKhmerNum(txt),"",c,16,FALSE,yes_unifont);
      } else {
        char txt[16];
        sprintf(txt,"%d",print_health);
        int sprite_x_health=(int)player.sprite_x-strlen(txt)*8/2;
        GrPrint(hdc,sprite_x_health,player.sprite_y-48,txt,c);
      }
    }
  }




  //debug: middle line
  //GrLine(hdc,GR_WIDTH/2,0,GR_WIDTH/2,GR_HEIGHT,BLACK);

  //===--- Draw player speed ---=== (Over Cursor)
  //hehehehe
  for (i=0;i<player.speed;i++) {
    double speed_angle=i*0.1;
    double speed_dist=64;
    double angle_limit=M_PI_4+M_PI_2;
    if (speed_angle>angle_limit) {
      int speed_times=speed_angle/angle_limit;
      speed_angle-=angle_limit*speed_times;
      speed_dist=64+8*speed_times;
      c2 = LTCYAN;//Highlight(IsInvertedBackground(),LTCYAN,RED);
    } else {
      //if (!IsInvertedBackground()) {
        if (i<5) c2=GREEN;
        else if (i<10) c2=LTGREEN;
        else c2=RED;
      /*} else {
        if (i<5) c2=DKGRAY;
        else if (i<10) c2=LTPURPLE;
        else c2=LTCYAN;
      }*/
    }
    GrCircle(hdc,
      //player.sprite_x-speed_dist*cos(speed_angle), //player centric
      //player.sprite_y-speed_dist*sin(speed_angle),
      mouse_x-speed_dist*cos(speed_angle),
      mouse_y-speed_dist*sin(speed_angle),
      3,c2,c2
    );
  }




  int c5=LTGREEN;//Highlight(IsInvertedBackground(),LTGREEN,LTPURPLE);
  int c10=LTRED;//Highlight(IsInvertedBackground(),LTRED,LTCYAN);


  int player_web_remaining=player.max_web_num-player.placed_web_num;

  //EXP Bar
  if (player.show_exp_timer>0) {
    GrRect(hdc,player.sprite_x-50,player.sprite_y+23,100,5,c4);
    for (int k=0;k<player.exp;k++) {
      GrRect(hdc,1+player.sprite_x-50+k*20,player.sprite_y+24,18,3,c5);
    }

    /*if (is_khmer) {
      wchar_t wbullettxt[4];
      swprintf(wbullettxt,4,L"%d",player_web_remaining);
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,ReplaceToKhmerNum(wbullettxt),"",c,16,FALSE,yes_unifont);
    } else {
      char bullettxt[4];
      sprintf(bullettxt,"%d",player_web_remaining);
      GrPrint(hdc,player.sprite_x+58,player.sprite_y+16,bullettxt,c);
    }*/
  }


  //Knives per throw Bar
  int knifethrowsx=mouse_x-32;
  int knifethrowsy=mouse_y+32;
  int knifethrowstxtx=mouse_x+32;
  int knifethrowstxty=mouse_y-48;
  GrRect(hdc,knifethrowsx,knifethrowsy+1,48*2,9,c4);
  GrRect(hdc,knifethrowsx,knifethrowsy,player.knives_per_throw*4,10,c10);
  c2 = LTGREEN;//Highlight(IsInvertedBackground(),LTGREEN,LTPURPLE);
  int ck=LTCYAN;//Highlight(IsInvertedBackground(),LTCYAN,LTRED);
  for (int k=0;k<PLAYER_BULLET_NUM-player.bullet_shot_num;k++) {
    //GrLine(hdc,player.sprite_x-48+k*4-2,player.sprite_y+32,player.sprite_x-48+k*4+2,player.sprite_y+36,c5);
    //GrLine(hdc,player.sprite_x-48+k*4+2,player.sprite_y+32,player.sprite_x-48+k*4-2,player.sprite_y+36,c5);
    //GrLine(hdc,player.sprite_x-48+k*4-2,player.sprite_y+32,player.sprite_x-24+k*4,player.sprite_y+36,c2);
    //GrCircle(hdc,player.sprite_x-48+k*4,player.sprite_y+34,2,c5,-1);
    int ckpt=Highlight((k<player.knives_per_throw),ck,c2);
    GrRect(hdc,knifethrowsx+k*4,knifethrowsy+2,4,6,ckpt);
  }

  //Print Bullets Remaining
  wchar_t bulletlefttxt[32]; //[200]/3
  wchar_t bulletlefttxt2[32]; //[200]/3
  int bullet_minus=0;
 /* switch (player.knives_per_throw) {
    case 1:
    case 3:
      {swprintf(bulletlefttxt,7,L"[%d] %d/0",player.player_web_remaining);}
      break;
    case 5:
      {swprintf(bulletlefttxt,7,L"[%d] %d/1",player_web_remaining);bullet_minus=1;}
      break;
    case 15:
      {swprintf(bulletlefttxt,7,L"[%d] %d/3",player_web_remaining);bullet_minus=3;}
      break;
  }*/
  if (player.knives_per_throw>3) {
    bullet_minus=player.knives_per_throw/5;
  }
  swprintf(bulletlefttxt,32,L"%d/%d",player.bullet_num,bullet_minus);
  swprintf(bulletlefttxt2,32,L"%d/%d [%d]",player.bullet_num,bullet_minus,player_web_remaining);
  //GrPrint(hdc,knifethrowsx-32-8,knifethrowsy-4,bulletlefttxt,c);
  int bc=Highlight(player.bullet_num<bullet_minus,c,LTRED);
  int bc2=Highlight(player_web_remaining==0,c,LTRED);
  if (is_khmer) {
    GrPrintW(hdc,knifethrowstxtx-32-30-8,knifethrowstxty-4,ReplaceToKhmerNum(bulletlefttxt2),"",bc2,16,FALSE,yes_unifont);
    GrPrintW(hdc,knifethrowstxtx-32-30-8,knifethrowstxty-4,ReplaceToKhmerNum(bulletlefttxt),"",bc,16,FALSE,yes_unifont);
  } else {
    GrPrintW(hdc,knifethrowstxtx-32-8,knifethrowstxty-4,bulletlefttxt2,"",bc2,16,FALSE,FALSE);
    GrPrintW(hdc,knifethrowstxtx-32-8,knifethrowstxty-4,bulletlefttxt,"",bc,16,FALSE,FALSE);
  }

  if (player.show_exp_timer>0) {
    if (is_khmer) {
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,ReplaceToKhmerNum(bulletlefttxt2),"",bc2,16,FALSE,yes_unifont);
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,ReplaceToKhmerNum(bulletlefttxt),"",bc,16,FALSE,yes_unifont);
    } else {
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,bulletlefttxt2,"",bc2,16,FALSE,FALSE);
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,bulletlefttxt,"",bc,16,FALSE,FALSE);
    }
  }

    //draw perfect block
    /*if (player.block_timer<=11) {
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24,4*player.block_timer,5,c4);
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24,4*-player.block_timer,5,c4);
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24+1,4*player.block_timer,3,c3);
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24+1,4*-player.block_timer,3,c3);
    } else if (player.block_timer<=23) {
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24,4*(23-player.block_timer),5,c4);
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24,4*-(23-player.block_timer),5,c4);
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24+1,4*(23-player.block_timer),3,c3);
      GrRect(hdc,knifethrowsx+32,knifethrowsy+24+1,4*-(23-player.block_timer),3,c3);
    }*/


  //char bulletlefttxt[3];
  //sprintf(bulletlefttxt,"%d",player.max_web_num-player.placed_web_num);
  //GrPrint(hdc,knifethrowsx-32,knifethrowsy-16,bulletlefttxt,c);



  //=== -- Draw Bullet Num -- ==
  //[%d] bullet num
  //[%d/%d],bullet num/bullet used per shot [ /1 /3]





  //===-- Draw Timebreaker Circle ---===
  c5 = //Highlight(IsInvertedBackground(),
        Highlight(player.time_breaker,PURPLE,LTPURPLE);//,
        //Highlight(player.time_breaker,LTRGREEN,LTGREEN)
  //);

  int c6 = LTPURPLE;//Highlight(IsInvertedBackground(),LTPURPLE,LTRLTGREEN);

  //draw player time breaker
  const int tb_circle_r=8;
  for (i=0;i<player.time_breaker_units;i++) {
    double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*i*2;
    /*GrCircle(hdc,
      player.sprite_x-32*cos(tb_angle),
      player.sprite_y-32*sin(tb_angle),
      2,c5,c5);*/ //player centric
    GrCircle(hdc,
      mouse_x-tb_circle_r*cos(tb_angle),
      mouse_y-tb_circle_r*sin(tb_angle),
      3,c5,c5);
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    int frame_t=frame_tick%20;
    for (i=0;i<3;i++) {
      double tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*(frame_t-i)*2;
      /*GrCircle(hdc,
        player.sprite_x-32*cos(tb_angle),
        player.sprite_y-32*sin(tb_angle),
        3,LTPURPLE,LTPURPLE);*/  //player centric
      GrCircle(hdc,
        mouse_x-tb_circle_r*cos(tb_angle),
        mouse_y-tb_circle_r*sin(tb_angle),
        7,c6,c6);
    }
  }


  //======= Draw controls are on ========
  if (player.uppercut)
    GrPrint(hdc,mouse_x-16,mouse_y+48,"E",LTGREEN);

  if (player.low_jump)
    GrPrint(hdc,mouse_x-32,mouse_y+48,"3",LTGREEN);

  if (player.block_timer>0) {
    if (player.block_timer<=23) {
      GrPrint(hdc,mouse_x+24,mouse_y+48,"{___}",WHITE);
    }
    GrPrint(hdc,mouse_x+36,mouse_y+48,"S",LTGREEN);
  }


  //debug
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
  } else if (call_help_timer<1000/*5000*/) {
    GrPrint(hdc,16+4,help_y+16,"Press '*' for Controls Help",c4);
    GrPrint(hdc,16+5,help_y+17,"Press '*' for Controls Help",c);
  }



}


#define MARBLE_NUM 256//256
struct marble
{
  int color_id;
  double x,y,angle;
} marble[MARBLE_NUM];

void InitMarbles(int max_marbles)
{
  for (int i=0;i<max_marbles;i++) {
    marble[i].x=GR_WIDTH/2;
    marble[i].y=GR_HEIGHT/2;
    marble[i].angle=(2*M_PI/(max_marbles+1)*i);
  }
}

void DrawLoading(HDC hDC,int max_marbles)
{
  //GrRect(hDC,0,0,GR_WIDTH+2,GR_HEIGHT+2,DKRDKGRAY);
  //printf("x:%5.4f\n",marble[0].x);
  int title_x0=GR_WIDTH/2-352/2+4;
  //int title_y=-32;
  if (title_x0>0) {
  for (int i=0;i<max_marbles;i++) {
    marble[i].x+=cos(marble[i].angle)*10;
    marble[i].y+=sin(marble[i].angle)*10;
    if (marble[i].x>GR_WIDTH-2 || marble[i].x<2 || marble[i].y<2 || marble[i].y>GR_HEIGHT-2) {
      if (marble[i].x>GR_WIDTH-2) {
        marble[i].x=3;
      } else if (marble[i].x<2) {
        marble[i].x=GR_WIDTH-3;
      }
      if (marble[i].y<2) {
        marble[i].y=GR_HEIGHT-3;
      } else if (marble[i].y>GR_HEIGHT-2) {
        marble[i].y=3;
      }
    }
    /*if (marble[i].x>GR_WIDTH-20 || marble[i].x<20) {
      marble[i].angle=GetBounceAngle(marble[i].angle,M_PI_2);
    } else if ( marble[i].y<20 || marble[i].y>GR_HEIGHT-20) {
      marble[i].angle=GetBounceAngle(marble[i].angle,0);
    }*/


    for (int j=0;j<max_marbles;j++) {
      if (j!=i) {
        if (GetDistance(marble[i].x,marble[i].y,marble[j].x,marble[j].y)<10) {
          marble[i].angle=GetBounceAngle(marble[i].angle,marble[j].angle);
        }
      }
    }
    GrCircle(hDC,marble[i].x,marble[i].y,10,rgbPaint[i],rgbPaint[i]);
  }
  }

  /*DrawBitmap(hDC,title_x0,
                 title_y,
                 0,
                 0,
                 352,
                 256,
                title_sprite_mask,SRCAND,FALSE,FALSE);
  //Draw platforms paint
  DrawBitmap(hDC,title_x0,
                 title_y,
                 0,
                 0,
                 352,
                 256,
                title_sprite,SRCPAINT,FALSE,FALSE);*/
    //}
    //DrawTitle(hDC);
    //GrCircle(hDC,mouse_x,mouse_y,(set_fragment%10)*2,LTPURPLE,-1);
    /*DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_body[0],FALSE); //left ete open
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_iris[0],FALSE);
    DrawSprite(hDC,mouse_x-2,mouse_y-2,&draw_player_cursor_pupil[0],FALSE);*/

  int extra_y=0;
  if (!hide_taskbar) {
    extra_y=32;
  }
  GrRect(hDC,32,GR_HEIGHT-48-extra_y,GR_WIDTH-64,8*3,WHITE);
  if (loading_denominator>0) {
    loading_percentage=loading_numerator/loading_denominator;
    //printf("Loading: %%%3.2f\r",loading_percentage);
    GrRect(hDC,34,GR_HEIGHT-46-extra_y,((double)(GR_WIDTH-34-34))*loading_percentage,6*3+1,GREEN);
  }
}


