#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_structure.h"
#include "compress.h"
#include "descompress.h"

void menu() {
	int command;
	printf("Escolha uma das opções:\n");
	printf("1 para comprimir ou 2 para descomprimir\n");
	scanf("%d", &command);

	if (command == 1)
		compress();
	else if (command == 2)
		descompress();
	else
		printf("Comando inválido!");
}

int main() {
	menu();
}
