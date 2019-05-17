/* Registro: declaração e funções associadas*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/registers.h"

/*---------------------------- Create Register ----------------------------*/
reg* createRegister(int id, double salario, char* telefone, char* nome, char* cargo) {
	reg* r = (reg*)calloc(1, sizeof(reg));

	// preenche campos de tamanho fixo
	r->removido = '-';
	r->encadeamentoLista = -1;
	r->idServidor = id;
	r->salarioServidor = salario;
	
	// checa existencia de telefone
	if(strlen(telefone) == 0) memcpy(r->telefoneServidor, "\0@@@@@@@@@@@@@", 14);
	else strcpy(r->telefoneServidor, telefone);

	// checa existencia do nome
	r->tamNomeServidor = strlen(nome);
	if(r->tamNomeServidor == 0) strcpy(r->nomeServidor, "\0");
	else strcpy(r->nomeServidor, nome);
	
	// checa existencia do cargo
	r->tamCargoServidor = strlen(cargo);
	if(r->tamCargoServidor == 0) strcpy(r->cargoServidor, "\0");
	else strcpy(r->cargoServidor, cargo);

	// calcula tamanho do registro
	r->tamanhoRegistro = 34 + strlen(nome) + strlen(cargo);
	if(strlen(nome) != 0) r->tamanhoRegistro += 6;
	if(strlen(cargo) != 0) r->tamanhoRegistro += 6;

	return r;
}

/*---------------------------- Read Register ------------------------------*/
reg* readRegister(FILE* input, int offset, int* numRegs) {
	reg* r = calloc(1, sizeof(reg));
	if(r == NULL) return NULL;

	char tag;
	int tam = 0;

	// le status e checa se ele indica que o registro foi removido
	fread(&r->removido, sizeof(char), 1, input);

	if(r->removido == lixo) return NULL;
	else (*numRegs)++;
	
	// le demais campos de tamanho fixo
	fread(&r->tamanhoRegistro, sizeof(int), 1, input);
	fread(&r->encadeamentoLista, sizeof(int64_t), 1, input);
	
	fread(&r->idServidor, sizeof(int), 1, input);
	fread(&r->salarioServidor, sizeof(double), 1, input);
	fread(&r->telefoneServidor, sizeof(char), 14, input);

	// le campos de tamanho variado, para isso checa a tag e se o ponteiro ja
	// chegou ao final do registro
	while(ftell(input) < fileSize(input) && ftell(input) < offset + r->tamanhoRegistro) {
		
		fread(&tam, sizeof(int), 1, input);
		fread(&tag, sizeof(char), 1, input);
		
		if(tag == 'n') {
			r->tamNomeServidor = tam;
			fread(&r->nomeServidor, sizeof(char), tam-1, input);
		} else if(tag == 'c') {
			r->tamCargoServidor = tam;
			fread(&r->cargoServidor, sizeof(char), tam-1, input);
		} else {
			return r;
		}
	}

	return r;
}

/*---------------------------- Write Register -----------------------------*/
void writeRegister(reg* r, FILE* output, int lastSize) {
	if(r == NULL || r->removido != '-') return;

	// armazena a quantidade de bytes da ultima pagina de disco
	int currPageSize = ftell(output) % pageSize;
	
	// armazena tamanho do registro atual
	int tamReg = r->tamanhoRegistro;
	if(r->tamNomeServidor != 0) tamReg += 2;
	if(r->tamCargoServidor != 0) tamReg += 2;

	// se nao for possivel escreve-lo na pagina atual, completa 
	// essa pagina com @ e muda tamanho do registro anterior
	if(currPageSize + tamReg > pageSize) {		
		int currPos = ftell(output);
		int newSize = lastSize + pageSize - currPageSize;

		fseek(output, -lastSize-4, SEEK_CUR);
		fwrite(&newSize, 1, sizeof(int), output);
		fseek(output, currPos, SEEK_SET);

		for(int i = 0; i < pageSize - currPageSize; i++)
			fwrite(&lixo, 1, sizeof(char), output);

	}	

	// escreve campos de tamanho fixo
	fwrite(&(r->removido), 1, sizeof(char), output);
	fwrite(&(r->tamanhoRegistro), 1, sizeof(int), output);
	fwrite(&(r->encadeamentoLista), 1, sizeof(int64_t), output);

	fwrite(&(r->idServidor), 1, sizeof(int), output);
	fwrite(&(r->salarioServidor), 1, sizeof(double), output);
	fwrite(&(r->telefoneServidor), 14, sizeof(char), output);
	
	// escreve nome e demais dados relacionados a ele, caso exista
	if(r->tamNomeServidor != 0) {
		int tam = r->tamNomeServidor + 2;
		fwrite(&tam, 1, sizeof(int), output);
		fwrite("n", 1, sizeof(char), output);
		fwrite(&(r->nomeServidor), r->tamNomeServidor, sizeof(char), output);
		fwrite("\0", 1, sizeof(char), output);
	}
	
	// escreve cargo e demais dados relacionados a ele, caso exista
	if(r->tamCargoServidor != 0) {
		int tam = r->tamCargoServidor + 2;
		fwrite(&tam, 1, sizeof(int), output);
		fwrite("c", 1, sizeof(char), output);
		fwrite(&(r->cargoServidor), r->tamCargoServidor, sizeof(char), output);
		fwrite("\0", 1, sizeof(char), output);
	}
	
	return;
}

