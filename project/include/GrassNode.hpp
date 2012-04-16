#include <memory>
#include <list>

#include "migl.hpp"
#include "Node.hpp"

#ifndef GRASSNODE_HPP_
#define GRASSNODE_HPP_

class GrassNode : public Node {
private:
    GLuint program;
    GLfloat *seeds;
    GLuint numSeeds;
    GLuint VAO;

    GLuint grassTexture, maskTexture;

    GLfloat windforce;
public:
    GrassNode(GLuint program, GLfloat seeds[], GLuint numSeeds);

    virtual void vUpdate(double delta);
    virtual void vRender(std::shared_ptr<Camera>);
    virtual ~GrassNode();
};

#endif
