//  ------------------------------------------------------
//  ------------------------------------------------------
//  ------------------------------------------------------
//  implementação de monitores com semáforos em c+pthreads
//  autor: Fernando Dotti
//  util nas disciplinas de Sistemas Operacionais e 
//  Modelos de Computação Concorrente
//
//  ATENCAO: codigo fornecido aos alunos como possivel
//  resposta do professor.
//  ENTRETANTO: ainda sujeito a testes pelo professor e alunos
//  Em caso de nao funcionamento, reportar o que esta
//  acontecendo.
//  LEMBRANDO: este codigo rodou corretamente
//  para 3 threads produtoras e 2 consumidoras. Compila
//  sem warnings.  Entao antes de reportar o primeiro
//  problema, tente fazer compilar e funcionar adequadamente.

// no mac-os gcc pthreads-buffer-sync-v3.c -o ts

#define _REENTRANT 1
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

//  ------------------------------------------------------
//  ------------------------------------------------------
//  estruturas genericas de monitores

struct Monitor{
    sem_t * mutex;          // garante exclusão mutua do monitor
    sem_t * next;           // bloqueia thread que sinaliza em favor de outra - vide signal de condition
    int next_count;         // conta threads em next, que podem prossegir
} ;

void initMonitor(struct Monitor *m) {
    m->mutex = sem_open("/mon.mutex",O_CREAT,0,1);    // 1 thread pode passar
    m->next = sem_open("/mon.next",O_CREAT,0,0);      // 0 inicia bloqueando e espera sema post
    m->next_count = 0;
}

void destructMonitor(struct Monitor *m) {
    sem_unlink("/mon.mutex");                   // esta opcao de semaforo nomeado tem que ser desligada e fechada
    sem_unlink("/mon.next");                    // senao ao rodar novamente o processo, obtem semaforo antigo
    sem_close(m->mutex);                        // de mesmo nome do nucleo do sistea
    sem_close(m->next);
    }

//  procedimentos do monitor

void monitorEntry(struct Monitor *m){
    sem_wait(m->mutex);                         // entrada no monitor ee so passar pelo mutex
}

void monitorExit(struct Monitor *m){
    if (m->next_count>0) sem_post(m->next);     // sai do monitor, libera de next com preferencia
    else sem_post(m->mutex);                    // pois ee uma thread que ja esteve no monitor, senao libera mutex
}


//  ------------------------------------------------------
//  ------------------------------------------------------
//  estruturas genericas de  variaveis condicao

struct Condition {
    sem_t * s;              // semaforo para bloquear na condicao
    int count;              // contador de bloqueados
    struct Monitor * m;     // monitor associado aa condicao - quando bloqueia na condicao libera o monitor (next ou mutex)
    char * name;
};

void initCondition(char * n, struct Condition *c, struct Monitor *m1) {
    c->s = sem_open(n , O_CREAT,0,0);    // 0 inicia bloqueando
    c->count = 0;                        // contadores de bloqueados nesta condicao
    c->m = m1;                           // o monitor associado
    c->name = n;
}

void destructCondition(struct Condition *c) {
    sem_unlink(c->name);                 // desligando e fechando o semaforo do nucleo para reusar nomes
    sem_close(c->s);
}


//  procedimentos de variaveis condicao

void condWait(struct Condition *c){
    printf("                                           wait %s \n", c->name);
    c->count++;                         // mais uma thread vai estar um bloqueado aqui nesta condition
    monitorExit(c->m);                  // libera o monitor associado aa condition
    sem_wait(c->s);                     // bloqueia !!     fica aqui ate alguem dar signal  !!
    c->count--;                         // esta linha é executada depois de alguem ter dado signal, entao um bloqueado a menos
}

void condSignal(struct Condition *c){
    if (c->count>0) {                   // tem alguem para sinalizar ?   se nao tem entao nao faz nada, signal nao tem efeito!
        c->m->next_count++;             // tem alguem para sinalizar, entao esta thread se bloqueia em favor da sinalizada
        sem_post(c->s);                 // incrementou contador de bloqueados em next,  sinaliza !!
        sem_wait(c->m->next);           // neste instante duas threads estao no monitor, esta abdica se bloqueando em next
        c->m->next_count--;             // ficou bloqueada ate ser desbloqueada (veja monitorExit), aqui desbloqueou, decrementa
    }
}

