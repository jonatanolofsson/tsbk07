#include <chrono>
#include <Eigen/Geometry>

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

    
    LoadTGATextureSimple("data/grass.tga", &grassTexture);
    LoadTGATextureSimple("data/grass_mask.tga", &maskTexture);
}

GrassNode::~GrassNode() {
    delete[] seeds;
}

void GrassNode::vUpdate(double delta) {
    static double time = 0;
    static bool add = true;

    if(glfwGetKey('Z'))
        add = !add;

    if(add)
        time += delta/1000;
    windforce = sin(time);
}

void GrassNode::vRender(std::shared_ptr<Camera> camera) {
    glUseProgram(this->program);

    glEnable(GL_DEPTH_TEST);

    glUniform3fv(glGetUniformLocation(this->program, "cameraPosition"), 1, camera->getCameraPosition().data());
    
    glUniformMatrix4fv(glGetUniformLocation(this->program, "projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix().data());
    glUniformMatrix4fv(glGetUniformLocation(this->program, "cameraMatrix"), 1, GL_FALSE, camera->getCameraMatrix().data());
    glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, this->getBaseMatrix().data());

    Eigen::AngleAxis<float> rot(3.1415, Eigen::Vector3f(0.0,1.0,0.0));
    Eigen::Vector3f wind(windforce, 0.0, 1.0);
    wind = rot * wind;

    glUniform3fv(glGetUniformLocation(this->program, "windforce"), 1, wind.data());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTexture);
    glUniform1i(glGetUniformLocation(this->program, "texUnit"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, maskTexture);
    glUniform1i(glGetUniformLocation(this->program, "maskUnit"), 1);
    
    glBindVertexArray(this->VAO);
	glDrawArrays(GL_POINTS, 0, this->numSeeds);

    glDisable(GL_DEPTH_TEST);
}

