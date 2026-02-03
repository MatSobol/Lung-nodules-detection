#include <glad/glad.h>

#include "cut.hpp"
#include "../const.hpp"
#include <iostream>

Cut::Cut(unsigned int shaderProgram, std::array<float, 3UL> maxValues)
{
    verticalCutLoc = glGetUniformLocation(shaderProgram, "verticalCut");
    horizontalCutLoc = glGetUniformLocation(shaderProgram, "horizontalCut");
    horizontalCut = maxValues[0] / 2 + 1.0f;
    verticalCut = maxValues[1] / 2 + 1.0f;
}

void Cut::handleInput(GLFWwindow *window, float deltaTime)
{
    float cutSpeedFinal = cutSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        verticalCut += cutSpeedFinal;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        verticalCut -= cutSpeedFinal;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        horizontalCut += cutSpeedFinal;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        horizontalCut -= cutSpeedFinal;
}

void Cut::update()
{
    glUniform1f(verticalCutLoc, verticalCut);
    glUniform1f(horizontalCutLoc, horizontalCut);
}