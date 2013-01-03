#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofBackground(240,240,240);
    garamondRegularH1.loadFont("fonts/AGaramondPro-Regular.otf", 22);

    // environment setup
    ofSetVerticalSync(true);
    ofDisableArbTex();
    ofEnableAlphaBlending();
    glEnable(GL_DEPTH_TEST);
    light.setPointLight();
    light.setPosition(2000,2000,2000);
    light.enable();
    ofFill();
    // camera
    camera.setSpeed(10);
    camera.setDecelerationMove(0.95);
    camera.setDecelerationRotate(0.9);
    camera.setKinect(true);

    // fbo
    fbo.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);

    // model landscape
    model.loadModel("landscape.3ds");
    model.setPosition(ofGetWidth()*.5, ofGetHeight() * 0.75, 0);

    // spheres
    for(int i = 0; i < 50; i++)
    {
        int x = ofRandom(-4000, 4000);
        int y = ofRandom(-4000, 4000);
        int z = ofRandom(-2000, 800);
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
    if (x > 4000 || y > 4000 || z > 1200 || x < -5000 || y < -4000 || z < -2000)
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
    int x = camera.getCamera().getGlobalPosition().x;
    int y = camera.getCamera().getGlobalPosition().y;
    int z = camera.getCamera().getGlobalPosition().z;

    camera.begin();
    model.setPosition(0, 0, -2600);
    model.setScale(30,30,15);
    model.drawWireframe();
    for(int i = 0; i < 30; i++)
    {
        ofVec3f avatarPos;
        avatarPos.set(x, y, z);
        ofVec3f spherePos = mySphere[i].getPostion();
        float distance = avatarPos.distance(spherePos);
        if(distance < 800)
        {
            mySphere[i].drawNear();
            sphereInfo = ofToString(mySphere[i].id);
        }
        else
        {
            mySphere[i].draw();
        }
    }
    camera.end();

    fbo.begin();

    if(sphereInfo != "")
    {
        ofSetColor(0, 0, 0);
        ofRect(0, 0, 220, 50);
        ofSetColor(0, 0, 255);
        garamondRegularH1.drawString("Sphere ID: " + sphereInfo, 5, 30);
    }
    ofSetColor(0, 0, 0);
    ofRect(0, ofGetHeight() - 50, ofGetWidth(), 50);
    ofSetColor(0, 0, 255);
    garamondRegularH1.drawString("X: " + ofToString(x) + " Y:" + ofToString(y) + " Z: " + ofToString(z), 10, ofGetHeight() - 20);
    garamondRegularH1.drawString("Speed: " + ofToString(cursor->kinectMovement.z), 420, ofGetHeight() - 20);
    garamondRegularH1.drawString("Rotation: " + ofToString(cursor->kinectMovement.x), 700, ofGetHeight() - 20);

    fbo.end();
    fbo.draw(0,0);

}

void WorldView::keyPressed(ofKeyEventArgs &e)
{

}
