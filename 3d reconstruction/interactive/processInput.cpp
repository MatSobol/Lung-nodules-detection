#include <iostream>

#include "processInput.hpp"

void processInput(GLFWwindow *window, float deltaTime, std::vector<IInteractive *> &inputHandlers)
{
    for (auto &handler : inputHandlers)
    {
        handler->handleInput(window, deltaTime);
    }
}