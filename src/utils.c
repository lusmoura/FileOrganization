/* Utilidades: Funcoes secundarias utilizadas pelas opções */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <ctype.h>
#include "utils.h"
#include "index.h"

char Lixo = '@';
int64_t oo = (1 << 30);

/*---------------------------- Id Comparator ------------------------------*/
int idComparator(const void* a, const void* b) {
	return ((Register*)a)->idServidor - ((Register*)b)->idServidor;
}

/*-------------------------- Index Comparator -----------------------------*/
int indexComparator(const void* a, const void* b) {
	Index* ind1 = (Index*)a;
	Index* ind2 = (Index*)b;
	
	if(!strcmp(ind1->chaveBusca, ind2->chaveBusca)) {
		return (ind1->byteOffset - ind2->byteOffset);
	} else {
		return strcmp(ind1->chaveBusca, ind2->chaveBusca);
	}
}

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
	for (i = 0; i < fl; i += sizeof(unsigned char)) {
		printf("%02X ", mb[i]);
		if ((i + 1) % 16 == 0)	printf("\n");
	}
	free(mb);
}

/*--------------------------- Scan Quote String ---------------------------*/
void scan_quote_string(char *str) {
	char R;

	while ((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if (R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if (R == '\"') {
		if (scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if (R != EOF) { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}

/*------------------------------ Real Size --------------------------------*/
int realSize(char* name) {
	int ret = 0;
	
	// percorre string contando os caracteres
	while(name[ret] != Lixo && name[ret] != '\0')
		ret++;

	return ret;
}

/*--------------------------- Fill With Trash -----------------------------*/
void fillWithTrash(FILE* input, int amount) {
	// percorre amount caracteres escrevendo @
	for (int i = 0; i < amount; i++)
		fwrite(&Lixo, sizeof(char), 1, input);

	return;
}