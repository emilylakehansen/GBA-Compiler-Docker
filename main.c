// notQuiteBouncingRectangle.c
#define RGB(R, G, B) ((R) | (G) << 5 | (B) << 10)
#define REG_DISPCNT (*(volatile unsigned short *)0x04000000)
#define MODE3 3
#define BG2_ENABLE (1<<10)
#define VIDEO_BUFFER ((volatile unsigned short *)0x06000000)

#define REG_KEYINPUT (*(volatile u16*)0x04000130)

#define KEY_A        0x0001
#define KEY_B        0x0002
#define KEY_SELECT   0x0004
#define KEY_START    0x0008
#define KEY_RIGHT    0x0010
#define KEY_LEFT     0x0020
#define KEY_UP       0x0040
#define KEY_DOWN     0x0080
#define KEY_R        0x0100
#define KEY_L        0x0200

#define KEY_DOWN_NOW(key) (~(REG_KEYINPUT) & key)


typedef unsigned short u16;
typedef unsigned char u8;

#define SetPixel(x, y, val) (VIDEO_BUFFER[(x) + (y)*240] = val)

int time = 0;
int ballSize, ballX, ballY, ball_Vx, ball_Vy;
int prevBallX, prevBallY;
int screenWidth, screenHeight;
volatile u16* scanlineCounter = (u16*) 0x04000006;

void drawSquare(u8 x, u8 y, u8 size, u16 color){
  for (u8 i=0; i<size; i++){
    for (u8 j=0; j<size; j++){
      SetPixel(x+i, y+j, color);
    }
  }
}

void waitForVBlank() {
  while (*scanlineCounter >= 160);  // wait until current VBlank ends
  while (*scanlineCounter < 160);  // wait until next VBlank starts
}

void updateBallPosition() {
    prevBallX = ballX;
    prevBallY = ballY;

    ballX += ball_Vx;
    ballY += ball_Vy;

    if (ballX < 0){
      ballX = -ballX;
      ball_Vx = -ball_Vx;
    }
    if (ballY < 0){
      ballY = -ballY;
      ball_Vy = -ball_Vy;
    }
    if (ballX + ballSize >= 240) {
      ballX -= ballX + ballSize - 240;
      ball_Vx = -ball_Vx;
    }
    if (ballY + ballSize >= 160) {
      ballY -= ballY + ballSize - 160;
      ball_Vy = -ball_Vy;
    }
}

u16 colorFromInputs(){
  u16 color = RGB(31, 0, 0);
  if (KEY_DOWN_NOW(KEY_A)) {
    color = color | RGB(0, 0, 31);
  }
  if (KEY_DOWN_NOW(KEY_B)) {
    color = color | RGB(0, 31, 0);
  }
  return color;
}

int main() {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  time = 0;
  ballSize = 5;
  screenWidth = 240;
  screenHeight = 160;
  ballX = screenWidth / 2;
  ballY = screenHeight / 2;
  ball_Vx = 1;
  ball_Vy = 2;

  while (1) {
    int skipFrames = 2;  // only do things every so many frames
    if (time % skipFrames == 0) {
      updateBallPosition();
    }

    waitForVBlank();
    drawSquare(prevBallX, prevBallY, ballSize, 0);
    drawSquare(ballX, ballY, ballSize, colorFromInputs());
    time++;
  }
  return 0;
}
