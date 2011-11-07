#include "binomial.h"

#include "debug.h"
#include "uniform.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Vrati binomicky koeficient, tedy "n nad k".
 *
 * Tato funkce pocita rekurzivne. :-)
 */
static unsigned int binom_coef(unsigned int n, unsigned int k) {
	double product = 1;
	unsigned int i;
	for(i = 1; i <= k; i++) {
		product *= (n - k + i) / (double) i;
	}
	return (unsigned int) product + 0.5;  /* castovani zaokrouhluje dolu */
}

/**
 * Vrati hranicni body intervalu, na kterych se lame hodnota inverzni kumulativni
 * distribucni funkce binomickeho rozdeleni s paramtery p, n. Napr pro p = 1/2 a n = 2
 * vrati {1/4, 3/4, 1}
 *
 * @return adresa pole n+1 intu, prvni _neni_ 0, a posledni _je_ 1; nutno free()-ovat.
 */
static double *binomial_intervals(double p, unsigned int n)
{
	double *ret = malloc((n + 1) * sizeof(double));
	unsigned int k;
	double sum = 0;
	for(k = 0; k < n + 1; k++) {
		sum += binom_coef(n, k) * pow(p, k) * pow(1-p, n-k);
		ret[k] = sum;
	}
	/* nastavime natvrdo jednicku, aby to neudelalo jinde nekonecnou smycku kvuli
	 * zaokrouhovacim chybam */
	ret[n] = 1.0;
	return ret;
}

static void print_intervals(const double *intervals, unsigned int n)
{
	unsigned int i;
	fprintf(stderr, "{");
	for(i = 0; i < n; i++) {
		if(i > 0)
			fprintf(stderr, "(%f) ", intervals[i] - intervals[i-1]);
		fprintf(stderr, "%f; ", intervals[i]);
	}
	fprintf(stderr, "}\n");
}

/**
 * Preusporada intervaly tak, aby se zacinalo nejvetsim. Funguje pouze na intervaly
 * binomickeho rozdeleni - predpoklada, je delka intervalu, tak jak jsou za sebou,
 * je konkavni funkce.
 *
 * Pozor, intervalu (a cisel v poli intervals) musi byt n+1, prvni cislo se neuvadi,
 * ale bere se jako nula
 *
 * @return pole integeru, aby se poznalo povodni usporadani. Predpoklada, ze se zacinalo
 *         cislem nula a koncilo cislem n. Pole je nutne posleze free()-ovat.
 */
