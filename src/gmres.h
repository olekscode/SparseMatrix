#ifndef GMRES_H
#define GMRES_H

#include <vector>

#include "sparse/cslr.h"
#include "sparse/csr.h"

#define VALUE_T double
#define SMTRX CSLR<VALUE_T>
#define SVEC std::vector<VALUE_T>

class GMRES
{
public:
	GMRES();
	static SVEC mult(SMTRX A, SVEC x);
};

#endif // GMRES_H