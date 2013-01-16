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
    void setClosestToCamera(bool closest);
    bool getClosestToCamera();
    std::string getFullName();

    ofVec3f getPostion();
    int id;


private:

    int x;
    int y;
    int z;

    std::string first_name;
    std::string last_name;
    std::string image_01;

    int size;
    int zoomLevel;
    float sphereColor_1;
    float sphereColor_2;

    bool closestToCamera;

};

#endif // OBJECT3D_H
