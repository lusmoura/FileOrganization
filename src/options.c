/* Opções: declaração e funções associadas*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "options.h"
#include "index.h"

/*--------------------------------- OP14 ----------------------------------*/
void op14(char* fileName, char* fileName2, char* fieldName, char* value) {
	int noIndex = 1, withIndex = 1;
	
	// executa busca sem arquivo de indices
	printf("*** Realizando a busca sem o auxílio de índice\n");
	op3(fileName, fieldName, value, &noIndex);
	
	// executa busca com arquivo de indices
	printf("*** Realizando a busca com o auxílio de um índice secundário fortemente ligado\n");
	op11(fileName, fileName2, fieldName, value, &withIndex);
	
	// mostra a diferença nos acessos
	printf("\nA diferença no número de páginas de disco acessadas: %d\n", noIndex - withIndex);
	return;
}

/*--------------------------------- OP13 ----------------------------------*/
void op13(char* fileName, char* fileName2, int times) {
	// abre arquivos e checa se há erro
	FILE* input = fopen(fileName, "rb");
	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	
	FILE* indexFile = fopen(fileName2, "rb+");
	if (indexFile == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		return;
	}

	Header* header = readHeader(input);
	if(header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		fclose(indexFile);
		return;
	}

	IndexHeader* indHeader = readIndexHeader(indexFile);
	if(indHeader == NULL || indHeader->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		free(header);
		fclose(input);
		fclose(indexFile);
		return;
	}
	
	int amount = indHeader->nroRegistros;
	Register* reg = NULL;
	char salarioStr[10] = "";
	char telefone[20] = "";
	char cargo[300] = "";
	char nome[300] = "";
	double salario;
	int id = 0;
	
	// inicializa lista de removidos
	List* removedList = createList();
	createRemovedList(input, removedList, header->topoLista);
	
	// carrega indices para a RAM
	Index indexes[maxSize];
	for (int i = 0; i < amount; i++)
		indexes[i] = readIndex(indexFile);

	// executa os n testes
    for (int i = 0; i < times; i++) {
        // le campos
		scanf("%d,", &id);
		
		scan_quote_string(salarioStr);
		if (!strcmp(salarioStr, "")) salario = -1;
		else sscanf(salarioStr, "%lf", &salario);
		
		scan_quote_string(telefone);
		scan_quote_string(nome);
		scan_quote_string(cargo);

		// cria registro
		reg = createRegister(id, salario, telefone, nome, cargo);            
        
		// insere registro
        int pos = insertRegister(input, reg, removedList);
		
		// caso o nome não seja nulo, insere no vetor de indices
		if(strlen(nome) > 0)
			insertIndex(reg, indexes, indHeader, pos);
		
		if (reg != NULL) free(reg);
    }

	// atualiza status e encadeamento no header 
	if (removedList->size == 0) 
		header->topoLista = -1;
	else 
		header->topoLista = removedList->begin->next->offset;
	
	updateLink(input, removedList);

	// reescreve o arquivo de indices - abre como escrita, ordena novamente e por fim reescreve
	freopen(fileName2, "wb+", indexFile);
	qsort(indexes, indHeader->nroRegistros, sizeof(Index), indexComparator);
	rewriteIndexFile(indexFile, indHeader, indexes);
	setAsConsistent(indexFile);

	binarioNaTela1(indexFile);

	destroyList(removedList);	
	if(reg != NULL) free(reg);
	if(input != NULL) fclose(input);
	if(header != NULL) free(header);
	if(indHeader != NULL) free(indHeader);
	if(indexFile != NULL) fclose(indexFile);

	return;
}
 
 /*--------------------------------- OP12 ----------------------------------*/
