//
//  Particle.h
//  week2_whitney
//
//  Created by Patricio Gonzalez Vivo on 9/20/12.
//
//

#ifndef WPARTICLE
#define WPARTICLE

#include "ofMain.h"

class wParticle : public ofPoint {
public:
    
    wParticle();
    
    void set(ofPoint _pos);
    void moveTo(float _x, float _y);
    
    void drawTrail(ofFloatColor _color);
    
    ofPoint origin;
    
private:
    vector <ofPoint> points;
};


#endif
