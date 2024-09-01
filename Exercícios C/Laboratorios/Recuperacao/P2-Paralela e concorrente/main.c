#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void matrixMultiplySequential(int **A, int **B, int **C, int N) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

void matrixMultiplyParallel(int **A, int **B, int **C, int N, int num_threads) {
    omp_set_num_threads(num_threads);
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0; // Dependência de dados: Modificação de C[i][j]
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j]; // Dependência de dados: Acesso a A, B e C
            }
        }
    }
}

int main() {
    int N = 100;

    int **A = (int **)malloc(N * sizeof(int *));
    int **B = (int **)malloc(N * sizeof(int *));
    int **C_seq = (int **)malloc(N * sizeof(int *));
    int **C_par = (int **)malloc(N * sizeof(int *));

    for (int i = 0; i < N; i++) {
        A[i] = (int *)malloc(N * sizeof(int));
        B[i] = (int *)malloc(N * sizeof(int));
        C_seq[i] = (int *)malloc(N * sizeof(int));
        C_par[i] = (int *)malloc(N * sizeof(int));
        for (int j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
        }
    }

    double start_seq = omp_get_wtime();
    matrixMultiplySequential(A, B, C_seq, N);
    double end_seq = omp_get_wtime();
    double time_taken_seq = end_seq - start_seq;

    double start_parallel = omp_get_wtime();
    matrixMultiplyParallel(A, B, C_par, N, 8);
    double end_parallel = omp_get_wtime();
    double time_taken_parallel = end_parallel - start_parallel;

    int equal = 1;
    for (int i = 0; i < N && equal; i++) {
        for (int j = 0; j < N; j++) {
            if (C_seq[i][j] != C_par[i][j]) {
                equal = 0;
                break;
            }
        }
    }

    if (equal) {
        printf("\nAs matrizes resultantes são iguais!\n");
    } else {
        printf("\nAs matrizes resultantes são diferentes!\n");
    }

    printf("\nTempo sequencial: %f segundos\n", time_taken_seq);
    printf("Tempo paralelo: %f segundos\n", time_taken_parallel);

    double speedup = time_taken_seq / time_taken_parallel;
    double efficiency = speedup / omp_get_max_threads();

    printf("\nSpeedup: %f\n", speedup);
    printf("Efficiency: %f\n", efficiency > 1 ? 1.0 : efficiency);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C_seq[i]);
        free(C_par[i]);
    }

    free(A);
    free(B);
    free(C_seq);
    free(C_par);

    return 0;
}
