/**
 * main.h
 * Created on Aug, 23th 2023
 * Author: Tiago Barros
 * Based on "From C to C++ course - 2002"
 */

#include <string.h>
#include <unistd.h> 

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

int x = 20, y = 22;
int incX = 1, incY = 1;
int bulletX = -1, bulletY = -1;
int bulletSpeed = 100000;

int direction = 'r'; // Direção inicial: 'r' (direita)
int drop = 0; // Necessidade de descer
int enemySpeed = 10;
int i = 0; 

int startX = 7, startY = 3;
struct enemies{
  char m;
  int vivo;
};

struct enemies enemy[5][10];

void matrizglobal(){
   for(int i=0;i<5;i++){
     for(int j=0; j<10; j++){
        enemy[i][j].m = 'M';
        enemy[i][j].vivo = 1;
     }
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
    
    if(colisaoInimigo(bulletX, bulletY - i)){
      break;
    }
    
    if (keyhit()) {
      char ch = readch();
      if(ch == 'a' || ch == 'A'|| ch == 'd' || ch == 'D' ){

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
  int enemyWidth = 3, enemyHeight = 2;
  
  if (direction == 'l') {
    startX -= 1; // Move para a esquerda
  } else {
    startX+= 1; // Move para a direita
  }

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      movimentar(ch);
      screenGotoxy(startX + j * 3,
                   startY + i * 2); // Posiciona-se para imprimir o inimigo atual
      printf("  %c", enemy[i][j].m); // Imprime o inimigo atual
      printf("  ");
    }
  }
}

int colisaoInimigo(int bulletX, int bulletY){
  
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 10; j++) {
      if(bulletX == startX + j * 3 && bulletY ==startY + i * 2 && enemy[i][j].vivo == 1){

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

int main() {
  static int ch = 0;
  
  screenInit(1);
  keyboardInit();
  timerInit(50);

  screenUpdate();

  matrizglobal();

  while (ch != 10) // enter
  {
    // Handle user input
    if (keyhit()) {
      ch = readch();

      movimentar(ch);
      printHello(x, y);
      screenUpdate();
    }
    enemies();
    usleep(500000);
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}
