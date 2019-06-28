#ifndef INDEXHEADER_H
#define INDEXHEADER_H

/*	Struct Cabeçalho de Indice  */
/*
		- char status - indicador da consistência do arquivo
		- int nroRegistros - quantidade de registros salvos no arquivo
*/
typedef struct _indexHeader {
    char status;
    int nroRegistros;
} IndexHeader;

/*------------------------- Create Index Header ---------------------------*/
/* Cria um indexHeader zerado                                       	   */
/*																		   */
/* Retorno: IndexHeader* - IndexHeader criado							   */
IndexHeader* createIndexHeader();

/*-------------------------- Read Index Header ----------------------------*/
/* Le header de um arquivo binario e o armazena na struct IndexHeader	   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o Header   */
/* Retorno: IndexHeader* - Header lido e criado							   */
IndexHeader* readIndexHeader(FILE* indFile);

/*------------------------------ Update Header ----------------------------*/
/* Atualiza o Header, alterando seu status e a quantidade de resgistros    */
/*																		   */
/* Parametros: IndexHeader* indHeader - header a ser atualizado            */
/*			   int amount - nova quantidade de registros				   */
void updateIndexHeader(IndexHeader* indHeader, int amount);

/*------------------------------- Write Header ----------------------------*/
/* Escreve o Header no arquivo binario									   */
/*																		   */
/* Parametros: FILE* indexFile - arquivo no qual o Header deve ser escrito */
/*			   IndexHeader* indHeader - Header a ser escrito		       */
void writeIndexHeader(IndexHeader* indHeader, FILE* indexFile);

/*------------------------------ Print Header -----------------------------*/
/* Imprime valores armazenados no Header  								   */
/*																		   */
/* Parametros: Header* header - Header a ser impresso   				   */
void printIndexHeader(IndexHeader* indHeader);

#endif