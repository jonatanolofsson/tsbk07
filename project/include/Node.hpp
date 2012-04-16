#include <iostream>
#include <memory>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>

#include "Camera.hpp"

#ifndef NODE_HPP_
#define NODE_HPP_

// Abstract classes should need .cpp files, do you agree?
class Node {
private:
    void setParent(Node* newParent) {
        this->parent = newParent;
    }    
protected:
    Node* parent;
    std::list<std::shared_ptr<Node>> children;

    // Lets start with a stupid/slow solution. Change to push/pop later.
    Eigen::Affine3f baseMatrix;
    virtual Eigen::Affine3f getBaseMatrix() {
        //std::cout << baseMatrix.matrix() << std::endl << std::endl;
        if(parent == nullptr) return baseMatrix;
        return parent->getBaseMatrix() * baseMatrix;
    }
    
public:
    Node() : parent(nullptr), children(), baseMatrix(Eigen::Affine3f::Identity()) {};

    void addChild(std::shared_ptr<Node> child) {
        child->setParent(this);
        children.push_back(child);
    }

    virtual void vUpdate(double delta) = 0;
    virtual void vRender(std::shared_ptr<Camera>) = 0;
    virtual ~Node() { children.clear(); };

    void update(double delta) {
        this->vUpdate(delta);
        for (auto it = children.begin(); it != children.end(); ++it) {
            (*it)->update(delta);
        }
    };

    void render(std::shared_ptr<Camera> camera) {
        this->vRender(camera);
        for (auto it = children.begin(); it != children.end(); ++it) {
            (*it)->render(camera);
        }
    }
};

#endif
