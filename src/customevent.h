#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include "ofMain.h"
#include "constants.h"

class CustomEvent : public ofEventArgs
{
    public:

        enum_views view;
        string dept;
        int objectId;

        CustomEvent(){

        }

        static ofEvent <CustomEvent> deptChange;
        static ofEvent <CustomEvent> viewChange;
        static ofEvent <CustomEvent> nearObject;
};

#endif // CUSTOMEVENT_H
