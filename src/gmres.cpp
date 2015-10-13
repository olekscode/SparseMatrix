#include "gmres.h"

#include <iostream>
using namespace std;

GMRES::GMRES()
{
	_r0 = _b - _A * _x0;
	_beta = norm(_r0);

	_H = new double*[m + 1];
	for (int i = 0; i < m + 1; ++i) {
		_H[i] = new double[m];
	}

	_g = new double[m + 1];
}

void GMRES::run()
{
	for (int i = 0; i < m + 1; ++i) {
		for (int j = 0; j < m; ++j) {
			_H[i][j] = 0;
		}
	}

	g[0] = _beta;

	for (int i = 1; i < m + 1; ++i) {
		_g[i] = 0;
	}
}

double GMRES::norm(SVEC vec) const
{
	double sum = 0;

	for (int i = 0; i < vec.size(); ++i) {
		sum += pow(vec.at(i), 2);
	}
	return sqrt(sum);
}