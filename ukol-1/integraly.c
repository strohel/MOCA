#include "integraly.h"
#include "random.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


/**
 * Vrati n-dimenzionalni miru n-rozmerneho intervalu. (n == parametr dim)
 * Musi platit: a_i < b_i \forall i
 */
double mira(double *a, double *b, unsigned int dim)
{
	unsigned int i;
	double ret = 1.0;  /* neutralni prvek vuci nasobeni */
	for(i = 0; i < dim; i++) {
		ret *= (b[i] - a[i]);
	}
	return ret;
}

struct vysledek integral_plocha(struct funkce_1d *func, unsigned int N)
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

struct vysledek integral_funkce(struct funkce_1d *func, unsigned int N)
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
	ret.I = (func->b - func->a)/N*sum;
	/* odchylka = (b-a)*sqrt((E[ksi^2] - (E[ksi])^2) / N) */
	/* E[ksi^2] = sum_sq / N */
	/* E[ksi] = sum / N */
	ret.s = (func->b - func->a)*sqrt((sum_sq/N - pow(sum/N, 2))/N);
	return ret;
}

struct vysledek integral_funkce_vyjm(struct funkce_1d *func, unsigned int N)
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

struct vysledek integral_funkce_skup(struct funkce_1d *func, unsigned int N)
{
	/* samply rozdelujeme mezi intervaly rovnomere */
	unsigned int i, N_int;
	double a_orig, b_orig;
	struct vysledek ret = {0.0, 0.0}, ret_int;

	a_orig = func->a;   /* Protoze budeme menit strukturu func, musime ji na konci */
	b_orig = func->b;   /* uvest do puvodniho stavu. */
	N_int = N / (func->poc_mezibodu);  /* pocet samplovani na kazdy interval */
	if(N % (func->poc_mezibodu) != 0)
		fprintf(stderr, "integral_funkce_skup: pocet kroku %i nejde rovnomerne rozdelit mezi %i intervalu!\n",
		        N, func->poc_mezibodu);
	assert(func->mezibody[func->poc_mezibodu-1] == func->b);  /* posledni mezibod musi byt b */

	for(i = 0; i < func->poc_mezibodu; i++) {
		assert(func->a < func->mezibody[i]);  /* mezibody musi monotonne rust */
		func->b = func->mezibody[i];

		ret_int = integral_funkce(func, N_int);  /* zavolame jiz hotovou metodu s parcialnim intervalem a poctem vzorku */
		ret.I += ret_int.I;
		ret.s += ret_int.s*ret_int.s;  /* !!! musime scitat variance (druhe mocniny odchylek) a pak najednou odmocnit) */

		func->a = func->b;  /* prejdeme na dalsi interval */
	}

	func->a = a_orig;
	func->b = b_orig;
	ret.s = sqrt(ret.s);  /* ve skutecnosti jsme scitali variance, musime odmocnit */
	return ret;
}

struct vysledek integral_plocha_dim(struct funkce_nd *func, unsigned int N)
{
	unsigned int i, hits = 0;
	double y_sample;
	double *x = malloc(func->dim * sizeof(double));
	struct vysledek ret;

	for(i = 0; i < N; i++) {
		/* vygenerujeme nahodny vektor z definicniho oboru a ulozime ho do promenne x */
		rovnom_nd(func->a, func->b, func->dim, x);
		y_sample = rovnom_1d(0, func->c);  /* nahodna hodnota z oboru hodnot */
		if(y_sample <= func->f(x))  /* pricteme jednicku, pokud vzorek padne pod graf funkce */
			hits++;
// 		fprintf(stderr, "(x_0,x_1)=(%f;%f) y=%f y_sample=%f hits=%i\n", x[0], x[1], func->f(x), y_sample, hits);
	}

	/* I = c*PRODUKT(b_i - a_i) * SUM(hits) / N */
	ret.I = func->c*mira(func->a, func->b, func->dim) * ((double) hits) / N;
	free(x);
	return ret;
}
