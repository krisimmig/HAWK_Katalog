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
    closestToCamera = false;
    id = _id;
    x = _x;
    y = _y;
    z = _z;
    size = 50;

    letters=ofSplitString("a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z", ",");
    letter = ofToString(letters[ofRandom(0,25)]);

    int number = ofRandom(1,5);
    switch(number)
    {
    case 1:
        fullName = "Katrin Mahler";
        break;
    case 2:
        fullName = "Stefanie Ernst";
        break;
    case 3:
        fullName = "Melanie Stahl";
        break;
    case 4:
        fullName = "Katharina Eberts";
        break;
    case 5:
        fullName = "Claudia Blau";
        break;
    }

    zoomLevel = 3;

    (ofRandom(10) > 7) ? isABC = true : isMaster = false;
    (ofRandom(10) > 8) ? isMaster = true : isMaster = false;

    int portraitNumber = ofRandom(1, 5);
    file_portrait = ofToString(portraitNumber) + ".jpg";

    // load image
    if(!image_portrait.loadImage(PORTRAITS_DIR "/" + file_portrait))
    {
        cout << "error loading image: " << file_portrait << endl;
    }
}

void Object3D::draw()
{
    int adjustedSize;
    if(zoomLevel > 1) closestToCamera = false;

    // master marker
    if(isMaster && zoomLevel < 3)
    {
        ofSetColor(200,200,0);
        ofRect(x-2,y-2,z,image_portrait.getWidth()+4, image_portrait.getHeight()+4);
    }

    // highlight portrait
    if(closestToCamera)
    {
        ofSetColor(10,10,10);
        ofRect(x-5,y-5,z,image_portrait.getWidth()+10, image_portrait.getHeight()+10);
    }
    // draw image
    ofSetColor(255,255,255);
    image_portrait.draw(x,y,z);

    if(isABC && zoomLevel == 2)
    {
        ofEnableAlphaBlending();
        ofSetColor(10,10,10,180);
        ofRect(x-5,y-5,z,image_portrait.getWidth()+10, image_portrait.getHeight()+10);
        ofDisableAlphaBlending();
        ofSetColor(255,255,255);
        font1->drawString(ofToUpper(letter), x + 15,y + 70);
    }
}

std::string Object3D::getFullName()
{
//    std::string fullName = first_name + " " + last_name;
    return fullName;
}

void Object3D::setFont(ofTrueTypeFont &_font)
{
    font1 = &_font;
}

void Object3D::setZoomLevel(int _zoomLevel)
{
    zoomLevel = _zoomLevel;
}

void Object3D::setClosestToCamera(bool _closest)
{
    closestToCamera = _closest;
}

bool Object3D::getClosestToCamera()
{
    return closestToCamera;
}

ofVec3f Object3D::getPostion()
{
    ofVec3f pos;
    pos.set(x + image_portrait.getWidth()/2, y + image_portrait.getHeight()/2 , z);
    return pos;
}
