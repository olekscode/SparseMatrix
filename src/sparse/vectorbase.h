#ifndef VECTORBASE_H
#define VECTORBASE_H

// TODO: Find out how to create the interface for operators
// 		 e.g. virtual operators (?)

template <typename T>
class VectorBase
{
protected:
	int _size;

public:
	VectorBase() {}
	VectorBase(int size)
		: _size(size) {}

	virtual ~VectorBase() {}

	int size() const
	{
		return _size;
	}

	virtual T& operator[] (int index) = 0;
	virtual T& at(int index) = 0;

	// Not sure if it's a good idea
	virtual T get(int index) const = 0;

	virtual void insert(const T &val, int index) = 0;
};

#endif // VECTORBASE_H