//
//  Firework.h
//
//  Created by Patricio Gonzalez Vivo on 10/7/12.
//
//

#ifndef FIREWORK
#define FIREWORK

#include "ofMain.h"
#include "particle.h"

class Firework {
public:
    
    Firework();
    
    void    init(int _x, int _y);
    
    bool    finish();
    
    void    update();
    void    draw();
    
private:
    vector<Particle> particles;
    
    ofColor color;
    
    ofPoint pos;
    ofPoint dst;
    
    float   timer;
    
    bool    bArrive;
};

#endif
