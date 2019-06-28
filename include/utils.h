#ifndef UTILS_H
#define UTILS_H

#include "registers.h"
#include "header.h"
#include "list.h"

#define pageSize 32000
#define constantSize 34
#define maxSize 6005

extern char Lixo;
extern int64_t oo;

/*	Struct Query  */
/*
		- char* fieldName - nome do campo da operação
		- char* value - nome do valor
		- int timeIn - tempo de inserção da query
*/
typedef struct _query {
    char fieldName[50];
    char value[300];
    int timeIn;
} Query;

/*--------------------------------- Size ----------------------------------*/
/* Calcula tamanho de um arquivo sem alterar a posicao do seu ponteiro	   */
/*																		   */
/* Parametros: FILE* fp - arquivo para qual o tamanho sera calculado	   */
/* Retorno: int - tamanho do arquivo									   */
int fileSize(FILE* fp);

/*--------------------------- Binario na Tela -----------------------------*/
/* Função fornecida pelo monitor da matéria 							   */
void binarioNaTela1(FILE *ponteiroArquivoBinario);

/*--------------------------- Scan Quote String ---------------------------*/
/* Função fornecida pelo monitor da matéria 							   */
void scan_quote_string(char *str);

/*----------------------------- id Comparator -----------------------------*/
/* Comparador para a ordenação. Utiliza os valores do id dos registros como*/
/* chaves para ordenar.                                                    */
/*                                                                         */
/* Parâmetros: const void* fst - ponteiro para primeiro elemento comparado */
/*             const void* snd - ponteiro para segundo elemento comparado  */
int idComparator(const void* a, const void* b);

/*---------------------------- index Comparator ---------------------------*/
/* Comparador para a ordenação. Utiliza os valores dos nomes nos indices   */
/* como chaves para ordenar e o offset como desempate.                     */
/*                                                                         */
/* Parâmetros: const void* fst - ponteiro para primeiro elemento comparado */
/*             const void* snd - ponteiro para segundo elemento comparado  */
int indexComparator(const void* a, const void* b);

/*------------------------------ Real Size --------------------------------*/
/* Calcula o tamanho de uma string, desconsiderando os @                   */
/*                                                                         */
/* Parametros: char* name - palavra da qual se quer o tamanho              */
int realSize(char* name);

/*--------------------------- Fill With Trash -----------------------------*/
/* Escreve uma determinada quantidad de @ no arquivo                       */
/*                                                                         */
/* Parametros: FILE* input - arquivo no qual será feita a escrita          */
/*             int amount - quantidade de @ para escrever                  */
void fillWithTrash(FILE* input, int amount);

#endif
