#include "binomial.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Vrati rozdil casu v sekundach
 */
static double time_difference(const struct timespec *start, const struct timespec *end)
{
	double ret = end->tv_sec - start->tv_sec;
	ret += (end->tv_nsec - start->tv_nsec) / 1000000000.0;
	return ret;
}

int main(void)
{
	/* pocty opakovani: */
	unsigned int N[] = {1000, 1000000};
	unsigned int N_count = sizeof(N)/sizeof(unsigned int);

	/* parametry pro binomicke rozdeleni: */
	unsigned int n = 20;
	double p = 0.75;

	/* pomocne promenne: */
	unsigned int j, i, k, *result_discrete;
	char distribution[30], *method, count[20], *filename;
	FILE *outfile;
	struct timespec start_time, stop_time;
	double mean, variance, N_inverse;

	/* hlavicka */
	printf("    distribuce,       N,   metoda,    prumer,  odchylka,       cas, soubor\n");

	snprintf(distribution, 30, "binom(%.2f;%i)", p, n);
	for(i = 0; i < N_count; i++) {
		snprintf(count, 20, "%i", N[i]);  /* do promenne count zapiseme textove pocet kroku */
		for(j = 0; j < binomial_generators_count; j++) {
			/* priprava */
			method = binomial_generators[j].name;
			filename = malloc(strlen(distribution) + 1 /* - */
			                + strlen(count) + 1 /* - */
			                + strlen(method)
			                + 4 /* .txt */ + 1 /* \0 */);
			sprintf(filename, "%s-%s-%s.txt", distribution, count, method); /* do filename zapise jmeno souboru */
			outfile = fopen(filename, "w");
			if(!outfile) {
				perror("Nepovedlo se otevrit soubor v aktualnim adresari pro zapis");
				free(filename);
				continue;
			}

			/* vlastni generovani */
			srand(42);
			clock_gettime(CLOCK_REALTIME, &start_time);
			result_discrete = binomial_generators[j].generator(p, n, N[i]);
			clock_gettime(CLOCK_REALTIME, &stop_time);

			/* zpracovani vysledku */
			mean = 0;
			variance = 0;
			N_inverse = 1.0/N[i];  /* nasobeni je rychlejsi nez deleni, takze delme jen jednou */
			for(k = 0; k < N[i]; k++) {
				fprintf(outfile, "%i\n", result_discrete[k]);
				/* v obou pripadech nasobime 1/N uz tady aby to nepreteklo: */
				mean += N_inverse * result_discrete[k];
				variance += N_inverse * result_discrete[k] * result_discrete[k];
			}
			variance -= mean*mean;  /* ve for cyklu se spocital NEcentralni moment, centralizujme */

			printf("%14s, %7i, %8s, %9f, %9f, %8fs, %s\n", distribution, N[i], method,
			       mean, sqrt(variance), time_difference(&start_time, &stop_time), filename);

			/* uklid */
			fclose(outfile);
			free(result_discrete);
			free(filename);
		}
	}

	return 0;
}
