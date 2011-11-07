#include "uniform.h"

#include <stdlib.h>

inline double uniform_01()
{
	return ((double) rand()) / ((double) RAND_MAX);
}
