#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "IndexBuffer.h"
#include "Render.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include <sstream>
#include "resource/glm/glm.hpp"
#include "resource/glm/gtc/matrix_transform.hpp"
#include "resource/glm/gtc/type_ptr.hpp"

// camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

float lastX = 400, lastY = 300;
float yaw = -90.0f, pitch = 0.0f;

void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = static_cast<float>(2.5 * deltaTime);
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos +=
        glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn) {
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  float xoffset = xpos - lastX;
  float yoffset =
      lastY - ypos; // reversed since y-coordinates range from bottom to top
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;
  yaw += xoffset;
  pitch += yoffset;
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}
int main(void) {
  GLFWwindow *window;
  /* Initialize the library */
  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  //
  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window) {
    std ::cout << "Hello" << window << std::endl;
    const char *description;
    if (glfwGetError(&description) != GLFW_NO_ERROR) {
      fprintf(stderr, "GLFW error: %s\n", description);
    }
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSwapInterval(1);
  // init glew
  if (glewInit() != GLEW_OK) {
    std::cout << "ERROR" << std::endl;
    return -1;
  }
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  std ::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
      0.5f,  0.5f,  1.5f,
      -0.5f, 0.5f,  -0.5f,
      -0.5f, 0.5f,  0.5f,
      0.5f,  -0.5f, -0.5f,
      -0.5f, -0.5f, -0.5f,
      0.5f,  0.5f,  -0.5f,
      0.5f,  -0.5f, 0.5f,
      -0.5f, -0.5f, 0.5f,
  };

  unsigned int indices[] = {0, 1, 2, 1, 3, 4, 5, 6, 3, 7, 3, 6,
                            2, 4, 7, 0, 7, 6, 0, 5, 1, 1, 5, 3,
                            5, 0, 6, 7, 4, 3, 2, 1, 4, 0, 2, 7};
  std::vector<float> pos;
  std::ifstream stream("src/resource/sample/something.txt");
  std::string line;
  while (getline(stream, line)){
      std::string coord;
      std::stringstream ss(line);
      while(getline(ss,coord, ' ')){
          pos.push_back(std::stof(coord)/750.0f);
          std::cout<<pos[pos.size()-1]<<std::endl;
      }
  }
        std::cout<<pos.size()<<std::endl;
  VertexArray va;
  VertexBuffer vb(pos.data(), pos.size() * sizeof(float));
  VertexBufferLayout layout;

  IndexBuffer ib(indices, 36);
  Shader shader("src/resource/shader/Basic.shader");
  shader.Bind();
  layout.Push<float>(3);
  va.AddBuffer(vb, layout);
  // shader.SetUniform4f("u_Color", 0.0f, 0.5f, 0.3f, 1.0f);
  shader.UnBind();
  va.UnBind();
  vb.UnBind();
  ib.UnBind();
  Render renderer;
  std::vector<glm::vec3> v;
  v.push_back(glm::vec3(0.0f, 0.0f, 0.0f));


  int fps = 0;
  float xRotation = 0.0f;
  float yRotation = 0.0f;
  float last = static_cast<float>(glfwGetTime());
  while (!glfwWindowShouldClose(window)) {
    fps++;
    renderer.Clear();
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    if (currentFrame - last >= 1.0) {
      std::cout << fps << std::endl;
      last = currentFrame;
      fps = 0;
    }
    lastFrame = currentFrame;
    processInput(window);
    glm::mat4 projection;
    projection =
        glm::perspective(glm::radians(45.0f), 640.0f / 480.0f, 0.1f, 100.0f);
    shader.SetUniformMatrix4fv("projection", projection);
    glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    shader.SetUniformMatrix4fv("view", view);

    for (int j = 0; j < v.size(); j++) {
      // calculate the model matrix for each object and pass it to shader before
      // drawing
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, v[j]);
      float angle = 20.0f * j;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      shader.SetUniformMatrix4fv("model", model);
      renderer.Draw(ib, va, shader);
    }

    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
