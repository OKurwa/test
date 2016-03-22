// OGTut.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdafx.h"
#include <gl\glew.h>
#include <GL\freeglut.h>
#include <glfw\glfw3.h>
GLFWwindow * hWindow;
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "loadShader.h"
#include "texLoader.h"
#include "controls.h"
#include "objLoader.h"
#include "indexer.h"
using namespace glm;

int main()
{
	//������������� ���������� GLFW

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	//��������� ���������� ���� 

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x �����������
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // ��� ����� OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed


														 // ������� ���� � �������� ��������

	hWindow = glfwCreateWindow(640, 480, "My OGL window", NULL, NULL);

	if (hWindow == NULL) {
		fprintf(stderr, "Failed to open GLFW window\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(hWindow);

	// �������������� GLEW

	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "не удалось инициализировать GLEW\n");
		return -1;
	}

	// ������� ��������� ������
	glfwSetInputMode(hWindow, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(hWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);


	//������� ������ ������
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//������ ������ ����

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("vertexShader.txt", "fragmentShader.txt");

	// Get a handle for our "MVP" uniform

	// Only during the initialisation

	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewID = glGetUniformLocation(programID, "M");
	GLuint ModelID = glGetUniformLocation(programID, "V");


	/* ����� ��� ����
	static GLfloat g_color_buffer_data_cube[12*3*3];
	// This will identify our vertex buffer
	for (int v = 0; v < 12*3 ; v++){

	g_color_buffer_data_cube[3*v+0] = static_cast<float>(rand()) / RAND_MAX;;

	g_color_buffer_data_cube[3*v+1] = static_cast<float>(rand()) / RAND_MAX;;

	g_color_buffer_data_cube[3*v+2] = static_cast<float>(rand()) / RAND_MAX;;

	}

	*/
	// Generate 1 buffer, put the resulting identifier in vertexbuffer

	/*------------------------------------------------------*/
	/*------------------------------------------------------*/
	//���������� ����
	/*------------------------------------------------------*/
	/*------------------------------------------------------*/


	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals;
	std::vector< glm::vec3 > out_vertices;
	std::vector< glm::vec2 > out_uvs;
	std::vector< glm::vec3 > out_normals;
	std::vector< unsigned short > out_indices;

	//bool done = loadOBJ("res/text.obj", vertices, uvs, normals);
	bool done = loadOBJ("res/suzanne.obj", vertices, uvs, normals);
	indexVBO(vertices, uvs, normals, out_indices, out_vertices, out_uvs, out_normals);


	//for (int i=0; i<uvs.size(); i++ ){
	//	uvs[i].y = 1.0f - uvs[i].y;
	//};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);


	//GLuint Texture = loadBMPtex("res/tex/iron1.bmp");
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
	GLuint Texture = loadDDStex("res/tex/uvmap.dds");
	//��������� ������

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_uvs.size() * sizeof(glm::vec2), &out_uvs[0], GL_STATIC_DRAW);


	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);

	GLuint elemBuffer;
	glGenBuffers(1, &elemBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size() * sizeof(unsigned short), &out_indices[0], GL_STATIC_DRAW);

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);


	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix

	glm::mat4 View = glm::lookAt(

		glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space

		glm::vec3(0, 0, 0), // and looks at the origin

		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)

		);
	// Model matrix : an identity matrix (model will be at the origin)

	glm::mat4 Model = glm::mat4(1.0f);

	// Use #include <glm/gtc/matrix_transform.hpp> and #include <glm/gtx/transform.hpp>
	//������
	glm::vec3 myScaleVector(1.f, 1.0f, 1.0f);
	glm::mat4 myScalingMatrix = glm::scale(myScaleVector);


	// Use #include <glm/gtc/matrix_transform.hpp> and #include <glm/gtx/transform.hpp>
	//�������
	glm::vec3 myRotationAxis(0, 1, 0);
	glm::mat4 myRotationMatrix = glm::rotate(60.0f, myRotationAxis);
	//��������
	glm::vec3 myTransVector(0.0f, 0.0f, 0.0f);
	glm::mat4 myTranslationMatrix = glm::translate(myTransVector);

	glm::mat4 myCubeModelMatrix = myTranslationMatrix * myRotationMatrix * myScalingMatrix;
	// Our ModelViewProjection : multiplication of our 3 matrices

	glm::mat4 mvp1 = Projection * View * myCubeModelMatrix; // Remember, matrix multiplication is the other way around


															// Use #include <glm/gtc/matrix_transform.hpp> and #include <glm/gtx/transform.hpp>
															//������

															//�������� 

															// Send our transformation to the currently bound shader, in the "MVP" uniform

															// This is done in the main loop since each model will have a different MVP matrix (At least for the M part)

															// Enable depth test
															//�������� ����� �������
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	//��������, ���� �����
	glDepthFunc(GL_LESS);

	/*------------------------------------------------------*/
	/*------------------------------------------------------*/
	//������� ����
	/*------------------------------------------------------*/
	/*------------------------------------------------------*/
	glUseProgram(programID);
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	do {


		/*------------------------------------------------------*/
		/*------------------------------------------------------*/
		//����������
		/*------------------------------------------------------*/
		/*------------------------------------------------------*/
		// Compute the MVP matrix from keyboard and mouse input

		computeMatricesFromInput();

		glm::mat4 ProjectionMatrix = getProjectionMatrix();

		glm::mat4 ViewMatrix = getViewMatrix();

		glm::mat4 ModelMatrix = glm::mat4(1.0);

		mvp1 = ProjectionMatrix * ViewMatrix * ModelMatrix;


		// 1rst attribute buffer : vertices
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(programID);


		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);
		/*------------------------------------------------------*/
		/*------------------------------------------------------*/
		//������ ���
		/*------------------------------------------------------*/
		/*------------------------------------------------------*/
		// Use our shader
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp1[0][0]);
		glUniformMatrix4fv(ViewID, 1, GL_FALSE, &ViewMatrix[0][0]);
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &ModelMatrix[0][0]);

		glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset

			);

		// 2nd attribute buffer : uv coords

		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);

		glVertexAttribPointer(

			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.

			2,                                // size

			GL_FLOAT,                         // type

			GL_FALSE,                         // normalized?

			0,                                // stride

			(void*)0                          // array buffer offset

			);

		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);

		glVertexAttribPointer(

			2,                                // attribute

			3,                                // size

			GL_FLOAT,                         // type

			GL_FALSE,                         // normalized?

			0,                                // stride

			(void*)0                          // array buffer offset

			);

		// Draw the cube !
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffer);
		glDrawElements(
			GL_TRIANGLES,
			out_indices.size(),
			GL_UNSIGNED_SHORT,
			(void*)0
			);


		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glfwSwapBuffers(hWindow);
		glfwPollEvents();
	} // ��������� Escape �� �������, ��� ������������ ������� ������ ����
	while ((glfwGetKey(hWindow, GLFW_KEY_E) != GLFW_PRESS) && (glfwWindowShouldClose(hWindow) == 0));

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();
    return 0;
}

