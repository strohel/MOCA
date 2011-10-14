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
		if(y_sample <= func->f(x))  /* pricteme jednicku, pokud vzorek padne pod graf funkce */
			hits++;
	}

	/* I = c*(b - a)/N * sum(ksi_i) */
	ret.I = func->c*(func->b - func->a)/((double) N) * ((double) hits);
	/* v nasledujicim vyuzivame toho, ze ksi_i^2 == ksi_i, nemusime si tudiz delat sumu druhych mocnin */
	s_squared = 1.0/(N - 1.0)*(hits - pow(hits, 2)/N);  /* vyberovy rozptyl thety (promenne hits) */
	ret.s = func->c*(func->b - func->a)*sqrt(s_squared/N);
	return ret;
}

struct vysledek integral_funkce(struct funkce_1d* func, unsigned int N)
{
	double sum = 0, sum_sq = 0, x, f_x;
	unsigned int i;
	struct vysledek ret;

	for(i = 0; i < N; i++) {
		x = rovnom_1d(func->a, func->b);  /* nahodna hodnota z definicniho oboru */
		f_x = func->f(x);  /* hodnota fce v bode */
		sum += f_x;  /* pricteme do sumy prvnich mocnin */
		sum_sq += f_x*f_x;  /* pricteme do sumy druhych mocnin */
	}

	/* I = (b-a)/N * sum(ksi); */
	ret.I = (func->b - func->a)/((double) N)*sum;
	/* odchylka = (b-a)*sqrt((E[ksi^2] - (E[ksi])^2) / N) */
	ret.s = (func->b - func->a)*sqrt(sum_sq/(((double) N) - pow(sum/((double) N), 2))/((double) N));
	return ret;
}

struct vysledek integral_funkce_vyjm(struct funkce_1d* func, unsigned int N)
{
	double sum = 0, sum_sq = 0, x, delta_x;
	unsigned int i;
	struct vysledek ret;

	for(i = 0; i < N; i++) {
		x = rovnom_1d(func->a, func->b);  /* nahodna hodnota z definicniho oboru */
		delta_x = func->f(x) - func->g(x);  /* hodnota fce v bode */
		sum += delta_x;  /* pricteme do sumy prvnich mocnin */
		sum_sq += delta_x*delta_x;  /* pricteme do sumy druhych mocnin */
	}

	/* I = (b-a)/N * sum(delta_i)  + Ig */
	ret.I = (func->b - func->a)/((double) N)*sum + func->Ig;
	/* odchylka = (b-a)*sqrt((E[delta^2] - (E[delta])^2) / N) */
	ret.s = (func->b - func->a)*sqrt(sum_sq/(((double) N) - pow(sum/((double) N), 2))/((double) N));
	return ret;
}
