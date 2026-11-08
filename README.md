# TinyGL

A lightweight, cross platform C++ graphics library built on top of OpenGL and GLFW.

## Overview
TinyGL is a lightweight C++ library designed to simplify graphics programming with OpenGL and GLFW.

The idea behind TinyGL is to provide a straightforward abstraction over common graphics operations, so developers can focus more on building their applications and less on handling the underlying setup.

Rather than being limited to window creation, TinyGL also provides utilities for drawing, text rendering, input handling, image loading, shaders, and other common operations.

The project is written in C++17 and aims to keep its API simple, accessible, and easy to work with.

**TinyGL is currently under development.**

## Requirements

- C++17 compatible compiler
- CMake 3.20 or newer
- GLFW 3
- OpenGL 3.3 compatible graphics support

## Building

TinyGL uses CMake and supports both shared and static library builds.

### Dependencies

Make sure GLFW is installed and its CMake package configuration is available to CMake.

### Clone the repository

```bash
git clone https://github.com/lichtkor/tinygl.git
cd tinygl
```

### Configure

```bash
cmake -B build -S .
```

### Build

```bash
cmake --build build --config Release
```

The compiled libraries are placed in:

```text
build/
└── dist/
    ├── bin/
    └── lib/
```

### Build options

TinyGL supports configuring shared and static builds independently.

For example, to build only the static library:

```bash
cmake -B build -S . -DTINYGL_BUILD_SHARED=OFF
cmake --build build --config Release
```

To build only the shared library:

```bash
cmake -B build -S . -DTINYGL_BUILD_STATIC=OFF
cmake --build build --config Release
```

## Usage

Include the TinyGL header in your C++ source file:

```cpp
#include <tinygl/tinygl.h>
```

TinyGL provides its functionality through the `TinyGL` namespace.

For example its API includes:

```cpp
TinyGL::ColorRGB color(255, 100, 50);

TinyGL::drawLine(10, 10, 100, 100, color);
TinyGL::drawCircle(100, 100, 30, color);
TinyGL::drawRect(20, 20, 120, 80, color);
```

These drawing functions are part of TinyGL's 2D graphics utilities.

## Dependencies

TinyGL uses the following libraries and components:

- [GLFW](https://www.glfw.org/) for window creation and input handling.
- [GLAD](https://glad.dav1d.de/) for loading OpenGL functions.
- [stb_image](https://github.com/nothings/stb) for image-related functionality.

If you find a bug, have a suggestion, or want to improve TinyGL, feel free to open an issue or submit a pull request.
