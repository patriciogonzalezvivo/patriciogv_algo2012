#ifndef PARTICLE
#define PARTICLE

#include "ofMain.h"

class Particle : public ofPoint {
public:
    Particle();
    
    void addForce( ofPoint &_force );
    void update();
    void draw();
	
    ofFloatColor    color;

    ofPoint pos;
    ofPoint vel;
    ofPoint acc;
    
    float   trailLength;
    float   damping;
    float   size;
    float   timer;
    
private:
    vector <ofPoint> trail;
    
};

#endif