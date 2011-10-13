#include "integraly.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FUNKCI_1D 2
#define POCTU_KROKU 5


/* zadani vstupnich funkci */

double f_1(double x)
{
	return sqrt(x) + 1.0;
}
double g_1(double x)
{
	return x/2.0 + 1.0;
}
struct funkce_1d func_1 = {
	f_1,  /* fce vracejici hodnotu */
	0.0, 4.0,  /* interval, na kterem chceme pocitat funkci (integral) */
	25.0/3.0,  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
	3.0,  /* funkce je na [a; b] omezena touto konstantou */
	g_1,  /* na [a; b] plati: f(x) >= g(x) */
	8.0,  /* integral z funkce g na intervalu [a; b] */
	"sqrt(x) + 1",  /* symbolicky zapis funkce f */
	"x/2 + 1",  /* symbolicky zapis funkce g */
};

double f_2(double x)
{
	return exp(x) + 2*x;
}
double g_2(double x)
{
	return 0;
}
struct funkce_1d func_2 = {
	f_2,  /* fce vracejici hodnotu */
	0.0, 1.0,  /* interval, na kterem chceme pocitat funkci (integral) */
	M_E,  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
	5.0,  /* funkce je na [a; b] omezena touto konstantou */
	g_2,  /* na [a; b] plati: f(x) >= g(x) */
	0.0,  /* integral z funkce g na intervalu [a; b] */
	"e^x + 2x",  /* symbolicky zapis funkce f */
	"???",  /* symbolicky zapis funkce g */
};


int main()
{
	struct funkce_1d *testovane_1d_funkce[FUNKCI_1D] = {&func_1, &func_2};
	struct funkce_1d *func;
	struct vysledek result;
	unsigned int pocty_kroku[POCTU_KROKU] = {10, 100, 10000, 1000000, 10000000}; /* tyto pocty kroku budeme zkouset */
	unsigned int i, j, N;
	char *nazev_funkce, *nazev_metody;

	srand(42);  /* nastavime nahodny generator na stejny seed pro reproducibilitu vysledku */
	fprintf(stdout, "funkce\t\"pocet kroku\"\tmetoda\tintegral\todchylka\n");

	/* vsechny funkce, pocty kroku a metody pro jednorozmerne funkce */
	for(i = 0; i < FUNKCI_1D; i++) {
		func = testovane_1d_funkce[i];
		nazev_funkce = func->jmeno;

		for(j = 0; j < POCTU_KROKU; j++) {
			N = pocty_kroku[j];
			nazev_metody = "vzorkovani plochy";
			result = integral_plocha(func, N);
			fprintf(stdout, "\"%s\"\t%i\t\"%s\"\t%f\t%f\n", nazev_funkce, N, nazev_metody, result.I, result.s);
		}
	}

	return 0;
}
