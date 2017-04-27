#include "matrix.h"

static int mat_capacity = 512;
static int mat_index = 0;
static double ***matrix;
static size_t *width;
static size_t *height;

void init_matrices(size_t capacity) {
	if(capacity > mat_capacity)
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
	if(++mat_index > mat_capacity) {
		matrix = realloc(matrix, (mat_capacity *= 2) * sizeof(double**));
	}
	return mat_index - 1;
}

double **get_matrix(int index) {
	return matrix[index];
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
	printf("\n");
}

void clear_matrices() {
	int k, i;
	for(k = 0; k < mat_capacity; ++k) {
		for(i = 0; i < height[k]; ++i) {
			free(matrix[k][i]);		
		}
		free(matrix[k]);
	}
	free(matrix);
	free(width);
	free(height);
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

//TODO: adding, subtracting, inverse, rref, rank, gauss-jordan, solving systems, Row, Column, Null, LeftNull, SVD
int add_matrices(int a, int b) {
	if(width[a] != width[b] || height[a] != height[b]) {
		printf("matrices cannot be added.\n");
		return -1;
	}
	double ab[height[a]][width[a]];
	int i, j;
	for(i = 0; i < height[a]; ++i) {
		for(j = 0; j < width[a]; ++j) {
			ab[i][j] = matrix[a][i][j] + matrix[b][i][j];
		}
	}
	return push_matrix(width[a], height[a], ab);
}

int subtract_matrices(int a, int b) {
	if(width[a] != width[b] || height[a] != height[b]) {
		printf("matrices cannot be subtracted.\n");
		return -1;
	}
	double ab[height[a]][width[a]];
	int i, j;
	for(i = 0; i < height[a]; ++i) {
		for(j = 0; j < width[a]; ++j) {
			ab[i][j] = matrix[a][i][j] - matrix[b][i][j];
		}
	}
	return push_matrix(width[a], height[a], ab);
}

int multiply_scalar(double k, int a) {
	double ka[height[a]][width[a]];	
	int i, j;
	for(i = 0; i < height[a]; ++i) {
		for(j = 0; j < width[a]; ++j) {
			ka[i][j] = k * matrix[a][i][j];
		}
	}
	return push_matrix(width[a], height[a], ka);
}

int rref(int a) {
	int i, j;	
	double arref[height[a]][width[a]];
	for(i = 0; i < height[a]; ++i) {
		memcpy(arref[i], matrix[a][i], sizeof(arref[i]));
	}

	//to speed up computation and save stack space we're gonna use a fucktonne of memcpy's for swapping, etc.
	//instead of multiplying by elementary matrices, etc.

	for(i = 0, j = 0; i < height[a]-1 && j < width[a]-1; ++i, ++j) {
		//terminology: i = current row, j = LEADING column

		//if leading coefficient is zero, then swap with a nonzero row
		if(arref[i][j] == 0) {
			int k;
			for(k = i; k < height[a]; ++k) {
				//if we found a row with nonzero leading coefficient, then this is the row we want to swap with.
				if(arref[k][j] != 0)
					break;
			}
			if(arref[k][j] != 0) {
				//swap row[i] with row[k]
				//NOTE: do not use matrix[a]! use arref! must be nondestructive!
				//NOTE: this could also be done with multiplication via elementary matrices but uh ... no.
				double rowi[width[a]];
				memcpy(rowi, arref[i], sizeof(rowi));
				memcpy(arref[i], arref[k], sizeof(arref[i]));
				memcpy(arref[k], rowi, sizeof(arref[k]));
			} else {
				//special case: if the entire column is zeroes, then increase the column
				j++;
			}
		}

		//divide entire row by leading coefficient, to get REDUCED row echelon
		int k;
		//could be k = j, but 0/n = 0 so
		double d = arref[i][j];
		for(k = 0; k < width[a]; ++k) {
			arref[i][k] /= d;
		}

		//eliminate other elements of jth column via subtracting multiples of the current row from other rows :I
		for(k = 0; k < height[a]; ++k) {
			if(k == i) {
				//skip the current row
				continue;
			}
			double c = arref[k][j];
			int l;
			for(l = j; l < width[a]; ++l) {
				arref[k][l] -= arref[i][l] * c;
			}
		}
	}

	return push_matrix(width[a], height[a], arref);
}

int rank(int a) {
	//rank = number of nonzero rows in rref form
	int rank = 0;
	int ar = rref(a);
	int i, j;
	for(i = 0; i < height[ar]; ++i) {
		for(j = 0; j < width[ar]; ++j) {
			if(matrix[ar][i][j] != 0) {
				rank++;
				break;
			}
		}
	}
	return rank;
}

//returns unique solution set, if it has one
//otherwise sets solutions[0] to 0
//(I would memset the whole array to 0 but there's the chance
//that the programmer didn't allocate enough space)
void solve_system(int ab, double solutions[]) {
	int abr = rref(ab);
	//int n = rank(ab);
	int i, j;
	//check number of solutions:
	//if it has an inconsistent row, then it's got no solution:
	for(i = 0; i < height[abr]; ++i) {
		if(matrix[abr][i][width[abr]] != 0) {
			for(j = 0; j < width[abr] - 1; ++j) {
				//if we have a nonzero entry, then this row is safe
				if(matrix[abr][i][j] != 0) {
					break;
				}
			}
			if(j == width[abr] - 1) {
				printf("Matrix inconsistent.\n");
				//memset(solutions, 0, n * sizeof(double));
				solutions[0] = 0;
				return;
			}
		}
	}
	//if it's got the same number of pivots (rank) as nonzero rows, 
	//then it's got a single solution and we can continue. 
	//otherwise, it's got infinite solutions.
	//(this function won't do complete solutions for now)
	if(rank(abr) != width[abr] - 1) {
		printf("More rows than variables, infinite solutions.\n");
		//memset(solutions, 0, n * sizeof(double));
		solutions[0] = 0;
		return;
	}
	//go upwards and rightwards through the matrix.
	for(i = height[abr] - 1; i >= 0; --i) {
		//keep track of which column is the pivot column for this row
		int pivot = 0;
		for(j = 0; j < width[abr] - 1; ++j) {
			//keep going until we hit a nonzero 1: that's the pivot
			if(matrix[abr][i][j] != 0) {
				pivot = j;
				break;
			}
		}
		//if the entire row was zeroes, then move a row up
		if(j == width[abr] - 1) {
			break;
		}
		for(j = pivot; j < width[abr] - 1; ++j) {
			//if we encounter a nonzero number,
			//then subtract from the solution set that number * solutions[j]
			if(matrix[abr][i][j] != 0) {
				matrix[abr][i][width[abr] - 1] -= matrix[abr][i][j] * solutions[j];
			}
		}
		//the result is simply the solution set after this operation
		printf("Solution number %d: %g\n", i, matrix[abr][i][width[abr] - 1]);
		solutions[i] = matrix[abr][i][width[abr] - 1];
	}
}

int determinant(int a) {
	return -1;	
}
