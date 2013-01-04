#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofAddListener(CustomEvent::nearObject, this, &WorldView::nearObjectListener);
    ofBackground(240,240,240);
    garamondRegularH1.loadFont("fonts/AGaramondPro-Regular.otf", 22);
    garamondRegularS.loadFont("fonts/AGaramondPro-Regular.otf", 15);
    exitInfoScreenButton.setup("Exit", ofGetWidth() - 100, ofGetHeight() / 2);
    drawInfo = false;

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

    // model landscape
    model.loadModel("landscape.3ds");
    model.setPosition(ofGetWidth()*.5, ofGetHeight() * 0.75, 0);

    // student setup
    numberOfStudents = Students::countAll();
    studentIdArray = Students::getStudentIds();
    cout << "numberOfStudents: " << numberOfStudents << endl;

    mySphere = new Object3D*[numberOfStudents];

    // spheres
    sphereSize = 100;
    for(int i = 0; i < numberOfStudents; i++)
    {
        int x = ofRandom(-4000, 4000);
        int y = ofRandom(-4000, 4000);
        int z = ofRandom(-2000, 800);
        mySphere[i] = new Object3D();
        mySphere[i]->setup(x,y,z, sphereSize, studentIdArray[i]);
    }
    currentSphere = -1;

}

WorldView::~WorldView()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &WorldView::draw);
    ofRemoveListener(ofEvents().update, this, &WorldView::update);
    ofRemoveListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofRemoveListener(CustomEvent().nearObject, this, &WorldView::nearObjectListener);
    delete mySphere;
}

void WorldView::update(ofEventArgs &e)
{
    camera.setKinectMovement(cursor->kinectMovement);
    if(!exitInfoScreenButton.active)
    {
        camera.update();
        drawInfo = false;
        cursor->visible = false;
    }

    int x = camera.getCamera().getGlobalPosition().x;
    int y = camera.getCamera().getGlobalPosition().y;
    int z = camera.getCamera().getGlobalPosition().z;
    if (x > 4000 || y > 4000 || z > 1200 || x < -5000 || y < -4000 || z < -2000)
    {
        cout << "reset position" << endl;
        camera.setPosition(0,-2000,0);
    }
}

void WorldView::setCursor(HandCursor *c)
{
    cursor = c;
    cursor->visible = false;
}

void WorldView::draw(ofEventArgs &e)
{

    camera.begin();

    model.setPosition(0, 0, -2600);
    model.setScale(30,30,15);
    model.drawWireframe();
    for(int i = 0; i < numberOfStudents; i++)
    {
        ofVec3f avatarPos;
        avatarPos.x = camera.getCamera().getGlobalPosition().x;
        avatarPos.y = camera.getCamera().getGlobalPosition().y;
        avatarPos.z = camera.getCamera().getGlobalPosition().z;

        ofVec3f spherePos = mySphere[i]->getPostion();
        float distance = avatarPos.distance(spherePos);
        if(distance < sphereSize * 2.2)
        {
            mySphere[i]->drawNear();
            currentSphere = i;
        }
        else
        {
            mySphere[i]->draw();
        }
    }
    camera.end();

    if(drawInfo)
    {
        drawSphereInfo();
    }
}

void WorldView::nearObjectListener(CustomEvent &e)
{
    cursor->visible = true;
    exitInfoScreenButton.active = true;
    drawInfo = true;
}

void WorldView::drawSphereInfo()
{
    // currentSphere is -1 at start of program
    if(currentSphere > -1)
    {
        // sphere id changed? update student if so
        if( currentStudent.id != mySphere[currentSphere]->id)
        {
            currentStudent.setup(mySphere[currentSphere]->id);
        }
        currentStudent.drawImage(250, 250, 300);
        ofSetColor(0, 255, 0);
        garamondRegularH1.drawString(currentStudent.first_name, 5, 30);
        garamondRegularH1.drawString(currentStudent.last_name, 5, 80);
        garamondRegularH1.drawString(currentStudent.titel, 5, 120);
    }
}

void WorldView::keyPressed(ofKeyEventArgs &e)
{

}