void op12(char* fileName, char* fileName2, int times) {
	// abre arquivos e checa se há erro
	FILE* input = fopen(fileName, "rb");
	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	
	FILE* indexFile = fopen(fileName2, "rb+");
	if (indexFile == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		return;
	}

	Header* header = readHeader(input);
	if(header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		fclose(indexFile);
		return;
	}

	IndexHeader* indHeader = readIndexHeader(indexFile);
	if(indHeader == NULL || indHeader->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		free(header);
		fclose(input);
		fclose(indexFile);
		return;
	}

	// inicializa lista de removidos
	List* removedList = createList();
	createRemovedList(input, removedList, header->topoLista);
    
	int amount = indHeader->nroRegistros;
	Index indexes[maxSize];
	Register* reg = NULL;
    int numRegs = 0;
	char fieldName[50];
	char value[300];

	// carrega indices para a RAM
	for (int i = 0; i < amount; i++)
		indexes[i] = readIndex(indexFile);

	// le todos os casos de teste buscando os registros
	for (int i = 0; i < times; i++) {
		scanf("%s", fieldName);	
        scan_quote_string(value);

		// busca registro
		int pos = binarySearch(indexes, value, amount);
		fseek(input, indexes[pos].byteOffset, SEEK_SET);
		readRegister(&reg, input, ftell(input), &numRegs);
		
		// enquanto houver um registro que atende a especificação
		while(!strcmp(value, reg->nomeServidor)) {
			if(reg->removido != '-') continue;
			
			// insere na lista de removidos
			insertList(removedList, reg->tamanhoRegistro, indexes[pos].byteOffset, i);
			// remove logicamente do vetor
			removeIndex(&indexes[pos]);
			(indHeader->nroRegistros)--;
			
			// lê o proximo
			if(reg != NULL) free(reg);
			fseek(input, indexes[++pos].byteOffset, SEEK_SET);
			readRegister(&reg, input, ftell(input), &numRegs);
		}
	}

	// atuliza arquivo de registros
	removeAllRegisters(input, removedList, header);
	updateLink(input, removedList);
	
	// reescreve o arquivo de indices - abre como escrita, ordena novamente e por fim reescreve
	freopen(fileName2, "wb+", indexFile);
	qsort(indexes, amount, sizeof(Index), indexComparator);
	rewriteIndexFile(indexFile, indHeader, indexes);
	setAsConsistent(indexFile);

	// imprime resultado
	binarioNaTela1(indexFile);

	destroyList(removedList);
	if(reg != NULL) free(reg);
	if(input != NULL) fclose(input);
	if(header != NULL) free(header);
	if(indHeader != NULL) free(indHeader);
	if(indexFile != NULL) fclose(indexFile);
	return;
}

/*--------------------------------- OP11 ----------------------------------*/
void op11(char* fileName, char* fileName2, char* fieldName, char* name, int* access) {
	// abre arquivos e checa se há erro
	FILE* input = fopen(fileName, "rb");
	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	
	FILE* indexFile = fopen(fileName2, "rb+");
	if (indexFile == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		return;
	}

	Header* header = readHeader(input);
	if(header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		fclose(indexFile);
		return;
	}

	IndexHeader* indHeader = readIndexHeader(indexFile);
	if(indHeader == NULL || indHeader->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		free(header);
		fclose(input);
		fclose(indexFile);
		return;
	}
	
	int amount = indHeader->nroRegistros;
	Index indexes[maxSize];
	Register* reg = NULL;
	int lastPage = 0;
	int numRegs = 0;
	bool statistics;

	// checa flag que indica se será feita a contagem para a opção 14
	if (*access != 1) {
		*access = 1;
		statistics = false;
	} else
		statistics = true;
	
	// carrega indices para a RAm
	for (int i = 0; i < amount; i++)
		indexes[i] = readIndex(indexFile);

	// busca registro e o lê
	int pos = binarySearch(indexes, name, amount);
	fseek(input, indexes[pos].byteOffset, SEEK_SET);
	
	readRegister(&reg, input, ftell(input), &numRegs);
	
	// percorre todos os registros que atendem à busca
	numRegs = 0;
	while(!strcmp(name, reg->nomeServidor)) {
		printRegister(reg, header, 3);
		numRegs++;
		pos++;

		// le o proximo registro
		if(reg != NULL) free(reg);
		fseek(input, indexes[pos].byteOffset, SEEK_SET);
		readRegister(&reg, input, ftell(input), &numRegs);
		
		// contabiliza se houve um acesso a uma nova pagina de disco
		if(indexes[pos].byteOffset/pageSize != lastPage) {
			lastPage = indexes[pos].byteOffset/pageSize;
			(*access)++;
		}	
	}
	
	// imprime resultado
	if(numRegs == 0)
		printf("Registro inexistente.\n");
	
	if (numRegs != 0 || statistics == true){
		printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", fileSize(indexFile)/pageSize + 1);
		printf("Número de páginas de disco para acessar o arquivo de dados: %d\n", *access);
	}

	if(reg != NULL) free(reg);
	if(input != NULL) fclose(input);
	if(header != NULL) free(header);
	if(indHeader != NULL) free(indHeader);
	if(indexFile != NULL) fclose(indexFile);
	
	return;
}

