#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "keyboard.h"
#include "screen.h"
#include "timer.h"

// Estrutura de um inimigo
struct Enemy {
  char m;    // Caractere para representar o inimigo na tela
  int vivo;  // 1 se o inimigo está vivo, 0 se está morto
  int death; // Contador para efeito de morte animada
};

// Variáveis globais
int x = 20, y = 22;                     // Posição da nave
int prevx, prevy;                       // Posição anterior da nave
int bulletX = 0, bulletY = 0;           // Posição da bala
int bulletSpeed = 10000;                // Velocidade da bala
int balaativa = 0;                      // Flag se a bala está ativa
int enemybulletX = 0, enemybulletY = 0; // Posição da bala do inimigo
int enemybalaativa = 0;                 // Flag se a bala do inimigo está ativa
int startX = 7, startY = 3;             // Posição inicial dos inimigos
int direction = 1; // Direção dos inimigos (1 para direita, -1 para esquerda)
int score = 0;     // Pontuação do jogo
int high_score;    // Recorde de pontuação
int multiplicador = 10;  // Multiplicador da pontuação
int contadorVictory = 0; // Contador de inimigos mortos

// Matriz global de inimigos
struct Enemy **enemies;

// Funções principais
void matrizglobal();
void freeEnemies();
void enemies();
void printHello();
void colisaoComNave();
void victory();
void showScore();

// Funções auxiliares
int colisaoInimigo();
void enemyBulletSpawn();
void enemyShoot();
void printBullet();

// Inicializa a matriz de inimigos
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

// Libera a memória alocada para a matriz de inimigos
void freeEnemies() {
  for (int i = 0; i < 4; i++) {
    free(enemies[i]);
  }
  free(enemies);
}

// Verifica se houve colisão da bala com algum inimigo
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

// Faz o spawn de uma bala do inimigo
void enemyBulletSpawn() {
  if (enemybalaativa)
    return;

  srand(time(NULL));
  int randI = rand() % 10;
  int randJ = rand() % 4;

  if (enemies[randJ][randI].vivo) {
    enemybalaativa = 1;
    enemybulletX = startX + randI * 3;
    enemybulletY = startX + randJ * 2;
  }
}

// Movimenta e imprime a bala do inimigo
void enemyShoot() {
  screenGotoxy(enemybulletX, enemybulletY);
  printf(" ");

  enemybulletY += 1;

  if (enemybulletY == 24) {
    enemybalaativa = 0;
    return;
  }

  screenSetColor(RED, DARKGRAY);
  screenGotoxy(enemybulletX, enemybulletY);
  colisaoComNave();
  printf("*");
}

// Movimenta e imprime a bala da nave
void printBullet() {
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(bulletX, bulletY);
  printf("  ");

  bulletY -= 1; // Move a bala para cima

  if (colisaoInimigo(bulletX, bulletY)) {
    screenGotoxy(bulletX, bulletY);
    printf("  ");
    balaativa = 0;
    bulletX = 0;
    bulletY = 0;
    return;
  }

  if (bulletY == 1) {
    balaativa = 0;
    return;
  }

  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(bulletX, bulletY);
  printf("^");

  screenSetColor(CYAN, DARKGRAY);
}

// Movimenta e imprime os inimigos
void enemies() {
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
        direction *= -1; // Muda a direção
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

// Imprime a nave do jogador
void printHello() {
  screenGotoxy(prevx, prevy);
  printf("  ");
  screenSetColor(CYAN, DARKGRAY);
  screenGotoxy(x, y);
  printf(" △ ");
  prevx = x;
  prevy = y;
}

// Verifica colisão da nave com a bala do inimigo
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

// Verifica se o jogador ganhou o jogo
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

// Imprime a pontuação na tela
void showScore() {
  screenSetColor(YELLOW, DARKGRAY);
  screenGotoxy(2, 0);
  printf("Score: %d ", score);
}

// Função principal
int main() {
  int ch = 0;
  screenInit(1);
  keyboardInit();
  timerInit(50);
  screenUpdate();

  FILE *file = fopen("recorde.txt", "r");
  if (file != NULL) {
    fscanf(file, "%d", &high_score);
    fclose(file);
  }

  matrizglobal();

  while (ch != 10) { // Enter
    colisaoComNave();
    victory();

    if (keyhit()) {
      ch = readch();
    }

    if (timerTimeOver() == 1) {
      // Atualiza a pontuação
      showScore();

      // Movimenta a nave do jogador
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

      // Dispara a bala da nave
      if (ch == ' ' && balaativa == 0) {
        bulletX = x;
        bulletY = y - 1;
        balaativa = 1;
        ch = 0;
      }

      // Imprime a nave do jogador
      printHello();

      // Imprime a bala do jogador se estiver ativa
      if (balaativa == 1) {
        printBullet();
      }

      // Movimenta e imprime os inimigos
      enemies();

      // Faz o inimigo atirar
      if (enemybalaativa) {
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