

void DrawWaterShader(HDC hdc,HDC hdc2) 
{
  if (player.in_water) {
    GrGlassRect(hdc,hdc2,0,0,GR_WIDTH,GR_HEIGHT,BLUE,64 /*128*/);
  }
}




int YParabola(int x) {
  //0 = y=GR_HEIGHT/2
  //GR_WIDTH/2 = y=0 
  /*
  y=240
  x=0

  320 ---> half of GR_WIDTH
  Example:
  y=(x-320)^2/k

  if x==0, y=240 ---> half of GR_HEIGHT

  240 = (-320)^2/k

  k = (-320)^2/240
  */

  int k = (GR_WIDTH/2*GR_WIDTH/2)/(1+GR_HEIGHT)*2; //adjust denominator to widen parabola
  if (k<=0) k=1;
  int X = (x-GR_WIDTH/2);
  int y = (X*X)/k; 
  return y; 
}

void InitOneScreenRainDrop(int i)
{
  //sc_raindrop[i].once=FALSE;
  int dice=0;
  switch (map_weather) {
    case 1:
      sc_raindrop[i].x=RandNum(-20,GR_WIDTH+20,&misc_rng_i,-1);
      sc_raindrop[i].oy=RandNum(-20,GR_HEIGHT+20,&misc_rng_i,-1);
      sc_raindrop[i].y=sc_raindrop[i].oy;
      sc_raindrop[i].speed=1;//RandNum(7,10,&misc_rng_i,-1)*0.1;
      //sc_raindrop[i].olifetime=RandNum(50,200,&misc_rng_i,-1);
      int min,lim;
      //dice=RandNum(0,7,&misc_rng_i,-1);
      //if (dice==0) {
      /*if (GR_WIDTH>GR_HEIGHT) {
        min=GR_WIDTH/4;
        lim=GR_WIDTH/3;
      } else {
        min=GR_HEIGHT/4;
        lim=GR_HEIGHT/3;
      }*/

      //if (min<480) {
        //min=480;
      //}
      //min=20;
      //lim=200;
      /*if (lim<640) {
        lim=640;
      }*/
      min=180;
      lim=250;
      sc_raindrop[i].olifetime=RandNum(min,lim,&misc_rng_i,-1);
      sc_raindrop[i].lifetime=sc_raindrop[i].olifetime;
      break;
    case 2: {
      sc_raindrop[i].x=RandNum(-10,GR_WIDTH+10,&misc_rng_i,-1);
      dice=RandNum(0,1,&misc_rng_i,-1);
      if (dice==0) //top
        sc_raindrop[i].oy=8+RandNum(1,YParabola(sc_raindrop[i].x),&misc_rng_i,-1);
      else //down->top 
        sc_raindrop[i].oy=GR_HEIGHT-16-RandNum(1,YParabola(sc_raindrop[i].x),&misc_rng_i,-1);
      sc_raindrop[i].y=sc_raindrop[i].oy;
      sc_raindrop[i].speed=RandNum(1,3,&misc_rng_i,-1)*0.01;
      sc_raindrop[i].olifetime=RandNum(3000,3100,&misc_rng_i,-1);
      sc_raindrop[i].lifetime=sc_raindrop[i].olifetime;
      }
      break;
    case 3:
      sc_raindrop[i].x=RandNum(-10,GR_WIDTH+10,&misc_rng_i,-1);
      dice=RandNum(0,1,&misc_rng_i,-1);
      if (dice==0) //top
        sc_raindrop[i].oy=8+RandNum(1,YParabola(sc_raindrop[i].x),&misc_rng_i,-1);
      else //down->top 
        sc_raindrop[i].oy=GR_HEIGHT-16-RandNum(1,YParabola(sc_raindrop[i].x),&misc_rng_i,-1);
      sc_raindrop[i].y=sc_raindrop[i].oy;
      sc_raindrop[i].speed=RandNum(1,3,&misc_rng_i,-1)*0.01;
      //sc_raindrop[i].olifetime=RandNum(100,300,&misc_rng_i,-1);
      sc_raindrop[i].olifetime=RandNum(3000,3100,&misc_rng_i,-1);
      sc_raindrop[i].lifetime=sc_raindrop[i].olifetime;
      break;
  }
}


void InitScreenRainDrop()
{
  sc_raindrop_num_=
  sc_raindrop_id_1=0;
  sc_raindrop_id_2=0;
  for (int i=0;i<SC_RAINDROP_NUM;i++) {
    InitOneScreenRainDrop(i);
  }
}


void ScreenRainDropAct()
{
//  for (int i=0;i<SC_RAINDROP_NUM;i++) {
  int a,b,c;
  //a = start scraindrop_i
  //b = end scraindrop_i

  //a = 0
  //b = start scraindrop

  //a = end scraindrop
  //b = SC_RAINDROP_NUM
  if (sc_raindrop_id_1==sc_raindrop_id_2) {
    c=0;
  } else if (sc_raindrop_id_1<sc_raindrop_id_2) {
    a=sc_raindrop_id_1;
    b=sc_raindrop_id_2;
    c=1;
  } else {
    c=2;
  }

  for (int j=0;j<c;j++) {
    if (c==2) {
      switch (j) {
        case 0: 
          a=0;
          b=sc_raindrop_id_2;
          break;
        case 1:
          a=sc_raindrop_id_1;
          b=SC_RAINDROP_NUM;
          break;
      }
    }

    for (int i=a;i<b;i++) {
      if (sc_raindrop[i].lifetime>0) {
        sc_raindrop[i].lifetime--;
        sc_raindrop[i].y+=sc_raindrop[i].speed;
        if (map_weather!=1) {
          sc_raindrop[i].oy+=sc_raindrop[i].speed/3;
        }
      } else { //reset position
        //if (player.rain_wet_timer>0) {
        if (i==sc_raindrop_id_1) {
          if (sc_raindrop_num_>0) {    
            sc_raindrop_num_--;
            sc_raindrop_id_1++;
          }
          if (sc_raindrop_id_1>=SC_RAINDROP_NUM) {
            sc_raindrop_id_1=0;
          }
          //Set future position
          InitOneScreenRainDrop(i);
        }
      } 
    }
  }
}



/*void DrawRainShader(HDC hdc,HDC hdcMem)
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
}*/




void DrawRainShader3(HDC hdcMem)
{
  int a,b,c;
  //a = start scraindrop_i
  //b = end scraindrop_i

  //a = 0
  //b = start scraindrop

  //a = end scraindrop
  //b = SC_RAINDROP_NUM
  if (sc_raindrop_id_1==sc_raindrop_id_2) {
    c=0;
  } else if (sc_raindrop_id_1<sc_raindrop_id_2) {
    a=sc_raindrop_id_1;
    b=sc_raindrop_id_2;
    c=1;
  } else {
    c=2;
  }

  //debug screen raindrop
  //GrCircle(hdcMem,sc_raindrop_id_1*5,100,3,GREEN,GREEN);
  //GrCircle(hdcMem,sc_raindrop_id_2*5,104,3,RED,RED);
  for (int j=0;j<c;j++) {
    if (c==2) {
      switch (j) {
        case 0: 
          a=0;
          b=sc_raindrop_id_2;
          break;
        case 1:
          a=sc_raindrop_id_1;
          b=SC_RAINDROP_NUM;
          break;
      }
    }    
    for (int i=a;i<b;i++) {
      if (sc_raindrop[i].lifetime>0) {
       switch (map_weather) {
          case 1: { //rain: expanding fading-out circle
            for (int s=0;s<3;s++) {
              bool small=FALSE;
              int size=(int)(sc_raindrop[i].y-sc_raindrop[i].oy)-(15*s);
              if (size<0) {
                size=(int)(sc_raindrop[i].y-sc_raindrop[i].oy)-(3*s);
                small=TRUE;
              }
              if (sc_raindrop[i].olifetime>200) {
                size+=size;
              }
              if (size>0) {
                if (small || (s==2) || 
                    (s==1 && sc_raindrop[i].lifetime>(sc_raindrop[i].olifetime/2)) ||
                    (s==0 && sc_raindrop[i].lifetime>(sc_raindrop[i].olifetime/2+sc_raindrop[i].olifetime/3))) {
                  GrCircle(hdcMem,sc_raindrop[i].x,sc_raindrop[i].oy,size,DKGRAY,-1);
                }
              }
            }}
            break;
          //old system
          /*case 1:
            GrLine(hdcMem,sc_raindrop[i].x,sc_raindrop[i].y,sc_raindrop[i].x,sc_raindrop[i].oy,DKGRAY); //trails of rain
            GrLine(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,sc_raindrop[i].x+1,sc_raindrop[i].oy,DKGRAY);
            GrRect(hdcMem,sc_raindrop[i].x-2,sc_raindrop[i].y-10,7,10,DKGRAY);
            GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,3,DKGRAY,-1);
            if (sc_raindrop[i].olifetime-sc_raindrop[i].lifetime<15)
              GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].oy,5,DKGRAY,-1);
            break;*/
          case 2:
            if (sc_raindrop[i].olifetime-sc_raindrop[i].lifetime<15)
              //GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].oy,14,WHITE,WHITE);
              GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].oy,5,WHITE,WHITE);
            else
              //GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,10,WHITE,WHITE);
              GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,3,WHITE,WHITE);
            GrLine(hdcMem,sc_raindrop[i].x-10,sc_raindrop[i].y,sc_raindrop[i].x+10,sc_raindrop[i].y,WHITE);
            GrLine(hdcMem,sc_raindrop[i].x,sc_raindrop[i].y-10,sc_raindrop[i].x,sc_raindrop[i].y+10,WHITE);
            GrLine(hdcMem,sc_raindrop[i].x-7,sc_raindrop[i].y-7,sc_raindrop[i].x+7,sc_raindrop[i].y+7,WHITE);
            GrLine(hdcMem,sc_raindrop[i].x+7,sc_raindrop[i].y-7,sc_raindrop[i].x-7,sc_raindrop[i].y+7,WHITE);
            break;
          case 3:
            if (sc_raindrop[i].olifetime-sc_raindrop[i].lifetime<15)
              GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].oy,4,WHITE,WHITE);
            else
              GrCircle(hdcMem,sc_raindrop[i].x+1,sc_raindrop[i].y,2,WHITE,WHITE);
            break;
        }
      }
    }
  }  
}



