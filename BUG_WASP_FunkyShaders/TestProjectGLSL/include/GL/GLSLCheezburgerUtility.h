/*
	GLSL Cheezburger Utility
	By Dan Buckstein

	Version 1.0

	COPYRIGHT 2012-2013 Dan Buckstein

	E-mail:		danbuckstein@gmail.com
	Website:	www.danbuckstein.com
	Twitter:	@dbuckstein

	Extra functions to change the way GLSL Cheezburger toolkit operates. Also 
	a few special-purpose functions, such as file reading.
*/


#ifndef _GLSLCHEEZBURGERUTILITY_H
#define _GLSLCHEEZBURGERUTILITY_H


#ifdef __cplusplus
extern "C" {
#endif	// __cplusplus


// Enable and disable printing. By default printing is turned ON.

void GLSLEnablePrinting();
void GLSLDisablePrinting();


// Enable and disable automatic program linking. By default this feature is ON.

void GLSLEnableAutoLinking();
void GLSLDisableAutoLinking();


// Compile a GLSL shader.

int GLSLCompileShader(unsigned int shaderHandle);


// Link a GLSL program.

int GLSLLinkProgram(unsigned int programHandle);


// Delete a GLSL shader or program.

void GLSLDeleteShader(unsigned int shaderHandle);
void GLSLDeleteProgram(unsigned int programHandle);


// Read a file and return its contents as a C-string.

const char *GLSLReadFile(const char *filePath);


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// _GLSLCHEEZBURGERUTILITY_H