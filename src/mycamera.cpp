#include "mycamera.h"

MyCamera::MyCamera()
{
    h = 0;
    v = 0;

    speedin = 20;

    mousedragged = false;
    kinect = false;
    rotationRightSpeed = 0;
    rotationLeftSpeed = 0;
    rotationDownSpeed = 0;
    rotationUpSpeed = 0;
    mousesens = 20;

    decelerationMove = 0.7;
    decelerationRotate = 0.9;

    ofAddListener(ofEvents().mouseDragged,this,&MyCamera::myMyCameraMouseDragged);

    camera.setFarClip(10000);
    freecam.setPosition(0,0,0);
    freecam.setOrientation(ofVec3f(90,0,0));

    moveup = 'u';
    movedown = 'p';
    moveforward = 'w';
    movebackward = 's';
    moveright = 'd';
    moveleft = 'a';

}

//--------------------------------------------------------------
MyCamera::~MyCamera()
{
    //dtor
}

void MyCamera::update()
{
    makerotations();
    kinectRotations();
    freecam.rotate(ofRadToDeg(rotationLeftSpeed), ofVec3f(0,0,1));
    freecam.rotate(ofRadToDeg(-rotationRightSpeed), ofVec3f(0,0,1));
    freecam.tilt(ofRadToDeg(rotationUpSpeed));
    freecam.tilt(ofRadToDeg(-rotationDownSpeed));

    makecontrols(freecam);
    kinectForward(freecam);
    camera.setParent(freecam);
}

//--------------------------------------------------------------
void MyCamera::begin()
{

    camera.begin();
    mousedragged = false;

}

//--------------------------------------------------------------
void MyCamera::end()
{

    camera.end();

}

//--------------------------------------------------------------
void MyCamera::setKinect(bool _kinect)
{

    kinect = _kinect;

}

//--------------------------------------------------------------
void MyCamera::setSpeed(float _speed)
{

    speedin = _speed;

}

//--------------------------------------------------------------
void MyCamera::setKinectMovement(ofVec3f _kinectMovement)
{

    kinectMovement = _kinectMovement;

}

//--------------------------------------------------------------
void MyCamera::setPosition(float _x, float _y, float _z)
{

    freecam.setPosition(_x, _y, _z);

}

//--------------------------------------------------------------
void MyCamera::setDecelerationMove(float _decelerationMove)
{

    decelerationMove = _decelerationMove;

}

//--------------------------------------------------------------
void MyCamera::setDecelerationRotate(float _decelerationRotate)
{

    decelerationRotate = _decelerationRotate;

}

void MyCamera::setButtonsMovement(char _moveforward, char _movebackward, char _moveleft, char _moveright, char _moveup, char _movedown)
{

    moveforward = _moveforward;
    movebackward = _movebackward;
    moveleft = _moveleft;
    moveright = _moveright;
    moveup = _moveup;
    movedown = _movedown;

}

//--------------------------------------------------------------
void MyCamera::setFarClip(float _farclp)
{

    camera.setFarClip(_farclp);

}

//--------------------------------------------------------------
void MyCamera::setNearClip(float _nearclp)
{

    camera.setNearClip(_nearclp);

}

//--------------------------------------------------------------
ofNode MyCamera::getCameraNode()
{

    return freecam;

}

//--------------------------------------------------------------
ofCamera MyCamera::getCamera()
{

    return camera;

}

//--------------------------------------------------------------
void MyCamera::draw()
{
    camera.draw();
}

void MyCamera::kinectForward(ofNode& innode)
{

    innode.dolly(-speed);

    if(kinectMovement.z == 0)
    {
        speed *= decelerationMove;
    }
    else
    {
        speed += kinectMovement.z * 0.003;
    }
}

//--------------------------------------------------------------
void MyCamera::makecontrols(ofNode& innode)
{

    if(ofGetKeyPressed(moveup))
    {
        speed = speedin;
        innode.boom(speed);
        lastcharpressed = moveup;
    }

    if(ofGetKeyPressed(movedown))
    {
        speed = speedin;
        innode.boom(-speed);
        lastcharpressed = movedown;
    }

    if(ofGetKeyPressed(moveleft))
    {
        speed = speedin;
        innode.truck(-speed);
        lastcharpressed = moveleft;
    }

    if(ofGetKeyPressed(moveright))
    {
        speed = speedin;
        innode.truck(speed);
        lastcharpressed = moveright;
    }

    if(ofGetKeyPressed(movebackward))
    {
        speed = speedin;
        innode.dolly(speed);
        lastcharpressed = movebackward;
    }

    if(ofGetKeyPressed(moveforward))
    {
        speed = speedin;
        innode.dolly(-speed);
        lastcharpressed = moveforward;
    }


    if(ofGetKeyPressed() == false)
    {
        speed *= decelerationMove;

        if(lastcharpressed == moveup)
        {
            innode.boom(speed);
        }

        if(lastcharpressed == movedown)
        {
            innode.boom(-speed);
        }

        if(lastcharpressed == moveleft)
        {
            innode.truck(-speed);
        }

        if(lastcharpressed == moveright)
        {
            innode.truck(speed);
        }

        if(lastcharpressed == movebackward)
        {
            innode.dolly(speed);
        }

        if(lastcharpressed == moveforward)
        {
            innode.dolly(-speed);
        }
    }

}