void DrawMMExtraKeys (HDC hdc)
{
  //draw extra keys on cursor
  if (mouse_y>GR_HEIGHT-144) {
    char printtxt[128];
    sprintf(printtxt,"[^: %d] [U:%d] [Y:%d] [l_SHIFT:%d,%d/%d,%d]",
        buffer_length_arr[casbs_i],
        wav_mode,
        hide_mm,
        audioData[0].play_loop,
        audioData[0].record_loop,
        audioData[1].play_loop,
        audioData[1].record_loop);
    GrPrint(hdc,mouse_x+64,mouse_y,printtxt,WHITE);


    //sprintf(printtxt,"Mid:[-%2.1f / %2.1f=]dB",audioData[0].mid_gain_db,audioData[1].mid_gain_db);
    //GrPrint(hdc,mouse_x+64,mouse_y-48-16*3,printtxt,WHITE);

    //sprintf(printtxt,"Hi: [:%2.1f / %2.1f\"]dB [SPACE + |:%d / %d]",audioData[0].high_gain_db,audioData[1].high_gain_db,audioData[0].high_eq_on,audioData[1].high_eq_on);
    //GrPrint(hdc,mouse_x+64,mouse_y-48-16*3,printtxt,WHITE);

    //sprintf(printtxt,"Low:[;%2.1f / %2.1f']dB [SPACE + \\:%d / %d]",audioData[0].low_gain_db,audioData[1].low_gain_db,audioData[0].low_eq_on,audioData[1].low_eq_on);
    //GrPrint(hdc,mouse_x+64,mouse_y-48-16*2,printtxt,WHITE);


    sprintf(printtxt,"HPF: [{%1.0f / %1.0f}]Hz [|: %d / %d]    Hi: [:%2.1f / %2.1f\"]dB [SPACE + |:%d / %d]",audioData[0].HIGH_CUTOFF_FREQUENCY,audioData[1].HIGH_CUTOFF_FREQUENCY,
        audioData[0].hpf_on,audioData[1].hpf_on,
        audioData[0].high_gain_db,audioData[1].high_gain_db,audioData[0].high_eq_on,audioData[1].high_eq_on
    );
    GrPrint(hdc,mouse_x+64,mouse_y-48,printtxt,WHITE);

    sprintf(printtxt,"LPF: [[%1.0f / %1.0f]]Hz [\\: %d / %d]  Low: [;%2.1f / %2.1f']dB [SPACE + \\:%d / %d]",audioData[0].LOW_CUTOFF_FREQUENCY,audioData[1].LOW_CUTOFF_FREQUENCY,
        audioData[0].lpf_on,audioData[1].lpf_on,
        audioData[0].low_gain_db,audioData[1].low_gain_db,audioData[0].low_eq_on,audioData[1].low_eq_on
    );
    GrPrint(hdc,mouse_x+64,mouse_y-32,printtxt,WHITE);


    sprintf(printtxt,"[h_SHIFT] [j_SHIFT] [7/&] [8/*] [- =][_ +]");
    GrPrint(hdc,mouse_x+64,mouse_y-16,printtxt,WHITE);
  }
}


void DrawPlayingMusic(HDC hdc,int x,int y,int c, int c4,int z)
{
    char r_txt[5];
    if (z==gct) {
      sprintf(r_txt,"[R]");
    } else {
      sprintf(r_txt,"");
    }
   float nc_=0;
   if (!stop_playing_song[z]) {
     float _d=audioData[z].sps_o*2;
     if (_d!=0) {
       nc_=audioData[z].sps_offset/_d;
     }
   }
  if (!stop_playing_song[z]) {
    if (song_num>0) {
      wchar_t txt[32+256];
      if (time_song_end[z]==-1 && current_song_time[z]==-1) {
        call_help_timer=0;
        //print song name. songname cutoff at 72 chars, longer = scroll mode

        if (showoff%30<5 || showoff%30>25) {
          swprintf(txt,32+256,L"%c%d/%d%c: %ls  [.  ]",171,song_rand_num[z]+1,song_num,187,/*song_names[song_rand_num[z]]*/print_song_playing[z]);
        } else if ((showoff%30>4 && showoff%30<10) || showoff%30>19){
          swprintf(txt,32+256,L"%c%d/%d%c: %ls  [.. ]",171,song_rand_num[z]+1,song_num,187,/*song_names[song_rand_num[z]]*/print_song_playing[z]);
        } else {
          swprintf(txt,32+256,L"%c%d/%d%c: %ls  [...]",171,song_rand_num[z]+1,song_num,187,/*song_names[song_rand_num[z]]*/print_song_playing[z]);
        }

      } else {
        int songtimediff;
        if (current_song_time[z]!=-1)
          songtimediff=(int)((time_song_end[z]-current_song_time[z])/1000);
        else
          songtimediff=(int)((song_audio_duration[z])/1000);//(int)((songAudio->duration)/1000);
        int min=songtimediff/60;
        int seconds=songtimediff%60;
        swprintf(txt,32+256,L"%c%d/%d%c: %ls  [K%c%d:%02d%cI]",171,song_rand_num[z]+1,song_num,187,print_song_playing[z]/*song_names[song_rand_num[z]]*/,171,min,seconds,187);
      }
      //%c 187
      //wchar_t txt[32+256];
      //swprintf(txt,32+256,L"%c%d/%d%c: %ls  []",171,song_rand_num[z]+1,song_num,187,song_names[song_rand_num[z]]);
      GrPrintW(hdc,x,y,txt,"",c,14,FALSE,yes_unifont);
      GrPrintW(hdc,x+1,y+1,txt,"",c4,14,FALSE,yes_unifont);

      

      char txt2[128];
      //char txt2_1[2046];
      //char txt2_2[256];
      /*for (int j=0;j<128;j++) {
        sprintf(txt2_1,"%ls %d:%c ",txt2_1,'z'+j,'z'+j);
      }*/ //max 256
      //note %c 134 is a cross

      switch (song_mode[z]) {
        case 0:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /?] [/X :[N%cSHIFT%cM]: /%c] [vb_SHIFT:%2.2f,%2.2f] [,<%1.1f>.][P:%d] %s",171,187,177, 171,187,187,nc_,audioData[z].tempo,audioData[z].volume,song_pause[z],r_txt);
          break;
        case 1:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [/? :[N%cSHIFT%cM]: /%c] [vb_SHIFT:%2.2f,%2.2f] [,<%1.1f>.][P:%d] %s",171,187,177,187,171,187,171,nc_,audioData[z].tempo,audioData[z].volume,song_pause[z],r_txt);
          break;
        case 2:
          sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /%c] [/%c :[N%cSHIFT%cM]: /X] [vb_SHIFT:%2.2f,%2.2f] [,<%1.1f>.][P:%d] %s",171,187,177, 171,187,171,187,nc_,audioData[z].tempo,audioData[z].volume,song_pause[z],r_txt);
          break;
      }
      GrPrint(hdc,x,y+16,txt2,c);   
      GrPrint(hdc,x+1,y+1+16,txt2,c4);
    }
  } else {
    char txt2[128];
    //sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /X] [/%c :[N%cSHIFT%cM]: /?][^ %d] [VB_SHIFT:%2.2f,%2.2f] [<%1.1f>]%s",171,187,177,171,171,187,buffer_length_arr[casbs_i],audioData[z].tempo,nc_,audioData[z].volume,r_txt);
    sprintf(txt2,"[9%cSHIFT%c0]: %c] [M: /X] [/%c :[N%cSHIFT%cM]: /?] [vb_SHIFT:%2.2f,%2.2f] [,<%1.1f>.][P:%d] %s",171,187,177,171,171,187,nc_,audioData[z].tempo,audioData[z].volume,song_pause[z],r_txt);
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

//int funnyrun=0;
void DrawPersianClock(HDC hdc,HDC hdc2)
{



  //Moon Pos
  int mcalendar_l=70;//64;
  int mcalendartxt_l=64;
  int mcalendar_x=GR_WIDTH-mcalendar_l*2;
  int mcalendar_y=GR_HEIGHT-mcalendar_l*2-40+8;//56;
  if (!hide_taskbar) { //task bar is shown
    mcalendar_y-=8*3; //go up abit
  }
  float moon_angle=0; 
  //Space Clock
  //Draw blue marbel
  //if (GR_WIDTH>=800 && lunar_day<=26) {
    /*GrCircle(hdc,mcalendar_x,mcalendar_y,10,LTBLUE,LTBLUE);
    GrRect(hdc,mcalendar_x,mcalendar_y,3,4,LTGREEN);
    GrRect(hdc,mcalendar_x-3,mcalendar_y,9,4,LTGREEN);
    GrRect(hdc,mcalendar_x+4,mcalendar_y,5,2,LTGREEN);
    GrRect(hdc,mcalendar_x-4,mcalendar_y-4,9,9,LTGREEN);
    GrCircle(hdc,mcalendar_x-4,mcalendar_y,2,LTGREEN,LTGREEN);
    GrCircle(hdc,mcalendar_x+2,mcalendar_y+5,3,LTGREEN,LTGREEN);
    GrCircle(hdc,mcalendar_x-2,mcalendar_y-3,3,LTGREEN,LTGREEN);
    GrCircle(hdc,mcalendar_x-3,mcalendar_y-5,2,LTGREEN,LTGREEN);
    GrCircle(hdc,mcalendar_x-3,mcalendar_y+5,2,LTGREEN,LTGREEN);*/
  //} else {
//    DrawSprite(hdc, hdc2,mcalendar_x,mcalendar_y,&draw_moon_cartoon_sprite,FALSE);
  //}

  double drawoffset=-M_PI/2;
  //funnyrun+=2000*20;
  //int64_t timenow=int64_current_timestamp()+funnyrun;

  //PersiaSolarTime(timenow,&solar_sec,&solar_min,&solar_hour,&solar_day,&solar_month,&solar_year,&solar_day_of_week,&solar_angle_day,&solar_leap_year,&solar_last_year_is_leap);
  //PersiaLunarTime(timenow,&lunar_sec,&lunar_min,&lunar_hour,&lunar_day,&lunar_month,&lunar_year,&lunar_day_of_week,&moon_angle_shift,&lunar_leap_year);


  if (lunar_day<27) //0 to 26
    moon_angle=(-2*M_PI/27 * lunar_day ) - moon_angle_shift;
  else
    moon_angle=-moon_angle_shift;

  for (int i=0;i<27;i++) {
    float tmp_angle=-2*M_PI/27 * i - moon_angle_shift   +drawoffset;
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
  GrCircle(hdc,mcalendar_x + mcalendar_l*cos(moon_angle+drawoffset), mcalendar_y + mcalendar_l*sin(moon_angle+drawoffset),5,WHITE,WHITE);

  //Sun Pos
  GrCircle(hdc,mcalendar_x + mcalendar_l*cos(-solar_angle_day+drawoffset), mcalendar_y + mcalendar_l*sin(-solar_angle_day+drawoffset),5,YELLOW,YELLOW);



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

      swprintf(s_hijri_row2,128,L":: %ls //%d. %ls(%d) .%d",
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


  swprintf(l_hijri_row2,128,L":: %ls //%d. %ls(%d) .%d",
    lunar_days_txt[lunar_day_of_week],
    lunar_day,
    lunar_months_txt[lunar_month-1],
    lunar_month,
    lunar_year
  );

  if ((main_menu_chosen==-1 || (GR_WIDTH>=800 && GR_HEIGHT>=600)) && show_hijiri) {
    GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24,mcalendar_y-64+4,time_row1,"",WHITE,16,FALSE,yes_unifont);

    if (is_khmer) {
      DrawBitmap(hdc,hdc2,mcalendar_x-mcalendartxt_l*7-24,
                     mcalendar_y-36,
                     0,
                     0,
                     119,
                     27,
                    mm0_kh_hijri_mask[0],SRCAND,FALSE,FALSE);
      DrawBitmap(hdc,hdc2,mcalendar_x-mcalendartxt_l*7-24,
                     mcalendar_y-36,
                     0,
                     0,
                     119,
                     27,
                    mm0_kh_hijri[0],SRCPAINT,FALSE,FALSE);
      GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24+8*16,mcalendar_y-32+4,L"","*",WHITE,16,TRUE,yes_unifont);
    } else {
      GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24,mcalendar_y-32+4,L"",s_hijri_row1,WHITE,16,TRUE,yes_unifont);
    }

    GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24,mcalendar_y-16+4,s_hijri_row2,"",WHITE,16,FALSE,yes_unifont);

    if (is_khmer) {
      DrawBitmap(hdc,hdc2,mcalendar_x-mcalendartxt_l*7-24,
                     mcalendar_y+8,
                     0,
                     0,
                     90,
                     27,
                    mm0_kh_hijri_mask[1],SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,hdc2,mcalendar_x-mcalendartxt_l*7-24,
                     mcalendar_y+8,
                     0,
                     0,
                     90,
                     27,
                    mm0_kh_hijri[1],SRCPAINT,FALSE,FALSE);
      GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24+8*12,mcalendar_y+13,L"",l_hijri_row1,WHITE,16,TRUE,yes_unifont);
    } else {
      GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24,mcalendar_y+16,L"",l_hijri_row1,WHITE,16,TRUE,yes_unifont);
    }

    GrPrintW(hdc,mcalendar_x-mcalendartxt_l*7-24,mcalendar_y+32,l_hijri_row2,"",WHITE,16,FALSE,yes_unifont);
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


  //SunRise Map
  /*int _k=50;
  sun_ctx_t tmp_sun_riseset;

  char printme[10];
  sprintf(printme,"%5.4f",solar_angle_day);
  GrPrint(hdc,200,200,printme,GREEN);
  for (int k=90;k>-91;k--) {
    tmp_sun_riseset.in_latitude  = k; 
    tmp_sun_riseset.in_longitude = 0;
    //double utc_offset=0;

    _k+=2;
    sun_ret_t ret = sun_compute(&tmp_sun_riseset,solar_angle_day,solar_leap_year,solar_last_year_is_leap);
    double prise=tmp_sun_riseset.out_sunrise_mins/60;
    double pset=tmp_sun_riseset.out_sunset_mins/60;
    double sunlight=pset-prise;

    if (k>0) {
      GrCircle(hdc,GR_WIDTH/2-(prise/2*50),50+(180*2+50)-_k,5,YELLOW,YELLOW);
      GrCircle(hdc,GR_WIDTH/2+(prise/2*50),50+(180*2+50)-_k,5,YELLOW,YELLOW);
    } else {
      GrCircle(hdc,GR_WIDTH/2-(prise/2*50),50+(180*2+50)-_k,5,LTCYAN,LTCYAN);
      GrCircle(hdc,GR_WIDTH/2+(prise/2*50),50+(180*2+50)-_k,5,LTCYAN,LTCYAN);
    }
  }*/
}


