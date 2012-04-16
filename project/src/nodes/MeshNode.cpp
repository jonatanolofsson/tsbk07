#include <GL/glew.h>
#include <GL/glfw.h>

#include "MeshNode.hpp"
#include "migl.hpp"

// The difference between initialization and assignement matters much?
MeshNode::MeshNode(GLuint program, GLfloat vertices[], GLuint numVertices,
                   GLuint index[], GLuint indices,
                   GLfloat texturePos[], GLuint numTexPos) {
    // Lack of words makes naming hard.
    this->program = program;
    this->mesh = vertices; // Why cant I do this?
    this->numIndices = indices;
        
    GLuint vertexVBO, indexVBO, texCoordVBO;
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

    if(texturePos != NULL && numTexPos > 0) {
        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        
        glBufferData(GL_ARRAY_BUFFER, numTexPos*2*sizeof(GLfloat), texturePos, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
    }
}

MeshNode::~MeshNode() {
    delete[] mesh;
}

void MeshNode::vUpdate(double delta) {
    // Does nothing. As in: The googles, they do naaathing!
}

void MeshNode::addTexture(const std::string filename) {
    GLuint nextID = textureIDs.size();
    LoadTGATextureSimple(const_cast<char*>(filename.c_str()), &nextID);
    textureIDs.push_back(nextID);
    
    // TODO: Make changeable
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}
void MeshNode::vRender(std::shared_ptr<Camera> camera) {
    glUseProgram(this->program);

    glUniformMatrix4fv(glGetUniformLocation(this->program, "projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix().data());
    glUniformMatrix4fv(glGetUniformLocation(this->program, "cameraMatrix"), 1, GL_FALSE, camera->getCameraMatrix().data());
    glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, this->getBaseMatrix().data());

    // TODO: Make so it auto adds all textures.
    if(textureIDs.size() > 0) {
         glActiveTexture(GL_TEXTURE0);
         glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
         glUniform1i(glGetUniformLocation(this->program, "texUnit"), 0);
    }
    
    glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0L);
}
