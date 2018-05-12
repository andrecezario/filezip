#ifndef DESCOMPRESS_H_INCLUDED
#define DESCOMPRESS_H_INCLUDED

//Funcao que recebe o arquivo comprimido e retorna a arvore de huffman do mesmo.
huffman_tree* search_tree(FILE *file, huffman_tree *huff_tree);

/*
*Funcao que recebe o arquivo comprimido, o arquivo a ser descomprimido, a arvore de huffman para este arquivo,
*o tamanho da arvore e o tamanho do lixo. Escreve os bytes descomprimidos no arquivo final.
*/
void write_descompress(FILE *file, FILE *file_descompress,
		huffman_tree *huff_tree, int size_tree, int trash);

//Funcao para realizar processos de descompressao de um arquivo
void descompress();

#endif // DESCOMPRESS_H_INCLUDED
