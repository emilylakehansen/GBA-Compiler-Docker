// main.c
#define RGB(R, G, B) ((R) | (G) << 5 | (B) << 10)
#define REG_DISPCNT (unsigned short *)0x04000000
#define PIXEL_00 0x06000000
#define p00 ((u16*)PIXEL_00)
#define SetPixel(x, y, val) (*(u16*)(PIXEL_00 + 2*((x) + 240*(y))) = val)
#define SetPixel2(x, y, val) (*((u16*)PIXEL_00 + ((x) + 240*(y))) = val)
#define SetPixel3(x, y, val) (p00[(x) + 240*(y)] = val)


typedef unsigned short u16;

void drawSquare(int x, int y, int size, u16 color) {
  int i, j;
  for (i = 0; i < size; i++) {
    for (j = 0; j < size; j++) {
      SetPixel3(x + i, y + j, color);
    }
  }
}

int main() {
  *REG_DISPCNT = 0x403;

  drawSquare(5, 5, 5, RGB(31, 0, 0));
  drawSquare(10, 10, 10, RGB(0, 31, 0));
  drawSquare(20, 20, 20, RGB(0, 0, 31));

  while (1) {} // just stay alive
  return 0;  // make strict compilers happy
}
