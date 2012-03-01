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

#include "skybox.hpp"

namespace CPGL {
    void Skybox::draw()
    {
        // Send in additional params
        glUseProgram(program);
        print_error("draw skybox0");

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_FALSE, parent->base.data());

        print_error("draw skybox1");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        print_error("draw skybox2");
        glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
        print_error("draw skybox3");
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        DrawModel(object);
        print_error("draw skybox4");
        glEnable(GL_DEPTH_TEST);
        print_error("draw skybox5");
    }
}

extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Skybox(c,p));
    }
}
