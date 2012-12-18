#include "my3dnode.h"

My3DNode::My3DNode()
{
    my_mesh.addVertex(ofVec3f(-1,0,1));
    my_mesh.addVertex(ofVec3f(1,0,1));
    my_mesh.addVertex(ofVec3f(1,0,-1));
    my_mesh.addVertex(ofVec3f(-1,0,-1));

    my_mesh.addColor(ofFloatColor(1.0,1.0,0.0,1.0));
    my_mesh.addColor(ofFloatColor(1.0,0.0,0.0,1.0));
    my_mesh.addColor(ofFloatColor(0.0,1.0,0.0,1.0));
    my_mesh.addColor(ofFloatColor(1.0,1.0,0.0,0.0));

    my_vbo.setMesh(my_mesh, GL_STATIC_DRAW);
}

void My3DNode::customDraw()
{
    setScale(100);
    glDisable(GL_CULL_FACE);
    rotate(0.05, getXAxis());
    my_vbo.draw(GL_QUADS, 0, 4);

    ofScale(1.1,1.1,1.1);
    my_vbo.draw(GL_LINE_LOOP, 0, 4);
}
