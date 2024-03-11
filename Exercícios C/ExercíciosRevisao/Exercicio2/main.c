#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    int matriz1[2][2], matriz2[2][2], resultado[2][2];
    int i, j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            matriz1[i][j] = atoi(argv[i * 2 + j + 1]);
        }
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            matriz2[i][j] = atoi(argv[i * 2 + j + 5]);
        }
    }

    printf("Primeira matriz (2x2):\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%d ", matriz1[i][j]);
        }
        printf("\n");
    }

    printf("Segunda matriz (2x2):\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%d ", matriz2[i][j]);
        }
        printf("\n");
    }

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            resultado[i][j] = matriz1[i][0] * matriz2[0][j] + matriz1[i][1] * matriz2[1][j];
        }
    }

    printf("Resultado da multiplicação das matrizes:\n");
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            printf("%d ", resultado[i][j]);
        }
        printf("\n");
    }

    printf("teste: %d - %d - %d  - %d :", matriz1[0][0], matriz1[0][1], matriz1[1][0],matriz1[1][1]);


    return 0;
}
