#ifndef MY3DNODE_H
#define MY3DNODE_H

#include "ofMain.h"
#include "constants.h"

class My3DNode : public ofNode
{
    public:
        My3DNode();
        virtual void customDraw();
        ofVbo my_vbo;
        ofMesh my_mesh;

};

#endif // MY3DNODE_H
