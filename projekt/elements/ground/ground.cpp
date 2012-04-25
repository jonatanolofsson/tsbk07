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


#include <Eigen/Core>
#include "ground.hpp"
#include "elements/camera/camera.hpp"

namespace CPGL {
    void Ground::draw()
    {
        static const int x = 0; static const int y = 1; static const int z = 2;
        print_error("display ground00");
        glUseProgram(program);
        print_error("display ground0");

        Vector3f camera_position = dynamic_cast<Camera*>(parent)->position();
        glUniform3fv(glGetUniformLocation(program, "camera_position"), 1, camera_position.data());

        camera_position[1] = config["base_level"].as<float>(0.0);
        base.translation() = camera_position;

        // Send in additional params
        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE, get_projection());
        glUniformMatrix4fv(glGetUniformLocation(program, "baseMatrix"), 1, GL_FALSE, get_base().data());
        print_error("display ground1");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(program, "texUnit"), 0); // Texture unit 0
        print_error("display ground3");
        glBindVertexArray(groundVertexArrayObjectID);   // Select VAO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0L);

        print_error("display ground4");
    }
}

extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Ground(c,p));
    }
}
