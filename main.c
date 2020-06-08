/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: hugogomes
 *
 * Created on 31 de março de 2020, 16:08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"
#include "historico.h"
#include "people.h"
#include "espacos.h"

historico *hist;
int *espacosCapacity;
ambiente *environment;

int totalSpaceSize = 0;
int totalIterations = 0;

local *loadEspacos(FILE *fptr, char *binFileToOpen, int *size){
    int areEspacosValid;
    int i, h;
    local tmp;
    int insertIndex;
    local *espacos;

    *size = findSize(binFileToOpen) / sizeof(local);
    espacos = malloc(sizeof(local) * (*size));
    espacosCapacity = malloc(sizeof(int) * (*size));
    environment = malloc(sizeof(ambiente) * (*size));

    if(espacos == NULL || espacosCapacity == NULL || environment == NULL){
        printf("# ERRO: Erro ao alocar memória.");
        exit(1);
    }

    fptr = fopen(binFileToOpen, "rb");

    if(fptr == NULL){
        printf("# ERRO: Erro ao ler o ficheiro binário.");
        exit(1);
    }

    for(i = 0; i < *size; i++){
        fread(&tmp, sizeof(local), 1, fptr);
        printf("%d  %d\n", i, tmp.id);
        insertIndex = searchInsertIndex(espacos, tmp.id, i);
        insertLocalInEspaco(espacos, tmp, insertIndex, *size);
        espacosCapacity[i] = tmp.capacidade;
        environment[i].local = espacos+i;
        environment[i].population = NULL;
        environment[i].nPeople = 0;
        totalSpaceSize += tmp.capacidade;
    }

    fclose(fptr);
    
    areEspacosValid = validateEspacos(espacos, *size);
 
    if(areEspacosValid == TRUE){
        return espacos;
    }else{
        return NULL;
    }
}

int loadPeople(FILE *fptr, char *txtFileToOpen, local *espacos, int *sizeEspacos){
    linkedList *newPersonNode;
    person newPerson;
    int nPeople = 0, randInt, valid;

    fptr = fopen(txtFileToOpen, "r");

    if(fptr == NULL){
        printf("# ERRO: Erro ao abrir o ficheiro txt\n\n");
        exit(1);
    }

    while(fscanf(fptr, "%s %d %c", newPerson.id, &(newPerson).age, &(newPerson).state) != EOF
        && nPeople < totalSpaceSize){
        if(newPerson.state == 'D')
            fscanf(fptr, "%d", &(newPerson).nDaysInfected);
        else
            newPerson.nDaysInfected = 0;

        valid = validatePerson(newPerson);
        if(valid == FALSE){
            printf("# ERRO:  Erro ao ler a lista de pessoas!\n");
            return FALSE;
        }

        do{
            randInt = intUniformRnd(0, (*sizeEspacos)-1);
        }while(environment[randInt].nPeople >= environment[randInt].local->capacidade);
        
        newPersonNode = malloc(sizeof(linkedList));
        newPersonNode->p = newPerson;
        newPersonNode->next = NULL;

        moveLLItem(environment+randInt, newPersonNode);
        environment[randInt].nPeople++;
        nPeople++;
    }
        


        printf("PPP => %d\n\n", nPeople);
    fclose(fptr);


    return TRUE;
} 
 
void fase_leitura(local *espacos, int *sizeEspacos){
    FILE *fptr;
    char binFileToOpen[20],
         txtFileToOpen[20];
    int statusLeituraPessoas;

    printf("Insira o nome do ficheiro que contem os locais: ");
    scanf(" %s", binFileToOpen);
    printf("Insira o nome do ficheiro que contem as pessoas: ");
    scanf(" %s", txtFileToOpen);

    espacos = loadEspacos(fptr, binFileToOpen, sizeEspacos);
    if(espacos != NULL){
        printf("# SUCESSO: Locais guardados com sucesso!\n\n");
    }else{
        printf("# ERRO: Erro ao ler os locais. Dados inválidos!\n\n");
        exit(1);
    }

    statusLeituraPessoas = loadPeople(fptr, txtFileToOpen, espacos, sizeEspacos);
    if(statusLeituraPessoas == FALSE){
        printf("# ERRO: Erro ao ler as pessoas! Dados inválidos\n\n");
        exit(1);
    }else
        printf("# SUCESSO: Pessoas guardadas co sucesso!\n\n");
}


