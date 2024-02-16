#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "Render.h"
#include "Assertion.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "resource/glm/glm.hpp"
#include "resource/glm/gtc/matrix_transform.hpp"
#include "resource/glm/gtc/type_ptr.hpp"

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std ::cout << "Hello" << window << std::endl;
        const char *description;
        if (glfwGetError(&description) != GLFW_NO_ERROR)
        {
            fprintf(stderr, "GLFW error: %s\n", description);
        }
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    // init glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    std ::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f,    // 0
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,  // 1
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,   // 2
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  // 3
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 4
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f,   // 5
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f,   // 6
        -0.5f, -0.5f, 0.5f, 1.0f, 1.0f   // 7
    };
    unsigned int indices[] = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7};
    VertexArray va;
    VertexBuffer vb(&positions, 5 * 8 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    IndexBuffer ib(indices, 36);
    Shader shader("src/resource/shader/Basic.shader");
    shader.Bind();
    va.AddBuffer(vb, layout);

    Texture texture("src/resource/asset/wall.jpg");
    std::cout << "sometimes maybe not" << std::endl;
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0);
    // shader.SetUniform4f("u_Color", 0.0f, 0.5f, 0.3f, 1.0f);
    shader.UnBind();
    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    Render renderer;
    std::vector<glm::vec3> v;
    v.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    v.push_back(glm::vec3(2.0f, 5.0f, -15.0f));
    v.push_back(glm::vec3(-1.5f, -2.2f, -2.5f));
    v.push_back(glm::vec3(-3.8f, -2.0f, -12.3f));
    v.push_back(glm::vec3(2.4f, -0.4f, -3.5f));
    v.push_back(glm::vec3(-1.7f, 3.0f, -7.5f));
    v.push_back(glm::vec3(1.3f, -2.0f, -2.5f));
    v.push_back(glm::vec3(1.5f, 2.0f, -2.5f));
    v.push_back(glm::vec3(1.5f, 0.2f, -1.5f));
    v.push_back(glm::vec3(-1.3f, 1.0f, -1.5f));

    float xRotation = 0.0f;
    float yRotation = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
        shader.SetUniformMatrix4fv("projection", projection);
        glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        float radius = 10.0f;
        float camX = static_cast<float>(sin(glfwGetTime()) * radius);
        float camZ = static_cast<float>(cos(glfwGetTime()) * radius);
        view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        shader.SetUniformMatrix4fv("view", view);

        for (int j = 0; j < v.size(); j++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, v[j]);
            float angle = 20.0f * j;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.SetUniformMatrix4fv("model", model);
            renderer.Draw(ib, va, shader);
        }

        glfwSwapBuffers(window);
        {
            int stateW = glfwGetKey(window, GLFW_KEY_W);
            if (stateW == GLFW_PRESS)
            {
                std::cout << "W" << std::endl;
                view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
            }
            int stateA = glfwGetKey(window, GLFW_KEY_A);
            if (stateA == GLFW_PRESS)
            {
                std::cout << "A" << std::endl;
            }
            int stateD = glfwGetKey(window, GLFW_KEY_D);
            if (stateD == GLFW_PRESS)
            {
                std::cout << "D" << std::endl;
            }
            int stateS = glfwGetKey(window, GLFW_KEY_S);
            if (stateS == GLFW_PRESS)
            {
                std::cout << "S" << std::endl;
            }
        }
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
