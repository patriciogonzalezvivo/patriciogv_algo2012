//
//  particle.h
//  drawingWithParticles
//
//  Created by Patricio Gonzalez Vivo on 10/8/12.
//
//

#ifndef PARTICLE
#define PARTICLE

#include "ofMain.h"

class Particle : public ofPoint {
public:
    Particle();
    
    void    init( ofPoint _pos, ofPoint _vel);
    
    void    addForce( ofPoint &_force );
    void    addNoise(float _angle, float _turbulence);
    
    void    update();
    void    draw();
	
    ofFloatColor    *color;

    ofPoint vel;
    ofPoint acc;
    
    float   trailLength;
    float   damping;
    float   size;
    
    bool    alphaTrail;
    
private:
    vector <ofPoint> trail;
};

#endif