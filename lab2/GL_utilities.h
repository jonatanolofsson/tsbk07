#ifndef _GL_UTILITIES_
#define _GL_UTILITIES_

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
#endif

void printError(const char *functionName);
GLuint loadShaders(const char *vertFileName, const char *fragFileName);
void dumpInfo(void);

#endif
