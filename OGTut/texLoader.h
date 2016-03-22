#ifndef texLoader_H
#define texLoader_H

#include <gl\glew.h>
#include <GL\freeglut.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>


GLuint loadBMPtex(const char * imagepath);
GLuint loadTGAtex(const char * imagepath);
GLuint loadDDStex(const char * imagepath);

#endif