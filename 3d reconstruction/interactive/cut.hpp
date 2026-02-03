#pragma once

#include <GLFW/glfw3.h>
#include <array>

#include "interactive.hpp"

class Cut : public IInteractive
{
private:
    unsigned int verticalCutLoc;
    unsigned int horizontalCutLoc;
    float verticalCut;
    float horizontalCut;

public:
    Cut(unsigned int shaderProgram, std::array<float, 3UL> maxValues);
    virtual void handleInput(GLFWwindow *window, float deltaTime);
    virtual void update();
};