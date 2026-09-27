#ifndef TINYGL_H
#define TINYGL_H

#include <iomanip>
#include <string>
#include <sstream>
#include <cstdint>

namespace TinyGL
{

using uint8 = std::uint8_t;
using uint32 = std::uint32_t;

/*
I decided to re implement the abs function because
it tends to not be working properly with some types
*/
template<typename T>
const T template_abs(const T &a)
{
    return (a < 0) ? -a : a;
}

template<typename T>
std::string valtostr(const T& val)
{
    std::ostringstream sstream;
    sstream << val;
    return sstream.str();
}

template<typename T>
T strtoval(const std::string& s)
{
    std::istringstream sstream(s);
    T val;
    sstream >> val;
    return val;
}

template<typename T>
std::string valtostr(const T& val, int length, bool fixed = true)
{
    std::ostringstream sstream;
    if(fixed) sstream << std::fixed;
    sstream << std::setprecision(length) << val;
    return sstream.str();
}

}

#endif