int recoverPerson(linkedList *env){
    int probRecup = probEvento(1/env->p.age);
    int taxaImunidade;
    if(probRecup == TRUE || env->p.nDaysInfected >= 5+(env->p.age/10)){
        taxaImunidade = probEvento(0.2);
        if(taxaImunidade == TRUE){
            env->p.state = 'I';
        }else{
            env->p.state = 'S';
        }
        env->p.nDaysInfected = 0;
        return TRUE;
    }
    return FALSE;
}

void disseminateInfection(int taxaDisseminacao, int envPos){
    int j;
    int randIndex;
    linkedList *newInfectedPointer;

    for(j = 0; j < taxaDisseminacao; j++){
        do{
            randIndex = intUniformRnd(0, environment[envPos].nPeople-1);
            newInfectedPointer = getLLItem((environment+1)->population, randIndex);
        }while(newInfectedPointer->p.state == 'I');
        newInfectedPointer->p.state = 'D';
    }
}

void transferPeople(ambiente *envOrigin, ambiente *envDestination, int *randomIndexes, int quantPeople, linkedList *prevOriginPop, linkedList *currOriginPop){
    int i = 0, randomIndexController = 0;
    
    while(i < envOrigin->nPeople && randomIndexController < quantPeople && currOriginPop != NULL){
        if(i == randomIndexes[randomIndexController]){
            moveLLItem(envDestination, currOriginPop);
            if(prevOriginPop != NULL){                              // Item está no meio da lista
                prevOriginPop->next = currOriginPop->next;
                currOriginPop->next = NULL;
                currOriginPop = prevOriginPop->next;
            }else{
                if(currOriginPop->next == NULL){                    // É o ultimo item na lista
                    envOrigin->population = NULL;
                }else{                                              // Item está em primeiro na lista
                    envOrigin->population = currOriginPop->next;
                    currOriginPop->next = NULL;
                    currOriginPop = envOrigin->population;
                }
            }
            randomIndexController++;
        }else{
            prevOriginPop = currOriginPop;
            currOriginPop = currOriginPop->next;
        }
        i++;
    }
}

void writeFinalPopulationFile(int sizeEnvironment){
    FILE *f;
    linkedList *prevPerson, *currPerson;
    int i;

    f = fopen("final_population.txt", "w");

    for(i = 0; i < sizeEnvironment; i++){
        prevPerson = NULL;
        currPerson = environment[i].population;

        while(currPerson != NULL){
            fprintf(f, "%s\t%d\t%c", currPerson->p.id, currPerson->p.age, currPerson->p.state);
            if(currPerson->p.state == 'D')
                fprintf(f, "\t%d\n", currPerson->p.nDaysInfected);
            else
                fprintf(f, "\n");
            if(prevPerson != NULL)
                free(prevPerson);

            prevPerson = currPerson;
            currPerson = currPerson->next;
        }
        prevPerson = currPerson;
    }
    free(environment);
    free(espacosCapacity);
    fclose(f);
}


void simulateAction(int *sizeEnvironment){
    int taxaDisseminacao, isPersonRecovered,
        durMaxInfection, taxaImunidade;
    int i, j;
    int randIndex;
    linkedList *envPopulation;
    linkedList *newInfectedPointer;

    addToHistory(hist, environment, &totalIterations, *sizeEnvironment);
    totalIterations++;
    for(i = 0; i < *sizeEnvironment; i++){
        taxaDisseminacao = environment[i].nPeople * 0.05;
        envPopulation = environment[i].population;

        while(envPopulation != NULL){
            if(envPopulation->p.state == 'D'){
                
                isPersonRecovered = recoverPerson(envPopulation);
                if(isPersonRecovered == TRUE)
                    continue;

                disseminateInfection(taxaDisseminacao, i);
                envPopulation->p.nDaysInfected++;
            }
            envPopulation = envPopulation->next;
        }

    }
    printEnv(environment, *sizeEnvironment);
}