/*--------------------------- Search Register -----------------------------*/
int searchRegister(reg* r, header* h, char* fieldName, char* value) {
	if(r == NULL) return 0;
    
	// procura pelo campo certo a partir do qual sera feita a busca
	if(fieldName[0] == h->tags[0]) {
		// para o caso do id, transforma string em int
		int id;
		sscanf(value, "%d", &id);
		return (r->idServidor == id);

	} else if(fieldName[0] == h->tags[1]) {
		// para o caso do salario, transforma string em double
		double sal;
		sscanf(value, "%lf", &sal);
		return (r->salarioServidor == sal);

	} else if(fieldName[0] == h->tags[2]) {
		return (!strcmp(r->telefoneServidor, value));
	
	} else if(fieldName[0] == h->tags[3]) {
		return (!strcmp(r->nomeServidor, value));
		
	} else if(fieldName[0] == h->tags[4]) {
		return (!strcmp(r->cargoServidor, value));
		
	}

	return 0;
}

/*---------------------------- Insert In Pos ------------------------------*/
void insertInPos(FILE* input, reg* r, int pos, int prevSize) {
	// salva tamanho do registro
	int currSize = r->tamanhoRegistro;
	
	// altera tamanho do registro, colocando o tamanho do espaço no qual ele será inserido
	r->tamanhoRegistro = prevSize;
	
	// escreve no arquivo
	fseek(input, pos, SEEK_SET);
	writeRegister(r, input, 0);
	
	// preenche espaço com @
	for(int i = 0; i < prevSize-currSize; i++)
		fwrite(&lixo, sizeof(char), 1, input);

	return;
}

/*--------------------------- Insert Register -----------------------------*/
void insertRegister(FILE* input, reg* r, list* l) {
    if(input == NULL || r == NULL || l == NULL) return;

	// recebe nó removido da lista
	node* removed = removeList(l, r->tamanhoRegistro);
	
	int64_t noNext = -1;
	int64_t pos = removed->offset;
	int64_t prev = removed->prev;
	int lastSize = 0;
	
	// altera posição para inserir no final do arquivo
	if(pos == -1) {
		pos = fileSize(input);
		lastSize = r->tamanhoRegistro;
	}

	// altera encadeamento do registro anterior
	if(prev != 0) {
		fseek(input, prev+5, SEEK_SET);
		fwrite(&noNext, sizeof(int64_t), 1, input);
	}
	
	// move ponteiro para posição na qual o registro deve ser escrito e o escreve
	fseek(input, pos, SEEK_SET);
	
	if(removed->size != -1) r->tamanhoRegistro = removed->size;
	writeRegister(r, input, lastSize);

	// libera memória
	if(removed != NULL) free(removed);
	
    return;
}

/*------------------------ Remove All Registers ---------------------------*/
void removeAllRegisters(FILE* input, list* l, header* h) {
	// caso a lista esteja vazia, não é preciso remover nenhum registro
	if(l->size == 0) return;
	
	node* currNode = l->begin->next; // ignora nó sentinela
	// char removed = '*';
	int64_t pos = currNode->offset;
	int size = 0;
	int64_t nextPos = -1;
	
	// altera topo lista do header
	h->topoLista = pos;
	
	// percorre a lista de removidos, fazendo a remoção fisica
	while(currNode != NULL) {
		// salva dados a serem escritos
		size = currNode->size;
		pos = currNode->offset;
		
		if(currNode->next != NULL) nextPos = currNode->next->offset;
		else nextPos = -1;

		// remove fisicamente
		removeOneRegister(input, pos, size, nextPos);

		// checa proxima posição da lista
		currNode = currNode->next;
		
	}
	
    return;
}

