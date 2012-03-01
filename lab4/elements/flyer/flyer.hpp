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
#ifndef FLYER_ELEMENT_HPP_
#define FLYER_ELEMENT_HPP_

#include "cpgl/cpgl.hpp"

namespace CPGL {
    using namespace core;
    class Flyer : public BaseElement {
        private:
            GLuint program;
            Model* object;
        public:
            Flyer(YAML::Node& c, BaseElement* p) : core::BaseElement(c, p) {
                program = tools::load_shaders("flyer", "flyer.vert", "flyer.frag");
                object = tools::load_model("flyer", config["model"].as<std::string>(), program, "inPosition", "inNormal", "inTexCoord");
                base.translation() << 0, 1, 0;
            }

            void draw();
    };
}

#endif
