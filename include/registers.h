#ifndef REGISTERS_H
#define REGISTERS_H

#include "header.h"
#include "utils.h"
#include "list.h"

/*	Struct Registro  */
/*		
		- char removido - caracter que indica se o registro foi removido
		- int tamanhoRegistro - tamanho total do registro
		- int64_t - inteiro de 8 bytes do offset do registro
		- int idServidor - numero de identificação do servidor
		- double salarioServidor - valor do salario do servidor
		- char telefoneServidor[20] - string do telefone do servidor
		- int tamNomeServidor - indicador de tamanho do nome do servidor
		- char nomeServidor[300] - string do nome do servidor
		- int tamCargoServidor - indicador de tamanho do cargo do servidor
		- char cargoServidor[300] - string do cargo do servidor
*/
typedef struct _reg {
	char removido;
	int tamanhoRegistro;
	int64_t encadeamentoLista;
	int idServidor;
	double salarioServidor;
	char telefoneServidor[20];
	int tamNomeServidor;
	char nomeServidor[300];
	int tamCargoServidor;
	char cargoServidor[300];
} reg;

/*---------------------------- Create Register ----------------------------*/
/* Recebe dados e, com eles, cria um registro 							   */
/*																		   */
/* Parametros: int id - id do servidor para qual o registro sera criado    */
/*             double salario - salario do servidor para qual o registro sera criado */
/*			   char* telefone - telefone do servidor para qual o registro sera criado*/
/*			   char* nome - nome do servidor para qual o registro sera criado        */
/*			   char* cargo - cargo do servidor para qual o registro sera criado      */
/* Retorno: reg* - registro criado								   */
reg* createRegister(int id, double salario, char* telefone, char* nome, char* cargo);

/*---------------------------- Read Register ------------------------------*/
/* Le um registro do arquivo binario e o armazena na struct reg			   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o registro */
/*			   int offset - posicao do ponteiro do arquivo antes da leitura*/
/*			   int* numRegs - endereco da variavel que armazena numero de regs*/
/* Retorno: reg* - registro lido e criado								   */
reg* readRegister(FILE* input, int offset, int* numRegs);

/*---------------------------- Write Register -----------------------------*/
/* Escreve registro em um arquivo binario    							   */
/*																		   */
/* Parametros: reg* r - registro a ser escrito							   */
/*			   FILE* output - arquivo no qual o registro sera escrito	   */
/*			   int lastSize - tamanho do ultimo registro lido              */
void writeRegister(reg* r, FILE* output, int lastSize);

/*--------------------------- Search Register -----------------------------*/
/* Busca por um valor de um determinado campo no registro				   */
/*																		   */
/* Parametros: reg* r - registro no qual o valor sera buscado			   */
/*			   header* h - header para saber qual a posicao do campo no reg*/
/*			   char* fieldName - nome do campo a ser buscado			   */
/*			   char* value - valor a ser buscado						   */
/* Retorno: int - indica se o valor foi encontrado (1 ser for, 0 se nao)   */
int searchRegister(reg* r, header* h, char* fieldName, char* value);

/*--------------------------- Print Register ------------------------------*/
/* Imprime valores armazenados no registro com as opcoes:				   */
/*		- 0 : debug														   */
/*		- 2 : funcionalidade 2 do codigo								   */
/*		- 3 : funcionalidade 3 do codigo								   */
/*																		   */
/* Parametros: reg* g - registro a ser impresso   						   */
/* 			   header* h - header a ser impresso (para op 3)			   */
/*			   int op - opcao do formato de impressao					   */
void printRegister(reg* r, header* h, int op);

/*-------------------------- Remove All Registers --------------------------*/
/* Remove fisicamente os registros indicados na lista, alterando os campos */
/* de removido, encademento lista e preenchendo o registro com @ 		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   list* l - lista ordenada com os registros removidos 		   */
/*			   header* h - header do arquivo 							   */
void removeAllRegisters(FILE* input, list* l, header* h);

/*---------------------------- Insert Register ----------------------------*/
/* Insere registros no arquivo nas posições livres indicadas na lista de   */
/* removidos ou no final do arquivo, atualizando o encadeamento 		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/*			   reg* r - registro a ser inserido							   */
/* 			   list* l - lista ordenada com os registros removidos 		   */
void insertRegister(FILE* input, reg* r, list* l);

/*-------------------------- Update Registers -----------------------------*/
/* Atualiza valor de um registro de acordo às especificações 			   */
/*																		   */
/* Parametros: reg* r - registro a ser atualizado						   */
/*			   header* h - header do arquivo 							   */
/* 			   char* newField - campo que será atualizado				   */
/* 			   char* newValue - novo valor que será escrito				   */
reg* updateRegister(reg* r, header* h, char* newField, char* newValue);

/*----------------------------- Insert In Pos -----------------------------*/
/* Insere um registro no arquivo na posição especificada				   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/*			   reg* r - registro a ser inserido							   */
/* 			   int pos - posição na qual o registro deve ser escrito	   */
/* 			   int prevSize - tamanho do espaço disponível no qual será escrito*/
void insertInPos(FILE* input, reg* r, int pos, int prevSize);

/*------------------------- Remove One Register ---------------------------*/
/* Remove fisicamente um registro que está na posição especificada		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   int pos - posição do registro que será removido			   */
/* 			   int prevSize - tamanho do espaço disponível				   */
/*			   int nextPos - posição do próximo removido 				   */
void removeOneRegister(FILE* input, int pos, int prevSize, int nextPos);


#endif