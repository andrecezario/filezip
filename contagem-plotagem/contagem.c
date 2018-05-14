#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//LIST
typedef struct node
{
  int data;
  struct node *next;
} node;

node* createList()
{
  return NULL;
}

node* insertInList(node *head, int data)
{
	node *link = (node*) malloc(sizeof(node));

	if (head == NULL) {
		link->data = data;
		link->next = head;

		return link;
	}

	else {
		node *tail = head;

		while (tail->next != NULL) {
			tail = tail->next;
		}

		link->data = data;
		tail->next = link;
		link->next = NULL;

		return head;
	}
}

int searchInList(node *head, int element)
{
   node* current = head;
   int comparisons = 0;

   while(current != NULL)
   {
   		comparisons++;
   		if(current->data == element)
   		{
        	return comparisons;
      	}
      	else
      	{
        	current = current->next;
      	}
   }

   return -1;
}

void freeList(node* head)
{
	node* tmp;
	while (head != NULL)
	{
    	tmp = head;
    	head = head->next;
    	free(tmp);
    	tmp = NULL;
  	}
}

//BINARY TREE
typedef struct binary_tree
{
	int item;
	struct binary_tree *right;
	struct binary_tree *left;
}binary_tree;

binary_tree* createBinarySearchTree()
{
	return NULL;
}

void printPreOrder(binary_tree *bt)
{
	if(bt != NULL)
	{
		printf("%d ", bt->item);
		printPreOrder(bt->left);
		printPreOrder(bt->right);
	}
}

binary_tree* insertInBinarySearchTree(binary_tree *bt, int item)
{
	binary_tree *new_node = (binary_tree*)malloc(sizeof(binary_tree));
	binary_tree *current;
	binary_tree *parent = NULL;

	new_node->item = item;
	new_node->left = NULL;
	new_node->right = NULL;

	if(bt == NULL)
	{
		bt = new_node;
		return bt;
	}
	else
	{
		current = bt;
		while(1)
		{
			parent = current;

			if(item < parent->item)
			{
				current = current->left;
				if(current == NULL)
				{
					parent->left = new_node;
					return bt;
				}
			}
			else
			{
				current = current->right;
				if(current == NULL)
				{
					parent->right = new_node;
					return bt;
				}
			}
		}
	}
}

int searchInBinarySearchTree(binary_tree *bt, int element, int comparisons)
{
	comparisons++;

	if(bt == NULL || bt->item == element)
	{
		return comparisons;
	}
	else if(bt->item > element)
	{
		return searchInBinarySearchTree(bt->left, element, comparisons);
	}
	else
	{
		return searchInBinarySearchTree(bt->right, element, comparisons);
	}
}

void freeBinarySearchTree(binary_tree* bt)
{
	if(bt == NULL)
	{
		return;
	}
	else
	{
		freeBinarySearchTree(bt->left);
		freeBinarySearchTree(bt->right);
		free(bt);
	}
}

int main()
{
	int n, i;
	binary_tree *bt = createBinarySearchTree();
	node *list1 = createList();
	FILE *f_list = fopen("comparisons_list.txt", "w");
	FILE *f_bst = fopen("comparisons_bst.txt", "w");

	printf("Number of elements to be added: ");
	scanf("%d", &n);

	int numbers[n];		// cria um array para armazenar os números que serão adicionados

	for(i = 0; i < n; i++)
	{
		numbers[i] = rand();	//sorteia um número para ser adicionado
		list1 = insertInList(list1, numbers[i]);
		bt = insertInBinarySearchTree(bt, numbers[i]);
	}

	int m, aux;
	int comparisons_bst, comparisons_list;

	printf("Number of elements to be sorted: ");
	scanf("%d", &m);

	for(i = 0; i < m; i++)
	{
		do{
			aux = rand()%n;		// sorteia um índice do array
		}while(numbers[aux] == -1);		// verifica se o número já havia sido sorteado
		comparisons_list = searchInList(list1, numbers[aux]);
		comparisons_bst = searchInBinarySearchTree(bt, numbers[aux], 0);

		//fprintf(f_list, "%s %d %s %d\n", "Sorted Index:", aux+1, "Comparisons:", comparisons_list);
		//fprintf(f_bst, "%s %d %s %d\n", "Sorted Index:", aux+1, "Comparisons:", comparisons_bst);
		fprintf(f_list, "%d %d\n",aux+1, comparisons_list);
		fprintf(f_bst, "%d %d\n",aux+1, comparisons_bst);
		numbers[aux] = -1;
	}

	fclose(f_list);
	fclose(f_bst);
	freeList(list1);
	freeBinarySearchTree(bt);

	return 0;
}
