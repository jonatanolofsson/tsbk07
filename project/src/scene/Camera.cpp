#include <GL/glew.h>
#include <GL/glfw.h>

#include "Camera.hpp"

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

void Camera::vUpdate() {
    // TODO: Create a more general controller, passed as argument to vUpdate
    // Because it's more elegant than global functions.
    // Some things should be event driven, some things indirect.
    // Event driven: Resetting translation
    // Indirect, movement.
    Eigen::Vector3f displacement = Eigen::Vector3f(0.0, 0.0, 0.0);
    if(glfwGetKey('W'))
        displacement = Eigen::Vector3f::UnitZ();

    if(glfwGetKey('S'))
        displacement = -Eigen::Vector3f::UnitZ();

    if(glfwGetKey('A'))
        displacement = -Eigen::Vector3f::UnitX();

    if(glfwGetKey('D'))
        displacement = Eigen::Vector3f::UnitX();

    if(glfwGetKey('Q'))
        displacement = Eigen::Vector3f::UnitY();
    if(glfwGetKey('E'))
        displacement = -Eigen::Vector3f::UnitY();

    baseMatrix.translation() += displacement * cameraSpeed;
    
    if(glfwGetKey('R'))
        baseMatrix.translation() = Eigen::Vector3f(0.0, 0.0, 0.0);

    std::cout << baseMatrix.translation() << std::endl;


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

void Camera::vRender() {
    
}
