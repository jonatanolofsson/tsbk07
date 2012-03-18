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
public:
    GrassNode(GLuint program, GLfloat seeds[], GLuint numSeeds);

    virtual void vUpdate();
    virtual void vRender();
    virtual ~GrassNode();
};

#endif
