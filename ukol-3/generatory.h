#ifndef GENERATORY_H
#define GENERATORY_H

typedef struct {
	/**
	 * Nastavi seminko generatoru
	 */
	void (*set_seed)(double seed);
	/**
	 * Vrati nahodne cislo v intervalu [0,1)
	 */
	double (*rand)();
	char *name; /// nazev generatoru
} generator;

extern generator generators[];
extern unsigned generator_count;

#endif
