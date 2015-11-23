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
#include "includes/soil/SOIL.h"
#include <iostream>
#include "objloader.hpp"
#include "includes/LoadShaders.h"
#include <vector>
GLuint vao[1];
GLuint vbo[1];
GLuint ebo[1];
GLuint model_matrix_uiform;
GLuint project_matrix_uniform;
GLuint texture;
GLuint program;
using namespace std;
using glm::vec3;
using glm::mat4;
void reshape(int width, int height);
void timer(int value);
float aspect;
float angle = 0;
int sizeOfVerticesB;
int sizeOfElements;

static void  APIENTRY openglCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam){

	cout << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity){
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << endl;
}
void gl_error()
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		auto str = gluErrorStringWIN(err);
		printf("%s", str);
		cerr << "opengl error: " << err << endl;
	}
}
void init(void)
{
	int a;

	glClearColor(1.f, 1.f, 1.f, 1.f);




	std::vector< glm::vec3 > vertices;
	std::vector< glm::vec2 > uvs;
	std::vector< glm::vec3 > normals; // Won't be used at the moment.
	bool res = loadOBJ("shaders/untitled.obj", vertices, uvs, normals);
	if (!res)
	{

	}
	ShaderInfo shaders[] = {
		{ GL_FRAGMENT_SHADER, getShaderFromFile("shaders/tri.frag") },
		{ GL_VERTEX_SHADER, getShaderFromFile("shaders/tri.vert") }
	};

	program = LoadShaders(shaders);
	glUseProgram(program);

	model_matrix_uiform = glGetUniformLocation(program, "model_matrix");
	project_matrix_uniform = glGetUniformLocation(program, "projection_matrix");

	// Set up the element array buffer
	/*glGenBuffers(1, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);*/

	// Set up the vertex attributes
	int sizeOfVertices = vertices.size()*sizeof(glm::vec3);
	int sizeOfUv = uvs.size()*sizeof(glm::vec2);
	sizeOfVerticesB = sizeOfVertices;
	float *vert = new float[sizeOfVerticesB];
	float *uv = new float[sizeOfUv];
	int previous = 0;
	for (int i = 0; i < vertices.size(); i++)
	{
		//cout << vertices[i].x << " " << " " << i << "\n";

	}

		glGenVertexArrays(1, vao);
		glBindVertexArray(vao[0]);

		glGenBuffers(1, vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);


		glBufferData(GL_ARRAY_BUFFER, sizeOfVertices + sizeOfUv, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeOfVertices, &vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeOfVertices, sizeOfUv, &uvs[0]);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, &sizeOfVertices);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//gl_error();

		GLuint sampler = glGetUniformLocation(program, "myTextureSampler");

		int width, height, channel;
		auto image = SOIL_load_image("shaders/cube.DDS", &width, &height, &channel, SOIL_LOAD_AUTO);
		//texture = SOIL_direct_load_DDS(
		//	"shaders/cube.dds", 
		//	SOIL_LOAD_AUTO,
		//	SOIL_CREATE_NEW_ID,
		//	SOIL_FLAG_POWER_OF_TWO
		//	//| SOIL_FLAG_MIPMAPS
		//	//| SOIL_FLAG_MULTIPLY_ALPHA
		//	//| SOIL_FLAG_COMPRESS_TO_DXT
		//	| SOIL_FLAG_DDS_LOAD_DIRECT
		//	//| SOIL_FLAG_NTSC_SAFE_RGB
		//	//| SOIL_FLAG_CoCg_Y
		//	//| SOIL_FLAG_TEXTURE_RECTANGLE
		//	);
		cout << "Load textue result " << SOIL_last_result();

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		const GLfloat red[] = { 1.0f, 0.0f, 0.0f, 1.0f }; // Opaque red.
		// Set the GL_TEXTURE_BORDER_COLOR for the sampler object
		glSamplerParameterfv(sampler, GL_TEXTURE_BORDER_COLOR, red);
		// Or alternatively, set the border color for a texture object.
		// This will be used when a texture is bound to a texture unit
		// without a corresponding sampler object.
		glActiveTexture(GL_TEXTURE0);
		float pixels[] = {
			0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 1.0f,
			0.0f, 0.0f, 0.0f
		};

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, red);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glUniform1i(sampler, 0);




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
		mat4 rotation_matrix = glm::rotate(mat4(1.0f), angle * 360, Y);//*glm::rotate(mat4(1.0f), angle*720, Z);
		mat4 model_matrix = (glm::translate(mat4(1.0f), vec3(0.0f, 0.0f, -50.0f)) *rotation_matrix);
		mat4 proj_matrix = glm::frustum(-1.0f, 1.0f, aspect, -aspect, 1.0f, 500.0f);

		glUniformMatrix4fv(model_matrix_uiform, 4, GL_FALSE, glm::value_ptr(model_matrix));
		glUniformMatrix4fv(project_matrix_uniform, 1, GL_FALSE, glm::value_ptr(proj_matrix));

		glDrawArrays(GL_TRIANGLES, 0, sizeOfVerticesB);
	   gl_error();
		glutSwapBuffers();
		
	}
	int _tmain(int argc, char * argv[])
	{

		glewExperimental = true;
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		glutInitWindowSize(512, 512);
		glutInitContextVersion(3, 3);
		glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);
		glutInitContextProfile(GLUT_CORE_PROFILE);
		
		glutCreateWindow(argv[0]);
		if (glewInit()) {
			cerr << "Unable to initialize GLEW ... exiting" << endl;
			exit(EXIT_FAILURE);
		}
		if (glDebugMessageCallback){
			cout << "Register OpenGL debug callback " << endl;
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			//glDebugMessageCallback(openglCallbackFunction, nullptr);
			GLuint unusedIds = 0;
			glDebugMessageControl(GL_DONT_CARE,
				GL_DONT_CARE,
				GL_DONT_CARE,
				0,
				&unusedIds,
				true);
		}
		else
			cout << "glDebugMessageCallback not available" << endl;
		init();
		glutDisplayFunc(display);
		glutReshapeFunc(reshape);
		glutTimerFunc(50, timer, 0);
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
		angle += 10;
		//printf("nu4");
		glutPostRedisplay();
		glutTimerFunc(100, timer, 0);
	}
	