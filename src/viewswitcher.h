#ifndef VIEWSWITCHER_H
#define VIEWSWITCHER_H

#include "constants.h"

class ViewManager
{
public:
    ViewManager();
    virtual ~ViewManager();
    void setView(enum_views view);
    enum_views getCurrentView();
protected:
private:
    enum_views currentView;
};

#endif // VIEWSWITCHER_H
