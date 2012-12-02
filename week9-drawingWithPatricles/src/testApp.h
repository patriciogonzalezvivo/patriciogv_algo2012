#pragma once

#include "ofMain.h"

#include "ofxFX.h"

#include "vectorField.h"
#include "particle.h"

class testApp : public ofBaseApp{
public:
    void    setup();
    void    update();
    void    draw();

    void    keyPressed  (int key);
    void    keyReleased(int key);
    void    mouseMoved(int x, int y );
    void    mouseDragged(int x, int y, int button);
    void    mousePressed(int x, int y, int button);
    void    mouseReleased(int x, int y, int button);
    void    windowResized(int w, int h);
    void    dragEvent(ofDragInfo dragInfo);
    void    gotMessage(ofMessage msg);
		
    void    init(int _width, int _height, int _screenWidth, int _screenHeight);
    void    preprocess(ofTexture &_text);
    
    //  INPUT
    //
    ofVideoGrabber  video;
    ofImage         image;
    
    //  PREPROCESS
    //
    ofxGrayscale    grayscale;
    ofxGaussianBlur blur;
    ofxNormals      normals;
    
    //  PROCESS
    //
    ofPixels        pixels;
    vectorField     VF;
    vector<Particle*> particles;
    
    //  RENDER
    //
    ofFbo           fbo;
    
    float   noise;
    
    int     width, height,scale;
    
    bool    bImage;
    bool    bTrails;
    bool    bDrawField;
    bool    bPrintScreen;
};
