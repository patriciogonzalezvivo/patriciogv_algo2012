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
    
    void    setColor( ofColor _color );
    
    void    set(int _x, int _y);
    void    set(ofPoint _pos);
    
    ofPoint getVel();
    float   getAngle();
    
    ofPoint getPositionForTime( float time );
    ofPoint getVelocityForTime( float time );
    
    float   getDuration();
    
    void    clear();
    void    update();
    
    void    draw();
    void    drawDebug();
    void    drawPathFollower();
    void    drawPathFollower(ofImage &_image);

    float   softness;
    float   height;
    float   damp;
    
    bool    bDown;
    int     passes;
    
private:
    vector<Particle*>   As;
    vector<Particle*>   Bs;
    vector<Spring>      springs;
    vector<ofPolyline>  tails;
    
    ofPolyline          path;
    
    ofPoint             prevPos;
    ofColor             color;
    
    float               playbackStartTime;
    float               startTime;
    float               width;
};

#endif