void DrawTitle(HDC hdc,HDC hdc2)
{
  int title_x=GR_WIDTH/2-352/2+4;
  int title_y=-32;
  int title_x2=GR_WIDTH-352*3/5-24;
  int title_y2=-24;
  int ti=0;
  if (!is_khmer) {
    ti=1;
  }
  if (GR_HEIGHT>=600) {

  } else {
    title_y=-48;
  }
  if (main_menu_chosen==-1) {
      DrawBitmap(hdc,hdc2,title_x,
                     title_y,
                     0,
                     0,
                     352,
                     256,
                    title_sprite_mask[ti],SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,hdc2,title_x,
                     title_y,
                     0,
                     0,
                     352,
                     256,
                    title_sprite[ti],SRCPAINT,FALSE,FALSE);

        //SelectObject(hdc2,title_sprite_mask[ti]);
        //BitBlt(hdc,title_x, title_y, 352, 256, hdc2, 0, 0, SRCAND);

        //SelectObject(hdc2,title_sprite[ti]);
        //BitBlt(hdc, title_x, title_y, 352, 256, hdc2, 0, 0, SRCPAINT);


  } else {
      DrawBitmap(hdc,hdc2,title_x2,
                     title_y2,
                     0,
                     0,
                     352*3/5,
                     256*3/5,
                    title_small_sprite_mask[ti],SRCAND,FALSE,FALSE);
      //Draw platforms paint
      DrawBitmap(hdc,hdc2,title_x2,
                     title_y2,
                     0,
                     0,
                     352*2/3,
                     256*2/3,
                    title_small_sprite[ti],SRCPAINT,FALSE,FALSE);

  }

}





