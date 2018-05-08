#ifndef DATA_STRUCTURE_H_INCLUDED
#define DATA_STRUCTURE_H_INCLUDED

#define MAX_SIZE 256

//*******************************ÁRVORE DE HUFFMAN*********************************
typedef struct huffman_tree {
	int frequency;
	void *item;
	struct huffman_tree *left;
	struct huffman_tree *right;
	struct huffman_tree *next;
} huffman_tree;

huffman_tree* create_empty_huffman_tree();

huffman_tree* create_huffman_tree(int frequency, void* item,
		 huffman_tree *left, huffman_tree *right, huffman_tree *next);


int is_empty_huff_tree(huffman_tree *huff_tree);


void print_tree(huffman_tree *tree);

huffman_tree* enqueue_node(huffman_tree *head_huff_tree,
		huffman_tree *new_huff_tree);

//********************************FILA DE PRIORIDADE*******************************
typedef struct element {
	void *key;
	char binary[9];
} element;

typedef struct hash_table {
	element *table[MAX_SIZE];
} hash_table;

hash_table* create_hash_table();

void put(hash_table *ht, void* key, char binary[]);

#endif // DATA_STRUCTURE_H_INCLUDED
