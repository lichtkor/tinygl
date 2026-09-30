#ifndef TINYGL_H
#define TINYGL_H

#include <glfw/glfw3.h>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <cstdint>

namespace TinyGL
{

using uint8 = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using size = std::size_t;

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

struct ColorRGB8bit;

struct ColorRGB
{
  int r;
  int g;
  int b;

  ColorRGB(uint8 r, uint8 g, uint8 b);
  ColorRGB(const ColorRGB8bit& color);
  ColorRGB();
};

ColorRGB operator+(const ColorRGB& color, const ColorRGB& color2);
ColorRGB operator-(const ColorRGB& color, const ColorRGB& color2);
ColorRGB operator*(const ColorRGB& color, int a);
ColorRGB operator*(int a, const ColorRGB& color);
ColorRGB operator/(const ColorRGB& color, int a);
bool operator==(const ColorRGB& color, const ColorRGB& color2);
bool operator!=(const ColorRGB& color, const ColorRGB& color2);

static const ColorRGB RGB_Black    (  0,   0,   0);
static const ColorRGB RGB_Red      (255,   0,   0);
static const ColorRGB RGB_Green    (  0, 255,   0);
static const ColorRGB RGB_Blue     (  0,   0, 255);
static const ColorRGB RGB_Cyan     (  0, 255, 255);
static const ColorRGB RGB_Magenta  (255,   0, 255);
static const ColorRGB RGB_Yellow   (255, 255,   0);
static const ColorRGB RGB_White    (255, 255, 255);
static const ColorRGB RGB_Gray     (128, 128, 128);
static const ColorRGB RGB_Grey     (192, 192, 192);
static const ColorRGB RGB_Maroon   (128,   0,   0);
static const ColorRGB RGB_Darkgreen(  0, 128,   0);
static const ColorRGB RGB_Navy     (  0,   0, 128);
static const ColorRGB RGB_Teal     (  0, 128, 128);
static const ColorRGB RGB_Purple   (128,   0, 128);
static const ColorRGB RGB_Olive    (128, 128,   0);

struct ColorRGB8bit
{
  uint8 r;
  uint8 g;
  uint8 b;

  ColorRGB8bit(uint8 r, uint8 g, uint8 b);
  ColorRGB8bit(const ColorRGB& color);
  ColorRGB8bit();
};

extern int width;
extern int height;

bool keyDown(int key);
bool keyPressed(int key);
bool keyReleased(int key);

void createWindow(int width, int height, std::string& title, GLFWmonitor* monitor, bool fullscreen = false);
void toggleFullScreen(bool fullscreen);
void windowSizeCallback(GLFWwindow* window, int width, int height);
void setWindowCallback(GLFWwindow* window);
void render();
void clearScreen(const ColorRGB& color = RGB_Black);
void pset(int x, int y, const ColorRGB& color);
ColorRGB pget(int x, int y);
void drawBuffer(uint32* buffer);
bool onScreen(int x, int y);

void sleep(double milliseconds);
void waitFrame(double oldTime, double frameDuration);
bool windowIsOpen(bool running);
void exit();

void setMouseLocked(bool locked);
void getMouseDelta(double &dx, double &dy);
void getMousePosition(int& mouseX, int& mouseY);
void getMousePosition(int& mouseX, int& mouseY, bool& LMB, bool& RMB);

unsigned long getTicks();
inline double getTime() { return getTicks() / 1000.0; }

bool horLine(int y, int x1, int x2, const ColorRGB& color);
bool verLine(int x, int y1, int y2, const ColorRGB& color);
bool drawLine(int x1, int y1, int x2, int y2, const ColorRGB& color);
bool drawCircle(int xc, int yc, int radius, const ColorRGB& color);
bool drawDisk(int xc, int yc, int radius, const ColorRGB& color);
bool drawRect(int x1, int y1, int x2, int y2, const ColorRGB& color);
bool clipLine(int x1, int y1, int x2, int y2, int &x3, int &y3, int &x4, int &y4);

uint32 RGBtoINT(const ColorRGB& color);
ColorRGB INTtoRGB(uint32 colorINT);

int loadImage(std::vector<ColorRGB>& out, unsigned long& w, unsigned long& h, const std::string& filename);
int loadImage(std::vector<uint32>& out, unsigned long& w, unsigned long& h, const std::string& filename);
int decodePNG(std::vector<unsigned char>& out_image, unsigned long& image_width, unsigned long& image_height, const unsigned char* in_png, size in_size, bool convert_to_rgba32 = true);
int decodePNG(std::vector<unsigned char>& out_image_32bit, unsigned long& image_width, unsigned long& image_height, const std::vector<unsigned char>& in_png);

extern bool font[256][8][8];
void drawLetter(unsigned char n, int x, int y, const ColorRGB& color = RGB_White, bool bg = false, const ColorRGB& color2 = RGB_Black);
int printString(const std::string& text, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = false, const ColorRGB& color2 = RGB_Black, int forceLength = 0);

template<typename T>
int print(const T& val, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = false, const ColorRGB& color2 = RGB_Black, int forceLength = 0)
{
    std::string text = valtostr(val);
    return printString(text, x, y, color, bg, color2, forceLength);
}

template<typename T>
int fprint(const T& val, int length, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = false, const ColorRGB& color2 = RGB_Black, int forceLength = 0)
{
    std::string text = valtpstr(val);
    return printString(text, x, y, color, bg, color2, forceLength);
}

uint8 getInputCharacter();
void getInputString(std::string& text, const std::string& message = "", bool clear = false, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = false, const ColorRGB& color2 = RGB_Black);

template<typename T>
T getInput(const std::string& message = "", bool clear = false, int x = 0, int y = 0, const ColorRGB& color = RGB_White, bool bg = false, const ColorRGB& color2 = RGB_Black)
{
    std::string text;
    getInputString(text, message, clear, x, y, color, bg, color2);
    return strtoval<T>(text);
}

}

#endif