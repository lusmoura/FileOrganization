#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "index.h"
#include "indexHeader.h"

/*------------------------- Create Index Header ---------------------------*/
IndexHeader* createIndexHeader() {
    IndexHeader* indHeader = calloc(1, sizeof(IndexHeader));
    indHeader->nroRegistros = 0;
    indHeader->status = '0';

    return indHeader;
}

/*-------------------------- Read Index Header ----------------------------*/
IndexHeader* readIndexHeader(FILE* indexFile) {
    rewind(indexFile);
    IndexHeader *indHeader = calloc(1, sizeof(IndexHeader));

    fread(&(indHeader->status), sizeof(char), 1, indexFile);
    fread(&(indHeader->nroRegistros), sizeof(int), 1, indexFile);

    fseek(indexFile, pageSize, SEEK_SET);
    return indHeader;
}

/*------------------------- Update Index Header ---------------------------*/
void updateIndexHeader(IndexHeader* indHeader, int amount) {
    indHeader->nroRegistros = amount;
    indHeader->status = '1';
    
    return;
}

/*------------------------- Write Index Header ----------------------------*/
void writeIndexHeader(IndexHeader* indHeader, FILE* indexFile) {
    rewind(indexFile);
    if (indHeader == NULL || indexFile == NULL)
        return;

    // escrevre header
    fwrite(&(indHeader->status), 1, sizeof(char), indexFile);
    fwrite(&(indHeader->nroRegistros), 1, sizeof(int), indexFile);

    // prrence pagina com @
    fillWithTrash(indexFile, pageSize - sizeof(char) - sizeof(int));

    return;
}

/*-------------------------- Print Index Header ---------------------------*/
void printIndexHeader(IndexHeader* indHeader) {
    printf("Status: %c\n", indHeader->status);
    printf("Nro Regs: %d\n\n", indHeader->nroRegistros);
}