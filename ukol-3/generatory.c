#include "generatory.h"

#include <stdint.h> /* uint64_t */
#include <stdio.h>
#include <stdlib.h>

void libc_rand_set_seed(double new_seed)
{
	srand(new_seed);
}

double libc_rand_rand()
{
	return rand() / (RAND_MAX + 1.0);
}


/**
 * Hack abychom mohli soupnout seed jako static promennou a zaroven ho nastavovat z funkce
 * set_seed
 */
double lincong_rand_with_seed(double new_seed)
{
	static uint64_t seed = 0;

	if(new_seed != 0.0)
		seed = new_seed;

	/* __UINT64_C je makro, ktere prida na konec konstanty UL nebo ULL (podle architektury)
	 * jinak by kompilator mohl ciselne konstanty oriznout na int. Pokud vas stdint.h
	 * nedefinuje tyto konstanty, pridejte na konec cisla UL pro architektury, kde je
	 * long int 64-bitovy a ULL, kde je long long in 64bitovy. */
	seed = seed *  __UINT64_C(347) + __UINT64_C(874329843209847);
	return ((double) seed) / (((uint64_t) -1) + 1.0);
}

void lincong_set_seed(double seed)
{
	lincong_rand_with_seed(seed);
}

double lincong_rand()
{
	return lincong_rand_with_seed(0.0);
}


/**
 * Vrati cislo, ktere ma v binarnim zapisu same nuly a pak @param bit_count jednicek
 */
static unsigned right_bitmask(unsigned bit_count)
{
	unsigned ret = 0, i;
	for(i = 0; i < bit_count; i++) {
		ret |= 1 << i;
	}
	return ret;
}

#define n 624  /* musi to byt jako makro, C kompilator nebere const int ve specifikaci delky pole */
double mersenne_rand_with_seed(double new_seed)
{
	/* parametry podle MT19937 */
	const unsigned m = 397, r = 31;
	const unsigned a = 0x9908B0DF;
	const unsigned u = 11;
	const unsigned s = 7, b = 0x9D2C5680;
	const unsigned t = 15, c = 0xEFC60000;
	const unsigned l = 18;

	/* promenne zachovane mezi volanimi */
	static unsigned numbers[n];
	static unsigned index = 0;

	/* pracovni promenne */
	unsigned i, y;

	// kod pro nastaveni seedu
	if(new_seed != 0.0) {
		// inicializujeme cisla pomoci standardniho generatoru z libc
		libc_rand_set_seed(new_seed);
		for(i = 0; i < n; i++) {
			numbers[i] = libc_rand_rand() * ((unsigned) -1);
		}
	}

	if (index == 0) {
		// pregenerovani cisel
		for(i = 0; i < n; i++) {
			y = (numbers[i] >> r)  +  (numbers[(i+1) % n] & right_bitmask(r));
			numbers[i] = numbers[(i+m) % n] ^ (y >> 1);
			if((y % 2) == 1) { // liche y
				numbers[i] ^= a;
			}
		}
	}

	// vlastni twistovani a temperovani
	y = numbers[index];
	y ^= (y >> u);
	y ^= (y << s) & b;
	y ^= (y << t) & c;
	y ^= (y >> l);

	index++;
	index %= n;
	return ((double) y) / (((unsigned) -1) + 1.0);
}

void mersenne_set_seed(double seed)
{
	mersenne_rand_with_seed(seed);
}

double mersenne_rand()
{
	return mersenne_rand_with_seed(0.0);
}


generator generators[] = {
	{libc_rand_set_seed, libc_rand_rand, "libc_rand"},
	{lincong_set_seed, lincong_rand, "lin_congr"},
	{mersenne_set_seed, mersenne_rand, "mersenne"},
};
unsigned generator_count = sizeof(generators) / sizeof(generator);
