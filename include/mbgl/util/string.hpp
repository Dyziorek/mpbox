#ifndef MBGL_UTIL_STRING
#define MBGL_UTIL_STRING

#include <string>

#ifndef _MSC_VER
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#endif
#include <boost/lexical_cast.hpp>
#ifndef _MSC_VER
#pragma GCC diagnostic pop
#endif

namespace mbgl {
namespace util {

template <typename... Args>
inline std::string toString(Args&&... args) {
    return boost::lexical_cast<std::string>(::std::forward<Args>(args)...);
}

// boost::lexical_cast() treats this as a character, but we are using it as number types.
inline std::string toString(int8_t num) {
    return boost::lexical_cast<std::string>(int(num));
}


template<size_t max, typename... Args>
inline std::string sprintf(const char *msg, Args... args) {
    char res[max];
    int len = snprintf(res, sizeof(res), msg, args...);
    return std::string(res, len);
}

template<size_t max, typename... Args>
inline std::string sprintf(const std::string &msg, Args... args) {
    return sprintf<max>(msg.c_str(), args...);
}

}
}

#endif
