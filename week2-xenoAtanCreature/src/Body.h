//
//  Body.h
//  week2-xenoAtanCreature
//
//  Created by Patricio Gonzalez Vivo on 10/1/12.
//
//

#ifndef BODY
#define BODY

#include "ofMain.h"

class Body : public ofPoint {
public:
    
    Body();
    
    void    setDst(Body *_dst);
    
    void    update();
    void    draw();

    ofColor color;
    float   angle, size, delay;
    float   catchUpSpeed;
    
private:
    
    ofPoint prevPos;
    Body    *dst;
};

#endif
