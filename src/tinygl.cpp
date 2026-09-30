#include "../third_party/glad/glad.h"
#include "../include/tinygl/tinygl.h"
#include <glfw/glfw3.h>
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <thread>
#include <vector>
#include <map>

namespace TinyGL
{

int width;
int height;

std::map<int, bool> keypressed;
std::vector<uint32> screenBuffer;

GLFWwindow* window = nullptr;
int windowWidth = 0;
int windowHeight = 0;
int windowedPosX = 0;
int windowedPosY = 0;
bool mouseLocked = false;
bool isFullscreen = false;
double lastMouseX = 0.0;
double lastMouseY = 0.0;
GLuint quadVAO, quadVBO, quadEBO;
GLuint shaderProgram;

/*
packing function for packing ColorRGB
into a 32bit int format for GL_RGBA
*/
inline uint32 packColor(const ColorRGB& color)
{
    return (255 << 24) | (color.b << 16) | (color.g << 8) | color.r;
}

inline ColorRGB unpackColor(uint32 colorINT)
{
    return ColorRGB(colorINT & 0xFF, (colorINT >> 8) & 0xFF, (colorINT >> 16) & 0xFF);
}

bool keyDown(int key)
{
    if (!window) return false;
    return glfwGetKey(window, key) == GLFW_PRESS;
}

bool keyPressed(int key)
{
    if (!window) return false;
    bool isDown = (glfwGetKey(window, key) == GLFW_PRESS);

    if (keypressed.find(key) == keypressed.end()) keypressed[key] = false;

    if (isDown)
    {
        if (!keypressed[key])
        {
            keypressed[key] = true;
            return true;
        }
    }
    else
    {
        keypressed[key] = false;
    }
    return false;
}

void setMouseLocked(bool locked)
{
    if (!window) return;
    mouseLocked = locked;
    if (locked)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if(glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
    }
    else
    {
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_FALSE);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

void getMouseDelta(double& dx, double &dy)
{
    dx = 0.0;
    dy = 0.0;
    if(!window || !mouseLocked) return;

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    dx = x - lastMouseX;
    dy = y - lastMouseY;
    lastMouseX = x;
    lastMouseY = y;
}

void getMouseState(int& mouseX, int& mouseY)
{
    mouseX = mouseY = 0;
    if(!window) return;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    mouseX = static_cast<int>(x);
    mouseY = static_cast<int>(y);
}


void createWindow(int width, int height, std::string& title, GLFWmonitor* monitor, bool fullscreen)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << '\n';
        std::exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << '\n';
        glfwDestroyWindow(window);
        window = nullptr;
        glfwTerminate();
        std::exit(1);
    }

    monitor = fullscreen ? glfwGetPrimaryMonitor() : nullptr;
    window = glfwCreateWindow(width, height, title.c_str(), monitor, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        std::exit(1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);
    glfwPollEvents();

    glViewport(0, 0, width, height);
    screenBuffer.resize(width * height, packColor(RGB_Black));
}

void toggleFullscreen(bool fullscreen)
{
    if (!window || fullscreen == isFullscreen) return;

    if (fullscreen)
    {
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if(!monitor) return;

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if(!mode) return;

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        isFullscreen = true;
    }
    else
    {
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, width, height, GLFW_DONT_CARE);
        isFullscreen = false;
    }

    glViewport(0, 0, width, height);
}

void clearScreen(const ColorRGB& color)
{
    uint32 packed = packColor(color);
    std::fill(screenBuffer.begin(), screenBuffer.end(), packed);
}

void pset(int x, int y, const ColorRGB& color)
{
    if (x < 0 || y < 0 || x >= width || y >= height) return;
    screenBuffer[y * width + x] = packColor(color);
}

ColorRGB pget(int x, int y)
{
    if(x < 0 || y < 0 || x >= width || y >= height) return RGB_Black;
    return unpackColor(screenBuffer[y * width + x]);
}

void sleep()
{
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void drawBuffer(uint32 *buffer)
{
    if(!buffer) return;
    memcpy(screenBuffer.data(), buffer, screenBuffer.size() * sizeof(uint32));
}

bool onScreen(int x, int y)
{
    return (x >= 0 && y >= 0 && x < width && y < height);
}

unsigned long getTicks()
{
    return static_cast<unsigned long>(glfwGetTime() * 1000.0);
}

bool horLine(int y,  int x1, int x2, const ColorRGB& color)
{
    if (x2 < x1) { x1 += x2; x2 = x1 - x2; x1 -= x2; }
    if(x2 < 0 || x1 >= width || y < 0 || y >= height) return 0; 
    if(x1 < 0) x1 = 0; 
    if(x2 >= width) x2 = width - 1;

    uint32 c = packColor(color);
    int offset = y * width;
    for(int x = x1; x <= x2; x++)
    {
        screenBuffer[offset + x] = c;
    }
    return 1;
}

bool verLine(int x, int y1, int y2, const ColorRGB& color)
{
    if(y2 < y1) {y1 += y2; y2 = y1 - y2; y1 -= y2;} 
    if(y2 < 0 || y1 >= height || x < 0 || x >= width) return 0; 
    if(y1 < 0) y1 = 0; 
    if(y2 >= height) y2 = height - 1; 

    uint32 c = packColor(color);
    for(int y = y1; y <= y2; y++)
    {
        screenBuffer[y * width + x] = c;
    }
    return 1;
}

}