#pragma once

#include <GLFW/glfw3.h>

class IInteractive
{
public:
    virtual ~IInteractive() = default;

    virtual void handleInput(GLFWwindow *window, float deltaTime) = 0;

    virtual void update() = 0;
};