/* Opções: declaração e funções associadas*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/options.h"

/*---------------------------------- OP6 ----------------------------------*/
void op6(char* fileName, int times, list* l){
    // abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb+");

	if(input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	header* h = readHeader(input);
	
    if(h == NULL || h->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		if(h != NULL) free(h);
		return;	
	}

	// atualiza status do header e cria lista de registros removidos
	updateHeader(input, h, '0');
	createRemovedList(input, l, h->topoLista);

	int filesize = fileSize(input);
	char searchField[30] = "";
	char searchValue[200] = "";
	char newField[30] = "";
	char newValue[200] = "";
	bool found = false;
	int numRegs = 0;
	
	// executa os n testes
    for(int i = 0; i < times; i++) {
		filesize = fileSize(input);
		rewind(input);
		
		// le dados busca
		scanf("%s", searchField);
		scan_quote_string(searchValue);

		// le dados atualização
		scanf("%s ", newField);
		scan_quote_string(newValue);

		// percorre arquivo buscando registros para atualizar
		while(ftell(input) < filesize) {
			int pos = ftell(input);
			// le registro
			reg* r = readRegister(input, ftell(input), &numRegs);
			
			if(r == NULL) continue;
			
			// busca registro
			found = searchRegister(r, h, searchField, searchValue);
			
			// se encontrou, checa se houve alteração no tamanho
			if(found == true) {
				int prevSize = r->tamanhoRegistro;
				// atualiza registro
				r = updateRegister(r, h, newField, newValue);
				
				// tamanho menor ou igual apenas atualiza na mesma posição
				if(r->tamanhoRegistro <= prevSize) {
					insertInPos(input, r, pos, prevSize);
				// tamanho maior, insere na lista de removidos, remove fisicamente e reinsere 
				} else {
					insertList(l, prevSize, pos);
					removeOneRegister(input, pos, prevSize, -1);
					insertRegister(input, r, l);
					// caso a inserção tenha ocorrido no final do arquivo, atualiza-se o tamanho
					filesize = ftell(input);
				}

				if(searchField[0] == 'i') {
					if(r != NULL) free(r);
					break;
				}
			}
		
			if(r != NULL) free(r);
		}
    }

	// atualiza encadeamento
	updateLink(input, l);
	
	// atualiza status e encadeamento no header 
	if(l->size == 0) h->topoLista = -1;
	else h->topoLista = l->begin->next->offset;
	updateHeader(input, h, '1');
    
	// imprime arquivo
	binarioNaTela1(input);

	if(h != NULL) free(h);
    if(input != NULL) fclose(input);
    return;
}

/*---------------------------------- OP5 ----------------------------------*/
void op5(char* fileName, int times, list* l){
    // abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb+");

	if(input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	header* h = readHeader(input);

    if(h == NULL || h->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		if(h != NULL) free(h);
		return;	
	}
    
	// atualiza status do header e cria lista de registros removidos
	updateHeader(input, h, '0');
	createRemovedList(input, l, h->topoLista);

	int id = 0;
	double salario;
	char salarioStr[10] = "";
	char telefone[20] = "";
	char nome[300] = "";
	char cargo[300] = "";

	// executa os n testes
    for(int i = 0; i < times; i++) {
        // le campos
		scanf("%d,", &id);
		
		scan_quote_string(salarioStr);
		if(!strcmp(salarioStr, "")) salario = -1;
		else sscanf(salarioStr, "%lf", &salario);
		
		scan_quote_string(telefone);
		scan_quote_string(nome);
		scan_quote_string(cargo);

		// cria registro
		reg* r = createRegister(id, salario, telefone, nome, cargo);            
        
		// insere registro
        insertRegister(input, r, l);

		if(r != NULL) free(r);

    }

	// atualiza status e encadeamento no header 
	if(l->size == 0) h->topoLista = -1;
	else h->topoLista = l->begin->next->offset;
	updateHeader(input, h, '1');
    
	// imprime arquivo
	binarioNaTela1(input);

	if(h != NULL) free(h);
    if(input != NULL) fclose(input);
    return;
}

/*---------------------------------- OP4 ----------------------------------*/
void op4(char* fileName, int times, list* l) {
    // abre arquivo e checa se há erro
    FILE* input = fopen(fileName, "rb+");

	if(input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
    
	// lê o header do arquivo binário e checha se há erro
	header* h = readHeader(input);

    if(h == NULL || h->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		if(h != NULL) free(h);
		return;	
	}
    
    // atualiza o status do header no arquivo
    updateHeader(input, h, '0');
    
    // inicializa lista de removidos com registros que já vem removidos no arquivo
    createRemovedList(input, l, h->topoLista);
    
    int filesize = fileSize(input);
    int access = 0;
    int numRegs = 0;
    char fieldName[50];
    char value[300];
    bool found = false;

    // executa os n testes
    for(int i = 0; i < times; i++) {
		scanf("%s", fieldName);
        scan_quote_string(value);
        
        // ignora pagina de header
        fseek(input, pageSize, SEEK_SET);

        while(ftell(input) < filesize) {
		
		    // contabiliza a quantidade de acessos a disco
		    if(ftell(input) > pageSize * access) access++;

		    // lê registro do arquivo binário e faz a busca
			int pos = ftell(input);
		    reg* r = readRegister(input, pos, &numRegs);
		    found = searchRegister(r, h, fieldName, value);
            
		    // insere registro na lista de removidos, se for encontrado e ainda não estiver removido
		    if(found && r->removido == '-') {
                insertList(l, r->tamanhoRegistro, pos);
                found = false;
                
				if(fieldName[0] == 'i') {
					if(r != NULL) free(r);
					break;
				}
		    }

		    if(r != NULL) free(r);
	    }
    }
    
    // remove os registros fisicamente de acordo à lista e atualiza encadeamento no arquivo
	removeAllRegisters(input, l, h);
	updateLink(input, l);

    // atualiza o status do header no arquivo
    updateHeader(input, h, '1');

    // imprime arquivo
    binarioNaTela1(input);
	
    if(h != NULL) free(h);
    if(input != NULL) fclose(input);
    return;
}

/*---------------------------------- OP3 ----------------------------------*/
void op3(char* fileName, char* fieldName, char* value){
	// abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb");

	if(input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	header* h = readHeader(input);

	if(h == NULL || h->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		if(h != NULL) free(h);
		return;	
	}

	int access = 1;
	int numRegs = 0;
	bool foundOnThisPage = false;
	bool foundAtAll = false;

	// armazena tamanho do registro
	int filesize = fileSize(input);
	fseek(input, pageSize, SEEK_SET);
	
	// enquanto não chega no fim do arquivo, lê os registros
	while(ftell(input) < filesize) {
		
		// contabiliza a quantidade de acessos a disco
		if(ftell(input) > pageSize * access) access++;

		// lê registro do arquivo binário e faz a busca
		reg* r = readRegister(input, ftell(input), &numRegs);
		foundOnThisPage = searchRegister(r, h, fieldName, value);

		// imprime registro, se for encontrado
		if(foundOnThisPage) {
			printRegister(r, h, 3);
			foundAtAll = true;
			foundOnThisPage = false;
			
			// se a busca for por um identificador, não é preciso continuar procurando
			// depois de encontrar a primeira ocorrencia
			if(fieldName[0] == 'i') break;
		}

		if(r != NULL) free(r);
	}

	if(foundAtAll) printf("Número de páginas de disco acessadas: %d\n", access);
	else printf("Registro inexistente.\n");

	if(h != NULL) free(h);
	if(input != NULL) fclose(input);
	
	return;
}

/*---------------------------------- OP2 ----------------------------------*/
void op2(char* fileName){
	
	// abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb");

	if(input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	header* h = readHeader(input);

	if(h == NULL || h->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		if(h != NULL) free(h);
		return;	
	}
    
	
    int access = 1;
	int numRegs = 0;

	// armazena tamanho do arquivo	
	int filesize = fileSize(input);
	fseek(input, pageSize, SEEK_SET);
	
	// enquanto não chega no fim do arquivo, lê os registros
	while(ftell(input) < filesize) {
		
		// contabiliza a quantidade de acessos a disco
		if(ftell(input) > pageSize * access) access++;

		// lê um registro do binário
		reg* r = readRegister(input, ftell(input), &numRegs);
		
		// se a leitura foi bem sucedida, imprime o arquivo e libera memória
		if(r != NULL) {
			printRegister(r, h, 2);
			free(r);
		}

	}

	if(numRegs != 0) printf("Número de páginas de disco acessadas: %d\n", access);
	else printf("Registro inexistente.\n");

	if(h != NULL) free(h);
	if(input != NULL) fclose(input);
	
	return;
}

/*---------------------------------- OP1 ----------------------------------*/
void op1(char* fileName){
	
	// abre arquivos e checa se há erro
	FILE* input = fopen(fileName, "r");
	FILE* output = fopen("arquivoTrab1.bin", "wb");

	if(input == NULL || output == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return;
	}

	// cria um header a partir do csv e checha se há erro
	header* h = createHeader(input, output);

	if(h == NULL) {
		printf("Falha no processamento do arquivo.\n");
		if(h != NULL) free(h);
		return;	
	}
    
    // atualiza o status do header no arquivo
    updateHeader(input, h, '0');

	// armazena tamanho do arquivo	
	int filesize = fileSize(input);
	int lastSize = 0;

	// enquanto nao chega ao final do arquivo de entrada, cria registros e
	// escreve-os no arquivo de saida
	while(ftell(input) < filesize) {
		reg* r = NULL;
		int id = 0;
		double salario;
		char telefone[20] = "";
		char nome[300] = "";
		char cargo[300] = "";

		// le dados do csv
		fscanf(input, "%d,", &id);
		fscanf(input, "%lf,", &salario);
		fscanf(input, "%[^,]", telefone);
		fscanf(input, ",%[^,]", nome);
		fscanf(input, ",%[^\n\r]", cargo);

		// cria registro, escreve-o na memoria e armazena seu tamanho
		if(id != 0) r = createRegister(id, salario, telefone, nome, cargo);
		if(r != NULL) {
			writeRegister(r, output, lastSize);
			lastSize = r->tamanhoRegistro;
			free(r);
		}
	}
    
    // atualiza o status do header no arquivo
    updateHeader(input, h, '1');
	
    printf("arquivoTrab1.bin");
	if(h != NULL) free(h);
	if(input != NULL) fclose(input);
	if(output != NULL) fclose(output);
	return;
}