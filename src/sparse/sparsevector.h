#ifndef SPARSEVECTOR_H
#define SPARSEVECTOR_H

#include "vectorbase.h"
#include "exception.h"

/**
 * @brief Reimplementation of sparse mathematical
 * vector.
 * @details Provides basic arithmetic operators
 * 
 * Vector is storred in following arrays:
 * - aelem - nonempty elements
 * - iptr - indices of corresponding aelem elements
 * 
 * @tparam T Type of data stored in vector 
 */
template <typename T>
class SparseVector : public VectorBase<T>
{
	T *_aelem;
	int *_iptr;

	int _num_of_aelem;

public:
	/**
	 * @brief Creates an instance of empty SparseVector
	 * @details Allocates memory for given number
	 * of elements. They must be inserted later.
	 * 
	 * @param size Size of SparseVector
	 * @param num_of_nonempty Number of nonempty elements
	 */
	SparseVector(int size, int num_of_nonempty)
		: VectorBase<T>(size)
	{
		_num_of_aelem = num_of_nonempty;
		_aelem = new T[_num_of_aelem];
		_iptr = new int[_num_of_aelem];
	}

	/**
	 * @brief Creates an instance of SparseVector
	 * @details The elements are coppied from given
	 * plain array.
	 * This constructor is time-expensive and therefore
	 * useful only for testing on small vectors
	 * 
	 * @param arr Plain array
	 * @param size Size of SparseVector
	 */
	SparseVector(T *arr, int size)
		: VectorBase<T>(size)
	{
		int *iptr_buff = new int[size];
		_num_of_aelem = 0;

		for (int i = 0; i < size; ++i) {
			if (arr[i] != 0) {
				iptr_buff[i] = i;
				++_num_of_aelem;
			}
		}

		_aelem = new T[_num_of_aelem];
		_iptr = new int[_num_of_aelem];

		for (int i = 0; i < _num_of_aelem; ++i) {
			_iptr[i] = iptr_buff[i];
			_aelem[i] = arr[_iptr[i]];
		}

		delete[] iptr_buff;
	}

	/**
	 * @brief Copies the data of SparseVector from
	 * other SparseVector
	 * 
	 * @param other Reference to other SparseVector
	 */
	SparseVector(const SparseVector &other)
	{
		this->_size = other._size;
		_num_of_aelem = other._num_of_aelem;

		_aelem = new T[_num_of_aelem];
		_iptr = new int[_num_of_aelem];

		for (int i = 0; i < _num_of_aelem; ++i) {
			_aelem[i] = other._aelem[i];
			_iptr[i] = other._iptr[i];
		}
	}

	/**
	 * @brief Deletes an instance of SparseVector
	 */
	~SparseVector()
	{
		delete[] _aelem;
		delete[] _iptr;
	}

	/**
	 * @brief Assignes an instance of SparseVector with
	 * another SparseVector.
	 * @details Copies all the data from other SparseVector
	 * 
	 * @param other Reference to other SparseVector
	 */
	SparseVector& operator= (const SparseVector &other)
	{
		this->_size = other._size;
		_num_of_aelem = other._num_of_aelem;

		_aelem = new T[_num_of_aelem];
		_iptr = new int[_num_of_aelem];

		for (int i = 0; i < _num_of_aelem; ++i) {
			_aelem[i] = other._aelem[i];
			_iptr[i] = other._iptr[i];
		}

		return *this;
	}

	/**
	 * @brief Gets aelem - an array of nonempty elements
	 * @return aelem array
	 */
	T* aelem() const
	{
		return _aelem;
	}

	/**
	 * @brief Gets iptr - an array of indices of all the
	 * nonempty elements
	 * @return iptr array
	 */
	int* iptr() const
	{
		return _iptr;
	}

	/**
	 * @brief Gets number of nonempty elements
	 * @return Number of nonempty elements
	 */
	int num_of_aelem() const
	{
		return _num_of_aelem;
	}

