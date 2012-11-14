#ifndef SPRING_H
#define SPRING_H

#include "ofMain.h"
#include "particle.h"

class spring {
public:

    spring();
		
    void update();
    void draw();
    
    particle * particleA;
    particle * particleB;
		
    float distance;
    float springiness;	 // this is the k, springiness constant
};

#endif