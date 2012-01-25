#ifndef loadobj_h
#define loadobj_h

#include <GL/gl.h>

typedef struct
{
  GLfloat* vertexArray;
  GLfloat* normalArray;
  GLfloat* texCoordArray;
  GLfloat* colorArray;
  GLuint* indexArray;
  int numVertices;
  int numIndices;
} Model;

Model* LoadModel(char* name);


#endif