	/**
	 * @brief Gets and sets the element by it's position
	 * in SparseVector
	 * @details This operator does not check if the index
	 * is out of range. Therefore it's less time-expensive
	 * but unsafe.
	 * 
	 * @param index Position in SparseVector
	 */
	T& operator[] (int index)
	{
		for (int i = 0; i < _num_of_aelem; ++i) {
			if (_iptr[i] == index) {
				return _aelem[i];
			}
			else if (_iptr[i] > index) {
				// TODO: Find a way to return 0 in this case
				//       but throw an error if out of range
				//       without explicitly checking
				throw "No such element";
			}
		}
		throw "No such element";
	}

	//TODO: Improve documentation

	/**
	 * @brief Gets and sets the element by it's position
	 * in Vector
	 * @details This method checks if the index
	 * is out of range. Therefore it's more time-expensive
	 * but safe.
	 * 
	 * @param index Position in Vector
	 */
	T& at(int index)
	{
		if (index < 0 || index >= this->_size) {
			throw VecOutOfRange();
		}

		return _arr[index];
	}

	//TODO: Improve documentation

	/**
	 * @brief Gets the element by it's position in Vector
	 * @details This method does not check if the index
	 * is out of range. Therefore it's less time-expensive
	 * but unsafe. Being a const-method it is guaranteed
	 * not to modify the data inside the vector.
	 * 
	 * @param index Position in Vector
	 */
	T get(int index) const
	{
		return _arr[index];
	}

	/**
	 * @brief Inserts an element to Vector
	 * @details The space for this element should
	 * already be allocated by Vector(int)
	 * constructor.
	 * 
	 * @param val Value to be inserted
	 * @param i Position in Vector
	 */
	void insert(const T &val, int index)
	{
		_arr[index] = val;
	}

	/**
	 * @brief Implements the elementwise sume of
	 * two Vector-s.
	 * 
	 * @param other Second vector (it will be added
	 * to this one)
	 * @return Sume of two vectors
	 */
	Vector operator+ (const Vector &other)
	{
		if (this->_size != other._size) {
			throw VecSizeMismatch();
		}

		Vector res(this->_size);

		for (int i = 0; i < this->_size; ++i) {
			res._arr[i] = _arr[i] + other._arr[i];
		}

		return res;
	}

	/**
	 * @brief Implements the elementwise difference of
	 * two Vector-s.
	 * 
	 * @param other Second vector (it will be substracted
	 * from this one)
	 * @return Difference of two vectors
	 */
	Vector operator- (const Vector &other)
	{
		if (this->_size != other._size) {
			throw VecSizeMismatch();
		}

		Vector res(this->_size);

		for (int i = 0; i < this->_size; ++i) {
			res._arr[i] = _arr[i] - other._arr[i];
		}

		return res;
	}

	/**
	 * @brief Implements multiplication of vector
	 * on given scalar value.
	 * @details Each element of vector is multiplied
	 * by scalar value.
	 * 
	 * @param val Scalar value
	 * @return Vector-product
	 */
	template <typename S>
	Vector operator* (const S &val) const
	{
		Vector res(this->_size);

		for (int i = 0; i < this->_size; ++i) {
			res._arr[i] = _arr[i] * val;
		}

		return res;
	}
	
	/**
	 * @brief Implements division of vector by given
	 * scalar value.
	 * @details Each element of vector is divided
	 * by scalar value.
	 * 
	 * @param val Scalar value
	 * @return Vector-division
	 */
	template <typename S>
	Vector operator/ (const S &val) const
	{
		if (val == 0) {
			throw DivideByZero();
		}

		Vector<T> res(this->_size);

		for (int i = 0; i < this->_size; ++i) {
			res._arr[i] = _arr[i] / val;
		}

		return res;
	}
};

/**
 * @brief Implements multiplication of scalar value
 * on Vector.
 * @details Makes cector-scalar multiplication
 * commutative.
 * 
 * @param val Scalar value
 * @param vec Vector
 * @tparam T Type of data stored in Vector
 * @tparam S Type of scalar value
 * @return Vector-product
 */
template <typename T, typename S>
Vector<T> operator* (const S &val, const Vector<T> &vec)
{
	return vec * val;
}

#endif // SPARSEVECTOR_H