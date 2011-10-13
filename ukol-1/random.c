#include "random.h"

#include <stdlib.h>


double rovnom_1d(double a, double b)
{
	return a + (b-a) * ((double) rand()) / ((double) RAND_MAX);
}

void rvonom_nd(double* a, double* b, unsigned int dim, double* out)
{
	unsigned int i;
	for(i = 0; i < dim; i++) {
		out[i] = rovnom_1d(a[i], b[i]);
	}
}
