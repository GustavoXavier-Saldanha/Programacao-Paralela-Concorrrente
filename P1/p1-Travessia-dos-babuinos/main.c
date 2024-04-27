#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_BABUINOS 1000
#define INTERVALO_MIN 1000 // 1ms em microssegundos
#define INTERVALO_MAX 4000 // 4ms em microssegundos

pthread_mutex_t mutex_corda = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_contagem_leste = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_contagem_oeste = PTHREAD_MUTEX_INITIALIZER;
int babuinos_leste_atuais = 0;
int babuinos_oeste_atuais = 0;
int MAX_BABUINOS_LESTE = 0;
int MAX_BABUINOS_OESTE = 0;

typedef struct {
    int numero;
    char direcao;
    int estado; // 0 para esperando, 1 para atravessando
} Babuino;

void babuino_quer_atravessar(Babuino *babuino) {
    printf("Babu�no %d quer atravessar para %s\n", babuino->numero, babuino->direcao == 'L' ? "LESTE" : "OESTE");
}

void pegar_corda(Babuino *babuino) {
    printf(">>> BABU�NO %d est� atravessando para %s\n", babuino->numero, babuino->direcao == 'L' ? "LESTE" : "OESTE");
    usleep(rand() % (INTERVALO_MAX - INTERVALO_MIN + 1) + INTERVALO_MIN);
}

void soltar_corda(Babuino *babuino) {
    printf(" BABU�NO %d chegou!\n", babuino->numero);
}

void *babuino_function(void *arg) {
    Babuino *babuino = (Babuino *)arg;

    while (1) {
        babuino_quer_atravessar(babuino);

        pthread_mutex_lock(&mutex_corda);

        // Verificar se � seguro atravessar a corda
        if ((babuino->direcao == 'L' && babuinos_oeste_atuais == 0) || (babuino->direcao == 'O' && babuinos_leste_atuais == 0)) {
            // Se for seguro, pegar a corda
            pegar_corda(babuino);

            // Atualizar o contador de babu�nos na corda para essa dire��o
            if (babuino->direcao == 'L') {
                babuinos_leste_atuais++;
            } else {
                babuinos_oeste_atuais++;
            }

            // Soltar a corda ap�s atravessar
            soltar_corda(babuino);

            // Atualizar o contador de babu�nos na corda para essa dire��o
            if (babuino->direcao == 'L') {
                babuinos_leste_atuais--;
            } else {
                babuinos_oeste_atuais--;
            }
        } else {
            // Se n�o for seguro atravessar, fa�a o babu�no esperar
            pthread_mutex_unlock(&mutex_corda);
            usleep(rand() % (INTERVALO_MAX - INTERVALO_MIN + 1) + INTERVALO_MIN);
            continue;
        }

        pthread_mutex_unlock(&mutex_corda);

        usleep(rand() % (INTERVALO_MAX - INTERVALO_MIN + 1) + INTERVALO_MIN); // Intervalo aleat�rio entre as tentativas de atravessar
    }

    return NULL;
}

void *gerar_babuinos(void *arg) {
    int *quantidade = (int *)arg;
    char direcao = *((char *)arg + sizeof(int));

    for (int i = 0; i < *quantidade; i++) {
        Babuino *babuino = (Babuino *)malloc(sizeof(Babuino));
        if (babuino == NULL) {
            perror("Erro ao alocar mem�ria para o babu�no");
            exit(EXIT_FAILURE);
        }

        babuino->numero = i * 2 + (direcao == 'L' ? 0 : 1);
        babuino->direcao = direcao;
        babuino->estado = 0; // Esperando

        pthread_t babuino_thread;
        if (pthread_create(&babuino_thread, NULL, babuino_function, (void *)babuino) != 0) {
            perror("Erro ao criar a thread do babu�no");
            exit(EXIT_FAILURE);
        }

        usleep(rand() % (INTERVALO_MAX - INTERVALO_MIN + 1) + INTERVALO_MIN); // Intervalo aleat�rio entre a cria��o de babu�nos
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso: %s <quantidade_babuinos>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int quantidade = atoi(argv[1]);
    if (quantidade <= 0 || quantidade > MAX_BABUINOS) {
        printf("Quantidade inv�lida de babu�nos. Por favor, insira um valor entre 1 e %d.\n", MAX_BABUINOS);
        exit(EXIT_FAILURE);
    }

    // Definir o n�mero m�ximo de babu�nos permitidos em cada dire��o
    MAX_BABUINOS_LESTE = quantidade;
    MAX_BABUINOS_OESTE = quantidade;

    srand(time(NULL)); // Inicializa a semente para gera��o de n�meros aleat�rios

    pthread_t gerador_leste, gerador_oeste;

    char direcao_leste = 'L';
    if (pthread_create(&gerador_leste, NULL, gerar_babuinos, &quantidade) != 0) {
        perror("Erro ao criar a thread do gerador de babu�nos para LESTE");
        exit(EXIT_FAILURE);
    }

    char direcao_oeste = 'O';
    if (pthread_create(&gerador_oeste, NULL, gerar_babuinos, &quantidade) != 0) {
        perror("Erro ao criar a thread do gerador de babu�nos para OESTE");
        exit(EXIT_FAILURE);
    }

    pthread_join(gerador_leste, NULL);
    pthread_join(gerador_oeste, NULL);

    return 0;
}
