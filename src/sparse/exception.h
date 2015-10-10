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

#endif // EXCEPTION_H