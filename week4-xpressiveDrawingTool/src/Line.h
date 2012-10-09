//
//  Line.h
//  drawingWithParticles
//
//  Created by Patricio Gonzalez Vivo on 10/8/12.
//
//

#ifndef LINE
#define LINE

#include "ofMain.h"
#include "particle.h"

class Line {
public:
    Line();
    
    void    clear();
    void    addParticle( ofPoint _pos, ofPoint _vel );
    
    void    update();
    void    draw();
    
    ofFloatColor color;
    
private:
    ofPolyline          path;
    vector <Particle>   drops;
    vector <Particle>   particles;
    
    float   life, initLife;
};

#endif