//  ------------------------------------------------------
//  ------------------------------------------------------


//  ------------------------------------------------------
//  ------------------------------------------------------
//  o monitor EXEMPLO

struct Monitor m;                       // um monitor
struct Condition cheio, vazio;          // duas variaveis condicao

#define N 10
int buffer[N], in = 0, out = 0 ;        // um buffer circular de 10 posicoes
int entraram = 0, sairam = 0;           // conta ao todo quantos entraram e sairam, para verificar consistencia ao final

int isFull(int i, int o){
    return ((( i + 1 ) % N ) == o );    // condicao de buffer cheio
}

int isEmpty(int i, int o){
    return (i == o );                   // condicao de buffer vazio
}

void initMyMonitor(){
    initMonitor(&m);                       // inicializa monitor
    initCondition("cheio", &cheio, &m);    // inicializa condicao e associa com este monitor
    initCondition("vazio", &vazio, &m);    // ... idem
}

void insere(int x) {
    monitorEntry(&m);                  // entrada no monitor
    if (isFull(in,out))                // se buffer cheio ?
        condWait(&cheio);              //     entao espera nao estar mais cheio
    buffer[in] = x ;                   // agora tem espaco, produz novo elemento
    in = (in + 1) % N ;                // ajusta marcador de entrada
    
    entraram++;                        // contabiliza para verificar
    
    condSignal(&vazio);                // como produziu, se alguem espera para consumir pois estava vazio, pode ser acordada!!
                                       // se nao houver ninguem esperando, signal é vazio, nao afeta!!
    monitorExit(&m);                   // sai do monitor
}

int retira() {
    int c;
    monitorEntry(&m);                 // entrada no monitor
    if (isEmpty(in,out))              // se buffer vazio ?
        condWait(&vazio);             //     entao espera ter algo no buffer para poder consumir
    c = buffer[out] ;                 // agora tem algo!  pega o item
    out = ( out + 1 ) % N ;           // ajusta marcador de saida
    
    sairam++;                         // contabiliza para verificar
    
    condSignal(&cheio);               // como consumiu, se alguem espera para produzir pois estava cheio, pode ser acordada!!
    monitorExit(&m);                  // se nao houver ninguem esperando, signal é vazio, nao afeta !!
    return c;                         // retorna o item consumido
}



//  ------------------------------------------------------
//  ------------------------------------------------------


//  ------------------------------------------------------
// -------------------------------------------------------
// USA monitor EXEMPLO

void * consumidor(){
    while (1){
        printf("                   retira %d, \n", retira()) ;
    }
}

void * produtor(){
    int i=0;
    while (1) {
        insere(i);
        printf("insere: %d, in: %d \n",i,in);
        i++;
    }
}

//  ------------------------------------------------------
//  ------------------------------------------------------


pthread_t tidp1, tidp2,tidp3, tidc1, tidc2;

int main(){


    initMyMonitor();                // inicia o monitor m - para ficar mais elegante, pode passar monitor como parametro ...
    
    printf("Eu sou o main\n");
    
    pthread_create(&tidc1, NULL, consumidor, NULL);
    pthread_create(&tidc2, NULL, consumidor, NULL);
    printf("Criei consumidores\n");

    pthread_create(&tidp1, NULL, produtor, NULL);
    pthread_create(&tidp2, NULL, produtor, NULL);
    pthread_create(&tidp3, NULL, produtor, NULL);
    printf("Criei produtores\n");

    for(long j=1;j<10000000;j++);    // espera ocupada - melhor seria usar um nanosecondsleep ...
    
    printf("Vou parar todas threads \n");
    pthread_cancel(tidc1);
    pthread_cancel(tidc2);
    pthread_cancel(tidp1);
    pthread_cancel(tidp2);
    pthread_cancel(tidp3);
    
    
    printf("Entraram: %d    Sairam: %d \n", entraram, sairam);
    
    destructMonitor(&m);            // desalocando estruturas do nucleo
    destructCondition(&cheio);      // caso nao faca isso, outra invocacao deste processo podera reusar a estrutura do nucleo
    destructCondition(&vazio);      // com valores anteriores
    
}



