#ifndef CSIR_H
#define CSIR_H

/**
 * @brief CSIR - Compressed Sparse (lower triangle) Row.
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
class CSIR
{
	T* _adiag;
	T* _altr;
	T* _autr;

	int _size;
	int _size_of_altr;

	int *_jptr;
	int *_iptr;

public:
	/**
	 * @brief Creates an instance of CSIR sparse matrix
	 * @details Parses a given plain symmetricmatrix into a CSIR format.
	 * Note that all the diagonal elements are treated as nonempty.
	 * 
	 * @param mtrx Plain symmetric matrix
	 * @param eval Empty value
	 * @param size Size of matrix (number of rows)
	 */
	CSIR(T **mtrx, T eval, int size)
	{
		_size = size;

		_adiag = new T[_size];
		_iptr = new int[_size + 1];
		int *jptr_buff = new int[_size * (_size - 1) / 2];

		_size_of_altr = 0;

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = mtrx[i][i];
			_iptr[i] = _size_of_altr;

			for (int j = 0; j < i; ++j) {
				if (mtrx[i][j] != eval) {
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
	 * @brief Deletes an instance of CSR sparse matrix
	 */
	~CSIR()
	{
		delete[] _adiag;
		delete[] _altr;
		delete[] _autr;
		delete[] _iptr;
		delete[] _jptr;
	}

	/**
	 * @brief Copies the data of CSIR sparse matrix
	 * from other CSIR matrix
	 * 
	 * @param other Reference to other CSIR matrix
	 */
	CSIR(CSIR &other)
	{
		_size = other._size;
		_size_of_altr = other._size_of_altr;

		_adiag = new T[_size];
		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];
		_iptr = new int[_size];
		_jptr = new int[_size_of_altr];

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = other._adiag[i];
			_iptr[i] = other._iptr[i];
		}

		for (int i = 0; i < _size_of_altr; ++i) {
			_altr[i] = other._altr[i];
			_autr[i] = other._autr[i];
			_jptr[i] = other._jptr[i];
		}
	}

	/**
	 * @brief Assignes an instance of CSIR sparse matrix with
	 * other CSIR matrix.
	 * @details Copies all the data from other matrix
	 * 
	 * @param other Reference to other CSIR matrix
	 */
	CSIR& operator= (CSIR &other)
	{
		_size = other._size;
		_size_of_altr = other._size_of_altr;

		_adiag = new T[_size];
		_altr = new T[_size_of_altr];
		_autr = new T[_size_of_altr];
		_iptr = new int[_size];
		_jptr = new int[_size_of_altr];

		for (int i = 0; i < _size; ++i) {
			_adiag[i] = other._adiag[i];
			_iptr[i] = other._iptr[i];
		}

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
	 * @brief Multiplies CSIR matrix by vector.
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

#endif // CSIR_H