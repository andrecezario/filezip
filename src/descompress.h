#ifndef DESCOMPRESS_H_INCLUDED
#define DESCOMPRESS_H_INCLUDED

huffman_tree* search_tree(FILE *file, huffman_tree *huff_tree);

void write_descompress(FILE *file, FILE *file_descompress,
		huffman_tree *huff_tree, int size_tree, int trash);

void descompress();

#endif // DESCOMPRESS_H_INCLUDED