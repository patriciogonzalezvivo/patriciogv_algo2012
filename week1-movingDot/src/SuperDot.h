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
enum AnimShaper {
    SHAPE_LINEAR,
    SHAPE_POWER,
    SHAPE_SIN_IN,
    SHAPE_SIN_OUT,
    SHAPE_XENO
};

//  What to do once it arrives to destination
//
enum AnimLooper {
    LOOP_NONE,
    LOOP_PONG,
    LOOP_REPLAY
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
    AnimShaper  shape;
    AnimLooper  loop;
    
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
