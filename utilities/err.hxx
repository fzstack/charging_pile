#ifndef UTILITIES_ERR_HXX_
#define UTILITIES_ERR_HXX_

#include <stdexcept>

class not_implemented: public std::runtime_error {
    using std::runtime_error::runtime_error;
public:
    not_implemented(): std::runtime_error("not implemented") {}
};

class timeout_error: virtual public std::runtime_error {
    using std::runtime_error::runtime_error;
};



#endif /* UTILITIES_ERR_HXX_ */
