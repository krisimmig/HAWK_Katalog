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
    id = _id;
    x = _x;
    y = _y;
    z = _z;
    size = _size;
    sphereColor_1 = ofRandom(180, 255);
    sphereColor_2 = ofRandom(180, 255);
    nearBool = false;

    // 3d model
    subject.loadModel("platonic.3ds");
    ring.loadModel("ring.3ds");
    subjectMesh = subject.getMesh(0);
    ringMesh = ring.getMesh(0);
    rotateDegree = 0;

    // light
    objectLight.setPosition(x,y,z);
    objectLight.setSpotlight(50, 3);
//    objectLight.enable();
}

void Object3D::draw()
{
    // draw subject form
    drawObject();
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
    drawObject();
}

void Object3D::drawObject()
{
    ofPushMatrix();

    // set color
    if(nearBool)
    {
        ofSetColor(0, 255, 0);
        ofTranslate(x,y,100);
    }
    else
    {
        ofSetColor(10, 200, 50);
    }

    // draw subject form
    int height;
    if(rotateDegree < 360.0)
    {
        rotateDegree++;
        height = 60 * sin(rotateDegree * 0.05);
    }
    else
    {
        rotateDegree = 0.0;
        height = 60 * sin(rotateDegree * 0.05);
    }

    if(!nearBool) {ofTranslate(x,y,100 + height);}

    ofRotate(rotateDegree);
    subjectMesh.drawFaces();
    glPointSize(3);
    subjectMesh.drawVertices();
    ofSetColor(100,100,100);
    subjectMesh.drawWireframe();
    ofPopMatrix();

}

ofVec3f Object3D::getPostion()
{
    ofVec3f pos;
    pos.set(x, y, z);
    return pos;
}
