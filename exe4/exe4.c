// 1) Implemente o modelo produtor consumidor utilizando threads e semáforos.   
//     Inicialmente seu programa deve funcionar para uma produtora e uma consumidora

// 2) Avalie se este programa suporta N threads produtoras e M consumidoras, com N e M >= 2.
//     Estenda para que o programa funcione para este caso.

// 3) Implemente uma solução para o Jantar dos Filósofos que seja livre de Deadlock.
//     Se o seu programa permite que um filosofo especifico sofra inanição (starvation), demonstre a situação.
//     Se o seu programa não tem starvation, argumente porque não.

//        Nome: Wagner Oliveira dos Santos

//        Comando de compilação: make

//        Ambiente em que foi desenvolvido:  macOS High Sierra v10.13.3 - compiler Apple LLVM version 9.0.0 (clang-900.0.39.2) Target: x86_64-apple-darwin17.4.0 Thread model: posix


#define _REENTRANT 1
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define size 10

pthread_t producer_id, consumer_id;

int shared_data[size];

sem_t* mutex;

void* producer();
void* consumer();
void log_shared_data(int *value);

int main() {
    printf("=========================== Main ===========================\n");
    
    log_shared_data(&shared_data);
    
    sem_init(&mutex,0,1);

    pthread_create(&producer_id, NULL, producer, NULL);
    pthread_create(&consumer_id, NULL, consumer, NULL);

    pthread_join(producer_id, NULL);
    pthread_join(consumer_id, NULL);
}


void* producer(void *param) {
    printf("=========================== Start Producer ===========================\n");
    sem_wait(&mutex);
    int a;
    for( a = 0; a < size; a++ ) {
        int value = a + 10;
        shared_data[a] = value;
        printf("Produced value shared_data[%d] = %d \n ", a, value);
    }
    sem_post(&mutex);
}


void* consumer(void *param) {
    printf("=========================== Start Consumer ===========================\n");
    sem_wait(&mutex);
    int i;
    for( i = 0; i < size; i++ ) {
        int value = shared_data[i];
        printf("Consumed value shared_data[%d] = %d \n ", i, value);
    }

    sem_post(&mutex);
}


// helper
void log_shared_data(int *value) {
    int i;
    for( i = 0; i < size; i++ ) {
        printf("Shared_data[%d] = %d \n", i, value[i]);
    }
}