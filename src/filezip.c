#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"
#include "compress.h"
#include "descompress.h"

void menu() {
	int command;

	while(1)
	{
		printf("-----------------------------------------------------------\n");
		printf("                            FILEZIP                        \n");
		printf("-----------------------------------------------------------\n");
		printf("Selecione uma das opções:\n");
		printf("1 - COMPRIMIR ARQUIVO\n2 - DESCOMPRIMIR ARQUIVO\n3 - SAIR\n");
		scanf("%d", &command);

		if (command == 1)
			compress();
		else if (command == 2)
			descompress();
		else if (command == 3) {
			printf("Bye!\n");
			break;
		}
		else
			printf("[ERRO] Comando inválido, digite 1,2 ou 3!");
	}
}

int main() {
	menu();
}
