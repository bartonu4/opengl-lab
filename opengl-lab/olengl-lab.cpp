// olengl-lab.cpp : Defines the entry point for the console application.
//
#define BUFFER_OFFSET(x)  ((const void*) (x))

#include "stdafx.h"
#include <GL\GLEW.h>
#include <GL\freeglut.h>
#include "packages\glm.0.9.7.1\build\native\include\glm\mat4x4.hpp"
#include "packages\glm.0.9.7.1\build\native\include\glm\vec3.hpp"// glm::vec4
#include "packages\glm.0.9.7.1\build\native\include\glm\vec4.hpp" // glm::mat4
#include "packages\glm.0.9.7.1\build\native\include\glm\gtc\matrix_transform.hpp"// glm::translate, glm::rotate, glm::scale, glm::perspective
#include "packages\glm.0.9.7.1\build\native\include\glm\gtc\type_ptr.hpp"
#include <iostream>

#include "includes/LoadShaders.h"

GLuint vao[1];
GLuint vbo[1];
GLuint ebo[1];
GLuint model_matrix_uiform;
GLuint project_matrix_uniform;

using namespace std;
using glm::vec3;
using glm::mat4;
void reshape(int width, int height);
void timer(int value);
float aspect;
float angle=0;

void init(void)
{
	int a;
	
	glClearColor(1.f, 1.f, 1.f, 1.f);
	static const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	// Color for each vertex
	static const GLfloat cube_colors[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f
	};

	// Indices for the triangle strips
	static const GLushort cube_indices[] =
	{
		0, 1, 2, 3, 6, 7, 4, 5,         // First strip
		0xFFFF,                         // <<-- This is the restart index
		2, 6, 0, 4, 1, 5, 3, 7          // Second strip
	};
	
	ShaderInfo shaders[] = {
		{ GL_FRAGMENT_SHADER, getShaderFromFile("shaders/tri.frag") },
		{ GL_VERTEX_SHADER, getShaderFromFile("shaders/tri.vert") }
	};

	GLuint program = LoadShaders(shaders);
	glUseProgram(program);

	model_matrix_uiform = glGetUniformLocation(program, "model_matrix");
	project_matrix_uniform = glGetUniformLocation(program, "projection_matrix");

	// Set up the element array buffer
	glGenBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

	// Set up the vertex attributes
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions)+sizeof(cube_colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cube_positions), cube_positions);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(cube_positions), sizeof(cube_colors), cube_colors);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(cube_positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	
}
void display(void)
{
	glEnable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	float t = 0;
	const vec3 X(1.0, 0.0, 0.0);
	const vec3 Y(0.0, 1.0, 0.0);
	const vec3 Z(0.0, 0.0, 1.0);
	mat4 rotation_matrix = glm::rotate(mat4(1.0f), angle * 360, Y)*glm::rotate(mat4(1.0f), angle*720, Z);
	mat4 model_matrix = (glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -5.0f)) *rotation_matrix);
	mat4 proj_matrix = glm::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 500.0f);
	
	glUniformMatrix4fv(model_matrix_uiform, 4, GL_FALSE,glm::value_ptr(model_matrix));
	glUniformMatrix4fv(project_matrix_uniform, 1, GL_FALSE, glm::value_ptr(proj_matrix));
 
		// When primitive restart is on, we can call one draw command
	/*	glEnable(GL_PRIMITIVE_RESTART);
	glPrimitiveRestartIndex(0xFFFF);
	glDrawElements(GL_TRIANGLE_STRIP, 17, GL_FLOAT, NULL);
	glDisable(GL_PRIMITIVE_RESTART);
	*/
	glDrawElements(GL_TRIANGLES, 8, GL_FLOAT, NULL);
	glDrawElements(GL_TRIANGLE_STRIP, 8, GL_FLOAT,
		(const GLvoid *)(9 * sizeof(GLushort)));
}
int _tmain(int argc, char * argv[])
{
	glewExperimental = true;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(50,timer,0);
	glutMainLoop();
	return 0;
}
void reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	aspect = float(height) / float(width);
}

void timer(int value)
{
	angle+=10;
	//printf("nu4");
	glutPostRedisplay();
	glutTimerFunc(40, timer, 0);
}