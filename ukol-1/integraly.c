#include "integraly.h"
#include "random.h"

#include <math.h>
#include <stdio.h>


struct vysledek integral_plocha(struct funkce_1d* func, unsigned int N)
{
	unsigned int i, hits = 0;
	double x, y_sample, s_squared;
	struct vysledek ret;

	for(i = 0; i < N; i++) {
		x = rovnom_1d(func->a, func->b);  /* nahodna hodnota z definicniho oboru */
		y_sample = rovnom_1d(0, func->c);  /* nahodna hodnota z oboru hodnot */
		if(y_sample <= func->f(x))
			hits++;
	}

	ret.I = func->c*(func->b - func->a)/((double) N) * ((double) hits);
	/* v nasledujicim vyuzivame toho, ze ksi_i^2 == ksi_i, nemusime si tudiz delat sumu druhych mocnin */
	s_squared = 1.0/(N - 1.0)*(hits - pow(hits, 2)/N);  /* vyberovy rozptyl thety (promenne hits) */
	ret.s = func->c*(func->b - func->a)*sqrt(s_squared/N);
	return ret;
}
