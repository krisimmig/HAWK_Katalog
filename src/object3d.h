#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ofMain.h"
#include "constants.h"
#include "customevent.h"

class Object3D
{
public:
    Object3D();
    virtual ~Object3D();
    void setup(int x, int y, int z, int size, int id);
    void draw();
    void setZoomLevel(int zoomLevel);
    void setFont(ofTrueTypeFont &font1);
    void setClosestToCamera(bool closest);
    bool getClosestToCamera();
    std::string getFullName();

    ofVec3f getPostion();
    int id;
    std::string letter;
    vector<string> letters;

    ofImage image_portrait;
    bool isMaster;
    bool isABC;

private:

    int x;
    int y;
    int z;

    ofTrueTypeFont *font1;

    std::string first_name;
    std::string last_name;
    std::string fullName;
    std::string image_01;
    std::string file_portrait;

    int size;
    int zoomLevel;
    float sphereColor_1;
    float sphereColor_2;

    bool closestToCamera;


};

#endif // OBJECT3D_H
