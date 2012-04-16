#include <list>
#include <string>
#include <memory>
#include <GL/glew.h>

#include "MeshNode.hpp"
#include "Camera.hpp"

#ifndef SCENE_HPP_
#define SCENE_HPP_

class Scene {
private:
    void initFBO(const int windowWidth, const int windowHeight);
protected:
    bool running;
    std::shared_ptr<Camera> camera;
    std::list<std::shared_ptr<Node>> children;

    GLuint fbo;
    GLuint renderTexture;
    GLuint depthTexture;

    std::shared_ptr<MeshNode> screen;
public:
    Scene() : running(false), camera(new Camera), children() {};

    void add(std::shared_ptr<Node> child);
    void update(double delta);
    void render();
    
    bool initScene(std::string windowTitle);

    std::shared_ptr<Camera> getCamera();
    void mainLoop();
    ~Scene();
};

#endif

