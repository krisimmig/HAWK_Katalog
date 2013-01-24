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
    gestureTimer = 0;

    // camera
    cameraHeight = 5000;
    camera.setPosition(0,0,cameraHeight);
    zoomLevel = 3;
    dragSpeed = 40.0f;
    currentDragSpeed = dragSpeed;

    // student setup
    numberOfStudents = 92;

    mySphere = new Object3D*[numberOfStudents];

    // spheres
    sphereSize = 550;
    int abstand = 750;
    int counter = 0;
    for(int i = 0; i < numberOfStudents; i++)
    {
        for(int j = 0; j < sqrt(numberOfStudents); j++)
//        for(int j = 0; j < 17; j++)
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

    if(gestureTimer > 0)
    {
        gestureTimer--;
    }
}

void WorldView::setCursor(HandCursor *c)
{
    cursor = c;
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
    kinectMove();

    camera.end();

    if(zoomLevel == 3)
    {
        ofVec3f middleWordlXYZ = mySphere[numberOfStudents/2]->getPostion();
        ofVec3f middleScreenXYZ = camera.worldToScreen(middleWordlXYZ, ofGetCurrentViewport() );

        std::string fachbereich = "GRAFIKDESIGN";
        ofSetColor(10,10,10);
        ofRectangle fachbereichRect = Helvetica22.getStringBoundingBox(fachbereich, 0,0);
        int x = middleScreenXYZ.x - fachbereichRect.width / 2;
        int y = middleScreenXYZ.y + fachbereichRect.height / 2;
        ofPushMatrix();
        ofTranslate(0,0,50);
        ofRect(x - 5, y - fachbereichRect.height - 5, fachbereichRect.width + 10, fachbereichRect.height +10);
        ofSetColor(255,255,255);

        Helvetica22.drawString(fachbereich, x, y);
        ofPopMatrix();
    }

    if(zoomLevel == 2)
    {
        for(int i = 0; i < numberOfStudents; i++)
        {
            int thisId = mySphere[i]->id;
            if(thisId % 5 == 0)
            {
                ofVec3f sphereWordlXYZ = mySphere[i]->getPostion();
                ofVec3f sphereScreenXYZ = camera.worldToScreen(sphereWordlXYZ, ofGetCurrentViewport() );

                int sphereId = mySphere[i]->id;
                ofSetColor(10,10,10);
                ofRectangle sphereIdRect = Helvetica15.getStringBoundingBox(ofToString(sphereId), 0,0);
                int x = sphereScreenXYZ.x - sphereIdRect.width / 2;
                int y = sphereScreenXYZ.y + sphereIdRect.height / 2;

                ofPushMatrix();
                ofTranslate(30,0,0);
                ofRect(x - 5, y - sphereIdRect.height - 5, sphereIdRect.width + 10, sphereIdRect.height +10);
                ofSetColor(255,255,255);
                Helvetica15.drawString(ofToString(sphereId), x, y);
                ofPopMatrix();
            }
        }
    }

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

    // draw sucher
    ofNoFill();
    ofEnableAlphaBlending();
    ofSetColor(10,10,10,130);
    ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,150, 150);
    ofSetColor(10,10,10,100);
    ofEllipse(ofGetWidth() / 2, ofGetHeight()/2,140, 140);
    ofDisableAlphaBlending();

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
    if(gestureTimer == 0)
    {
        // 0 = zoomout
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

        // 1 = zoomin
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

        // change camera move speed
        switch(zoomLevel)
        {
        case 1:
            currentDragSpeed = dragSpeed * 0.5f;
            break;
        case 2:
            currentDragSpeed = dragSpeed * 0.7f;
            break;
        case 3:
            currentDragSpeed = dragSpeed;
            break;
        }
        gestureTimer = 15;
    }
}

void WorldView::mouseDragged(ofMouseEventArgs &e)
{
    ofVec2f mouseCoords;
    mouseCoords.set(e.x, e.y);
    moveScreen(mouseCoords);
}
//
void WorldView::kinectMove()
{
    if(cursor->cursorDrag)
    {
        moveScreen(cursor->moveVector);
    }
}

void WorldView::moveScreen(ofVec2f moveVector)
{
    // left - right
    if(moveVector.x > pmouseX)
    {
        camera.truck(-currentDragSpeed);
    }
    if(moveVector.x < pmouseX)
    {
        camera.truck(currentDragSpeed);
    }

    // up - down
    if(moveVector.y > pmouseY)
    {
        camera.boom(currentDragSpeed);
    }
    if(moveVector.y < pmouseY)
    {
        camera.boom(-currentDragSpeed);
    }

    pmouseX = moveVector.x;
    pmouseY = moveVector.y;
}



