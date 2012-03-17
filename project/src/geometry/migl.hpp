#ifndef MIGL_INCL
#define MIGL_INCL

#ifdef __cplusplus
extern "C" {
#endif

#include <GL/glew.h>
#include <GL/glfw.h>

void printError(const char *functionName);
GLuint loadShaders(const char *vertFileName, const char *geoFileName, const char *fragFileName);
void dumpInfo(void);

typedef struct TextureData		// Create A Structure for .tga loading.
{
	GLubyte	*imageData;			// Image Data (Up To 32 Bits)
	GLuint	bpp;				// Image Color Depth In Bits Per Pixel.
	GLuint	width;				// Image Width
	GLuint	height;				// Image Height
	GLuint	texID;				// Texture ID Used To Select A Texture
	GLfloat	texWidth, texHeight;
} TextureData, *TextureDataPtr;					// Structure Name

bool LoadTGATexture(char *filename, TextureData *texture);
void LoadTGATextureSimple(char *filename, GLuint *tex);

#ifdef __cplusplus
}
#endif

#endif // MIGL_INCL
