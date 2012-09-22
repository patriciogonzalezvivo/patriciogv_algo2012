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
    
    void moveTo(float _x, float _y);
    
    void drawDot(float _alpha);
    void drawTrail(float _alpha);
    
private:
    vector <ofPoint> points;
};


#endif
