#pragma once

#include "ofMain.h"

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

    VectorField VF;

    ofImage     frame;
    
    ofColor     inkColor;
    ofTrueTypeFont  font;
    
    Body        clouds;
    Body        ouroboros;
    Body        bicefal;
    
    ofPoint     loopPos;
    float       initialAngle;
    bool        blockCounter;
    
    int         counter;
    
    float       prevMouseX, prevMouseY;
    int         width, height;
    bool        bDebug, bPlay, bNoise, bLoop, bScreenShot;
};
