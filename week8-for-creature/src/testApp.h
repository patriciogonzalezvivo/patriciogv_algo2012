#pragma once

#include "ofMain.h"

#include "ofxFX.h"

#include "Body.h"
#include "VectorField.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    //  Vector Field
    //
    VectorField VF;
    float       prevMouseX, prevMouseY;

    //  Images & Fonts
    //
    ofImage         background;
    ofImage         frame;
    ofTrueTypeFont  font;
    
    //  Caracters
    //
    Body        clouds;
    Body        ouroboros;
    
    //  Loop Counter
    //
    ofColor     inkColor;
    float       initialAngle;
    bool        blockCounter;
    int         counter;
    
    //  Global Variables
    //
    ofPoint     loopPos;
    float       scale;
    int         width, height;
    bool        bDebug, bPlay, bNoise, bLoop, bScreenShot;
};
