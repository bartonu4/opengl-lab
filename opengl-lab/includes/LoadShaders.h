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
#include <sstream>
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
    string  filetext;
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
		GLint   len = strlen(shaderinfo[i].filetext.c_str());
		GLint   compileStatus;
		printf(shaderinfo[i].filetext.c_str());
		
		/*char *text = new char[shaderinfo[i].filetext.size()];
		strcpy(text, shaderinfo[i].filetext.c_str());*/
		const char * text = shaderinfo[i].filetext.c_str();
		glShaderSource(shader, 1, &(text), &len);
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


//----------------------------------------------------------------------------

#ifdef __cplusplus
string getShaderFromFile(char * filename)
{
	ifstream fs(filename, ifstream::in);
	char byte;

	std::stringstream filestream;
	filestream << fs.rdbuf();
	//std::cout << filestream.str().c_str();
	return filestream.str();
}
};
#endif // __cplusplus

#endif // __LOAD_SHADERS_H__
