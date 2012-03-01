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

#include "glider.hpp"
#include <cmath>

namespace CPGL {
    Glider::Glider(YAML::Node& c, BaseElement* p) : core::BaseElement(c, p) {
        program = tools::load_shaders("glider", "glider.vert", "glider.frag");
        object = tools::load_model("glider", config["model"].as<std::string>(), program, "inPosition", "inNormal", "inTexCoord");
        std::cout << "Getting terrain" << std::endl;
        terrain = dynamic_cast<Terrain*>(get("ground"));
        std::cout << "Got terrain: " << terrain << std::endl;
    }

    void Glider::draw()
    {
        glUseProgram(program);

        float t = glutGet(GLUT_ELAPSED_TIME)/500.0;
        glUniform1f(glGetUniformLocation(program, "t"), t);

        double R = config["radius"].as<float>(1.0);
        Vector3f pos;
        pos <<
            R * std::sin(t) + config["X"].as<float>(20.0),
            0,
            R * std::cos(t) + config["Z"].as<float>(30.0);
        terrain->get_height(pos, direction);
        base.translation() = pos;

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_FALSE, get_base().data());

        DrawModel(object);
    }
}


extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Glider(c,p));
    }
}
