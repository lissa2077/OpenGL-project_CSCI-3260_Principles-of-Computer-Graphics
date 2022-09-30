/*
Type your name and student ID here
	- Name:
	- Student ID:
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>


GLint programID;
float x_delta = 0.1f;
int x_press_num = 0;

int mole_pos[9] = {0,0,0,0,0,0,0,0,0};
int current_pos = 4;

GLuint VAO[20];
GLuint VBO[20];
GLuint EBO[20];

void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void sendDataToOpenGL() {
	// TODO:
	// create 3D objects and/or 2D objects and/or lines (points) here and bind to VAOs & VBOs

	{
		const GLfloat triangle[] =
		{
			-0.5f, -0.0f, +0.0f,  // left
			+1.0f, +0.0f, +0.0f,  // color

			+0.5f, -0.0f, +0.0f,  // right
			+1.0f, +0.0f, +0.0f,

			+0.0f, +1.0f, +0.0f,  // top
			+1.0f, +0.0f, +0.0f,

			+0.0f, -0.0f, -0.5f,  // back
			+1.0f, +0.0f, +0.0f,
		};

		glGenVertexArrays(1, &VAO[0]);
		glGenBuffers(1, &VBO[0]);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


		glGenBuffers(1, &EBO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		GLuint indices[] = {
			0, 1, 2,
			2, 3, 1,
			2, 3, 0,
			0, 2, 3

		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	//block
	{
		const GLfloat platform[] =
		{
			-25.0f, 0.0f, -50.0f,  // left-top
			+1.0f, +1.0f, +0.0f,  // color

			25.0f, 0.0f, -50.0f,  // right-top
			+1.0f, +1.0f, +0.0f,

			25.0f, 0.0f, 0.0f,  // right-bottom
			+1.0f, +1.0f, +0.0f,

			-25.0f, 0.0f, 0.0f,  // left-bottom
			+1.0f, +1.0f, +0.0f,
		};

		glGenVertexArrays(1, &VAO[1]);
		glGenBuffers(1, &VBO[1]);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(platform), platform, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


		glGenBuffers(1, &EBO[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);

		GLuint indices[] = {
			0,1,2,
			2,3,0

		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	//platform
	{
		const GLfloat platform[] =
		{
			-25.0f, 0.0f, -49.0f,  // left-top
			+0.0f, +1.0f, +0.0f,  // color

			24.0f, 0.0f, -49.0f,  // right-top
			+0.0f, +1.0f, +0.0f,

			24.0f, 0.0f, 0.0f,  // right-bottom
			+0.0f, +1.0f, +0.0f,

			-25.0f, 0.0f, 0.0f,  // left-bottom
			+0.0f, +1.0f, +0.0f,
		};

		glGenVertexArrays(1, &VAO[1]);
		glGenBuffers(1, &VBO[1]);

		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(platform), platform, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


		glGenBuffers(1, &EBO[1]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);

		GLuint indices[] = {
			0,1,2,
			2,3,0

		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}

	//block
	{
		const GLfloat block[] =
		{
			0.0f, 0.0f, 0.0f,  // bottom left-bottom
			+1.0f, +1.0f, +0.0f,  // color

			14.0f, 0.0f, 0.0f,  // bottom right-bottom
			+1.0f, +1.0f, +0.0f,

			14.0f, 0.0f, -14.0f,  // bottom right-top
			+1.0f, +1.0f, +0.0f,

			0.0f, 0.0f, -14.0f,  // bottom left-top
			+1.0f, +1.0f, +0.0f,

			0.0f, 2.0f, 0.0f,  // top left-bottom
			+1.0f, +1.0f, +0.0f,  // color

			14.0f, 2.0f, 0.0f,  // top right-bottom
			+1.0f, +1.0f, +0.0f,

			14.0f, 2.0f, -14.0f,  // top right-top
			+1.0f, +1.0f, +0.0f,

			0.0f, 2.0f, -14.0f,  // top left-top
			+1.0f, +1.0f, +0.0f,
		};

		glGenVertexArrays(1, &VAO[2]);
		glGenBuffers(1, &VBO[2]);

		glBindVertexArray(VAO[2]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(block), block, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


		glGenBuffers(1, &EBO[2]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[2]);

		GLuint indices[] = {
			0,1,2,
			2,3,0,

			4,5,6,
			6,7,4,

			1,2,6,
			1,5,6,

			3,2,6,
			2,3,7,

			3,0,4,
			4,3,7,

			4,0,1,
			4,5,1,

		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	//mole
	{
		const GLfloat mole[] =
		{
			//bottom
			0.0f, 0.0f, 0.0f,  // bottom left-bottom
			+0.5f, +0.3f, +0.2f,  // color

			10.5f, 0.0f, 0.0f,  // bottom right-bottom
			+0.5f, +0.3f, +0.2f,

			10.5f, 0.0f, -10.5f,  // bottom right-top
			+0.5f, +0.3f, +0.2f,

			0.0f, 0.0f, -10.5f,  // bottom left-top
			+0.5f, +0.3f, +0.2f,

			//medium
			0.0f, 12.5f, 0.0f,  // medium left-bottom
			+0.8f, +0.5f, +0.3f,  // color

			10.5f, 12.5f, 0.0f,  // medium right-bottom
			+0.8f, +0.5f, +0.3f,

			10.5f, 12.5f, -10.5f,  // medium right-top
			+0.8f, +0.5f, +0.3f,

			0.0f, 12.5f, -10.5f,  // medium left-top
			+0.8f, +0.5f, +0.3f,

			//top
			2.0f, 15.0f, -2.0f,  // top left-bottom
			+0.9f, +0.7f, +0.5f,  // color

			8.5f, 15.0f, -2.0f,  // top right-bottom
			+0.9f, +0.7f, +0.5f,

			8.5f, 15.0f, -8.5f,  // top right-top
			+0.9f, +0.7f, +0.5f,

			2.0f, 15.0f, -8.5f,  // top left-top
			+0.9f, +0.7f, +0.5f,

			//left eye
			4.0f, 15.1f, -5.0f,  // top left-bottom
			+0.0f, +0.0f, +0.0f,  // color

			4.5f, 15.1f, -5.0f,  // top right-bottom
			+0.0f, +0.0f, +0.0f,

			4.5f, 15.1f, -7.5f,  // top right-top
			+0.0f, +0.0f, +0.0f,

			4.0f, 15.1f, -7.5f,  // top left-top
			+0.0f, +0.0f, +0.0f,

			//right eye
			6.0f, 15.1f, -5.0f,  // top left-bottom
			+0.0f, +0.0f, +0.0f,  // color

			6.5f, 15.1f, -5.0f,  // top right-bottom
			+0.0f, +0.0f, +0.0f,

			6.5f, 15.1f, -7.5f,  // top right-top
			+0.0f, +0.0f, +0.0f,

			6.0f, 15.1f, -7.5f,  // top left-top
			+0.0f, +0.0f, +0.0f,

			//left chin
			2.0f, 15.1f, -3.0f,  // top left-bottom
			+1.0f, +0.8f, +0.9f,  // color

			4.0f, 15.1f, -3.0f,  // top right-bottom
			+1.0f, +0.8f, +0.9f,

			4.0f, 15.1f, -4.0f,  // top right-top
			+1.0f, +0.8f, +0.9f,

			2.0f, 15.1f, -4.0f,  // top left-top
			+1.0f, +0.8f, +0.9f,

			//right chin
			6.5f, 15.1f, -3.0f,  // top left-bottom
			+1.0f, +0.8f, +0.9f,  // color

			8.5f, 15.1f, -3.0f,  // top right-bottom
			+1.0f, +0.8f, +0.9f,

			8.5f, 15.1f, -4.0f,  // top right-top
			+1.0f, +0.8f, +0.9f,

			6.5f, 15.1f, -4.0f,  // top left-top
			+1.0f, +0.8f, +0.9f,
		};

		glGenVertexArrays(1, &VAO[3]);
		glGenBuffers(1, &VBO[3]);

		glBindVertexArray(VAO[3]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(mole), mole, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


		glGenBuffers(1, &EBO[3]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[3]);

		GLuint indices[] = {
			//0,1,2,
			//2,3,0,

			4,5,6,
			6,7,4,

			1,2,6,
			1,5,6,

			3,2,6,
			2,3,7,

			3,0,4,
			4,3,7,

			4,0,1,
			4,5,1,


			8,9,10,
			10,11,8,

			9,5,6,
			9,10,6,

			11,10,6,
			11,7,6,

			7,11,8,
			7,4,8,

			8,4,5,
			8,9,5,

			//eye
			12,13,14,
			12,15,14,

			16,17,18,
			16,19,18,

			//chin
			20,21,22,
			20,23,22,

			24,25,26,
			24,27,26,
		};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	//select box
	{
	const GLfloat select_box[] =
	{
		0.0f, 0.0f, 0.0f,  // bottom left-bottom
		+1.0f, +1.0f, +0.0f,  // color

		14.0f, 0.0f, 0.0f,  // bottom right-bottom
		+1.0f, +1.0f, +0.0f,

		14.0f, 0.0f, -14.0f,  // bottom right-top
		+1.0f, +1.0f, +0.0f,

		0.0f, 0.0f, -14.0f,  // bottom left-top
		+1.0f, +1.0f, +0.0f,
	};

	glGenVertexArrays(1, &VAO[4]);
	glGenBuffers(1, &VBO[4]);

	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(select_box), select_box, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


	glGenBuffers(1, &EBO[4]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[4]);

	GLuint indices[] = {
		0,1,2,
		2,3,0,

		4,5,6,
		6,7,4,

		1,2,6,
		1,5,6,

		3,2,6,
		2,3,7,

		3,0,4,
		4,3,7,

		4,0,1,
		4,5,1,

	};
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
}

void paintGL(void) {
	// always run
	// TODO:
	// render your objects and control the transformation here
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  //specify the background color
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 modelTransformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(x_delta * x_press_num, 0.0f, 0.0f));

	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 1.0f, 100.0f);
	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(0.0f, 64.0f, 10.0f),
		glm::vec3(0.0, 39.0f, -2.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));

	GLint modelTransformMatrixUniformLocation =
		glGetUniformLocation(programID, "modelTransformMatrix");
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1,
		GL_FALSE, &modelTransformMatrix[0][0]);
	GLint projectionMatrixUniformLocation =
		glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixUniformLocation, 1,
		GL_FALSE, &projectionMatrix[0][0]);
	GLint viewMatrixUniformLocation =
		glGetUniformLocation(programID, "viewMatrix");
	glUniformMatrix4fv(viewMatrixUniformLocation, 1,
		GL_FALSE, &viewMatrix[0][0]);
	{
		glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(0.0f, 5.0f, 0.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
	}
	//platform
	{
		glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(0.0f, 0.0f, 2.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	//blocks
	{
		//7
		glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-22.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//4
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-22.5f, 0.0f, -16.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//1
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-22.5f, 0.0f, -32.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//8
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-7.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//5
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-7.5f, 0.0f, -16.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//2
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-7.5f, 0.0f, -32.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//9
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(7.5f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//6
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(7.5f, 0.0f, -16.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		//3
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(7.5f, 0.0f, -32.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[2]);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	//mole
	{
		//7
		glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-20.5f, 0.0f, -2.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//4
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-20.5f, 0.0f, -18.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//1
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-20.5f, 0.0f, -32.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//8
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-5.5f, 0.0f, -2.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//5
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-5.5f, 0.0f, -18.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//2
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(-5.5f, 0.0f, -32.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//9
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(9.5f, 0.0f, -2.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//6
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(9.5f, 0.0f, -18.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);

		//3
		modelTransformMatrix = glm::mat4(1.0f);
		modelTransformMatrix = glm::translate(modelTransformMatrix, glm::vec3(9.5f, 0.0f, -32.0f));
		glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
		glBindVertexArray(VAO[3]);
		glDrawElements(GL_TRIANGLES, 84, GL_UNSIGNED_INT, 0);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* do not allow resizing */
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback); // TODO
	// TODO: mouse callback, etc.

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