void DrawMusicWav(HDC hdc)
{
  float c_x1,c_x2,c_y1,c_y2,prev_x1,prev_x2,prev_y1,prev_y2;
  int djbgcolor=player_color;
  /*djbgcolor=player_color+(audioData[gct].played_units/4)*16;  
  if (djbgcolor>255) {
    djbgcolor-=256;
  }*/
  //if (player_color>127) {
    /*djbgcolor=player_color+(audioData[gct].played_units)*16;  
    if (djbgcolor<0) {
      djbgcolor=0;
    }*/
  /*} else {
    djbgcolor=player_color+(audioData[gct].played_units)*16;  
    if (djbgcolor>254) {
      djbgcolor=254;
    }
  }*/
    if (audioData[gct].played_units>0 && audioData[gct].played_units<3) {
      djbgcolor=player_color+(3-(audioData[gct].played_units))*16;
      if (djbgcolor>255) {
        djbgcolor-=256;
      }
    }


  GrRect(hdc,0,0,GR_WIDTH+1,GR_HEIGHT+1,rgbPaint[djbgcolor]);

  int z_m=2;
  float z_m_height=0.006; //taller when 1 playing, shorter when 2 playing
  if (stop_playing_song[1]) { //wav 2 not playing
    z_m=1;    
  }
  if (wav_mode<=2) {
    if (z_m==2) {
      z_m_height=0.002;
    }
    for (int z=0;z<z_m;z++) {
      int _y=GR_HEIGHT/4;
      if (z_m==1) {
        _y=GR_HEIGHT/2;
      } else {
        if (z==1) {
          _y=GR_HEIGHT/4*3;
        }
      }
      c_x1=0;
      c_x2=0;
      c_y1=_y+audioData[z].buffer1[z]*z_m_height;//*audioData[z].volume*0.006;
      c_y2=_y+audioData[z].buffer2[z]*z_m_height;//*audioData[z].volume*0.006;

      prev_x1=c_x1;
      prev_y1=c_y1;
      prev_x2=c_x2;
      prev_y2=c_y2;

      if (!stop_playing_song[z]) {
        for (int i=0;i<chosen_buffer_length_o;i++) {
            //if (audioData[z].buffer1[i]>20000) {
              c_x1=((float)i/chosen_buffer_length_o)*GR_WIDTH;
              c_y1=_y+audioData[z].buffer1[i]*z_m_height;//*audioData[z].volume*0.006;
            //}
            //if (audioData[z].buffer2[i]>20000)
              c_x2=((float)i/chosen_buffer_length_o)*GR_WIDTH;
              c_y2=_y+audioData[z].buffer2[i]*z_m_height;//*audioData[z].volume*0.006;
            //}

            if (wav_mode==1) {
                if (audioData[z].double_buffer) {
                  GrLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_iris_color]);
                } else {
                  GrLine(hdc,prev_x2,prev_y2,c_x2,c_y2,rgbPaint[player_pupil_color]);
                }
            } else {
              GrLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_iris_color]);
              GrLine(hdc,prev_x2,prev_y2,c_x2,c_y2,rgbPaint[player_pupil_color]);
            }
            prev_x1=c_x1;
            prev_y1=c_y1;
            prev_x2=c_x2;
            prev_y2=c_y2;
        }
      }
    }
  } else { //dj-ing 2 tracks
    for (int z=0;z<2;z++) {
      int _y=GR_HEIGHT/4;
      if (z_m==1) {
        _y=GR_HEIGHT/2;
      } else {
        if (z==1) {
          _y=GR_HEIGHT/4*3;
        }
      }
      if (!stop_playing_song[z]) {
        int audio_index=0;
        if (z==0) {
          GrLine(hdc,0,_y,GR_WIDTH,_y,rgbPaint[player_iris_color]);
        } else {
          GrLine(hdc,0,_y,GR_WIDTH,_y,rgbPaint[player_pupil_color]);
        }           
        for (int i=0;i<READ_BUFFER_NUM;i++) {
          audio_index=(i+audioData[z].queue_read_buffer)%READ_BUFFER_NUM;
          c_x1=(GR_WIDTH/READ_BUFFER_NUM)*i;
          c_y1=_y+audioData[z].read_buffer[audio_index][z]*0.0012*(1+GR_HEIGHT/480);//*audioData[z].volume*0.006;
          prev_x1=c_x1;
          prev_y1=c_y1;
          
          //draw loop line
          /*if (audioData[z]->play_loop) {
            GrLine(hdc,audioBufferRect[z].x1,0,audioBufferRect[z].x1,GR_HEGIHT,LTBLUE);
            GrLine(hdc,audioBufferRect[z].x2,0,audioBufferRect[z].x2,GR_HEGIHT,YELLOW);
          }*/
          int beat_c=0;
          int beat_max=12;
          bool beat_line=FALSE;
          int16_t amp[AUDIO_STREAM_BUFFER_SIZE0];

          int cy2=_y+(audioData[z].max_amp2-4000)*0.0012*(1+GR_HEIGHT/480);
          GrLine(hdc,0,cy2,GR_WIDTH,cy2,rgbPaint[player_pupil_color]);
          cy2=_y-(audioData[z].max_amp2-4000)*0.0012*(1+GR_HEIGHT/480);
          GrLine(hdc,0,cy2,GR_WIDTH,cy2,rgbPaint[player_pupil_color]);

          for (int j=0;j<audioData[z].read_size;j+=128) {
              c_x1=((GR_WIDTH/READ_BUFFER_NUM)*i) + ((float)j/audioData[z].read_size)*(GR_WIDTH/5);
              c_y1=_y+audioData[z].read_buffer[audio_index][j]*0.0012*(1+GR_HEIGHT/480);//audioData[z].volume*0.006;
            if (!beat_line) {
              memcpy(amp,audioData[z].read_buffer[audio_index],audioData[z].read_size);
              //lowPassFilter(amp,audioData[z].read_size);
              if (abs(amp[j])>=audioData[z].max_amp2-4000 && abs(amp[j])>6000)
                beat_line=TRUE;
            } else {
              if (abs(amp[j])>=audioData[z].max_amp2-4000) {
                beat_c=0;
              } else {
                beat_c++;
                if (beat_c>=beat_max-1) {
                  beat_c=0;
                  beat_line=FALSE;
                }
              }
            }

            if (z==0) {
              if (!beat_line) {
                GrLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_iris_color]);
              } else {
                if (prev_x1>GR_WIDTH/2) {
                  GrLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_pupil_color]);
                } else {
                  GrDottedLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_pupil_color]);
                }
              }

            } else {
              if (!beat_line) {
                GrLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_pupil_color]);
              } else {
                if (prev_x1>GR_WIDTH/2) {
                  GrLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_iris_color]);
                } else {
                  GrDottedLine(hdc,prev_x1,prev_y1,c_x1,c_y1,rgbPaint[player_iris_color]);
                }
              }
            }
            prev_x1=c_x1;
            prev_y1=c_y1;
          }   
        }
      }

      if (z_m==1) {
         GrLine(hdc,(GR_WIDTH/2),0,(GR_WIDTH/2),GR_HEIGHT,rgbPaint[player_pupil_color]);
      } else {
        if (z==0) { //spindle
          GrLine(hdc,(GR_WIDTH/2),GR_HEIGHT/2,(GR_WIDTH/2),0,rgbPaint[player_pupil_color]);
        } else {
          GrLine(hdc,(GR_WIDTH/2),GR_HEIGHT/2,(GR_WIDTH/2),GR_HEIGHT,rgbPaint[player_iris_color]);
        }
      }
    }
  }
}

void DrawKhBool(HDC hdc,HDC hdc2,int x,int y,bool t,bool sel)
{
  if (t) { //on
    DrawBitmap(hdc,hdc2,x,y,0,0,34,14,kh_bool_mask[1],SRCAND,FALSE,FALSE);
    if (sel) {
      DrawBitmap(hdc,hdc2,x,y,0,0,34,14,kh_bool_green[1],SRCPAINT,FALSE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,x,y,0,0,34,14,kh_bool_white[1],SRCPAINT,FALSE,FALSE);
    }
  } else { //off
    DrawBitmap(hdc,hdc2,x,y+1,0,0,30,13,kh_bool_mask[0],SRCAND,FALSE,FALSE);
    if (sel) {
      DrawBitmap(hdc,hdc2,x,y+1,0,0,30,13,kh_bool_green[0],SRCPAINT,FALSE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,x,y+1,0,0,30,13,kh_bool_white[0],SRCPAINT,FALSE,FALSE);
    }
  }
}


void DrawKhDiffic(HDC hdc,HDC hdc2,int x,int y,bool t,bool sel)
{
  if (t) { //on
    DrawBitmap(hdc,hdc2,x,y,0,0,43,14,kh_difficulty_mask[1],SRCAND,FALSE,FALSE);
    if (sel) {
      DrawBitmap(hdc,hdc2,x,y,0,0,43,14,kh_difficulty_green[1],SRCPAINT,FALSE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,x,y,0,0,43,14,kh_difficulty_white[1],SRCPAINT,FALSE,FALSE);
    }
  } else { //off
    DrawBitmap(hdc,hdc2,x,y+3,0,0,43,18,kh_difficulty_mask[0],SRCAND,FALSE,FALSE);
    if (sel) {
      DrawBitmap(hdc,hdc2,x,y+3,0,0,43,18,kh_difficulty_green[0],SRCPAINT,FALSE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,x,y+3,0,0,43,18,kh_difficulty_white[0],SRCPAINT,FALSE,FALSE);
    }
  }
}



void DrawKhCornMid(HDC hdc,HDC hdc2,int x,int y,bool sel)
{
    DrawBitmap(hdc,hdc2,x,y,0,0,94,19,kh_cornmid_mask,SRCAND,FALSE,FALSE);
    if (sel) {
      DrawBitmap(hdc,hdc2,x,y,0,0,94,19,kh_cornmid_green,SRCPAINT,FALSE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,x,y,0,0,94,19,kh_cornmid_white,SRCPAINT,FALSE,FALSE);
    }
}


void DrawMM2Kh(HDC hdc,HDC hdc2,int x,int y,int l,int w,int t)
{
    DrawBitmap(hdc,hdc2,x,y,0,0,l,w,mm2_kh_mask[t],SRCAND,FALSE,FALSE);
    if (option_choose==t) {
      DrawBitmap(hdc,hdc2,x,y,0,0,l,w,mm2_kh_green[t],SRCPAINT,FALSE,FALSE);
    } else {
      DrawBitmap(hdc,hdc2,x,y,0,0,l,w,mm2_kh_white[t],SRCPAINT,FALSE,FALSE);
    }
}




