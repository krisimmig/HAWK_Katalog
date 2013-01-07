#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofAddListener(CustomEvent::nearObject, this, &WorldView::nearObjectListener);
    ofBackground(255,255,255);
    garamondRegularH1.loadFont("fonts/AGaramondPro-Regular.otf", 22);
    garamondRegularS.loadFont("fonts/AGaramondPro-Regular.otf", 15);
    exitInfoScreenButton.setup("Exit", ofGetWidth() - 100, ofGetHeight() / 2);
    drawInfo = false;

    // environment setup
    ofSetVerticalSync(true);
    ofDisableArbTex();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_FOG);
    glPointSize(2);

    light.setPointLight();
    light.setPosition(0,0,400);
    light.enable();
//    avatarLight.setSpotlight(10, 2);
//    avatarLight.setDirectional();
//    ofColor color = ofColor(255,255,255);
//    avatarLight.setDiffuseColor(color);
//    avatarLight.enable();
    score = 0;

    // fog
    glFogi(GL_FOG_MODE, GL_LINEAR);
    glHint(GL_FOG_HINT, GL_DONT_CARE);
    GLfloat fogColor[3] = {255,255,255};
//    GLfloat fogColor[3] = {0,0,0};
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_START, 800);
    fogDistance = 3500;
    glFogf(GL_FOG_END, fogDistance);

    // camera
    camera.setSpeed(5);
    camera.setDecelerationMove(0.9);
    camera.setDecelerationRotate(0.9);
    camera.setKinect(true);
    camera.setPosition(0,0,180);


    // model landscape
    landscape.loadModel("landscape.3ds");
    human.loadModel("human.3ds");

    // student setup
    numberOfStudents = Students::countAll();
    studentIdArray = Students::getStudentIds();
    cout << "numberOfStudents: " << numberOfStudents << endl;
    // load image
    if(! infoBackground.loadImage("data/studentInfo.png"))
    {
        cout << "error loading image: data/studentInfo.png" << endl;
    }

    mySphere = new Object3D*[numberOfStudents];

    // spheres
    sphereSize = 100;
    for(int i = 0; i < numberOfStudents; i++)
    {
        int x = ofRandom(-4000, 4000);
        int y = ofRandom(-4000, 4000);
        int z = ofRandom(100, 150);
        mySphere[i] = new Object3D();
        mySphere[i]->setup(x,y,z, sphereSize, studentIdArray[i]);
    }

    currentSphere = -1;
    ofFill();
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

    avatarXPos = camera.getCamera().getGlobalPosition().x;
    avatarYPos = camera.getCamera().getGlobalPosition().y;
    avatarZPos = camera.getCamera().getGlobalPosition().z;

    avatarPos.x = avatarXPos;
    avatarPos.y = avatarYPos;
    avatarPos.z = avatarZPos;

    avatarLight.setPosition(avatarPos);
    cout << "light: " << avatarLight.getPosition().x << " "<< avatarLight.getPosition().y << " "<< avatarLight.getPosition().z << endl;
    if (avatarXPos > 4000 || avatarYPos > 4000 || avatarZPos > 1200 || avatarXPos < -5000 || avatarYPos < -4000 || avatarZPos < -2000)
    {
        cout << "reset position" << endl;
        camera.setPosition(0,0,180);
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

    // draw environment
    drawLandscape();
    // fog
    ofSetColor(255,255,255);
    ofFill();
    ofSphere(avatarXPos, avatarYPos, 0, fogDistance);

    // draw human
    ofVboMesh humamMesh = human.getMesh(0);
    ofPushMatrix();
    ofTranslate(0,200,0);
    humamMesh.drawFaces();
    ofSetColor(100,100,100);
    humamMesh.drawWireframe();
    ofPopMatrix();

    // draw objects
    for(int i = 0; i < numberOfStudents; i++)
    {
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

    // scores
    drawScore();

    // if near object, draw info
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
    score++;
}

void WorldView::drawLandscape()
{
    // sky
    ofSetColor(240,240,240);
    landscape.setPosition(0, 0, 750);
    landscape.setScale(50,50,15);
    landscape.drawWireframe();

    // sky faces
    ofSetColor(200,200,240);
    landscape.setPosition(0, 0,751);
    landscape.drawFaces();

    // floor wire
    ofSetColor(100,100,100);
    landscape.setPosition(0, 0, -499);
    landscape.setScale(10,10,6);
    landscape.drawWireframe();

    // floor faces
    ofSetColor(200,200,240);
    landscape.setPosition(0, 0, -500);
    landscape.drawFaces();
}

void WorldView::drawScore()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);

    ofSetColor(10,10,10);
    garamondRegularH1.drawString("Score: " + ofToString(score) + " / 1350", 50, 50);

    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
}

void WorldView::drawSphereInfo()
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_FOG);

    ofEnableAlphaBlending();
    ofSetColor(255,255,255);

    int infoXPos = 50;
    int infoYPos = 200;
    // currentSphere is -1 at start of program
    if(currentSphere > -1)
    {
        // sphere id changed? update student if so
        if( currentStudent.id != mySphere[currentSphere]->id)
        {
            currentStudent.setup(mySphere[currentSphere]->id);
        }
        currentStudent.drawImage(infoXPos + 500, infoYPos, 300);
        infoBackground.draw(infoXPos - 30, infoYPos - 30, -1);
        ofSetColor(10, 255, 10);
        garamondRegularH1.drawString(currentStudent.first_name, infoXPos, infoYPos);
        garamondRegularH1.drawString(currentStudent.last_name, infoXPos, infoYPos + 25);
        garamondRegularH1.drawString(currentStudent.titel, infoXPos, infoYPos + 50);
    }

    ofDisableAlphaBlending();

    glEnable(GL_FOG);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

}



void WorldView::keyPressed(ofKeyEventArgs &e)
{

}
