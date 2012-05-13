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

#include <cstring>

#include "terrain.hpp"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <cmath>
#include <algorithm>
#include "cpgl/opencl.hpp"
#include "GL/glx.h"
#include "elements/camera/camera.hpp"

namespace CPGL {
    template<int HEIGHT, int BLOCKSIZE = 1>
    int index(const int row, const int col) {
        return BLOCKSIZE * (row + col * HEIGHT);
    }

    GLfloat heightmap(const unsigned int x, const unsigned int z) {
        return std::sin(0.1*x) + std::cos(0.1*z);
    }

    template<unsigned int X, unsigned int Z>
    void Terrain::generate_terrain(GLfloat(*heightfn)(const unsigned int, const unsigned int)) {
        GLfloat vertices[3*X*Z];
        GLfloat* vertex;
        for(unsigned int xi = 0; xi < X; ++xi) {
            for(unsigned int zi = 0; zi < Z; ++zi) {
                vertex = &vertices[index<Z,3>(zi,xi)];
                vertex[0] = xi / map_scale;
                vertex[1] = heightfn(xi,zi);
                vertex[2] = zi / map_scale;
                //~ std::cout << "Generated point @" << index<Z,3>(zi,xi) << ": (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ")" << std::endl;
            }
        }

        glGenBuffers(1, &vertices_id);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_id);

        //~ std::cout << "Uploading " << sizeof(vertices) << " bytes to vertex buffer, containing" << std::endl;
        //~ std::copy(vertices, vertices+sizeof(vertices)/sizeof(vertices[0]), std::ostream_iterator<GLfloat>(std::cout, " "));
        //~ std::cout << std::endl;

        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        int xp[] = {1, 1, 0, -1, -1, 0, 1};
        int zp[] = {0, -1, -1, 0, 1, 1, 0};
        float normal_weights[] = {1/8., 1/8., 2/8., 1/8., 1/8., 2/8.};
        GLfloat normals[3*X*Z];

        for(unsigned int xi = 1; xi < X-1; ++xi) {
            for(unsigned int zi = 1; zi < Z-1; ++zi) {
                Vector3f sum; sum.setZero();
                Map<Vector3f> p(&vertices[index<Z,3>(zi,xi)]);
                for (int n = 0; n < 6; ++n) {
                    Map<Vector3f> p1(&vertices[index<Z,3>(zi+zp[n],xi+xp[n])]);
                    Map<Vector3f> p2(&vertices[index<Z,3>(zi+zp[n+1],xi+xp[n+1])]);
                    sum += normal_weights[n] * ((p1-p).cross((p2-p))).normalized();
                    //~ std::cout << "p: " << p.transpose() << "\tp1: " << p1.transpose() << "\tp2: " << p2.transpose() << "\tcross: " << normal_weights[n] * ((p1-p).cross((p2-p))).normalized() << std::endl;
                }
                Map<Vector3f> normal(&normals[index<Z,3>(xi,zi)]);
                normal = sum.normalized();
                //~ std::cout << "Generated normal @" << xi << "," << zi << ": " << sum.normalized().transpose() << std::endl;
            }
        }

        glGenBuffers(1, &normals_id);
        glBindBuffer(GL_ARRAY_BUFFER, normals_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);


        GLfloat texture[2*X*Z];
        for(unsigned int xi = 0; xi < X; ++xi) {
            for(unsigned int zi = 0; zi < Z; ++zi) {
                texture[index<Z,2>(zi,xi) + 0] = xi;
                texture[index<Z,2>(zi,xi) + 1] = zi;
            }
        }

