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

void printHello(int nextX, int nextY) {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf("           ");

  screenGotoxy(x, y);
  printf("           ");

  printf("Nave");
  printf("           ");
}

void printKey(int ch) {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(35, 22);

  screenGotoxy(34, 23);
  printf("            ");

  if (ch == 27)
    screenGotoxy(36, 23);
  else
    screenGotoxy(39, 23);

  printf("%d ", ch);
  while (keyhit()) {
    printf("%d ", readch());
  }
}

void printBullet() {
  if (bulletX != -1 && bulletY != -1) { // Se houver uma bala
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(bulletX, bulletY);
    printf("^");
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
        for (int i = 0; i < 10; i++) {

          bulletX = x + 13;
          bulletY = y - i;

        }
      }
      if (ch == 'a' || ch == 'A') {
        x -= 1; // Move a nave para a esquerda
      } else if (ch == 'd' || ch == 'D') {
        x += 1; // Move a nave para a direita
      }

      printKey(ch);
      printBullet();
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

      printKey(ch);
      printHello(newX, newY);

      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
