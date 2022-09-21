#include "utils.h"
#include "CGPlus.h"

GLuint shaderProgram = 0;
GLuint VBO = 0;
GLuint VAO = 0;

void init()
{
  shaderProgram = loadProgram("vertex.glsl", "fragment.glsl");
  float vertices[] = {
      -0.8f, -0.8f, 0.0f,
      0.8f, -0.8f, 0.0f,
      0.0f, 0.8f, 0.0f};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void draw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLES, 0, 3);
}
void resize(int width, int height)
{
  glViewport(0, 0, width, height);
}
void release()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shaderProgram);
}
int main()
{
  CGPlus::GLESAppConfig config;
  config.renderCallback = draw;
  config.initCallback = init;
  config.resizeCallback = resize;
  config.exitCallback = release;
  CGPlus::createApp(&config);
}