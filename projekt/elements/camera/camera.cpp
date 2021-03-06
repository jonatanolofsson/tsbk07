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

#include "camera.hpp"

namespace CPGL {

    void Camera::initFBO(const int width, const int height) {
        tools::print_error("initFBO 1");
        // FBO
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        tools::print_error("initFBO 2");

        // FBO texture2
        glGenTextures(1, &positionTexture);
        glBindTexture(GL_TEXTURE_2D, positionTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // FBO Depth
        glGenRenderbuffers(1, &depthBuffer);
        glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

        // FBO-depth-texture
        glGenTextures(1, &depthTexture);
        glBindTexture(GL_TEXTURE_2D, depthTexture);
        glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // FBO-texture
        glGenTextures(1, &renderTexture);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, positionTexture, 0);
        

        tools::print_error("initFBO 3");

        GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
        glDrawBuffers(2, DrawBuffers);

        tools::print_error("initFBO 3.5");
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "FRAME BUFFER OBJECT WAS NOT OK!" << std::endl;
        tools::print_error("initFBO 4");
    }

    Camera::Camera(YAML::Node& c, BaseElement* p) : core::BaseElement(c, p) {
        initFBO(1024,1024);
        look_at((Vector3f() << 20.5,5.5,20.5).finished(), (Vector3f() << 5,0,5).finished(), Vector3f::UnitY());
    }

    void Camera::rotation_from_dxdy(int dx, int dy) {
        static const double fiscale = 100.0;

        base =
            (AngleAxis<float>(((float)dy)/fiscale, Vector3f::UnitX()))*
            (AngleAxis<float>(((float)dx)/fiscale, base.linear()*Vector3f::UnitY()))*
            base;
    }

    bool Camera::keyboard(unsigned char key, int, int) {
        if(key == 'l') {
            rotation_from_dxdy(1,0);
            return false;
        } else if(key == 'j') {
            rotation_from_dxdy(-1,0);
            return false;
        } else if(key == 'i') {
            rotation_from_dxdy(0,1);
            return false;
        } else if(key == 'k') {
            rotation_from_dxdy(0,-1);
            return false;
        }

        Vector3f dposition; dposition.setZero();
        if(key == 'w') {
            dposition = -Vector3f::UnitZ();
        } else if(key == 's') {
            dposition = Vector3f::UnitZ();
        } else if(key == 'd') {
            dposition = Vector3f::UnitX();
        } else if(key == 'a') {
            dposition = -Vector3f::UnitX();
        } else if(key == 'e') {
            dposition = Vector3f::UnitY();
        } else if(key == 'q') {
            dposition = -Vector3f::UnitY();
        } else return false;

        base.translation() -= dposition * config["speed_factor"].as<float>(1.0) * 0.1;
        //~ std::cout << "Position: " << (base.linear().transpose()*base.translation()).transpose() << std::endl;
        return false;
    }


    bool Camera::motion(int x, int y) {
        rotation_from_dxdy(x-mouse_click[0], y-mouse_click[1]);

        mouse_click[0] = x;
        mouse_click[1] = y;
        return false;
    }


    bool Camera::mouse(int button, int state, int x, int y) {
        mouse_click[0] = x;
        mouse_click[1] = y;
        return false;
    }

    Vector3f Camera::position() {
        return -base.linear().transpose() * base.translation();
    }

    void Camera::draw() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0,0,1024,1024);

        tools::print_error("render() before FBO");

        glClearColor(0.66, 0.72, 1.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

extern "C" {
    using namespace CPGL::core;
    BaseElement* factory(YAML::Node& c, BaseElement* p) {
        return dynamic_cast<BaseElement*>(new CPGL::Camera(c,p));
    }
}
