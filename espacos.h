/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   espacos.h
 * Author: hugogomes
 *
 * Created on 1 de abril de 2020, 14:49
 */

#include "structs.h"

#ifndef ESPACOS_H
#define ESPACOS_H

#define TRUE 1
#define FALSE 0


int checkIfConnectionValid(int connection, local *espacoToCheck);

int findSize(char *fileName);

int searchInsertIndex(local *espaco, int idToInsert, int size);

void insertLocalInEspaco(local *espaco, local espacoToInsert, int insertIndex, int size);

local *findLocalById(local *espacos, int lowIndex, int size, int id);

int validateEspacosConnection(local *espaco1, local *espaco2);
int validateEspacos(local *espacos, int size);
int findLocalInEnvById(ambiente *env, int id, int size);
ambiente *getTransferPeopleOrigin(ambiente *environment, int sizeEnvironment, int *envOriginIndex);
ambiente *getTransferPeopleDestination(ambiente *environment, int sizeEnvironment, int envOriginIndex, ambiente envOrigin);
int getTransferPeopleQuant(int sizeEnvironment, ambiente envOrigin, ambiente envDestination);
void printEnv(ambiente *environment, int size);


#endif /* ESPACOS_H */

