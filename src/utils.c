/* Funcoes secundarias utilizadas pelas opções */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/utils.h"

/*--------------------------------- Size ----------------------------------*/
int fileSize(FILE* fp) {
	int prevPos = ftell(fp);

	// move ponteiro para o final do arquivo
	fseek(fp, 0L, SEEK_END);
	
	// salva posicao dele
	int fileSize = ftell(fp);

	// move ponteiro para ignorar pagina de cabecalho
	fseek(fp, prevPos, SEEK_SET);
	
	return fileSize;
}

/*-------------------------- Create Removed List --------------------------*/
void createRemovedList(FILE* input, list* l, int64_t pos) {
	int numRegs = 0;

	// percorre arquivo por meio do encadeamento lista, inserindo os registros na lista
	while(pos != -1) {
		// move ponteiro para a próxima posição
		fseek(input, pos, SEEK_SET);
		// lê registro e o insere no final da lista
		reg* r = readRegister(input, ftell(input), &numRegs);
		insertListEnd(l, r->tamanhoRegistro, pos);
		pos = r->encadeamentoLista;
	}
	
	return;
}

/*--------------------------- Binario na Tela -----------------------------*/
void binarioNaTela1(FILE *ponteiroArquivoBinario) {
	unsigned char *mb;
	unsigned long i;
	size_t fl;
	fseek(ponteiroArquivoBinario, 0, SEEK_END);
	fl = ftell(ponteiroArquivoBinario);
	fseek(ponteiroArquivoBinario, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, ponteiroArquivoBinario);
	for(i = 0; i < fl; i += sizeof(unsigned char)) {
		printf("%02X ", mb[i]);
		if((i + 1) % 16 == 0)	printf("\n");
	}
	free(mb);
}

/*----------------------------- Update Link -------------------------------*/
void updateLink(FILE* input, list* l) {
	node* currNode = l->begin->next;
	int64_t nextPos = currNode->next->offset;

	// percorre lista atualizando o encadeamento
	while(currNode != NULL) {
		fseek(input, currNode->offset + 5, SEEK_SET);
		
		if(currNode->next == NULL)
			nextPos = -1;
		else 
			nextPos = currNode->next->offset;

		fwrite(&nextPos, sizeof(int64_t), 1, input);
		
		currNode = currNode->next;
	}

	return;
}

/*--------------------------- Scan Quote String ---------------------------*/
void scan_quote_string(char *str) {
	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}