#pragma once

#include <GLFW/glfw3.h>

#include "interactive.hpp"

class Camera : public IInteractive
{
private:
    float yaw;
    float pitch;
    float radius;
    double lastX;
    double lastY;

    unsigned int viewLoc;
    unsigned int viewPosLoc;
    unsigned int projLoc;
    unsigned int lightPosLoc;
    unsigned int lightColorLoc;

public:
    Camera(GLFWwindow *window, unsigned int shaderProgram, float radius);
    virtual void handleInput(GLFWwindow *window, float deltaTime);
    virtual void update();
};