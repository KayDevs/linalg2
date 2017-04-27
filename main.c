#include "matrix.h"

int main(int argc, char **argv) {
	init_matrices(3);
	double init[3][3] = {{1,2,3},{4,5,6},{7,8,9}};
	int m = push_matrix(3, 3, init);
	double init2[3][2] = {{1,0}, {0, 1}, {0, 0}};
	int n = push_matrix(2, 3, init2);
	print_matrix(m);
	print_matrix(n);
	int c = multiply_matrices(m, n);
	print_matrix(c);
	int mr = rref(m);
	print_matrix(mr);

	double init3[6][3] = {{1,2,3},{3,4,5},{5,6,7},{7,8,9},{9,10,11},{11,12,13}};
	int d = push_matrix(3, 6, init3);
	int dr = rref(d);
	print_matrix(dr);

	printf("rank of dr: %d\n", rank(dr));

	double system[2][3] = {{1,0,3},{0,1,4}};
	int s = push_matrix(3, 2, system);
	print_matrix(s);
	double solutions[2];
	solve_system(s, solutions);
	if(solutions[0]) {
		printf("x: %g, y: %g\n", solutions[0], solutions[1]);
	}

	clear_matrices();
	return 0;
}
