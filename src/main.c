/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <string.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

int x = 34, y = 22;
int incX = 1, incY = 1;
int bulletX = -1, bulletY = -1;
int bulletSpeed = 100000;

void printHello(int nextX, int nextY) {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf("            ");

  screenGotoxy(x, y);
  printf("           ");

  printf(" △ ");

  printf("           ");
}


void printBullet() {
  if (bulletX != -1 && bulletY != -1) { // Se houver uma bala
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(bulletX, bulletY);

    for (int i = 0; i < 10; i++) {
      screenGotoxy(bulletX, bulletY - i);
      printf("^");
      usleep(bulletSpeed);
      screenUpdate();
      screenGotoxy(bulletX, bulletY - i);
      printf("  ");

     
    }
    bulletX = -1;
    bulletY = -1;
  }
}

int main() {
  static int ch = 0;

  screenInit(1);
  keyboardInit();
  timerInit(50);

  printHello(x, y);
  screenUpdate();

  while (ch != 10) // enter
  {
    // Handle user input
    if (keyhit()) {
      ch = readch();

      if (ch == ' ') {
        bulletX = x + 13;
        bulletY = y - 1;
        printBullet();
      }
      if (ch == 'a' || ch == 'A') {
        x -= 1; // Move a nave para a esquerda
      } else if (ch == 'd' || ch == 'D') {
        x += 1; // Move a nave para a direita
      }

      screenUpdate();
    }

    // Update game state (move elements, verify collision, etc)
    if (timerTimeOver() == 1) {
      int newX = x + incX;
      if (newX >= (MAXX - strlen("Hello World") - 1) || newX <= MINX + 1)
        incX = -incX;
      int newY = y + incY;
      if (newY >= MAXY - 1 || newY <= MINY + 1)
        incY = -incY;

      printHello(newX, newY);

      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