        glGenBuffers(1, &texture_id);
        glBindBuffer(GL_ARRAY_BUFFER, texture_id);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texture), texture, GL_STATIC_DRAW);

        glGenBuffers(1, &indices_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*(X-1)*(Z-1) * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
        cl_indices = clProgram.bind_gl_vbo(indices_id);
        generate_indices.set_arg<0>(cl_indices);

        glGenBuffers(1, &grass_indices_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_indices_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*(X-1)*(Z-1) * sizeof(GLuint), NULL, GL_DYNAMIC_DRAW);
        cl_grass_indices = clProgram.bind_gl_vbo(grass_indices_id);
        generate_indices.set_arg<1>(cl_grass_indices);
        std::cout << "Generated terrain" << std::endl;
    }

    void get_limits(Vector4f z, const int order_offset, const int Zmax, int& top, int& bottom) {
        //~ std::cout << "Incoming z: " << z.transpose() << std::endl;
        std::sort(z.data(), z.data()+4);
        bottom = std::max(0, (int)std::floor(z(order_offset)) - extra);
        top = std::min(Zmax, (int)std::ceil(z(order_offset+1)) + extra);
        //~ std::cout << "Get limits from z: " << z.transpose() << " using offset " << order_offset << " and " << extra << " extra. :: 0 <= [" << bottom << "," << top << "] <= " << Zmax << std::endl;
    }

    int Terrain::frustum_culling() {
        Vector4f dx;
        Vector4f zx;
        Vector2f delta;
        // 1: Generate frustum coordinates
        Matrix<float, 4, 4> frustum_Frustum; frustum_Frustum <<
                -1, 1, 1, -1, // X
                0, 0, 0, 0,   // Y
                1, 1, -1, -1, // Z
                1,1,1,1;      // 1

        Matrix4f base = get_full_base().matrix();

        auto baselu = base.lu();
        Matrix<float, 4, 4> x = baselu.solve(frustum_Frustum);
        const int lim = 10;
        int count = 0;

        while((x.row(1).array().abs() > 1e-4).any() && ++count <= lim) {
            frustum_Frustum.row(1) -= 2.0 * x.row(1);
            x = baselu.solve(frustum_Frustum);
        }
        //~ std::cout << "Finished " << count << " iterations" << std::endl;

        for(int i = 0; i < 4; ++i) x.col(i).array() /= x(3,i);
        //~ std::cout << " x : " << std::endl << x << std::endl;

                    //~ Matrix<float, 4, 4> z = base * x;
                    //~ for(int i = 0; i < 4; ++i) z.col(i).array() /= z(3,i);
                    //~ std::cout << " z : " << std::endl << z << std::endl;

        Matrix<float, 2, 4> world2map; world2map <<
            map_scale, 0.0, 0.0, 0.0,
            0.0, 0.0, map_scale, 0.0;

        Matrix<float, 2, 4> Z = world2map * x;

        //~ std::cout << " Z : " << std::endl << Z << std::endl;

        // 2: Find visible map x-limits
        //~ std::cout << "Step 2" << std::endl;
        //~ std::cout << "Min Z: " << Z.row(1).minCoeff() << " floored: " << std::floor(Z.row(1).minCoeff()) << " as int: " << (int)std::floor(Z.row(1).minCoeff()) << " compared to 0" << std::endl;
        //~ std::cout << "Max Z: " << Z.row(1).maxCoeff() << " floored: " << std::floor(Z.row(1).maxCoeff()) << " as int: " << (int)std::floor(Z.row(1).maxCoeff()) << " compared to " << terrain_size_X-1 << std::endl;
        int left = std::max(0, (int)std::floor(Z.row(1).minCoeff()) - extra);
        int right = std::min(terrain_size_X-1, (int)std::ceil(Z.row(1).maxCoeff()) + extra);

        //~ std::cout << "Clipping from " << left << " to " << right << std::endl;
        if(right <= left) return 0;

        //~ std::cout << "Step 2:1" << std::endl;
        static const int CORNER_INDEX[5] = {0, 1, 2, 3, 0};
        int order_offset = int(Z(0,0) > Z(0,3)) + int(Z(0,2) > Z(0,1));
        //~ std::cout << "Order offset: " << order_offset << std::endl;
        for(int i = 0; i < 4; ++i) {
            delta = Z.col(CORNER_INDEX[i+1]) - Z.col(CORNER_INDEX[i]);
            //~ std::cout << "Using delta: " << delta.transpose() << std::endl;
            if(std::abs(delta.x()) < 1e-7) {
                order_offset = 0;
                dx[i] = 0;
                zx[i] = 1e9;
            } else {
                dx[i] = delta.y() / delta.x();
                zx[i] = Z(1,i) - dx(i) * Z(0,i) + dx(i) * left;
            }
        }

        //~ std::cout << "dx: " << dx.transpose() << std::endl;
        //~ std::cout << "zx: " << zx.transpose() << std::endl;

        int top, bottom, N_add, N = 0;

        //~ std::cout << "Step 2:2" << std::endl;
        generate_indices.acquire_gl(&cl_indices);
        generate_indices.acquire_gl(&cl_grass_indices);

        //~ std::cout << "Step 2:3" << std::endl;

        for(int xi = left; xi < right; ++xi) {
            get_limits(zx, order_offset, terrain_size_Z-1, top, bottom);
            N_add = top-bottom;

            //~ std::cout << "generate_terrain(*,*, " << N << ", " << bottom << ", " << xi << ", " << terrain_size_Z << ")" << " :: Sweep at x=" << xi << ", adding " << N_add << ". Using limits: [" << bottom << ", " << top << "]" << std::endl;
            if(N_add <= 0) continue;
            if(N_add > 1e5) {
                std::cout << "Tried to add " << N_add << " triangles. Don't. " << std::endl;
                std::cout
                    << "Z: " << std::endl << Z << std::endl;
                    //~ sleep(10000);
                continue;
            }
            generate_indices.local_work_size[0] = N_add;
            generate_indices.global_work_size[0] = N_add;
            generate_indices.set_arg<2>(N);
            generate_indices.set_arg<3>(bottom);
            generate_indices.set_arg<4>(xi);
            //~ std::cout << "Run the kernel: " << generate_indices.local_work_size[0] << ", " << generate_indices.global_work_size[0] << std::endl;
            generate_indices();
        generate_indices.finish();
            zx += dx;
            N += N_add;
        }
        //~ std::cout << "Displaying " << N << " squares" << std::endl;

        //~ std::cout << "Step 2:4" << std::endl;
        generate_indices.finish();

        //~ std::cout << "Step 2:5" << std::endl;
        generate_indices.release_gl(&cl_grass_indices);
        generate_indices.release_gl(&cl_indices);

        return 2*N;
    }

    void Terrain::draw_grass(const int numSeeds) {
//~ std::cout << "Draw grass: " << numSeeds << std::endl;
        static float time = 0;
        time += 0.05;
        GLfloat windforce = std::sin(time);

        glUseProgram(program[PROGRAM_GRASS]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grass_indices_id);

        glEnable(GL_DEPTH_TEST);

        Vector3f camera_position = dynamic_cast<Camera*>(parent)->position();
        glUniform3fv(glGetUniformLocation(program[PROGRAM_GRASS], "cameraPosition"), 1, camera_position.data());

        glUniformMatrix4fv(glGetUniformLocation(program[PROGRAM_GRASS], "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program[PROGRAM_GRASS], "baseMatrix"), 1, GL_FALSE, get_base().data());
        //~ glUniformMatrix4fv(glGetUniformLocation(this->program, "cameraMatrix"), 1, GL_FALSE, camera->getCameraMatrix().data());

        Eigen::AngleAxis<float> rot(M_PI, Vector3f::UnitY());
        Eigen::Vector3f wind(windforce, 0.0, 1.0);
        wind = rot * wind;

        glUniform3fv(glGetUniformLocation(program[PROGRAM_GRASS], "windforce"), 1, wind.data());
        glUniform1f(glGetUniformLocation(program[PROGRAM_GRASS], "time"), time);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture[TEXTURE_GRASS]);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture[TEXTURE_GRASS_MASK]);
//~ std::cout << "Number of seeds: " << numSeeds << std::endl;
        glDrawArrays(GL_POINTS, 0, numSeeds);

        glDisable(GL_DEPTH_TEST);
    }

    void Terrain::draw_ground(const int number_of_visible_triangles) {
        glUseProgram(program[PROGRAM_TERRAIN]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);

        //FIXME: Remove?
        //~ glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_id);
        //~ glBindBuffer(GL_ARRAY_BUFFER, vertices_id);

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program[PROGRAM_TERRAIN], "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program[PROGRAM_TERRAIN], "baseMatrix"), 1, GL_FALSE, get_base().data());

        //~ std::cout << "Drawing " << number_of_visible_triangles << " triangles" << std::endl;
        glBindTexture(GL_TEXTURE_2D, texture[TEXTURE_TERRAIN]);     // Bind Our Texture tex1
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, number_of_visible_triangles*3, GL_UNSIGNED_INT, 0L);
        glEnable(GL_CULL_FACE);
    }

    void Terrain::draw()
    {
        tools::print_error("Pre-terrain draw");
        glBindVertexArray(VAO);

        number_of_visible_triangles = frustum_culling();

        draw_ground(number_of_visible_triangles);
        draw_grass(number_of_visible_triangles*3);

        tools::print_error("display terrain");
        glBindVertexArray(0);
    }


    void Terrain::init_attribs(const GLuint program, const std::string vertexVariableName, const std::string normalVariableName) {
        glBindBuffer(GL_ARRAY_BUFFER, vertices_id);
        glVertexAttribPointer(glGetAttribLocation(program, vertexVariableName.c_str()), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, vertexVariableName.c_str()));

        glBindBuffer(GL_ARRAY_BUFFER, normals_id);
        glVertexAttribPointer(glGetAttribLocation(program, normalVariableName.c_str()), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program, normalVariableName.c_str()));
    }
    void Terrain::init_terrain() {
        const std::string texCoordVariableName = "inTexCoord";

        program[PROGRAM_TERRAIN]    = tools::load_shaders("terrain", "terrain.vert", "terrain.frag");
        generate_terrain<terrain_size_X, terrain_size_Z>(heightmap);
        generate_indices.global_work_size[0] = 1;
        generate_indices.set_arg<5>(terrain_size_Z);

        texture[TEXTURE_TERRAIN]        = tools::load_texture("terrain", config["textures"]["terrain"].as<std::string>("grass.tga"), GL_TEXTURE0, true);
        glUniform1i(glGetUniformLocation(program[PROGRAM_TERRAIN], "tex"), 0); // Texture unit 0

        init_attribs(program[PROGRAM_TERRAIN]);

        glBindBuffer(GL_ARRAY_BUFFER, texture_id);
        glVertexAttribPointer(glGetAttribLocation(program[PROGRAM_TERRAIN], texCoordVariableName.c_str()), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(glGetAttribLocation(program[PROGRAM_TERRAIN], texCoordVariableName.c_str()));
    }

    void Terrain::init_grass() {
        program[PROGRAM_GRASS]          = tools::load_shaders("terrain", "seeds.vs", "grass.gs", "grass.fs");
        texture[TEXTURE_GRASS]          = tools::load_texture("terrain", config["textures"]["grass"].as<std::string>("grassstrain.tga"), GL_TEXTURE0, true);
        texture[TEXTURE_GRASS_MASK]     = tools::load_texture("terrain", config["textures"]["grass_mask"].as<std::string>("grass_mask.tga"), GL_TEXTURE1, true);
        glUniform1i(glGetUniformLocation(program[PROGRAM_GRASS], "texUnit"), 0);
        glUniform1i(glGetUniformLocation(program[PROGRAM_GRASS], "maskUnit"), 1);

        init_attribs(program[PROGRAM_GRASS]);
    }

    Terrain::Terrain(YAML::Node& c, BaseElement* p) : core::BaseElement(c, p), clProgram("terrain/culling"), generate_indices(&clProgram, "generate_indices") {
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        init_terrain();
        init_grass();
        glBindVertexArray(0);
    }
}


extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Terrain(c,p));
    }
}
