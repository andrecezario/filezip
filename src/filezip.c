#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

typedef unsigned char byte;

//*******************************ÁRVORE DE HUFFMAN*********************************
typedef struct huffman_tree {
	int frequency;
	byte item;
	struct huffman_tree *left;
	struct huffman_tree *right;
	struct huffman_tree *next;
} huffman_tree;

huffman_tree* create_empty_huffman_tree() {
	return NULL;
}

huffman_tree* create_huffman_tree(int frequency, byte item, huffman_tree *left,
		huffman_tree *right, huffman_tree *next) {
	huffman_tree *new_huffman_tree = (huffman_tree*) malloc(
			sizeof(huffman_tree));
	new_huffman_tree->frequency = frequency;
	new_huffman_tree->item = item;
	new_huffman_tree->left = left;
	new_huffman_tree->right = right;
	new_huffman_tree->next = next;

	return new_huffman_tree;
}

int is_empty_huff_tree(huffman_tree *huff_tree) {
	return (huff_tree == NULL);
}

void print_tree(huffman_tree *tree) {
	if (tree != NULL) {
		printf("%c", tree->item);
		print_tree(tree->left);
		print_tree(tree->right);
	}
}
//********************************FILA DE PRIORIDADE*******************************
huffman_tree* enqueue_node(huffman_tree *head_huff_tree,
		huffman_tree *new_huff_tree) {
	//Inseri na cabeça
	if (is_empty_huff_tree(head_huff_tree)) {
		head_huff_tree = new_huff_tree;
	}

	else if (head_huff_tree->frequency >= new_huff_tree->frequency) {
		new_huff_tree->next = head_huff_tree;
		head_huff_tree = new_huff_tree;
	}
	//Se não, percorre a fila até achar o de maior frequência e adicionar antes
	else {
		huffman_tree *current = head_huff_tree;

		while ((current->next != NULL)
				&& (current->next->frequency < new_huff_tree->frequency)) {
			current = current->next;
		}

		new_huff_tree->next = current->next;
		current->next = new_huff_tree;
	}

	return head_huff_tree;
}

//*************************************HASH-TABLE**********************************
typedef struct element {
	byte key;
	char binary[9];
} element;

typedef struct hash_table {
	element *table[MAX_SIZE];
} hash_table;

hash_table* create_hash_table() {
	hash_table *new_hash_table = (hash_table*) malloc(sizeof(hash_table));
	int i;
	for (i = 0; i < MAX_SIZE; i++) {
		new_hash_table->table[i] = NULL;
	}
	return new_hash_table;
}

void put(hash_table *ht, byte key, char binary[]) {
	int h = (int) key, i;
	if (ht->table[h] != NULL) {
		for (i = 0; i < strlen(binary); i++)
			ht->table[h]->binary[i] = binary[i];
	} else {
		element *new_element = (element*) malloc(sizeof(element));
		new_element->key = key;
		for (i = 0; i < strlen(binary); i++)
			new_element->binary[i] = binary[i];
		ht->table[h] = new_element;
	}
}

/*element* get(hash_table *ht, byte key) {
 int h = (int) key;
 if (ht->table[h] != NULL && ht->table[h]->key == key) {
 return ht->table[h];
 } else {
 return NULL;
 }
 }*/
