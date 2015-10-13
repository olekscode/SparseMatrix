#ifndef VECTOR_H
#define VECTOR_H

#include "vectorbase.h"
#include "exception.h"

template <typename T>
class Vector : public VectorBase<T>
{
	T *_arr;

public:
	Vector(int size)
		: VectorBase<T>(size)
	{
		_arr = new T[size];
	}

	Vector(T *arr, int size)
		: VectorBase<T>(size)
	{
		_arr = new T[size];

		for (int i = 0; i < size; ++i) {
			_arr[i] = arr[i];
		}
	}

	Vector(const Vector &other)
	{
		this->_size = other._size;
		_arr = new T[this->_size];

		for (int i = 0; i < this->_size; ++i) {
			_arr[i] = other._arr[i];
		}
	}

	~Vector()
	{
		delete[] _arr;
	}

	Vector& operator= (const Vector &other)
	{
		this->_size = other._size;
		_arr = new T[this->_size];

		for (int i = 0; i < this->_size; ++i) {
			_arr[i] = other._arr[i];
		}

		return *this;
	}

	T& operator[] (int index)
	{
		return _arr[index];
	}

	T& at(int index)
	{
		if (index < 0 || index >= this->_size) {
			throw VecOutOfRange();
		}

		return _arr[index];
	}

	T get(int index) const
	{
		return _arr[index];
	}

	void insert(const T &val, int index)
	{
		_arr[index] = val;
	}

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


	template <typename>
	friend Vector operator* (const Vector &vec, const T &val);
	
	template <typename>
	friend Vector operator/ (const Vector &vec, const T &val);
};

template <typename T>
Vector<T> operator* (const Vector<T> &vec, const T &val)
{
	Vector<T> res(vec._size);

	for (int i = 0; i < vec._size; ++i) {
		res._arr[i] = vec._arr[i] * val;
	}

	return res;
}

template <typename T>
Vector<T> operator/ (const Vector<T> &vec, const T &val)
{
	if (val == 0) {
		throw DivideByZero();
	}

	Vector<T> res(vec._size);

	for (int i = 0; i < vec._size; ++i) {
		res._arr[i] = vec._arr[i] / val;
	}

	return res;
}

#endif // VECTOR_H