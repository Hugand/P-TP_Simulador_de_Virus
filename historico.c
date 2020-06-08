/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "historico.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void printHist(historico *hist){
    int i;

    for(i = 0; i < 3; i++){
        printf("HIST pos %d => %d\n", i, hist[i].iterationNumber);
    }
}

void addToHistory(historico *hist, ambiente *environment, int *totalIterations, int sizeEnvironment){
    int i;
    linkedList *iterator;
    linkedList *histPopulation;
    for(i = 2; i > 0; i--){
        hist[i] = hist[i-1];
    }
    hist[0].env = malloc(sizeof(ambiente) * sizeEnvironment);
    hist[0].iterationNumber = *totalIterations;
    for(i = 0; i < sizeEnvironment; i++){
        iterator = environment[i].population;
        hist[0].env[i].nPeople = environment[i].nPeople;
        hist[0].env[i].local = environment[i].local;
        if(environment[i].population != NULL){
            hist[0].env[i].population = malloc(sizeof(linkedList));
            histPopulation = hist[0].env[i].population;

            while(iterator != NULL){
                histPopulation->p = iterator->p;
                iterator = iterator->next;
                if(iterator == NULL)
                    histPopulation = NULL;
                else{
                    histPopulation->next = malloc(sizeof(linkedList));
                    histPopulation = histPopulation->next;
                }
            }
        }else
            hist[0].env[i].population = NULL;
    }

    free(iterator);
    free(histPopulation);
}

void shiftHistory(historico *hist, int nDays){
    int i;
    for(i = 0; i < 3-nDays; i++){
        hist[i] = hist[i+nDays];
    }
    for(i = 3-nDays; i < 3; i++){
        hist[i].iterationNumber = -1;
        hist[i].env = NULL;
    }
}

void recoverHistoryState(historico *hist, ambiente *environment, int *totalIterations, int sizeEnvironment, int nDays){
    int i;
    linkedList *prev, *curr;

    for(i = 0; i < sizeEnvironment; i++){
        environment[i].local = hist[nDays-1].env[i].local;
        environment[i].nPeople = hist[nDays-1].env[i].nPeople;

        prev = NULL;
        curr = environment[i].population;
        int h = 0;
        while(curr != NULL){
            prev = curr;
            curr = curr->next;
            free(prev);
            h++;
        }
        
        environment[i].population = hist[nDays-1].env[i].population;
        *totalIterations = hist[nDays-1].iterationNumber;
    }

}
