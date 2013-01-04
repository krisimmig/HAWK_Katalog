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
    void drawNear();

    ofVec3f getPostion();
    int id;
    bool nearBool;

    std::string first_name;
    std::string last_name;
    std::string image_01;

private:

    int x;
    int y;
    int z;
    int size;
    float sphereColor_1;
    float sphereColor_2;
};

#endif // OBJECT3D_H
