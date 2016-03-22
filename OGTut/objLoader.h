#ifndef objLoader_H
#define objLoader_H

#include <gl\glew.h>
#include <GL\freeglut.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>
#include <vector>



bool loadOBJ(

     const char * path,

     std::vector < glm::vec3 > & out_vertices,

     std::vector < glm::vec2 > & out_uvs,

     std::vector < glm::vec3 > & out_normals

 );

#endif