/*--------------------------------- OP10 ----------------------------------*/
void op10(char* fileName, char* fileName2) {
	// abre arquivos e checa se há erro
	FILE* input = fopen(fileName, "rb");
	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	
	FILE* indexFile = fopen(fileName2, "wb+");
	if (indexFile == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		return;
	}

	Header* header = readHeader(input);
	if(header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(input);
		fclose(indexFile);
		return;
	}

	int filesize = fileSize(input);
	Index indexes[maxSize];
	Register* reg = NULL;
	int numRegs = 0;
	int amount = 0;
	
	// cria um header de indices e o escreve
	IndexHeader* indHeader = createIndexHeader();
	writeIndexHeader(indHeader, indexFile);

	// percorre arquivo de registros inserindo-os no vetor de indices caso tenham nome e não estejam removidos
	while(ftell(input) < filesize) {
		int pos = ftell(input);
		readRegister(&reg, input, pos, &numRegs);
		
		if (reg == NULL) continue;
		else if (reg->removido != '-' || reg->tamNomeServidor == 0) {
			free(reg);
			continue;
		}

		indexes[amount] = createIndex(pos, reg->nomeServidor);
		amount++;

		if(reg != NULL) free(reg);
	}

	// ordena indices pelo nome
	qsort(indexes, amount, sizeof(Index), indexComparator);

	// escreve todos os indices no arquivo
	for (int i = 0; i < amount; i++)
		writeIndex(indexes+i, indexFile);

	// atualiza header
	updateIndexHeader(indHeader, amount);
	writeIndexHeader(indHeader, indexFile);
	setAsConsistent(indexFile);

	binarioNaTela1(indexFile);
	
	if(indHeader != NULL) free(indHeader);
	if(header != NULL) free(header);
	if(input != NULL) fclose(input);
	if(indexFile != NULL) fclose(indexFile);
	
	return;
}

