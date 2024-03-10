#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int soma = 0;

    for (int i = 1; i < argc; i++) {
        int aux = atoi(argv[i]);
        soma += aux;
    }

    float media = (float)soma / (argc - 1);
    printf("Média: %.2f\n", media);

    return 0;
}
