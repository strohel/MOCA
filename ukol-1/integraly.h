#include "funkce.h"

struct vysledek {
	double I;  /* hodnota integralu */
	double s;  /* odhad smerodatne odchylky (odmocniny z variance) */
};

/**
 * 1. vzorkovani plochy
 *
 * Predpoklada, ze funkce je zespoda omezana nulou a shora konstantou c, ktera je v @param func.
 *
 * @param func popis funkce, ktera se ma integrovat
 * @param N pocet iteraci
 */
struct vysledek integral_plocha(struct funkce_1d *func, unsigned int N);

/**
 * 2. vzorkovani funkce
 *
 * @param func popis funkce, ktera se ma integrovat
 * @param N pocet iteraci
 */
struct vysledek integral_funkce(struct funkce_1d *func, unsigned int N);

/**
 * 3. vzorkovani funkce vyjmutim hlavni casti; f >= g >= 0
 *
 * Pouziva funkci g >= f z @param func.
 *
 * @param func popis funkce, ktera se ma integrovat
 * @param N pocet iteraci
 */
struct vysledek integral_funkce_vyjm(struct funkce_1d *func, unsigned int N);

/**
 * 4. vzorkovani funkce se skupinovym vyberem
 *
 * Pouziva mezibody, ktere jsou ulozeny v @param func, vzorkovani rozdeli rovnomerne do intervalu,
 * kterych je o jedna vic nez mezibodu.
 *
 * @param func popis funkce, ktera se ma integrovat
 * @param N pocet iteraci
 */
struct vysledek integral_funkce_skup(struct funkce_1d *func, unsigned int N);

/* 5. vicerozmerne vzorkovani plochy */
struct vysledek integral_funkce_dim(struct funkce_nd *func, unsigned int N);
