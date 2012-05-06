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

#ifndef CPGL_CAMERA_HPP_
#define CPGL_CAMERA_HPP_

#include "cpgl/cpgl.hpp"
#include <Eigen/Core>
#include <Eigen/Geometry>

namespace CPGL {
    using namespace core;
    using namespace tools;
    class Camera : public BaseElement {
        public:
            GLuint program, fbo;
            Model* object;
            int mouse_click[2];
            void initFBO(const int width, const int height);

        public:
            GLuint depthBuffer, depthTexture, renderTexture, positionTexture;
            Camera(YAML::Node& c, BaseElement* p);

            void draw();

            Vector3f position();
            void rotation_from_dxdy(int dx, int dy);

            bool keyboard(unsigned char key,int x, int y);
            bool motion(int x, int y);
            bool mouse(int button, int state, int x, int y);
    };
}

#endif
