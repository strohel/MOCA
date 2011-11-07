#ifndef BINOMIAL_H
#define BINOMIAL_H

typedef struct {
	/**
	 * Generator binomickeho rozdeleni s parametry p, n.
	 *
	 * @param N tolik hodnot se ma vygenerovat
	 * @return adresa pole N integeru. Je nalokovana pomoci malloc, volajici musi uvolnit
	 *         pamet pomoci free()
	 */
	unsigned int *(*generator)(double p, unsigned int n, unsigned int N);
	char *name;  /// nazev tohoto generatoru
} binomial_generator;

extern binomial_generator binomial_generators[];
extern unsigned int binomial_generators_count;

#endif
