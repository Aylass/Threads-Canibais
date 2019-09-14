/*
exemplo com mutex

uma unica thread acessa a regiao critica por vez.
*/

#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS		5

sem_t mutex;
int flag;
int interesse[THREADS];
int vez = 0;

void *task(void *arg){
	int item, tid, i;

	tid = (int)(long int)arg;
	while(1){
            //printf("oi  %d\n\n", vez);
            interesse[0] = 1;//fala q tem interesse
            while(interesse[1]){ //enquanto tiver alguem com interesse
               if(vez != 0){ // e nao for /minha vez
                    interesse[0] = 0; //digo q n tenho interesse
                    while(vez != 0);// enquanto n for minha vez
                    interesse[0] = 1; //eu aviso q tenho interesse
               } // é minha vez
            }
                printf("thread %d..\n", tid);
                 usleep(990000);
                vez = 1;
                interesse[0] = 0;
                    //escolhevez();

	}
}
void *task2(void *arg){
	int item, tid, i;

	tid = (int)(long int)arg;
	while(1){
            //printf("oi  %d\n\n", vez);
            interesse[1] = 1;//fala q tem interesse
            while(interesse[0]){ //enquanto tiver alguem com interesse
               if(vez != 1){ // e nao for /minha vez
                    interesse[1] = 0; //digo q n tenho interesse
                    while(vez != 1);// enquanto n for minha vez
                    interesse[1] = 1; //eu aviso q tenho interesse
               }
            }
                   printf("thread %d\n", tid);
                    usleep(990000);
                    //escolhevez();
                    vez = 0;
                    interesse[1] = 0;
	}
}

void *task3(void *arg){
	int item, tid, i;

	tid = (int)(long int)arg;
	while(1){
            //printf("oi  %d\n\n", vez);
            interesse[tid] = 1;//fala q tem interesse
            while(alguemcominteresse(tid)){ //enquanto tiver alguem com interesse
               if(vez != tid){ // e nao for /minha vez
                    interesse[tid] = 0; //digo q n tenho interesse
                    while(vez != tid);// enquanto n for minha vez
                    interesse[tid] = 1; //eu aviso q tenho interesse
               }
            }
                   printf("thread %d\n", tid);
                    usleep(990000);
                    printf("Minha vez: %d\n", vez);
                    escolhevez();
                    printf("Amiguinho vez: %d\n", vez);
                    //vez = 0;
                    usleep(990000);
                    interesse[tid] = 0;
	}
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

	sem_init(&mutex,0,1);

    for(i = 0; i < THREADS; i++)
		pthread_create(&threads[i], NULL, task3, (void *)i);
		//pthread_create(&threads[1], NULL, task2, (void *)1);

	pthread_exit(NULL);
	return(0);
}