/*---------------------------------- OP9 ----------------------------------*/
void op9(char* fileName, char* fileName2, char* outputName) {
	// abre arquivos e checa se há erro
	FILE* output = fopen(outputName, "wb+");
	if (output == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	FILE* first = fopen(fileName, "rb");
	if (first == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(output);
		return;
	}
	
	FILE* second = fopen(fileName2, "rb");
	if (second == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(first);
		fclose(output);
		return;
	}

	// le headers e checa se há erro
	Header* header1 = readHeader(first);
	Header* header2 = readHeader(second);

	if(header1 == NULL || header2 == NULL || header1->status == '0' || header2->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		fclose(first);
		fclose(output);
		return;
	}

	Register* firstRegisters = calloc(maxSize, sizeof(Register));
	Register* secondRegisters = calloc(maxSize, sizeof(Register));
	Register* outputRegisters = calloc(2*maxSize, sizeof(Register));
	int lastSize = 0;

	// le registros e os ordena
	int amount1 = sortRegisters(first, &firstRegisters);
	int amount2 = sortRegisters(second, &secondRegisters);

	// escreve header 
	header1->topoLista = -1;
	writeHeader(output, header1);
	setAsInconsistent(output);

	// faz o matching
	int cont = matchRegisters(firstRegisters, amount1, secondRegisters, amount2, &outputRegisters);

	// escreve registros no arquivo de saída
	for (int i = 0; i < cont; i++) {
		writeRegister(outputRegisters+i, output, lastSize);
		lastSize = outputRegisters[i].tamanhoRegistro;
	}

	setAsConsistent(output);
	binarioNaTela1(output);

	// libera memória
	if (header1 != NULL) free(header1);
	if (header2 != NULL) free(header2);
	if (first != NULL) fclose(first);
	if (second != NULL) fclose(second);
	if(firstRegisters != NULL) free(firstRegisters);
	if(secondRegisters != NULL) free(secondRegisters);
	if(outputRegisters != NULL) free(outputRegisters);

	return;
}

/*---------------------------------- OP8 ----------------------------------*/
void op8(char* fileName, char* fileName2, char* outputName) {
	// abre arquivos e checa se há erro
	FILE* output = fopen(outputName, "wb+");
	if (output == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	FILE* first = fopen(fileName, "rb");
	if (first == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(output);
		return;
	}
	
	FILE* second = fopen(fileName2, "rb");
	if (second == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(first);
		fclose(output);
		return;
	}

	// le headers e checa se há erros
	Header* header1 = readHeader(first);
	Header* header2 = readHeader(second);

	if(header1 == NULL || header2 == NULL || header1->status == '0' || header2->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		if(header1 != NULL) free(header1);
		if(header2 != NULL) free(header2);
		fclose(first);
		fclose(output);
		return;
	}

	Register* firstRegisters = calloc(maxSize, sizeof(Register));
	Register* secondRegisters = calloc(maxSize, sizeof(Register));
	Register* outputRegisters = calloc(2*maxSize, sizeof(Register));
	int lastSize = 0;

	// le e ordena os arquivos
	int amount1 = sortRegisters(first, &firstRegisters);
	int amount2 = sortRegisters(second, &secondRegisters);

	// escreve cabeçalho
	header1->topoLista = -1;
	writeHeader(output, header1);
	setAsInconsistent(output);

	// faz o merge dos arquivos
	int cont = mergeRegisters(firstRegisters, amount1, secondRegisters, amount2, &outputRegisters);

	// escreve na saída
	for (int i = 0; i < cont; i++) {
		writeRegister(outputRegisters+i, output, lastSize);
		lastSize = outputRegisters[i].tamanhoRegistro;
	}

	setAsConsistent(output);
	binarioNaTela1(output);

	// libera memória
	if (header1 != NULL) free(header1);
	if (header2 != NULL) free(header2);
	if (first != NULL) fclose(first);
	if (second != NULL) fclose(second);
	if(firstRegisters != NULL) free(firstRegisters);
	if(secondRegisters != NULL) free(secondRegisters);
	if(outputRegisters != NULL) free(outputRegisters);

	return;
}

/*---------------------------------- OP7 ----------------------------------*/
void op7(char* firstFileName, char* secondFIleName) {
	// abre arquivos e checa erro
	FILE* input = fopen(firstFileName, "rb");
	FILE* output = fopen(secondFIleName, "wb+");

	if (input == NULL || output == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	Header* header = readHeader(input);
	
    if (header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}

	Register* registers = calloc(sizeof(Register), maxSize);
	int lastSize = 0;

	// ignora pagina do header
	fseek(input, pageSize, SEEK_SET);
	
	// chamada função que le registro e os ordena
	int amount = sortRegisters(input, &registers);

	// escreve na saida o cabeçalho
	header->topoLista = -1;
	writeHeader(output, header);
	setAsInconsistent(output);

	// escreve na saida todos os registros depois de ordenados
	for (int i = 0; i < amount; i++) {
		writeRegister(registers+i, output, lastSize);
		lastSize = registers[i].tamanhoRegistro;
	}

	setAsConsistent(output);

	binarioNaTela1(output);
	
	// libera memória
	if (header != NULL) free(header);
	if (registers != NULL) free(registers);
	if (input != NULL) fclose(input);
	if (output != NULL) fclose(output);
	
	return;
}

/*---------------------------------- OP6 ----------------------------------*/
void op6(char* fileName, int times) {
    // abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb+");

	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	Header* header = readHeader(input);
	
    if (header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}

	// atualiza status do header e cria lista de registros removidos
	setAsInconsistent(input);
	List* removedList = createList();
	createRemovedList(input, removedList, header->topoLista);

	Register* reg = NULL;
	int filesize = fileSize(input);
	char searchField[30] = "";
	char searchValue[200] = "";
	char newField[30] = "";
	char newValue[200] = "";
	bool found = false;
	int numRegs = 0;
	
	// executa os n testes
    for (int i = 0; i < times; i++) {
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
			readRegister(&reg, input, ftell(input), &numRegs);
			
			if (reg == NULL) continue;
			
			// busca registro
			found = searchRegister(reg, header, searchField, searchValue);
			
			// se encontrou, checa se houve alteração no tamanho
			if (found == true) {
				int prevSize = reg->tamanhoRegistro;
				
				// atualiza registro
				reg = updateRegister(reg, header, newField, newValue);
				
				// tamanho menor ou igual apenas atualiza na mesma posição
				if (reg->tamanhoRegistro <= prevSize) {
					insertInPos(input, reg, pos, prevSize);
				// tamanho maior, insere na lista de removidos, remove fisicamente e reinsere 
				} else {
					insertList(removedList, prevSize, pos, 0);
					removeOneRegister(input, pos, prevSize, -1);
					insertRegister(input, reg, removedList);
					// caso a inserção tenha ocorrido no final do arquivo, atualiza-se o tamanho
					filesize = ftell(input);
				}

				if (searchField[0] == 'i') {
					if (reg != NULL) free(reg);
					break;
				}
			}
		
			if (reg != NULL) free(reg);
		}
    }

	// atualiza encadeamento
	updateLink(input, removedList);
	
	// atualiza status e encadeamento no header 
	if (removedList->size == 0) 
		header->topoLista = -1;
	else 
		header->topoLista = removedList->begin->next->offset;
	
	setAsConsistent(input);
    
	// imprime arquivo
	binarioNaTela1(input);

	destroyList(removedList);
	if (header != NULL) free(header);
    if (input != NULL) fclose(input);
    return;
}

/*---------------------------------- OP5 ----------------------------------*/
void op5(char* fileName, int times) {
    // abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb+");

	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	Header* header = readHeader(input);

    if (header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}
    
	// atualiza status do header e cria lista de registros removidos
	setAsInconsistent(input);
	List* removedList = createList();
	createRemovedList(input, removedList, header->topoLista);

	int id = 0;
	double salario;
	char salarioStr[10] = "";
	char telefone[20] = "";
	char nome[300] = "";
	char cargo[300] = "";

	// executa os n testes
    for (int i = 0; i < times; i++) {
        // le campos
		scanf("%d,", &id);
		
		scan_quote_string(salarioStr);
		if (!strcmp(salarioStr, "")) salario = -1;
		else sscanf(salarioStr, "%lf", &salario);
		
		scan_quote_string(telefone);
		scan_quote_string(nome);
		scan_quote_string(cargo);

		// cria registro
		Register* reg = createRegister(id, salario, telefone, nome, cargo);            
        
		// insere registro
        insertRegister(input, reg, removedList);

		if (reg != NULL) free(reg);

    }

	// atualiza status e encadeamento no header 
	if (removedList->size == 0) 
		header->topoLista = -1;
	else 
		header->topoLista = removedList->begin->next->offset;
	
	updateLink(input, removedList);
	setAsConsistent(input);
    
	// imprime arquivo
	binarioNaTela1(input);

	destroyList(removedList);
	if (header != NULL) free(header);
    if (input != NULL) fclose(input);
    return;
}

/*---------------------------------- OP4 ----------------------------------*/
void op4(char* fileName, int times) {
    // abre arquivo e checa se há erro
    FILE* input = fopen(fileName, "rb+");

	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}
    
	// lê o header do arquivo binário e checha se há erro
	Header* header = readHeader(input);

    if (header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}
    
    // atualiza o status do header no arquivo
    setAsInconsistent(input);
    
    // inicializa lista de removidos com registros que já vem removidos no arquivo
    List* removedList = createList();
	createRemovedList(input, removedList, header->topoLista);
    
	Register* reg = NULL;
	int filesize = fileSize(input);
    int access = 0;
    int numRegs = 0;
    bool found = false;
	Query queries[times+1];

	// le e armazena todas as queries
	for (int i = 0; i < times; i++) {
		scanf("%s", queries[i].fieldName);	
        scan_quote_string(queries[i].value);
		queries[i].timeIn = i+1;
	}

	// ignora cabeçalho
    fseek(input, pageSize, SEEK_SET);
	
	// percorre o arquivo
	while(ftell(input) < filesize) {
	    // conta a quantidade de paginas de disco acessadas
		if (ftell(input) > pageSize * access) access++;
		
		// le um registro
		int pos = ftell(input);
		readRegister(&reg, input, pos, &numRegs);
		
		// ve se ele satisfaz alguma das queries
		for (int i = 0; i < times; i++) {
			// se a busca for por id, o timeIn sera substituido por -1 apos o primeiro match
			if (queries[i].timeIn == -1) continue;
			
			// busca registro
			found = searchRegister(reg, header, queries[i].fieldName, queries[i].value);
			
			// encontrando alguma query que ele satisfaça, o insere na lista de removidos
			if (found && reg->removido == '-') {
				insertList(removedList, reg->tamanhoRegistro, pos, queries[i].timeIn);
				
				// caso seja um id, é assinalado que essa query ja foi processada
				if (queries[i].fieldName[0] == 'i') queries[i].timeIn = -1;

				found = false;
				break;
			}
		}
	}

    // remove os registros fisicamente de acordo à lista e atualiza encadeamento no arquivo
	removeAllRegisters(input, removedList, header);
	updateLink(input, removedList);

    // atualiza o status do header no arquivo
    setAsConsistent(input);

    // imprime arquivo
    binarioNaTela1(input);
	
	destroyList(removedList);
    if (header != NULL) free(header);
    if (input != NULL) fclose(input);
    return;
}

/*---------------------------------- OP3 ----------------------------------*/
void op3(char* fileName, char* fieldName, char* value, int* access) {
	// abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb");

	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	Header* header = readHeader(input);

	if (header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}

	Register* reg = NULL;
	*access = 1;
	int numRegs = 0;
	bool foundOnThisPage = false;
	bool foundAtAll = false;

	// armazena tamanho do registro
	int filesize = fileSize(input);
	fseek(input, pageSize, SEEK_SET);
	
	// enquanto não chega no fim do arquivo, lê os registros
	while(ftell(input) < filesize) {
		
		// contabiliza a quantidade de acessos a disco
		if (ftell(input) > pageSize * (*access)) (*access)++;

		// lê registro do arquivo binário e faz a busca
		readRegister(&reg, input, ftell(input), &numRegs);
		foundOnThisPage = searchRegister(reg, header, fieldName, value);
		
		// imprime registro, se for encontrado
		if (foundOnThisPage) {
			printRegister(reg, header, 3);
			foundAtAll = true;
			foundOnThisPage = false;
			
			// se a busca for por um identificador, não é preciso continuar procurando
			// depois de encontrar a primeira ocorrencia
			if (fieldName[0] == 'i') break;
		}

		if (reg != NULL) free(reg);
	}

	if (foundAtAll == false) 
		printf("Registro inexistente.\n");
	
	printf("Número de páginas de disco acessadas: %d\n", *access);

	if (header != NULL) free(header);
	if (input != NULL) fclose(input);
	
	return;
}

/*---------------------------------- OP2 ----------------------------------*/
void op2(char* fileName) {
	
	// abre arquivo e checa se há erro
	FILE* input = fopen(fileName, "rb");

	if (input == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	// lê o header do arquivo binário e checha se há erro
	Header* header = readHeader(input);

	if (header == NULL || header->status == '0') {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}
    
	Register* reg = NULL;
    int access = 1;
	int numRegs = 0;

	// armazena tamanho do arquivo	
	int filesize = fileSize(input);
	fseek(input, pageSize, SEEK_SET);
	
	// enquanto não chega no fim do arquivo, lê os registros
	while(ftell(input) < filesize) {
		
		// contabiliza a quantidade de acessos a disco
		if (ftell(input) > pageSize * access) access++;

		// lê um registro do binário
		readRegister(&reg, input, ftell(input), &numRegs);
		
		// se a leitura foi bem sucedida, imprime o arquivo e libera memória
		if(reg == NULL) continue;
		else if (reg-> removido == '-') printRegister(reg, header, 2);
		
		free(reg);
	}

	if (numRegs != 0) 
		printf("Número de páginas de disco acessadas: %d\n", access);
	else 
		printf("Registro inexistente.\n");

	if (header != NULL) free(header);
	if (input != NULL) fclose(input);
	
	return;
}

/*---------------------------------- OP1 ----------------------------------*/
void op1(char* fileName) {
	
	// abre arquivos e checa se há erro
	FILE* input = fopen(fileName, "r");
	FILE* output = fopen("arquivoTrab1.bin", "wb");

	if (input == NULL || output == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return;
	}

	// cria um header a partir do csv e checha se há erro
	Header* header = createHeader(input, output);

	if (header == NULL) {
		printf("Falha no processamento do arquivo.\n");
		
		if (input != NULL) fclose(input);
		if (header != NULL) free(header);
		return;	
	}
    
    // atualiza o status do header no arquivo
    setAsInconsistent(input);

	// armazena tamanho do arquivo	
	int filesize = fileSize(input);
	int lastSize = 0;

	// enquanto nao chega ao final do arquivo de entrada, cria registros e
	// escreve-os no arquivo de saida
	while(ftell(input) < filesize) {
		Register* reg = NULL;
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
		if (id != 0) 
			reg = createRegister(id, salario, telefone, nome, cargo);
		
		if (reg != NULL) {
			writeRegister(reg, output, lastSize);
			lastSize = reg->tamanhoRegistro;
			free(reg);
		}
	}
    
    // atualiza o status do header no arquivo
    setAsConsistent(input);
	
    printf("arquivoTrab1.bin");
	
	if (header != NULL) free(header);
	if (input != NULL) fclose(input);
	if (output != NULL) fclose(output);
	return;
}