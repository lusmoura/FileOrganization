#ifndef UTILS_H
#define UTILS_H

#include "registers.h"
#include "header.h"
#include "list.h"

#define pageSize 32000

extern char lixo;

/*----------------------------- Update Link -------------------------------*/
/* Atualiza encadeamento da lista de removidos fisicamente no arquivo	   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   int pos - posição do registro que será removido			   */
/* 			   list* l - lista ordenada com os registros removidos 		   */
void updateLink(FILE* input, list* l);

/*--------------------------------- Size ----------------------------------*/
/* Calcula tamanho de um arquivo sem alterar a posicao do seu ponteiro	   */
/*																		   */
/* Parametros: FILE* fp - arquivo para qual o tamanho sera calculado	   */
/* Retorno: int - tamanho do arquivo									   */
int fileSize(FILE* fp);

/*-------------------------- Create Removed List --------------------------*/
/* Cria lista de removidos a partir dos registros que já estão removidos no*/
/* arquivo de entrada.													   */
/*																		   */
/* Parametros: FILE* fp - arquivo do qual serão lidos os registros   	   */
/* 			   list* l - lista na qual serão inseridos os registros   	   */
/*			   int64_t firstPos - posição 							   */
void createRemovedList(FILE* input, list* l, int64_t firstPos);	

/*--------------------------- Binario na Tela -----------------------------*/
/* Função fornecida pelo monitor da matéria 							   */
void binarioNaTela1(FILE *ponteiroArquivoBinario);

/*--------------------------- Scan Quote String ---------------------------*/
/* Função fornecida pelo monitor da matéria 							   */
void scan_quote_string(char *str);

#endif
