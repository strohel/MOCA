/* jednorozmerna nezaporna funkce: R -> R+ */
struct funkce_1d {
	double (*f)(double);  /* fce vracejici hodnotu */
	double a, b;  /* interval, na kterem chceme pocitat funkci (integral) */
	double If;  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */

	double c;  /* funkce je na [a; b] omezena touto konstantou */

	double (*g)(double);  /* na [a; b] plati: f(x) >= g(x) */
	double Ig;  /* integral z funkce g na intervalu [a; b] */

	double *mezibody;  /* mnozina bodu a_1 .. a_m kde a < a_1 < ... < a_m = b  (!!! ano, a_m = b) */
	unsigned int poc_mezibodu;  /* pocet mezibodu (pocet intervalu je pak take m) */

	char *jmeno;  /* symbolicky zapis funkce f */
	char *jmeno_g;  /* symbolicky zapis funkce g */
};

/* n-rozmerna nezaporna funkce: R^dim -> R+ */
struct funkce_nd {
	unsigned int dim;  /* dimenze definicniho oboru */
	double (*f)(double *);  /* fce vracejici hodnotu, rozmer pole musi byt presne dim */
	double *a, *b;  /* vicerozmerny interval, na kterem pocitame (integral) */
	double If;  /* skutecna (presna) hodnota integralu z f predpocitana analyticky */
	double c;  /* konstanta omezujici f na [a; b] */
	char *jmeno;  /* pro hezci vypisy */
};

/* rekneme C-cku, ze tyto promenne jsou nadefinovany, ovsem fyzicky jsou ulozeny v jine kompilacni
 * jednotce */
extern struct funkce_1d func_1, func_2;
extern struct funkce_nd func_nd;
