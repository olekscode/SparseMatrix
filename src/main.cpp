#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "gmres.h"
#include "sparse/csr.h"
#include "sparse/cslr.h"

#define VALUE_T double
#define SMTRX CSLR<VALUE_T>
#define SVEC std::vector<VALUE_T>

using namespace std;

SMTRX loadMtrx(const char* path)
{
	ifstream file(path);

	vector<int> jptr;

	string line;

	// get num_in_rows
	getline(file, line);
	istringstream* isstrm = new istringstream(line);

	int num;
	int counter = 0;
	vector<int> num_in_rows;

	while (!isstrm->eof()) {
		*isstrm >> num;
		num_in_rows.push_back(num);
		counter += num;
	}

	delete isstrm;

	// get jptr
	getline(file, line);
	isstrm = new istringstream(line);

	for (int k = 0; k < counter; ++k) {
		*isstrm >> num;
		jptr.push_back(num);
	}

	delete isstrm;

	// allocate space
	SMTRX A(&num_in_rows[0], &jptr[0], num_in_rows.size());

	// insert values
	VALUE_T val;
	int i;
	int j;

	while (!file.eof()) {
		getline(file, line);
		isstrm = new istringstream(line);

		*isstrm >> val >> i >> j;
		A.insert(val, i, j);

		delete isstrm;
	}

	file.close();
	return A;
}

SVEC loadVec(const char* path)
{
	ifstream file(path);
	SVEC res;
	VALUE_T val;

	while (file >> val) {
		res.push_back(val);
	}

	file.close();
	return res;
}

int main(int argc, char* argv[])
{
	try {
		SMTRX A = loadMtrx("/home/olk/Desktop/A.txt");
		SVEC x = loadVec("/home/olk/Desktop/x.txt");

		for (int i = 0; i < A.size_of_altr(); ++i) {
			cout << A.altr()[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < A.size_of_altr(); ++i) {
			cout << A.autr()[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < A.size(); ++i) {
			cout << A.adiag()[i] << " ";
		}
		cout << endl;

		for (int i = 0; i < x.size(); ++i) {
			cout << x.at(i) << " ";
		}
		cout << endl;

		SVEC b = GMRES::mult(A, x);

		for (int i = 0; i < b.size(); ++i) {
			cout << b.at(i) << " ";
		}
		cout << endl;

		return 0;
	}
	catch (exception &e) {
		cerr << "ERROR: " << e.what() << endl;
		return 1;
	}
}