void DrawMainMenu(HDC hdc,HDC hdc2)
{
  if (wav_mode==0) {
    if (level_loaded) {
      DrawBackground(hdc,hdc2);
      if (!hide_mm) {
        DrawPersianClock(hdc,hdc2);
      }
      if (!blank_level) {
        DrawPlatforms(hdc,hdc2);
        DrawSunRays(hdc,hdc2);
        DrawFirePlatforms(hdc);
        DrawWebs(hdc);
        DrawEnemy(hdc,hdc2);
        DrawPlayer(hdc,hdc2,player.type);
        //DrawWaterPlatforms(hdc,hdc2);

        //if (is_shadows && game_shadow && SHADOW_GRID_NUM>0) {
          //DrawShadows(hdc,hdc2);
        //}
        if (map_weather>0) {
          DrawRain(hdc,hdc2);
          DrawRainShader3(hdc);
        }
      }
    }
  } else {
    DrawMusicWav(hdc);
  }


  //debugging stuff with loading bitmaps
  /*SelectObject(hdc2,LoadPlayerSprite.sprite_2);
  BitBlt(hdc,0,64,32,32,hdc2,0,0,SRCCOPY);*/

  /*SelectObject(hdc2,PlayerSpriteCanvas[0].canvas);
  BitBlt(hdc,0,0,64,64,hdc2,0,0,SRCCOPY);

  SelectObject(hdc2,PlayerSprite[0].sprite_1[0].sprite_paint);
  BitBlt(hdc,0,64,32,32,hdc2,0,0,SRCCOPY);

  SelectObject(hdc2,LoadPlayerSprite.sprite_1);
  BitBlt(hdc,0,128,32,32,hdc2,0,0,SRCCOPY);*/

  int main_menu_y=0;
  int main_menu_y2=0;
  int help_y=GR_HEIGHT-128;
  if (!hide_taskbar) { //task bar is shown
    help_y-=8*4; //go up abit
  }


  if (!hide_mm) {
    DrawTitle(hdc,hdc2);
    if (hide_taskbar) {
      main_menu_y=15;
    } else {
      main_menu_y=0;
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
        DrawBitmap(hdc,hdc2,GR_WIDTH/2-121/2,GR_HEIGHT/2-16*4-main_menu_y2-31/2-3,0,0,121,31,mm0_kh_mask[0],SRCAND,FALSE,FALSE);
        if (select_main_menu==0) {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-121/2,GR_HEIGHT/2-16*4-main_menu_y2-31/2-3,0,0,121,31,mm0_kh_green[0],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-121/2,GR_HEIGHT/2-16*4-main_menu_y2-31/2-3,0,0,121,31,mm0_kh_white[0],SRCPAINT,FALSE,FALSE);
        }
        
        //Options
        DrawBitmap(hdc,hdc2,GR_WIDTH/2-114/2,GR_HEIGHT/2-16*2-main_menu_y2-36/2,0,0,145,36,mm0_kh_mask[1],SRCAND,FALSE,FALSE);
        if (select_main_menu==1) {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-114/2,GR_HEIGHT/2-16*2-main_menu_y2-36/2,0,0,145,36,mm0_kh_green[1],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-114/2,GR_HEIGHT/2-16*2-main_menu_y2-36/2,0,0,145,36,mm0_kh_white[1],SRCPAINT,FALSE,FALSE);
        }


        //Exit
        DrawBitmap(hdc,hdc2,GR_WIDTH/2-82/2-2,GR_HEIGHT/2-main_menu_y2-36/2,0,0,145,36,mm0_kh_mask[2],SRCAND,FALSE,FALSE);
        if (select_main_menu==2) {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-82/2-2,GR_HEIGHT/2-main_menu_y2-36/2,0,0,145,36,mm0_kh_green[2],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-82/2-2,GR_HEIGHT/2-main_menu_y2-36/2,0,0,145,36,mm0_kh_white[2],SRCPAINT,FALSE,FALSE);
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


        DrawBitmap(hdc,hdc2,GR_WIDTH/2-71/2,GR_HEIGHT/2-main_menu_y2+24-25/2,0,0,145,36,mm0_kh_mask[3],SRCAND,FALSE,FALSE);
        if (select_main_menu==3) {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-71/2,GR_HEIGHT/2-main_menu_y2+24-25/2,0,0,145,36,mm0_kh_green[3],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,GR_WIDTH/2-71/2,GR_HEIGHT/2-main_menu_y2+24-25/2,0,0,145,36,mm0_kh_white[3],SRCPAINT,FALSE,FALSE);
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
      float max_page=ceil(((float)(level_num)/max_lvl_rows));
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
      if (is_khmer) {
        DrawBitmap(hdc,hdc2,30,main_menu_y+10+16*2,0,0,145,36,mm0_kh_mask[1],SRCAND,FALSE,FALSE);
        DrawBitmap(hdc,hdc2,30,main_menu_y+10+16*2,0,0,145,36,mm0_kh_white[1],SRCPAINT,FALSE,FALSE);
      } else {
        GrPrint(hdc,30,main_menu_y+10+16*2,"OPTIONS",WHITE); 
      }
      int c,soptions_y=16*4;
      if (hide_taskbar)
        soptions_y+=main_menu_y;

      int mm2y1=10+soptions_y-2;
      int mm2y2=10+soptions_y;


      //Graphics
      c=Highlight((option_choose==0),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,mm2y1,87,14,0);
      } else {
        GrPrint(hdc,30,mm2y2,"Player Color:",c);
      }


      GrPrint(hdc,30+20*8,mm2y2,"[      ]",c);
      GrRect(hdc,30+8*21,mm2y2,16,16,WHITE);
      if (color_chooser.is_choosing_color && option_choose==0) {
        if (color_chooser.color_id_choosing<256 && color_chooser.color_id_choosing>-1)
          GrRect(hdc,30+8*21+2,mm2y2+2,12,12,rgbPaint[color_chooser.color_id_choosing]);
      } else {
        GrRect(hdc,30+8*21+2,mm2y2+2,12,12,rgbPaint[player_color]);
      }




      mm2y1=10+soptions_y+16-2;
      mm2y2=10+soptions_y+16;
      c=Highlight((option_choose==1),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,mm2y1,79,18,1);
      } else {
        GrPrint(hdc,30,mm2y2,"Iris:",c);
      }


      GrPrint(hdc,30+20*8,mm2y2,"[      ]",c);
      GrRect(hdc,30+8*21,mm2y2,16,16,WHITE);
      if (color_chooser.is_choosing_color && option_choose==1) {
        if (color_chooser.color_id_choosing<256 && color_chooser.color_id_choosing>-1)
          GrRect(hdc,30+8*21+2,mm2y2+2,12,12,rgbPaint[color_chooser.color_id_choosing]);
      } else {
        GrRect(hdc,30+8*21+2,mm2y2+2,12,12,rgbPaint[player_iris_color]);
      }



      mm2y1=10+soptions_y+16*2-2;
      mm2y2=10+soptions_y+16*2;
      c=Highlight((option_choose==2),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,mm2y1,74,18,2);
      } else {
        GrPrint(hdc,30,mm2y2,"Pupil:",c);
      }


      GrPrint(hdc,30+20*8,mm2y2,"[      ]",c);
      GrRect(hdc,30+8*21,mm2y2,16,16,WHITE);
      if (color_chooser.is_choosing_color && option_choose==2) {
        if (color_chooser.color_id_choosing<256 && color_chooser.color_id_choosing>-1)
          GrRect(hdc,30+8*21+2,mm2y2+2,12,12,rgbPaint[color_chooser.color_id_choosing]);
      } else {
        GrRect(hdc,30+8*21+2,mm2y2+2,12,12,rgbPaint[player_pupil_color]);
      }
      if (option_choose>=0 && option_choose<=2 && color_chooser.is_choosing_color) {
        DrawPaintSquare(hdc,30+220+24,10+soptions_y,color_chooser.color_id,color_chooser.color_id_choosing);
      }





      //Game Options
      mm2y1=10+soptions_y+16*3-2;//+4;
      mm2y2=10+soptions_y+16*3;
      c=Highlight((option_choose==3),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,mm2y1,52,18,3);
      } else {
        GrPrint(hdc,30,mm2y2,"Camera Shake:",c);
      }


      if (is_khmer) {
        if (game_cam_shake) {
          DrawKhBool(hdc,hdc2,30+20*8,mm2y2,1,(option_choose==3));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,mm2y2,0,(option_choose==3));
        }        
      } else {
        if (game_cam_shake) {
          GrPrint(hdc,30+20*8,mm2y2,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,mm2y2,"<OFF>",c);
        }
      }



      mm2y1=10+soptions_y+16*4-2;
      mm2y2=10+soptions_y+16*4;
      c=Highlight((option_choose==4),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,mm2y1,38,18,4);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*4,"Audio:",c);
      }


      if (is_khmer) {
        if (game_audio) {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*4-2,1,(option_choose==4));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*4-2,0,(option_choose==4));
        }        
      } else {
        if (game_audio) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*4,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*4,"<OFF>",c);
        }
      }

      //Sound=============
      c=Highlight((option_choose==5),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*5-2,101,18,5);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*5,"Sound Effects Volume:",c);
      }
      if (is_khmer) {
        wchar_t wprint_volume[8];
        swprintf(wprint_volume,8,L"<%1.0f%>",game_volume*100);
        GrPrintW(hdc,30+20*8,10+soptions_y+16*5-4,ReplaceToKhmerNum(wprint_volume),"",c,16,FALSE,yes_unifont);
      } else {
        char print_volume[8];
        sprintf(print_volume,"<%1.0f%>",game_volume*100);
        GrPrint(hdc,30+20*8,10+soptions_y+16*5,print_volume,c);
      }


      /*c=Highlight((option_choose==6),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*6-2,101,18,6);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*6,"Raw Wav Volume:",c);
      }
      char print_wav_out_volume[7];
      sprintf(print_wav_out_volume,"<%1.0f%%>",wav_out_volume*100);
      GrPrint(hdc,30+20*8,10+soptions_y+16*6,print_wav_out_volume,c);*/



      //Graphics===============
      c=Highlight((option_choose==7),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*7-2,43,19,7);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*7,"Unifont:",c);
      }
      if (is_khmer) {
        if (yes_unifont) {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*7-2,1,(option_choose==7));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*7-2,0,(option_choose==7));
        }        
      } else {
        if (yes_unifont) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*7,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*7,"<OFF>",c);
        }
      }

      c=Highlight((option_choose==8),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*8+1,49,14,8);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*8,"Shadows:",c);
      }
      if (is_khmer) {
        if (game_shadow) {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*8-2,1,(option_choose==8));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*8-2,0,(option_choose==8));
        }        
      } else {
        if (game_shadow) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*8,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*8,"<OFF>",c);
        }
      }



      //Misc============
      c=Highlight((option_choose==9),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*9-2,48,20,9);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*9,"Window Borders:",c);
      }
      if (is_khmer) {
        if (!hide_taskbar) {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*9-2,1,(option_choose==9));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*9-2,0,(option_choose==9));
        }        
      } else {
        if (!hide_taskbar) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*9,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*9,"<OFF>",c);
        }
      }


      c=Highlight((option_choose==10),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*10-2,62,22,10);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*10,"Resolution:",c);
      }
      
      if (is_khmer) {
        wchar_t wprintres[32];
        swprintf(wprintres,32,L"<%dx%d> [%ls]",RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],WRESOLUTION_NAME[resolution_choose]);
        GrPrintW(hdc,30+20*8,10+soptions_y+16*10-4,ReplaceToKhmerNum(wprintres),"",c,16,FALSE,yes_unifont);
      } else {
        char printres[32];
        sprintf(printres,"<%dx%d> [%s]",RESOLUTION_X[resolution_choose],RESOLUTION_Y[resolution_choose],RESOLUTION_NAME[resolution_choose]);
        GrPrint(hdc,30+20*8,10+soptions_y+16*10,printres,c);
      }



      c=Highlight((option_choose==11),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*11-2,66,22,11);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*11,"Window Align Position:",c);
      }
      if (is_khmer) {
        DrawKhCornMid(hdc,hdc2,30+20*8,10+soptions_y+16*11-2,(option_choose==11));
      } else {
        GrPrint(hdc,30+20*8,10+soptions_y+16*11,"<Corner || Middle>",c);
      }



      c=Highlight((option_choose==12),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*12-2,141,22,12);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*12,"Show FPS:",c);
      }
      if (is_khmer) {
        if (show_fps) {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*12-2,1,(option_choose==12));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*12-2,0,(option_choose==12));
        }        
      } else {
        if (show_fps) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*12,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*12,"<OFF>",c);
        }
      }


      c=Highlight((option_choose==13),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*13-2,52,18,13);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*13,"Show Hijiri:",c);
      }
      if (is_khmer) {
        if (show_hijiri) {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*13-2,1,(option_choose==13));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*13-2,0,(option_choose==13));
        }        
      } else {
        if (show_hijiri) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*13,"<OFF>",c);
        }
      }







      c=Highlight((option_choose==14),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*14-2,129,22,14);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*14,"Difficulty:",c);
      }

      if (is_khmer) {
        if (game_hard) {
          DrawKhDiffic(hdc,hdc2,30+20*8,10+soptions_y+16*14-2,1,(option_choose==14));
        } else {
          DrawKhDiffic(hdc,hdc2,30+20*8,10+soptions_y+16*14-2,0,(option_choose==14));
        }
      } else {
        if (game_hard) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*14,"<HARD>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*14,"<NORMAL>",c);
        }
      }




      c=Highlight((option_choose==15),WHITE,LTGREEN);
      if (is_khmer) {
        DrawMM2Kh(hdc,hdc2,30,10+soptions_y+16*15,34,11,15);
      } else {
        GrPrint(hdc,30,10+soptions_y+16*15,"Free Will:",c);
      }

      if (is_khmer) {
        if (free_will) { //វាសនា
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*15-4,0,(option_choose==15));
        } else {
          DrawKhBool(hdc,hdc2,30+20*8,10+soptions_y+16*15-4,1,(option_choose==15));
        }        
      } else {
        if (free_will) {
          GrPrint(hdc,30+20*8,10+soptions_y+16*15,"<ON>",c);
        } else {
          GrPrint(hdc,30+20*8,10+soptions_y+16*15,"<OFF>",c);
        }
      }

      //===========================

      //int add_option_choose=0;
      //if (option_choose>8) {
        //add_option_choose=16*3;
      //} else if (option_choose>6) {
        //add_option_choose=16*2;
      //} else if (option_choose>3) {
        //add_option_choose=16;
      //}
      GrPrint(hdc,20,10+soptions_y+16*option_choose/*+add_option_choose*/,"*",LTGREEN);


      if (is_khmer) {
        if (hide_taskbar) {
          GrPrint(hdc,30,main_menu_y+10+16*21+8,"[SHIFT_ESC]:",WHITE);
          DrawBitmap(hdc,hdc2,30+8*13,main_menu_y+10+16*21+8,0,0,47,19,kh_goback_mask,SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,30+8*13,main_menu_y+10+16*21+8,0,0,47,19,kh_goback,SRCPAINT,FALSE,FALSE);
        } else {
          GrPrint(hdc,30,main_menu_y+10+16*20+8,"[SHIFT_ESC]:",WHITE);
          DrawBitmap(hdc,hdc2,30+8*13,main_menu_y+10+16*20+8,0,0,47,19,kh_goback_mask,SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,30+8*13,main_menu_y+10+16*20+8,0,0,47,19,kh_goback,SRCPAINT,FALSE,FALSE);
        }
      } else {
        if (hide_taskbar)
          GrPrint(hdc,30,main_menu_y+10+16*21+8,"[SHIFT_ESC]: Back.",WHITE);
        else
          GrPrint(hdc,30,main_menu_y+10+16*20+8,"[SHIFT_ESC]: Back.",WHITE);
      }

      break;





    case 2: //Create New Level 
    case 3:
       GrRect(hdc,0,0,GR_WIDTH+1,GR_HEIGHT+1,BLACK);
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
  }
  DrawPlayingMusic(hdc,16+4,help_y+48,BLACK,WHITE,0);
  if (!stop_playing_song[1] || (stop_playing_song[1] && gct==1)) {
    DrawPlayingMusic(hdc,16+4,help_y+48+34,BLACK,WHITE,1);
  }

  if (show_fps) {
    char fpstxt[10];
    sprintf(fpstxt,"FPS: %d / %d",saved_showoff,FPS);
    int FPS_x=GR_WIDTH-strlen(fpstxt)*8-14;
    GrPrintThick(hdc,FPS_x,main_menu_y+10+64,fpstxt,WHITE,BLACK);
  }
}







