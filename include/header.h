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
typedef struct _Header {
	char status;
	int64_t topoLista;
	char tags[5];
	char campos[5][40];
} Header;

/*----------------------------- Create Header -----------------------------*/
/* Le Header de um arquivo csv e o escreve num arquivo binario			   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o Header   */
/*			   FILE* output - arquivo no qual sera escrito o Header        */
/* Retorno: Header* - Header lido e criado							       */
Header* createHeader(FILE* input, FILE* output);

/*------------------------------ Read Header ------------------------------*/
/* Le Header de um arquivo binario e o armazena na struct Header		   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o Header   */
/* Retorno: Header* - Header lido e criado							   */
Header* readHeader(FILE* input);

/*------------------------------ Update Header ----------------------------*/
/* Atualiza o Header e escreve no arquivo, mudando status quando o arquivo */
/* começa a ser alterado e tambem quando seu processamento acaba		   */
/*																		   */
/* Parametros: FILE* input - arquivo no qual o Header deve ser escrito     */
/*			   Header* header - Header a ser atualizado					   */
/*			   char status - status para o qual o Header será atualizado   */
void updateHeader(FILE* input, Header* header, char status);

/*----------------------------- Set As Consistent -------------------------*/
/* Atualiza arquivo, marcando o arquivo como consistente				   */
/*																		   */
/* Parâmetros: FILE* input - arquivo que será marcado como consistente     */
void setAsConsistent(FILE* input);

/*---------------------------- Set As Inconsistent ------------------------*/
/* Atualiza arquivo, marcando o arquivo como inconsistente				   */
/*																		   */
/* Parâmetros: FILE* input - arquivo que será marcado como inconsistente   */
void setAsInconsistent(FILE* input);

/*------------------------------- Write Header ----------------------------*/
/* Escreve o Header no arquivo binario									   */
/*																		   */
/* Parametros: FILE* input - arquivo no qual o Header deve ser escrito     */
/*			   Header* header - Header a ser escrito		    		   */
void writeHeader(FILE* input, Header* header);

/*------------------------------ Print Header -----------------------------*/
/* Imprime valores armazenados no Header  								   */
/*																		   */
/* Parametros: Header* header - Header a ser impresso   				   */
void printHeader(Header* header);

#endif