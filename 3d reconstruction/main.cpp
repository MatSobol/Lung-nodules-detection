#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <array>
#include <fstream>

#include "read.hpp"
#include "interactive/camera.hpp"
#include "interactive/cut.hpp"
#include "interactive/processInput.hpp"
#include "setup/init.hpp"
#include "const.hpp"

int main()
{
    auto [vertices, maxValues] = readVertices();

    std::cout << "read\n";

    glm::vec3 modelPos = {-maxValues[0] / 2, -maxValues[1] / 2, -maxValues[2] / 2};

    auto [window, shaderProgram] = init();

    unsigned int VBO,
        VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<IInteractive *> inputHandlers;

    inputHandlers.push_back(new Camera(window, shaderProgram, maxValues[2] * 2));
    inputHandlers.push_back(new Cut(shaderProgram, maxValues));

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window, deltaTime, inputHandlers);

        glUseProgram(shaderProgram);

        glm::vec4 clippingPlane(0.0f, 0.0f, 0.0f, 0.0f);

        int clipLoc = glGetUniformLocation(shaderProgram, "clippingPlane");
        glUniform4fv(clipLoc, 1, glm::value_ptr(clippingPlane));

        for (auto &handler : inputHandlers)
        {
            handler->update();
        }

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniform3f(glGetUniformLocation(shaderProgram, "objectColor"), 0.0f, 0.0f, 0.7f);

        glBindVertexArray(VAO);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");

        glm::mat4 model = glm::translate(glm::mat4(1.0f), modelPos);

        model = glm::scale(model, glm::vec3(scale[0], scale[1], scale[2]));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    for (auto &handler : inputHandlers)
    {
        delete handler;
    }

    glfwTerminate();
    return 0;
}
