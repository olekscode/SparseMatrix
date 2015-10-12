// TODO: _iptr should contain _size elements, not _size + 1
// 		 (lower triangular matrix starts from the second row)

#ifndef CSLR_H
#define CSLR_H

#include <vector>

#include "exception.h"

/**
 * @brief CSLR - Compressed Sparse (lower triangle) Row.
 * @details Sparse matrix format for asymmetric matrices
 * with symmetric portraits. A.k.a. Skyline format.
 * 
 * Matrix is stored in following vectors:
 * - adiag - diagonal elements
 * - altr - nonempty elements of lower triangular matrix
 * - autr - nonempty elements of upper triangular matrix
 * - iptr - i-th element holds the position in which the i-th
 * row appears in altr for the first time
 * - jptr - column-indices of the corresponding altr elements
 * 
 * @tparam T - Type of data stored in matrix.
 */
template <typename T>
class CSLR
{
	T* _adiag;
	T* _altr;
	T* _autr;

	int _size;
	int _size_of_altr;

	int *_jptr;
	int *_iptr;

	T _eval;

public:
	/**
	 * @brief Creates an instance of CSLR matrix
	 * @details Parses a given plain symmetricmatrix into a CSLR format.
	 * Note that all the diagonal elements are treated as nonempty.
	 * This constructor is time-expensive and therefore useful only for
	 * testing on small matrices
	 * 
	 * @param mtrx Plain symmetric matrix
	 * @param size Size of matrix (number of rows)
	 * @param eval Empty value
	 */
	CSLR(T **mtrx, int size, T eval = 0)
	{
		_size = size;
		_eval = eval;

		_adiag = new T[_size];
		_iptr = new int[_size + 1];
		int *jptr_buff = new int[_size * (_size - 1) / 2];

		_size_of_altr = 0;

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = mtrx[i][i];
			_iptr[i] = _size_of_altr;

			for (int j = 0; j < i; ++j) {
				if (mtrx[i][j] != _eval) {
					jptr_buff[_size_of_altr++] = j;
				}
			}
		}

		_iptr[_size] = _size_of_altr;

