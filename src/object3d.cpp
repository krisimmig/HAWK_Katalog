#include "object3d.h"

Object3D::Object3D()
{

}
Object3D::~Object3D()
{

}

void Object3D::setup(int _x, int _y, int _z, int _size, int _id)
{
    // general
    closestToCamera = false;
    id = _id;
    x = _x;
    y = _y;
    z = _z;
    size = _size;
    zoomLevel = 3;
    sphereColor_1 = ofRandom(180, 255);
    sphereColor_2 = ofRandom(180, 255);

    first_name = "Max";
    last_name = "Mustermann";
}

void Object3D::draw()
{
    int adjustedSize;
    switch(zoomLevel)
    {
    case 1:
        adjustedSize = size * 0.3;
        break;
    case 2:
        adjustedSize = size * 0.6;
        break;
    case 3:
        adjustedSize = size;
        break;
    }

    ofSetColor(50,50,50);
    ofNoFill();
    ofEllipse(x,y,z,adjustedSize,adjustedSize);
    ofFill();
    ofEllipse(x,y,z,adjustedSize - adjustedSize * 0.3,adjustedSize - adjustedSize * 0.3);

    if(closestToCamera)
    {
        ofNoFill();
        ofSetColor(50,0,100);
        ofEllipse(x,y,z,adjustedSize*1.3,adjustedSize*1.3);
        ofFill();
        ofSetColor(200,50,50);
        ofEllipse(x,y,z,adjustedSize - adjustedSize * 0.3,adjustedSize - adjustedSize * 0.3);
    }
}

std::string Object3D::getFullName()
{
    std::string fullName = first_name + " " + last_name;
    return fullName;
}

void Object3D::setZoomLevel(int _zoomLevel)
{
    zoomLevel = _zoomLevel;
}

void Object3D::setClosestToCamera(bool _closest)
{
    closestToCamera = _closest;
}

bool Object3D::getClosestToCamera()
{
    return closestToCamera;
}

ofVec3f Object3D::getPostion()
{
    ofVec3f pos;
    pos.set(x, y, z);
    return pos;
}
