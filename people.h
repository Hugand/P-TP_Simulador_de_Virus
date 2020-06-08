/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   people.h
 * Author: hugogomes
 *
 * Created on 1 de abril de 2020, 15:26
 */
#include "structs.h"

#ifndef PEOPLE_H
#define PEOPLE_H

#define LL_END NULL
#define LL_START 0

#define TRUE 1
#define FALSE 0

int validatePerson(person p);
void moveLLItem(ambiente *destination, linkedList *target);
linkedList *getLLItem(linkedList *ll, int pos);
void getRandomPeopleIndexArray(int *randomIndexes, ambiente *envOrigin, linkedList *prevOriginPop, linkedList *currOriginPop, int quantPeople);

#endif /* PEOPLE_H */

