#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include "ofMain.h"

class CustomEvent : public ofEventArgs
{
    public:
        CustomEvent(){}

        string dept;
        static ofEvent <CustomEvent> deptChange;
};

#endif // CUSTOMEVENT_H
