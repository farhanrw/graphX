//example skeleton code 2019 winter comp371
//modified from http://learnopengl.com/

#include "stdafx.h"

#include "..\glew\glew.h"	// include GL Extension Wrangler
#include "..\glfw\glfw3.h"	// include GLFW helper library
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"
#include "OBJLoader.h" //include the object loader
#include "IndexedObj.h" //include the object loader
#include "shaderloader.h"
using namespace std;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;
GLFWwindow *window;

glm::vec3 campos(0.0f, 0.0f, 15.0f);
glm::vec3 camdir(0.0f, 0.0f, -1.0f);
glm::vec3 camup(0.0f, 1.0f, 0.0f);

float translateX = 0.0f , translateY = 0.0f, translateZ = 0.0f;

bool lightsOn = false, greenChannel = true, blueChannel = true, redChannel = true, allChannel = true, phong = false;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	//std::cout << key << std::endl;	

	if (key == GLFW_KEY_I)
		translateZ -= 0.5f;
	if (key == GLFW_KEY_K)
		translateZ += 0.5f;
	if (key == GLFW_KEY_J)
		translateX -= 0.5f;
	if (key == GLFW_KEY_L)
		translateX += 0.5f;
	if (key == GLFW_KEY_PAGE_UP)
		translateY += 0.5f;
	if (key == GLFW_KEY_PAGE_DOWN)
		translateY -= 0.5f;
	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		lightsOn = !lightsOn;
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		redChannel = !redChannel;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		greenChannel = !greenChannel;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		blueChannel = !blueChannel;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		allChannel = true;
	}
	if (key == GLFW_KEY_5 && action == GLFW_PRESS)
	{
		phong = !phong;
	}
}

int init() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	//WINDOW
	window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 1", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}
	return 0;
}
// The MAIN function, from here we start the application and run the game loop
int main()
{
	if (init() != 0)
		return EXIT_FAILURE;
	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	// Build and compile our shader program
	// Vertex shader
	glEnable(GL_DEPTH_TEST);

	GLuint shader = loadSHADER("vertex.shader", "fragment.shader");
	glUseProgram(shader);



	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> UVs;
	
	std::vector<int> indices;

	loadOBJ2("heracles.obj", indices, vertices, normals, UVs); //read the vertices from the cube.obj file


	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	GLuint vertices_VBO;

	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &vertices_VBO);

	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// ****************************************** NORMALS ****************************************** 

	GLuint normals_VBO;
	glGenBuffers(1, &normals_VBO);

	glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices.front(),GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 TranslateMatrix;
	glm::mat4 RotateMatrix;

	int model, view, projection, lightInfo, redInfo, greenInfo, blueInfo, phongInfo;

	model = glGetUniformLocation(shader, "Model");
	view = glGetUniformLocation(shader, "View");
	projection = glGetUniformLocation(shader, "Projection");
	lightInfo = glGetUniformLocation(shader, "lightsOn");
	redInfo = glGetUniformLocation(shader, "redOn");
	greenInfo = glGetUniformLocation(shader, "greenOn");
	blueInfo = glGetUniformLocation(shader, "blueOn");
	phongInfo = glGetUniformLocation(shader, "phong");

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translateX, translateY, translateZ));
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));;

		ModelMatrix = TranslateMatrix * RotateMatrix;
		
		ViewMatrix = glm::lookAt(campos, camdir+campos, camup);
		ProjectionMatrix = glm::perspective(45.0f, (float)WIDTH / HEIGHT, 1.0f, 100.0f);

		glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(ModelMatrix));
		glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniform1i(lightInfo, lightsOn);
		glUniform1i(redInfo, redChannel);
		glUniform1i(greenInfo, greenChannel);
		glUniform1i(blueInfo, blueChannel);
		glUniform1i(phongInfo, phong);

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (GLvoid*)0);
		glBindVertexArray(0);

		/////////////////////// MODEL 2//////////////////////////

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