#define HELP_TEXT_ARR_NUM1   17
char help_txt_arr1[HELP_TEXT_ARR_NUM1][64]=
{
  "Controls:",
  "'W' - Jump from Surface",
  "'S'_- Block while on Ground",
  " |__- Hold while in air for Bounce Attack",
  " |__- Let Go and Jump after for High Jump",
  "[Shift] + 'S' or 'W' - Switch position on Web",
  "'A' - Move Left (Clockwise)",
  "'D' - Move Right (Anti-Clockwise)",
  "'Q' - Pick Up Web Standing On",
  "'Z' - Time Breaker Ability",
  "'E' - Creep (Slower) Mode",
  "'2' - Change Pellet Shooting Mode",
  "'3' - Toggle Web Builder Mode",
  "[Space] or 'C' - Increase Reaction Time",
  "[Left Click] or '1' - Shoot Pellet & Stop Web-Sling",
  "[Right Click] or '4' - Sling Web",
  "[Enter] - Restart Level",
};



#define HELP_TEXT_ARR_NUM2   7
char help_txt_arr2[HELP_TEXT_ARR_NUM2][64]=
{
  "Controls While Swinging:",
  "'W' - Decrease Web Length - Move towards Pivot",
  "'S' - Increase Web Length - Move Away from Pivot",
  "'A' - Swing Clockwise",
  "'D' - Swing Anti-Clockwise",
  "[Left Click] or '1' - Swing without Web Placement",
  "[Right Click] or '2' - Swing with Web Placement"
};


//======== UI =========

