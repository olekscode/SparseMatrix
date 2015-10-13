#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <sstream>

/**
 * @brief Exception that is thrown when trying to multiply the
 * CSR or CSIR matrix on vector of the invalid size
 */
class MultSizeMismatch : public std::exception
{
public:
    const char* what() const throw()
    {
        return "Cannot multiply: row-size of first matrix and " \
               "col-size of second-one do not match.";
    }
};

/**
 * @brief Exception that is thrown when trying to insert the
 * nonexisting element CSR or CSIR matrix
 */
class InsertNoSuchElement : public std::exception
{
	int _i, _j;

public:
	InsertNoSuchElement(int i, int j)
		: _i(i), _j(j)
	{
	}

	const char* what() const throw()
	{
		std::ostringstream osstrm;
		osstrm << "Cannot insert: this matrix was not meant " \
			  	  "to have a nonempty element at ("
			   << _i << ", " << _j << ")";
		return osstrm.str().c_str();
	}
};

/**
 * @brief Exception that is thrown when Vector or SparseVector
 * gets out of range
 */
class VecOutOfRange : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Vector got out of range";
	}
};

/**
 * @brief Exception that is thrown when trying to apply an
 * arithmetic operation to Vector-s or SparseVector-s of
 * unsuitable sizes
 */
class VecSizeMismatch : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Cannot apply arithmetic operations: sizes of " \
			   "operands don't match";
	}
};

/**
 * @brief Exception that is thrown when trying to divide by zero
 */
class DivideByZero : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Division by zero";
	}
};

#endif // EXCEPTION_H