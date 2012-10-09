//
//  SmokeLine.h
//  drawingWithParticles
//
//  Created by Patricio Gonzalez Vivo on 10/8/12.
//
//

#ifndef SMOKELINE
#define SMOKELINE

#include "ofMain.h"
#include "particle.h"

class SmokeLine {
public:
    SmokeLine();
    
    int     size();
    void    clear();
    void    addParticle( ofPoint _pos, ofPoint _vel );
    
    void    update();
    void    draw();
    
private:
    vector <Particle> particles;
    
    float   life, initLife;
};

#endif
