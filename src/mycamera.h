#ifndef MYCAMERA_H
#define MYCAMERA_H

#include "ofMain.h"

class MyCamera
{
public:
    MyCamera();
    virtual ~MyCamera();

    void        myMyCameraMouseDragged(ofMouseEventArgs& args);
    void        update();
    void        begin();
    void        end();
    void        draw();

    void        setKinectMovement(ofVec3f _kinectMovement);
    void        setKinect(bool _kinect);

    void        setSpeed(float _speed);
    void        setDecelerationMove(float _decelerationMove);
    void        setDecelerationRotate(float _decelerationRotate);
    void        setPosition(float _x, float _y, float _z);
    void        setButtonsMovement(char _moveforward, char _movebackward, char _moveleft, char _moveright, char _moveup, char _movedown);
    void        setFarClip(float _farclp);
    void        setNearClip(float _nearclp);

    ofCamera    getCamera();
    ofNode      getCameraNode();

    ofCamera    camera;
    ofNode      freecam;

private:

    void        makecontrols(ofNode& innode);
    void        makerotations();

    void        kinectForward(ofNode& innode);
    void        kinectRotations();
    float       kinectRotationX;
    float       kinectRotationY;
    ofVec3f     kinectMovement;

    float       rotationRightSpeed;
    float       rotationLeftSpeed;
    float       rotationDownSpeed;
    float       rotationUpSpeed;
    float       mousesens;

    float       h;
    float       v;

    float       speed;
    float       speedin;
    float       decelerationMove;
    float       decelerationRotate;

    char        lastcharpressed;

    bool        mousedragged;
    bool        kinect;

    char        moveforward;
    char        movebackward;
    char        moveleft;
    char        moveright;
    char        moveup;
    char        movedown;


};

#endif // MYCAMERA_H
