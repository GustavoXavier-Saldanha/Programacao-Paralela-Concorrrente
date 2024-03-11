#include <stdio.h>
#include <stdlib.h>

void exibirMatriz(int **matriz, int linhas, int colunas) {
    int i, j;
    for (i = 0; i < linhas; i++) {
        for (j = 0; j < colunas; j++) {
            printf("%d ", matriz[i][j]);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <tamanho> <elementos_matriz1> <elementos_matriz2>\n", argv[0]);
        return 1;
    }

    int tamanho = atoi(argv[1]);
    if (argc != 2 + tamanho * tamanho * 2) {
        printf("Número de elementos fornecidos não corresponde ao tamanho especificado.\n");
        return 1;
    }

    int **matriz1 = (int **)malloc(tamanho * sizeof(int *));
    int **matriz2 = (int **)malloc(tamanho * sizeof(int *));
    int **resultado = (int **)malloc(tamanho * sizeof(int *));
    int i, j, k;
    for (i = 0; i < tamanho; i++) {
        matriz1[i] = (int *)malloc(tamanho * sizeof(int));
        matriz2[i] = (int *)malloc(tamanho * sizeof(int));
        resultado[i] = (int *)malloc(tamanho * sizeof(int));
    }

    k = 0;
    for (i = 0; i < tamanho; i++) {
        for (j = 0; j < tamanho; j++) {
            matriz1[i][j] = atoi(argv[k + 2]);
            k++;
        }
    }

    for (i = 0; i < tamanho; i++) {
        for (j = 0; j < tamanho; j++) {
            matriz2[i][j] = atoi(argv[k + 2]);
            k++;
        }
    }

    printf("Primeira matriz (%dx%d):\n", tamanho, tamanho);
    exibirMatriz(matriz1, tamanho, tamanho);

    printf("Segunda matriz (%dx%d):\n", tamanho, tamanho);
    exibirMatriz(matriz2, tamanho, tamanho);

    for (i = 0; i < tamanho; i++) {
        for (j = 0; j < tamanho; j++) {
            resultado[i][j] = 0;
            for (k = 0; k < tamanho; k++) {
                resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }

    printf("Resultado da multiplicação das matrizes:\n");
    exibirMatriz(resultado, tamanho, tamanho);

    for (i = 0; i < tamanho; i++) {
        free(matriz1[i]);
        free(matriz2[i]);
        free(resultado[i]);
    }
    free(matriz1);
    free(matriz2);
    free(resultado);

    return 0;
}
