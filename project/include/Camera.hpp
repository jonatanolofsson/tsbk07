#include <memory>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Node.hpp"

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

class Camera : public Node {
private:
    int lastMouseX, lastMouseY;
    float cameraSpeed;
    Eigen::Affine3f projectionMatrix;

    void rotation_from_dxdy(int dx, int dy);

protected:
    Eigen::Affine3f getBaseMatrix(bool withProjection) {
        return withProjection ? projectionMatrix * baseMatrix :  baseMatrix;
    }
    
public:
    Camera() : cameraSpeed(0.1f),
               projectionMatrix(Eigen::Affine3f::Identity()) {};

    void setProjection(
            const float near,
            const float far,
            const float right,
            const float left,
            const float top,
            const float bottom);

    void vUpdate();
    void vRender();
    virtual ~Camera() {};
};

#endif
