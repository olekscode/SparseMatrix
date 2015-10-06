#ifndef CSR_H
#define CSR_H

#include <vector>

#include "exception.h"

/**
 * @brief CSR - Compressed Sparse Row.
 * @details Sparse matrix format for asymmetric matrices
 * with asymmetric portraits.
 * 
 * Matrix is stored in following vectors:
 * - aelem - nonempty elements of matrix
 * - iptr - i-th element holds the position in which the i-th
 * row appears in aelem for the first time
 * - jptr - column-indices of the corresponding aelem elements
 * 
 * @tparam T Type of data stored in matrix.
 */
template <typename T>
class CSR
{
    T *_aelem;
    int *_jptr;
    int *_iptr;

    int _size_of_aelem;
    int _rows;
    int _cols;

    T _eval;

public:
    /**
     * @brief Creates an instance of CSR sparse matrix
     * @details Parses a given plain matrix into a CSR format
     * This constructor is time-expensive and therefore useful only for
     * testing on small matrices
     * 
     * @param mtrx Plain matrix
     * @param rows Number of rows in matrix
     * @param cols Number of columns in matrix
     * @param eval Empty value
     */
    CSR(T **mtrx, int rows, int cols, T eval = 0)
    {
        _rows = rows;
        _cols = (cols == 0) ? rows : cols;
        _eval = 0;

        _iptr = new int[_rows + 1];
        int *jptr_buff = new int[_rows * _cols];

        _size_of_aelem = 0;

        for (int i = 0; i < _rows; ++i) {
            _iptr[i] = _size_of_aelem;

            for (int j = 0; j < _cols; ++j) {
                if (mtrx[i][j] != eval) {
                    jptr_buff[_size_of_aelem++] = j;
                }
            }
        }

        _iptr[_rows] = _size_of_aelem;

        _jptr = new int[_size_of_aelem];
        _aelem = new T[_size_of_aelem];

        for (int i = 1; i <= _rows; ++i) {
            for (int k = _iptr[i - 1]; k < _iptr[i]; ++k) {
                _jptr[k] = jptr_buff[k];
                _aelem[k] = mtrx[i - 1][_jptr[k]];
            }
        }

        delete[] jptr_buff;
    }

    /**
     * @brief Copies the data of CSR sparse matrix
     * from other CSR matrix
     * 
     * @param other Reference to other CSR matrix
     */
    CSR(CSR &other)
    {
        _rows = other._rows;
        _cols = other._cols;
        _size_of_aelem = other._size_of_aelem;
        _eval = other._eval;

        _aelem = new T[_size_of_aelem];
        _iptr = new int[_rows];
        _jptr = new int[_size_of_aelem];

        for (int i = 0; i < _rows; ++i) {
            _iptr[i] = other._iptr[i];
        }

        for (int i = 0; i < _size_of_aelem; ++i) {
            _aelem[i] = other._aelem[i];
            _jptr[i] = other._jptr[i];
        }
    }

    /**
     * @brief Creates an instance of CSR matrix
     * @details All the CSR members are passed dirrectly as arguments
     * 
     * @param aelem Array of nonempty elements
     * @param iptr Array of position in which the corresponding rows
     * appear in aelem for the first time
     * @param jptr Array of column-indices of the corresponding
     * nonempty elements
     * @param rows Number of rows
     * @param cols Number of columns
     * @param size_of_aelem Number of nonempty elements
     * @param eval Empty value
     */
    CSR(T *aelem, int *iptr, int *jptr,
         int rows, int cols, int size_of_aelem, T eval = 0)
    {
        _rows = rows;
        _cols = cols;
        _size_of_aelem = size_of_aelem;
        _eval = eval;

        _aelem = new T[_size_of_aelem];
        _iptr = new int[_rows];
        _jptr = new int[_size_of_aelem];

        for (int i = 0; i < _rows; ++i) {
            _iptr[i] = iptr[i];
        }

        for (int i = 0; i < _size_of_aelem; ++i) {
            _aelem[i] = aelem[i];
            _jptr[i] = jptr[i];
        }
    }

