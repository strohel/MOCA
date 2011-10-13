#include "funkce.h"

struct vysledek {
	double I;  /* hodnota integralu */
	double s;  /* odhad smerodatne odchylky (odmocniny z variance) */
};

/* 1. vzorkovani plochy */
struct vysledek integral_plocha(struct funkce_1d *func, unsigned int N);

/* 2. vzorkovani funkce */
struct vysledek integral_funkce(struct funkce_1d *func, unsigned int N);

/* 3. vzorkovani funkce vyjmutim hlavni casti; f >= g >= 0 */
struct vysledek integral_funkce_vyjm(struct funkce_1d *func, unsigned int N);

/* 4. vzorkovani funkce se skupinovym vyberem */
struct vysledek integral_funkce_skup(struct funkce_1d *func, unsigned int *N, unsigned int m);

/* 5. vicerozmerne vzorkovani plochy */
struct vysledek integral_funkce_dim(struct funkce_nd *func, unsigned int N);
