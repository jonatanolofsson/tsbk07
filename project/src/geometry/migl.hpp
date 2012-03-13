#ifndef MIGL_INCL
#define MIGL_INCL

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>
#include <GL/glfw.h>

void printError(const char *functionName);
GLuint loadShaders(const char *vertFileName, const char *geoFileName,	 const char *fragFileName);
void dumpInfo(void);

#ifdef __cplusplus
}
#endif

#endif // MIGL_INCL
