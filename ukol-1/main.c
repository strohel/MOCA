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
	28.0/3.0,  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
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
	21.0/8.0,  /* integral z funkce g na intervalu [a; b] */
	"e^x + 2x",  /* symbolicky zapis funkce f */
	"3.65x + 0.8",  /* symbolicky zapis funkce g */
};


int main()
{
	struct funkce_1d *testovane_1d_funkce[FUNKCI_1D] = {&func_1, &func_2};
	struct funkce_1d *func;
	struct vysledek result;
	unsigned int pocty_kroku[POCTU_KROKU] = {10, 100, 10000, 1000000, 10000000}; /* tyto pocty kroku budeme zkouset */
	unsigned int i, j, N;
	char *nazev_funkce, *nazev_metody;
	double delta;

	srand(42);  /* nastavime nahodny generator na stejny seed pro reproducibilitu vysledku */

	/* vsechny funkce, pocty kroku a metody pro jednorozmerne funkce */
	for(i = 0; i < FUNKCI_1D; i++) {
		func = testovane_1d_funkce[i];
		nazev_funkce = func->jmeno;
		fprintf(stderr, "Jdeme integrovat funkci f(x) = %s na intervalu [%f; %f]\n", nazev_funkce, func->a, func->b);
		fprintf(stderr, "Na intervalu by melo platit: a) f(x) <= %f  b) g(x) = %s <= f(x)  c) I(g(x)) = %f\n", func->c, func->jmeno_g, func->Ig);
		fprintf(stderr, "Analyticky spocitana hodnota integralu = %f\n", func->If);

		fprintf(stdout, "\"funkce\"\t\"pocet kroku\"\t\"metoda\"\t\"integral\"\t\"odhad odchylky\"\t\"skutecna odchylka\"\n");
		for(j = 0; j < POCTU_KROKU; j++) {
			N = pocty_kroku[j];
			nazev_metody = "vzorkovani plochy";
			result = integral_plocha(func, N);
			delta = fabs(result.I - func->If);
			fprintf(stdout, "\"%s\"\t%i\t\"%s\"\t%f\t%f\t%f\n", nazev_funkce, N, nazev_metody, result.I, result.s, delta);
		}
		fprintf(stderr, "\n");
	}

	return 0;
}
