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
static const GLushort cube_indices[] =
{
	0, 1, 2, 3, 6, 7, 4, 5,8,9,10,11,13,14,15,         // First strip
	0xFFFF,                         // <<-- This is the restart index
	2, 6, 0, 4, 1, 5, 3, 7, 8,9, 10,11,12,13,14,15         // Second strip
};
void init(void)
{
	int a;
	
	glClearColor(1.f, 1.f, 1.f, 1.f);
	static const GLfloat cube_positions[] =
	{
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	// Color for each vertex
	static const GLfloat cube_colors[] =
	{
		0.583f, 0.771f, 0.014f,
		0.609f, 0.115f, 0.436f,
		0.327f, 0.483f, 0.844f,
		0.822f, 0.569f, 0.201f,
		0.435f, 0.602f, 0.223f,
		0.310f, 0.747f, 0.185f,
		0.597f, 0.770f, 0.761f,
		0.559f, 0.436f, 0.730f,
		0.359f, 0.583f, 0.152f,
		0.483f, 0.596f, 0.789f,
		0.559f, 0.861f, 0.639f,
		0.195f, 0.548f, 0.859f,
		0.014f, 0.184f, 0.576f,
		0.771f, 0.328f, 0.970f,
		0.406f, 0.615f, 0.116f,
		0.676f, 0.977f, 0.133f,
		0.971f, 0.572f, 0.833f,
		0.140f, 0.616f, 0.489f,
		0.997f, 0.513f, 0.064f,
		0.945f, 0.719f, 0.592f,
		0.543f, 0.021f, 0.978f,
		0.279f, 0.317f, 0.505f,
		0.167f, 0.620f, 0.077f,
		0.347f, 0.857f, 0.137f,
		0.055f, 0.953f, 0.042f,
		0.714f, 0.505f, 0.345f,
		0.783f, 0.290f, 0.734f,
		0.722f, 0.645f, 0.174f,
		0.302f, 0.455f, 0.848f,
		0.225f, 0.587f, 0.040f,
		0.517f, 0.713f, 0.338f,
		0.053f, 0.959f, 0.120f,
		0.393f, 0.621f, 0.362f,
		0.673f, 0.211f, 0.457f,
		0.820f, 0.883f, 0.371f,
		0.982f, 0.099f, 0.879f
	};

	// Indices for the triangle strips
	
	
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(cube_positions));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	
}
void display(void)
{
	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_MULTISAMPLE);
	float t = 0;
	const vec3 X(1.0, 0.0, 0.0);
	const vec3 Y(0.0, 1.0, 0.0);
	const vec3 Z(0.0, 0.0, 1.0);
	mat4 rotation_matrix = glm::rotate(mat4(1.0f), angle * 360, Y)/*glm::rotate(mat4(1.0f), angle*720, Z)*/;
	mat4 model_matrix = (glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -50.0f)) *rotation_matrix);
	mat4 proj_matrix = glm::frustum(-1.0f, 1.0f, aspect, -aspect, 10.0f,150.0f);
	
	glUniformMatrix4fv(model_matrix_uiform, 4, GL_FALSE,&model_matrix[0][0]);
	glUniformMatrix4fv(project_matrix_uniform, 1, GL_FALSE, &proj_matrix[0][0]);
 
		
	//glDisable(GL_PRIMITIVE_RESTART);
	
	glDrawArrays(GL_TRIANGLES, 0,36);
	
	glutSwapBuffers();
}
int _tmain(int argc, char * argv[])
{
	glewExperimental = true;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(4, 1);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);
	if (glewInit()) {
		cerr << "Unable to initialize GLEW ... exiting" << endl;
		exit(EXIT_FAILURE);
	}
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(300,timer,0);
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
	glutTimerFunc(100, timer, 0);
}