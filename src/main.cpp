#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Render.h"
#include "Assertion.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
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
    // glfwSwapInterval(1);
    // init glew
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }
    std ::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,0.0f,0.0f,
        0.5f, -0.5f, 1.0f,0.0f,
        0.5f, 0.5f, 1.0f,1.0f,
        -0.5f, 0.5f,0.0f,1.0f};
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0};

    VertexArray va;
    VertexBuffer vb(&positions, 4 * 4 * sizeof(float));

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    IndexBuffer ib(indices, 6);
    Shader shader("src/resource/shader/Basic.shader");
    shader.Bind();
    va.AddBuffer(vb, layout);

    // get version info
    const GLubyte *rendererInfo = glGetString(GL_RENDERER); // get renderer string
    const GLubyte *versionInfo = glGetString(GL_VERSION);   // version as a string
    std::cout << "Renderer: " << rendererInfo << std::endl;
    std::cout << "OpenGL version supported " << versionInfo << std::endl;
    std::cout << "sometimes seppuku" << std::endl;
    Texture texture("src/resource/asset/wall.jpg");
    std::cout << "sometimes maybe not" << std::endl;
    texture.Bind();
    shader.SetUniform1i("u_Texture",0);
    // shader.SetUniform4f("u_Color", 0.0f, 0.5f, 0.3f, 1.0f);
    shader.UnBind();
    va.UnBind();
    vb.UnBind();
    ib.UnBind();

    /* Loop until the user closes the window */
    Render renderer;
    float r = 0.0f;
    float increment = 0.005f;
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        // shader.Bind();
        // shader.SetUniform4f("u_Color", r, 0.5f, 0.3f, 1.0f);
        renderer.Draw(ib,va,shader);

        if (r > 1.0f)
        {
            increment = -0.005f;
        }
        else if (r < 0.0f)
        {
            increment = 0.005f;
        }
        r += increment;
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
 