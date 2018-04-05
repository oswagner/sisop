#define _REENTRANT 1
#include <pthread.h>

pthread_t tid1,tid2;
pthread_attr_t attr ;

#define N 10
int buffer[N], in = 0, out = 0 ;

void* produtor(){

  int i, p = 10 ;
  printf("Produtor\n");

  for(i=0; i<20; i++) 

     {
       printf("Produtor\n");
       while ((( in + 1 ) % N ) == out ) {} /* buffer cheio */
       buffer[in] = p ;
       in = (in + 1) % N ;       
       p++ ;         
     }
  }

void* consumidor(){
  int i, c ;

  for (i=0; i<20; i++) 
    {
      printf("Conumidor\n");
      while (in == out ) {} /* buffer vazio*/
      c = buffer[out] ;
      printf(" Consumi o valor %d da posicao %d \n",c, out) ;
      out = ( out + 1 ) % N ;      
    }
}

main(){
  int result ;

  printf("Eu sou o main\n");
  pthread_create(&tid1, NULL, consumidor, NULL);
  printf("Eu sou o main\n");
  result=pthread_create(&tid2, NULL, produtor, NULL);
  printf("Eu sou o main\n");
   /* pthread_cancel(tid1) ; */

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

}

