#include "object3d.h"

Object3D::Object3D()
{

}
Object3D::~Object3D()
{

}

void Object3D::setup(int _x, int _y, int _z, int _size)
{
    id = ofRandom(100);
    x = _x;
    y = _y;
    z = _z;
    size = ofRandom(_size * 0.25, _size * 3.5);
    sphereColor_1 = ofRandom(100, 255);
    sphereColor_2 = ofRandom(100, 255);
}

void Object3D::draw()
{
    ofColor color(125, sphereColor_2, sphereColor_1);
    ofSetColor(color);
    ofSphere(x, y, z, size);
}

void Object3D::drawNear()
{
    ofColor color(255, 0, 0);
    ofSetColor(color);
    ofSphere(x, y, z, size * 1.05);
}

ofVec3f Object3D::getPostion()
{
    ofVec3f pos;
    pos.set(x, y, z);
    return pos;
}
