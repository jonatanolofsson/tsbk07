#include <GL/glew.h>
#include <GL/glfw.h>

#include "MeshNode.hpp"

// The difference between initialization and assignement matters much?
MeshNode::MeshNode(GLuint program, GLfloat vertices[], GLuint numVertices, GLuint index[], GLuint indices) {
    // Lack of words makes naming hard.
    this->program = program;
    this->mesh = vertices; // Why cant I do this?
    this->numIndices = indices;
        
    GLuint vertexVBO, indexVBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &vertexVBO);
    glGenBuffers(1, &indexVBO);
	
	glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), this->mesh, GL_STATIC_DRAW);
	glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0); 
	glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices*sizeof(GLuint), index, GL_STATIC_DRAW);
}

MeshNode::~MeshNode() {
    delete mesh;
}

void MeshNode::vUpdate() {
    // Does nothing. As in: The googles, they do naaathing!
}

void MeshNode::vRender() {
    glUseProgram(this->program);

    glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, this->getBaseMatrix(true).data());
    
    glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0L);
}

