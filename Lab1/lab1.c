//Programação Concorrente (ICP361)
//Professora: Silvana Rossetto
//Laboratório 1

//Nome: Eduardo Kota Otomo
//DRE: 118147443
//Data: 9 de setembro de 2022.

#include<stdio.h>
#include<pthread.h>
#include<stdlib.h> 


#define NTHREADS  2 //número de threads
#define TAMVETOR 10000 //tamanho do vetor

double vetor[TAMVETOR];

typedef struct {
   int thread, nThreads; 
} t_Args;


//função que a thread irá executar
void * tarefa (void * arg){
    t_Args * args = (t_Args*) arg;
    int i;

    if (args->thread) {
        for (i = TAMVETOR/2; i < TAMVETOR; i++) {
            vetor[i] *= 1.1;
        }
    } else {
        for (i = 0; i < TAMVETOR/2; i++) {
            vetor[i] *= 1.1;
        }
    }
    printf("Thread %d ok\n", args->thread);
    pthread_exit(NULL);
}

// Imprime parte do vetor para visualizar e verificar se está correto
void imprime() {
  int i = 0;
  printf("[");
  for (i = 0; i < 4; i++) {
     printf("%g, ", vetor[i]);
  }
  printf("...");
  for (i = TAMVETOR-4 ; i < TAMVETOR; i++) {
     printf(", %g", vetor[i]);
  }
  printf("]\n");
}


//função principal
int main(void) {
    pthread_t tid[NTHREADS]; //identificador da thread no sistema
    int thread; //identificador local da thread
    //cria threads

    t_Args *arg; //receberá argumentos para a thread

    //cria vetor
    int i = 0;
    for (i = 0; i < 10000; i++)
        vetor[i] = i+1;

    imprime();

    for(thread=0; thread<NTHREADS; thread++){
        //Aloca e preenche argumentos para thread
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->thread = thread;
        arg->nThreads = NTHREADS;


        //cria a thread nova
        if(pthread_create(&tid[thread], NULL, tarefa, (void *) arg))
            printf("ERROR -- pthread_create\n");
    }
    //espera as threads terminarem
    for(thread=0; thread<NTHREADS; thread++){
        if(pthread_join(tid[thread], NULL)){
            printf("ERROR -- pthread_join\n"); exit(-1);
        }
    }

    imprime();

    printf("Thread principal terminou\n");

    pthread_exit(NULL);
    return 0;
}
