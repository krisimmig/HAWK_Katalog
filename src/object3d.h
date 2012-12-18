#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "ofMain.h"
#include "constants.h"

class Object3D
{
public:
    Object3D();
    virtual ~Object3D();
    void setup(int x, int y, int z, int size);
    void draw();
    void drawNear();
    ofVec3f getPostion();
    int id;

private:

    int x;
    int y;
    int z;
    int size;
    float sphereColor_1;
    float sphereColor_2;
};

#endif // OBJECT3D_H
