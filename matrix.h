#ifndef MATRIX_H
#define MATRIX_H
#include <stdio.h>
#include <stdlib.h>

void init_matrices(size_t capacity);
int push_matrix(size_t w, size_t h, double mat[h][w]);
double **get_matrix(int index);
void print_matrix(int index);
void clear_matrices(void);
int multiply_matrices(int a, int b);

#endif
