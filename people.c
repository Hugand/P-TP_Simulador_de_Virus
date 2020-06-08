/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "people.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int validatePerson(person p){
    if(p.age <= 0 || p.nDaysInfected < 0 || (p.state != 'S' && p.state != 'D' && p.state != 'I') ){
        return FALSE;
    }
    return TRUE;
}

void moveLLItem(ambiente *destination, linkedList *target){
    linkedList *dest = destination->population;

    if(destination->population == NULL){
        destination->population = target;
    }else{
        while(dest->next != NULL){
            dest = dest->next;
        }

        dest->next = target;
    }
    
}

linkedList *getLLItem(linkedList *ll, int pos){
    linkedList *currPos = ll;
    int i;

    for(i=0; i < pos && currPos->next != NULL; i++){
        currPos = currPos->next;
    }
    return currPos;
}


void getRandomPeopleIndexArray(int *randomIndexes, ambiente *envOrigin, linkedList *prevOriginPop, linkedList *currOriginPop, int quantPeople){
    int i;

    for(i = 0; i < quantPeople; i++){
        do{
            randomIndexes[i] = intUniformRnd(0, (envOrigin->nPeople)-1);
        }while(isIntArrValueUnique(randomIndexes, randomIndexes[i], i) == FALSE);
    } 
    qsort(randomIndexes, quantPeople, sizeof(int), cmpfunc);
}
