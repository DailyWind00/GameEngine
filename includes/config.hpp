#pragma once

/// Defines
# define COLOR_HEADER_CXX

/// System includes
# include <iostream>
# include <glad/glad.h>
# include <GLFW/glfw3.h>
# include <fstream>
# include <string.h>

/// Custom includes
# include "color.h"

/// Global variables
using namespace std;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern bool VERBOSE;

/// Functions
// window.cpp
GLFWwindow *CreateWindow();
void		DestroyWindow(GLFWwindow *window);

// utils.cpp
void    printVerbose(const string &message, bool newline = true);