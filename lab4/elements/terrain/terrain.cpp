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

#include "terrain.hpp"

namespace CPGL {
    Model* GenerateTerrain(TextureData *tex,
                            GLuint program,
                            char* vertexVariableName,
                            char* normalVariableName,
                            char* texCoordVariableName)
    {
        int vertexCount = tex->width * tex->height;
        int triangleCount = (tex->width-1) * (tex->height-1) * 2;
        unsigned int x, z;

        Model* model = (Model*)malloc(sizeof(Model));
        memset(model, 0, sizeof(Model));

        model->vertexArray = (GLfloat*)malloc(sizeof(Model));

        model->vertexArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
        model->normalArray = (GLfloat*)malloc(sizeof(GLfloat) * 3 * vertexCount);
        model->texCoordArray = (GLfloat*)malloc(sizeof(GLfloat) * 2 * vertexCount);
        model->indexArray = (GLuint*)malloc(sizeof(GLuint) * triangleCount*3);

        model->numVertices = vertexCount;
        model->numIndices = triangleCount*3;

        printf("bpp %d\n", tex->bpp);
        for (x = 0; x < tex->width; x++)
            for (z = 0; z < tex->height; z++)
            {
    // Vertex array. You need to scale this properly
                model->vertexArray[(x + z * tex->width)*3 + 0] = x / 1.0;
                model->vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0;
                model->vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
    // Normal vectors. You need to calculate these.
                model->normalArray[(x + z * tex->width)*3 + 0] = 0.0;
                model->normalArray[(x + z * tex->width)*3 + 1] = 1.0;
                model->normalArray[(x + z * tex->width)*3 + 2] = 0.0;
    // Texture coordinates. You may want to scale them.
                model->texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
                model->texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
            }
        for (x = 0; x < tex->width-1; x++)
            for (z = 0; z < tex->height-1; z++)
            {
            // Triangle 1
                model->indexArray[(x + z * (tex->width-1))*6 + 0] = x + z * tex->width;
                model->indexArray[(x + z * (tex->width-1))*6 + 1] = x + (z+1) * tex->width;
                model->indexArray[(x + z * (tex->width-1))*6 + 2] = x+1 + z * tex->width;
            // Triangle 2
                model->indexArray[(x + z * (tex->width-1))*6 + 3] = x+1 + z * tex->width;
                model->indexArray[(x + z * (tex->width-1))*6 + 4] = x + (z+1) * tex->width;
                model->indexArray[(x + z * (tex->width-1))*6 + 5] = x+1 + (z+1) * tex->width;
            }

        // End of terrain generation

        // Upload and set variables like LoadModelPlus
        glGenVertexArrays(1, &model->vao);
        glGenBuffers(1, &model->vb);
        glGenBuffers(1, &model->ib);
        glGenBuffers(1, &model->nb);
        if (model->texCoordArray != NULL)
            glGenBuffers(1, &model->tb);

        glBindVertexArray(model->vao);

        // VBO for vertex data
        glBindBuffer(GL_ARRAY_BUFFER, model->vb);
        glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->vertexArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, vertexVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, vertexVariableName));

        // VBO for normal data
        glBindBuffer(GL_ARRAY_BUFFER, model->nb);
        glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->normalArray, GL_STATIC_DRAW);
        glVertexAttribPointer(glGetAttribLocation(program, normalVariableName), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, normalVariableName));

        // VBO for texture coordinate data
        if (model->texCoordArray != NULL)
        {
            glBindBuffer(GL_ARRAY_BUFFER, model->tb);
            glBufferData(GL_ARRAY_BUFFER, model->numVertices*2*sizeof(GLfloat), model->texCoordArray, GL_STATIC_DRAW);
            glVertexAttribPointer(glGetAttribLocation(program, texCoordVariableName), 2, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(glGetAttribLocation(program, texCoordVariableName));
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ib);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->numIndices*sizeof(GLuint), model->indexArray, GL_STATIC_DRAW);

        return model;
    }

    TextureData ttex; // terrain

    Terrain::Terrain(YAML::Node& c, BaseElement* p) : core::BaseElement(c, p) {
        program = tools::load_shaders("terrain", "terrain.vert", "terrain.frag");
        texture = tools::load_texture("terrain", "maskros512.tga");


        glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

    // Load terrain data

        ttex = tools::load_texture_struct("terrain", "44-terrain.tga");
        object = GenerateTerrain(&ttex, program, "inPosition", "inNormal", "inTexCoord");
        tools::print_error("init terrain");
    }


    void Terrain::draw()
    {
        glUseProgram(program);

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_FALSE, get_base().data());


        glBindTexture(GL_TEXTURE_2D, texture);     // Bind Our Texture tex1
        DrawModel(object);
    }
}


extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return (BaseElement*)new CPGL::Terrain(c,p);
    }
}
