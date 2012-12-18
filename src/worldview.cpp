#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofBackground(240,240,240);
    garamondRegularH1.loadFont("fonts/AGaramondPro-Regular.otf", 25);

    // environment setup
    ofSetVerticalSync(true);
    ofDisableArbTex();
    glEnable(GL_DEPTH_TEST);
    light.setPointLight();
    light.setPosition(2000,2000,2000);
    light.enable();

    // camera
    camera.setPosition(0,0,0);
    camera.setKinect(true);
    camera.setSpeed(10);
    camera.setDecelerationMove(0.95);
    camera.setDecelerationRotate(0.9);
    camera.setButtonsMovement('w', 's', 'a', 'd', 'u', 'j');

    for(int i = 0; i < 30; i++)
    {
        int x = ofRandom(-2000, 2000);
        int y = ofRandom(-2000, 2000);
        int z = ofRandom(-2000, 2000);
        mySphere[i].setup(x,y,z, 80);
    }

}

WorldView::~WorldView()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &WorldView::draw);
    ofRemoveListener(ofEvents().update, this, &WorldView::update);
    ofRemoveListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
}

void WorldView::update(ofEventArgs &e)
{
    camera.setKinectMovement(cursor->kinectMovement);
    camera.update();

    int x = camera.getCamera().getGlobalPosition().x;
    int y = camera.getCamera().getGlobalPosition().y;
    int z = camera.getCamera().getGlobalPosition().z;
    if (x > 8000 || y > 8000 || z > 8000 || x < -8000 || y < -8000 || z < -8000)
    {
        cout << "reset position" << endl;
        camera.setPosition(0,0,0);
    }
}

void WorldView::setCursor(HandCursor *c)
{
    cursor = c;
}

void WorldView::draw(ofEventArgs &e)
{
    ofSetColor(150, 150, 150);
    ofDrawBitmapString("WorldView", 15, 15);

    int x = camera.getCamera().getGlobalPosition().x;
    int y = camera.getCamera().getGlobalPosition().y;
    int z = camera.getCamera().getGlobalPosition().z;

    garamondRegularH1.drawString("Speed: " + ofToString(cursor->kinectMovement.z), 10, 40);
    garamondRegularH1.drawString("Rotation: " + ofToString(cursor->kinectMovement.x), 10, 80);
    garamondRegularH1.drawString("X: " + ofToString(x) + " Y:" + ofToString(y) + " Z: " + ofToString(z), 10, ofGetHeight() - 40);

    camera.begin();
    ofFill();
    for(int i = 0; i < 30; i++)
    {

        ofVec3f avatarPos;
        int x = camera.getCamera().getGlobalPosition().x;
        int y = camera.getCamera().getGlobalPosition().y;
        int z = camera.getCamera().getGlobalPosition().z;
        avatarPos.set(x, y, z);
        ofVec3f spherePos = mySphere[i].getPostion();
        float distance = avatarPos.distance(spherePos);
        if(distance < 800)
        {
            mySphere[i].drawNear();
            garamondRegularH1.drawString("Sphere ID: " + ofToString(mySphere[i].id), 300, 400);
        }
        else
        {
            mySphere[i].draw();
        }
    }

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            ofBox(i*300,j*300,200,200);
        }
    }
    camera.end();
}

void WorldView::keyPressed(ofKeyEventArgs &e)
{

}
