#ifndef UTILS_H
#define UTILS_H

#define pageSize 32000
#include <stdint.h>
#include "list.h"

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

/*--------------------------- Remove Registers ----------------------------*/
/* Remove fisicamente os registros indicados na lista, alterando os campos */
/* de removido, encademento lista e preenchendo o registro com @ 		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   list* l - lista ordenada com os registros removidos 		   */
/*			   header* h - header do arquivo 							   */
void removeRegisters(FILE* input, list* l, header* h);

/*---------------------------- Insert Register ----------------------------*/
/* Insere registros no arquivo nas posições livres indicadas na lista de   */
/* removidos ou no final do arquivo, atualizando o encadeamento 		   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/*			   reg* r - registro a ser inserido							   */
/* 			   list* l - lista ordenada com os registros removidos 		   */
void insertRegister(FILE* input, reg* r, list* l);



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
/*			   header* h - cabeçalho do arquivo							   */
void createRemovedList(FILE* input, list* l, header* h);	

/*--------------------------- Binario na Tela -----------------------------*/
/* Função fornecida pelo monitor da matéria 							   */
void binarioNaTela1(FILE *ponteiroArquivoBinario);

/*--------------------------- Scan Quote String ---------------------------*/
/* Função fornecida pelo monitor da matéria 							   */
void scan_quote_string(char *str);

#endif
