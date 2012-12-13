#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include "ofMain.h"
#include "constants.h"

class ViewManager
{
public:
    ViewManager();
    void setView(enum_views v);
    enum_views getCurrentView();

protected:
private:
    enum_views currentView;
};

#endif // VIEWMANAGER_H
