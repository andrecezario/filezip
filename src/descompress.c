#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "descompress.h"

huffman_tree* search_tree(FILE *file, huffman_tree *huff_tree) 
{
	unsigned char byte = getc(file);
	int condition = 1; //se for um pai

	if (byte == '\\') {
		byte = getc(file); //pega o proximo, pode se um '*' ou '\'
		condition = 0;

	} else if (byte != '*') { //eh uma folha
		condition = 0;
	}
	unsigned char *result = (unsigned char*) malloc(sizeof(unsigned char));
	*result = byte;
	huff_tree = create_huffman_tree(0, result, NULL, NULL, NULL);

	if (condition) //eh um pai
	{
		huff_tree->left = search_tree(file, huff_tree->left);
		huff_tree->right = search_tree(file, huff_tree->right);
	}
	return huff_tree;
}

void write_descompress(FILE *file, FILE *file_descompress,
		huffman_tree *huff_tree, int size_tree, int trash)
{
	int i,j,size_file;
	huffman_tree *aux_tree = huff_tree;
	unsigned char byte, last_byte;

	//Tamanho da arquivo
	fseek(file, 0, SEEK_END);
	size_file = ftell(file);
	fseek(file, 0, SEEK_SET);
	fseek(file, 2 + size_tree, SEEK_SET); //posicionar o ponteiro a 2+size_tree bytes

	//Escrevendo no arquivo
	for (i = 0; i < size_file - size_tree - 3; i++) { //lemos apenas os bytes do arquivo codificado (-tamanho da arvore -2 bytes iniciais - 1 byte final
		byte = fgetc(file);

		for (j = 0; j < 8; j++) {

			if (aux_tree->left == NULL && aux_tree->right == NULL) //e uma folha
			{
				fputc(*((unsigned char*)aux_tree->item), file_descompress);
				aux_tree = huff_tree;
			}

			if (byte & 128) //1
			{
				aux_tree = aux_tree->right;
			} else //0
			{
				aux_tree = aux_tree->left;
			}
			byte = byte << 1;

		}
	}

	if(trash!=0)
	{
		last_byte = fgetc(file);
		for (i = 0; i < (8 - trash); i++) {
			if (last_byte & 128) //1; 128 -> 10000000
					{
				aux_tree = aux_tree->right;
			} else //0
			{
				aux_tree = aux_tree->left;
			}
			last_byte = last_byte << 1;

			if (aux_tree->left == NULL && aux_tree->right == NULL) //eh uma folha
			{
				fputc(*((unsigned char*)aux_tree->item), file_descompress);
				aux_tree = huff_tree;
			}
		}
	}
}

void descompress() {
	char address_file[250];
	char name_file_descompress[250];
	FILE *file;
	FILE *file_descompress;
	huffman_tree *huff_tree = create_empty_huffman_tree();

	printf("-----------------------------------------------------------\n");
	printf("                         DECOMPRIMIR                         \n");
	printf("-----------------------------------------------------------\n");
	printf("Digite o endereço do arquivo de entrada: ");
	getchar();
	scanf("%[^\n]s", address_file);

	//Verificando extensao
    int condition = 1,i,j = 4,size = strlen(address_file);
    char *extension;
    extension = ".huff";

    for(i = size-1; i>=size-5 ; i--,j--)
        if(address_file[i] != extension[j])
        	condition = 0;

    if(condition)
    {
		printf("Digite o nome do arquivo de saída: ");
		getchar();
		scanf("%[^\n]s", name_file_descompress);

		file = fopen(address_file, "rb");
		file_descompress = fopen(name_file_descompress, "wb");

		if (file == NULL) {
			printf("\n*********************************************************\n");
			printf("ERRO! Dados inválidos, verifique se o arquivo de entrada \n"
					"foi digitado corretamente e tente novamente.");
			printf("\n*********************************************************");
		}
		else {
			//DESCOMPRIMIR ARQUIVO
			// Tamanho do lixo
			int trash, size_tree = 0; //int -> 16 bits
			unsigned char byte = fgetc(file);
			trash = byte >> 5;

			// Tamanho da arvore
			size_tree = byte & 31; //15 = 00011111
			size_tree = size_tree << 8;
			byte = fgetc(file);
			size_tree = size_tree | byte;

			//Escrevendo arquivo descomprimido
			write_descompress(file, file_descompress, search_tree(file, huff_tree),
					size_tree, trash);
			fclose(file_descompress);
			fclose(file);
			printf("\n*********************************************\n");
			printf("[SUCESSO] Arquivo descomprimido com êxito!");
			printf("\n*********************************************");
		}
    }
    else
    {
		printf("\n*********************************************\n");
    	printf("ERRO! O arquivo não possui a extensão .huff!"
    			"\nNão será possível descompactá-lo");
		printf("\n*********************************************");
    }
    printf("\n-----------------------------------------------------------\n");
}
