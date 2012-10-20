#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"

class Particle : public ofPoint {
public:
    
    Particle();
    
    void    init( ofPoint _pos, ofPoint _vel);
    
    void    addForce( ofPoint &_force );
    
    void    infinitWalls();
    
    void    addRepulsionForce(ofPoint p, float radius, float scale);
    void    addAttractionForce(ofPoint p, float radius, float scale);
    void    addRepulsionForce(Particle &p, float radius, float scale);
    void    addAttractionForce(Particle &p, float radius, float scale);
    void    addClockwiseForce(Particle &p, float radius, float scale);
    void    addCounterClockwiseForce(Particle &p, float radius, float scale);
    
    void    update();
    void    draw(bool _drawTrail = true);
	
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
