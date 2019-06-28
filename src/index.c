#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "index.h"

/*---------------------------- Create Index -------------------------------*/
Index createIndex(int pos, char* name) {
    Index index;
    
    index.byteOffset = pos;
	strcpy(index.chaveBusca, name);
    
    return index;
}

/*----------------------------- Read Index --------------------------------*/
Index readIndex(FILE* indexFile) {
    Index tempIndex;
    fread(&(tempIndex.chaveBusca), sizeof(char), 120, indexFile);
    fread(&(tempIndex.byteOffset), sizeof(int64_t), 1, indexFile);

    return tempIndex;
}

/*---------------------------- Insert Index -------------------------------*/
void insertIndex(Register* reg, Index* indexes, IndexHeader* indHeader, int pos) {
    int amount = indHeader->nroRegistros;
    
    strcpy(indexes[amount].chaveBusca, reg->nomeServidor);
    indexes[amount].byteOffset = pos;
    
    (indHeader->nroRegistros)++;
    
    return;
}

/*----------------------------- Remove Index ------------------------------*/
void removeIndex(Index* index) {
    // preenche o indice com valores altos
    strcpy(index->chaveBusca, "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz");
    index->byteOffset = oo;
    
    return;
}

/*----------------------------- Write Index -------------------------------*/
void writeIndex(Index* index, FILE* indexFile) {
    if (index == NULL)
        return;

    int size = strlen(index->chaveBusca);
    char endLine = '\0';

    // escreve nome
    fwrite(&(index->chaveBusca), 1, size, indexFile);
    fwrite(&endLine, 1, sizeof(char), indexFile);

    // prrenche campo de tamanho fixo com @
    fillWithTrash(indexFile, 120 - size - 1);

    // escreve offset
    fwrite(&(index->byteOffset), 1, sizeof(int64_t), indexFile);

    return;
}

/*----------------------------- Print Index -------------------------------*/
void printIndex(Index* index) {
    printf("Nome: %s\n", index->chaveBusca);
    printf("Offset: %ld\n\n", index->byteOffset);
}

/*---------------------------- Binary Search ------------------------------*/
int binarySearch(Index* indexes, char* name, int amount) {
    int high = amount;
    int low = 0;

    while (high > low) {
        int mid = (high + low) / 2;
        int comp = strcmp(indexes[mid].chaveBusca, name);

        if (comp >= 0)
            high = mid;
        else if (comp < 0)
            low = mid + 1;
    }

    return low;
}

/*-------------------------- Rewrite Index File ---------------------------*/
void rewriteIndexFile(FILE* indexFile, IndexHeader* indHeader, Index* indexes) {
    if (indexFile == NULL)
        return;

    // escreve indice
    indHeader->status = '0';
    writeIndexHeader(indHeader, indexFile);

    // escrevev cada indice
    for (int i = 0; i < indHeader->nroRegistros; i++)
        writeIndex(indexes + i, indexFile);

    return;
}