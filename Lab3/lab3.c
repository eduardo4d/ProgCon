/*
 * Computação Concorrente - 2022.2 - UFRJ
 * Profa. Silvana Rossetto
 * Aluno: Eduardo Kota Otomo
 * DRE: 118147443
 * Laboratório 3: Multiplicação de matrizes, concorrente
 */

#include<stdio.h>
#include<stdlib.h>
#include "timer.h"
#include<pthread.h>

/*matrizes A*B = C */
float *matrizA, *matrizB, *matrizC;

int nthreads;
pthread_t *tid;

typedef struct {
   int id, dim;
} t_Args;


/* Multiplica matrizes*/
void * multiplica (void * arg){
    t_Args * args = (t_Args*) arg;
    int i, j, k;
    for(i = args->id; i < args->dim; i+=nthreads) {
        for(j = 0; j < args->dim; j++) {
            for(k = 0; args->dim; k++) {
                matrizC[i* args->dim + j] +=
                    matrizA[i* args->dim  + k] * matrizB[k* args->dim + j];
            }
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char* argv[]){

    t_Args *args;

    /*dimensoes das matrizes A e B*/
    int linhasA, colunasA, linhasB, colunasB;

    /*quantidade de elementos nas matrizes*/
    long long tamA, tamB, tamC;

    /*descritores dos arquivo*/
    FILE *arq1, *arq2, *arq3;
    /*retorno da funcao de escrita ou leitura nos arquivos*/
    size_t ret1, ret2, ret3;

    double inicio, fim, delta[4]; /*tempos*/

    GET_TIME(inicio);
    /*avalia argumentos de entrada*/
    if(argc < 4){
        printf("Digite: %s <arquivo entrada1> <arquivo entrada2> <arquivo saida>\n", argv[0]);
        return 1;
    }
    int dim = atoi(argv[1]); /*dimensão da matriz de entrada*/


    /*abre o arquivos*/
    arq1 = fopen(argv[1], "rb");
    if(!arq1) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }
    arq2 = fopen(argv[2], "rb");
    if(!arq2) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 2;
    }

    /*le as dimensoes das matrizes*/
    ret1 = fread(&linhasA, sizeof(int), 1, arq1);
    if(!ret1) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 1\n");
        return 3;
    }
    ret1 = fread(&colunasA, sizeof(int), 1, arq1);
    if(!ret1) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 1\n");
        return 3;
    }
    ret2 = fread(&linhasB, sizeof(int), 1, arq2);
    if(!ret2) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 2\n");
        return 3;
    }
    ret2 = fread(&colunasB, sizeof(int), 1, arq2);
    if(!ret2) {
        fprintf(stderr, "Erro de leitura das dimensoes da matriz arquivo 2\n");
        return 3;
   }

    /*calcula a qtde de elementos da matrizes A,B,C*/
    tamA = linhasA * colunasA;
    tamB = linhasB * colunasB;
    tamC = linhasA * colunasB;

    /*aloca memória para as matrizes */
    matrizA = (float*) malloc(tamA * sizeof(float));
    matrizB = (float*) malloc(tamB * sizeof(float));
    matrizC = (float*) calloc(tamC, sizeof(float));
    if(!matrizA || !matrizB || !matrizC) {
        fprintf(stderr, "Erro de alocao da memoria da matriz\n");
        return 3;
    }

    /*carrega matrizes do arquivo*/
    ret1 = fread(matrizA, sizeof(float), tamA, arq1);
    ret2 = fread(matrizB, sizeof(float), tamB, arq2);
    if(ret1 < tamA || ret2 < tamB) {
        fprintf(stderr, "Erro de leitura dos elementos da matriz\n");
        return 4;
    }

    nthreads = atoi(argv[4]);

    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL){
        fprintf(stderr, "ERRO: malloc");return 2;
    }

    args = (t_Args*) malloc(sizeof(t_Args)*nthreads);
    if(args==NULL){
        fprintf(stderr, "ERRO: malloc");return 2;
    }

    GET_TIME(fim);
    delta[1] = fim - inicio;
    delta[0] = delta[1];

    /* Multiplica matrizes*/
    GET_TIME(inicio);
    for (int i = 0; i < nthreads; i++)
    {
        (args+i) ->id =i;
        (args+i) ->dim =dim;
        if(pthread_create(tid+i, NULL,multiplica, (void *) (args+i))){
            fprintf(stderr, "ERRO: pthread_create");
            return 6;
        }
    }
    for(int i=0; i<nthreads; i++) {
      if(pthread_join(*(tid+i), NULL) ) {
          fprintf(stderr, "ERRO: pthread_join");
          return 7;
      }
   }
    GET_TIME(fim);
    delta[2] = fim - inicio;
    delta[0] += delta[2];



    GET_TIME(inicio);
    /*abre o arquivo para escrita binaria*/

    arq3 = fopen(argv[3], "wb");
    if(!arq3) {
        fprintf(stderr, "Erro de abertura do arquivo\n");
        return 3;
    }

    /*escreve numero de linhas e de colunas*/
    ret3 = fwrite(&linhasA, sizeof(int), 1, arq3);
    ret3 = fwrite(&colunasB, sizeof(int), 1, arq3);

    /*escreve os elementos da matriz C = A*B, o resultado de A*B*/
    ret3 = fwrite(matrizC, sizeof(float), tamC, arq3);
    if(ret3 < tamC) {
        fprintf(stderr, "Erro de escrita no arquivo\n");
        return 4;
    }

    /*fecha arquivos e libera memória*/
    fclose(arq1);
    fclose(arq2);
    fclose(arq3);
    free(matrizA);
    free(matrizB);
    free(matrizC);
    GET_TIME(fim);
    delta[3] = fim - inicio;
    delta[0] += delta[3];

    printf("\nTempo de inicialização: %.lf ns\n", delta[1]);
    printf("Tempo de processamento: %.lf ns\n", delta[2]);
    printf("Tempo de finalização: %.lf ns\n", delta[3]);

    printf("\nTempo de execução total: %.lf ns\n", delta[0]);

    return 0;


}
