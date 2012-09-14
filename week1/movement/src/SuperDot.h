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

enum AnimAlgo {
    ALGO_LINEAR,
    ALGO_POWER,
    ALGO_XENO
};

class SuperDot : public ofPoint {
public:
    
    SuperDot();
    
    void    init(ofPoint * _start, ofPoint * _end);
    
    void    start();
    void    stop();
    
    bool    update();   // goes to the dst
    
    void    draw();
    
    ofColor color;
    
    AnimAlgo    algorithm;
    
    float       shaper;
    float       incrementFactor;
    float       percentage;
    
    bool    bMoving;
    
private:
    ofPoint prev;
    
    ofPoint *org;
    ofPoint *dst;
};

#endif
