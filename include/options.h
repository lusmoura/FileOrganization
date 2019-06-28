#ifndef OPTIONS_H
#define OPTIONS_H

#include "registers.h"
#include "header.h"
#include "utils.h"
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
void op3(char* fileName, char* fieldName, char* value, int* access);

/*------------------------------- op4 -------------------------------------*/
/* Faz as operacoes referentes a opcao 4, isto eh, faz a busca de registos */
/* remove do arquivo os registros encontrados em buscas por um certo campo */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   int times - quantidade de chaves a serem buscadas           */
void op4(char* fileName, int times);

/*------------------------------- op5 -------------------------------------*/
/* Faz as operacoes referentes a opcao 5, isto eh, lê campos e cria um novo*/
/* registro com esses dados. Em seguida o insere numa posição disponível   */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   int times - quantidade de registros a serem inseridos       */
void op5(char* fileName, int times);

/*------------------------------- op6 -------------------------------------*/
/* Faz as operacoes referentes a opcao 6, isto eh, busca um registro       */
/* registro com esses dados. Em seguida o insere numa posição disponível   */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   int times - quantidade de registros a serem atualizados     */
void op6(char* fileName, int times);

/*------------------------------- op7 -------------------------------------*/
/* Faz as operacoes referentes a opcao 7, isto eh, ordena registros de arquivo */
/*																		   */
/* Parametros: char* firstFileName - nome do primero arquivo a ser aberto  */
/* 			   char* secondFileName - nome do segundo arquivo a ser aberto */
void op7(char* firstFileName, char* secondFIleName);

/*------------------------------- op8 -------------------------------------*/
/* Faz as operacoes referentes a opcao 8, isto eh, faz merge de registros  */
/*																		   */
/* Parametros: char* firstFileName - nome do primero arquivo a ser aberto  */
/* 			   char* secondFileName - nome do segundo arquivo a ser aberto */
/*             char* outputName - nome do arquivo de saída                 */
void op8(char* fileName, char* fileName2, char* outputName);

/*------------------------------- op9 -------------------------------------*/
/* Faz as operacoes referentes a opcao 9, isto eh, faz match de registros  */
/*																		   */
/* Parametros: char* firstFileName - nome do primero arquivo a ser aberto  */
/* 			   char* secondFileName - nome do segundo arquivo a ser aberto */
/*             char* outputName - nome do arquivo de saída                 */
void op9(char* fileName, char* fileName2, char* outputName);

/*--------------------------------- op10 ----------------------------------*/
/* Faz as operacoes referentes a opcao 10, isto eh, cria um arquivo de indices    */
/*																		   */
/* Parametros: char* fileName - nome do primero arquivo a ser aberto       */
/* 			   char* fileName2 - nome do segundo arquivo a ser aberto (de indices)*/
void op10(char* fileName, char* fileName2);

/*--------------------------------- op11 ----------------------------------*/
/* Faz as operacoes referentes a opcao 11, isto eh, busca registros pelo   */
/* arquivo de indices                                                      */
/*																		   */
/* Parametros: char* fileName - nome do primero arquivo a ser aberto       */
/* 			   char* fileName2 - nome do segundo arquivo a ser aberto (de indices)  */
/*             char* fieldName - nome do campo a ser buscado               */
/*             char* name - valor a ser buscado                            */
/*             int* access - ponteiro para inteiro que terá a qtd de acessos a disco*/
/*                         '-> se for enviado com 1, será impressa a qtd de acessos */ 
void op11(char* fileName, char* fileName2, char* fieldName, char* name, int* access);

/*--------------------------------- op12 ----------------------------------*/
/* Faz as operacoes referentes a opcao 12, isto eh, remove um registro do  */
/* arquivo de dados e do arquivo de indices                                */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   char* fileName2 - nome do segundo arquivo a ser aberto (de indices)  */
/* 			   int times - quantidade de registros a serem atualizados     */
void op12(char* fileName, char* fileName2, int times);

/*--------------------------------- op13 ----------------------------------*/
/* Faz as operacoes referentes a opcao 13, isto eh, insere um novo registro*/
/* no arquivo de dados e no arquivo de indices                             */ 
/*																		   */
/* Parametros: char* fileName - nome do arquivo a ser aberto               */
/* 			   char* fileName2 - nome do segundo arquivo a ser aberto (de indices)  */
/* 			   int times - quantidade de registros a serem atualizados     */
void op13(char* fileName, char* fileName2, int times);

/*--------------------------------- op14 ----------------------------------*/
/* Faz as operacoes referentes a opcao 14, isto eh, executa uma busca com e*/
/* sem o arquivo de indices, indicando a diferença de acessos entre elas   */
/*																		   */
/* Parametros: char* fileName - nome do primero arquivo a ser aberto       */
/* 			   char* fileName2 - nome do segundo arquivo a ser aberto (de indices)  */
/*             char* fieldName - nome do campo a ser buscado               */
/*             char* valor - valor a ser buscado                           */
void op14(char* fileName, char* fileName2, char* fieldName, char* value);

#endif