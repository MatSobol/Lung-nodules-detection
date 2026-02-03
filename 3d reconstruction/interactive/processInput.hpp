#pragma once

#include <GLFW/glfw3.h>
#include <vector>

#include "interactive.hpp"

void processInput(GLFWwindow *window, float deltaTime, std::vector<IInteractive *> &inputHandlers);