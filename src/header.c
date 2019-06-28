/* Cabeçalho: declaração e funções associadas*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "header.h"

/*----------------------------- Create Header -----------------------------*/
Header* createHeader(FILE* input, FILE* output) {	Header* header = calloc(1, sizeof(Header));
	if (header == NULL) return NULL;

	char tag[5] = {'i', 's', 't', 'n', 'c'};
	char desc[5][40];
	
	// le a descricao de cada campo que termina com virgula e o \n
	for (int i = 0; i < 4; i++) {
		fscanf(input, "%[^,]", desc[i]);
		fgetc(input);
	}

	// le a descricao do ultimo campo e o \n
	fscanf(input, "%[^\r\n]", desc[4]);
	fgetc(input);

	// escreve o status e o topo lista
	header->status = '1';
	header->topoLista = -1;
	
	// escreve cada tag e sua descrição, preenchendo o Lixo com '@'
	for (int i = 0; i < 5; i++) {
		header->tags[i] = tag[i];
		
		strcpy(header->campos[i], desc[i]);
		int lastPos = strlen(desc[i]) + 1;
		
		fillWithTrash(input, 40-lastPos);
	}

	// escreve Header no arquivo
	writeHeader(output, header);

	return header;
}

/*------------------------------ Read Header ------------------------------*/
Header* readHeader(FILE* input){
	Header* header = calloc(1, sizeof(Header));

	// le status e topoLista do Header
	fread(&header->status, sizeof(char), 1, input);
	fread(&header->topoLista, sizeof(int64_t), 1, input);

	// le cada tag e cada descricao (de tamanho fixo 40 bytes)
	for (int i = 0; i < 5; i++) {
		fread(&header->tags[i], sizeof(char), 1, input);
		fread(header->campos[i], sizeof(char), 40, input);
		header->campos[i][40] = '\0';
		
		// coloca um \0 no final da string
		for (int j = 0; j <= 40 && header->campos[i][j] != '\0'; j++) {
			if (header->campos[i][j] == '@') header->campos[i][j] = '\0';
		}
		
	}
	
	return header;
}

/*----------------------------- Write Header ------------------------------*/
void writeHeader(FILE* input, Header* header) {	

	// escreve no arquivo o status e o topo lista do Header
	fwrite(&(header->status), 1, sizeof(header->status), input);
	fwrite(&(header->topoLista), 1, sizeof(header->topoLista), input);

	// escreve cada tag e sua descrição, preenchendo o Lixo com '@'
	for (int i = 0; i < 5; i++) {
		fwrite(&(header->tags[i]), 1, sizeof(char), input);
		fwrite(&(header->campos[i]), 1, sizeof(header->campos[i]), input);
	}

	// preenche o resto da página com '@'
	while (ftell(input) < pageSize){
		fwrite(&Lixo, 1, sizeof(char), input);
	}
}

/*---------------------------- Set As Inconsistent ------------------------*/
void setAsInconsistent(FILE* input) {
	// salva posição atual do ponteiro no arquivo
	int64_t pos = ftell(input);
	char inconsistent = '0';

	// volta ponteiro para inicio do arquivo, onde o Header é escrito
	rewind(input);
	fwrite(&inconsistent, 1, sizeof(char), input);

	// ponteiro retorna à posição original
	fseek(input, pos, SEEK_SET);
	
	return;
}

/*----------------------------- Set As Consistent -------------------------*/
void setAsConsistent(FILE* input) {
	// salva posição atual do ponteiro no arquivo
	int64_t pos = ftell(input);
	char consistent = '1';
	
	// volta ponteiro para inicio do arquivo, onde o Header é escrito
	rewind(input);
	fwrite(&consistent, 1, sizeof(char), input);

	// ponteiro retorna à posição original
	fseek(input, pos, SEEK_SET);
	
	return;
}

/*------------------------------ Print Header -----------------------------*/
void printHeader(Header* header){
	// mensagem de erro caso o Header seja nulo
	if (header == NULL) {
		printf("Header nao criado ou com erro\n");
		return;
	}

	// impressao de cada campo do Header
	printf("Status: %c\n", header->status);
	printf("Topo: %ld\n", header->topoLista);

	for (int i = 0; i < 5; i++) {
		printf("Tag %d: %c\n", i+1, header->tags[i]);
		printf("Campo %d: %s\n", i+1, header->campos[i]);
	}
}