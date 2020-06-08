/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   historico.h
 * Author: hugogomes
 *
 * Created on 7 de junho de 2020, 04:00
 */

#ifndef HISTORICO_H
#define HISTORICO_H
#include "structs.h"
#include "espacos.h"

void printHist(historico *hist);
void addToHistory(historico *hist, ambiente *environment, int *totalIterations, int sizeEnvironment);
void shiftHistory(historico *hist, int nDays);
void recoverHistoryState(historico *hist, ambiente *environment, int *totalIterations, int sizeEnvironment, int nDays);

#endif /* HISTORICO_H */

