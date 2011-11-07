#include "binomial.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Vrati binomicky koeficient, tedy "n nad k".
 *
 * Tato funkce pocita rekurzivne. :-)
 */
static unsigned int binom_coef(unsigned int n, unsigned int k) {
	if(k == 0)
		return 1;
	if(n == 0)
		return 0;
	return binom_coef(n-1, k-1) + binom_coef(n-1, k);
}

/**
 * Vrati hranicni body intervalu, na kterych se lame hodnota inverzni kumulativni
 * distribucni funkce binomickeho rozdeleni s paramtery p, n. Napr pro p = 1/2 a n = 2
 * vrati {1/4, 3/4, 1}
 *
 * @return adresa pole n+1 intu, prvni _neni_ 0, a posledni _je_ 1; nutno free()-ovat.
 */
static double *binomial_intervals(double p, unsigned int n)
{
	double *ret = malloc((n + 1) * sizeof(double));
	unsigned int k;
	double sum = 0;
	for(k = 0; k < n + 1; k++) {
		sum += binom_coef(n, k) * pow(p, k) * pow(1-p, n-k);
		ret[k] = sum;
	}
	return ret;
}

unsigned int *basic_binomial(double p, unsigned int n, unsigned int N)
{
	unsigned int *ret = malloc(N * sizeof(unsigned int));
	double *intervals = binomial_intervals(p, n);
	unsigned int i;

	fprintf(stderr, "intervals = {");
	for(i = 0; i < n + 1; i++) {
		fprintf(stderr, "%f ", intervals[i]);
	}
	fprintf(stderr, "}\n");

	free(intervals);
	return ret;
}

binomial_generator binomial_generators[] = {
	{basic_binomial, "zakladni"}
};
unsigned int binomial_generators_count = sizeof(binomial_generators)/sizeof(binomial_generator);
