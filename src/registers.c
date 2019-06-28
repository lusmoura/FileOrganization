/* Registro: declaração e funções associadas*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "registers.h"

/*---------------------------- Create Register ----------------------------*/
Register* createRegister(int id, double salario, char* telefone, char* nome, char* cargo) {
	Register* reg = (Register*)calloc(1, sizeof(Register));

	// preenche campos de tamanho fixo
	reg->removido = '-';
	reg->encadeamentoLista = -1;
	reg->idServidor = id;
	reg->salarioServidor = salario;
	
	// checa existencia de telefone
	if (strlen(telefone) == 0) memcpy(reg->telefoneServidor, "\0@@@@@@@@@@@@@", 14);
	else strcpy(reg->telefoneServidor, telefone);

	// checa existencia do nome
	reg->tamNomeServidor = strlen(nome);
	if (reg->tamNomeServidor == 0) strcpy(reg->nomeServidor, "\0");
	else strcpy(reg->nomeServidor, nome);
	
	// checa existencia do cargo
	reg->tamCargoServidor = strlen(cargo);
	if (reg->tamCargoServidor == 0) strcpy(reg->cargoServidor, "\0");
	else strcpy(reg->cargoServidor, cargo);

	// calcula tamanho do registro
	reg->tamanhoRegistro = constantSize + strlen(nome) + strlen(cargo);
	if (strlen(nome) != 0) reg->tamanhoRegistro += 6;
	if (strlen(cargo) != 0) reg->tamanhoRegistro += 6;

	return reg;
}


/*---------------------------- Read Register ------------------------------*/
void readRegister(Register** reg, FILE* input, int offset, int* numRegs) {
	int size = fileSize(input);
	if(ftell(input) == size) {
		(*reg) = NULL;
		return;
	}

	(*reg) = (Register*)calloc(1, sizeof(Register));
	if ((*reg) == NULL) return;
	char tag;
	int tam = 0;

	// le status e checa se ele indica que o registro foi removido
	fread(&(*reg)->removido, sizeof(char), 1, input);

	if ((*reg)->removido != '-') return;
	else (*numRegs)++;
	
	// le demais campos de tamanho fixo
	fread(&(*reg)->tamanhoRegistro, sizeof(int), 1, input);
	fread(&(*reg)->encadeamentoLista, sizeof(int64_t), 1, input);
	
	fread(&(*reg)->idServidor, sizeof(int), 1, input);
	fread(&(*reg)->salarioServidor, sizeof(double), 1, input);
	fread(&(*reg)->telefoneServidor, sizeof(char), 14, input);

	// le campos de tamanho variado, para isso checa a tag e se o ponteiro ja
	// chegou ao final do registro
	while (ftell(input) < fileSize(input) && ftell(input) < offset + (*reg)->tamanhoRegistro) {
		
		fread(&tam, sizeof(int), 1, input);
		fread(&tag, sizeof(char), 1, input);
		
		if (tag == 'n') {
			(*reg)->tamNomeServidor = tam;
			fread(&(*reg)->nomeServidor, sizeof(char), tam-1, input);
		} else if (tag == 'c') {
			(*reg)->tamCargoServidor = tam;
			fread(&(*reg)->cargoServidor, sizeof(char), tam-1, input);
		} else {
			return;
		}
	}

	return;
}

