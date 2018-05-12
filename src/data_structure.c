#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"

//********************************ARVORE DE HUFFMAN*****************************
huffman_tree* create_empty_huffman_tree() {
	return NULL;
}

huffman_tree* create_huffman_tree(int frequency, void* item, huffman_tree *left,
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

//********************************FILA DE PRIORIDADE*******************************
huffman_tree* enqueue_node(huffman_tree *head_huff_tree,
		huffman_tree *new_huff_tree) {
	//Inseri na cabeca
	if (is_empty_huff_tree(head_huff_tree)) {
		head_huff_tree = new_huff_tree;
	}

	else if (head_huff_tree->frequency >= new_huff_tree->frequency) {
		new_huff_tree->next = head_huff_tree;
		head_huff_tree = new_huff_tree;
	}
	//Se nao, percorre a fila ate achar o de maior frequencia e adiciona
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
//*************************************HASH TABLE***********************************
hash_table* create_hash_table() {
	hash_table *new_hash_table = (hash_table*) malloc(sizeof(hash_table));
	int i;
	for (i = 0; i < MAX_SIZE; i++) {
		new_hash_table->table[i] = NULL;
	}
	return new_hash_table;
}

void put(hash_table *ht, void *key, char binary[]) {
	int h = *((int*) key), i;

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
