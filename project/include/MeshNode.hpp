#include <memory>
#include <list>

#include "migl.hpp"
#include "Node.hpp"

#ifndef MESHNODE_HPP_
#define MESHNODE_HPP_

class MeshNode : public Node {
private:
    GLfloat *mesh;
    GLuint numIndices;
    GLuint VAO;
    std::vector<GLuint> textureIDs;
    std::vector<std::string> samplers;
public:
    MeshNode(GLuint, GLfloat[], GLuint, GLuint[], GLuint, GLfloat[], GLuint);

    virtual void addTexture(const GLuint id, const std::string samplerName);
    virtual void vUpdate(double delta);
    virtual void vRender(std::shared_ptr<Camera>);
    virtual ~MeshNode();
};

#endif
