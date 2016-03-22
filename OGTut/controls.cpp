#include "stdafx.h"
#include <gl\glew.h>
#include <GL\freeglut.h>
#include <glfw\glfw3.h>
#include <glm\glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "controls.h"
// position

 extern GLFWwindow * hWindow;
 glm::vec3 position = glm::vec3( 0, 0, 5 );

 // horizontal angle : toward -Z

 float horizontalAngle = 3.14f;

 // vertical angle : 0, look at the horizon

 float verticalAngle = 0.0f;

 // Initial Field of View

 float initialFoV = 45.0f;

 glm::mat4 Projection;
 glm::mat4 ViewMatrix ;

 float speed = 3.0f; // 3 units / second

 float mouseSpeed = 0.5f;

 

 void computeMatricesFromInput(){

	 // Get mouse position
	static double lastTime=glfwGetTime();

	double xpos, ypos;

	double currentTime = glfwGetTime();

	float deltaTime = float(currentTime - lastTime);

	lastTime = currentTime ;

	glfwGetCursorPos(hWindow, &xpos , &ypos);

	// Reset mouse position for next frame

	glfwSetCursorPos(hWindow, 640/2, 480/2);

	// Compute new orientation

	horizontalAngle += mouseSpeed * deltaTime * float(640/2 - xpos );

	verticalAngle   += mouseSpeed * deltaTime * float(480/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion

	glm::vec3 direction(

     cos(verticalAngle) * sin(horizontalAngle),

     sin(verticalAngle),

     cos(verticalAngle) * cos(horizontalAngle)
	 );

	 // Right vector

	glm::vec3 right = glm::vec3(

    sin(horizontalAngle - 3.14f/2.0f),

    0,

    cos(horizontalAngle - 3.14f/2.0f)

 	);
 
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward

	if (glfwGetKey(hWindow, GLFW_KEY_W ) == GLFW_PRESS){

		position += direction * deltaTime * speed;

	}

	// Move backward

	if (glfwGetKey(hWindow, GLFW_KEY_S ) == GLFW_PRESS){

		position -= direction * deltaTime * speed;

	}

	// Strafe right

	if (glfwGetKey(hWindow, GLFW_KEY_D ) == GLFW_PRESS){

		position += right * deltaTime * speed;

	}

	// Strafe left

	if (glfwGetKey(hWindow, GLFW_KEY_A ) == GLFW_PRESS){

		position -= right * deltaTime * speed;

	}
	// Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

	// Camera matrix

	ViewMatrix       = glm::lookAt(

		position,           // Camera is here

		position+direction, // and looks here : at the same position, plus "direction"

		up                 // Head is up (set to 0,-1,0 to look upside-down)

	);
	 
	
 };

 glm::mat4 getProjectionMatrix(){
		return	Projection ;
	};

glm::mat4 getViewMatrix(){
		return	ViewMatrix  ;

	};