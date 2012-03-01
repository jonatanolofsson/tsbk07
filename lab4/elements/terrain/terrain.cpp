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
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace CPGL {
    Model* GenerateTerrain(TextureData *tex,
                            GLuint program,
                            char* vertexVariableName,
                            char* normalVariableName,
                            char* texCoordVariableName,
                            double yscale)
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
                model->vertexArray[(x + z * tex->width)*3 + 1] = tex->imageData[(x + z * tex->width) * (tex->bpp/8)] / 100.0 * yscale;
                model->vertexArray[(x + z * tex->width)*3 + 2] = z / 1.0;
    // Normal vectors. You need to calculate these.
                model->normalArray[(x + z * tex->width)*3 + 0] = 0.0;
                model->normalArray[(x + z * tex->width)*3 + 1] = 1.0;
                model->normalArray[(x + z * tex->width)*3 + 2] = 0.0;
    // Texture coordinates. You may want to scale them.
                model->texCoordArray[(x + z * tex->width)*2 + 0] = x; // (float)x / tex->width;
                model->texCoordArray[(x + z * tex->width)*2 + 1] = z; // (float)z / tex->height;
            }

        int xp[] = {1, 1, 0, -1, -1, 0, 1};
        int zp[] = {0, -1, -1, 0, 1, 1, 0};
        float scales[] = {1/8., 1/8., 2/8., 1/8., 1/8., 2/8.};

        for (x = 1; x < tex->width-1; x++)
            for (z = 1; z < tex->height-1; z++)
            {
    // Normal vectors. You need to calculate these.
                Vector3f sum; sum.setZero();
                Map<Vector3f> p(&model->vertexArray[(x + z * tex->width)*3]);
                for (int n = 0; n < 6; ++n) {
                    Map<Vector3f> p1(&model->vertexArray[(x + xp[n] + (z + zp[n]) * tex->width)*3]);
                    Map<Vector3f> p2(&model->vertexArray[(x + xp[n+1] + (z + zp[n+1]) * tex->width)*3]);
                    sum += scales[n] * ((p1-p).cross((p2-p))).normalized();
                }
                Map<Vector3f> normal(&model->normalArray[(x + z * tex->width)*3]);
                normal = sum.normalized();
                //~ model->normalArray[(x + z * tex->width)*3 + 0] = 0.0;
                //~ model->normalArray[(x + z * tex->width)*3 + 1] = 1.0;
                //~ model->normalArray[(x + z * tex->width)*3 + 2] = 0.0;
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

    void Terrain::get_height(Vector3f& position, Vector2f& direction) {
        using std::floor;
        using std::ceil;
        static const float x_scale = 1.0;
        static const float z_scale = 1.0;
        float x = position.x() * x_scale; // Scale 1.0
        float z = position.z() * z_scale; // Scale 1.0

        float y1, y2, y3, dydx, dydz, height;

        y1 = object->vertexArray[(int(x) + (int(z) + 1) * ttex.width)*3 + 1]; // Lower left
        y2 = object->vertexArray[((int(x)+1) + int(z) * ttex.width)*3 + 1]; // Upper right
        float dx = x-floor(x);
        float dz = z-floor(z);
        if(dx + dz > 1) { // Lower triangle
            y3 = object->vertexArray[((int(x)+1) + (int(z)+1) * ttex.width)*3 + 1];

            dydx = (y3 - y1)/x_scale;
            dydz = (y3 - y2)/z_scale;
            height = y3 - (1-dx)*dydx - (1-dz)*dydz;
        } else {
            y3 = object->vertexArray[(int(x) + int(z) * ttex.width)*3 + 1];

            dydx = (y2 - y3)/x_scale;
            dydz = (y1 - y3)/z_scale;
            height = y3 + dx * dydx + dz * dydz;
        }
        direction << dydx, dydz;
        position[1] = height;
    }

    Terrain::Terrain(YAML::Node& c, BaseElement* p) : core::BaseElement(c, p) {
        program = tools::load_shaders("terrain", "terrain.vert", "terrain.frag");
        texture = tools::load_texture("terrain", config["texture"].as<std::string>("maskros512.tga"));
        tools::generate_mipmaps(texture);

        glUniform1i(glGetUniformLocation(program, "tex"), 0); // Texture unit 0

    // Load terrain data

        ttex = tools::load_texture_struct("terrain", config["terrain"].as<std::string>());
        object = GenerateTerrain(&ttex, program, "inPosition", "inNormal", "inTexCoord", config["scale"].as<double>(1.0));
        tools::print_error("init terrain");
    }


    void Terrain::draw()
    {
        glUseProgram(program);

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        //~ std::cout << "Base matrix: " << get_base().matrix() << std::endl;
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_FALSE, get_base().data());


        glBindTexture(GL_TEXTURE_2D, texture);     // Bind Our Texture tex1
        DrawModel(object);
    }
}


extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Terrain(c,p));
    }
}
