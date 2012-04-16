#include <GL/glew.h>
#include <GL/glfw.h>
#include <thread>

#include "Scene.hpp"
#include "migl.hpp"
#include "MeshNode.hpp"

Scene::~Scene() {
    glfwTerminate();
}

void Scene::update(double delta) {
    camera->update(delta);
    printError("update() camera");
    int i = 0;
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->update(delta);
        printError("update() object " + i);
        i++;
    }
}

void Scene::add(std::shared_ptr<Node> child) {
    children.push_back(child);
}

void Scene::render() {
    // Render to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0,0,800,600);
    
    printError("render() before FBO");
    
    glClearColor(0.66, 0.72, 1.0, 1.0);
   	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    int i = 0;
    for (auto it = children.begin(); it != children.end(); ++it) {
        (*it)->render(camera);
        printError("render() object " + i);
        i++;
    }
    
    printError("render() to fbo");
    
    // render to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, 800, 600);
    
    glClearColor(0.66, 0.72, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
    screen->render(camera);

    printError("render() to screen");
    glfwSwapBuffers();
}

void Scene::initFBO(const int width, const int height) {
    printError("initFBO 1");
    // FBO
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    // FBO-texture
    glGenTextures(1, &renderTexture);
    glBindTexture(GL_TEXTURE_2D, renderTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    
    printError("initFBO 2");

    // FBO Depth
    GLuint depthBuffer;
    glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
    
    // FBO-depth-texture
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    printError("initFBO 3");
    
    GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
	glDrawBuffers(2, DrawBuffers);
    
    printError("initFBO 3.5");
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "FRAME BUFFER OBJECT WAS NOT OK!" << std::endl;
    printError("initFBO 4");
    GLfloat positions[] = {-1.0f, -1.0f, 0.0f,
                                    1.0f, -1.0f, 0.0f,
                                    -1.0f,  1.0f, 0.0f,
                                    1.0f, 1.0f, 0.0f};

    GLuint indices[] = {0, 1, 2, 1, 3, 2};
    GLfloat texturePos[] = {0, 0,
                              1, 0,
                              0, 1,
                              1, 1};
    printError("initFBO 5");
    GLuint screenProgram = loadShaders("data/shaders/pass.vs", NULL, "data/shaders/screen.fs");
    this->screen = std::shared_ptr<MeshNode>(new MeshNode(screenProgram, positions, 4u, indices, 6, texturePos, 8u));
    this->screen->addTexture(renderTexture, "texUnit");
    this->screen->addTexture(depthTexture, "depthUnit");
    
    printError("initFBO");
}

bool Scene::initScene(std::string windowTitle) {

    const int width = 800;
    const int height = 600;

    // Hints
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwInit();
    glfwOpenWindow(width,height, 0,0,0,0,0,0, GLFW_WINDOW);
    const char *title = windowTitle.c_str();
    glfwSetWindowTitle(title);
    glewInit();

    initFBO(width, height);

    // TODO: Add real checking here.
    return true;
}

void Scene::mainLoop() {
    using namespace std::chrono;

    
    this->running = true;
    auto now = high_resolution_clock::now();
    printError("Start mainLoop");
	while(this->running) {
        auto last = high_resolution_clock::now();
        auto delta = duration_cast<milliseconds>(last - now);

        //std::cout << "Measured time: " << delta.count() << " ms" << std::endl;
        this->update(delta.count());
		this->render();
        now = last;
        printError("Main Loop");
		running = !glfwGetKey( GLFW_KEY_ESC ) &&
				  glfwGetWindowParam( GLFW_OPENED );
        // Sleep for some ms
        std::this_thread::sleep_for(milliseconds(20));
	}
}

std::shared_ptr<Camera> Scene::getCamera() {
    return camera;
}
