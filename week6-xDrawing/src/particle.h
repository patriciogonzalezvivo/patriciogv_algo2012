#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle : public ofPoint {
public:
    
    Particle();
    
    void    init( ofPoint _pos, ofPoint _vel);
    
    void    addForce( ofPoint &_force );
    
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(ofPoint p, float radius, float scale);
    void    addRepulsionForce(Particle &p, float scale);
    void    addRepulsionForce(Particle &p, float radius, float scale);
    void    addAttractionForce(Particle &p, float radius, float scale);
    void    addClockwiseForce(Particle &p, float radius, float scale);
    void    addCounterClockwiseForce(Particle &p, float radius, float scale);
    
    void    infinitWalls();
    void    bounceOffWalls();
    
    void    update();
    void    draw();
	
    ofFloatColor color;
    
    ofPoint vel;
    ofPoint acc;
    
    float   *damping;
    float   size;
};

#endif // PARTICLE_H
