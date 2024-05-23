#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

int x = 20, y = 22;
int prevx, prevy;
int incX = 1, incY = 1;

int bulletX = 0, bulletY = 0;
int bulletSpeed = 10000;
int balaativa = 1;

int enemybulletX = 0, enemybulletY = 0;
int enemybalaativa = 1;
int enemybalacooldown = 0;

int startX = 7, startY = 3;

int descer = 0;
static int direction = 1; // 1 para direita, -1 para esquerda

int score = 0;
int high_score = 0;

struct enemies {
  char m;
  int vivo;
  int death;
};

int enemymoviment;

struct enemies enemy[4][10];

void matrizglobal() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      enemy[i][j].m = 'M';
      enemy[i][j].vivo = 1;
    }
  }
}

int colisaoInimigo() {
  int x = 7, y = 3;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      if ((bulletX == startX + j * 3 || bulletX == (startX + j * 3) + 1 ||
           bulletX == (startX + j * 3) - 1) &&
          (bulletY == startY + i * 2 || bulletY == (startY + i * 2) - 1 ||
           bulletY == (startY + i * 2) + 1) &&
            enemy[i][j].vivo == 1) {
            enemy[i][j].vivo = 0;
            enemy[i][j].m = 'X';
            enemy[i][j].death = 5;
            score=score+50;
            return 1;
      }
    }
  }

  return 0;
}

void enemyBulletSpawn() {
  if (enemybalaativa == 0)
    return;

  srand(time(NULL));

  int randI = rand() % 10;
  int randJ = rand() % 4;

  if (enemy[randJ][randI].vivo) {

    enemybalaativa = 0;
    enemybulletX = startX + randI * 3;
    enemybulletY = startX + randJ * 2;

    screenGotoxy(enemybulletX, enemybulletY);
  }
}

void enemyShoot() {
  screenGotoxy(enemybulletX, enemybulletY);
  printf(" ");

  enemybulletY += 1;

  if (enemybulletY == 24) {
    enemybalaativa = 1;
    return;
  }

  screenSetColor(RED, DARKGRAY);
  screenGotoxy(enemybulletX, enemybulletY);
  colisaoInimigo();
  printf("*");
}

void printBullet() {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(bulletX, bulletY);

  screenGotoxy(bulletX, bulletY);
  printf("  ");

  bulletY -= 1;

  if (colisaoInimigo(bulletX, bulletY)) {
    screenGotoxy(bulletX, bulletY);
    printf("  ");
    balaativa = 1;
    return;
  }

  if (bulletY == 1) {
    balaativa = 1;
    return;
  }

  screenGotoxy(bulletX, bulletY);
  printf("^");
  usleep(bulletSpeed);

  screenSetColor(CYAN, DARKGRAY);
}

void enemies() {
  if (enemymoviment > 0) {
    enemymoviment--;
    return;
  }
  enemymoviment = 3;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      if ((enemy[i][j].vivo && direction == -1 && startX <= 2) ||
          (enemy[i][j].vivo && direction == 1 && startX + 9 * 3 >= 34)) {

        screenGotoxy(startX + 1, startY);
        direction *= -1; // Mudar direção
        break;
      }
    }
  }

  // Atualizar a posição dos inimigos
  startX += direction;
  enemyBulletSpawn();

  // Imprimir os inimigos na nova posição
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {

      if (enemy[i][j].vivo) { // Imprimir apenas inimigos vivos

        screenGotoxy(startX + j * 3, startY + i * 2);
        printf("  %c  ", enemy[i][j].m);

      } else {
        screenGotoxy(startX + j * 3, startY + i * 2);
        if (enemy[i][j].vivo == 0 && enemy[i][j].death > 0) {
          printf(" %c ", enemy[i][j].m);
          enemy[i][j].death -= 1;
        } else
          printf("    ");
      }
    }
  }
}

void printHello() {
  static int ch = 0;
  screenGotoxy(prevx, prevy);
  printf("  ");
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" △ ");

  prevx = x;
  prevy = y;
}

void colisaoComNave() {
  if ((x == enemybulletX || x == enemybulletX - 1 || x == enemybulletX + 1) && (y == enemybulletY || y == enemybulletY - 1 || y == enemybulletY + 1 )) {
    screenGotoxy(20, 10);
    
    screenInit();

    printf("GAME OVER\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    exit(1);

  }
}

int main() {
  static int ch = 0;

  screenInit(1);
  keyboardInit();
  timerInit(50);

  screenUpdate();

  FILE *file = fopen("highscore.txt", "r");
  if (file != NULL) fscanf(file, "%d", &high_score);
  fclose(file);

  matrizglobal();

  while (ch != 10) // enter
  {
    colisaoComNave();
    // Handle user input
    if (keyhit()) {
      ch = readch();
    }
    if (timerTimeOver() == 1) {

      // movimentar
      if (ch == 'a' || ch == 'A') {
        if (x > 2) { // Verifica se não está no limite esquerdo
          x -= 2;
        }
        ch = 0;
      } else if (ch == 'd' || ch == 'D') {
        if (x < 36) { // Verifica se não está no limite direito
          x += 2;
        }
        ch = 0;
      }

      if (ch == ' ' && balaativa == 1) {
        bulletX = x;
        bulletY = y - 1;
        balaativa = 0;
        ch = 0;
      }
      screenGotoxy(2, 2);
      printf(" %d ", enemybalaativa);

      printHello();
      if (balaativa == 0)
        printBullet();
      enemies();
      if (enemybalaativa == 0)
        enemyShoot();
      screenUpdate();
    }
  }

  if (score>high_score){
    file = fopen("highscore.txt", "w");
    if(file != NULL) fprintf(highscore,"%d",score);
    fclose(file);
  }

  

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  return 0;
}