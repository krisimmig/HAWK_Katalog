#include "worldview.h"

WorldView::WorldView()
{
    //ctor
    ofAddListener(ofEvents().draw, this, &WorldView::draw);
    ofAddListener(ofEvents().update, this, &WorldView::update);
    ofAddListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    ofAddListener(ofEvents().keyPressed, this, &WorldView::keyPressed);
    ofAddListener(ofEvents().mouseDragged, this, &WorldView::mouseDragged);
    ofAddListener(CustomEvent().zoomChange, this, &WorldView::zoomChangeListener);

    Helvetica22.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 30);
    Helvetica15.loadFont("fonts/HelveticaNeueLTStd-Cn.otf", 15);
    ofEnableSmoothing();

    // camera
    cameraHeight = 2000;
    camera.setPosition(0,0,cameraHeight);
    zoomLevel = 3;

    // student setup
    numberOfStudents = 90;

    mySphere = new Object3D*[numberOfStudents];

    // spheres
    sphereSize = 150;
    int abstand = 250;
    int counter = 0;
    for(int i = 0; i < numberOfStudents; i++)
    {
        for(int j = 0; j < sqrt(numberOfStudents); j++)
        {
            if(counter < numberOfStudents)
            {
                int x = abstand * i;
                int y = abstand * j;
                int z = -800;
                mySphere[counter] = new Object3D();
                mySphere[counter]->setup(x,y,z, sphereSize, counter);
                counter++;
            }
        }
    }
    ofFill();
}

WorldView::~WorldView()
{
    //dtor
    ofRemoveListener(ofEvents().draw, this, &WorldView::draw);
    ofRemoveListener(ofEvents().update, this, &WorldView::update);
    ofRemoveListener(ofEvents().keyReleased, this, &WorldView::keyReleased);
    delete mySphere;
}

void WorldView::update(ofEventArgs &e)
{
    if(camera.getPosition().z < cameraHeight)
    {
        camera.dolly(40);
    }

    if(camera.getPosition().z > cameraHeight)
    {
        camera.dolly(-40);
    }

    for(int i = 0; i < numberOfStudents; i++)
    {
        mySphere[i]->setZoomLevel(zoomLevel);
    }
}

void WorldView::setCursor(HandCursor *c)
{
    cursor = c;
    cursor->visible = false;
}

void WorldView::draw(ofEventArgs &e)
{
    ofBackgroundGradient(ofColor(255), ofColor(175));
    camera.begin();

    // draw objects
    for(int i = 0; i < numberOfStudents; i++)
    {
        ofVec3f spherePos = mySphere[i]->getPostion();
        ofVec3f cameraPos = camera.getPosition();
        float distance = cameraPos.distance(spherePos);
        if(distance < 809)
        {
            mySphere[i]->setClosestToCamera(true);
            mySphere[i]->draw();
            std::string fullName = mySphere[i]->getFullName();
        }
        else
        {
            mySphere[i]->setClosestToCamera(false);
            mySphere[i]->draw();
        }
    }
    camera.end();

    for(int i = 0; i < numberOfStudents; i++)
    {
        if(mySphere[i]->getClosestToCamera())
        {
            std::string fullName = mySphere[i]->getFullName();
            int id = mySphere[i]->id;
            std::string info = fullName + " / " + ofToString(id);
            ofSetColor(10,10,10);
            ofRectangle nameRect = Helvetica22.getStringBoundingBox(info, 45,45);
            ofRect(nameRect.x, nameRect.y, nameRect.width +10, nameRect.height +10);
            ofSetColor(255,255,255);
            Helvetica22.drawString(info, 50,50);

            ofVec3f worldXYZ = mySphere[i]->getPostion();
            ofVec3f screenXYZ = camera.worldToScreen(worldXYZ, ofGetCurrentViewport());

            ofSetColor(10,10,10);
            ofRectangle idRect = Helvetica15.getStringBoundingBox(ofToString(id), 0,0);
            int x = screenXYZ.x + 47;
            int y = screenXYZ.y + 10;
            ofRect(x - 5, y - idRect.height - 5, idRect.width + 10, idRect.height +10);
            ofSetColor(255,255,255);
            Helvetica15.drawString(ofToString(id), x, y);
        }
    }
}

void WorldView::keyReleased(ofKeyEventArgs &e)
{
    switch(e.key)
    {
    case 'e':
        changeZoomLevel(1);
        break;
    case 'q':
        changeZoomLevel(0);
        break;
    }
}


void WorldView::keyPressed(ofKeyEventArgs &e)
{
    switch(e.key)
    {
    case 'w':
        camera.boom(1.0f);
        break;
    case 's':
        camera.boom(-1.0f);
        break;
    case 'a':
        camera.truck(-1.0f);
        break;
    case 'd':
        camera.truck(1.0f);
        break;
    }
}


void WorldView::changeZoomLevel(int _zoomLevel)
{
    static CustomEvent changeZoomLevel;
    changeZoomLevel.zoomLevel = _zoomLevel;
    ofNotifyEvent(CustomEvent::zoomChange, changeZoomLevel);
}

void WorldView::zoomChangeListener(CustomEvent &e)
{
    if(e.zoomLevel == 0)
    {
        if(zoomLevel < 3)
        {
            zoomLevel++;
            cameraHeight += 1000;
        }
        else
        {
            cout << "Max Zoomout." << endl;
        }
    }
    else if (e.zoomLevel == 1)
    {
        if(zoomLevel > 1)
        {
            zoomLevel--;
            cameraHeight -= 1000;
        }
        else
        {
            cout << "Max Zoomin." << endl;
        }

    }
    cout << "cameraHeight: " << cameraHeight << endl;
    cout << "zoomLevel: " << zoomLevel << endl;
}

void WorldView::mouseDragged(ofMouseEventArgs &e)
{
    float dragSpeed;
    switch(zoomLevel)
    {
    case 1:
        dragSpeed = 10.0f;
        break;
    case 2:
        dragSpeed = 20.0f;
        break;
    case 3:
        dragSpeed = 30.0f;
        break;
    }

    if(e.x > pmouseX)
    {
        camera.truck(-dragSpeed);
    }
    if(e.x < pmouseX)
    {
        camera.truck(dragSpeed);
    }
    if(e.y > pmouseY)
    {
        camera.boom(dragSpeed);
    }
    if(e.y < pmouseY)
    {
        camera.boom(-dragSpeed);
    }

    pmouseX = e.x;
    pmouseY = e.y;
}



