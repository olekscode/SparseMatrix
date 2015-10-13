#ifndef VECTOR_H
#define VECTOR_H

#include "vectorbase.h"
#include "exception.h"

/**
 * @brief Reimplemented mathematical vector
 * @details Provides basic arithmetic operators
 * 
 * @tparam T Type of data stored in vector 
 */
template <typename T>
class Vector : public VectorBase<T>
{
	T *_arr;

public:
	/**
	 * @brief Creates an instance of empty Vector
	 * @details Allocates memory for given number
	 * of elements. They must be inserted later.
	 * 
	 * @param size Number of elements
	 */
	Vector(int size)
		: VectorBase<T>(size)
	{
		_arr = new T[size];
	}

	/**
	 * @brief Creates an instance of Vector
	 * @details The elements are coppied from given
	 * plain array
	 * 
	 * @param arr Plain array
	 * @param size Number of elements
	 */
	Vector(T *arr, int size)
		: VectorBase<T>(size)
	{
		_arr = new T[size];

		for (int i = 0; i < size; ++i) {
			_arr[i] = arr[i];
		}
	}

	/**
	 * @brief Copies the data of Vector from
	 * other Vector
	 * 
	 * @param other Reference to other Vector
	 */
	Vector(const Vector &other)
	{
		this->_size = other._size;
		_arr = new T[this->_size];

		for (int i = 0; i < this->_size; ++i) {
			_arr[i] = other._arr[i];
		}
	}

	/**
	 * @brief Deletes an instance of Vector
	 */
	~Vector()
	{
		delete[] _arr;
	}

	/**
	 * @brief Assignes an instance of Vector with
	 * another Vector.
	 * @details Copies all the data from other Vector
	 * 
	 * @param other Reference to other Vector
	 */
	Vector& operator= (const Vector &other)
	{
		this->_size = other._size;
		_arr = new T[this->_size];

		for (int i = 0; i < this->_size; ++i) {
			_arr[i] = other._arr[i];
		}

		return *this;
	}

	/**
	 * @brief Gets and sets the element by it's position
	 * in Vector
	 * @details This operator does not check if the index
	 * is out of range. Therefore it's less time-expensive
	 * but unsafe.
	 * 
	 * @param index Position in Vector
	 */
	T& operator[] (int index)
	{
		return _arr[index];
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

#endif // VECTOR_H