// Faça um programa quê valores inteiros da console, e repassa o mesmo número para outros dois processos concorrentes.
// Um processo simplesmente multiplica o valor por 2 e o outro por 3 (uma metéfora de uma computação sobre a entrada – poderia ser processamento de imagem, etc).
// Estes processos passam seus valores para um terceiro, que então escreve os dois valores em uma mesma linha, sempre na mesma ordem (x2 e depois x3).

// Use fork(s) e pipe(s) para tal.

    //   Nome: Wagner Oliveira dos Santos e Renata Soria

    //   Comando de compilação: make

    //   Ambiente em que foi desenvolvido:  macOS High Sierra v10.13.3 - compiler Apple LLVM version 9.0.0 (clang-900.0.39.2) Target: x86_64-apple-darwin17.4.0 Thread model: posix


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int fileDescriptorP1[2], fileDescriptorP2[2], fileDescriptorP3[2], fileDescriptorP4[2];
int input, first_value, second_value, first_result, second_result;

pid_t p1, p2, p3;

int main() {
  
  puts("Input some value");
  scanf("%d",&input);
  fflush(stdin);
  printf("Input N = %d \n", input);

  pipe(fileDescriptorP1);
  pipe(fileDescriptorP2);
  pipe(fileDescriptorP3);
  pipe(fileDescriptorP4);

  p1 = fork();
  if (p1 > 0) {
    write(fileDescriptorP1[1], &input, sizeof(input));
    write(fileDescriptorP2[1], &input, sizeof(input));
  } else if(p1 == 0 ) {
    read(fileDescriptorP1[0], &first_value, sizeof(first_value));
    int result = first_value * 2;
    write(fileDescriptorP3[1], &result, sizeof(result));
  }
  p2 = fork();
  if (p2 > 0) {
    read(fileDescriptorP2[0], &second_value, sizeof(second_value));
    int result = second_value * 3;
    write(fileDescriptorP4[1], &result, sizeof(result));
  } else if (p2 == 0) {
    read(fileDescriptorP3[0], &first_result, sizeof(first_result));
    read(fileDescriptorP4[0], &second_result, sizeof(second_result));
    printf("Result of n x 2 = %d - Result of n x 3 = %d \n", first_result, second_result);
  }
}