#include "mainmenu.h"

MainMenu::MainMenu()
{
    //ctor
}

MainMenu::~MainMenu()
{
    //dtor
}

void MainMenu::setupButtons()
{
    deptGD_Button.setup("GD");
    deptAD_Button.setup("AD");
    deptDM_Button.setup("DM");
}

void MainMenu::draw()
{
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("Mainview", ofGetWidth() / 2, 50);

    deptGD_Button.draw(ofGetWidth() / 2 , 250);
    deptAD_Button.draw(ofGetWidth() / 2 , 350);
    deptDM_Button.draw(ofGetWidth() / 2 , 450);

    if(deptGD_Button.hoverState(cursor->xPos, cursor->yPos))
    {
        deptChangeEvent("GD");
    }
    if(deptAD_Button.hoverState(cursor->xPos, cursor->yPos))
    {
        deptChangeEvent("AD");
    }
    if(deptDM_Button.hoverState(cursor->xPos, cursor->yPos))
    {
        deptChangeEvent("DM");
    }
}

void MainMenu::myMouseReleased(ofMouseEventArgs &args)
{
    if(deptGD_Button.mouseReleased(args.x, args.y, 0) )
    {
        deptChangeEvent("GD");
    }
    if(deptAD_Button.mouseReleased(args.x, args.y, 0) )
    {
        deptChangeEvent("AD");
    }
    if(deptDM_Button.mouseReleased(args.x, args.y, 0) )
    {
        deptChangeEvent("DM");
    }
}

void MainMenu::deptChangeEvent(std::string d)
{
    // event
    static CustomEvent newEvent;
    newEvent.dept = d;
    ofNotifyEvent(CustomEvent::deptChange, newEvent);
    // change view
    viewmanager->setView(OVERVIEW);
}
