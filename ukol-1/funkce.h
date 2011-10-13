/* jednorozmerna nezaporna funkce: R -> R+ */
struct funkce_1d {
	double (*f)(double);  /* fce vracejici hodnotu */
	double a, b;  /* interval, na kterem chceme pocitat funkci (integral) */
	double If;  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */

	double c;  /* funkce je na [a; b] omezena touto konstantou */

	double (*g)(double);  /* na [a; b] plati: f(x) >= g(x) */
	double Ig;  /* integral z funkce g na intervalu [a; b] */

	char *jmeno;  /* symbolicky zapis funkce f */
	char *jmeno_g;  /* symbolicky zapis funkce g */
};

/* n-rozmerna nezaporna funkce: R^dim -> R+ */
struct funkce_nd {
	unsigned int dim;  /* dimenze definicniho oboru */
	double (*f)(double *);  /* fce vracejici hodnotu, rozmer pole musi byt presne dim */
	double *a, *b;  /* vicerozmerny intergal, na kterem pocitame (integral) */
	double c;  /* konstanta omezujici f na [a; b] */
	char *jmeno;  /* pro hezci vypisy */
};
