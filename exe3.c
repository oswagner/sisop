// Faça um programa quê valores inteiros da console, e repassa o mesmo número para outros dois processos concorrentes.
// Um processo simplesmente multiplica o valor por 2 e o outro por 3 (uma metéfora de uma computação sobre a entrada – poderia ser processamento de imagem, etc).
// Estes processos passam seus valores para um terceiro, que então escreve os dois valores em uma mesma linha, sempre na mesma ordem (x2 e depois x3).

// Use fork(s) e pipe(s) para tal.

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int fileDescriptor[2];
int value, pid;

int main() {
  
  puts("Digite um valor");
  scanf("%d",&value);
  fflush(stdin);
  printf("Input = %d \n", value);

  // error to create a pipe
  if (pipe(fileDescriptor) > 0) {
    exit(1);
  }

  if (fork() != 0) {
    write(fileDescriptor[1], &value, sizeof(value));
    printf("Parent(%d) send value: %d\n", getpid(), value);
  } else {
    read(fileDescriptor[0], &value, sizeof(value));
    printf("Child(%d) received value: %d\n", getpid(), value);
  }


//  printf("Eu sou o processo %ld, e meu pai e %ld\n",(long) getpid(), (long) getppid());
}

// int p0[2], p1[2] ;
// char c, k ;
// main()
// {
// int i, pid, pid1;
// pipe(p0) ;
// pid = fork() ;
// if (pid != 0) {
//         for (i=0; i<10; i++){
//             puts("Digite um caracter: ");
//             fflush(stdin);
//             scanf("%c",&c); 
//             write(p0[1], &c, sizeof(char));
//             }
//         } 
// else {
//      for (i=0;i<100;i++){
//        read(p0[0], &k, sizeof(char)) ;
//        printf("Lido do pipe: %c\n", k );
//        }   
//      }
// }
