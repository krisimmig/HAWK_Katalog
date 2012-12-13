#include "overview.h"

Overview::Overview()
{
    //ctor

}

void Overview::setupButtons()
{
    singleviewButton.setup("Single");
    mainmenuButton.setup("Main");
    ofAddListener(CustomEvent::deptChange, this, &Overview::deptChanged);
}

void Overview::draw()
{
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("Overview", ofGetWidth() / 2, 50);
    ofDrawBitmapString("Fachbereich: " + fachbereich, ofGetWidth() / 2, 90);

    mainmenuButton.draw(ofGetWidth() - 100 ,100);
    singleviewButton.draw(ofGetWidth() - 100 ,250);

    if(mainmenuButton.hoverState(cursor->xPos, cursor->yPos))
    {
        viewmanager->setView(MAINMENU);
    }
    if(singleviewButton.hoverState(cursor->xPos, cursor->yPos))
    {
        viewmanager->setView(SINGLE);
    }
}

void Overview::myMouseReleased(ofMouseEventArgs &args)
{
    if(mainmenuButton.mouseReleased(args.x, args.y, 0) )
    {
        viewmanager->setView(MAINMENU);
    }
    if(singleviewButton.mouseReleased(args.x, args.y, 0) )
    {
        viewmanager->setView(SINGLE);
    }
}

void Overview::deptChanged(CustomEvent &args)
{
    fachbereich = args.dept;
}
