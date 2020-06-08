// Trabalho Pratico Programacao - LEI
// DEIS-ISEC 2019-2020

#ifndef UTILS_H
#define UTILS_H


#define TRUE 1
#define FALSE 0
// Inicializa o gerador de numeros aleatorios.
// Esta funcao deve ser chamada apenas uma vez no inicio da execucao do programa
void initRandom();

//Devolve um valor inteiro aleatorio distribuido uniformemente entre [a, b]
int intUniformRnd(int a, int b);

// Devolve o valor 1 com probabilidade prob. Caso contrario, devolve 0
int probEvento(float prob);

int cmpfunc (const void * a, const void * b);
int isIntArrValueUnique(int *arr, int val, int size);

#endif /* UTILS_H */
