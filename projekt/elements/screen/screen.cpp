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

    Screen::Screen(YAML::Node& c, BaseElement* p) : BaseElement(c, p) {
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
    }

    void Screen::draw()
    {
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

        //~ glUniformMatrix4fv(glGetUniformLocation(this->program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        //~ glUniformMatrix4fv(glGetUniformLocation(this->program, "cameraMatrix"), 1, GL_FALSE, get_base().data());
        //~ glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, get_base().data());
        glUniformMatrix4fv(glGetUniformLocation(this->program, "baseMatrix"), 1, GL_FALSE, base.data());

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
        glUniform1i(glGetUniformLocation(program, "depthUnit"), 1);

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
