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

#ifndef GROUND_ELEMENT_HPP_
#define GROUND_ELEMENT_HPP_

#include "cpgl/cpgl.hpp"

namespace CPGL {
    using namespace core;
    using namespace tools;
    class Ground : public BaseElement {
        private:
            GLuint program;
            GLuint texture;
            GLuint groundVertexArrayObjectID;

        public:
            Ground(YAML::Node& c, BaseElement* p) : BaseElement(c, p) {
                static const float GROUND_SIZE = 100.0f;
                GLfloat groundGlyphPosition[] = {-GROUND_SIZE, 0.0f, GROUND_SIZE,
                                                GROUND_SIZE, 0.0f, GROUND_SIZE,
                                                GROUND_SIZE, 0.0f, -GROUND_SIZE,
                                                -GROUND_SIZE, 0.0f, -GROUND_SIZE};

                GLuint groundGlyphIndices[] = {0, 1, 2, 2, 3, 0};

                GLfloat groundTexturePos[] = {0, 0,
                                              100, 0,
                                              100, 100,
                                              0, 100};

                program = load_shaders("ground", "ground.vert", "ground.frag");
                print_error("init ground-1");
                texture = load_texture("ground", config["texture"].as<std::string>("grass.tga"));
                print_error("init ground0");
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

                GLuint vertexBufferID, indexBufferID, texCoordBufferID;
                print_error("init ground1");
                glGenVertexArrays(1, &groundVertexArrayObjectID);
                glGenBuffers(1, &vertexBufferID);
                glGenBuffers(1, &indexBufferID);
                glGenBuffers(1, &texCoordBufferID);

                glBindVertexArray(groundVertexArrayObjectID);
                print_error("init ground2");

                // VBO for vertex data
                glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
                glBufferData(GL_ARRAY_BUFFER, sizeof(groundGlyphPosition), groundGlyphPosition, GL_STATIC_DRAW);
                glVertexAttribPointer(glGetAttribLocation(program, "inPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
                glEnableVertexAttribArray(glGetAttribLocation(program, "inPosition"));
                print_error("init ground3");

                glBindBuffer(GL_ARRAY_BUFFER, texCoordBufferID);
                print_error("init ground4");
                glBufferData(GL_ARRAY_BUFFER, sizeof(groundTexturePos), groundTexturePos, GL_STATIC_DRAW);

                glVertexAttribPointer(glGetAttribLocation(program, "inTexCoord"), 2, GL_FLOAT, GL_FALSE, 0, NULL);

                glEnableVertexAttribArray(glGetAttribLocation(program, "inTexCoord"));


                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(groundGlyphIndices), groundGlyphIndices, GL_STATIC_DRAW);

                print_error("init ground");
            }

            void draw();
    };
}

#endif
