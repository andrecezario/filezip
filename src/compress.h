#ifndef COMPRESS_H_INCLUDED
#define COMPRESS_H_INCLUDED

/*
*Funcao sem retorno para calcular a frequencia de cada byte no arquivo. Recebe um ponteiro para o arquivo original e outro para um array de *inteiros, correspondente ao numero de ocorrencias de bytes no arquivo.
*/
void frequency_bytes(FILE *file, int *frequency);

//Funcao que converte uma fila em uma arvore de huffuman, retornando a raiz dessa arvore.
huffman_tree* convert_queue_to_tree(huffman_tree* huff_tree);

//Funcao que recebe uma arvore e adiciona numa hash a codificacao de huffman para cada byte do arquivo.
void build_code_table(hash_table *ht, huffman_tree *huff_tree, char binary[],
		int size);

//Funcao que recebe um byte e a posição onde será alocado o bit, retornando o byte com o novo bit setado.
char set_bit(unsigned char byte, int i);

/*
*Funcao que recebe o arquivo original, o arquivo a ser comprimido, a hash com a codificacao dos bytes
*e o tamanho da arvore. Escreve os bytes comprimidos no arquivo final.
*/
void write_code_compress(FILE *file, FILE *file_compress, hash_table *ht,
		int size_tree);

/*
*Funcao que recebe o arquivo a ser comprimido, a arvore o tamanho da mesma. Escrevendo a arvore em pre-ordem no arquivo final
*/
void write_tree_pre_order(FILE *file, huffman_tree *huff_tree, int *size_tree);

//Funcao para realizar processos de compressao de um arquivo
void compress();

#endif // COMPRESS_H_INCLUDED
