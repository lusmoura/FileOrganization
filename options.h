#ifndef OPTIONS_H
#define OPTIONS_H

#include "list.h"

/*------------------------------- op1 -------------------------------------*/
/* Faz as operacoes referentes a opcao 1, isto eh, abre um arquivo csv, le */
/* o cabecalho e os registros e escreve todos os dados num arquivo binario */
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
void op1(char* fileName);

/*------------------------------- op2 -------------------------------------*/
/* Faz as operacoes referentes a opcao 2, isto eh, abre um arquivo binario */
/* e le todos os registros, imprimindo-os ou indicando possiveis erros     */
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
void op2(char* fileName);

/*------------------------------- op3 -------------------------------------*/
/* Faz as operacoes referentes a opcao 3, isto eh, faz a busca de registos */
/* no arquivo binario. Para isso, le registro por registro e compara os    */
/* campos do registro que esta aberto com os dados procurados			   */
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   char* fieldName - nome do campo que se busca                */
/* 			   char* value - valor do campo que se busca                   */
void op3(char* fileName, char* fieldName, char* value);

/*------------------------------- op4 -------------------------------------*/
/* Faz as operacoes referentes a opcao 4, isto eh, faz a busca de registos */
/* remove do arquivo os registros encontrados em buscas por um certo campo */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   int times - quantidade de chaves a serem buscadas           */
/* 			   list* l - lista de removidos                                */
void op4(char* fileName, int times, list* l);

/*------------------------------- op5 -------------------------------------*/
/* Faz as operacoes referentes a opcao 5, isto eh, lê campos e cria um novo*/
/* registro com esses dados. Em seguida o insere numa posição disponível   */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   int times - quantidade de registros a serem inseridos       */
/* 			   list* l - lista de removidos                                */
void op5(char* fileName, int times, list* l);

//
void op6();

#endif