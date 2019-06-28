#ifndef INDEX_H
#define INDEX_H

#include <stdint.h>
#include "indexHeader.h"

/*	Struct Indice  */
/*
		- char* chaveBusca - nome que estava no registro
		- int64_t byteoffset - posição do resgistro no arquivo de dados
*/
typedef struct _index {
    char chaveBusca[120];
    int64_t byteOffset;
} Index;


/*------------------------------ Create Index -----------------------------*/
/* Cria um novo indice                                                 	   */
/*																		   */
/* Retorno: IndexHeader - IndexHeader criado							   */
Index createIndex(int pos, char* name);

/*---=-------------------------- Read Index -------------------------------*/
/* Le indice de um arquivo binario e o armazena na struct Index     	   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o Header   */
/* Retorno: Index* - Indice lido e criado							       */
Index readIndex(FILE* indexFile);

/*------------------------------- Insert Index ----------------------------*/
/* Insere um novo indice no vetor a partir de um regustro       		   */
/*																		   */
/* Parametros: Reg* reg - Registro cujo nome será inserido no vetor        */
/*             Index* indexes - vetor de indices                           */
/*			   IndexHeader* indHeader - Header do arquivo                  */
/*             int pos - byteoffset do registro                            */
void insertIndex(Register* reg, Index* indexes, IndexHeader* indHeader, int pos);

/*------------------------------- Remove Index ----------------------------*/
/* Remove logicamente um indice do vetor de indices                		   */
/*																		   */
/* Parametros: Index* indexes - indice a ser removido                      */
void removeIndex(Index* index);

/*------------------------------- Write Index -----------------------------*/
/* Escreve o indice no arquivo binario									   */
/*																		   */
/* Parametros: FILE* indexFile - arquivo no qual o indice deve ser escrito */
/*			   Index* index - indice a ser escrito          		       */
void writeIndex(Index* index, FILE* indexFile);

/*------------------------------ Print Index ------------------------------*/
/* Imprime valores armazenados no indice  								   */
/*																		   */
/* Parametros: Index* index - Index a ser impresso   				       */
void printIndex(Index* index);

/*---------------------------- Binary Search ------------------------------*/
/* Faz uma busca binária no vetor de indices procurando um determinado nome*/
/*																		   */
/* Parametros: Index* indexes - vetor de indices       				       */
/*             char* nome - nome a ser buscado                             */
/*             int amount - quantidade de indices no vetor                 */
int binarySearch(Index* indexes, char* name, int amount);

/*-------------------------- Rewrite Index File ---------------------------*/
/* Reescreve todo o arquivo de indices após as modificações feitas em RAM  */
/*																		   */
/* Parâmetros: FILE* indexFile - arquivo no qual será feita a escrita      */
/*			   IndexHeader* indHeader - novo Header do arquivo             */
/*             Index* indexes - vetor de indices atualizado 		       */
void rewriteIndexFile(FILE* indexFile, IndexHeader* indHeader, Index* indexes);

#endif