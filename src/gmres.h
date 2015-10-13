#ifndef GMRES_H
#define GMRES_H

#include <vector>
#include <math.h>

#include "sparse/csr.h"
#include "sparse/cslr.h"

#define SMTRX CSIR<double>
#define SVEC std::vector<double>

class GMRES
{
	int _n;
	int _m;
	SMTRX _A;
	SVEC _b;
	SVEC _x0;
	SVEC _r0;
	double _beta;
	double **_H;
	double *_g;

public:
	GMRES();
	static void run();

private:
	double norm(SVEC vec) const;
};

#endif // GMRES_H