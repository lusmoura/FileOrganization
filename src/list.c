/* Lista: declaração e funções associadas*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "list.h"
#include "registers.h"

/*------------------------------ Create Node ------------------------------*/
Node* createNode(int size, int64_t offset, int timeIn) {
    // cria nó
    Node* node = malloc(sizeof(Node));
    if (node == NULL) return NULL;

    // inicializa seus campos
    node->prev = 0;
    node->size = size;
    node->offset = offset;
    node->next = NULL;
    node->timeIn = timeIn;

    return node;
}

/*------------------------------ Create List ------------------------------*/
List* createList() {
    // cria lista
    List* list = malloc(sizeof(List));
    if (list == NULL) return NULL;

    // inicializa seus campos
    list->begin = createNode(-1, -1, -1);
    list->end = NULL;
    list->begin->next = list->end;
    list->size = 0;

    return list;
}

/*------------------------------ Insert List ------------------------------*/
void insertList(List* list, int size, int64_t offset, int timeIn) {
    // cria novo nó
    Node* newNode = createNode(size, offset, timeIn);
    if (newNode == NULL) return;

    // busca posição na qual o nó deve ser inserido para manter a lsita ordenada
    Node* currNode = list->begin;
    
    while (currNode->next != NULL) {
        // caso o tamanho do proximo seja maior, já se encontrou a posição
        if (currNode->next->size > size) {
            newNode->next = currNode->next;
            currNode->next = newNode;
            list->size++;
            return;
        // caso o tamanho seja igual, é preciso considerar o tempo de inserção como desempate
        } else if (currNode->next->size == size && currNode->next->timeIn <= timeIn) {
            newNode->next = currNode->next;
            currNode->next = newNode;
            list->size++;
            return;
        }

        currNode = currNode->next;
    }

    // chegando aqui, o nó é inserido no final da lista
    newNode->next = currNode->next;
    currNode->next = newNode;
    list->size++;

    return;
}

/*---------------------------- Insert List End ----------------------------*/
void insertListEnd(List* list, int size, int64_t offset) {
    // cria novo nó
    Node* newNode = createNode(size, offset, 0);
    if (newNode == NULL) return;

    // altera encadeamento de acordo ao fim da lista
    if (list->end == NULL) 
        list->begin->next = newNode;
    else 
        list->end->next = newNode;
    
    // insere nó no final e aumenta tamanho da lista
    list->end = newNode;
    list->size++;

    return;
}

/*------------------------------ Remove List ------------------------------*/
Node* removeList(List* list, int size) {
    if (list == NULL) return NULL;

    // cria um nó fake que será retornado caso não haja nó maior que o desejado
    Node* toBeRemoved = createNode(-1, -1, 0);
    Node* currNode = list->begin->next;
    Node* prev = list->begin;

    if (list->size == 0) return toBeRemoved;
    
    // busca nó de tamanho suficiente e salva seu antecessor
    while (currNode != NULL && currNode->size < size) {
        prev = currNode;
        currNode = currNode->next;
    }

    // caso o nó exista e seja maior, alteram-se os ponteiros e o tamanho da lista
    if (currNode != NULL && currNode->size >= size) {
        toBeRemoved = currNode;
        prev->next = currNode->next;
        list->size--;
    } 

    // salva posição do nó anterior
    if (currNode != NULL && currNode->next == NULL && prev != NULL) currNode->prev = prev->offset;

    return toBeRemoved;
}

/*--------------------------- Recursive Destroy ---------------------------*/
void recursiveDestroy(Node* currNode) {
    // libera memória recursivamente
    if (currNode != NULL)
        recursiveDestroy(currNode->next);

    free(currNode);
}

/*------------------------------ Destroy List -----------------------------*/
void destroyList(List* list) {
    // encapsulamento para função que libera memória
    if (list == NULL) return;
    recursiveDestroy(list->begin);
    free(list);
}

/*------------------------------- Print List ------------------------------*/
void printList(List* list) {
    // imprime ponteiro incial e valores de cada nó da lista
    Node* currNode = list->begin;
    printf("Begin: %p\n", list->begin);
    
    while (currNode != NULL) {
        printf("size: %d\toffset: %ld\ttimeIn: %d\t %p\t %p\n", currNode->size, currNode->offset, currNode->timeIn, currNode, currNode->next);
        currNode = currNode->next;
    }
}

/*----------------------------- Update Link -------------------------------*/
void updateLink(FILE* input, List* list) {
	if(list->size == 0) return;
	Node* currNode = list->begin->next;
	
	int64_t nextPos = -1;

	// percorre lista atualizando o encadeamento
	while (currNode != NULL) {
		fseek(input, currNode->offset + 5, SEEK_SET);
		
		if (currNode->next == NULL)
			nextPos = -1;
		else 
			nextPos = currNode->next->offset;

		fwrite(&nextPos, sizeof(int64_t), 1, input);
		
		currNode = currNode->next;
	}

	return;
}

/*-------------------------- Create Removed List --------------------------*/
void createRemovedList(FILE* input, List* list, int64_t pos) {
	int numRegs = 0;
	Register* reg = NULL;

	// percorre arquivo por meio do encadeamento lista, inserindo os registros na lista
	while (pos != -1) {
		// move ponteiro para a próxima posição
		fseek(input, pos, SEEK_SET);
		
        // lê registro e o insere no final da lista
		readRegister(&reg, input, ftell(input), &numRegs);
		insertListEnd(list, reg->tamanhoRegistro, pos);
		pos = reg->encadeamentoLista;

		if (reg != NULL) free(reg);
	}

	return;
}