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

#include "cpgl/cpgl.hpp"
#include "cpgl/opencl.hpp"
#include <Eigen/Core>

namespace CPGL {
    const int terrain_size_X = 200;
    const int terrain_size_Z = 200;
    const float map_scale = 10;
    const int extra = 10;

    using namespace core;
    class Terrain : public BaseElement {
        private:
            enum {
                TEXTURE_TERRAIN,
                TEXTURE_GRASS,
                TEXTURE_GRASS_MASK,
                NUMBER_OF_TEXTURES
            };
            enum {
                PROGRAM_TERRAIN,
                PROGRAM_GRASS,
                NUMBER_OF_PROGRAMS
            };

            GLuint VAO;
            GLuint program[NUMBER_OF_PROGRAMS];
            GLuint texture[NUMBER_OF_TEXTURES];

            GLuint indices_id;
            GLuint grass_indices_id;
            GLuint normals_id;
            GLuint vertices_id;
            GLuint texture_id;
            int frustum_culling();
            CL::Host clProgram;
            CL::Kernel<1> generate_indices;
            cl_mem cl_indices, cl_grass_indices;
            int number_of_visible_triangles;
            void draw_grass(const int);
            void draw_ground(const int);

        public:
            Terrain(YAML::Node& c, BaseElement* p);

            void init_attribs(
                const GLuint,
                const std::string = "inPosition",
                const std::string = "inNormal");
            void init_terrain();
            void init_grass();


            template<unsigned int X, unsigned int Z> void generate_terrain(GLfloat(*heightfn)(const unsigned int, const unsigned int));
            void get_height(Vector3f& position, Vector2f& direction);
            void draw();
    };
}
