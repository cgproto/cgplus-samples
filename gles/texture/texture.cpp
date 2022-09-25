#include "utils.h"
#include "CGPlus.h"

GLuint shaderProgram = 0;
GLuint VBO = 0;
GLuint VAO = 0;
GLuint EBO = 0;
GLuint texture = 0;

void init()
{
  shaderProgram = loadProgram("vertex.glsl", "fragment.glsl");
  texture = loadTexture("../../resources/logo.png");

  glUseProgram(shaderProgram);
  glUniform1i(glGetUniformLocation(shaderProgram, "u_texture"), 0);

  float vertices[] = {
      0.8f, 0.8f, 0.0f, 1.0f, 1.0f,
      0.8f, -0.8f, 0.0f, 1.0f, 0.0f,
      -0.8f, -0.8f, 0.0f, 0.0f, 0.0f,
      -0.8f, 0.8f, 0.0f, 0.0f, 1.0f};
  unsigned int indices[] = {
      0, 1, 3,
      1, 2, 3};

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}
void draw()
{
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
void resize(int width, int height)
{
  glViewport(0, 0, width, height);
}
void release()
{
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);
  glDeleteTextures(1, &texture);
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