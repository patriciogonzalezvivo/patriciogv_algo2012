//
//  StopMotion.h
//  week1-movingRec
//
//  Created by Patricio Gonzalez Vivo on 9/15/12.
//
//

#ifndef STOPMOTION
#define STOPMOTION

#include "ofMain.h"

typedef struct{
    unsigned char * pixels;
    unsigned long timeStamp;
} Frame;

class StopMotion {
public:

    StopMotion();
    
    void    allocate( int _width, int _height );
    void    load(string _path);
    void    save(string _path);
    
    void    addFrame( unsigned char * _pixels );
    
    void    nextFrame();
    void    goFrame();
    
    void    goPct(float _pct);
    
    void    draw(int _x, int _y, int _width = -1, int _height = -1);
    
private:
    vector< Frame > buffer;
    
    unsigned long startTime;
    int     width, height;
    
};

#endif
