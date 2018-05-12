#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "compress.h"

void frequency_bytes(FILE *file, int *frequency) {
	int i, size;
	//Tamanho do arquivo
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Frequencia de cada byte
	for (i = 0; i < size; i++) {
		frequency[fgetc(file)]++;
	}
}

huffman_tree* convert_queue_to_tree(huffman_tree* huff_tree) {
	huffman_tree *left;
	huffman_tree *right;
	huffman_tree *new_head;
	huffman_tree *parent;
	int new_frequency;

	while (huff_tree != NULL && huff_tree->next != NULL) {
		left = huff_tree;
		right = left->next;
		new_head = right->next;
		new_frequency = left->frequency + right->frequency;

		unsigned char *caracter_parent = (unsigned char*) malloc(sizeof(unsigned char));
		*caracter_parent = '*';
		parent = create_huffman_tree(new_frequency, caracter_parent, left,
				right, NULL);
		left->next = NULL;
		right->next = NULL;

		huff_tree = enqueue_node(new_head, parent);
	}

	return huff_tree;

}
void build_code_table(hash_table *ht, huffman_tree *huff_tree, char binary[],
		int size) {
	if (huff_tree->left == NULL && huff_tree->right == NULL) //é uma folha(?)
	{
		binary[size] = '\0'; //se não, os bits não preenchidos serão adicionados como lixo
		put(ht, huff_tree->item, binary); //adicione na hash

	} else {
		binary[size] = '0';
		build_code_table(ht, huff_tree->left, binary, size + 1);

		binary[size] = '1';
		build_code_table(ht, huff_tree->right, binary, size + 1);
	}
}

char set_bit(unsigned char byte, int i) {
	unsigned char mask = 0;
	mask = 1 << i;
	return mask | byte;
}

void write_code_compress(FILE *file, FILE *file_compress, hash_table *ht,
		int size_tree) {
	int i, j, size, shift, count_bit = 0;
	int old_caracter;
	unsigned char trash = 0;
	unsigned char byte = 0;

	//Tamanho da arquivo
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	for (i = 0; i < size; i++) {
		old_caracter = (int) fgetc(file);

		for (j = 0; ht->table[old_caracter]->binary[j] != '\0'; j++) {
			if (count_bit == 8) {
				fputc(byte, file_compress);
				byte = 0;
				count_bit = 0;
			}
			if (ht->table[old_caracter]->binary[j] == '1') {
				shift = 7 - count_bit;
				byte = set_bit(byte, shift);
			}

			count_bit++;
		}
	}

	fputc(byte, file_compress);

	fseek(file_compress, 0, SEEK_SET);
	trash = 8 - count_bit;
	trash = trash << 5;

	unsigned char byte_one = trash | size_tree >> 8;
	fputc(byte_one, file_compress);

	unsigned char byte_two = size_tree;
	fputc(byte_two, file_compress);
}

void write_tree_pre_order(FILE *file, huffman_tree *huff_tree, int *size_tree) {
	if (huff_tree != NULL) {
		//Se for uma folha e o item for igual a * ou \ = '\\',escreva '\\'
		if (huff_tree->left == NULL && huff_tree->right == NULL) {
			if ( *((unsigned char*)huff_tree->item) == '\\' ||  *((unsigned char*)huff_tree->item) == '*') {

				unsigned char aux = '\\';
				fputc(aux, file);
				*size_tree+=1;
			}
		}

		fputc(*((unsigned char*)huff_tree->item), file);
		*size_tree+=1;

		write_tree_pre_order(file, huff_tree->left, size_tree);
		write_tree_pre_order(file, huff_tree->right, size_tree);
	}
}
//************************************************COMPRESS***********************************

void compress()
{
	char address_file[250];
	char name_file_compress[250];
	int i, frequency[256] = { 0 };
	FILE *file;
	FILE *file_compress;
	huffman_tree *huff_tree = create_empty_huffman_tree();
	huffman_tree *new_huff_tree = NULL;
	hash_table *ht_code = create_hash_table();

	printf("Digite o endereço do arquivo de entrada: ");
	getchar();
	scanf("%[^\n]s", address_file);

	printf("Digite o nome do arquivo de saída: ");
	getchar();
	scanf("%[^\n]s",name_file_compress);

	file = fopen(address_file, "rb");

	strcat(name_file_compress, ".huff");
	file_compress = fopen(name_file_compress, "wb");

	if (file == NULL)
		printf("ERRO! Dados inválidos, verifique se o arquivo de entrada foi digitado corretamente e tente novamente.");

	else {
		frequency_bytes(file, frequency);

		//Adicionar itens na fila de prioridade
		for (i = 0; i < MAX_SIZE; i++) {
			unsigned char *item = (unsigned char*) malloc(sizeof(unsigned char));
			*item = i;
			if (frequency[i] > 0) {
				new_huff_tree = create_huffman_tree(frequency[i], item, NULL,
				NULL, NULL);
				huff_tree = enqueue_node(huff_tree, new_huff_tree);
			}
		}

		//Converter fila em arvore
		huff_tree = convert_queue_to_tree(huff_tree);

		//Sequencias de bits na hash
		char binary[9];
		build_code_table(ht_code, huff_tree, binary, 0);

		//COMPRIMIR ARQUIVO
		//Reserva os 16 bits para o lixo e tamanho da árvore
		unsigned char byte = 0;
		fputc(byte, file_compress);
		fputc(byte, file_compress);

		//Adicionando arvore
		int *size_tree = (int*) malloc(sizeof(int));
		*size_tree = 0;
		write_tree_pre_order(file_compress, huff_tree, size_tree);

		//Adicionando bytes codificados
		write_code_compress(file, file_compress, ht_code, *size_tree);

		fclose(file_compress);
		fclose(file);
	}
}
