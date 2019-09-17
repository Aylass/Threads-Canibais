#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define THREADS 10
#define COMIDAS 4

sem_t semPratos;
sem_t semCozinheiro;
sem_t barreiraComida;
sem_t mutex;

int interesse[THREADS];
int vez = 0;
int pratos = 1;
volatile int cozinha = 0;

void *canibalCode(void *arg)
{
    int tid = (int)(long int)arg;

    while (1)
    {
        sem_wait(&semPratos);
        printf("Thread %d entrou na regiao critica.\n", tid);
        lock(tid);
        //RC begin
        pratos--;
        printf("Thread %d pegou comida.\n", tid);
        if (pratos == 0)
        {
            cozinha = 1;
            sem_post(&barreiraComida);
            while (cozinha)
                ;
        }
        //RC end
        unlock(tid);
        printf(" Thread Comendo!\n");
    }
}
void *cozinheiroCode(void *arg)
{
    int item, tid, i;
    tid = (int)(long int)arg;
    while (1)
    {
        sem_wait(&barreiraComida);
        printf("\n Fazendo comida...\n");
        for (int i = 0; i < COMIDAS; i++)
            sem_post(&semPratos);
        pratos = COMIDAS;
        cozinha = 0;
        usleep(550000);
    }
}
void lock(int id)
{
    interesse[id] = 1; //fala q tem interesse
    while (anyoneTrue(id))
    { //enquanto tiver alguem com interesse
        if (vez != id)
        {                      // e nao for /minha vez
            interesse[id] = 0; //digo q n tenho interesse
            while (vez != id);              // enquanto n for minha vez
            interesse[id] = 1; //eu aviso q tenho interesse
        }
    }
}
void unlock(int id)
{
    nextTurn();
    interesse[id] = 0;
}
void nextTurn()
{
    vez = (vez + 1) % (THREADS - 1);
}

int anyoneTrue(int tid)
{
    for (int i = 0; i < THREADS; i++)
    {
        if (i != tid)
        { // nao qero saber se eu mesmo tenho interesse
            if (interesse[i])
            {
                return 1; //alguem tem interesse
            }
        }
    }
    return 0; //ninguem tem interesse
}

int main(void)
{
    long int i;
    pthread_t threads[THREADS];

    for (i = 0; i < THREADS; i++)
        interesse[i] = 0;

    sem_init(&semPratos, 0, 1);
    sem_init(&barreiraComida, 0, 0);

    for (i = 0; i < THREADS - 1; i++)
        pthread_create(&threads[i], NULL, canibalCode, (void *)i);
    pthread_create(&threads[THREADS - 1], NULL, cozinheiroCode, (void *)1);
    pthread_exit(NULL);
    return (0);
}