/*---------------------------- Write Register -----------------------------*/
void writeRegister(Register* reg, FILE* output, int lastSize) {
	if (reg == NULL || reg->removido != '-') return;
	
	// armazena a quantidade de bytes da ultima pagina de disco
	int currPageSize = ftell(output) % pageSize;
	
	// armazena tamanho do registro atual
	int tamReg = reg->tamanhoRegistro;
	if (realSize(reg->nomeServidor) != 0) tamReg += 2;
	if (realSize(reg->cargoServidor) != 0) tamReg += 2;

	// se nao for possivel escreve-lo na pagina atual, completa 
	// essa pagina com @ e muda tamanho do registro anterior
	if (currPageSize + tamReg >= pageSize) {
		int currPos = ftell(output);
		int newSize = lastSize + pageSize - currPageSize;

		fseek(output, -lastSize-4, SEEK_CUR);
		fwrite(&newSize, 1, sizeof(int), output);
		fseek(output, currPos, SEEK_SET);

		fillWithTrash(output, pageSize-currPageSize);
	}

	// escreve campos de tamanho fixo
	fwrite(&(reg->removido), 1, sizeof(char), output);
	fwrite(&(reg->tamanhoRegistro), 1, sizeof(int), output);
	fwrite(&(reg->encadeamentoLista), 1, sizeof(int64_t), output);

	fwrite(&(reg->idServidor), 1, sizeof(int), output);
	fwrite(&(reg->salarioServidor), 1, sizeof(double), output);
	fwrite(&(reg->telefoneServidor), 14, sizeof(char), output);
	
	// escreve nome e demais dados relacionados a ele, caso exista
	if (reg->tamNomeServidor != 0) {
		int tam = reg->tamNomeServidor;
		fwrite(&tam, 1, sizeof(int), output);
		fwrite("n", 1, sizeof(char), output);
		fwrite(&(reg->nomeServidor), reg->tamNomeServidor-1, sizeof(char), output);
	}
	
	// escreve cargo e demais dados relacionados a ele, caso exista
	if (reg->tamCargoServidor != 0) {
		int tam = reg->tamCargoServidor;
		fwrite(&tam, 1, sizeof(int), output);
		fwrite("c", 1, sizeof(char), output);
		fwrite(&(reg->cargoServidor), reg->tamCargoServidor-1, sizeof(char), output);
	}
	
	return;
}

/*--------------------------- Search Register -----------------------------*/
int searchRegister(Register* reg, Header* header, char* fieldName, char* value) {
	if (reg == NULL) return 0;

	// procura pelo campo certo a partir do qual sera feita a busca
	if (fieldName[0] == header->tags[0]) {
		// para o caso do id, transforma string em int
		int id;
		sscanf(value, "%d", &id);
		return (reg->idServidor == id);

	} else if (fieldName[0] == header->tags[1]) {
		// para o caso do salario, transforma string em double
		double sal;
		sscanf(value, "%lf", &sal);
		return (reg->salarioServidor == sal);

	} else if (fieldName[0] == header->tags[2]) {
		return (!strcmp(reg->telefoneServidor, value));
	
	} else if (fieldName[0] == header->tags[3]) {
		return (!strcmp(reg->nomeServidor, value));
		
	} else if (fieldName[0] == header->tags[4]) {
		return (!strcmp(reg->cargoServidor, value));
		
	}

	return 0;
}

/*---------------------------- Insert In Pos ------------------------------*/
void insertInPos(FILE* input, Register* reg, int pos, int prevSize) {
	// salva tamanho do registro
	int currSize = reg->tamanhoRegistro;
	
	// altera tamanho do registro, colocando o tamanho do espaço no qual ele será inserido
	reg->tamanhoRegistro = prevSize;
	
	// escreve no arquivo
	fseek(input, pos, SEEK_SET);
	writeRegister(reg, input, 0);
	
	// preenche espaço com @
	fillWithTrash(input, prevSize-currSize);

	return;
}

/*--------------------------- Insert Register -----------------------------*/
int insertRegister(FILE* input, Register* reg, List* l) {
    if (input == NULL || reg == NULL || l == NULL) return 0;

	// recebe nó removido da lista
	Node* removed = removeList(l, reg->tamanhoRegistro);
	
	int64_t noNext = -1;
	int64_t pos = removed->offset;
	int64_t prev = removed->prev;
	int lastSize = 0;
	
	// altera posição para inserir no final do arquivo
	if (pos == -1) {
		pos = fileSize(input);
		lastSize = reg->tamanhoRegistro;
	}

	// altera encadeamento do registro anterior
	if (prev != 0) {
		fseek(input, prev+5, SEEK_SET);
		fwrite(&noNext, sizeof(int64_t), 1, input);
	}
	
	// move ponteiro para posição na qual o registro deve ser escrito e o escreve
	fseek(input, pos, SEEK_SET);
	
	if (removed->size != -1) reg->tamanhoRegistro = removed->size;
	writeRegister(reg, input, lastSize);

	// libera memória
	if (removed != NULL) free(removed);
	
    return pos;
}