//------------------------------------------------------------------------------
void MyCamera::kinectRotations()
{
    kinectRotationX = kinectMovement.x;
    kinectRotationY = kinectMovement.y;

    // left - right
    if (kinectRotationX < -50)
    {
        rotationRightSpeed = ofMap(kinectRotationX, -600, -1, 0.2, 0.001);
        h -= rotationRightSpeed;
    }

    if (kinectRotationX > 50)
    {
        rotationLeftSpeed = ofMap(kinectRotationX, 1, 600, 0.001, 0.2);
        h += rotationLeftSpeed;
    }

    // up - down
    if (kinectRotationY < -50)
    {
        rotationUpSpeed = ofMap(kinectRotationY, -600, -1, 0.2, 0.001);
        v -= rotationUpSpeed;
    }

    if (kinectRotationY > 50)
    {
        rotationDownSpeed = ofMap(kinectRotationY, 1, 600, 0.001, 0.2);
        v += rotationDownSpeed;
    }


    // decelerate
    if (rotationRightSpeed > 0.0001)
    {
        rotationRightSpeed *= decelerationRotate;
    }
    else
    {
        rotationRightSpeed = 0;
    }

    if (rotationLeftSpeed > 0.0001)
    {
        rotationLeftSpeed *= decelerationRotate;
    }
    else
    {
        rotationLeftSpeed = 0;
    }


    if (rotationDownSpeed > 0.0001)
    {
        rotationDownSpeed *= decelerationRotate;
    }
    else
    {
        rotationDownSpeed = 0;
    }

    if (rotationUpSpeed > 0.0001)
    {
        rotationUpSpeed *= decelerationRotate;
    }
    else
    {
        rotationUpSpeed = 0;
    }


    h -= rotationRightSpeed;
    h += rotationLeftSpeed;

    v -= rotationDownSpeed;
    v += rotationUpSpeed;

}
//------------------------------------------------------------------------------
void MyCamera::makerotations()
{

    if (mousedragged == true)
    {
        int button;
        ofGetMousePressed(button);
        if (ofGetMousePressed(button) && button == 0)
        {

            float differX = ofGetPreviousMouseX() - ofGetMouseX();

            if (ofGetPreviousMouseX() - ofGetMouseX() < 0)
            {
                rotationRightSpeed = ofMap(differX, -100, -1, 0.2, 0.001);
                h -= rotationRightSpeed;
            }

            if (ofGetPreviousMouseX() - ofGetMouseX() > 0)
            {
                rotationLeftSpeed = ofMap(differX, 1, 100, 0.001, 0.2);
                h += rotationLeftSpeed;
            }

            float differY = ofGetPreviousMouseY() - ofGetMouseY();

            if (ofGetPreviousMouseY() - ofGetMouseY() < 0)
            {
                rotationDownSpeed = ofMap(differY, -100, -1, 0.2, 0.001);
                v -= rotationDownSpeed;
            }

            if (ofGetPreviousMouseY() - ofGetMouseY() > 0)
            {
                rotationUpSpeed = ofMap(differY, 1, 100, 0.01, 0.3);
                v += rotationUpSpeed;
            }
        }
    }



    if (rotationRightSpeed > 0.0001)
    {
        rotationRightSpeed *= decelerationRotate;
    }
    else
    {
        rotationRightSpeed = 0;
    }

    if (rotationLeftSpeed > 0.0001)
    {
        rotationLeftSpeed *= decelerationRotate;
    }
    else
    {
        rotationLeftSpeed = 0;
    }

    if (rotationDownSpeed > 0.0001)
    {
        rotationDownSpeed *= decelerationRotate;
    }
    else
    {
        rotationDownSpeed = 0;
    }

    if (rotationUpSpeed > 0.0001)
    {
        rotationUpSpeed *= decelerationRotate;
    }
    else
    {
        rotationUpSpeed = 0;
    }


    h -= rotationRightSpeed;
    h += rotationLeftSpeed;

    v -= rotationDownSpeed;
    v += rotationUpSpeed;

}

//--------------------------------------------------------------
void MyCamera::myMyCameraMouseDragged(ofMouseEventArgs& args)
{

    mousedragged = true;

}

