#include <memory>
#include <list>

#include "migl.hpp"
#include "Node.hpp"

#ifndef MESHNODE_HPP_
#define MESHNODE_HPP_

class MeshNode : public Node {
private:
    GLuint program;
    GLfloat *mesh;
    GLuint numIndices;
    GLuint VAO;
public:
    MeshNode(GLuint, GLfloat[], GLuint, GLuint[], GLuint);

    virtual void vUpdate();
    virtual void vRender();
    virtual ~MeshNode();
};

#endif
