#include <GL/glew.h>
#include <GL/glfw.h>

#include "GrassNode.hpp"

// The difference between initialization and assignement matters much?
GrassNode::GrassNode(GLuint program, GLfloat seeds[], GLuint numSeeds) {
    // Lack of words makes naming hard.
    this->program = program;
    this->seeds = seeds;
    this->numSeeds = numSeeds;
        
    GLuint vertexVBO, indexVBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &indexVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, numSeeds*3*sizeof(GLfloat), this->seeds, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "seedPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "seedPosition"));
}

GrassNode::~GrassNode() {
    delete seeds;
}

void GrassNode::vUpdate() {
    // Does nothing. As in: The googles, they do naaathing!
}

void GrassNode::vRender() {
    glUseProgram(this->program);

    glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, this->getBaseMatrix(true).data());
    glBindVertexArray(this->VAO);
	glDrawArrays(GL_POINTS, 0, this->numSeeds);
}