		_jptr = new int[_size_of_altr];
		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];

		for (int i = 1; i <= _size; ++i) {
			for (int k = _iptr[i - 1]; k < _iptr[i]; ++k) {
				_jptr[k] = jptr_buff[k];
				_altr[k] = mtrx[i - 1][_jptr[k]];
				_autr[k] = mtrx[_jptr[k]][i - 1];
			}
		}

		delete[] jptr_buff;
	}

	/**
	 * @brief Creates an instance of CSLR matrix
	 * @details All the CSLR members are passed dirrectly as arguments
	 * 
	 * @param adiag Array of diagonal elements (treated as nonempty)
	 * @param altr Array of nonempty elements of lower triangular matrix
	 * @param autr Array of nonempty elements of upper triangular matrix
	 * @param iptr Array of position in which the corresponding rows
	 * appear in altr for the first time
	 * @param jptr Array of column-indices of the corresponding
	 * nonempty elements of lower triangular matrix
	 * @param size Size of matrix (number of rows)
	 * @param size_of_altr Number of nonempty elements in lower
	 * triangular matrix
	 * @param eval Empty value
	 */
	CSLR(T *adiag, T *altr, T *autr, int *iptr, int *jptr,
		 int size, int size_of_altr, T eval = 0)
	{
		_size = size;
		_size_of_altr = size_of_altr;
		_eval = eval;

		_adiag = new T[_size];
		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];
		_iptr = new int[_size];
		_jptr = new int[_size_of_altr];

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = adiag[i];
			_iptr[i] = iptr[i];
		}

		_iptr[_size] = iptr[_size];

		for (int i = 0; i < _size_of_altr; ++i) {
			_altr[i] = altr[i];
			_autr[i] = autr[i];
			_jptr[i] = jptr[i];
		}
	}

	/**
	 * @brief Creates an instance of empty CSLR matrix
	 * @details Stores indices of all nonempty elements and
	 * allocates memory for their values (_adiag, _altr and _autr).
	 * Actual values are left empty (assigneg with eval)
	 * 
	 * @param num_in_ltrows Array of numbers of nonempty elements
	 * in the corresponding rows of lower triangular matrix
	 * @param jptr Array of column-indices of the corresponding
	 * nonempty elements of lower triangular matrix
	 * @param size Size of matrix (number of rows)
	 * @param eval Empty value
	 */
	CSLR(int *num_in_ltrows, int *jptr, int size, T eval = 0)
	{
		_size = size;
		_eval = eval;

		_adiag = new T[_size];
		_iptr = new int[_size + 1];

		_size_of_altr = 0;

		for (int i = 0; i < _size; ++i) {
			_iptr[i] = _size_of_altr;
			_size_of_altr += num_in_ltrows[i];
			_adiag[i] = 0;
		}

		_iptr[_size] = _size_of_altr;

		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];
		_jptr = new int[_size_of_altr];

		for (int i = 0; i < _size_of_altr; ++i) {
			_altr[i] = 0;
			_autr[i] = 0;
			_jptr[i] = jptr[i];
		}
	}

	/**
	 * @brief Copies the data of CSLR matrix
	 * from other CSLR matrix
	 * 
	 * @param other Reference to other CSLR matrix
	 */
	CSLR(const CSLR &other)
	{
		_size = other._size;
		_size_of_altr = other._size_of_altr;
		_eval = other._eval;

		_adiag = new T[_size];
		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];
		_iptr = new int[_size];
		_jptr = new int[_size_of_altr];

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = other._adiag[i];
			_iptr[i] = other._iptr[i];
		}

		_iptr[_size] = other._iptr[_size];

		for (int i = 0; i < _size_of_altr; ++i) {
			_altr[i] = other._altr[i];
			_autr[i] = other._autr[i];
			_jptr[i] = other._jptr[i];
		}
	}

	/**
	 * @brief Deletes an instance of CSLR matrix
	 */
	~CSLR()
	{
		delete[] _adiag;
		delete[] _altr;
		delete[] _autr;
		delete[] _iptr;
		delete[] _jptr;
	}

	/**
	 * @brief Assignes an instance of CSLR matrix with
	 * other CSLR matrix.
	 * @details Copies all the data from other matrix
	 * 
	 * @param other Reference to other CSLR matrix
	 */
	CSLR& operator= (const CSLR &other)
	{
		_size = other._size;
		_size_of_altr = other._size_of_altr;
		_eval = other._eval;

		_adiag = new T[_size];
		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];
		_iptr = new int[_size];
		_jptr = new int[_size_of_altr];

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = other._adiag[i];
			_iptr[i] = other._iptr[i];
		}

		_iptr[_size] = other._iptr[_size];

		for (int i = 0; i < _size_of_altr; ++i) {
			_altr[i] = other._altr[i];
			_autr[i] = other._autr[i];
			_jptr[i] = other._jptr[i];
		}

		return *this;
	}

	/**
	 * @brief Gets adiag - an array of diagonal elements
	 * @return adiag array
	 */
	T* adiag() const
	{
		return _adiag;
	}

	/**
	 * @brief Gets altr - an array of nonempty elements of
	 * lower triangular matrix
	 * @return altr array
	 */
	T* altr() const
	{
		return _altr;
	}

	/**
	 * @brief Gets autr - an array of nonempty elements of
	 * upper triangular matrix
	 * @return autr array
	 */
	T* autr() const
	{
		return _autr;
	}

	/**
	 * @brief Gets the iptr - an array of position in which the
	 * corresponding rows appear in altr for the first time
	 * @return iptr array
	 */
	int* iptr() const
	{
		return _iptr;
	}

	/**
	 * @brief Gets column-indices of the corresponding altr
	 * elements
	 * @return jptr array
	 */
	int* jptr() const
	{
		return _jptr;
	}

	/**
	 * @brief Gets size of matrix (number of rows)
	 * @return Size of matrix
	 */
	int size() const
	{
		return _size;
	}

	/**
	 * @brief Gets size of altr - number of nonempty elements
	 * in lower triangular matrix
	 * @return Size of altr
	 */
	int size_of_altr() const
	{
		return _size_of_altr;
	}

	/**
	 * @brief Inserts an element to the matrix
	 * @details The space for this element should already
	 * be allocated by CSLR(int*, int*, int, T) constructor.
	 * If not - an error will be thrown.
	 * 
	 * @param val Value to be inserted
	 * @param i Row-index
	 * @param j Column-index
	 */
	void insert(T val, int i, int j)
	{
		// TODO: Make it cleaner

		if (i > j) {
			for (int k = _iptr[i]; k < _iptr[i + 1]; ++k) {
				if (_jptr[k] == j) {
					_altr[k] = val;
					return;
				}
				else if (_jptr[k] > j) {
					break;
				}
			}
			throw InsertNoSuchElement(i, j);
		}
		else if (i < j) {
			for (int k = _iptr[j]; k < _iptr[j + 1]; ++k) {
				if (_jptr[k] == i) {
					_autr[k] = val;
					return;
				}
				else if (_jptr[k] > j) {
					break;
				}
			}
			throw InsertNoSuchElement(i, j);
		}
		else {
			_adiag[i] = val;
		}
	}

	/**
	 * @brief Multiplies CSLR matrix by vector.
	 * @details Note that for large sparse matrices this
	 * multiplication will be extremely efficient.
	 * 
	 * @param vec Given vector
	 * @return Result of multiplication
	 */
	std::vector<T> operator* (std::vector<T> &vec)
	{
		if (vec.size() != _size) {
			throw MultSizeMismatch();
		}

		std::vector<T> res(_size);

		for (int i = 0; i < _size; ++i) {
			res.at(i) = _adiag[i] * vec.at(i);

			for (int j = _iptr[i]; j < _iptr[i + 1]; ++j) {
				res.at(i) += _altr[j] * vec.at(_jptr[j]);
				res.at(_jptr[j]) += _autr[j] * vec.at(i);
			}
		}

		return res;
	}
};

#endif // CSLR_H