    /**
     * @brief Creates an instance of empty CSR matrix
     * @details Stores indices of all nonempty elements and
     * allocates memory for their values (_aelem).
     * Actual values are left empty (assigneg with eval)
     * 
     * @param num_in_rows Array of numbers of nonempty elements
     * in the corresponding rows
     * @param jptr Array of column-indices of the corresponding
     * nonempty elements
     * @param rows Number of rows
     * @param cols Number of columns
     * @param eval Empty value
     */
    CSR(int *num_in_rows, int *jptr, int rows, int cols, T eval = 0)
    {
        _rows = rows;
        _cols = cols;
        _eval = eval;

        _iptr = new int[_rows + 1];

        int _size_of_aelem = 0;

        for (int i = 0; i < _rows; ++i) {
            _iptr[i] = _size_of_aelem;
            _size_of_aelem += num_in_rows[i];
        }

        _iptr[_rows] = _size_of_aelem;

        _aelem = new T[_size_of_aelem];
        _jptr = new int[_size_of_aelem];

        for (int i = 0; i < _size_of_aelem; ++i) {
            _aelem[i] = 0;
            _jptr[i] = jptr[i];
        }
    }

    /**
     * @brief Deletes an instance of CSR sparse matrix
     */
    ~CSR()
    {
        delete[] _aelem;
        delete[] _iptr;
        delete[] _jptr;
    }

    /**
     * @brief Assignes an instance of CSR sparse matrix with
     * other CSR matrix.
     * @details Copies all the data from other matrix
     * 
     * @param other Reference to other CSR matrix
     */
    CSR& operator= (CSR &other)
    {
        _rows = other._rows;
        _cols = other._cols;
        _size_of_aelem = other._size_of_aelem;
        _eval = other._eval;

        _aelem = new T[_size_of_aelem];
        _iptr = new int[_rows];
        _jptr = new int[_size_of_aelem];

        for (int i = 0; i < _rows; ++i) {
            _iptr[i] = other._iptr[i];
        }

        for (int i = 0; i < _size_of_aelem; ++i) {
            _aelem[i] = other._aelem[i];
            _jptr[i] = other._jptr[i];
        }

        return *this;
    }

    /**
	 * @brief Gets aelem - an array of nonempty elements
	 * of matrix
	 * @return aelem array
	 */
    T* aelem() const
    {
        return _aelem;
    }

    /**
	 * @brief Gets the iptr - an array of position in which the
	 * corresponding rows appear in aelem for the first time
	 * @return iptr array
	 */
    int* iptr() const
    {
        return _iptr;
    }

    /**
	 * @brief Gets column-indices of the corresponding aelem
	 * elements
	 * @return jptr array
	 */
    int* jptr() const
    {
        return _jptr;
    }

    /**
	 * @brief Gets the number of rows in matrix
	 * @return Number of rows
	 */
    int rows() const
    {
        return _rows;
    }

    /**
	 * @brief Gets the number of columns in matrix
	 * @return Number of columns
	 */
    int cols() const
    {
        return _cols;
    }

    /**
	 * @brief Gets size of aelem - number of nonempty elements
	 * in matrix
	 * @return Size of aelem
	 */
    int size_of_aelem() const
    {
        return _size_of_aelem;
    }

    /**
	 * @brief Multiplies CSR matrix by vector.
	 * @details Note that for large sparse matrices this
	 * multiplication will be extremely efficient.
	 * 
	 * @param vec Given vector
	 * @return Result of multiplication
	 */
    std::vector<T> operator* (std::vector<T> &vec)
    {
        if (vec.size() != _cols) {
            throw MultSizeMismatch();
        }

        std::vector<T> res(_rows);

        for (int i = 0; i < _rows; ++i) {
            res.at(i) = _eval;

            for (int j = _iptr[i]; j < _iptr[i + 1]; ++j) {
                res.at(i) += _aelem[j] * vec.at(_jptr[j]);
            }
        }

        return res;
    }
};

#endif // CSR_H