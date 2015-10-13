#ifndef VECTOR_H
#define VECTOR_H

#include "vectorbase.h"

template <typename T>
class Vector : public VectorBase<T>
{
	T *_arr;

public:
	Vector(int size)
		: VectorBase<T>(size)
	{
		_arr = new T[_size];
	}

	Vector(T *arr, int size)
		: VectorBase<T>(size)
	{
		_arr = new T[_size];

		for (int i = 0; i < _size; ++i) {
			_arr[i] = arr[i];
		}
	}

	Vector(const Vector &other)
	{
		_size = other._size;
		_arr = new T[_size];

		for (int i = 0; i < _size; ++i) {
			_arr[i] = other._arr[i];
		}
	}

	~Vector()
	{
		delete[] _arr;
	}

	Vector& operator= (const Vector &other)
	{
		_size = other._size;
		_arr = new T[_size];

		for (int i = 0; i < _size; ++i) {
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
		if (index < 0 || index >= _size) {
			// TODO: Create an exception
			throw "OutOfRange";
		}

		return _arr[index];
	}

	void insert(const T &val, int index)
	{
		_arr[index] = val;
	}

	Vector operator+ (const Vector &other)
	{
		if (_size != other._size) {
			// TODO: Create an exception
			throw "SizesMismatch";
		}

		Vector res(_size);

		for (int i = 0; i < _size; ++i) {
			res._arr[i] = _arr[i] + other._arr[i];
		}

		return res;
	}

	Vector operator- (const Vector &other)
	{
		if (_size != other._size) {
			// TODO: Create an exception
			throw "SizesMismatch";
		}

		Vector res(_size);

		for (int i = 0; i < _size; ++i) {
			res._arr[i] = _arr[i] - other._arr[i];
		}

		return res;
	}

	friend Vector operator* (const Vector &vec, const T &val);
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
		// TODO: Create an exception
		throw "DivideByZero";
	}

	Vector<T> res(vec._size);

	for (int i = 0; i < vec._size; ++i) {
		res._arr[i] = vec._arr[i] / val;
	}

	return res;
}

#endif // VECTOR_H