#include <stdio.h>

#include "migl.hpp"
#include "math.h"

// These are generally in math.h, but the compiler must be pleased...
# define M_PI           3.14159265358979323846  /* pi */
# define M_PI_2         1.57079632679489661923  /* pi/2 */


#define near 1.0
#define far 80.0
#define right 0.5
#define left -0.5
#define top 0.5
#define bottom -0.5
GLfloat projectionMatrix[] = {    2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                                            0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                                            0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                                            0.0f, 0.0f, -1.0f, 0.0f };
                                            
GLfloat vertices[] = {	-0.0f,-0.0f,0.0f,
						-0.8f,0.8f,0.0f,
						0.8f,0.8f,0.0f,
                        -0.07f,-0.02f,-0.02f };

GLuint program;
GLuint VAO;

GLuint texture;
GLuint mask;

void init(void)
{
	dumpInfo();

	glClearColor(1.0,0.0,0.0,0);
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	program = loadShaders("src/geometry/passthrough.vs", "src/geometry/add_one.gs", "src/geometry/white.fs");
	
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

    LoadTGATextureSimple("data/grass.tga", &texture);
    LoadTGATextureSimple("data/grass_mask.tga", &mask);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(program, "texUnit"), 0);

    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mask);
    glUniform1i(glGetUniformLocation(program, "maskUnit"), 1);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 4);

	glfwSwapBuffers();
}

int main(int argc, char *argv[])
{
	glfwInit();
    glfwOpenWindow(800,600, 0,0,0,0,0,0, GLFW_WINDOW);
    glfwSetWindowTitle("My Amazing Grass");
    glewInit();
    
	init();
	
	bool running = true;
	while(running) {
		display();
		running = !glfwGetKey( GLFW_KEY_ESC ) &&
				  glfwGetWindowParam( GLFW_OPENED );
	}
	
	glfwTerminate();
}
