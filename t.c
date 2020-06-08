#include <stdio.h>
#include <stdlib.h>

typedef struct sala local, *plocal;

struct sala {
    int id;
    int capacidade;
    int liga[3];
};

int main() {
    FILE *f;
    int i=0, j, x=0,total_salas= 0,k=0,posicao=0,l=0;

    plocal aux;
    plocal shit;


    f = fopen("E2.bin", "rb");
    if (f == NULL) {
        printf("Erro no acesso ao ficheiro\n");
        return 1;
    }

    aux = malloc(sizeof (struct sala));
    if (aux == NULL) {
        printf("Erro na alocacao de memoria");
        return 0;
    }
   while(1){
        fread(aux, sizeof (struct sala), 1, f);
        shit = realloc(shit, sizeof (struct sala));
        shit[i].id = aux->id;
        shit[i].capacidade = aux->capacidade;
        for (j = 0; j < 3; j++) {
            shit[i].liga[j] = aux->liga[j];
        }
        if(feof(f))
            break;
        i++;

    }
    total_salas=i;

    fclose(f);
    for (i = 0; i < total_salas; i++) {
        printf("\n \nid da sala =%d\n", shit[i].id);
        printf("capacidade da sala= %d\n", shit[i].capacidade);
        for (j = 0; j < 3; j++) {
            printf("\nsalas ligadas: %d",shit[i].liga[j] );
            if (shit[i].liga[j] > 0) {
                printf("\n%d",shit[i].liga[j]);
                while(shit[i].liga[j] != shit[k].id && k<total_salas){
                printf("\n%d",k);
                    k++;
                }

            }
        }
    }
    return 0;
}