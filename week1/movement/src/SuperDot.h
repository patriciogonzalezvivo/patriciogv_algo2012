//
//  Dot.h
//  movement
//
//  Created by Patricio Gonzalez Vivo on 9/13/12.
//
//

#ifndef DOT 
#define DOT

#include "ofMain.h"

//  How to increment
//
enum AnimAlgo {
    ALGO_LINEAR,
    ALGO_POWER,
    ALGO_XENO
};

//  What to do once it arrives to destination
//
enum AnimMode {
    MODE_STOP,
    MODE_PONG,
    MODE_REPLAY
};

class SuperDot : public ofPoint {
public:
    
    SuperDot();
    
    void    init(ofPoint * _start, ofPoint * _end);
    
    void    start();
    void    stop();
    
    bool    update();   // goes to the dst
    void    draw();
    
    bool    mouseClick(int _x, int _y);
    
    ofColor     color;
    AnimAlgo    algorithm;
    AnimMode    mode;
    
    float       shaper;             //  Only use on ALGO_POWER
    float       incrementFactor;    //  0.0 - 1.0
    float       percentage;         //  0.0 - 1.0
    
    bool    bMoving;    //  Is it moving?
    
private:
    void    wawArrive();
    
    ofPoint prev;   //  Previus position
    
    ofPoint *org;   //  Origin
    ofPoint *dst;   //  Destination
};

#endif