void multipleSimulationAction(int *sizeEnvironment){
    int nSimulations, i;

    printf("Insira o nº de simulacoes a iterar: ");
    scanf("%d", &nSimulations);

    for(i = 0; i < nSimulations; i++){
        simulateAction(sizeEnvironment);
    }
}

void displayStatisticAction(int sizeEnvironment){
    int i, h, totalData[3] = {0, 0, 0}; // (0)Saudaves - (1)Doentes - (2)Imunes
    linkedList *envPop;
    for(i = 0; i < sizeEnvironment; i++){
        envPop = environment[i].population;

        totalData[0] = 0;
        totalData[1] = 0;
        totalData[2] = 0;
      printf("\n=============================\n");
        printf("| ID: %2d                    |\n", environment[i].local->id);
        printf("| MAX: %2d                   |\n", environment[i].local->capacidade);
        printf("|             (n pessoas/%c) |\n", '%');
        printf("-----------------------------\n");
        // for(h = 0; h < environment[i].nPeople; h++){
        if(environment[i].nPeople > 0){
            while(envPop != NULL){
                switch (envPop->p.state){
                    case 'S':
                        totalData[0]++;
                        break;
                    case 'D':
                        totalData[1]++;
                        break;
                    case 'I':
                        totalData[2]++;
                        break;
                    default:
                        break;
                }

                envPop = envPop->next;
            }
            printf("| Saudaveis:   %2d/%4.2f%c    |\n", totalData[0], (float) totalData[0]*100/environment[i].nPeople, '%');
            printf("| Doentes:     %2d/%4.2f%c    |\n", totalData[1], (float) totalData[1]*100/environment[i].nPeople, '%');
            printf("| Imunes:      %2d/%4.2f%c    |\n", totalData[2], (float) totalData[2]*100/environment[i].nPeople, '%');
            printf("-----------------------------\n");
            printf("| Total de pessoas:  %d     |\n", environment[i].nPeople);
        }else{
            printf("| Local vazio               |\n");
        }
        printf("=============================\n");
    }
}
 
void addInfectedPersonAction(int sizeEnvironment){
    int localIdToSearch, envPos;
    person newPerson;
    linkedList *newPersonNode = malloc(sizeof(linkedList));

    do{
        printf("Indique o id do local ao qual pretende adicionar um doente: ");
        scanf("%d", &localIdToSearch);
        if(localIdToSearch == -1){
            printf("# Acao cancelada.\n");
            return;
        }

        envPos = findLocalInEnvById(environment, localIdToSearch, sizeEnvironment);

        if(envPos == -1){
            printf("# ERRO: Espaco com o id %d não existe.\n", localIdToSearch);
        }else if(environment[envPos].nPeople >= environment[envPos].local->capacidade){
            printf("# ERRO: Espaco com o id %d está cheio, tente outro espaco.\n", localIdToSearch);
        }

    }while(envPos == -1 || environment[envPos].nPeople >= environment[envPos].local->capacidade);

    printf("Selected id %d\n", (environment+envPos)->local->id);
    
    printf("Introduza as informações do doente\n");
    printf("Id: ");
    scanf("%s", newPerson.id);
    printf("Idade: ");
    // fflush(stdin);
    scanf("%d", &newPerson.age);
    printf("Nº dias infetado: ");
    scanf("%d", &newPerson.nDaysInfected);
    newPerson.state = 'D';

    printf("%s %d %c %d\n", newPerson.id, newPerson.age, newPerson.state, newPerson.nDaysInfected);

    newPersonNode->p = newPerson;
    newPersonNode->next = NULL;

    moveLLItem((environment+envPos), newPersonNode);
    (environment+envPos)->nPeople++;

    printf("Nova pessoa infetada adicionada: %s %d %c %d\n", newPerson.id, newPerson.age, newPerson.state, newPerson.nDaysInfected);
    printEnv(environment, sizeEnvironment);
}
 
