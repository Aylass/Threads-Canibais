/*
exemplo com semPratos

uma unica thread acessa a regiao critica por vez.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS		30

sem_t semPratos;
sem_t mutexCozinha;
sem_t semCanibal;
int qtdPratos;
int qtdCanibal;
int pratos;
void *task(void *arg){
	int item, tid, i;

	tid = (int)(long int)arg;

	while(1){
        sem_wait(&semCanibal);
		if( pratos == 0){
            printf("Acabo comida! Chamando o Cozinheiro\n");
            sem_post(&mutexCozinha);
		}
        sem_wait(&semPratos);
        pratos--;
        printf("thread %d Comendo!\n", tid);
        usleep(550000);
		sem_post(&semCanibal);
	}
}
void *task2(void *arg){
	int item, tid, i;

	tid = (int)(long int)arg;
	while(1){
        sem_wait(&mutexCozinha);
        for(int i = 0; i < qtdPratos;i++){
            sem_post(&semPratos);
            }
            pratos = qtdPratos;
            printf("\n Fazendo comida\n");
            usleep(990000);
	    }
}
int main(void){
	long int i;
	pthread_t threads[THREADS];
	printf("Digite a quantidade de Canibais: \n  ");
	scanf("%d", &qtdCanibal);
	printf("Digite a quantidade de pratos: \n  ");
	scanf("%d", &qtdPratos);

	pratos = 0;

	sem_init(&semPratos,0,0);
	sem_init(&mutexCozinha,0,0);
	sem_init(&semCanibal,0,1);

	for(i = 0; i < qtdCanibal; i++)
		pthread_create(&threads[i], NULL, task, (void *)i);
	pthread_t cozinha;
		pthread_create(&cozinha, NULL, task2, (void *)i);

	pthread_exit(NULL);
	return(0);
}

