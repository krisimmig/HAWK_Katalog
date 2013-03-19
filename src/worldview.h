#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include "ofMain.h"
#include "constants.h"
#include "cursor.h"
#include "object3d.h"
#include "customevent.h"
#include "students.h"



class WorldView
{
public:
    WorldView();
    virtual ~WorldView();
    void setCursor(HandCursor *cursor);


protected:
private:
    void draw(ofEventArgs &e);
    void drawInfo();
    void drawSucher();
    void drawBottomInterface();
    void drawHandIndicator();

    void update(ofEventArgs &e);
    void updateDepartment();
    void updateStudenObjectsPosition();
    void updateZoomLevel();
    void updateInfoPanelPosition();
    void updateScreenPosition();
    void shakeInfoPanel();

    void keyReleased(ofKeyEventArgs &e);
    void keyPressed(ofKeyEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void changeZoomLevel(zoomLevelEnum zoomLevel);
    void swipeGestureEvent(swipeGesturesEnum swipeDirection);
    void listenerZoomChange(CustomEvent &e);
    void listenerSwipeGesture(CustomEvent &e);
    void moveScreen();
    void kinectMove();
    string wrapString(string text, int width);
    HandCursor* cursor;
    bool firstStart;

    // environment + camera
    ofCamera camera;
    float cameraHeight;
    float currentCameraHeight;
    float dragSpeed;
    float currentXDragSpeed;
    float currentYDragSpeed;
    int zoomLevel;
    bool zooming;
    int random10;
    int speedXCounter;
    int speedYCounter;
    float speedFactor;

    float pmouseX;
    float pmouseY;

    float cameraX;
    float cameraY;

    // gestures
    int gestureTimerZoom;
    int gestureTimerSwipe;
    int gestureTimeout;

    // studentObjects
    Object3D** studentObjects;
    int studentObjectAbstandX;
    int studentObjectAbstandY;
    int currentSphere;
    ofVec3f closestObjectVector;
    float studentObjectsXPos;
    float studentObjectsYPos;
    float studentObjectsXPosFuture;
    float studentObjectsYPosFuture;
    float studentObjectsXPosDefault;
    float studentObjectsYPosDefault;

    // student stuff
    int numberOfStudents;
    int currentStudent;
    std::string fullName;
    std::string description;
    int* studentIdArray;

    // fachbereich stuff
    int currentDeptNumber;
    int totalDeptNumber;
    fachbereichEnum fachbereichArray[10];
    bool departmentChangedToLeft;
    bool departmentChangedToRight;
    std::string currentDepartmentString;

    // info panel
    bool justArrived;
    int shakingTimer;
    int infoPanelWidth;
    int projectImagesYPosition;
    int currentProjectImagesYPosition;
    float infoPanelFinalYPosition;
    float infoPanelFinalXPosition;
    float infoPanelYPosition;
    float futureProjectImagesYPosition;
    float infoPanelXPosition;
    float infoPanelResetPosition;
    bool infoPanelToLeft;
    bool infoPanelToRight;
    int currentImageNumber;
    int currentImageHeight;
    float totalImageColumnHeight;
    bool leavingInfoPanelView;
    bool studentSwitched;

    // cursor & menu
    int xPosLeftHandMenu;
    int yPosLeftHandMenu;
    bool menuActivated;
    bool menuActive;
    bool justZoomed;
    bool cursorPointerTop;
    bool cursorPointer;
    ofVec2f menuMiddle;

    // fonts
    ofTrueTypeFont fontXL;
    ofTrueTypeFont fontL;
    ofTrueTypeFont fontLBold;
    ofTrueTypeFont fontLLight;
    ofTrueTypeFont fontS;
    ofTrueTypeFont fontSLight;
    ofTrueTypeFont fontSLLight;

    // audio
    ofSoundPlayer menuOpen;
    ofSoundPlayer menuClick;
    ofSoundPlayer error;

    // grafix
    ofImage bottomInterface;
    ofImage cursor_handClosed;
    ofImage cursor_handOpened;
    ofImage cursor_handPointer;
    ofImage auswahlMitte;
    ofImage auswahlMitte_hover;

    ofImage auswahlOben;
    ofImage auswahlOben_hover;
    ofImage ubersichtOben;
    ofImage ubersichtOben_hover;

    ofImage fachbereicheUnten;
    ofImage fachbereicheUnten_hover;

    ofImage fachbereichTitel;
    ofImage arrowLeft;
    ofImage arrowRight;


};

#endif // WORLDVIEW_H

