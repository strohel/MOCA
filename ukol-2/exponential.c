#include "exponential.h"

#include "uniform.h"

#include <math.h>
#include <stdlib.h>

/**
 * Funkce inverzni ke kumulativni distribucni funkci exponencialniho rozdeleni.
 */
static inline double exp_inv(double y, double lambda)
{
	return -log(1 - y)/lambda;
}

double *inverse_exponential(double lambda, unsigned int N)
{
	double* ret = malloc(N * sizeof(double));
	unsigned int i;
	double random;

	// jdeme vygenerovat N hodnot
	for(i = 0; i < N; i++) {
		random = uniform_01();
		ret[i] = exp_inv(random, lambda);
	}
	return ret;
}

exponential_generator exponential_generators[] = {
	{inverse_exponential, "inverzni"},
};
unsigned int exponential_generators_count = sizeof(exponential_generators)/sizeof(exponential_generator);
