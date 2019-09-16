#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS		5

sem_t semPratos;
sem_t mutexCozinha;
sem_t mutex;

int flag;
int interesse[THREADS];
int vez = 0;
int auxvez;
int pratos = 2;
int cozinha = 0;

void *task3(void *arg){
	int item, tid, i;

	tid = (int)(long int)arg;
	while(1){
            interesse[tid] = 1;//fala q tem interesse
            while(alguemcominteresse(tid)){ //enquanto tiver alguem com interesse
               if(vez != tid){ // e nao for /minha vez
                    interesse[tid] = 0; //digo q n tenho interesse
                    while(vez != tid);// enquanto n for minha vez
                    interesse[tid] = 1; //eu aviso q tenho interesse
               }
            }
            //RC begin
            printf("Thread %d\n", tid);
            if( pratos <= 0){
                //auxvez = vez;
                //vez = 99;
                printf("Acabo comida! Chamando o Cozinheiro\n");
                cozinha = 1;
                sem_post(&mutexCozinha);
                while(cozinha);
            }
            printf("Vou comer...");
            sem_wait(&semPratos);
            pratos--;
            printf("thread Comendo!\n");
            //usleep(550000);
            escolhevez();
            //RC end
            interesse[tid] = 0;
	}
}

void *task2(void *arg){
	int item, tid, i;
    int vezaux = vez;
    //vez = 99;
	tid = (int)(long int)arg;
	while(1){
        sem_wait(&mutexCozinha);
        for(int i = 0; i < 2;i++){
            sem_post(&semPratos);
            }
            pratos = 2;
            printf("\n Fazendo comida...\n");
            cozinha = 0;
	    }
	    //vez = vezaux;
}

void escolhevez(){
    if(vez == THREADS-1){
        vez = 0;
    }else{
        vez = vez + 1;
    }
}

int alguemcominteresse(int tid){
    for(int i = 0; i < THREADS;i++){
        if(i != tid){ // nao qero saber se eu mesmo tenho interesse
            if(interesse[i]){
                return 1; //alguem tem interesse
            }
        }
    }
    return 0; //ninguem tem interesse
}

int main(void){
	long int i;
	pthread_t threads[THREADS];

	for(i = 0; i < THREADS; i++)
        interesse[i] = 0;
    sem_init(&semPratos,0,pratos);
	sem_init(&mutexCozinha,0,0);

    for(i = 0; i < THREADS; i++)
		pthread_create(&threads[i], NULL, task3, (void *)i);
		//pthread_create(&threads[1], NULL, task2, (void *)1);

    pthread_t cozinha;
		pthread_create(&cozinha, NULL, task2, (void *)i);
	pthread_exit(NULL);
	return(0);
}

