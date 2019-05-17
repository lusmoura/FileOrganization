#ifndef LIST_H
#define LIST_H

/*	Struct node  */
/*
		- int size - tamanho da posição vazia
		- int64_t offset - byte offset da posição vazia
		- int64_t offset - byte offset da posição anterior na lista
		- node* next - próximo elemento da lista
*/
typedef struct _node {
    int size;
    int64_t offset;
    int64_t prev;
    struct _node* next;
} node;

/*	Struct list  */
/*
		- node* begin - ponteiro para inicio da lista
        - node* end - ponteiro para final da lista
        - int size - tamanho da lista
*/
typedef struct _list {
    node* begin;
    node* end;
    int size;
} list;

/*------------------------------ Create List ------------------------------*/
/* Cria lista, alocando espaço na memória e inicizaliando seus campos      */
/*																		   */
/* Retorno: list* - lista criada                                           */
list* createList();

/*------------------------------ Insert List ------------------------------*/
/* Insere elemento na lista de forma que a mesma se mantenha ordenada com  */
/* base no campo size dos nós											   */
/*																		   */
/* Parâmetros: list* - lista na qual deve ser inserido o novo elemento     */
/*             int size - tamanho da posição que será salva no novo nó     */
/*             int64_t offset - byte offset do registro que foi removido   */
void insertList(list* l, int size, int64_t offset);


/*---------------------------- Insert List End ----------------------------*/
/* Insere elemento no final da lista, independente dos valores dos campos  */
/*																		   */
/* Parâmetros: list* - lista na qual deve ser inserido o novo elemento     */
/*             int size - tamanho da posição que será salva no novo nó     */
/*             int64_t offset - byte offset do registro que foi removido   */
void insertListEnd(list* l, int size, int64_t offset);

/*------------------------------ Remove List ------------------------------*/
/* Busca na lista um elemento com tamanho maior ou igual ao desejado,      */
/* altera o encadeamento da lista e retorna esse nó removido               */
/*																		   */
/* Parâmetros: list* - lista na qual deve ser removido o novo elemento     */
/*             int size - tamanho utilizado para buscar um nó adequado     */
/* Retorno: node* - nó removido. Caso não haja nó com tamanho suficiente ou*/
/*                  a lista estja vazia, é retornado um nó com dados -1, -1*/
node* removeList(list* l, int size);

/*------------------------------ Destroy List -----------------------------*/
/* Libera toda a memória alocada pela lista                                */
/*																		   */
/* Parâmetros: list* - lista para liberar a memória                        */
void destroyList(list* l);

/*------------------------------- Print List ------------------------------*/
/* Imprime a lista, seus valores e informações                             */
/*																		   */
/* Parâmetros: list* - lista a ser impressa                                */
void printList(list* l);

#endif