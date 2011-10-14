#ifndef OSI2_STREAMING_EXCEPTION
#define OSI2_STREAMING_EXCEPTION

#include "../Osi2/config.h"
#if defined(HAVE_CSTDINT)
# include <cstdint>
#elif defined(HAVE_STDINT_H)
# include <stdint.h>
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <stdexcept>

class StreamingException : public std::runtime_error {
public:
    StreamingException(const std::string filename = "", uint32_t = 0) :
        std::runtime_error(""),
        ss_(std::auto_ptr<std::stringstream>
            (new std::stringstream())) {
    }

    ~StreamingException() throw() {
    }

    template <typename T>
    StreamingException & operator << (const T & t) {
        (*ss_) << t;
        return *this;
    }

    virtual const char * what() const throw() {
        s_ = ss_->str();
        return s_.c_str();
    }

public: // fields
    std::string  filename_;
    uint32_t line_;

private:
    mutable std::auto_ptr<std::stringstream> ss_;
    mutable std::string s_;
};

#endif		// OSI2_STREAMING_EXCEPTION


