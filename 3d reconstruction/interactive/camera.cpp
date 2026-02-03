#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#include "camera.hpp"
#include "../const.hpp"

const glm::vec3 target = {0.0f, 0.0f, 0.0f};

float fov = 60.0f;

void framebufferSizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 60.0f)
        fov = 60.0f;
}

Camera::Camera(GLFWwindow *window, unsigned int shaderProgram, float radius)
    : yaw(0.0f), pitch(0.0f)
{
    this->radius = radius;
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    viewLoc = glGetUniformLocation(shaderProgram, "view");
    projLoc = glGetUniformLocation(shaderProgram, "projection");
    viewPosLoc = glGetUniformLocation(shaderProgram, "viewPos");
    lightPosLoc = glGetUniformLocation(shaderProgram, "lightPos");
    lightColorLoc = glGetUniformLocation(shaderProgram, "lightColor");
}

bool lastLeftClickState = false;

void Camera::handleInput(GLFWwindow *window, float deltaTime)
{
    float cameraSpeedFinal = cameraSpeed * deltaTime;

    bool leftClick = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (leftClick && !lastLeftClickState)
    {
        lastX = xpos;
        lastY = ypos;
        glfwSetCursor(window, glfwCreateStandardCursor(GLFW_HAND_CURSOR));
    }

    if (!leftClick && lastLeftClickState)
    {
        glfwSetCursor(window, NULL);
    }

    lastLeftClickState = leftClick;

    if (leftClick)
    {

        double offsetX = xpos - lastX;
        double offsetY = ypos - lastY;

        lastX = xpos;
        lastY = ypos;

        yaw += cameraSpeedFinal * offsetX * 1.5f;
        pitch -= cameraSpeedFinal * offsetY;
        pitch = std::clamp(pitch, -1.3f, 1.3f);
    }

    // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    //     pitch += cameraSpeedFinal;
    // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    //     pitch -= cameraSpeedFinal;
    // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    //     yaw -= cameraSpeedFinal;
    // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    //     yaw += cameraSpeedFinal;

    // pitch = std::clamp(pitch, -1.3f, 1.3f);
}

void Camera::update()
{
    glm::vec3 cameraPos = target + glm::vec3(
                                       radius * cos(pitch) * sin(yaw),
                                       radius * sin(pitch),
                                       radius * cos(pitch) * cos(yaw));

    glm::mat4 view = glm::lookAt(cameraPos, target, glm::vec3(0.0f, 1.0f, 0.0f));

    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(lightPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / SCR_HEIGHT, 0.1f, 700.0f);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}