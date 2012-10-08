//
//  Particle.h
//  week4-niceLookingParticles
//
//  Created by Patricio Gonzalez Vivo on 10/7/12.
//
//

#ifndef PARTICLE
#define PARTICLE

#include "ofMain.h"

class Particle : public ofPoint {
public:
    Particle();
    
    void    init(float _life);
    void    addForce( ofPoint &_force );
    void    addNoise(float _angle, float _turbulence);
    
    void    bounceOffWalls();
    void    infinitWalls();
    
    void    update();
    void    draw(ofImage &_image);
    
    float   life;
    
private:
    ofFloatColor    color;
    
    ofPoint         vel;
    ofPoint         acc;
    
    float           damping;
    float           initLife;
};

#endif
