#include "Scene.hpp"
#include "MeshNode.hpp"
#include "GrassNode.hpp"
#include "migl.hpp"

int main(int argc, char *argv[])
{
    // A Scene is more "Program/Window" I think?
    Scene theScene;

    // Should init be done in the constructor? Is it more RAII?
    theScene.initScene("My Amazing Demo");
    dumpInfo();
    
    std::shared_ptr<Camera> myCamera = theScene.getCamera();
    //myCamera->setProjection(1.0, 50.0, -.5, .5, .5, -.5);
    
    myCamera->lookAt(Eigen::Vector3f(0, 10, 0), Eigen::Vector3f(0, 10, 1));

    GLuint numSeeds = 200000;
    GLfloat seeds[3*numSeeds];
    srand(300); // Random displacement for now
 
    for(int i = 0; i < 3*numSeeds; i += 3) {
        float rx = (rand() % 100)/100.0;
        float rz = (rand() % 100)/100.0;
        
        seeds[i] = rx + (i % 1000)/20.0;
        seeds[i+1] = 0;
        seeds[i+2] = rz + (i / 100)/20.0;
    }
    
    GLuint grassProgram = loadShaders("data/shaders/seeds.vs", "data/shaders/grass.gs", "data/shaders/grass.fs");
    
    auto someGrass = std::shared_ptr<Node>(new GrassNode(grassProgram, seeds, numSeeds));
    
    #define GROUND_SIZE 100.0f
    GLfloat groundGlyphPosition[] = {-GROUND_SIZE, 0.0f, GROUND_SIZE,
                                GROUND_SIZE, 0.0f, GROUND_SIZE,
                                GROUND_SIZE, 0.0f, -GROUND_SIZE,
                                -GROUND_SIZE, 0.0f, -GROUND_SIZE};

    GLuint groundGlyphIndices[] = {0, 1, 2, 2, 3, 0};
    GLfloat groundTexturePos[] = {0, 0,
                              100, 0,
                              100, 100,
                              0, 100};

    GLuint groundProgram = loadShaders("data/shaders/billboard.vert", NULL, "data/shaders/billboard.frag");
    auto ground = std::shared_ptr<Node>(new MeshNode(groundProgram, groundGlyphPosition, 4u, groundGlyphIndices, 6, groundTexturePos, 8u));
    theScene.add(ground);
    theScene.add(someGrass);
    
    theScene.mainLoop();
}
