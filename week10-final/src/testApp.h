#pragma once

#include "ofMain.h"

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
		
    //  INPUT
    //
    ofImage         image;
    ofImage         brush;
    
    //  FILTERS
    //
    ofxGrayscale    grayscale;
    ofxInverse      inverse;
    ofxGaussianBlur blur;
    ofxNormals      normals;
    ofxMask         mask;
    
    //  SHADER
    //
    ofFbo           pingpong[2];
    ofShader        shader;
    
    int     timer, width, height, blurRadius;
    
    bool    bDebug;
    bool    bImage;
};
