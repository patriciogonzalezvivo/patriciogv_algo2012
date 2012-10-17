#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle : public ofPoint {
public:
    
    Particle();
    
    void    init( ofPoint _pos, ofPoint _vel);
    
    void    addForce( ofPoint &_force );
    
    void    infinitWalls();
    
    void    update();
    void    draw();
	
    ofFloatColor color;
    
    ofPoint vel;
    ofPoint acc;
    
    float   trailLength;
    float   damping;
    float   size;
    
private:
    vector <ofPoint> trail;
};

#endif // PARTICLE_H
