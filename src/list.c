/* Lista: declaração e funções associadas*/

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "../include/list.h"

char lixo = '@';

/*------------------------------ Create Node ------------------------------*/
node* createNode(int size, int64_t offset) {
    // cria nó
    node* n = malloc(sizeof(node));
    if(n == NULL) return NULL;

    // inicializa seus campos
    n->prev = 0;
    n->size = size;
    n->offset = offset;
    n->next = NULL;
    
    return n;
}

/*------------------------------ Create List ------------------------------*/
list* createList() {
    // cria lista
    list* l = malloc(sizeof(list));
    if(l == NULL) return NULL;

    // inicializa seus campos
    l->begin = createNode(-1, -1);
    l->end = NULL;
    l->begin->next = l->end;
    l->size = 0;

    return l;
}

/*------------------------------ Insert List ------------------------------*/
void insertList(list* l, int size, int64_t offset) {
    // cria novo nó
    node* newNode = createNode(size, offset);
    if(newNode == NULL) return;

    // busca posição na qual o nó deve ser inserido para manter a lsita ordenada
    node* currNode = l->begin;
    while(currNode->next != NULL && currNode->next->size < size) {
        currNode = currNode->next;
    }

    // altera ponteiros e aumenta tamanho da lista
    newNode->next = currNode->next;
    currNode->next = newNode;
    
    l->size++;

    return;
}

/*---------------------------- Insert List End ----------------------------*/
void insertListEnd(list* l, int size, int64_t offset) {
    // cria novo nó
    node* newNode = createNode(size, offset);
    if(newNode == NULL) return;

    // altera encadeamento de acordo ao fim da lista
    if(l->end == NULL) 
        l->begin->next = newNode;
    else 
        l->end->next = newNode;
    
    // insere nó no final e aumenta tamanho da lista
    l->end = newNode;
    l->size++;

    return;
}

/*------------------------------ Remove List ------------------------------*/
node* removeList(list* l, int size) {
    if(l == NULL) return NULL;

    // cria um nó fake que será retornado caso não haja nó maior que o desejado
    node* toBeRemoved = createNode(-1, -1);
    node* currNode = l->begin->next;
    node* prev = l->begin;

    if(l->size == 0) return toBeRemoved;
    
    // busca nó de tamanho suficiente e salva seu antecessor
    while(currNode != NULL && currNode->size < size) {
        prev = currNode;
        currNode = currNode->next;
    }

    // caso o nó exista e seja maior, alteram-se os ponteiros e o tamanho da lista
    if(currNode != NULL && currNode->size >= size) {
        toBeRemoved = currNode;
        prev->next = currNode->next;
        l->size--;
    } 

    // salva posição do nó anterior
    if(currNode != NULL && currNode->next == NULL && prev != NULL) currNode->prev = prev->offset;

    return toBeRemoved;
}

/*--------------------------- Recursive Destroy ---------------------------*/
void recursiveDestroy(node* currNode) {
    // libera memória recursivamente
    if(currNode != NULL)
        recursiveDestroy(currNode->next);

    free(currNode);
}

/*------------------------------ Destroy List -----------------------------*/
void destroyList(list* l) {
    // encapsulamento para função que libera memória
    if(l == NULL) return;
    recursiveDestroy(l->begin);
    free(l);
}

/*------------------------------- Print List ------------------------------*/
void printList(list* l) {
    // imprime ponteiro incial e valores de cada nó da lista
    node* currNode = l->begin;
    printf("Begin: %p\n", l->begin);
    
    while(currNode != NULL) {
        printf("size: %d\toffset: %ld\t %p\t %p\n", currNode->size, currNode->offset, currNode, currNode->next);
        currNode = currNode->next;
    }
}