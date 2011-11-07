#include "binomial.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	(void) argc;
	(void) argv;
	unsigned int i, n = 2, N = 100;
	double p = 1.0/4.0;
	unsigned int *result_discrete;

	fprintf(stderr, "binomicke rozdeleni\n");
	for(i = 0; i < binomial_generators_count; i++) {
		fprintf(stdout, "%s\n", binomial_generators[i].name);
		result_discrete = binomial_generators[i].generator(p, n, N);
		free(result_discrete);
	}

	return 0;
}