void transferPeopleAction(int sizeEnvironment){
    int envOriginIndex;
    int quantPeople, *randomIndexes;
    ambiente *envOrigin, *envDestination;
    linkedList *currOriginPop, *prevOriginPop;

    envOrigin = getTransferPeopleOrigin(environment, sizeEnvironment, &envOriginIndex);
    if(envOrigin == NULL)
        return;

    envDestination = getTransferPeopleDestination(environment, sizeEnvironment, envOriginIndex, *envOrigin);
    if(envDestination == NULL)
        return;
    quantPeople = getTransferPeopleQuant(sizeEnvironment, *envOrigin, *envDestination);

    do{
        randomIndexes = malloc(sizeof(int) * quantPeople);
    }while(randomIndexes == NULL);
    prevOriginPop = NULL;
    currOriginPop = envOrigin->population;

    getRandomPeopleIndexArray(randomIndexes, envOrigin, prevOriginPop, currOriginPop, quantPeople);
    transferPeople(envOrigin, envDestination, randomIndexes, quantPeople, prevOriginPop, currOriginPop);
    
    envOrigin->nPeople-=quantPeople;
    envDestination->nPeople+=quantPeople;

    printf("\n# SUCCESS: Foram movidas com sucesso %d pessoas do local %d para o local %d\n\n", quantPeople, envOrigin->local->id, envDestination->local->id);

    printEnv(environment, sizeEnvironment);

    free(randomIndexes);
}

void writeFinalReportAction(int sizeEnvironment){
    FILE *f;

    f = fopen("report.txt", "w");

    if(f == NULL){
        printf("# ERRO: Erro ao abrir o ficheiro result.txt\n");
        exit(1);
    }

    int i, h, totalData[3] = {0, 0, 0}; // (0)Saudaves - (1)Doentes - (2)Imunes
    int totalSaudaveis = 0, totalDoentes = 0, totalImunes = 0;
    linkedList *envPop;
    for(i = 0; i < sizeEnvironment; i++){
        envPop = environment[i].population;

        totalData[0] = 0;
        totalData[1] = 0;
        totalData[2] = 0;
      fprintf(f, "\n=============================\n");
        fprintf(f, "| ID: %2d                    |\n", environment[i].local->id);
        fprintf(f, "| MAX: %2d                   |\n", environment[i].local->capacidade);
        fprintf(f, "|             (n pessoas/%c) |\n", '%');
        fprintf(f, "-----------------------------\n");
        // for(h = 0; h < environment[i].nPeople; h++){
        if(environment[i].nPeople > 0){
            while(envPop != NULL){
                switch (envPop->p.state){
                    case 'S':
                        totalData[0]++;
                        totalSaudaveis++;
                        break;
                    case 'D':
                        totalData[1]++;
                        totalDoentes++;
                        break;
                    case 'I':
                        totalData[2]++;
                        totalImunes++;
                        break;
                }

                envPop = envPop->next;
            }
            fprintf(f, "| Saudaveis:   %2d/%4.2f%c    |\n", totalData[0], (float) totalData[0]*100/environment[i].nPeople, '%');
            fprintf(f, "| Doentes:     %2d/%4.2f%c    |\n", totalData[1], (float) totalData[1]*100/environment[i].nPeople, '%');
            fprintf(f, "| Imunes:      %2d/%4.2f%c    |\n", totalData[2], (float) totalData[2]*100/environment[i].nPeople, '%');
            fprintf(f, "-----------------------------\n");
            fprintf(f, "| Total de pessoas:  %d     |\n", environment[i].nPeople);
        }else{
            fprintf(f, "| Local vazio               |\n");
        }
        fprintf(f, "=============================\n");
    }

    fprintf(f, "\nTotal de pessoas saudaveis: %d", totalSaudaveis);
    fprintf(f, "\nTotal de pessoas imunes: %d", totalImunes);
    fprintf(f, "\nTotal de pessoas doentes: %d\n", totalDoentes);
    fprintf(f, "\nTotal de iteracoes: %d", totalIterations);
    fclose(f);

    writeFinalPopulationFile(sizeEnvironment);

    printf("Relatorio final guardado em report.txt\n");
    printf("Populacao final guardada em final_population.txt\n");
}

