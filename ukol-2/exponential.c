#include "exponential.h"

#include "uniform.h"

#include <math.h>
#include <stdlib.h>

/**
 * Funkce inverzni ke kumulativni distribucni funkci exponencialniho rozdeleni.
 */
static inline double exp_cdf_inv(double y, double lambda)
{
	return -log(1 - y)/lambda;
}

double *inverse_exponential(double lambda, unsigned int N)
{
	double *ret = malloc(N * sizeof(double));
	unsigned int i;
	double random;

	/* jdeme vygenerovat N hodnot */
	for(i = 0; i < N; i++) {
		random = uniform_01();
		ret[i] = exp_cdf_inv(random, lambda);
	}
	return ret;
}

static inline double uniform(double a, double b)
{
	return a + uniform_01() * (b - a);
}

/**
 * Hustota pravdepodobnosti exponencialniho rozdeleni
 */
static inline double exp_pdf(double x, double lambda)
{
	return lambda * exp(-lambda * x);
}

double *rejection_exponential(double lambda, unsigned int N)
{
	double *ret = malloc(N * sizeof(double));
	unsigned int i;
	double left, right, x, u;  /* exponencialu budeme schovavat do obdelniku (right - left) x c */

	left = 0.0;  /* exponenciala zacina na nule */
	right = 5.0/lambda;  /* zanedbatelne mnozstvi prvku je za touto hranici */
	/* hustota pravdepodobnosti je omezena shora lambdou */

	/* jdeme vygenerovat N hodnot */
	for(i = 0; i < N; i++) {
		while(1) {
			x = uniform(left, right);
			u = uniform(0.0, lambda);
			if(u < exp_pdf(x, lambda)) {
				ret[i] = x;
				break;
			}
		}
	}
	return ret;
}

exponential_generator exponential_generators[] = {
	{inverse_exponential, "inverzni"},
	{rejection_exponential, "zamitaci"},
};
unsigned int exponential_generators_count = sizeof(exponential_generators)/sizeof(exponential_generator);
