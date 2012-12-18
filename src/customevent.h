#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include "ofMain.h"
#include "constants.h"

class CustomEvent : public ofEventArgs
{
    public:

        enum_views view;
        string dept;

        CustomEvent(){

        }

        static ofEvent <CustomEvent> deptChange;
        static ofEvent <CustomEvent> viewChange;
};

#endif // CUSTOMEVENT_H
