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

huffman_tree* create_huffman_tree(int frequency, byte item, huffman_tree *left, huffman_tree *right, huffman_tree *next)
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

	else if (head_huff_tree->frequency >= new_huff_tree->frequency)
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

	return head_huff_tree;
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

void print_queue(huffman_tree *tree)
{
	while(tree!=NULL)
	{
		printf("Item: %d (decimal) Frequência: %d\n",tree->item,tree->frequency);
		tree = tree->next;
	}
}
//*************************************HASH-TABLE**********************************
/*typedef struct element {
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
*/
//************************************************COMPRESS***********************************
void frequency_bytes(FILE *file, int *frequency)
{
	int i,size;
	//Tamanho do arquivo
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);

	//Frequencia de cada byte
	for(i=1;i<=size;i++)
	{
		frequency[fgetc(file)]++;
	}
}

huffman_tree* convert_queue_to_tree(huffman_tree* huff_tree)
{
	huffman_tree *left;
	huffman_tree *right;
	huffman_tree *new_node;
	huffman_tree *parent;
	int new_frequency;

	while(huff_tree!=NULL && huff_tree->next!=NULL)
	{
		left = huff_tree;
		right = left->next;
		new_node = right->next;
		new_frequency = left->frequency + right->frequency;

		byte caracter_parent = '*';
		parent = create_huffman_tree(new_frequency,caracter_parent,left,right,NULL);
		left->next = NULL;
		right->next = NULL;

		huff_tree = enqueue_node(new_node, parent);
	}

	return huff_tree;

}

void print_pre_order(huffman_tree *bt)
{
	if(bt!=NULL)
	{
		//printf("%c ",bt->item);
		printf("%d ",bt->frequency);
		print_pre_order(bt->left);
		print_pre_order(bt->right);
	}
}
void compress(){
	char address_file[250];
	int i, frequency[256] = {0};
	long int size_file = 0;
	FILE *file;
	byte item;
	huffman_tree *tree = create_empty_huffman_tree();
	huffman_tree *new_huff_tree = NULL;

	printf("Digite o endereço do arquivo: ");
	scanf("%s",address_file);
	file = fopen(address_file,"rb");

	if(file==NULL) printf("Dados inválidos!");

	else {
		frequency_bytes(file,frequency);

		//Adicionar itens na fila de prioridade
		for(i=0;i<MAX_SIZE;i++)
		{
			item = i;
			if(frequency[i]>0)
			{
				new_huff_tree = create_huffman_tree(frequency[i],item,NULL,NULL,NULL);
				tree = enqueue_node(tree,new_huff_tree);
			}
		}

		//Converter fila em árvore
		tree = convert_queue_to_tree(tree);
		print_pre_order(tree);
		printf("\n");
	}

}

void descompress(){


}

//*******************************************MAIN****************************************
int menu()
{
	int command;
	printf("Escolha uma das opções:\n");
	printf("1 p/ comprimir ou 2 p/ descomprimir\n");
	scanf("%d",&command);

	if(command==1) compress();
	else if(command==2)descompress();
	else printf("Comando inválido!");
}

int main()
{
	int i;
	menu();
}
