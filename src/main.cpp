#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

#include "csr.h"
#include "csir.h"

using namespace std;

bool shouldNotBeEmpty(int sparse_level)
{
	if (sparse_level == 0) {
		return true;
	}

	return rand() % (sparse_level + 1) / sparse_level;
}

char** generateAsymmetricMtrx(int rows, int cols, int sparse_level)
{
	char **res = new char*[rows];

	for (int i = 0; i < rows; ++i) {
		res[i] = new char[cols];

		for (int j = 0; j < cols; ++j) {
			res[i][j] = shouldNotBeEmpty(sparse_level)
								? char(int('a') + rand() % 26)
								: '_';
		}
	}
	return res;
}

char** generateSymmetricMtrx(int size, int sparse_level)
{
	char **res = new char*[size];

	for (int i = 0; i < size; ++i) {
		res[i] = new char[size];

		for (int j = 0; j < i; ++j) {
			if (shouldNotBeEmpty(sparse_level)) {
				res[i][j] = char(int('a') + rand() % 26);
				res[j][i] = char(int('a') + rand() % 26);
			}
			else {
				res[i][j] = res[j][i] = '_';
			}
		}

		res[i][i] = char(int('a') + rand() % 26);
	}
	return res;
}

template <typename T>
void outputMatrix(T **mtrx, int rows, int cols)
{
	for (int i = 0; i < rows; ++i) {
		cout << "\t";

		for (int j = 0; j < cols; ++j) {
			cout << mtrx[i][j] << " ";
		}
		cout << "\n";
	}
	cout << "\n";
}

template <typename T>
void outputVector(T *vec, int size, string label)
{
	cout << label << ": [";

	for (int i = 0; i < size - 1; ++i) {
		cout << vec[i] << " ";
	} 

	cout << vec[size - 1] << "]\n\n";
}

int main(int argc, char* argv[])
{
	srand(time(0));

	int m = 40;
	int n = 20;
	int sparse_level = 40;

	char **asym_mtrx = generateAsymmetricMtrx(m, n, sparse_level);
	outputMatrix(asym_mtrx, m, n);

	CSR<char> *sm1 = new CSR<char> (asym_mtrx, '_', m, n);

	outputVector(sm1->aelem(), sm1->size_of_aelem(), "aelem");
	outputVector(sm1->iptr(), sm1->rows() + 1, "iptr");
	outputVector(sm1->jptr(), sm1->size_of_aelem(), "jptr");

	char **sym_mtrx = generateSymmetricMtrx(n, sparse_level);
	outputMatrix(sym_mtrx, n, n);

	CSIR<char> *sm2 = new CSIR<char> (sym_mtrx, '_', n);

	outputVector(sm2->adiag(), sm2->size(), "adiag");
	outputVector(sm2->altr(), sm2->size_of_altr(), "altr");
	outputVector(sm2->autr(), sm2->size_of_altr(), "autr");
	outputVector(sm2->iptr(), sm2->size() + 1, "iptr");
	outputVector(sm2->jptr(), sm2->size_of_altr(), "jptr");

	//delete sm1;
	delete sm2;
	return 0;
}