bool display_controls=FALSE;
void DrawUI(HDC hdc,HDC hdc2) 
{
  //
  //char leprinttest[10];
  //sprintf(leprinttest,"%d",player.placed_web_pos);
  //GrPrint(hdc,8,100,leprinttest,LTBLUE);


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


  if (player.health>0) {
  if (is_khmer) {
      int c_kh1=BROWN;//GREEN;
      int c_kh2=YELLOW;//LTGREEN;
      if (game_hard) {
        c_kh1=LTCYAN;
        c_kh2=LTBLUE;
      }
    //Draw Best Score
        if (!game_hard) {
          DrawBitmap(hdc,hdc2,16+8,1+8+16+digit_num4,0,0,65,19,ga0_kh_mask[0],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,16+8,1+8+16+digit_num4,0,0,65,19,ga0_kh[0],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,16+8,1+8+16+digit_num4,0,0,65,19,ga0_khhard_mask[0],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,16+8,1+8+16+digit_num4,0,0,65,19,ga0_khhard[0],SRCPAINT,FALSE,FALSE);
        }

      if (best_time<2147483.645) {
        swprintf(wgamebesttimetxt,32,L"%5.3f",best_time);
      } else {
        swprintf(wgamebesttimetxt,32,L"  ----");
      }


      GrPrintWThick(hdc,16+4,24+8+16+digit_num4,ReplaceToKhmerNum(wgamebesttimetxt),"",c_kh2,c_kh1,16,FALSE,yes_unifont);

    //Draw Current Time/Congrats
      if (!game_over) {
        if (!game_hard) {
          DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_kh_mask[1],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_kh[1],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_khhard_mask[1],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_khhard[1],SRCPAINT,FALSE,FALSE);
        }

        swprintf(wgametimetxt,32,L"%5.3f",print_time_ms);
        digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16-24;
        GrPrintWThick(hdc,digit_num-4,24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh2,c_kh1,16,FALSE,yes_unifont);

      } else { //game is over
        if (game_timer<int_best_score) { //New Score :D
          DrawBitmap(hdc,hdc2,GR_WIDTH-106-32,8+16+digit_num4,0,0,123,37,ga0_kh_mask[2],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,GR_WIDTH-106-32,8+16+digit_num4,0,0,123,37,ga0_kh[2],SRCPAINT,FALSE,FALSE);


          swprintf(wgametimetxt,32,L"%5.3f",print_time_ms);
          digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16-24;
          GrPrintWThick(hdc,digit_num-4,16+24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",LTPURPLE,PURPLE,16,FALSE,yes_unifont);
        } else {
          if (!game_hard) {
            DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_kh_mask[1],SRCAND,FALSE,FALSE);
            DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_kh[1],SRCPAINT,FALSE,FALSE);
          } else {
            DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_khhard_mask[1],SRCAND,FALSE,FALSE);
            DrawBitmap(hdc,hdc2,GR_WIDTH-69-32,8+16+4+digit_num4,0,0,69,16,ga0_khhard[1],SRCPAINT,FALSE,FALSE);
          }

          swprintf(wgametimetxt,32,L"%5.3f",print_time_ms);
          digit_num=GR_WIDTH-wcslen(wgametimetxt)*12-16-24;
          GrPrintWThick(hdc,digit_num-4,24+8+16+digit_num4,ReplaceToKhmerNum(wgametimetxt),"",c_kh2,c_kh1,16,FALSE,yes_unifont);
        }
      }


    //Draw Enemy Num/game Over
      if (printenemykills>0) {
        digit_num1=GR_WIDTH/2-133/2+4;//-(12*8)/2+4+4;
        if (!game_hard) {
          DrawBitmap(hdc,hdc2,digit_num1,16+10+digit_num4-4,0,0,133,26,ga0_kh_mask[3],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,digit_num1,16+10+digit_num4-4,0,0,133,26,ga0_kh[3],SRCPAINT,FALSE,FALSE);
        } else {
          DrawBitmap(hdc,hdc2,digit_num1,16+10+digit_num4-4,0,0,133,26,ga0_khhard_mask[2],SRCAND,FALSE,FALSE);
          DrawBitmap(hdc,hdc2,digit_num1,16+10+digit_num4-4,0,0,133,26,ga0_khhard[2],SRCPAINT,FALSE,FALSE);
        }
        //print enemy kills below
        swprintf(wenemykills,10,L"%d",printenemykills);
        digit_num2=GR_WIDTH/2-(wcslen(wenemykills)*12)/2+4;
        GrPrintWThick(hdc,digit_num2-4,16+8+24+digit_num3+digit_num4,ReplaceToKhmerNum(wenemykills),"",c_kh2,c_kh1,16,FALSE,yes_unifont);
      } else { //GAME OVER
        digit_num3=-32;//16;
        if (frame_tick<FPS/2) {
          if (!game_hard) {
            DrawBitmap(hdc,hdc2,GR_WIDTH/2-52/2+4,16+10+32+digit_num3+digit_num4,0,0,52,22,ga0_kh_mask[4],SRCAND,FALSE,FALSE);
            DrawBitmap(hdc,hdc2,GR_WIDTH/2-52/2+4,16+10+32+digit_num3+digit_num4,0,0,52,22,ga0_kh[4],SRCPAINT,FALSE,FALSE);
          } else {
            DrawBitmap(hdc,hdc2,GR_WIDTH/2-52/2+4,16+10+32+digit_num3+digit_num4,0,0,52,22,ga0_khhard_mask[3],SRCAND,FALSE,FALSE);
            DrawBitmap(hdc,hdc2,GR_WIDTH/2-52/2+4,16+10+32+digit_num3+digit_num4,0,0,52,22,ga0_khhard[3],SRCPAINT,FALSE,FALSE);
          }
        }
      }

  } else { //not-khmer
      if (!game_over) {
        sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
        digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
        if (!game_hard) {
          GrPrintThick(hdc,digit_num-4,8+16+digit_num4,gametimetxt,BROWN,YELLOW);
        } else {
          GrPrintThick(hdc,digit_num-4,8+16+digit_num4,gametimetxt,LTCYAN,LTBLUE);
        }
      } else { //game is over
        if (game_timer<int_best_score) { //New Score :D
          sprintf(gametimetxt,"New Best! :D %5.3f",print_time_ms);
          digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
          GrPrint(hdc,digit_num+5,9+16+digit_num4,gametimetxt,LTPURPLE);
        } else {
          sprintf(gametimetxt,"Time: %5.3f",print_time_ms);
          digit_num=GR_WIDTH-strlen(gametimetxt)*8-16;
          if (!game_hard) {
            GrPrintThick(hdc,digit_num-4,8+16+digit_num4,gametimetxt,BROWN,YELLOW);
          } else {
            GrPrintThick(hdc,digit_num-4,8+16+digit_num4,gametimetxt,LTCYAN,LTBLUE);
          }
        }
      }

      if (best_time<2147483.645) {
        sprintf(gamebesttimetxt,"Best: %5.3f",best_time);
      } else {
        sprintf(gamebesttimetxt,"Best:   - - - -");
      }
      if (!game_hard) {
        GrPrintThick(hdc,16+4,8+16+digit_num4,gamebesttimetxt,BROWN,YELLOW);
      } else {
        GrPrintThick(hdc,16+4,8+16+digit_num4,gamebesttimetxt,LTCYAN,LTBLUE);
      }

      digit_num3=0;
      if (printenemykills>0) {
        digit_num1=GR_WIDTH/2-(12*8)/2+4+4;
        if (!game_hard) {
          GrPrintThick(hdc,digit_num1-4,8+16+digit_num4,"Enemies Left:",BROWN,YELLOW);
        } else {
          GrPrintThick(hdc,digit_num1-4,8+16+digit_num4,"Enemies Left:",LTCYAN,LTBLUE);
        }

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
      if (!game_hard) {
        GrPrintThick(hdc,digit_num2-4,8+32+digit_num3+digit_num4,enemykills,BROWN,YELLOW);
      } else {
        GrPrintThick(hdc,digit_num2-4,8+32+digit_num3+digit_num4,enemykills,LTCYAN,LTBLUE);
      }
    }
  }



//========================

  if (call_help_timer<1000/*5000*/) {
    DrawPlayingMusic(hdc,16+4,help_y+48,c,c4,0);
    if (!stop_playing_song[1] || (stop_playing_song[1] && gct==1)) {
      DrawPlayingMusic(hdc,16+4,help_y+48+34,c,c4,1);
    }
  }

  if (show_fps) {
    char fpstxt[24];
    sprintf(fpstxt,"FPS: %d / %d",saved_showoff,FPS);
    int FPS_x=GR_WIDTH-strlen(fpstxt)*8-14;
    if (!is_khmer) {
      GrPrintThick(hdc,FPS_x,10+32,fpstxt,c,BLACK);
    } else {
      GrPrintThick(hdc,FPS_x,10+64,fpstxt,c,BLACK);
    }
  }

