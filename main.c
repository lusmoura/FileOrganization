/* BACKLOG
    - fazer op6
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "options.h"
#include "utils.h"
#include "list.h"

int main(int argc, char* argv[]) {
	int op, times;
	char value[50];
	char fileName[50];
	char fieldName[50];
	list* l = NULL;

	// lê opção e nome do arquivo
	scanf("%d", &op);
	scanf("%s", fileName);
	
	// chama a função referente a cada opção
	switch(op){
		case 1:
			op1(fileName);
			break;
		
		case 2:
			op2(fileName);
			break;
		
		case 3:	
			scanf("%s ", fieldName);
			scanf("%[^\r\n]", value);
			
			op3(fileName, fieldName, value);
			break;
		
		case 4:
			scanf("%d\n", &times);
			l = createList();
			
			op4(fileName, times, l);	
			
			destroyList(l);
			break;
		
		case 5:
			scanf("%d\n", &times);
			l = createList();
			
			op5(fileName, times, l);	
			
			destroyList(l);
			break;
		case 6:
            // fazer
			break;

		default:
			break;

	}

	return 0;
}