/* Execução principal */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "registers.h"
#include "options.h"
#include "header.h"
#include "utils.h"
#include "list.h"

int main(int argc, char* argv[]) {
	int op, times, access = 0;
	char value[50];
	char fileName[50];
	char fileName2[50];
	char outputName[50];
	char fieldName[50];

	// lê opção e nome do arquivo
	scanf("%d", &op);
	scanf("%s", fileName);

	// chama a função referente a cada opção
	if (op == 1) {
		op1(fileName);

	} else if (op == 2) {
		op2(fileName);

	} else if (op == 3) {
		scanf("%s ", fieldName);
		scanf("%[^\r\n]", value);
		op3(fileName, fieldName, value, &access);

	} else if (op == 4) {
		scanf("%d\n", &times);
		op4(fileName, times);

	} else if (op == 5) {
		scanf("%d\n", &times);
		op5(fileName, times);

	} else if (op == 6) {
		scanf("%d\n", &times);
		op6(fileName, times);

	} else if (op == 7) {
		scanf("%s", fileName2);
		op7(fileName, fileName2);

	} else if (op == 8) {
		scanf("%s %s", fileName2, outputName);
		op8(fileName, fileName2, outputName);

	} else if (op == 9) {
		scanf("%s %s", fileName2, outputName);
		op9(fileName, fileName2, outputName);

	} else if (op == 10) {
		scanf("%s", fileName2);
		op10(fileName, fileName2);

	} else if (op == 11) {
		scanf("%s %s %[^\r\n]", fileName2, fieldName, value);
		op11(fileName, fileName2, fieldName, value, &access);

	} else if (op == 12) {
		scanf("%s %d", fileName2, &times);
		op12(fileName, fileName2, times);

	} else if (op == 13) {
		scanf("%s %d", fileName2, &times);
		op13(fileName, fileName2, times);

	} else if (op == 14) {
		scanf("%s %s %[^\r\n]", fileName2, fieldName, value);
		op14(fileName, fileName2, fieldName, value);
	}
	
	return 0;
}