//=========Draw Player UI
  int i=0,j=0;
  int c2,c3,ca;

  c2 = LTRED;//Highlight(IsInvertedBackground(),LTRED,LTCYAN);
  ca = RED;//Highlight(IsInvertedBackground(),RED,CYAN);
  c3 = LTGRAY;//Highlight(IsInvertedBackground(),LTGRAY,DKGRAY);

  if (player.health>0) {
  float percentage=player.health/DEFAULT_PLAYER_HEALTH;
  float health_length=64*percentage;

  float bpercentage=player.block_health/player.block_health_max;
  float bhealth_length=150*bpercentage;

  //Draw Block Health
  if (player.show_block_health_timer>0 || (player.health<=PLAYER_LOW_HEALTH+1)) {
//    GrRect(hdc,player.sprite_x-76,player.sprite_y-50-3,152,8,c4);
//    GrRect(hdc,player.sprite_x-75,player.sprite_y-50-2,bhealth_length,6,c3);
    GrRect(hdc,player.sprite_x-76,player.sprite_y-41-3,152,10,c4);
    GrRect(hdc,player.sprite_x-75,player.sprite_y-41-2,bhealth_length,8,c3);
  }

  //Draw Player Health
  if (player.show_health_timer>0 || (player.health<=PLAYER_LOW_HEALTH+1)) {
    if (health_length>150) {
      health_length=150;
    }

    if (player.health>PLAYER_LOW_HEALTH+1) {
      //GrRect(hdc,player.sprite_x-health_length/2-2,player.sprite_y-42-1,health_length+4,6,BLACK);
      GrRect(hdc,player.sprite_x-health_length/2,player.sprite_y-42,health_length,6,c2);
    } else {
      health_length*=4;
      //GrRect(hdc,player.sprite_x-health_length/2-2,player.sprite_y-42-1,health_length+4,6,BLACK);
      if (frame_tick%15<7) {
        GrRect(hdc,player.sprite_x-health_length/2,player.sprite_y-42,health_length,6,ca);
      } else {
        GrRect(hdc,player.sprite_x-health_length/2,player.sprite_y-42,health_length,6,c2);
      }
    }


    //if (health_length>=0) {
      //print health numbers
    if (player.health>PLAYER_LOW_HEALTH+1) {
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
  if (player.type==0) {
  for (i=0;i<player.speed;i++) {
    float speed_angle=i*0.1;
    float speed_dist=64;
    float angle_limit=M_PI_4+M_PI_2;
    if (speed_angle>angle_limit) {
      int speed_times=speed_angle/angle_limit;
      speed_angle-=angle_limit*speed_times;
      speed_dist=64+8*speed_times;
      c2 = LTCYAN;
    } else {
        if (i<5) c2=GREEN;
        else if (i<10) c2=LTGREEN;
        else c2=RED;
    }
    GrCircle(hdc,
      //player.sprite_x-speed_dist*cos(speed_angle), //player centric
      //player.sprite_y-speed_dist*sin(speed_angle),
      mouse_x-speed_dist*cos(speed_angle),
      mouse_y-speed_dist*sin(speed_angle),
      3,c2,BLACK
    );
  }
  }



  int c5=LTGREEN;//Highlight(IsInvertedBackground(),LTGREEN,LTPURPLE);
  int c10=LTRED;//Highlight(IsInvertedBackground(),LTRED,LTCYAN);


  int player_web_remaining=player.max_web_num-player.placed_web_num;

  //EXP Bar
  //if (player.show_exp_timer>0) {
    int exp_l=9;//18;
    int lk=10;//20;
    int loffset=0;
    if (player.type==1)
      loffset=8;
    //GrRect(hdc,player.sprite_x-50,player.sprite_y+23,100,5,c4);
    GrRect(hdc,mouse_x-50+14,mouse_y+23+20-loffset,100,5,c4);
    if (game_hard) {
      exp_l=18;//31;
      lk=20;//33;
    }
    for (int k=0;k<player.exp;k++) {
      //GrRect(hdc,1+player.sprite_x-50+k*lk,player.sprite_y+24,exp_l,3,c5);
      GrRect(hdc,1+mouse_x-50+k*lk+14,mouse_y+24+20-loffset,exp_l,3,c5);      
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
  //}


  //Knives per throw Bar
  int knifethrowsx=mouse_x-40;
  int knifethrowsy=mouse_y+32;
  int knifethrowstxtx=mouse_x+32;
//  int knifethrowstxty=mouse_y-48;
  int knifethrowstxty=mouse_y+52;
  //32 bullets
//  GrRect(hdc,knifethrowsx,knifethrowsy+1,PLAYER_BULLET_NUM*3,9,c4); //black
//  GrRect(hdc,knifethrowsx,knifethrowsy,player.knives_per_throw*3,10,c10);

  if (player.type==1) {
    knifethrowsy-=8;
  }

  GrRect(hdc,knifethrowsx-1,knifethrowsy+1,PLAYER_BULLET_NUM*7+2+1,9,c4); //black
  GrRect(hdc,knifethrowsx,knifethrowsy,player.knives_per_throw*7,10,c10);


  c2 = LTGREEN;
  int ck=LTCYAN;
  for (int k=0;k<PLAYER_BULLET_NUM-player.bullet_shot_num;k++) {
    //GrLine(hdc,player.sprite_x-48+k*4-2,player.sprite_y+32,player.sprite_x-48+k*4+2,player.sprite_y+36,c5);
    //GrLine(hdc,player.sprite_x-48+k*4+2,player.sprite_y+32,player.sprite_x-48+k*4-2,player.sprite_y+36,c5);
    //GrLine(hdc,player.sprite_x-48+k*4-2,player.sprite_y+32,player.sprite_x-24+k*4,player.sprite_y+36,c2);
    //GrCircle(hdc,player.sprite_x-48+k*4,player.sprite_y+34,2,c5,-1);
    int ckpt=Highlight((k<player.knives_per_throw),ck,c2);
    GrRect(hdc,knifethrowsx+k*7,knifethrowsy+2,7,6,ckpt);
    //GrRect(hdc,knifethrowsx+k*4,knifethrowsy+2,4,6,ckpt);
    //GrRect(hdc,knifethrowsx+k*3,knifethrowsy+2,2,6,ckpt); //32 bullets

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
  if (player.type==0) {
    swprintf(bulletlefttxt,32,L"%d/%d",player.bullet_num,bullet_minus);
    swprintf(bulletlefttxt2,32,L"%d/%d [%d]",player.bullet_num,bullet_minus,player_web_remaining);
  } else if (player.type==1) {
    swprintf(bulletlefttxt,32,L"%d/%d",player.bullet_num,bullet_minus);
    swprintf(bulletlefttxt2,32,L"%d/%d",player.bullet_num,bullet_minus);
  }
  //GrPrint(hdc,knifethrowsx-32-8,knifethrowsy-4,bulletlefttxt,c);
  int bc=Highlight(player.bullet_num<bullet_minus,c,LTRED);
  int bc2=Highlight(player_web_remaining==0,c,LTRED);
  if (is_khmer) {
    GrPrintW(hdc,knifethrowstxtx-32-30-8,knifethrowstxty-4-loffset,ReplaceToKhmerNum(bulletlefttxt2),"",bc2,16,FALSE,yes_unifont);
    GrPrintW(hdc,knifethrowstxtx-32-30-8,knifethrowstxty-4-loffset,ReplaceToKhmerNum(bulletlefttxt),"",bc,16,FALSE,yes_unifont);
  } else {
    GrPrintW(hdc,knifethrowstxtx-32-8,knifethrowstxty-4-loffset,bulletlefttxt2,"",bc2,16,FALSE,yes_unifont);
    GrPrintW(hdc,knifethrowstxtx-32-8,knifethrowstxty-4-loffset,bulletlefttxt,"",bc,16,FALSE,yes_unifont);
  }

  /*if (player.show_exp_timer>0) {
    if (is_khmer) {
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,ReplaceToKhmerNum(bulletlefttxt2),"",bc2,16,FALSE,yes_unifont);
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,ReplaceToKhmerNum(bulletlefttxt),"",bc,16,FALSE,yes_unifont);
    } else {
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,bulletlefttxt2,"",bc2,16,FALSE,FALSE);
      GrPrintW(hdc,player.sprite_x+58,player.sprite_y+16,bulletlefttxt,"",bc,16,FALSE,FALSE);
    }
  }*/



  //===-- Draw Timebreaker Circle ---===
  if (!game_hard) {
    c5 = Highlight(player.time_breaker,LTPURPLE,YELLOW);
  } else {
    c5 = Highlight(player.time_breaker,LTCYAN,YELLOW);
  }

  int c6 = Highlight(game_hard,PURPLE,LTBLUE);

  //draw player time breaker
  const int tb_circle_r=8;
  if (player.time_breaker_units<player.time_breaker_units_max || (player.time_breaker_units==player.time_breaker_units_max && frame_tick%16<8)) {
  for (i=0;i<player.time_breaker_units;i++) {
    float tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*i*2;
    if (player.time_breaker || (IsSpeedBreaking() && game_hard)) {
      if ((player.time_breaker_units<=4 && frame_tick%10<5) || player.time_breaker_units>4) {
        GrCircle(hdc,
          player.sprite_x-32*cos(tb_angle),
          player.sprite_y-32*sin(tb_angle),
          3,c5,BLACK); //player centric
      }
    }

    //on cursor
    if (!player.time_breaker) {
        GrCircle(hdc,
          mouse_x-tb_circle_r*cos(tb_angle),
          mouse_y-tb_circle_r*sin(tb_angle),
          2,c5,BLACK);
    } else {
        GrCircle(hdc,
          mouse_x-tb_circle_r*cos(tb_angle),
          mouse_y-tb_circle_r*sin(tb_angle),
          3,c5,c5);
    }
  }
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    int frame_t=frame_tick%20;
    for (i=0;i<3;i++) {
      float tb_angle=M_PI_2+2*M_PI_2/player.time_breaker_units_max*(frame_t-i)*2;
      /*GrCircle(hdc,
        player.sprite_x-32*cos(tb_angle),
        player.sprite_y-32*sin(tb_angle),
        3,LTPURPLE,LTPURPLE);*/  //player centric
      GrCircle(hdc,
        mouse_x-tb_circle_r*cos(tb_angle),
        mouse_y-tb_circle_r*sin(tb_angle),
        4,c6,c6);
    }
  }


  //======= Draw controls are on ========
  if (player.uppercut)
    GrPrint(hdc,mouse_x-16,mouse_y-48,"E",LTGREEN);
  else
    GrPrint(hdc,mouse_x-16,mouse_y-48,"E",LTRED);

  if (player.low_jump)
    GrPrint(hdc,mouse_x-32,mouse_y-48,"3",LTGREEN);
  else
    GrPrint(hdc,mouse_x-32,mouse_y-48,"3",LTRED);

  if (player.block_timer>0) {
    if (player.block_timer<=23) {
      //GrPrint(hdc,mouse_x+20,mouse_y-48,"{__}",WHITE);
      GrPrint(hdc,mouse_x+17,mouse_y-48,"S",LTGREEN);
    } else {
      GrPrint(hdc,mouse_x+17,mouse_y-48,"S",CYAN);
    }
  } else {
    GrPrint(hdc,mouse_x+17,mouse_y-48,"S",LTRED);
  }

  if ((!game_hard) ||
      (game_hard && !player.time_breaker && player.time_breaker_units>0)
    )
  {
    if (IsSpeedBreaking())
      GrPrint(hdc,mouse_x+48,mouse_y-48,"C",LTGREEN);
    else
      GrPrint(hdc,mouse_x+48,mouse_y-48,"C",LTRED);
  }

  if (player.time_breaker_units==player.time_breaker_units_max && !player.time_breaker) {
    if (frame_tick%16<8) {
      GrPrint(hdc,mouse_x,mouse_y-48,"Z",LTCYAN);
    }
  } else {
    GrPrint(hdc,mouse_x,mouse_y-48,"Z",LTRED);
  }

  if (player.time_breaker) {
    GrPrint(hdc,mouse_x,mouse_y-48,"Z",YELLOW);
  }
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
  } else if (call_help_timer<2000/*5000*/) {
    GrPrint(hdc,16+4,help_y+16,"Press '*' for Controls Help",c4);
    GrPrint(hdc,16+5,help_y+17,"Press '*' for Controls Help",c);
  }



}


/*#define MARBLE_NUM 256//256
struct marble
{
  int color_id;
  float x,y,angle;
} marble[MARBLE_NUM];

void InitMarbles(int max_marbles)
{
  for (int i=0;i<max_marbles;i++) {
    marble[i].x=GR_WIDTH/2;
    marble[i].y=GR_HEIGHT/2;
    marble[i].angle=(2*M_PI/(max_marbles+1)*i);
  }
}*/


int loading_dummy=0;

void DrawLoading(HDC hDC/*,int max_marbles*/)
{
  //GrRect(hDC,0,0,GR_WIDTH+2,GR_HEIGHT+2,DKRDKGRAY);
  //printf("x:%5.4f\n",marble[0].x);
  /*int title_x0=GR_WIDTH/2-352/2+4;
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

/*
    for (int j=0;j<max_marbles;j++) {
      if (j!=i) {
        if (GetDistance(marble[i].x,marble[i].y,marble[j].x,marble[j].y)<10) {
          marble[i].angle=GetBounceAngle(marble[i].angle,marble[j].angle);
        }
      }
    }
    GrCircle(hDC,marble[i].x,marble[i].y,10,rgbPaint[i],rgbPaint[i]);
  }
  }*/

  /*DrawBitmap(hdc,hdc2,title_x0,
                 title_y,
                 0,
                 0,
                 352,
                 256,
                title_sprite_mask,SRCAND,FALSE,FALSE);
  //Draw platforms paint
  DrawBitmap(hdc,hdc2,title_x0,
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
  float grectl=(GR_WIDTH-34-34)*loading_percentage;
  if (!hide_taskbar) {
    extra_y=32;
  }
  //if (flag_display_long_loading) {
    //GrPrintThick(hDC,34,GR_HEIGHT-46-extra_y-32,"Loading shadows, this will take a while...",WHITE,BLACK);
  //}
  GrRect(hDC,32,GR_HEIGHT-48-extra_y,GR_WIDTH-64,8*3,WHITE);
  if (loading_denominator>0) {
    loading_percentage=loading_numerator/loading_denominator;   
    GrRect(hDC,34,GR_HEIGHT-46-extra_y,grectl,6*3+1,GREEN);
    GrRect(hDC,34+6+loading_dummy,GR_HEIGHT-46-extra_y,6,6*3+1,LTGREEN);
    loading_dummy+=8;
    if (loading_dummy>grectl-6) {
      loading_dummy=0;
    }
  }
}


