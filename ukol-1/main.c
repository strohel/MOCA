#include "integraly.h"

#include <stdio.h>
#include <stdlib.h>

#define FUNKCI_1D 2
#define POCTU_KROKU 5
#define METOD 4


int main()
{
	/* tyto funkce jsou nadefinovany v souboru funkce.c */
	struct funkce_1d *testovane_1d_funkce[FUNKCI_1D] = {&func_1, &func_2};
	unsigned int pocty_kroku[POCTU_KROKU] = {10, 100, 10000, 1000000, 10000000}; /* tyto pocty kroku budeme zkouset */

	/* nasledujici zruda je array funkci (metod), ktere pocitaji integraly */
	struct vysledek (*metody[])(struct funkce_1d *, unsigned int) =
		{integral_plocha, integral_funkce, integral_funkce_skup, integral_funkce_vyjm};
	char *nazvy_metod[METOD] =
		{"plocha", "funkce", "skup._vyb.", "vyjmuti_casti"};

	struct funkce_1d *func;
	struct vysledek result;
	unsigned int i, j, k, N;
	char *nazev_funkce, *nazev_metody;

	/* vsechny funkce, pocty kroku a metody pro jednorozmerne funkce */
	for(i = 0; i < FUNKCI_1D; i++) {
		func = testovane_1d_funkce[i];
		nazev_funkce = func->jmeno;
		fprintf(stderr, "Jdeme integrovat funkci f(x) = %s na intervalu [%f; %f]\n", nazev_funkce, func->a, func->b);
		fprintf(stderr, "Na intervalu by melo platit: a) f(x) <= %f  b) g(x) = %s <= f(x)  c) I(g(x)) = %f\n", func->c, func->jmeno_g, func->Ig);
		fprintf(stderr, "Analyticky spocitana hodnota integralu = %f\n", func->If);
		fprintf(stderr, "Hranicni body pro skupinovy vyber: (%f", func->a);
		for(j = 0; j < func->poc_mezibodu; j++) {
			fprintf(stderr, "; %f", func->mezibody[j]);
		}
		fprintf(stderr, ") na kazdy interval je stejny pocet vzorku\n");

		// vypiseme hlavicku
		fprintf(stdout, "         N");
		for(k = 0; k < METOD; k++) {
			nazev_metody = nazvy_metod[k];
			fprintf(stdout, "   %13s     s", nazev_metody);
		}
		fprintf(stdout, "\n");

		for(j = 0; j < POCTU_KROKU; j++) {
			N = pocty_kroku[j];

			fprintf(stdout, "%10i", N);  /* pocet kroku */
			for(k = 0; k < METOD; k++) {
				nazev_metody = nazvy_metod[k];
				srand(42);  /* nastavime nahodny generator na stejny seed pro reproducibilitu vysledku */
				result = metody[k](func, N);
				fprintf(stdout, " %10f %10f", result.I, result.s);
			}
			fprintf(stdout, "\n");
		}
		fprintf(stderr, "\n");
	}

	/* vicerozmerna metoda */
	fprintf(stderr, "Jdeme integrovat funkci f(x, y) = %s na intervalu (%f; %f) az (%f, %f)\n",
	        func_nd.jmeno, func_nd.a[0], func_nd.a[1], func_nd.b[0], func_nd.b[1]);
	fprintf(stderr, "Na intervalu ma platit: 0 <= f(x, y) <= %f\n", func_nd.c);
	fprintf(stderr, "Analyticky spocitana hodnota integralu = %f\n", func_nd.If);
	fprintf(stdout, "         N     plocha          s      delta\n");
	for(j = 0; j < POCTU_KROKU; j++) {
		N = pocty_kroku[j];
		result = integral_plocha_dim(&func_nd, N);
		fprintf(stdout, "%10i %10f %10f\n", N, result.I, result.s);
	}

	return 0;
}
