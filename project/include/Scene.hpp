#include <list>
#include <string>
#include <memory>

#include "Node.hpp"
#include "Camera.hpp"

#ifndef SCENE_HPP_
#define SCENE_HPP_

// Or is a scene and camera the same thing? Goddamnit OOP is hard...
// I understand that Torvalds sais about Cpp breeds idiots and shit programmers..
// This is givving me a head ache.
class Scene {
protected:
    bool running;
    // A Scene should probably have a camera. But is the camera just another node?
    // It as a Projection Matrix, a "Camera Matrix" and everything in the world is its child.
    std::shared_ptr<Camera> camera;
public:
    Scene() : running(false), camera(new Camera) {};

    void add(std::shared_ptr<Node> child);
    void update();
    void render();
    
    bool initScene(std::string windowTitle);

    std::shared_ptr<Camera> getCamera();
    void mainLoop();
    ~Scene();
};

#endif

