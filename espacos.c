#include "espacos.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int findSize(char *fileName){
    FILE *fp = fopen(fileName, "rb");

    if(fp == NULL){
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    int res = ftell(fp);

    fclose(fp);

    return res;
}

// TODO: fix this :)
int searchInsertIndex(local *espaco, int idToInsert, int size){
    int low = 0;
    int high = size;
    int h = (low+high)/2;
    while(1 == 1){
        if((size == 1 && espaco[h].id <= idToInsert) || h == size-1){
            h++;
            break;
        }else if(size == 1 && espaco[h].id >= idToInsert){
            h--;
            break;
        }if(espaco[h].id == idToInsert || h == 0 ||
            idToInsert == espaco[h-1].id || (espaco[h].id >= idToInsert && espaco[h-1].id <= idToInsert)){
            break;
        }else if(idToInsert == espaco[h+1].id || (espaco[h].id <= idToInsert && espaco[h+1].id >= idToInsert)){
            h++;
            break;
        }else if(espaco[h].id > idToInsert){
            high = h;
            h = (low+high)/2;
        }else if(espaco[h].id < idToInsert){
            low = h;
            h = (low+high)/2;
        }
    }

    return h;
}

void insertLocalInEspaco(local *espaco, local espacoToInsert, int insertIndex, int size){
    int i;
    for(i = size-1; i > insertIndex; i--){
        espaco[i] = espaco[i-1];
    }
    espaco[insertIndex] = espacoToInsert;
}


local *findLocalById(local *espacos, int lowIndex, int size, int id){
    int i, low = lowIndex, high = size-1;

    if(espacos[high].id == id)
        return espacos+(high);
    else if(espacos[low].id == id)
        return espacos+low;

    while(low <= high){
        i = (low+high)/2;
        if(espacos[i].id == id)
            return espacos+i;
        if(espacos[i].id < id)
            low = i+1;
        else
            high = i - 1;
    }

    return NULL;
}


int findLocalInEnvById(ambiente *env, int id, int size){
    int i, low = 0, high = size-1;

    if(env[high].local->id == id){
        printf("-> %d\n", (env+high)->local->id);
        return high;
    }else if(env[low].local->id == id)
        return low;

    while(low <= high){
        i = (low+high)/2;
        if(env[i].local->id == id)
            return i;
        if(env[i].local->id < id)
            low = i+1;
        else
            high = i - 1;
    }

    return -1;
}

int checkIfConnectionValid(int connection, local *espacoToCheck){
    int i;

    for(i = 0; i < 3; i++){
        if(espacoToCheck->liga[i] == connection)
            return TRUE;
    }

    return FALSE;
}

int validateEspacosConnection(local *espaco1, local *espaco2){
    int i;
    for(i = 0; i < 3; i++){
        if(espaco2->liga[i] == espaco1->id){
            return TRUE;
        }
    }
    return FALSE;
}

int validateEspacos(local *espacos, int size){
    int i, h;
    local *ligacaoLocal;
    int isConnectionValid;

    for(i = 0; i < size; i++){
        for(h = 0; h < 3; h++){
            if(espacos[i].liga[h] > espacos[i].id){
                if(espacos[i].liga[h] >= 0){
                    ligacaoLocal = findLocalById(espacos, i+1, size, espacos[i].liga[h]);
                    if(ligacaoLocal == NULL)
                        return FALSE;
                    isConnectionValid = validateEspacosConnection(espacos+i, ligacaoLocal);
                    
                    if(isConnectionValid == FALSE)
                        return FALSE;
                }else if(espacos[i].liga[h] < -1){
                    printf("Erro\n");
                    return FALSE;
                }
            }
        }
    }
    return TRUE;
}

ambiente *getTransferPeopleOrigin(ambiente *environment, int sizeEnvironment, int *envOriginIndex){
    int idOrigin;
    do{
        printf("Id local origem: ");
        scanf("%d", &idOrigin);
        if(idOrigin == -1){
            printf("# Acao cancelada.\n");
            return NULL;
        }

        *envOriginIndex = findLocalInEnvById(environment, idOrigin, sizeEnvironment);

        if(*envOriginIndex == -1){
            printf("# ERRO: Espaco com o id %d não existe.\n", idOrigin);
        }
        
    }while(*envOriginIndex == -1);
    return environment + *envOriginIndex;
}

ambiente *getTransferPeopleDestination(ambiente *environment, int sizeEnvironment, int envOriginIndex, ambiente envOrigin){
    int isEnvDestinationConnectionValid;
    int idDestination, envDestinationIndex;

    do{
        printf("Id local destination: ");
        scanf("%d", &idDestination);


        if(idDestination == -1){
            printf("# Acao cancelada.\n");
            return NULL;
        }

        envDestinationIndex = findLocalInEnvById(environment, idDestination, sizeEnvironment);
        isEnvDestinationConnectionValid = checkIfConnectionValid(idDestination, envOrigin.local);
        
        if(envDestinationIndex == -1)
            printf("# ERRO: Espaco com o id %d não existe.\n", idDestination);
        else if(isEnvDestinationConnectionValid == FALSE)
            printf("# ERRO: Local de destino não está ligado ao local de origem!\n");
        else if(environment[envDestinationIndex].nPeople >= environment[envDestinationIndex].local->capacidade){
            printf("# ERRO: Espaco com o id %d está cheio, tente outro espaco.\n", idDestination);
        }
    
    }while(envDestinationIndex == -1 || envDestinationIndex == envOriginIndex 
        || isEnvDestinationConnectionValid == FALSE || environment[envDestinationIndex].nPeople >= environment[envDestinationIndex].local->capacidade);

    return environment+envDestinationIndex;
}

int getTransferPeopleQuant(int sizeEnvironment, ambiente envOrigin, ambiente envDestination){
    int quantPeople;
    do{
        printf("Quantas pessoas pretende transferir: ");
        scanf("%d", &quantPeople);

        if(quantPeople > envOrigin.nPeople)
            printf("# ERRO: Nº de pessoas a transferir excede o nº de pessoas no local!\n");
        else if(quantPeople < 0)
            printf("# ERRO: Nº de pessoas inválido\n");
 
    }while(quantPeople < 0 || quantPeople > envOrigin.nPeople);
    printf("%d %d %d %d\n", sizeEnvironment, envOrigin.nPeople, envDestination.nPeople, quantPeople);

    return quantPeople;
}

void printEnv(ambiente *environment, int size){
    int i;
    linkedList *envPop;
    for(i = 0; i<size;i++){
        envPop = environment[i].population;
        printf("LOCAL ID => %d %d %d\n", environment[i].local->id, environment[i].nPeople, environment[i].local->capacidade);
        while(envPop != NULL){
            // printf("->LOCAL ID => %d %d\n", environment[i].local->id, environment[i].population == NULL);
            printf("%c ", envPop->p.state);
            envPop = envPop->next;
        }
        printf("\n\n");
    }
}




