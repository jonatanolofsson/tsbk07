#include <GL/glew.h>
#include <GL/glfw.h>
#include <iostream>

#include "Camera.hpp"
#include "migl.hpp"

void Camera::setProjection(
            const float near,
            const float far,
            const float right,
            const float left,
            const float top,
            const float bottom){
            projectionMatrix.matrix() << 2.0f*near/(right-left), 0.0f, (right+left)/(right-left), 0.0f,
                0.0f, 2.0f*near/(top-bottom), (top+bottom)/(top-bottom), 0.0f,
                0.0f, 0.0f, -(far + near)/(far - near), -2*far*near/(far - near),
                0.0f, 0.0f, -1.0f, 0.0f;
        }

void Camera::setPerspective(const float fov, const float aspect, float zNear, float zFar) {

   GLfloat xMin, xMax, yMin, yMax;

   yMax = zNear * tan(fov * M_PI / 360.0);
   yMin = -yMax;
   xMin = yMin * aspect;
   xMax = yMax * aspect;

   setProjection(zNear, zFar, xMax, xMin, yMax, yMin);
}

void Camera::lookAt(Eigen::Vector3f eye, Eigen::Vector3f center) {

    Eigen::Vector3f up = Eigen::Vector3f::UnitY();
    Eigen::Vector3f forward = center - eye;
    forward.normalize();

    Eigen::Vector3f side = forward.cross(up);
    side.normalize();
    up = side.cross(forward);
    
    baseMatrix.matrix() << side[0], up[0], -forward[0], 0,
                            side[1], up[1], -forward[1], 0,
                            side[2], up[2], -forward[2], 0,
                            0.0, 0.0, 0.0, 1.0;
                            
}

void Camera::update(double delta) {
    // TODO: Create a more general controller, passed as argument to vUpdate
    // Because it's more elegant than global functions.
    // Some things should be event driven, some things indirect.
    // Event driven: Resetting translation
    // Indirect: movement.

    printError("update() updateCamera");
    
    Eigen::Vector3f displacement = Eigen::Vector3f(0.0, 0.0, 0.0);
    if(glfwGetKey('W'))
        displacement = Eigen::Vector3f::UnitZ();

    if(glfwGetKey('S'))
        displacement = -Eigen::Vector3f::UnitZ();

    if(glfwGetKey('A'))
        displacement = Eigen::Vector3f::UnitX();

    if(glfwGetKey('D'))
        displacement = -Eigen::Vector3f::UnitX();

    if(glfwGetKey('Q'))
        displacement = Eigen::Vector3f::UnitY();
    if(glfwGetKey('E'))
        displacement = -Eigen::Vector3f::UnitY();

    baseMatrix.translation() += displacement * cameraSpeed;
    cameraPosition = baseMatrix.linear().transpose() * baseMatrix.translation();
    printError("update() calc camPos");
    
    if(glfwGetKey('R'))
        baseMatrix = Eigen::Affine3f::Identity();

    //std::cout << baseMatrix.translation() << std::endl;

    if(glfwGetKey('J')) ++fov;
    if(glfwGetKey('K')) --fov;
    
    setPerspective(fov, 1, 0.1, 50);
    
    int tempX, tempY;
    glfwGetMousePos(&tempX, &tempY);
    if(glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        float dx = lastMouseX - tempX;
        float dy = lastMouseY - tempY;

        rotation_from_dxdy(dx, dy);
    }
    lastMouseX = tempX;
    lastMouseY = tempY;
}

void Camera::rotation_from_dxdy(int dx, int dy) {
    static const double fiscale = 200.0;

    baseMatrix =
            (Eigen::AngleAxis<float>(((float)dy)/fiscale, Eigen::Vector3f::UnitX())) *
            (Eigen::AngleAxis<float>(((float)dx)/fiscale, baseMatrix.linear()*Eigen::Vector3f::UnitY())) *
            baseMatrix;
}

Eigen::Affine3f Camera::getCameraMatrix() { return baseMatrix; }
Eigen::Affine3f Camera::getProjectionMatrix() { return projectionMatrix; }
Eigen::Vector3f Camera::getCameraPosition() { return cameraPosition; };
