#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "data_structure.h"
#include "compress.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>

FILE* file = NULL;

int inicial()
{
	if (NULL ==( file = fopen("entrada.txt", "rb")))
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
int final()
{
	if (0 != fclose(file))
	{
		return -1;
	}
	else
	{
		file = NULL;
		return 0;
	}
}

void write_tree(huffman_tree *tree, unsigned char *S_tree, int *x)
{
	if (tree != NULL)
	{
		unsigned char *y;
		y = tree->item;
		S_tree[*x] = *y;
		*x += 1;
		write_tree(tree->left, S_tree, x);
		write_tree(tree->right, S_tree, x);
	}
}
int decoding_table(huffman_tree* huff_tree, hash_table* ht, int i)
{
	huffman_tree* new_huffman_tree = huff_tree;
	int x = strlen(ht->table[i]->binary), j;
	for (j = 0; j < x; j++)
	{
		if (ht->table[i]->binary[j]=='0')
		{
			new_huffman_tree = new_huffman_tree->left;
		}
		else
		{
			new_huffman_tree = new_huffman_tree->right;
		}
	}
	return *((int*)new_huffman_tree->item);
}
void test_create_empty_tree(void)
{
	huffman_tree* new_tree = create_empty_huffman_tree();
	CU_ASSERT_PTR_NULL(new_tree);
}
void test_create_huffman_tree(void)
{	
	//ARRANGE
		int x = 5, y = 7, z = 9;
		huffman_tree* node_1 = create_empty_huffman_tree();
		huffman_tree* node_2 = create_empty_huffman_tree();
		huffman_tree* node_3 = create_empty_huffman_tree();
		//huffman_tree* node_4 = create_empty_huffman_tree();
		unsigned char item1 = 'A', item2 = 'B', item3 = 'C';
	//ACT
		node_2 = create_huffman_tree(x, &item2, NULL, NULL, NULL);
		node_3 = create_huffman_tree(y, &item3, NULL, NULL, NULL);
	//ASSERT
		//CU_ASSERT_PTR_NOT_NULL(node_4);
		CU_ASSERT_PTR_NOT_NULL(node_2);
		CU_ASSERT_PTR_NOT_NULL(node_3);
	//NEW ACT
		node_1 = create_huffman_tree(z, &item1, node_2, node_3, NULL);
	//NEW ASSERT
		CU_ASSERT_PTR_NOT_NULL(node_1);
		CU_ASSERT_PTR_EQUAL(node_1->left, node_2);
		CU_ASSERT_PTR_EQUAL(node_1->right, node_3);
		//Frequency
		CU_ASSERT_EQUAL(node_1->frequency, z);
		CU_ASSERT_EQUAL(node_1->left->frequency, x);
		CU_ASSERT_EQUAL(node_1->right->frequency, y);
		//Pointer for item
		CU_ASSERT_PTR_EQUAL(node_1->item, &item1);
		CU_ASSERT_PTR_EQUAL(node_1->left->item, &item2);
		CU_ASSERT_PTR_EQUAL(node_1->right->item, &item3);
		//Value for pointer
		CU_ASSERT_EQUAL(item1, *((unsigned char*)node_1->item));
		CU_ASSERT_EQUAL(item2, *((unsigned char*)node_1->left->item));
		CU_ASSERT_EQUAL(item3, *((unsigned char*)node_1->right->item));
}
void test_frequency_bytes(void)
{
	//ARRANGE
		int i, frequency[MAX_SIZE] = { 0 };
		int a = 'a', b = 'b', c = 'c', d = 'd', e = 'e';
	//ACT
		frequency_bytes(file, frequency);
	//ASSERT
		//Expected result: failed 5 cases 
		for (int i = 0; i < MAX_SIZE; i++)
		{
			CU_ASSERT_EQUAL(frequency[i], 0);
		}
		CU_ASSERT_EQUAL(frequency[a], 5);
		CU_ASSERT_EQUAL(frequency[b], 4);
		CU_ASSERT_EQUAL(frequency[c], 3);
		CU_ASSERT_EQUAL(frequency[d], 2);
		CU_ASSERT_EQUAL(frequency[e], 1);
}
void test_priority_queue_empty_file(void)
{
	//ARRANGE
		int x = 10, i, frequency[MAX_SIZE] = { 0 };
		huffman_tree *huff_tree = create_empty_huffman_tree();
		huffman_tree *new_huff_tree = NULL;
	//ACT
		frequency_bytes(file, frequency);
		for (i = 0; i < MAX_SIZE; i++) {
			unsigned char *item = (unsigned char*) malloc(sizeof(unsigned char));
			*item = i;
			if (frequency[i] > 0) {
				new_huff_tree = create_huffman_tree(frequency[i], item, NULL,
				NULL, NULL);
				huff_tree = enqueue_node(huff_tree, new_huff_tree);
			}
		}
	//ASSERT
		CU_ASSERT_PTR_NULL(new_huff_tree);
		CU_ASSERT_PTR_NULL(huff_tree);
}
void test_priority_queue(void)
{
	//ARRANGE
		fseek(file, 0, SEEK_SET);
		int i, frequency[MAX_SIZE] = { 0 };
		unsigned char* x = NULL, *y = NULL;
		unsigned char element_list[MAX_SIZE], N_E1 = 'R', N_E2 = '@', N_E3 = 'p';
		y = malloc(sizeof(unsigned char));
		huffman_tree *huff_tree = create_empty_huffman_tree();
		huffman_tree *new_huff_tree = NULL, *aux = NULL;

	//ACT
		frequency_bytes(file, frequency);
		for (i = 0; i < MAX_SIZE; i++) {
			unsigned char *item = (unsigned char*) malloc(sizeof(unsigned char));
			*item = i;
			if (frequency[i] > 0) {
				new_huff_tree = create_huffman_tree(frequency[i], item, NULL,
				NULL, NULL);
				huff_tree = enqueue_node(huff_tree, new_huff_tree);
			}
		}
	//ASSERT
		CU_ASSERT_PTR_NOT_NULL(huff_tree);
		if (huff_tree != NULL)
		{
			CU_ASSERT_EQUAL(huff_tree->frequency, 1);
			CU_ASSERT_EQUAL('e', *((unsigned char*)huff_tree->item));
			//NEW ACT
			//inserting new nodes
				huff_tree = enqueue_node(huff_tree, create_huffman_tree(6, &N_E1, NULL, NULL, NULL));
				huff_tree = enqueue_node(huff_tree, create_huffman_tree(3, &N_E2, NULL, NULL, NULL));
				huff_tree = enqueue_node(huff_tree, create_huffman_tree(1, &N_E3, NULL, NULL, NULL));
			//NEW ASSERT
				CU_ASSERT_EQUAL(huff_tree->frequency, 1);
				CU_ASSERT_EQUAL('p', *((unsigned char*)huff_tree->item));
			//NEW ACT
				aux = huff_tree;
				i = 0;
				//Writing a string with the items in the queue
				while(aux!=NULL)
				{
					y = aux->item;
					element_list[i] = *y;
					aux = aux->next;
					i++;
				}
			//ASSERT
			CU_ASSERT_NSTRING_EQUAL(element_list, "ped@cbaR", i);
		}
}
void test_convertion(void)
{
	//ARRANGE
		//Queue generated from file 
		int i, frequency[256] = { 0 }, size = 0;
		huffman_tree *huff_tree = create_empty_huffman_tree();
		huffman_tree *new_huff_tree = NULL;
		fseek(file, 0, SEEK_SET);
		frequency_bytes(file, frequency);
		unsigned char *x;
		unsigned char p_order_tree[511];
		unsigned char test_tree[5];
		int *psize = malloc(sizeof(int));
		*psize = 0;
		for (i = 0; i < MAX_SIZE; i++)
		{	
			unsigned char *item = (unsigned char*) malloc(sizeof(unsigned char));
			*item = i;
			if (frequency[i] > 0) {
				new_huff_tree = create_huffman_tree(frequency[i], item, NULL,
				NULL, NULL);
				huff_tree = enqueue_node(huff_tree, new_huff_tree);
			}
		}
		//Queue generated for testing
		unsigned char element1 = 'A', element2 = 'B', element3 = 'C';
		huffman_tree *tree_to_test = create_huffman_tree(1, &element1, NULL, NULL,
		create_huffman_tree(2, &element2, NULL, NULL, create_huffman_tree(3, &element3, NULL, NULL, NULL)));
	//ACT
		huff_tree = convert_queue_to_tree(huff_tree);
		tree_to_test = convert_queue_to_tree(tree_to_test);
		write_tree(huff_tree, p_order_tree, psize);
		write_tree(tree_to_test, test_tree, &size);
	//ASSERT
		//Tree generated for testing
		CU_ASSERT_PTR_NOT_NULL(tree_to_test);
		CU_ASSERT_EQUAL(6, tree_to_test->frequency);
		CU_ASSERT_EQUAL('*', *((unsigned char*)tree_to_test->item));
		CU_ASSERT_EQUAL(tree_to_test->right->frequency, 3);
		CU_ASSERT_PTR_NULL(tree_to_test->right->right);
		CU_ASSERT_PTR_NULL(tree_to_test->right->left);
		CU_ASSERT_NSTRING_EQUAL("**ABC", test_tree, size);
		//Tree generated from file
		CU_ASSERT_EQUAL(huff_tree->frequency, (huff_tree->left->frequency+huff_tree->right->frequency))
		CU_ASSERT_NSTRING_EQUAL("***edc*ba", p_order_tree, *psize);
}
void test_hash_table(void)
{
	//ARRANGE
	fseek(file, 0, SEEK_SET);
	int i, frequency[256] = { 0 };
	huffman_tree *huff_tree = create_empty_huffman_tree();
	huffman_tree *new_huff_tree = NULL;
	hash_table *ht_code = create_hash_table();
	frequency_bytes(file, frequency);
	//Adicionar itens na fila de prioridade
	for (i = 0; i < MAX_SIZE; i++) 
	{
		unsigned char *item = (unsigned char*) malloc(sizeof(unsigned char));
		*item = i;
		if (frequency[i] > 0)
		{
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
	//ASSERT
	for (i = 0; i < MAX_SIZE; i++)
	{
		if (ht_code->table[i]!=NULL)
		{
			CU_ASSERT_EQUAL(decoding_table(huff_tree, ht_code, i), *((int*)ht_code->table[i]->key));
		}
	}
}
int main(void)
{
	CU_pSuite P_priority_queue = NULL, P_hufffman_tree = NULL, P_hash_table = NULL;
	//inicialização
		if (CUE_SUCCESS != CU_initialize_registry())
		{
			return CU_get_error();
		}
	//Adicionado suite 1
		P_priority_queue = CU_add_suite("Test_Priority_queue", inicial, final);
		if (P_priority_queue==NULL)
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
		if (NULL == CU_add_test(P_priority_queue,"\n\n test_frequency_bytes......\n\n",
			 test_frequency_bytes))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
		if (NULL == CU_add_test(P_priority_queue,"\n\n test_priority_queue_empty_file......\n\n",
			 test_priority_queue_empty_file))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}

		if (NULL == CU_add_test(P_priority_queue,"\n\n test_priority_queue......\n\n",
			 test_priority_queue))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
	//Adicionado suite 2
		P_hufffman_tree = CU_add_suite("test_create_huffman_tree", inicial, final);
		if (P_hufffman_tree == NULL)
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
		if (NULL == CU_add_test(P_hufffman_tree,"\n\n inicialização da arvore......\n\n",
		 test_create_empty_tree))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
		if (NULL == CU_add_test(P_hufffman_tree,"\n\n Criação da arvore......\n\n",
		 test_create_huffman_tree))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
		if (NULL == CU_add_test(P_hufffman_tree,"\n\n Convert queue to tree......\n\n",
		 test_convertion))
		{
			CU_cleanup_registry();
			return CU_get_error();
		}
		P_hash_table = CU_add_suite("Test_hash_table", inicial, final);
		CU_pTest teste2 = CU_add_test(P_hash_table,"\n\n Hash table......\n\n",
		 test_hash_table);

	CU_basic_run_tests();
	//CU_basic_set_mode(CU_BRM_VERBOSE);
	//CU_basic_run_suite(P_priority_queue);
	//CU_console_run_tests();
	//CU_automated_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}
