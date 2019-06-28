#ifndef LIST_H
#define LIST_H

/*	Struct node  */
/*
		- int size - tamanho da posição vazia
		- int timeIn - tempo de inserção do registro na Lista - 0 quando não há queries
        - int64_t offset - byte offset da posição vazia
		- int64_t offset - byte offset da posição anterior na Lista
		- node* next - próximo elemento da Lista
*/
typedef struct _node {
    int size;
    int timeIn;
    int64_t offset;
    int64_t prev;
    struct _node* next;
} Node;

/*	Struct List  */
/*
		- Node* begin - ponteiro para inicio da Lista
        - Node* end - ponteiro para final da Lista
        - int size - tamanho da Lista
*/
typedef struct _List {
    Node* begin;
    Node* end;
    int size;
} List;

/*------------------------------ Create Node ------------------------------*/
/* Cria nó, alocando espaço e inicializando seus campos                    */
/*																		   */
/* Parâmetros: int size - tamanho do espaço livre       				   */
/*             int64_t offset - byteoffset da posição do registro no arquivo*/
/*             int timeIn - tempo de inserção, 0 quando não há queries     */
/* Retorno: List* - Lista criada                                           */
Node* createNode(int size, int64_t offset, int timeIn);

/*------------------------------ Create List ------------------------------*/
/* Cria Lista, alocando espaço na memória e inicizaliando seus campos      */
/*																		   */
/* Retorno: List* - Lista criada                                           */
List* createList();

/*------------------------------ Insert List ------------------------------*/
/* Insere elemento na Lista de forma que a mesma se mantenha ordenada com  */
/* base no campo size dos nós											   */
/*																		   */
/* Parâmetros: List* - Lista na qual deve ser inserido o novo elemento     */
/*             int size - tamanho da posição que será salva no novo nó     */
/*             int64_t offset - byte offset do registro que foi removido   */
void insertList(List* l, int size, int64_t offset, int timeIn);

/*---------------------------- Insert List End ----------------------------*/
/* Insere elemento no final da Lista, independente dos valores dos campos  */
/*																		   */
/* Parâmetros: List* - Lista na qual deve ser inserido o novo elemento     */
/*             int size - tamanho da posição que será salva no novo nó     */
/*             int64_t offset - byte offset do registro que foi removido   */
void insertListEnd(List* l, int size, int64_t offset);

/*------------------------------ Remove List ------------------------------*/
/* Busca na Lista um elemento com tamanho maior ou igual ao desejado,      */
/* altera o encadeamento da Lista e retorna esse nó removido               */
/*																		   */
/* Parâmetros: List* - Lista na qual deve ser removido o novo elemento     */
/*             int size - tamanho utilizado para buscar um nó adequado     */
/* Retorno: Node* - nó removido. Caso não haja nó com tamanho suficiente ou*/
/*                  a Lista estja vazia, é retornado um nó com dados -1, -1*/
Node* removeList(List* l, int size);

/*------------------------------ Destroy List -----------------------------*/
/* Libera toda a memória alocada pela Lista                                */
/*																		   */
/* Parâmetros: List* - Lista para liberar a memória                        */
void destroyList(List* l);

/*------------------------------- Print List ------------------------------*/
/* Imprime a Lista, seus valores e informações                             */
/*																		   */
/* Parâmetros: List* - Lista a ser impressa                                */
void printList(List* l);

/*----------------------------- Update Link -------------------------------*/
/* Atualiza encadeamento da Lista de removidos fisicamente no arquivo	   */
/*																		   */
/* Parametros: FILE* input - arquivo que será alterado					   */
/* 			   int pos - posição do registro que será removido			   */
/* 			   List* list - Lista ordenada com os registros removidos 	   */
void updateLink(FILE* input, List* list);

/*-------------------------- Create Removed List --------------------------*/
/* Cria Lista de removidos a partir dos registros que já estão removidos no*/
/* arquivo de entrada.													   */
/*																		   */
/* Parametros: FILE* fp - arquivo do qual serão lidos os registros   	   */
/* 			   List* list - Lista na qual serão inseridos os registros     */
/*			   int64_t firstPos - posição 							       */
void createRemovedList(FILE* input, List* list, int64_t firstPos);	

#endif