#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include "ofMain.h"
#include "constants.h"

class CustomEvent : public ofEventArgs
{
    public:

        viewsEnum view;
        string dept;
        int objectId;
        zoomLevelEnum zoomLevel;
        swipeGesturesEnum swipeDirection;

        CustomEvent(){

        }

        static ofEvent <CustomEvent> deptChange;
        static ofEvent <CustomEvent> viewChange;
        static ofEvent <CustomEvent> nearObject;
        static ofEvent <CustomEvent> zoomChange;
        static ofEvent <CustomEvent> swipeGesture;
};

#endif // CUSTOMEVENT_H
