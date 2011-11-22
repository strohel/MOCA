#include "generatory.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void)
{
	/* pocty opakovani: */
	unsigned int N[] = {100, 10000, 1000000};
	unsigned int N_count = sizeof(N)/sizeof(unsigned int);

	/* pomocne promenne: */
	unsigned int j, i, k;
	double mean, variance, N_inverse, result, x, y, arg;
	char *task, *method, count[20], *filename;
	FILE *outfile;
	generator *gen;

	printf("priklad,       N,  generator,    prumer,  odchylka, soubor\n");
	task = "U(0,1)";
	for(i = 0; i < N_count; i++) {
		snprintf(count, 20, "%i", N[i]);  /* do promenne count zapiseme textove pocet kroku */
		for(j = 0; j < generator_count; j++) {
			/* priprava */
			gen = generators + j; /* pointerova aritmetika */
			method = gen->name;
			filename = malloc(strlen(task) + 1 /* - */
			                + strlen(count) + 1 /* - */
			                + strlen(method)
			                + 4 /* .txt */ + 1 /* \0 */);
			sprintf(filename, "%s-%s-%s.txt", task, count, method); /* do filename zapise jmeno souboru */
			outfile = fopen(filename, "w");
			if(!outfile) {
				perror("Nepovedlo se otevrit soubor v aktualnim adresari pro zapis");
				free(filename);
				continue;
			}

			/* priprava na generovani */
			mean = 0;
			variance = 0;
			N_inverse = 1.0/N[i];  /* nasobeni je rychlejsi nez deleni, takze delme jen jednou */
			gen->set_seed(4242.0);

			/* vlastni generovani */
			for(k = 0; k < N[i]; k++) {
				result = gen->rand();
				fprintf(outfile, "%f\n", result);
				/* v obou pripadech nasobime 1/N uz tady aby to nepreteklo: */
				mean += N_inverse * result;
				variance += N_inverse * result*result;
			}

			/* zpracovani vysleku */
			variance -= mean*mean;  /* ve for cyklu se spocital NEcentralni moment, centralizujme */

			printf("%7s, %7i, %10s, %9f, %9f, %s\n", task, N[i], method,
			       mean, sqrt(variance), filename);

			/* uklid */
			fclose(outfile);
			free(filename);
		}
	}
	printf("\n"); // novy priklad

	printf("priklad,       N,  generator,    prumer,  odchylka, skutecna odchylka\n");
	task = "pi";
	for(i = 0; i < N_count; i++) {
		snprintf(count, 20, "%i", N[i]);  /* do promenne count zapiseme textove pocet kroku */
		for(j = 0; j < generator_count; j++) {
			/* priprava */
			gen = generators + j; /* pointerova aritmetika */
			method = gen->name;

			/* priprava na generovani */
			mean = 0;
			variance = 0;
			N_inverse = 1.0/N[i];  /* nasobeni je rychlejsi nez deleni, takze delme jen jednou */
			gen->set_seed(4242.0);

			/* vlastni generovani */
			for(k = 0; k < N[i]; k++) {
				x = gen->rand();
				y = gen->rand();
				result = gen->rand();
				arg = 1.0 - x*x - y*y;
				if(arg >= 0 && result <= sqrt(arg))
					mean += N_inverse * 1.0;
			}

			/* zpracovani vysleku */
			variance = N_inverse * (mean - mean*mean);
			/* pi je sestinasobek objemu osminy jednotkove koule */
			mean *= 6.0;
			variance *= 6.0;

			printf("%7s, %7i, %10s, %9f, %9f, %f\n", task, N[i], method,
			       mean, sqrt(variance), fabs(M_PI - mean));
		}
	}

	return 0;
}
