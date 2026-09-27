#include "glad.h"
#include "tinygl.h"
#include <glfw/glfw3.h>
#include <iostream>
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

GLFWwindow* createWindow(int width, int height, std::string& title, GLFWmonitor* monitor, bool fullscreen)
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
    // TODO: fix error
    // screenBuffer.resize(width * height, RGB_Black);
}

}