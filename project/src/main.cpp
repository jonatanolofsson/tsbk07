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
    myCamera->setProjection(2.0, 100.0, 1.5, -1.5, 1.5, -1.5);

    GLfloat vertices[8*3] = {	-0.5f,	-0.5f,	4+-0.5f,
                                0.5f,	-0.5f,	4+-0.5f,
                                0.5f,	0.5f,	4+-0.5f,
                                -0.5f,	0.5f,	4+-0.5f,			
                                -0.5f,	-0.5f,	4+0.5f,
                                0.5f,	-0.5f,	4+0.5f,
                                0.5f,	0.5f,	4+0.5f,
                                -0.5f,	0.5f,	4+0.5f};

    GLuint indices[] = {0,3,2, 0,2,1, 
                        2,3,7, 2,7,6,
                        0,4,7, 0,7,3, 
                        1,2,6, 1,6,5, 
                        4,5,6, 4,6,7, 
                        0,1,5, 0,5,4};

    GLuint cubeProgram = loadShaders("data/shaders/pass.vs", NULL, "data/shaders/white.fs");


    std::shared_ptr<Node> myEpicQuad = std::shared_ptr<Node>(
        new MeshNode(cubeProgram, vertices, 8u*3u, indices, 36u));
    theScene.add(myEpicQuad);

    GLuint rows = 10, cols = 10;
    GLfloat seeds[3*rows*cols];
     
    for(int y = 0; y < cols; ++y) {
        for(int x = 0; x < rows; ++x) {
            seeds[y*rows + x + 0] = x/10.0;
            seeds[y*rows + x + 1] = 0.0;
            seeds[y*rows + x + 2] = y/10.0;
        }
    }

    GLuint grassProgram = loadShaders("data/shaders/seeds.vs", "data/shaders/grass.gs", "data/shaders/grass.fs");
    
    std::shared_ptr<Node> someGrass = std::shared_ptr<Node>(
        new GrassNode(grassProgram, seeds, rows*cols));

    theScene.add(someGrass);
    
    theScene.mainLoop();
}

