/*
 * Computação Concorrente - 2022.2 - UFRJ
 * Profa. Silvana Rossetto
 * Aluno: Eduardo Kota Otomo
 * Laboratório 2: Multiplicação de matriz quadrada por um vetor
 */


/*desempenho: A=tS/tC = Te+Tp+Ts/(Te+Ts+Tp/N) */

/*matriz[1][0] ~~ matriz[1x3+0]*/

#include<stdio.h>
#include<stdlib.h>
#include "timer.h"
//#include<pthread.h>

float *matriz,    /*matriz de entrada*/
    *vetor,    /*vetor de entrada*/
    *saida;    /*matriz de saída*/


int main(int argc, char* argv[]){

    int i, j; /*indices*/

    double inicio, fim, delta;

    GET_TIME(inicio);
    /*leitura e avaliação de parâmetros de entrada*/
    if(argc<2){
        printf("Digite: %s <dimensão da matriz>\n", argv[0]);
        return 1;
    }
    int dim =atoi(argv[1]); /*dimensão da matriz de entrada*/

    /*malloc estruturas de dados*/
    matriz = malloc(sizeof(float) * dim * dim);
    if (matriz == NULL){printf("ERRO: malloc\n"); return 2;}
    vetor = malloc(sizeof(float) * dim);
    if (vetor == NULL){printf("ERRO: malloc\n"); return 2;}
    saida = malloc(sizeof(float) * dim);
    if (saida == NULL){printf("ERRO: malloc\n"); return 2;}


    /*inicializa estruturas de dados de entrada e saida*/
    for(i=0; i<dim; i++){
        for(j=0; j<dim; j++)
            matriz[i*dim+j] = 1;   //matriz[i][j]
        vetor[i] =1;
        saida[i] =0;
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de inicialização: %.lf\n", delta);

    GET_TIME(inicio);
    /*multiplica matriz quadada pelo vetor*/
    for(i=0; i<dim; i++){
        for(j=0; j<dim; j++)
            saida[i] += matriz[i*dim+j]*vetor[j];
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de processamento: %.lf\n", delta);

    GET_TIME(inicio);
    /*exibe resultado*/
    printf("\nMatriz de entrada: \n");
    for(i=0; i<dim; i++){
        for(j=0; j<dim; j++)
            printf("%.1f ", matriz[i*dim+j]);
        puts("");
    }
    printf("\nVetor de entrada: \n");
    for(int j=0; j<dim; j++)
        printf("%.1f ", vetor[j]);
    puts("");
    printf("\nVetor de saída: \n");
    for(int j=0; j<dim; j++)
        printf("%.1f ", saida[j]);
    puts("");

    /*libera memória*/
    free(matriz);
    free(vetor);
    free(saida);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de finalização: %.lf\n", delta);

    return 0;
}
