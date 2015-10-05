#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

class MultSizeMismatch : public std::exception
{
public:
	const char* what() const throw()
	{
		return "Cannot multiply: row-size of first matrix and " \
			   "col-size of second-one do not match.";
	}
};

#endif // EXCEPTION_H