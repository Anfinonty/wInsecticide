bool once=true;
//Player
struct player {
  bool rst_left,rst_right,rst_up,is_left,in_air;
  int jump_height, sprite_timer;
  double grav,x,y;
  HBITMAP sprite_1;
  HBITMAP sprite_2;
  HBITMAP sprite_jump;
} player;

void InitPlayer() {
  player.grav=1;
  player.sprite_timer=0;
  player.in_air=player.rst_left=player.rst_right=player.rst_up=false;
  player.is_left=false;
  player.jump_height=0;
  player.x=GR_WIDTH/2;
  player.y=GR_HEIGHT/2;
  if (once) {
    player.sprite_1 = (HBITMAP) LoadImageW(NULL, L"sprites/player1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.sprite_2 = (HBITMAP) LoadImageW(NULL, L"sprites/player2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    player.sprite_jump = (HBITMAP) LoadImageW(NULL, L"sprites/player3-1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    once=false;
  }
}

void PlayerAct() {
  //gravity
  if (player.jump_height<-10 && player.grav<2) {
    player.grav++;
  }
  if (player.grav>1) {
    player.in_air=true;
  }

  player.y+=1;
  player.jump_height--;
  //movement
  if (player.jump_height==62) {
    player.jump_height=0;
  }
  if (player.rst_up && player.jump_height==63) {
    player.jump_height=50;
  }
  if (0<player.jump_height && player.jump_height<51) {
    player.y-=2;
    player.in_air=true;
  }
  if (player.rst_left && player.x>0) {
    player.x--;
    player.is_left=true;
    if (player.sprite_timer==0)
      player.sprite_timer=20;
  } else if (player.rst_right && player.x<GR_WIDTH) {
    player.x++;
    player.is_left=false;
    if (player.sprite_timer==0)
      player.sprite_timer=20;
  }

  //Sprite timer
  if (player.sprite_timer>0) {
    player.sprite_timer--;
  }
}


void DrawPlayer(HWND hwnd, HDC hdc, PAINTSTRUCT ps) {
  if (player.in_air) {
      GrSprite(hwnd,hdc,ps,player.x,player.y-6,player.sprite_jump,player.is_left);
  } else {
    if (player.sprite_timer<10) {
      GrSprite(hwnd,hdc,ps,player.x,player.y,player.sprite_1,player.is_left);
    } else {
      GrSprite(hwnd,hdc,ps,player.x,player.y,player.sprite_2,player.is_left);
    }
  }
  //GrRect(hwnd,hdc,ps,player.x,player.y,PLAYER_WIDTH,PLAYER_HEIGHT,RGB(0, 76, 255));
}