static unsigned int *reorder_intervals(double *intervals, unsigned int n)
{
	unsigned int *ret = malloc((n + 1) * sizeof(unsigned int));
	unsigned int i, max_interval_index = -1;
	int left, right;
	double *orig_intervals = malloc((n + 2) * sizeof(double));
	double curr_len, max_len = 0.0, curr_len_left, curr_len_right;  /* pracovni a nejdelsi delka intervalu */

	/* potrebujeme kopii puvodnich intervalu, navic chceme pridat nulu jako prvni prvek */
	memcpy(orig_intervals + 1, intervals, (n + 1) * sizeof(double));
	orig_intervals[0] = 0.0;

	DEBUG_CALL(fprintf(stderr, "Intervaly pred setrizenim\n"));
	DEBUG_CALL(print_intervals(orig_intervals, n + 2));
	/* nejdrive najdeme index koncoveho bodu nejdelsiho intervalu */
	for(i = 1; i < n + 2; i++) {
		curr_len = orig_intervals[i] - orig_intervals[i - 1];
		if(curr_len > max_len) {
			max_len = curr_len;
			max_interval_index = i-1; /* cislujeme pravy bod v _puvodnim_ poli (soupnute o 1) */
		}
	}
	DEBUG_CALL(fprintf(stderr, "Nejdelsi interval je %i-ty (cislovano od nuly)\n", max_interval_index));

	/* pridejme do intervalu a do indexu prvni (nejdelsi) interval */
	intervals[0] = max_len;
	ret[0] = max_interval_index;

	left = max_interval_index - 1;  /* kandidat na dalsi nejdelsi interval */
	right = max_interval_index + 1; /* kandidat na dalsi nejdelsi interval */
	/* v kazdem kroku pridame do intervalu a do indexu dalsi nejdelsi interval */
	for(i = 1; i < n + 1; i++) {
		if(right <= n)
			curr_len_right = orig_intervals[right+1] - orig_intervals[right];
		else
			curr_len_right = -1.0; /* znacka, ze doprava uz nelze */

		if(left >= 0)
			curr_len_left = orig_intervals[left+1] - orig_intervals[left];
		else
			curr_len_left = -1.0; /* znacka, ze doleva uz nelze */

		assert(curr_len_left >= 0 || curr_len_right >= 0);
		if(curr_len_left >= curr_len_right) { /* dalsi nejdelsi interval bereme z leve casti */
			intervals[i] = intervals[i - 1] + curr_len_left;
			ret[i] = left;
			left--;
		} else { /* dalsi nejdelsi interval bereme z prave casti */
			intervals[i] = intervals[i - 1] + curr_len_right;
			ret[i] = right;
			right++;
		}
	}
	assert(left == -1 && right == n + 1);

	DEBUG_CALL(fprintf(stderr, "Intervaly po setrizeni\n"));
	DEBUG_CALL(print_intervals(intervals, n + 1));
	DEBUG_CALL(for(i = 0; i < n + 1; i++) { fprintf(stderr, "%i   ", ret[i]); })
	DEBUG_CALL(fprintf(stderr, "\n"));

	/* nastavime natvrdo jednicku, aby to neudelalo jinde nekonecnou smycku kvuli
	 * zaokrouhovacim chybam */
	intervals[n] = 1.0;
	return ret;
}

unsigned int *basic_binomial(double p, unsigned int n, unsigned int N)
{
	unsigned int *ret = malloc(N * sizeof(unsigned int));
	double *intervals = binomial_intervals(p, n);
	unsigned int i, k;
	double random;

	// mame generovat N cisel
	for(i = 0; i < N; i++) {
		/* vezmeme jeden vzorek z U(0, 1): */
		random = uniform_01();
		/* kvuli ochrane proti nekonecne smycce radsi overime, zda jsme spravne: */
		assert(random >= 0.0 && random <= 1.0);
		/* najdeme nejmensi takove k, ze random <= intervals[k]
		 * urcite plati 0 <= k <= n, protoze intervals[n] == 1 a 0 <= rand <= 1 */ 
		for(k = 0; random > intervals[k]; k++) {
			// nic na praci, vse je napsano ve for klauzuli
		}
		ret[i] = k;
	}

	free(intervals);
	return ret;
}

unsigned int *reindexed_binomial(double p, unsigned int n, unsigned int N)
{
	unsigned int *ret = malloc(N * sizeof(unsigned int));
	double *intervals = binomial_intervals(p, n);
	unsigned int *indices = reorder_intervals(intervals, n);
	unsigned int i, k;
	double random;

	// mame generovat N cisel
	for(i = 0; i < N; i++) {
		/* vezmeme jeden vzorek z U(0, 1): */
		random = uniform_01();
		/* kvuli ochrane proti nekonecne smycce radsi overime, zda jsme spravne: */
		assert(random >= 0.0 && random <= 1.0);
		/* najdeme nejmensi takove k, ze random <= intervals[k]
		 * urcite plati 0 <= k <= n, protoze intervals[n] == 1 a 0 <= rand <= 1 */ 
		for(k = 0; random > intervals[k]; k++) {
			// nic na praci, vse je napsano ve for klauzuli
		}
		ret[i] = indices[k];
	}

	free(indices);
	free(intervals);
	return ret;
}

binomial_generator binomial_generators[] = {
	{basic_binomial, "zakladni"},
	{reindexed_binomial, "preindex"},
};
unsigned int binomial_generators_count = sizeof(binomial_generators)/sizeof(binomial_generator);