void regressXDaysAction(int sizeEnvironment){
    int nDays, i, h;
    do{
        printf("Quantos dias pretende regredir na simulacao? (MAX: 3 dias) ");
        scanf("%d", &nDays);
        if(nDays == -1){
            printf("# Acao cancelada.\n");
            return;
        }

        if(nDays < 0 || nDays > 3)
            printf("# ERRO: N de dias a regredir na simulacao é inválido.\n");
        else if(nDays > totalIterations)
            printf("# ERRO: N de dias a regredir na simulacao é excede o numero de iteracoes realizada.\n");
        else if(hist[nDays-1].iterationNumber == -1)
            printf("# ERRO: N de dias a regredir na simulacao é excede o numero de iteracoes guardadas no historico.\n");

    }while(nDays < 0 || nDays > 3 || nDays > totalIterations || hist[nDays-1].iterationNumber == -1);

    if(nDays != 0){
        recoverHistoryState(hist, environment, &totalIterations, sizeEnvironment, nDays);
        shiftHistory(hist, nDays);
    }
    
    printf("==> TOTAL ITERATIONS: %d\n", totalIterations);
    printEnv(environment, sizeEnvironment);
}


void fase_simulacao(int *sizeEnvironment){
    int action;

    do{
    printf("\n\n#############################################\n");
        printf("#    (1) Avancar 1 iteracao na simulacao    #\n");
        printf("#    (2) Avancar x iteracoes na simulacao   #\n");
        printf("#    (3) Apresentar estatistica             #\n");
        printf("#    (4) Adicionar doente                   #\n");
        printf("#    (5) Transferir pessoas                 #\n");
        printf("#    (6) Regredir x dias na simulação       #\n");
        printf("#    (7) Terminar simulacao                 #\n");
        printf("#############################################\n");

        printf("Insira o numero da acao que pretenda executar: ");
        scanf("%d", &action);
        putchar('\n');
 
        switch (action){
            case 1:
                simulateAction(sizeEnvironment);
                break;
            case 2:
                multipleSimulationAction(sizeEnvironment);
                break;
            case 3:
                displayStatisticAction(*sizeEnvironment);
                break;
            case 4:
                addInfectedPersonAction(*sizeEnvironment);
                break;
            case 5:
                transferPeopleAction(*sizeEnvironment);
                break;
            case 6:
                regressXDaysAction(*sizeEnvironment);
                break;
            case 7:
                writeFinalReportAction(*sizeEnvironment);
                break;
        }
    }while(action != 7);
}

int main(int argc, char** argv) {
    local *espacos;
    int *sizeEspacos = malloc(sizeof(int));
    int statusFaseLeitura;
    int i;

    hist = malloc(sizeof(historico) * 3);
    for(i = 0; i < 3; i++){
        hist[i].env = NULL;
        hist[i].iterationNumber = -1;
    }

    initRandom(); // Random init
    
    fase_leitura(espacos, sizeEspacos);

    fase_simulacao(sizeEspacos);

    printf("\n\nObrigado por usar o simulador. STAY HOME!!!\n\n");

    free(sizeEspacos);
    free(espacos);
    free(hist);
    return (EXIT_SUCCESS);
}

