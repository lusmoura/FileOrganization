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
typedef struct _Register {
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
} Register;

/*---------------------------- Create Register ----------------------------*/
/* Recebe dados e, com eles, cria um registro 							   */
/*																		   */
/* Parametros: int id - id do servidor para qual o registro sera criado    */
/*             double salario - salario do servidor para qual o registro sera criado */
/*			   char* telefone - telefone do servidor para qual o registro sera criado*/
/*			   char* nome - nome do servidor para qual o registro sera criado        */
/*			   char* cargo - cargo do servidor para qual o registro sera criado      */
/* Retorno: reg* - registro criado								   */
Register* createRegister(int id, double salario, char* telefone, char* nome, char* cargo);

/*---------------------------- Read Register ------------------------------*/
/* Le um registro do arquivo binario e o armazena na struct reg			   */
/*																		   */
/* Parametros: FILE* input - arquivo a partir do qual sera lido o registro */
/*			   int offset - posicao do ponteiro do arquivo antes da leitura*/
/*			   int* numRegs - endereco da variavel que armazena numero de regs      */
/* 			   Register** reg - ponteiro para endereço no qual o registro será salvo*/
void readRegister(Register** reg, FILE* input, int offset, int* numRegs);

/*---------------------------- Write Register -----------------------------*/
/* Escreve registro em um arquivo binario    							   */
/*																		   */
/* Parametros: Register* reg - registro a ser escrito					   */
/*			   FILE* output - arquivo no qual o registro sera escrito	   */
/*			   int lastSize - tamanho do ultimo registro lido              */
void writeRegister(Register* reg, FILE* output, int lastSize);

/*--------------------------- Search Register -----------------------------*/
/* Busca por um valor de um determinado campo no registro				   */
/*																		   */
/* Parametros: Register* reg - registro no qual o valor sera buscado	   */
/*			   Header* header - Header para saber qual a posicao do campo no reg*/
/*			   char* fieldName - nome do campo a ser buscado			   */
/*			   char* value - valor a ser buscado						   */
/* Retorno: int - indica se o valor foi encontrado (1 ser for, 0 se nao)   */
int searchRegister(Register* reg, Header* header, char* fieldName, char* value);

/*--------------------------- Print Register ------------------------------*/
/* Imprime valores armazenados no registro com as opcoes:				   */
/*		- 0 : debug														   */
/*		- 2 : funcionalidade 2 do codigo								   */
/*		- 3 : funcionalidade 3 do codigo								   */
/*																		   */
/* Parametros: Register* reg - registro a ser impresso					   */
/* 			   Header* header - Header a ser impresso (para op 3)		   */
/*			   int op - opcao do formato de impressao					   */
void printRegister(Register* reg, Header* header, int op);

/*-------------------------- Remove All Registers --------------------------*/
/* Remove fisicamente os registros indicados na lista, alterando os campos */
/* de removido, encademento lista e preenchendo o registro com @ 		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   List* list - lista ordenada com os registros removidos 	   */
/*			   Header* header - Header do arquivo 						   */
void removeAllRegisters(FILE* input, List* list, Header* header);

/*---------------------------- Insert Register ----------------------------*/
/* Insere registros no arquivo nas posições livres indicadas na lista de   */
/* removidos ou no final do arquivo, atualizando o encadeamento 		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/*			   Register* reg - registro a ser inserido					   */
/* 			   List* list - lista ordenada com os registros removidos 	   */
// COLOCAR RETORNO
int insertRegister(FILE* input, Register* reg, List* list);

/*-------------------------- Update Registers -----------------------------*/
/* Atualiza valor de um registro de acordo às especificações 			   */
/*																		   */
/* Parametros: Register* reg - registro a ser atualizado				   */
/*			   Header* header - Header do arquivo 						   */
/* 			   char* newField - campo que será atualizado				   */
/* 			   char* newValue - novo valor que será escrito				   */
Register* updateRegister(Register* reg, Header* header, char* newField, char* newValue);

/*----------------------------- Update Size -------------------------------*/
/* Atualiza tamanho de um registro, passando a descosiderar os @ dos nomes */
/*																		   */
/* Parametros: Register** reg - ponteiro para registro que será atualizado */
void updateSize(Register** reg);

/*----------------------------- Insert In Pos -----------------------------*/
/* Insere um registro no arquivo na posição especificada				   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/*			   Register* reg - registro a ser inserido							   */
/* 			   int pos - posição na qual o registro deve ser escrito	   */
/* 			   int prevSize - tamanho do espaço disponível no qual será escrito*/
void insertInPos(FILE* input, Register* reg, int pos, int prevSize);

/*------------------------- Remove One Register ---------------------------*/
/* Remove fisicamente um registro que está na posição especificada		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   int pos - posição do registro que será removido			   */
/* 			   int prevSize - tamanho do espaço disponível				   */
/*			   int nextPos - posição do próximo removido 				   */
void removeOneRegister(FILE* input, int pos, int prevSize, int nextPos);

/*---------------------------- Sort Registers -----------------------------*/
/* Le registros de um arquivo e os ordena num vetor						   */
/*																		   */
/* Parâmetros: FILE* fp - ponteiro para arquivo dos registros			   */
/*			   Register** registers - vetor no qual serão salvos os registos*/
int sortRegisters(FILE* fp, Register** registers);

/*---------------------------- Merge Registers ----------------------------*/
/* Faz o merge de dois arquivos                                            */
/*                                                                         */
/* Parâmetros: Register* first - vetor com os registros do primeiro arquivo*/
/*             int amount1 - quantidade de elementos do primeiro vetor     */
/*             Register* second - vetor com os registros do segundo arquivo*/
/*             int amount2 - quantidade de elementos do segundo vetor      */
/*             Register** output - ponteiro para vetor no qual será salvo o merge*/
/* Retorno: int - quantidade de elementos no vetor de saída				   */
int mergeRegisters(Register* firstRegisters, int amount1, Register* secondRegisters, int amount2, Register** outputRegisters);

/*---------------------------- Match Registers ----------------------------*/
/* Faz o match de dois arquivos                                            */
/*                                                                         */
/* Parâmetros: Register* first - vetor com os registros do primeiro arquivo*/
/*             int amount1 - quantidade de elementos do primeiro vetor     */
/*             Register* second - vetor com os registros do segundo arquivo*/
/*             int amount2 - quantidade de elementos do segundo vetor      */
/*             Register** output - ponteiro para vetor no qual será salvo o merge*/
/* Retorno: int - quantidade de elementos no vetor de saída				   */
int matchRegisters(Register* firstRegisters, int amount1, Register* secondRegisters, int amount2, Register** outputRegisters);

#endif