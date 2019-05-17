#ifndef HEADER_H
#define HEADER_H

#include <stdint.h>

/*	Struct Cabeçalho  */
/*
		- char status - indicador da consistência do arquivo
		- int topoLista - byte offset
		- char tags[] = vetor de letras que indicam as tags
		- char campos[][] = vetor de strings de tamanho 4 que tem a descrição do campo
*/
typedef struct _header {
	char status;
	int64_t topoLista;
	char tags[5];
	char campos[5][40];
} header;

/*----------------------------- Create Header -----------------------------*/
/* Le header de um arquivo csv e o escreve num arquivo binario			   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o header   */
/*			   FILE* output - arquivo no qual sera escrito o header        */
/* Retorno: header* - header lido e criado							       */
header* createHeader(FILE* input, FILE* output);

/*------------------------------ Read Header ------------------------------*/
/* Le header de um arquivo binario e o armazena na struct header		   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o header   */
/* Retorno: header* - header lido e criado							   */
header* readHeader(FILE* input);

/*------------------------------ Update Header ----------------------------*/
/* Atualiza o header e escreve no arquivo, mudando status quando o arquivo */
/* começa a ser alterado e tambem quando seu processamento acaba		   */
/*																		   */
/* Parametros: FILE* input - arquivo no qual o header deve ser escrito     */
/*			   header* h - header a ser atualizado						   */
/*			   char status - status para o qual o header será atualizado  */
void updateHeader(FILE* input, header* h, char status);

/*------------------------------- Write Header ----------------------------*/
/* Escreve o header no arquivo binario									   */
/*																		   */
/* Parametros: FILE* input - arquivo no qual o header deve ser escrito     */
/*			   header* h - header a ser escrito						   */
void writeHeader(FILE* input, header* h);

/*------------------------------ Print Header -----------------------------*/
/* Imprime valores armazenados no header  								   */
/*																		   */
/* Parametros: header* h - header a ser impresso   						   */
void printHeader(header* h);

#endif