#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"


struct Enemy {
  char m;    
  int vivo;  
  int death; 
};


int x = 20, y = 22;                     
int prevx, prevy;                       
int bulletX = 0, bulletY = 0;           
int bulletSpeed = 10000;                
int balaativa = 1;                      
int enemybulletX = 0, enemybulletY = 0; 
int enemybalaativa = 1;                 
int startX = 7, startY = 3;             
int direction = 1; 
int score = 0;     
int high_score;    
int multiplicador = 10;  
int contadorVictory = 0; 


struct Enemy **enemies;


void matrizglobal();
void freeEnemies();
void enemies_func();
void printHello();
void colisaoComNave();
void victory();
void showScore();


int colisaoInimigo();
void enemyBulletSpawn();
void enemyShoot();
void printBullet();


void matrizglobal() {
  enemies = (struct Enemy **)malloc(4 * sizeof(struct Enemy *));
  for (int i = 0; i < 4; i++) {
    enemies[i] = (struct Enemy *)malloc(10 * sizeof(struct Enemy));
    for (int j = 0; j < 10; j++) {
      enemies[i][j].m = 'M';
      enemies[i][j].vivo = 1;
      enemies[i][j].death = 0;
    }
  }
}


void freeEnemies() {
  for (int i = 0; i < 4; i++) {
    free(enemies[i]);
  }
  free(enemies);
}


int colisaoInimigo() {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      if ((bulletX == startX + j * 3 || bulletX == (startX + j * 3) + 1 ||
           bulletX == (startX + j * 3) + 2) &&
          bulletY == startY + i * 2 && (enemies[i][j].vivo == 1)) {
        enemies[i][j].vivo = 0;
        enemies[i][j].m = 'X';
        enemies[i][j].death = 5;
        score += 50;
        contadorVictory++;
        return 1;
      }
    }
  }
  return 0;
}


void enemyBulletSpawn() {
  if (!enemybalaativa)
    return;

  srand(time(NULL));
  int randI = rand() % 10;
  int randJ = rand() % 4;

  if (enemies[randJ][randI].vivo) {
    enemybalaativa = 0;
    enemybulletX = startX + randI * 3;
    enemybulletY = startX + randJ * 2;
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
  colisaoComNave();
  printf("*");
}


void printBullet() {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(bulletX, bulletY);
  printf("  ");

  bulletY -= 1; 

  if (colisaoInimigo(bulletX, bulletY)) {
    screenGotoxy(bulletX, bulletY);
    printf("  ");
    balaativa = 1;
    bulletX = 0;
    bulletY = 0;
    return;
  }

  if (bulletY == 1) {
    balaativa = 1;
    return;
  }

  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(bulletX, bulletY);
  printf("^");

  screenSetColor(CYAN, DARKGRAY);
}


void enemies_func() {
  static int enemymoviment = 0;

  if (enemymoviment > 0) {
    enemymoviment--;
    return;
  }
  enemymoviment = 3;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      if ((enemies[i][j].vivo && direction == -1 && startX <= 2) ||
          (enemies[i][j].vivo && direction == 1 && startX + 9 * 3 >= 34)) {
        screenGotoxy(startX + 1, startY);
        direction *= -1; 
        break;
      }
    }
  }

  startX += direction;
  enemyBulletSpawn();

  screenSetColor(RED, DARKGRAY);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 10; j++) {
      if (enemies[i][j].vivo) {
        screenGotoxy(startX + j * 3, startY + i * 2);
        printf("  %c  ", enemies[i][j].m);
      } else {
        screenGotoxy(startX + j * 3, startY + i * 2);
        if (enemies[i][j].death > 0) {
          printf(" %c ", enemies[i][j].m);
          enemies[i][j].death--;
        } else {
          printf("    ");
        }
      }
    }
  }
}


void printHello() {
  screenGotoxy(prevx, prevy);
  printf("  ");
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" △ ");
  prevx = x;
  prevy = y;
}


void colisaoComNave() {
  if ((x == enemybulletX || x == (enemybulletX + 1) ||
       x == (enemybulletX - 1)) &&
      y == enemybulletY) {
    score *= multiplicador;
    if (score > high_score) {
      FILE *file = fopen("recorde.txt", "w");
      if (file != NULL) {
        fprintf(file, "%d", score);
        fclose(file);
        high_score = score;
      }
    }

    screenClear();
    screenGotoxy(20, 10);
    printf("GAME OVER");
    screenGotoxy(20, 12);
    printf("SCORE: %d", score);
    screenGotoxy(20, 14);
    printf("HIGH SCORE: %d\n\n\n\n\n\n\n\n\n\n", high_score);
    freeEnemies();
    exit(0);
  }
}


void victory() {
  if (contadorVictory == 40) {
    score *= multiplicador;

    if (score > high_score) {
      FILE *file = fopen("recorde.txt", "w");
      if (file != NULL) {
        fprintf(file, "%d", score);
        fclose(file);
        high_score = score;
      }
    }

    screenSetColor(CYAN, DARKGRAY);
    screenClear();
    screenGotoxy(20, 10);
    printf("VICTORY");
    screenGotoxy(20, 12);
    printf("SCORE: %d", score);
    screenGotoxy(20, 14);
    printf("HIGH SCORE: %d\n\n\n\n\n\n\n\n\n\n", high_score);
    freeEnemies();
    exit(0);
  }
}


void showScore() {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(2, 0);
  printf("Score: %d ", score);
}


int main() {
  int ch = 0;
  screenInit(1);
  keyboardInit();
  timerInit(150);
  screenUpdate();

  FILE *file = fopen("recorde.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }

  matrizglobal();

  while (ch != 10) { 
    colisaoComNave();
    victory();

    if (keyhit()) {
      ch = readch();
    }

    if (timerTimeOver() == 1) {

      showScore();


      if (ch == 'a' || ch == 'A') {
        if (x > 2) {
          x -= 2;
        }
        ch = 0;
      } else if (ch == 'd' || ch == 'D') {
        if (x < 36) {
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


      printHello();


      if (balaativa == 0) {
        printBullet();
      }


      enemies_func();


      if (enemybalaativa == 0) {
        enemyShoot();
      }

      screenUpdate();
    }
  }

  keyboardDestroy();
  screenDestroy();
  timerDestroy();

  freeEnemies();

  return 0;
}