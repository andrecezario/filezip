#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE 100000

//*****************************ÁRVORE DE HUFFMAN*******************************
typedef struct huffman_tree {
	int frequency;
	unsigned char item; //substituir por *void
	struct huffman_tree *left;
	struct huffman_tree *right;
	struct huffman_tree *next;
} huffman_tree;

huffman_tree* create_empty_huffman_tree() {
	return NULL;
}

huffman_tree* create_huffman_tree(int frequency, unsigned char item, huffman_tree *left, huffman_tree *right, huffman_tree *next)
{
	huffman_tree *new_huffman_tree = (huffman_tree*) malloc(sizeof(huffman_tree));
	new_huffman_tree->frequency = frequency;
	new_huffman_tree->item = item;
	new_huffman_tree->left = left;
	new_huffman_tree->right = right;
	new_huffman_tree->next = next;
}

int is_empty_huff_tree(huffman_tree *huff_tree)
{
	return (huff_tree == NULL);
}

//********************************FILA DE PRIORIDADE*******************************
huffman_tree* enqueue_node(huffman_tree *head_huff_tree, huffman_tree *new_huff_tree)
{
	//Inseri na cabeça
	if(is_empty_huff_tree(head_huff_tree))
	{
		head_huff_tree = new_huff_tree;
	}

	else if (new_huff_tree->frequency >= head_huff_tree->frequency)
	{
		new_huff_tree->next = head_huff_tree;
		head_huff_tree = new_huff_tree;
	}
	//Se não, percorre a fila até achar o de maior frequência e adicionar antes
	else
	{
		huffman_tree *current = head_huff_tree;

		while((current->next != NULL) &&
			(current->next->frequency < new_huff_tree->frequency))
		{
			current = current->next;
		}

		new_huff_tree->next = current->next;
		current->next = new_huff_tree;
	}
}

huffman_tree* dequeued_node(huffman_tree *head_huff_tree)
{
	if(is_empty_huff_tree(head_huff_tree))
	{
		huffman_tree *node = head_huff_tree;
		head_huff_tree = head_huff_tree -> next;
		node->next = NULL;

		return node;
	}

	return NULL;
}

//*************************************HASH-TABLE**********************************
typedef struct element {
	char key; //substituir por *void
	int frequency;
} element;

typedef struct hash_table {
	element *table[MAX_SIZE];
} hash_table;

hash_table* create_hash_table()
{
	hash_table *new_hash_table = (hash_table*) malloc(sizeof(hash_table));
	int i;
	for(i=0;i<MAX_SIZE;i++)
	{
		new_hash_table->table[i] = NULL;
	}
	return new_hash_table;
}

void put(hash_table *ht,unsigned char key,int frequency)
{
	int h = (int) key;
	if(ht->table[h] != NULL)
	{
		ht->table[h]->frequency = frequency;
	}
	else
	{
		element *new_element = (element*) malloc(sizeof(element));
		new_element->key = key;
		new_element->frequency = frequency;
		ht->table[h] = new_element;
	}
}

element* get(hash_table *ht,unsigned char key)
{
	int h = (int) key;
	if(ht->table[h] != NULL && ht->table[h]->key == key)
	{
		return ht->table[h];
	}
	else
	{
		return NULL;
	}
}

//************************************************ARQUIVO***********************************
int frequency(FILE * caracter,unsigned char text[])
{
    int frequency = 0,i;

    for(i=0;i<strlen(text);i++)
    {
        if(text[i] == caracter) frequency++;
    }

    return frequency;
}

void print_queue(huffman_tree *tree)
{
	while(tree!=NULL)
	{
		printf("Item: %c / frequência: %d\n",tree->item, tree->frequency);
		tree = tree->next;
	}
}

//************************************************PRINCIPAL***********************************
int main()
{
	int i;
	unsigned char text[MAX_SIZE];
	fgets(text, MAX_SIZE,stdin);

	huffman_tree *tree = create_empty_huffman_tree();
	huffman_tree *new_huff_tree = NULL;

	hash_table *ht = create_hash_table();

	for(i=0;i<strlen(text);i++)
	{
		//new_huff_tree = create_huffman_tree(frequency(text[i],text),text[i],NULL,NULL,NULL);
		//tree = enqueue_node(tree,new_huff_tree);
		put(ht,text[i],frequency(text[i],text));

	}

	for(i=0;i<MAX_SIZE;i++)
	{
		if(ht->table[i] != NULL)
		{
			new_huff_tree = create_huffman_tree(ht->table[i]->frequency,ht->table[i]->key,NULL,NULL,NULL);
			tree = enqueue_node(tree,new_huff_tree);
		}
	}

	print_queue(tree);
}
