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
#include <unistd.h>

int x = 20, y = 22;
int incX = 1, incY = 1;
int bulletX = -1, bulletY = -1;
int bulletSpeed = 100000;

void printHello(int nextX, int nextY) {
  static int ch = 0;
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" △ ");

  if (keyhit()) {
    ch = readch();

    if (ch == 'a' || ch == 'A') {
      if (x > 2) { // Verifica se não está no limite esquerdo
        screenGotoxy(x, y);
        printf("  ");
        x -= 1;
        screenGotoxy(x, y);
        printf(" △ ");
      }
    } else if (ch == 'd' || ch == 'D') {
      if (x < 36) { // Verifica se não está no limite direito
        screenGotoxy(x, y);
        printf("  ");
        x += 1;
        screenGotoxy(x, y);
        printf(" △ ");
      }
    }
  }
}

void printBullet() {
  static int ch = 0;
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
    
    
      if (keyhit()) {
        ch = readch();

        if (ch == 'a' || ch == 'A') {
          if (x > 2) { // Verifica se não está no limite esquerdo
            screenGotoxy(x, y);
            printf("  ");
            x -= 1;
            screenGotoxy(x, y);
            printf(" △ ");
          }
        } else if (ch == 'd' || ch == 'D') {
          if (x < 36) { // Verifica se não está no limite direito
            screenGotoxy(x, y);
            printf("  ");
            x += 1;
            screenGotoxy(x, y);
            printf(" △ ");
          }
        }
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

  screenUpdate();

  screenGotoxy(20, 30);
  printf("sexo");

  while (ch != 10) // enter
  {
    // Handle user input
    if (keyhit()) {
      ch = readch();

      if (ch == ' ') {
        bulletX = x + 1;
        bulletY = y - 1;
        printBullet();
      }
      printHello(x, y);

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