//************************************************COMPRESS***********************************
void frequency_bytes(FILE *file, int *frequency) {
	int i, size;
	//Tamanho do arquivo
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Frequência de cada byte
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

		byte caracter_parent = '*';
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

int size_huffman_tree = 0;
void write_tree_pre_order(FILE *file, huffman_tree *huff_tree) {
	if (huff_tree != NULL) {
		//Se for uma folha e o item for igual a * ou \ = '\\',escreva '\\'
		if (huff_tree->left == NULL && huff_tree->right == NULL) {
			if (huff_tree->item == '\\' || huff_tree->item == '*') {

				byte aux = '\\';
				fputc(aux, file);
				size_huffman_tree++;
			}
		}

		fputc(huff_tree->item, file);
		size_huffman_tree++;

		write_tree_pre_order(file, huff_tree->left);
		write_tree_pre_order(file, huff_tree->right);
	}
}

void compress() {
	char address_file[250];
	char name_file_compress[250];
	int i, frequency[256] = { 0 };
	FILE *file;
	FILE *file_compress;
	byte item;
	huffman_tree *huff_tree = create_empty_huffman_tree();
	huffman_tree *new_huff_tree = NULL;
	hash_table *ht_code = create_hash_table();

	printf("Digite o endereço do arquivo de entrada: ");
	scanf("%s", address_file);
	printf("Digite o nome do arquivo de saída: ");
	scanf("%s", name_file_compress);

	file = fopen(address_file, "rb");

	strcat(name_file_compress, ".huff");
	file_compress = fopen(name_file_compress, "wb");

	if (file == NULL)
		printf("Dados inválidos!");

	else {
		frequency_bytes(file, frequency);

		//Adicionar itens na fila de prioridade
		for (i = 0; i < MAX_SIZE; i++) {
			item = i;
			if (frequency[i] > 0) {
				new_huff_tree = create_huffman_tree(frequency[i], item, NULL,
				NULL, NULL);
				huff_tree = enqueue_node(huff_tree, new_huff_tree);
			}
		}

		//Converter fila em árvore
		huff_tree = convert_queue_to_tree(huff_tree);

		//Sequências de bits na hash
		char binary[9];
		build_code_table(ht_code, huff_tree, binary, 0);

		//COMPRIMIR ARQUIVO
		//Reserva os 16 bits para o lixo e tamanho da árvore
		unsigned char byte = 0;
		fputc(byte, file_compress);
		fputc(byte, file_compress);

		//Adicionando árvore
		write_tree_pre_order(file_compress, huff_tree);

		//Adicionando bytes codificados
		write_code_compress(file, file_compress, ht_code, size_huffman_tree);

		fclose(file_compress);
		fclose(file);
	}
}

//**********************************************DECOMPRESS***********************************
huffman_tree* search_tree(FILE *file, huffman_tree *huff_tree) {
	unsigned char byte = getc(file);
	int condition = 1; //se for um pai

	if (byte == '\\') {
		byte = getc(file); //pega o próximo, pode se um '*' ou '\'
		condition = 0;

	} else if (byte != '*') { //é uma folha
		condition = 0;
	}

	huff_tree = create_huffman_tree(0, byte, NULL, NULL, NULL);

	if (condition) //é um pai
	{
		huff_tree->left = search_tree(file, huff_tree->left);
		huff_tree->right = search_tree(file, huff_tree->right);
	}
	return huff_tree;
}

void write_descompress(FILE *file, FILE *file_descompress,
		huffman_tree *huff_tree, int size_tree, int trash) {
	int i,j,size_file;
	huffman_tree *aux_tree = huff_tree;
	unsigned char byte, last_byte;

	//Tamanho da arquivo
	fseek(file, 0, SEEK_END);
	size_file = ftell(file);
	fseek(file, 0, SEEK_SET);
	fseek(file, 2 + size_tree, SEEK_SET); //posicionar o ponteiro a 2+size_tree bytes

	//Escrevendo no arquivo
	for (i = 0; i < size_file - size_tree - 3; i++) { //lemos apenas os bytes do arquivo codificado (-tamanho da árvore -2 bytes iniciais - 1 byte final
		byte = fgetc(file);

		for (j = 0; j < 8; j++) {

			if (aux_tree->left == NULL && aux_tree->right == NULL) //é uma folha
			{
				fputc(aux_tree->item, file_descompress);
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

	last_byte = fgetc(file);

	//escrevendo lixo
	for (i = 0; i < (8 - trash); i++) {
		if (last_byte & 128) //1; 128 -> 10000000
				{
			aux_tree = aux_tree->right;
		} else //0
		{
			aux_tree = aux_tree->left;
		}
		last_byte = last_byte << 1;

		if (aux_tree->left == NULL && aux_tree->right == NULL) //é uma folha
		{
			fputc(aux_tree->item, file_descompress);
			aux_tree = huff_tree;
		}
	}
}

void descompress() {
	char address_file[250];
	char name_file_descompress[250];
	FILE *file;
	FILE *file_descompress;
	huffman_tree *huff_tree = create_empty_huffman_tree();

	printf("Digite o endereço do arquivo de entrada: ");
	scanf("%s", address_file);
	printf("Digite o nome do arquivo de saída: ");
	scanf("%s", name_file_descompress);

	file = fopen(address_file, "rb");
	file_descompress = fopen(name_file_descompress, "wb");

	if (file == NULL)
		printf("Dados inválidos!");

	else {
		//DESCOMPRIMIR ARQUIVO
		// Tamanho do lixo
		int trash, size_tree = 0; //int -> 16 bits
		unsigned char byte = fgetc(file);
		trash = byte >> 5;

		// Tamanho da árvore
		size_tree = byte & 31; //15 = 00011111
		size_tree = size_tree << 8;
		byte = fgetc(file);
		size_tree = size_tree | byte;

		//Escrevendo arquivo descomprimido
		write_descompress(file, file_descompress, search_tree(file, huff_tree),
				size_tree, trash);
	}
}
//*******************************************MAIN****************************************
void menu() {
	int command;
	printf("Escolha uma das opções:\n");
	printf("1 p/ comprimir ou 2 p/ descomprimir\n");
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
