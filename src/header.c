/* Cabeçalho: declaração e funções associadas*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/utils.h"
#include "../include/header.h"

/*----------------------------- Create Header -----------------------------*/
header* createHeader(FILE* input, FILE* output) {
	
	header* h = calloc(1, sizeof(header));
	if(h == NULL) return NULL;

	char tag[5] = {'i', 's', 't', 'n', 'c'};
	char desc[5][40];
	
	// le a descricao de cada campo que termina com virgula e o \n
	for(int i = 0; i < 4; i++) {
		fscanf(input, "%[^,]", desc[i]);
		fgetc(input);
	}

	// le a descricao do ultimo campo e o \n
	fscanf(input, "%[^\r\n]", desc[4]);
	fgetc(input);

	// escreve o status e o topo lista
	h->status = '1';
	h->topoLista = -1;
	
	// escreve cada tag e sua descrição, preenchendo o lixo com '@'
	for(int i = 0; i < 5; i++) {
		h->tags[i] = tag[i];
		
		strcpy(h->campos[i], desc[i]);
		int lastPos = strlen(desc[i]) + 1;
		
		while(lastPos < 40) {
			h->campos[i][lastPos] = lixo;
			lastPos++;
		}
	}

	// escreve header no arquivo
	writeHeader(output, h);

	return h;
}

/*------------------------------ Read Header ------------------------------*/
header* readHeader(FILE* input){
	header* h = calloc(1, sizeof(header));

	// le status e topoLista do header
	fread(&h->status, sizeof(char), 1, input);
	fread(&h->topoLista, sizeof(int64_t), 1, input);

	// le cada tag e cada descricao (de tamanho fixo 40 bytes)
	for(int i = 0; i < 5; i++) {
		fread(&h->tags[i], sizeof(char), 1, input);
		fread(h->campos[i], sizeof(char), 40, input);
		h->campos[i][40] = '\0';
		
		// coloca um \0 no final da string
		for(int j = 0; j <= 40 && h->campos[i][j] != '\0'; j++) {
			if(h->campos[i][j] == '@') h->campos[i][j] = '\0';
		}
		
	}
	
	return h;
}

/*----------------------------- Write Header ------------------------------*/
void writeHeader(FILE* input, header* h) {	

	// escreve no arquivo o status e o topo lista do header
	fwrite(&(h->status), 1, sizeof(h->status), input);
	fwrite(&(h->topoLista), 1, sizeof(h->topoLista), input);

	// escreve cada tag e sua descrição, preenchendo o lixo com '@'
	for(int i = 0; i < 5; i++) {
		fwrite(&(h->tags[i]), 1, sizeof(char), input);
		fwrite(&(h->campos[i]), 1, sizeof(h->campos[i]), input);
	}

	// preenche o resto da página com '@'
	while(ftell(input) < pageSize){
		fwrite(&lixo, 1, sizeof(char), input);
	}
}

/*------------------------------ Update Header --------------------------*/
void updateHeader(FILE* input, header* h, char status) {
	// salva posição atual do ponteiro no arquivo
	int64_t pos = ftell(input);
	
	// altera status do header
	h->status = status;
	
	// volta ponteiro para inicio do arquivo, onde o header é escrito
	rewind(input);
	writeHeader(input, h);

	// ponteiro retorna à posição original
	fseek(input, pos, SEEK_SET);
	
	return;
}

/*------------------------------ Print Header -----------------------------*/
void printHeader(header* h){
	// mensagem de erro caso o header seja nulo
	if(h == NULL) {
		printf("Header nao criado ou com erro\n");
		return;
	}

	// impressao de cada campo do header
	printf("Status: %c\n", h->status);
	printf("Topo: %ld\n", h->topoLista);

	for(int i = 0; i < 5; i++) {
		printf("Tag %d: %c\n", i+1, h->tags[i]);
		printf("Campo %d: %s\n", i+1, h->campos[i]);
	}
}