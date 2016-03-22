#ifndef loadShader_H
#define loadShader_H

#include <gl\glew.h>
#include <GL\freeglut.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>


GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);

#endif