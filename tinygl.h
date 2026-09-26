#ifndef TINYGL_H
#define TINYGL_H

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

}

#endif