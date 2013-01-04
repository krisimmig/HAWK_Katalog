#include "object3d.h"

Object3D::Object3D()
{

}
Object3D::~Object3D()
{

}

void Object3D::setup(int _x, int _y, int _z, int _size, int _id)
{
    id = _id;
    x = _x;
    y = _y;
    z = _z;
    size = _size;
    sphereColor_1 = ofRandom(100, 255);
    sphereColor_2 = ofRandom(100, 255);
    nearBool = false;

    first_name = "Max";
    last_name = "Muster";
    image_01 = "500x500-02.jpg";
}

void Object3D::draw()
{
    // draw regular sphere
    ofColor color(125, sphereColor_2, sphereColor_1);
    ofSetColor(color);
    ofSphere(x, y, z, size);
    nearBool = false;
}

void Object3D::drawNear()
{
    // notify listeners
    if(!nearBool)
    {
        static CustomEvent nearObjectEvent;
        nearObjectEvent.objectId = id;
        ofNotifyEvent(CustomEvent::nearObject, nearObjectEvent);
    }

    nearBool = true;
    // draw wireframe sphere
    ofNoFill();
    ofColor color(255, 0, 0);
    ofSetColor(color);
    ofSphere(x, y, z, size);
    ofFill();
}

ofVec3f Object3D::getPostion()
{
    ofVec3f pos;
    pos.set(x, y, z);
    return pos;
}
