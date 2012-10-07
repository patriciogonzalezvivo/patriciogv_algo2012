//
//  Firework.cpp
//  week4-fireworks
//
//  Created by Patricio Gonzalez Vivo on 10/7/12.
//
//

#include "Firework.h"

Firework::Firework(){
    color.set(255, 0, 0);
    color.setHue( ofRandom(255) );
    
    bArrive = false;
}

void Firework::init(int _x, int _y){
    particles.clear();
    
    dst.set(_x, _y);
    pos.set(_x + ofRandom(-100,100), ofGetHeight());
    
    bArrive = false;
    timer = 0.0f;
}

bool Firework::finish(){
    if ( bArrive && (particles.size() == 0) )
        return true;
    else
        return false;
}

void Firework::update(){
    if (!bArrive){
        
        if ( pos.distance(dst) < 5 ){
            
            //  Explotion!
            //
            int nParticles = ofRandom(50,100);
            for (int i = 0; i < nParticles; i++){
                Particle myParticle;
                myParticle.set( dst );
                myParticle.color.set(color);
                myParticle.color.setHue(  myParticle.color.getHue() + ofRandom(-0.05,0.05) );
                myParticle.color.setSaturation(  ofRandom(0.5,1.0) );
                myParticle.color.setBrightness(  ofRandom(0.5,1.0) );
                
                myParticle.trailLength = ofRandom(10,30);
                myParticle.damping = ofRandom(0.001, 0.05);
                
                particles.push_back(myParticle);
            }
            
            bArrive = true;
            
        } else {
            
            float pct = powf(timer, 0.6);
            
            pos.x = (1-pct) * pos.x + (pct) * dst.x;
            pos.y = (1-pct) * pos.y + (pct) * dst.y;
            
            timer += 0.001f;
        }
        
    } else {
        ofPoint gravity = ofPoint(0.0, 0.28) ;
        
        for (int i = particles.size()-1; i >= 0; i--){
            
            //  Erase particles that are not longer visible
            //  or update the ones that are
            //
            if (particles[i].y > ofGetHeight()){
                particles.erase(particles.begin()+i);
            } else {
                particles[i].addForce( gravity );
                particles[i].update();
                
                //  Reduce progressively the brighness, size and trail
                //
                particles[i].color.setBrightness( particles[i].color.getBrightness() * 0.995 );
                particles[i].size *= 0.995;
                particles[i].trailLength *= 0.995;
            }
        }
    }
}

void Firework::draw(){
    if (!bArrive){
        ofSetColor(255);
        ofCircle(pos, 2);
    } else {
        
        for (int i = 0; i < particles.size(); i++){
            particles[i].draw();
        }
        
    }
}



