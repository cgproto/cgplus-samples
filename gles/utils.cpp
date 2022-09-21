#include "utils.h"
#include "CGPlus.h"
#include <stdio.h>

GLuint loadProgram(const char *vertexShaderPath, const char *fragmentShaderPath)
{
  const char *vertexShaderSource = (const char *)CGPlus::readFileSync(vertexShaderPath, NULL);
  const char *fragmentShaderSource = (const char *)CGPlus::readFileSync(fragmentShaderPath, NULL);

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("compile vertex shader failed:\n%s\n", infoLog);
    return 0;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    printf("compile fragment shader failed:\n%s\n", infoLog);
    return 0;
  }

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    printf("link program failed:\n%s\n", infoLog);
    return 0;
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  return shaderProgram;
}