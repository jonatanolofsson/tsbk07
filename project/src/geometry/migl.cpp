#define GL_GLEXT_PROTOTYPES
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "migl.hpp"

char* readFile(char *file)
{
	FILE *fptr;
	long length;
	char *buf;
	
	fptr = fopen(file, "rb");
	if (!fptr)
		return NULL;
	fseek(fptr, 0, SEEK_END);
	length = ftell(fptr);
	buf = (char*)malloc(length+1);
	fseek(fptr, 0, SEEK_SET);
	fread(buf, length, 1, fptr);
	fclose(fptr); 
	buf[length] = 0;
	
	return buf;
}

void printShaderInfoLog(GLuint obj)
{
	GLint infologLength = 0;
	GLint charsWritten  = 0;
	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

void printProgramInfoLog(GLuint obj)
{
	GLint infologLength = 0;
	GLint charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0) {
		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);
		free(infoLog);
	}
}

GLuint compileShaders(const char *vs, const char *gs, const char *fs)
{
	GLuint v,g,f,p;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(v, 1, &vs, NULL);
	glShaderSource(f, 1, &fs, NULL);
	glCompileShader(v);
	glCompileShader(f);
	p = glCreateProgram();
	glAttachShader(p,v);
	glAttachShader(p,f);
	
	if(gs != NULL) {
		g = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(g, 1, &gs, NULL);
		glCompileShader(g);
		printShaderInfoLog(g);
		glAttachShader(p,g);
	}
	
	glLinkProgram(p);
	glUseProgram(p);
	
	printShaderInfoLog(v);
	printShaderInfoLog(f);
	
	printProgramInfoLog(p);
	
	return p;
}

GLuint loadShaders(const char *vertFileName, const char *geoFileName, const char *fragFileName)
{
	char *vs, *gs, *fs;
	GLuint p;
	
	vs = readFile((char *)vertFileName);
	gs = readFile((char *)geoFileName);
	fs = readFile((char *)fragFileName);
	if (vs==NULL) {
		printf("Failed to load Vertex Shader source.\n");
		return 0;
	}
	if (fs==NULL) {
		printf("Failed to load Fragment Shader source.\n");
		return 0;
	}
	
	p = compileShaders(vs, gs, fs);
	if (vs != NULL) free(vs);
	if (gs != NULL) free(gs);
	if (fs != NULL) free(fs);
	return p;
}

// End of ShaderLoader


void dumpInfo(void)
{
   printf ("Vendor: %s\n", glGetString(GL_VENDOR));
   printf ("Renderer: %s\n", glGetString(GL_RENDERER));
   printf ("Version: %s\n", glGetString(GL_VERSION));
   printf ("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
   printError ("GL Errors from dumpInfo()");
}

void printError(const char *functionName)
{
   GLenum error;
   while (( error = glGetError() ) != GL_NO_ERROR) {
	  fprintf (stderr, "GL error 0x%X detected in %s\n", error, functionName);
   }
}
