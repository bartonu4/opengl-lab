// olengl-lab.cpp : Defines the entry point for the console application.
//
#define BUFFER_OFFSET(x)  ((const void*) (x))

#include "stdafx.h"
#include <GL\GLEW.h>
#include <GL\freeglut.h>

#include <iostream>

#include "includes/LoadShaders.h"
enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };
GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;

using namespace std;
const char * vertexSource = // vertex shader
"#version 430 core \n"
"layout(location = 0) in vec4 vPosition; \n"
"void main(){gl_Position = vPosition;}";

const char * fragmentSource = // fragment shader
"#version 430 core \n"
"out vec4 fColor; \n"
"void "
"main() "
"{"
"fColor = vec4(0.0, 0.0, 1.0, 1.0);"
"}";
void init(void)
{
	
			glGenVertexArrays(NumVAOs, VAOs);
			glBindVertexArray(VAOs[Triangles]);
			GLfloat vertices[NumVertices][2] = {
				{ -0.90, -0.90 }, // Triangle 1
				{ 0.85, -0.90 },
				{ -0.90, 0.85 },
				{ 0.90, -0.85 }, // Triangle 2
				{ 0.90, 0.90 },
				{ -0.85, 0.90 }
			};
			glGenBuffers(NumBuffers, Buffers);
			glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),
				vertices, GL_STATIC_DRAW);
			ShaderInfo shaders[] = {
				{ GL_FRAGMENT_SHADER, getShaderFromFile("shaders/tri.frag") }, 
				{ GL_VERTEX_SHADER, getShaderFromFile("shaders/tri.vert")} 
			};
			
			GLuint program = LoadShaders(shaders);
			glUseProgram(program);
			glVertexAttribPointer(vPosition, 2, GL_FLOAT,
				GL_FALSE, 0, BUFFER_OFFSET(0));
			glEnableVertexAttribArray(vPosition);
		
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAOs[Triangles]);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	glFlush();
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
	glutMainLoop();
	return 0;
}

