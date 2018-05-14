#ifndef DATA_STRUCTURE_H_INCLUDED
#define DATA_STRUCTURE_H_INCLUDED

#define MAX_SIZE 256

//*******************************ARVORE DE HUFFMAN*********************************
//Definicao da estrutura da arvore de huffman
typedef struct huffman_tree {
	int frequency;
	void *item;
	struct huffman_tree *left;
	struct huffman_tree *right;
	struct huffman_tree *next;
} huffman_tree;

//Criando a arvore vazia de huffman
huffman_tree* create_empty_huffman_tree();

/*Criando um no da arvore de huffman, a partir da frequencia do byte, o byte, a esquerda
 * e a direita, e um ponteiro para o proximo
 */
huffman_tree* create_huffman_tree(int frequency, void* item,
		 huffman_tree *left, huffman_tree *right, huffman_tree *next);

//Verifica se a arvore de huffman eh vazia
int is_empty_huff_tree(huffman_tree *huff_tree);

/*Funcao que recebe a cabeca da arvore de huffman e um no a ser enfileirado, enfileira  o mesmo,
 * gerando um fila de prioridade, inicialmente a arvore de huffman eh uma fila
 */
huffman_tree* enqueue_node(huffman_tree *head_huff_tree,
		huffman_tree *new_huff_tree);

//********************************FILA DE PRIORIDADE*******************************
/*Definicao do elemento da hash que é composto de uma chave key
 * e um char correspondente a codificacao de um byte com o algoritmo de huffman
 */
typedef struct element {
	void *key;
	char binary[9];
} element;

//Definicao da estrutura da hash-table
typedef struct hash_table {
	element *table[MAX_SIZE];
} hash_table;

//Criando a hash-table
hash_table* create_hash_table();

//Funcao de put que adiciona um element na hash, dada uma chave e sua codificacao
void put(hash_table *ht, void* key, char binary[]);

#endif // DATA_STRUCTURE_H_INCLUDED
