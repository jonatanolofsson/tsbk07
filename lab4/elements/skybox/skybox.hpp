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

#ifndef SKYBOX_ELEMENT_HPP_
#define SKYBOX_ELEMENT_HPP_

#include "cpgl/cpgl.hpp"

namespace CPGL {
    using namespace core;
    using namespace tools;
    class Skybox : public BaseElement {
        private:
            GLuint program;
            Model* object;
            GLuint texture;

        public:
            Skybox(YAML::Node& c, BaseElement* p) : BaseElement(c, p) {
                program = load_shaders("skybox", "skybox.vert", "skybox.frag");
                object = load_model("skybox", "skybox.obj", program, "inPosition", "inNormal", "inTexCoord");
                texture = load_texture("skybox", "SkyBox512.tga");
            }

            void draw();
    };
}

#endif
