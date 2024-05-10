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

int x = 20, y = 22;
int incX = 1, incY = 1;
int bulletX = -1, bulletY = -1;
int bulletSpeed = 100000;

void movimentar(char ch);

void printBullet() {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(bulletX, bulletY);

  for (int i = 0; i < 20; i++) {
    screenGotoxy(bulletX, bulletY - i);
    printf("^");
    usleep(bulletSpeed);
    screenUpdate();
    screenGotoxy(bulletX, bulletY - i);
    printf("  ");
    
    screenSetColor(CYAN, DARKGRAY);

    if (keyhit()) {
      char ch = readch();
      movimentar(ch);
      ch = 0;
    }
  }
}

void movimentar(char ch) {
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
  if (ch == ' ') {
    bulletX = x + 1;
    bulletY = y - 1;
    printBullet();
  }
}

void enemies() {
  int x = 7, y = 3;

  char inimigos[5][10] = {{'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M'},
                          {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M'},
                          {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M'},
                          {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M'},
                          {'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M', 'M'}};

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      screenGotoxy(x + j * 3,
                   y + i * 2); // Posiciona-se para imprimir o inimigo atual
      printf("%c", inimigos[i][j]); // Imprime o inimigo atual
      printf("  ");
    }
  }
}
void printHello(int nextX, int nextY) {
  static int ch = 0;
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" △ ");

  movimentar(ch);
}

int main() {
  static int ch = 0;

  screenInit(1);
  keyboardInit();
  timerInit(50);

  screenUpdate();

  enemies();

  while (ch != 10) // enter
  {
    // Handle user input
    if (keyhit()) {
      ch = readch();

      movimentar(ch);
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
