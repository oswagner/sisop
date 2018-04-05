// 1) Implemente o modelo produtor consumidor utilizando monitores.   
//     Seu programa tem threads produtoras que inserem valores.
//     As consumidoras podem ser de dois tipos, interessadas em pares ou em ímpares.
//     Implemente um monitor que administra espera diferenciadas para threads consumidoras de valores pares ou impares

    //   Nome: Wagner Oliveira dos Santos

    //   Comando de compilação: make

    //   Ambiente em que foi desenvolvido:  macOS High Sierra v10.13.3 - compiler Apple LLVM version 9.0.0 (clang-900.0.39.2) Target: x86_64-apple-darwin17.4.0 Thread model: posix


#define _REENTRANT 1
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