/*------------------------ Remove One Register ----------------------------*/
void removeOneRegister(FILE* input, int pos, int prevSize, int nextPos) {
	char removed = '*';

	// move ponteiro para posição a ser removida
	fseek(input, pos, SEEK_SET); 
	
	// escreve char que indica remoção
	fwrite(&removed, sizeof(char), 1, input);
	
	// pula o tamanho do registro
	fseek(input, 4, SEEK_CUR);
	
	// escreve o novo encadeamento lista
	fwrite(&nextPos, sizeof(int64_t), 1, input);
	
	// preenche o resto do registro com @
	for(int i = 0; i < prevSize - 8; i++) {
		fwrite(&lixo, sizeof(char), 1, input);
	}
	
	return;
}

/*--------------------------- Print Register ------------------------------*/
void printRegister(reg* r, header* h, int op) {
	if(r == NULL) return;

	char naoDeclarado[20] = "valor nao declarado";
	
	switch(op) {
		// impressao de todos os campos para debug
		case 0:
			printf("Removido: %c\n", r->removido);
			printf("encadeamentoLista: %ld\n", r->encadeamentoLista);
			printf("id: %d\n", r->idServidor);
			printf("salarioServidor: %.2lf\n", r->salarioServidor);
			printf("telefone: %s\n", r->telefoneServidor);
			printf("tam: %d, nome: %s\n", r->tamNomeServidor, r->nomeServidor);
			printf("tam: %d, cargo: %s\n", r->tamCargoServidor, r->cargoServidor);
			printf("tam: %d\n", r->tamanhoRegistro);
			printf("\n");
			break;
		
		case 2:
			// impressao do registro numa mesma linha para funcionalidade 2
			if(r->removido == '*') return;

			printf("%d ", r->idServidor);
			
			if(r->salarioServidor != -1) printf("%.2lf ", r->salarioServidor);
			else printf("         ");
			
			if(strlen(r->telefoneServidor) == 0) printf("              ");
			else printf("%.14s", r->telefoneServidor);
			
			if(r->tamNomeServidor != 0) printf(" %d %s", r->tamNomeServidor - 2, r->nomeServidor);
			
			if(r->tamCargoServidor) printf(" %d %s", r->tamCargoServidor - 2, r->cargoServidor);

			printf("\n");
			break;
		
		case 3:
			// impressao do registro com descricao dos campos a para funcionalidade 3
			if(r->removido == '*') return;
			
			printf("%s: ", h->campos[0]);
			printf("%d\n", r->idServidor);
			
			printf("%s: ", h->campos[1]);
			if(r->salarioServidor == -1) printf("%s\n", naoDeclarado);
			else printf("%.2lf\n", r->salarioServidor);
			
			printf("%s: ", h->campos[2]);
			if(strlen(r->telefoneServidor) == 0) printf("%s\n", naoDeclarado);
			else printf("%.14s\n", r->telefoneServidor);

			printf("%s: ", h->campos[3]);
			if(r->tamNomeServidor == 0) printf("%s\n", naoDeclarado);
			else printf("%s\n", r->nomeServidor);

			printf("%s: ", h->campos[4]);
			if(r->tamCargoServidor == 0) printf("%s\n", naoDeclarado);
			else printf("%s\n", r->cargoServidor);
			
			printf("\n");

			break;
		
		default:
			break;
	}
}

/*-------------------------- Update Registers -----------------------------*/
reg* updateRegister(reg* r, header* h, char* newField, char* newValue) {
	if(r == NULL) return NULL;
	reg* newReg;
	
	// atualiza o id, caso esse seja o campo especificado
	if(newField[0] == h->tags[0]){
		int id;
		sscanf(newValue, "%d", &id);
		newReg = createRegister(id, r->salarioServidor, r->telefoneServidor, r->nomeServidor, r->cargoServidor);
	// atualiza o salario, caso esse seja o campo especificado
	} else if(newField[0] == h->tags[1]) {
		double salario;
		if(!strcmp(newValue, "")) salario = -1;
		else sscanf(newValue, "%lf", &salario);
		newReg = createRegister(r->idServidor, salario, r->telefoneServidor, r->nomeServidor, r->cargoServidor);
	// atualiza o telefone, caso esse seja o campo especificado
	} else if(newField[0] == h->tags[2]) {
		newReg = createRegister(r->idServidor, r->salarioServidor, newValue, r->nomeServidor, r->cargoServidor);
	// atualiza o nome, caso esse seja o campo especificado
	} else if(newField[0] == h->tags[3]) {
		newReg = createRegister(r->idServidor, r->salarioServidor, r->telefoneServidor, newValue, r->cargoServidor);
	// atualiza o cargo, caso esse seja o campo especificado
	} else if(newField[0] == h->tags[4]) {
		newReg = createRegister(r->idServidor, r->salarioServidor, r->telefoneServidor, r->nomeServidor, newValue);
	}
	
	if(r != NULL) free(r);
	return newReg;
}