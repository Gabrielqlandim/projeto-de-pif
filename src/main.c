/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

int x = 20, y = 22;
int incX = 1, incY = 1;
int bulletX = -1, bulletY = -1;
int bulletSpeed = 150000;

struct enemies {
  char m;
  int vivo;
};

struct enemies enemy[5][10];

void matrizglobal() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      enemy[i][j].m = 'M';
      enemy[i][j].vivo = 1;
    }
  }
  for (int j = 0; j < 10; j++) {
    enemy[4][j].m = 'O';
    enemy[4][j].vivo = 1;
  }
}

void movimentar(char ch);

int colisaoInimigo();

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
    enemies();

    screenSetColor(CYAN, DARKGRAY);

    if (colisaoInimigo(bulletX, bulletY - i)) {
      break;
    }

    if (keyhit()) {
      char ch = readch();
      if (ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D') {

        movimentar(ch);
        ch = 0;
      }
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

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      screenGotoxy(x + j * 3,
                   y + i * 2);     // Posiciona-se para imprimir o inimigo atual
      printf("%c", enemy[i][j].m); // Imprime o inimigo atual
      printf("  ");
    }
  }
}
int colisaoInimigo(int bulletX, int bulletY) {
  int x = 7, y = 3;

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      if (bulletX == x + j * 3 && bulletY == y + i * 2 &&
          enemy[i][j].vivo == 1) {

        enemy[i][j].vivo = 0;
        enemy[i][j].m = 'X';
        enemies();
        screenUpdate();
        usleep(200000);

        enemy[i][j].m = ' ';
        enemies();
        return 1;
      }
    }
  }

  return 0;
}
void printHello(int nextX, int nextY) {
  static int ch = 0;
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" △ ");

  movimentar(ch);
}

void enemyShoot() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            if (enemy[i][j].vivo && i < 4 && !enemy[i + 1][j].vivo) {
                
                    int bulletX = (7 + j * 3 + 1) - 1;   // Posição x da bala
                    int bulletY = 3 + i * 2 + 1;   // Posição y da bala
                    // Disparar a bala
                    
                    for(int l = 0; l < 20; l++){
                      screenSetColor(RED, DARKGRAY);
                      screenGotoxy(bulletX, bulletY + l);
                      printf("*");
                      usleep(bulletSpeed);
                      screenUpdate();

                      screenGotoxy(bulletX, bulletY + l);
                      printf(" ");

                      if (keyhit()) {
                        char ch = readch();
                        if (ch == 'a' || ch == 'A' || ch == 'd' || ch == 'D' || ch == ' ') {

                          movimentar(ch);
                          ch = 0;
                        }
                      }
                      if(bulletY + l == 22){
                        break;
                      }
                    }      
                  
                    // Apagar a bala após um curto intervalo
                    screenGotoxy(bulletX, bulletY);
                    printf(" ");
                }
            }
        }
    }


int main() {
  static int ch = 0;

  screenInit(1);
  keyboardInit();
  timerInit(50);

  screenUpdate();

  matrizglobal();

  while (ch != 10) // enter
  {
    enemyShoot();
    // Handle user input
    if (keyhit()) {
      ch = readch();

      movimentar(ch);
      printHello(x, y);
      screenUpdate();
    }
    enemies();
    usleep(50000);
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
