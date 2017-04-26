#include "matrix.h"

static int mat_capacity;
static int mat_index = 0;
static double ***matrix;
static size_t *width;
static size_t *height;

void init_matrices(size_t capacity) {
	mat_capacity = capacity;
	matrix = calloc(mat_capacity, sizeof(double**));
	width = calloc(mat_capacity, sizeof(size_t));
	height = calloc(mat_capacity, sizeof(size_t));
}

int push_matrix(size_t w, size_t h, double mat[h][w]) {
	width[mat_index] = w;
	height[mat_index] = h;
	matrix[mat_index] = calloc(h, sizeof(double*));
	int i, j;
	for(i = 0; i < h; ++i) {
		matrix[mat_index][i] = calloc(w, sizeof(double));	
		for(j = 0; j < w; ++j) {
			matrix[mat_index][i][j] = mat[i][j];
		}
	}
	return mat_index++;
}

void print_matrix(int index) {
	int i, j;
	for(i = 0; i < height[index]; ++i) {
		printf("[");
		for(j = 0; j < width[index]; ++j) {
			printf("%2.2g, ", matrix[index][i][j]);	
		}
		printf("\b\b]\n");
	}
}

void clear_matrices() {
	int k, i, j;
	for(k = 0; k < mat_capacity; ++k) {
		for(i = 0; i < height[k]; ++i) {
			free(matrix[k][i]);		
		}
		free(matrix[k]);
	}
	free(matrix);
}


//FUNCTIONALITY
int multiply_matrices(int a, int b) {
	if(width[a] != height[b]) {
		printf("matrices cannot be multiplied.\n");
		return -1;
	}
	double ab[height[a]][width[b]];
	int i, j, k;
	for(i = 0; i < height[a]; ++i) {
		for(j = 0; j < width[b]; ++j) {	
			double dot = 0;
			//width[a] = height[b] 
			for(k = 0; k < width[a]; ++k) {
				dot += matrix[a][i][k] * matrix[b][k][j];
			}
			ab[i][j] = dot;
		}
	}
	return push_matrix(width[b], height[a], ab);
}
