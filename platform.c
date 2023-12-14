


//Platform
struct Platform {
  double x;
  double y;
  int length;
} platform[PLATFORM_NUM];


void SetPlatform(int i,int y) {
  platform[i].x=8*RandNum(0,GR_WIDTH/8);
  platform[i].y=y;
  platform[i].length=8*RandNum(1,8);
}


void InitPlatform() {
  platform[0].x=player.x-20;
  platform[0].y=player.y+PLAYER_HEIGHT;
  platform[0].length=40;
  for (int i=1;i<PLATFORM_NUM;i++) {
    SetPlatform(i,8*RandNum(0,GR_HEIGHT/8));
  }
}


void PlatformAct(int pid,int g) {
  //gravity
  if (g==0) {
    platform[pid].y+=0.5;
  }
  if (platform[pid].y>GR_HEIGHT) {
    SetPlatform(pid,8*RandNum(0,4));
  }
  //collision
  if ((platform[pid].y-2<=player.y && player.y<=platform[pid].y) &&
       ((platform[pid].x-16<=player.x && player.x<=platform[pid].x+platform[pid].length+16) ||
        (platform[pid].x-16<=player.x && player.x<=platform[pid].x+platform[pid].length+16)
      )) {
    player.grav=1;
    player.jump_height=64;
    player.y-=2;
    player.in_air=false;
  }
}


void DrawPlatforms(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  for (int i=0;i<PLATFORM_NUM;i++) {
    GrLine(hwnd,hdc,ps,platform[i].x,platform[i].y,platform[i].x+platform[i].length,platform[i].y,RGB(255,255,255));
  }
}






