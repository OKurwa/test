#ifndef controls_H
#define controls_H

#include <gl\glew.h>
#include <GL\freeglut.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>


void computeMatricesFromInput();
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();

#endif