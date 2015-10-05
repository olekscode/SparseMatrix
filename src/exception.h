#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>

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

#endif // EXCEPTION_H