/*------------------------ Remove All Registers ---------------------------*/
void removeAllRegisters(FILE* input, List* l, Header* header) {
	// caso a lista esteja vazia, não é preciso remover nenhum registro
	if (l->size == 0) return;
	
	Node* currNode = l->begin->next; // ignora nó sentinela
	// char removed = '*';
	int64_t pos = currNode->offset;
	int size = 0;
	int64_t nextPos = -1;
	
	// altera topo lista do header
	header->topoLista = pos;
	
	// percorre a lista de removidos, fazendo a remoção fisica
	while (currNode != NULL) {
		// salva dados a serem escritos
		size = currNode->size;
		pos = currNode->offset;
		
		if (currNode->next != NULL) nextPos = currNode->next->offset;
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
	fillWithTrash(input, prevSize-8);
	
	return;
}

/*--------------------------- Print Register ------------------------------*/
void printRegister(Register* reg, Header* header, int op) {
	if (reg == NULL) return;
	char naoDeclarado[20] = "valor nao declarado";
	
	switch(op) {
		// impressao de todos os campos para debug
		case 0:
			printf("Removido: %c\n", reg->removido);
			printf("encadeamentoLista: %ld\n", reg->encadeamentoLista);
			printf("id: %d\n", reg->idServidor);
			printf("salarioServidor: %.2lf\n", reg->salarioServidor);
			printf("telefone: %s\n", reg->telefoneServidor);
			printf("tam: %d, nome: %s\n", reg->tamNomeServidor, reg->nomeServidor);
			printf("tam: %d, cargo: %s\n", reg->tamCargoServidor, reg->cargoServidor);
			printf("tam: %d\n", reg->tamanhoRegistro);
			printf("\n");
			break;
		
		case 2:
			// impressao do registro numa mesma linha para funcionalidade 2
			if (reg->removido == '*') return;

			printf("%d ", reg->idServidor);
			
			if (reg->salarioServidor != -1) printf("%.2lf ", reg->salarioServidor);
			else printf("         ");
			
			if (strlen(reg->telefoneServidor) == 0) printf("              ");
			else printf("%.14s", reg->telefoneServidor);
			
			if (reg->tamNomeServidor != 0) printf(" %d %s", reg->tamNomeServidor - 2, reg->nomeServidor);
			
			if (reg->tamCargoServidor) printf(" %d %s", reg->tamCargoServidor - 2, reg->cargoServidor);

			printf("\n");
			break;
		
		case 3:
			// impressao do registro com descricao dos campos a para funcionalidade 3
			if (reg->removido == '*') return;
			
			printf("%s: ", header->campos[0]);
			printf("%d\n", reg->idServidor);
			
			printf("%s: ", header->campos[1]);
			if (reg->salarioServidor == -1) printf("%s\n", naoDeclarado);
			else printf("%.2lf\n", reg->salarioServidor);
			
			printf("%s: ", header->campos[2]);
			if (strlen(reg->telefoneServidor) == 0) printf("%s\n", naoDeclarado);
			else printf("%.14s\n", reg->telefoneServidor);

			printf("%s: ", header->campos[3]);
			if (reg->tamNomeServidor == 0) printf("%s\n", naoDeclarado);
			else printf("%s\n", reg->nomeServidor);

			printf("%s: ", header->campos[4]);
			if (reg->tamCargoServidor == 0) printf("%s\n", naoDeclarado);
			else printf("%s\n", reg->cargoServidor);
			
			printf("\n");

			break;
		
		default:
			break;
	}

	return;
}

/*-------------------------- Update Registers -----------------------------*/
Register* updateRegister(Register* reg, Header* header, char* newField, char* newValue) {
	if (reg == NULL) return NULL;
	Register* newReg;
	
	// atualiza o id, caso esse seja o campo especificado
	if (newField[0] == header->tags[0]){
		int id;
		sscanf(newValue, "%d", &id);
		newReg = createRegister(id, reg->salarioServidor, reg->telefoneServidor, reg->nomeServidor, reg->cargoServidor);
	// atualiza o salario, caso esse seja o campo especificado
	} else if (newField[0] == header->tags[1]) {
		double salario;
		if (!strcmp(newValue, "")) salario = -1;
		else sscanf(newValue, "%lf", &salario);
		newReg = createRegister(reg->idServidor, salario, reg->telefoneServidor, reg->nomeServidor, reg->cargoServidor);
	// atualiza o telefone, caso esse seja o campo especificado
	} else if (newField[0] == header->tags[2]) {
		newReg = createRegister(reg->idServidor, reg->salarioServidor, newValue, reg->nomeServidor, reg->cargoServidor);
	// atualiza o nome, caso esse seja o campo especificado
	} else if (newField[0] == header->tags[3]) {
		newReg = createRegister(reg->idServidor, reg->salarioServidor, reg->telefoneServidor, newValue, reg->cargoServidor);
	// atualiza o cargo, caso esse seja o campo especificado
	} else if (newField[0] == header->tags[4]) {
		newReg = createRegister(reg->idServidor, reg->salarioServidor, reg->telefoneServidor, reg->nomeServidor, newValue);
	}
	
	if (reg != NULL) free(reg);
	return newReg;
}

/*----------------------------- Update Size -------------------------------*/
void updateSize(Register** reg) {
	(*reg)->tamanhoRegistro = constantSize + realSize((*reg)->nomeServidor) + realSize((*reg)->cargoServidor);
	if (realSize((*reg)->nomeServidor) != 0) (*reg)->tamanhoRegistro += 6;
	if (realSize(((*reg)->cargoServidor)) != 0) (*reg)->tamanhoRegistro += 6;
	return;
}

/*---------------------------- Sort Registers -----------------------------*/
int sortRegisters(FILE* fp, Register** registers) {	
	Register* reg = NULL;
	int amount = 0;
	int numRegs = 0;
	int size = fileSize(fp);
	
	// percorre arquivo salvando os registro em um vetor
	fseek(fp, pageSize, SEEK_SET);
	while(ftell(fp) < size) {
		readRegister(&reg, fp, ftell(fp), &numRegs);
		if (reg == NULL) continue; 
		else if(reg->removido != '-' || reg->telefoneServidor[0] == Lixo) {
			free(reg);
			continue;
		}
		
		updateSize(&reg);
		(*registers)[amount] = (*reg);
		amount++;
		if (reg != NULL) free(reg);
	}

	// ordena os registros
	qsort(*registers, amount, sizeof(Register), idComparator);	

	return amount;
}

/*---------------------------- Merge Registers ----------------------------*/
int mergeRegisters(Register* firstRegisters, int amount1, Register* secondRegisters, int amount2, Register** outputRegisters) {
	int cont1 = 0;
	int cont2 = 0;
	int cont3 = 0;
	
	// percorre vetores com dois ponteiros
	while (cont1 < amount1 && cont2 < amount2) {
		if (firstRegisters[cont1].idServidor < secondRegisters[cont2].idServidor) {
			(*outputRegisters)[cont3++] = firstRegisters[cont1++];
		} else if (firstRegisters[cont1].idServidor > secondRegisters[cont2].idServidor) {
			(*outputRegisters)[cont3++] = secondRegisters[cont2++];
		} else if (firstRegisters[cont1].idServidor == secondRegisters[cont2].idServidor) {
			(*outputRegisters)[cont3++] = firstRegisters[cont1++];
			cont2++;
		}
	}

	// salva o que sobrou do primeiro arquivo
	while (cont1 < amount1)
		(*outputRegisters)[cont3++] = firstRegisters[cont1++];
	
	// salva o que sobrou do segundo arquivo
	while (cont2 < amount2)
		(*outputRegisters)[cont3++] = secondRegisters[cont2++];

	return cont3;
}

/*---------------------------- Match Registers ----------------------------*/
int matchRegisters(Register* firstRegisters, int amount1, Register* secondRegisters, int amount2, Register** outputRegisters) {
	int cont1 = 0;
	int cont2 = 0;
	int cont3 = 0;
	
	// percorre vetores com dois ponteiros
	while (cont1 < amount1 && cont2 < amount2) {
		if (firstRegisters[cont1].idServidor < secondRegisters[cont2].idServidor) {
			cont1++;
		} else if (firstRegisters[cont1].idServidor > secondRegisters[cont2].idServidor) {
			cont2++;
		} else if (firstRegisters[cont1].idServidor == secondRegisters[cont2].idServidor) {
			(*outputRegisters)[cont3++] = firstRegisters[cont1++];
			cont2++;
		}
	}

	return cont3;
}