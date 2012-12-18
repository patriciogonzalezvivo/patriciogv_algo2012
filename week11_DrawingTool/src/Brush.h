//
//  Brush.h
//
//  Created by Patricio Gonzalez Vivo on 12/17/12.
//
//

#ifndef BRUSH
#define BRUSH

#include "ofMain.h"

#include "Particle.h"
#include "Spring.h"

class Brush : public ofPoint {
public:
    Brush();

    void    begin();
    void    end();
    
    void    setNum(int _num);
    void    setWidth(float _width);
    
    void    set(int _x, int _y);
    void    set(ofPoint _pos);
    
    ofPoint getVel();
    float   getAngle();

    void    update();
    void    draw();

    float   softness;
    float   height;
    float   damp;
    
    bool    bDown;
    bool    bDebug;
    
private:
    vector<Particle*>   As;
    vector<Particle*>   Bs;
    vector<Spring>      springs;
    vector<ofPolyline>  tails;
    
    ofPolyline          tail;
    
    ofPoint             prevPos;
    
    float               width;
};

#endif
