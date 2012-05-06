/**
 * Copyright 2011, 2012 Jonatan Olofsson
 *
 * This file is part of C++ GL Framework (CPGL).
 *
 * CPGL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CPGL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CPGL.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <Eigen/Core>
#include "screen.hpp"
#include "elements/camera/camera.hpp"

namespace CPGL {

    Screen::Screen(YAML::Node& c, BaseElement* p) : BaseElement(c, p), clProgram("screen/blur"), blurrKernel(&clProgram, "blurr") {
        GLfloat positions[] = {-1.0f, -1.0f, 0.0f,
                                        1.0f, -1.0f, 0.0f,
                                        -1.0f,  1.0f, 0.0f,
                                        1.0f, 1.0f, 0.0f};

        GLuint indices[] = {0, 1, 2, 1, 3, 2};
        GLfloat texturePos[] = {0, 0,
                                  1, 0,
                                  0, 1,
                                  1, 1};
        tools::print_error("init screen 5");
        program = tools::load_shaders("screen", "pass.vs", "screen.fs");

        GLuint vertexVBO, indexVBO, texCoordVBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &vertexVBO);
        glGenBuffers(1, &indexVBO);
        tools::print_error("MeshNode genBuffers");

        glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
        tools::print_error("MeshNode inPosition");

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
        tools::print_error("MeshNode indices");

        glGenBuffers(1, &texCoordVBO);
        glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
        tools::print_error("MeshNode genCoordVBO");
        glBufferData(GL_ARRAY_BUFFER, sizeof(texturePos), texturePos, GL_STATIC_DRAW);
        tools::print_error("MeshNode BufferVBOData");
        glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, NULL);
        tools::print_error("MeshNode intexcoord?");
        glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));
        tools::print_error("MeshNode inTexCoord");

        tools::print_error("init screen");



        ///// OpenCL code for blurred texture
        blurrKernel.local_work_size[0] = 32;
        blurrKernel.local_work_size[1] = 32;
        blurrKernel.global_work_size[0] = 1024;
        blurrKernel.global_work_size[1] = 1024;

        // For some reason this doesnt work.
        //~ glGenTextures(1, &guassian_texture);
        //~ glBindTexture(GL_TEXTURE_2D, guassian_texture);
        //~ glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        // But this does. Kinda.
        guassian_texture = tools::load_texture("terrain", "grass.tga", GL_TEXTURE3, false);
        
        cl_int err;
        guassian_buffer = clCreateFromGLTexture2D(clProgram.context, CL_MEM_WRITE_ONLY, GL_TEXTURE_2D, 0, guassian_texture, &err);
        std::cout << "Creating Guassian buffer: " << CL::error_string(err) << std::endl;
        std::cout << "Checking status guassian buffer: " << CL::error_string(clGetGLObjectInfo(guassian_buffer, NULL, NULL)) << std::endl;

        sceneCreated = false;
    }

    void Screen::draw()
    {
        glFlush();
        // Create the blurred image, this is fucking ugly but we cant access the camera untill here so..
        if(!sceneCreated) {
            cl_int err;
            GLuint renderTexture = dynamic_cast<Camera*>(get("camera"))->renderTexture;
            scene_buffer = clCreateFromGLTexture2D(clProgram.context, CL_MEM_READ_ONLY, GL_TEXTURE_2D, 0, renderTexture, &err);
            std::cout << "Creating shared input image: " << CL::error_string(err) << std::endl;
            std::cout << "Checking status scene: " << CL::error_string(clGetGLObjectInfo(scene_buffer, NULL, NULL)) << std::endl;
                
            err = blurrKernel.set_arg<0>(scene_buffer);
            err |= blurrKernel.set_arg<1>(guassian_buffer);
            std::cout << "Setting kernel arguments: " << CL::error_string(err) << std::endl;
            sceneCreated = true;
        }
            
        
        blurrKernel.acquire_gl(&scene_buffer);
        blurrKernel.acquire_gl(&guassian_buffer);
        blurrKernel.run();
        blurrKernel.release_gl(&scene_buffer);
        blurrKernel.release_gl(&guassian_buffer);
        blurrKernel.finish();

        // Render like normal
        
        tools::print_error("draw screen 1");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, 800, 600);
        tools::print_error("draw screen 2");
        //~ std::cout << "Draw screen 2 ok" << std::endl;

        glClearColor(0.66, 0.72, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        tools::print_error("draw screen 3");
        //~ std::cout << "Draw screen 3 ok" << std::endl;
        glUseProgram(program);

        glUniformMatrix4fv(glGetUniformLocation(this->program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        //~ glUniformMatrix4fv(glGetUniformLocation(this->program, "cameraMatrix"), 1, GL_FALSE, get_base().data());
        //~ glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, get_base().data());
        glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, base.data());
        
        glUniform3fv(glGetUniformLocation(this->program, "cameraPosition"), 1, dynamic_cast<Camera*>(get("camera"))->position().data());

        //~ static GLenum enums[] = {GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3 };

        tools::print_error("draw screen 4");
        //~ if(!camera) camera = dynamic_cast<Camera*>(get("camera"));
        //~ std::cout << "Camera then: " << camera << std::endl;
        //~ std::cout << "Camera renderTexture: " << dynamic_cast<Camera*>(get("camera"))->renderTexture << std::endl;
        //~ std::cout << "Camera now: " << dynamic_cast<Camera*>(get("camera")) << std::endl;

        //~ std::cout << "Draw screen 4 ok" << std::endl;

        
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, dynamic_cast<Camera*>(get("camera"))->renderTexture);
        glUniform1i(glGetUniformLocation(program, "texUnit"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, dynamic_cast<Camera*>(get("camera"))->depthTexture);
        glUniform1i(glGetUniformLocation(program, "zUnit"), 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, dynamic_cast<Camera*>(get("camera"))->positionTexture);
        glUniform1i(glGetUniformLocation(program, "depthUnit"), 2);

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, guassian_texture);
        glUniform1i(glGetUniformLocation(program, "blurrUnit"), 3);

        tools::print_error("draw screen 5");
        //~ std::cout << "Draw screen 5 ok" << std::endl;
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0L);
        tools::print_error("draw screen 6");
        //~ std::cout << "Draw screen 6 ok" << std::endl;
    }
}

extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Screen(c,p));
    }
}
