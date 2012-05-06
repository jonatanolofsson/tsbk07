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
#include "cpgl/opencl.hpp"
//~ #include "elements/camera/camera.hpp"

namespace CPGL {
    using namespace core;
    using namespace tools;
    class Screen : public BaseElement {
        private:
            GLuint VAO, program;
            //~ Camera* camera;
            CL::Host clProgram;
            CL::Kernel<2> blurrKernel;
            cl_mem guassian_buffer;
            cl_mem scene_buffer;

            GLuint guassian_texture;

            bool sceneCreated;
        public:
            Screen(YAML::Node& c, BaseElement* p);
            void draw();
    };
}

#endif
