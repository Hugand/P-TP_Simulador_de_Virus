/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   structs.h
 * Author: hugogomes
 *
 * Created on 7 de junho de 2020, 19:22
 */

#ifndef STRUCTS_H
#define STRUCTS_H


typedef struct sala local, *plocal;
struct sala{
    int id;
    int capacidade;
    int liga[3];
};

typedef struct pessoa person;
struct pessoa{
    char id[40];
    int age;
    char state; // S, D, I
    int nDaysInfected;
};

typedef struct ll linkedList;
struct ll{
    person p;
    linkedList *next;
};

typedef struct esp ambiente;
struct esp{
    local *local;
    int nPeople;
    linkedList *population;
};

typedef struct history historico;
struct history{
    int iterationNumber;
    ambiente *env;
};

#endif /* STRUCTS_H */

