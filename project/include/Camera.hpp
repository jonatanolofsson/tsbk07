#include <memory>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

class Camera {
private:
    int lastMouseX, lastMouseY;
    float cameraSpeed;
    float fov;
    Eigen::Affine3f projectionMatrix;
    Eigen::Affine3f baseMatrix;
    Eigen::Vector3f cameraPosition;

    void rotation_from_dxdy(int dx, int dy);
    
public:
    Camera() : cameraSpeed(0.1f),
               fov(90.0f),
               projectionMatrix(Eigen::Affine3f::Identity()),
               baseMatrix(Eigen::Affine3f::Identity()),
               cameraPosition(0, 0, 0) {};

    void setProjection(const float near,
                        const float far,
                        const float right,
                        const float left,
                        const float top,
                        const float bottom);

    void setPerspective(const float fov,
                   const float aspect,
                   const float zNear,
                   const float zFar);
    
    void lookAt(Eigen::Vector3f eye, Eigen::Vector3f center);

    Eigen::Affine3f getCameraMatrix();
    Eigen::Affine3f getProjectionMatrix();
    Eigen::Vector3f getCameraPosition();

    void update(double delta);
    virtual ~Camera() {};
};

#endif
