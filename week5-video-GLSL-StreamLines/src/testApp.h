#pragma once

#include "ofMain.h"

#include "vectorField.h"

//  ofxFX addon need from https://github.com/patriciogonzalezvivo/ofxFX
//
#include "ofxFX.h"

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
		
    ofVideoGrabber  video;
    ofxGrayscale    grayscale;
    ofxGaussianBlur blur;
    ofxNormals      normals;
    
    ofFbo           pingpong[2];
    ofShader        shader;
    
    vectorField     VF;
    
    int             timer, width, height,scale;
    
    bool            bDrawVideo;
    bool            bDrawNormals;
    bool            bDrawField;
    bool            bCapture;
};
