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

#include "flyer.hpp"

namespace CPGL {
    void Flyer::draw()
    {
        glUseProgram(program);

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_FALSE, get_base().data());


        float t = glutGet(GLUT_ELAPSED_TIME)/500.0;
        glUniform1f(glGetUniformLocation(program, "t"), t);

        DrawModel(object);
    }
}


extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Flyer(c,p));
    }
}
