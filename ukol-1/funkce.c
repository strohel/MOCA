#include "funkce.h"

#include <math.h>

/* zde jsou naimplementovany matematicke funkce, na kterych testujeme integracni metody */


double f_1(double x)
{
	return sqrt(x) + 1.0;
}
double g_1(double x)
{
	return x/2.0 + 1.0;
}
/* vlevo funkce roste rychleji, chceme tam hustsi vzorkovani
 * intervaly jsou zvoleny tak, ze na nich funkce ma vzdy stejny prirustek */
double f_1_mezibody[] = {0.16, 0.64, 1.44, 2.56, 4.0};
struct funkce_1d func_1 = {
	f_1,  /* fce vracejici hodnotu */
	0.0, 4.0,  /* interval, na kterem chceme pocitat funkci (integral) */
	28.0/3.0,  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
	3.0,  /* funkce je na [a; b] omezena touto konstantou */
	g_1,  /* na [a; b] plati: f(x) >= g(x) */
	8.0,  /* integral z funkce g na intervalu [a; b] */
	f_1_mezibody,  /* mnozina bodu a_1 .. a_m kde a < a_1 < ... < a_m = b  (!!! ano, a_m = b) */
	5,  /* pocet mezibodu (pocet intervalu je pak take m) */
	"sqrt(x) + 1",  /* symbolicky zapis funkce f */
	"x/2 + 1",  /* symbolicky zapis funkce g */
};

double f_2(double x)
{
	return exp(x) + 2*x;
}
double g_2(double x)
{
	return 3.65*x + 0.8;
}
/* tato fce roste o trochju vice na pravem kraji intervalu - mejme tam kratsi intervaly aby bylo
 * vzorkovani presnejsi */
double f_2_mezibody[] = {12.0/50, 23.0/50, 33.0/50, 42.0/50, 50.0/50};
struct funkce_1d func_2 = {
	f_2,  /* fce vracejici hodnotu */
	0.0, 1.0,  /* interval, na kterem chceme pocitat funkci (integral) */
	M_E,  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
	5.0,  /* funkce je na [a; b] omezena touto konstantou */
	g_2,  /* na [a; b] plati: f(x) >= g(x) */
	21.0/8.0,  /* integral z funkce g na intervalu [a; b] */
	f_2_mezibody,  /* mnozina bodu a_1 .. a_m kde a < a_1 < ... < a_m = b  (!!! ano, a_m = b) */
	5,  /* pocet mezibodu (pocet intervalu je pak take m) */
	"e^x + 2x",  /* symbolicky zapis funkce f */
	"3.65x + 0.8",  /* symbolicky zapis funkce g */
};


double f_3(double *x)
{
	return 8.0 - x[0]*x[0] - x[1]*x[1];
}
double f_3_a[] = {0.0, 0.0};
double f_3_b[] = {0.25, 0.25};
struct funkce_nd func_nd = {
	2,  /* dimenze definicniho oboru */
	f_3,  /* fce vracejici hodnotu, rozmer pole musi byt presne dim */
	f_3_a, f_3_b,  /* vicerozmerny interval, na kterem pocitame (integral) */
	191.0/384,     /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
	8.0,  /* konstanta omezujici f na [a; b] */
	"8 - x^2 - y^2",  /* jmeno pro hezci vypisy */
};
