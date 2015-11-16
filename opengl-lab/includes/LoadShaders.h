//////////////////////////////////////////////////////////////////////////////
//
//  --- LoadShaders.h ---
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __LOAD_SHADERS_H__
#define __LOAD_SHADERS_H__

#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <string>
using std::ifstream;
using std::string;
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

//----------------------------------------------------------------------------
//
//  LoadShaders() takes an array of ShaderFile structures, each of which
//    contains the type of the shader, and a pointer a C-style character
//    string (i.e., a NULL-terminated array of characters) containing the
//    entire shader source.
//
//  The array of structures is terminated by a final Shader with the
//    "type" field set to GL_NONE.
//
//  LoadShaders() returns the shader program value (as returned by
//    glCreateProgram()) on success, or zero on failure. 
//

typedef struct {
    GLenum       type;
    const char*  filename;
    GLuint       shader;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo *shaderinfo)
{
	GLuint	program = glCreateProgram();

	GLint	linked;
	int shaders_size = 2;
	for (int i = 0; i <shaders_size ; i++)
	{
		
		GLuint shader = glCreateShader(shaderinfo[i].type);
		GLint   len = strlen(shaderinfo[i].filename);
		GLint   compileStatus;

		glShaderSource(shader, 1, &shaderinfo[i].filename, &len);
		//printf(shaderinfo[i].filename);
		// compile the shader
		glCompileShader(shader);

		glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

		if (compileStatus == 0)           // some error
		{
			printf("Error comping shader.\n");
			
			GLenum err =GL_NO_ERROR;
			char error[200];
			GLsizei length;
			glGetShaderInfoLog(shader, sizeof(error), &length, error);
			printf(error);
			return 0;
		}
		glAttachShader(program, shader);
	}
	

	
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked)
	{
		printf("Error linking program\n");

		return 0;
	}

	return program;
	

	
}

const char * getShaderFromFile(char * filename)
{
	ifstream fs(filename, ifstream::in);
	char byte;
	string text;
	while (fs.get(byte))
	{
		text.push_back(byte);
	}
	std::cout << text.c_str();
	return text.c_str();
}
//----------------------------------------------------------------------------

#ifdef __cplusplus
};
#endif // __cplusplus

#endif // __LOAD_SHADERS_H__
