#include <iostream>
#include <memory>
#include <list>
#include <Eigen/Core>
#include <Eigen/Geometry>

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
    virtual Eigen::Affine3f getBaseMatrix(bool withProjection) {
        //std::cout << baseMatrix.matrix() << std::endl << std::endl;
        if(parent == nullptr) return baseMatrix;
        return parent->getBaseMatrix(withProjection) * baseMatrix;
    }
    
public:
    Node() : parent(nullptr), children(), baseMatrix(Eigen::Affine3f::Identity()) {};

    void addChild(std::shared_ptr<Node> child) {
        child->setParent(this);
        children.push_back(child);
    }

    virtual void vUpdate() {};
    virtual void vRender() = 0;
    virtual ~Node() { children.clear(); };

    void update() {
        this->vUpdate();
        for (auto it = children.begin(); it != children.end(); ++it) {
            (*it)->update();
        }
    };

    void render() {
        this->vRender();
        for (auto it = children.begin(); it != children.end(); ++it) {
            (*it)->render();
        }
    }
};

#endif
