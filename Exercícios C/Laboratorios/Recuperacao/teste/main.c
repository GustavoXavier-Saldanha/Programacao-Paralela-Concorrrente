/*
* © Elias Bernardo & Erick Giffoni - 2020
* Universidade de Brasilia - UnB
* Engenharia de Software - Faculdade do Gama
* Projeto de Algoritmos - Trabalho 3: Greed
* Repo URL : TODO
* Problem : The Closest Pair Problem, URI 1295
* compile : $ gcc closest_pair.c -O2 -lm -o closest.out
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

/*
Algorithm:
    Divide: draw vertical line L so that roughly 1⁄2n points on each side.
    Conquer: find closest pair in each side recursively.
    Combine: find closest pair with one point in each side.
    Return best of 3 solutions.

Reference : Kleinberge, Jon; Tardos, Eva. Algorithm Design. Pearson. Chapter 5 : Divide and Conquer.
*/

typedef struct ponto {
    int x;
    int y;
}Ponto;

float euclidiana(Ponto * a, Ponto * b);
void merge(Ponto *vet, int esq, int meio, int dir, int x_or_y);
void mergeSort(Ponto *vet, int esq, int dir, int x_or_y);
float encontra_pontos_mais_proximos(Ponto * pontos, int esq, int dir);
Ponto mediana(Ponto * pontos, int esq, int dir);

int main(){

    int N;
    Ponto * pontos = (Ponto *) calloc(10000, sizeof(Ponto));
    float menorDistancia;
    while(1){
        scanf(" %d", &N);
        if(N == 0)
            break;
        for(int i=0; i<N; i++){ //leitura dos pontos
            scanf(" %d", &pontos[i].x);
            scanf(" %d", &pontos[i].y);
        }//end for leitura dos pontos

        //achar menor distancia
        mergeSort(pontos, 0, N-1, 0); // ordena todos em x
        menorDistancia = encontra_pontos_mais_proximos(pontos, 0, N -1);

        //print resultado
        if(menorDistancia < 10000)
            printf("%.4f\n", menorDistancia);
        else
            printf("INFINITY\n");

    }//end while
    free(pontos);

    return 0;
}//end main

float encontra_pontos_mais_proximos(Ponto * pontos, int esq, int dir){
    // precisa de condicao de parada ?? se esq >= dir return ?? dir - esq == 1

    int meio = ceil((esq + dir)/2.0); //'linha' de separacao
    //if(meio == esq)
    //    meio++;

    if (dir-esq == 0)
        return 10000;
    else if(dir-esq == 1){
        //printf("entrei\nesq %d dir %d\n", esq, dir);
        merge(pontos, esq, meio, dir, 1);
        return euclidiana(&pontos[esq], &pontos[dir]);
    }

    float menorDistEsquerda = encontra_pontos_mais_proximos(pontos, esq, meio);
    float menorDistDireita = encontra_pontos_mais_proximos(pontos, meio + 1 , dir);

    float menorDistanciaOficial = menorDistEsquerda < menorDistDireita ? menorDistEsquerda : menorDistDireita;

    Ponto _mediana = mediana(pontos, esq, dir); //pegar a mediana de x aqui
    merge(pontos, esq, meio, dir, 1); //merge em y

    Ponto * pontosAUX = (Ponto *) calloc(dir-esq+1, sizeof(Ponto)); // S'

    int posicaoAUX = 0; //qtd elementos em S'

    //deletar pontos cuja distancia para o meio seja maior que a menor distancia ja calculada
    for(int i=esq; i<=dir; i++){
        _mediana.y = pontos[i].y;

        float tempEuclidiana = euclidiana(&pontos[i], &_mediana);

        if(tempEuclidiana < menorDistanciaOficial){
            pontosAUX[posicaoAUX] = pontos[i];
            posicaoAUX++;
        }//end if excluir ponto
    }//end for deleta

    // scan S' em y e comparar distancia com ate 7 vizinhos, atualizar menorDistanciaOficial se for o caso
    float _euclid; //aux para comparar a euclidiana entre os pontos

    for(int i=esq; i<=posicaoAUX; i++){
        for(int j=i+1, conta=0; j < posicaoAUX && conta < 7; j++, conta++){
            _euclid = euclidiana(&pontosAUX[i], &pontosAUX[j]);
            if(_euclid < menorDistanciaOficial)
                menorDistanciaOficial = _euclid;
        }//end for compara ate 7 vizinhos
    }//end for scan e compara pontos
    free(pontosAUX);
    return menorDistanciaOficial;
}//end encontra ponto mais proximo

Ponto mediana(Ponto * pontos, int esq, int dir){
    // essa funcao encontra a mediana em x, considerando que o vetor esta ordenado
    Ponto mediana;

    int meio = (esq + dir);

    if(meio % 2 != 0) //se for impar, round pra cima
        meio = (int) ceil(meio/2.0);
    else
        meio = meio/2;
    mediana.x = pontos[meio].x;
    mediana.y = 0; //default
    return mediana;
}//end mediana

float euclidiana(Ponto *a, Ponto *b){
    int x = b->x - a->x;
    int y = b->y - a->y;
    float hipotenusa = sqrt( ( powf((float)x, 2.0) + powf((float)y, 2.0) ) );
    return hipotenusa;
}//end euclidiana

void merge(Ponto *vet, int esq, int meio, int dir, int x_or_y){
    // x_or_y : 0 para ordenar em x, 1 para ordenar em y
    Ponto *aux = (Ponto *)calloc((dir - esq + 1), sizeof(Ponto));
    int k = 0, i = esq, j = meio + 1;
    switch (x_or_y){
    case 0:
        while (i <= meio && j <= dir){
            if (vet[i].x <= vet[j].x) //assim fica estavel
                aux[k++] = vet[i++];
            else
                aux[k++] = vet[j++];
        } //end while
        while (i <= meio)
            aux[k++] = vet[i++];
        while (j <= dir)
            aux[k++] = vet[j++];

        for (i = esq; i <= dir; i++)
            vet[i] = aux[k++];
        break;

    case 1:
        while (i <= meio && j <= dir){
            if (vet[i].y <= vet[j].y) //assim fica estavel
                aux[k++] = vet[i++];
            else
                aux[k++] = vet[j++];
        } //end while
        while (i <= meio)
            aux[k++] = vet[i++];
        while (j <= dir)
            aux[k++] = vet[j++];

        k = 0;
        for (i = esq; i <= dir; i++)
            vet[i] = aux[k++];
        break;

    default:
        break;
    } //end switch

    free(aux);
} //end merge

void mergeSort(Ponto *vet, int esq, int dir, int x_or_y){
    // x_or_y : 0 para ordenar em x, 1 para ordenar em y
    if (esq >= dir)
        return;
    int meio = (dir + esq) / 2;
    mergeSort(vet, esq, meio, x_or_y);
    mergeSort(vet, meio + 1, dir, x_or_y);
    merge(vet, esq, meio, dir, x_or_y);
}//end mergesort



/* Codigos de TESTE

    // ordenar e print
        mergeSort(pontos, 0, N-1, 1);
        for(int i=0; i<N; i++)
            printf("%d %d\n", pontos[i].x, pontos[i].y);

*/
