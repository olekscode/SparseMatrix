#ifndef CSIR_H
#define CSIR_H

/**
 * @brief CSIR - Compressed Sparse (lower triangle) Row.
 * @details Sparse matrix format for asymmetric matrices
 * with symmetric portraits. A.k.a. Skyline format.
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
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	T* adiag() const
	{
		return _adiag;
	}

	/**
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	T* altr() const
	{
		return _altr;
	}

	/**
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	T* autr() const
	{
		return _autr;
	}

	/**
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	int* iptr() const
	{
		return _iptr;
	}

	/**
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	int* jptr() const
	{
		return _jptr;
	}

	/**
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	virtual int size() const
	{
		return _size;
	}

	/**
	 * @brief [brief description]
	 * @details [long description]
	 * @return [description]
	 */
	virtual int size_of_altr() const
	{
		return _size_of_altr;
	}
};

#endif // CSIR_H