#include "testApp.h"
#include "stdio.h"

testApp::testApp()
{

}

void testApp::setup()
{
    ofEnableSmoothing();
    ofSetFrameRate(30);
    float filterFactor = 0.1f;

    garamondRegularH1.loadFont("fonts/AGaramondPro-Regular.otf", 25);

    // use app with or without kinect
    useKinect = true;
    if(useKinect)
    {
        // ---------------------------------
        // kinect setup
        // ---------------------------------
        openNIDevice.setup();
        openNIDevice.addDepthGenerator();
        openNIDevice.setMirror(true);
        openNIDevice.addUserGenerator();
        openNIDevice.setMaxNumUsers(3);
        openNIDevice.start();
    }

    viewmanager.setCursor(cursor);

}

void testApp::update()
{
    if(useKinect)
    {
        openNIDevice.update();
        // upadte cursor position
        cursor.update(cursorXPos, cursorYPos);
        // get number of current hands
        int numUsers = openNIDevice.getNumTrackedUsers();

        userInfo = ofToString(numUsers);
        if(numUsers > 0)
        {
            for (int i = 0; i < numUsers; i++)
            {
                user = &openNIDevice.getTrackedUser(i);
                userInfo += ", id: ";
                userInfo += ofToString(user->getXnID());
                user->drawSkeleton();

                // rotation
                float leftHandZ = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().z;
                float leftShoulderZ = user->getJoint(JOINT_LEFT_SHOULDER).getWorldPosition().z;
                if( (leftShoulderZ - leftHandZ) > 400 )
                {
                    // left - right
                    float leftElbowX = user->getJoint(JOINT_LEFT_ELBOW).getWorldPosition().x;
                    float leftHandX = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().x;
                    kinectMovementX = leftElbowX - leftHandX;

                    // up - down
                    float elbowY = user->getJoint(JOINT_LEFT_ELBOW).getWorldPosition().y;
                    float handY = user->getJoint(JOINT_LEFT_HAND).getWorldPosition().y;
                    kinectMovementY = elbowY - handY;
                }
                else
                {
                    kinectMovementX = 0;
                    kinectMovementY = 0;
                }

                // speed
                float rightHandZ = user->getJoint(JOINT_RIGHT_HAND).getWorldPosition().z;
                float rightShoulderZ = user->getJoint(JOINT_RIGHT_SHOULDER).getWorldPosition().z;
                if( (rightShoulderZ - rightHandZ) > 200 || (rightShoulderZ - rightHandZ) < -200  )
                {

                    kinectMovementZ = rightShoulderZ - rightHandZ;
                }
                else
                {
                    kinectMovementZ = 0;
                }
            }
        }
        cursor.kinectMovement.set(kinectMovementX, kinectMovementY, kinectMovementZ);
    }
}

void testApp::draw()
{
    ofFill();
    ofSetColor(20,230,20);
    ofRect(500, 15, 200, 50);
    ofSetColor(255,255,255);
    garamondRegularH1.drawString("User: " + userInfo, 550, 45);
}

void testApp::exit()
{
    if(useKinect)
    {
        openNIDevice.stop();
    }
}
