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


}