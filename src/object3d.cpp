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
    // set color
    if(nearBool)
    {
        ofSetColor(255, 0, 0);
    }
    else
    {
        ofSetColor(200, sphereColor_2, sphereColor_1);
    }

    // draw subject form
    ofPushMatrix();
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

    ofTranslate(x,y,100 + height);
    ofRotate(rotateDegree);
    subjectMesh.drawFaces();
//    ring.drawFaces();
    glPointSize(3);
    subjectMesh.drawVertices();
    ofSetColor(100,100,100);
    subjectMesh.drawWireframe();
    ofPopMatrix();

    // draw ring
//    ofPushMatrix();
//    ofEnableAlphaBlending();
////    ofSetColor(125, sphereColor_2, sphereColor_1, 50);
//    ofTranslate(x,y,100);
//    ring.drawFaces();
//    ring.drawVertices();
//    ofPopMatrix();
//    ofDisableAlphaBlending();
}

ofVec3f Object3D::getPostion()
{
    ofVec3f pos;
    pos.set(x, y, z);
    return pos;
}
