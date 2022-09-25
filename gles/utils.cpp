#include "utils.h"
#include "CGPlus.h"
#include <stdio.h>
#include "stb_image.h"

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
  free((void *)vertexShaderSource);
  if (!success)
  {
    free((void *)fragmentShaderSource);
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    printf("compile vertex shader failed:\n%s\n", infoLog);
    return 0;
  }

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  free((void *)fragmentShaderSource);
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

GLuint loadTexture(const char *imagePath)
{
  int width, height, numChannels;
  unsigned char *data = stbi_load(imagePath, &width, &height, &numChannels, 4);
  if (data)
  {
    GLuint texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
  }
  return 0;
}