#ifndef TINYGL_H
#define TINYGL_H

#include <iomanip>
#include <string>
#include <sstream>
#include <cstdint>

namespace TinyGL
{

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
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

struct ColorRGB
{
    uint8 r{0};
    uint8 g{0};
    uint8 b{0};

    constexpr ColorRGB() = default;
    constexpr ColorRGB(uint8 r, uint8 g, uint8 b) 
        : r(r), g(g), b(b) {}

    constexpr ColorRGB(uint32 hex)
        : r(static_cast<uint8>((hex >> 16) & 0xFF)),
          g(static_cast<uint8>((hex >> 8) & 0xFF)),
          b(static_cast<uint8>(hex & 0xFF)) {}
};

constexpr ColorRGB operator+(const ColorRGB& c1, const ColorRGB& c2) {
    return ColorRGB(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}

constexpr ColorRGB operator-(const ColorRGB& c1, const ColorRGB& c2) {
    return ColorRGB(c1.r - c2.r, c1.g - c2.g, c1.b - c2.b);
}

constexpr ColorRGB operator*(const ColorRGB& c, int a) {
    return ColorRGB(c.r * a, c.g * a, c.b * a);
}

constexpr ColorRGB operator*(int a, const ColorRGB& c) {
    return c * a;
}

constexpr ColorRGB operator/(const ColorRGB& c, int a) {
    return ColorRGB(c.r / a, c.g / a, c.b / a);
}

constexpr bool operator==(const ColorRGB& c1, const ColorRGB& c2) {
    return c1.r == c2.r && c1.g == c2.g && c1.b == c2.b;
}

constexpr bool operator!=(const ColorRGB& c1, const ColorRGB& c2) {
    return !(c1 == c2);
}

namespace Colors 
{
    inline constexpr ColorRGB Red      (255,   0,   0);
    inline constexpr ColorRGB Green    (  0, 255,   0);
    inline constexpr ColorRGB Blue     (  0,   0, 255);
    inline constexpr ColorRGB Black    (  0,   0,   0);
    inline constexpr ColorRGB White    (255, 255, 255);
    inline constexpr ColorRGB Yellow   (255, 255,   0);
    inline constexpr ColorRGB Cyan     (  0, 255, 255);
    inline constexpr ColorRGB Magenta  (255,   0, 255);
    inline constexpr ColorRGB Gray     (128, 128, 128);

    inline constexpr ColorRGB HEX_Red    (0xFF0000);
    inline constexpr ColorRGB HEX_Green  (0x00FF00);
    inline constexpr ColorRGB HEX_Blue   (0x0000FF);
    inline constexpr ColorRGB HEX_Black  (0x000000);
    inline constexpr ColorRGB HEX_White  (0xFFFFFF);
    inline constexpr ColorRGB HEX_Yellow (0xFFFF00);
}

}

#endif