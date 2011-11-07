#ifndef EXPONENTIAL_H
#define EXPONENTIAL_H

typedef struct {
	/**
	 * Generator exponencialnoho rozdeleni s parametrem lambda
	 *
	 * @param N tolik hodnot se ma vygenerovat
	 * @return adresa pole N doublu. Je nalokovana pomoci malloc, volajici musi uvolnit
	 *         pamet pomoci free()
	 */
	double *(*generator)(double lambda, unsigned int N);
	char *name;  /// nazev tohoto generatoru
} exponential_generator;

extern exponential_generator exponential_generators[];
extern unsigned int exponential_generators_count;

#endif
