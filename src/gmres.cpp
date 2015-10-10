#include "gmres.h"

GMRES::GMRES()
{
}

SVEC GMRES::mult(SMTRX A, SVEC x)
{
	return A * x;
}