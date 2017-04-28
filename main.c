#include "matrix.h"

int main(int argc, char **argv) {
	init_matrices(3);
	double init[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	int m = push_matrix(3, 3, init);
	print_matrix(m);
	int mr = rref(m);
	print_matrix(mr);

	double init2[3][2] = {{1,0}, {0, 1}, {0, 0}};
	int n = push_matrix(2, 3, init2);
	print_matrix(n);
	int c = multiply_matrices(m, n);
	print_matrix(c);

	double system[3][4] = {{1,4,2,3},{0,1,6,4},{0,0,2,5}};
	int s = push_matrix(4, 3, system);
	print_matrix(s);
	print_matrix(rref(s));
	double solutions[3];
	solve_system(s, solutions);
	if(solutions[0]) {
		printf("x: %g, y: %g, z: %g\n", solutions[0], solutions[1], solutions[2]);
	}

	clear_matrices();
	return 0;
}
