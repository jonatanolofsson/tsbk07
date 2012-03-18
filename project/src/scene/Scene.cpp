#include <GL/glew.h>
#include <GL/glfw.h>
#include <thread>

#include "Scene.hpp"
#include "migl.hpp"

Scene::~Scene() {
    glfwTerminate();
}

void Scene::update() {
    // Update suddenly got a lot less existential right
    camera->update();
}

void Scene::render() {
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera->render();
    glfwSwapBuffers();
}

std::shared_ptr<Camera> Scene::getCamera() {
    return camera;
}

bool Scene::initScene(std::string windowTitle) {
    glfwInit();
    glfwOpenWindow(800,600, 0,0,0,0,0,0, GLFW_WINDOW);
    const char *title = windowTitle.c_str();
    glfwSetWindowTitle(title);
    glewInit();

    // TODO: Add real checking here.
    return true;
}

void Scene::add(std::shared_ptr<Node> child) {
    camera->addChild(child);
}

void Scene::mainLoop() {
    using namespace std::chrono;
    
    this->running = true;
    auto now = high_resolution_clock::now();
	while(this->running) {
        auto last = high_resolution_clock::now();
        auto delta = duration_cast<milliseconds>(last - now);

        //std::cout << "Measured time: " << delta.count() << " ms" << std::endl;
        
        this->update();
		this->render();
        now = last;
        printError("Main Loop");
		running = !glfwGetKey( GLFW_KEY_ESC ) &&
				  glfwGetWindowParam( GLFW_OPENED );
        // Sleep for some ms
        std::this_thread::sleep_for(milliseconds(100));
	}
}
