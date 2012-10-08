//
//  Particle.cpp
//  week4-niceLookingParticles
//
//  Created by Patricio Gonzalez Vivo on 10/7/12.
//
//

#include "Particle.h"

//------------------------------------------------------------
Particle::Particle(){
	set(0,0,0);
    vel.set(ofRandom(-10,10), ofRandom(-10,10));
    acc.set(0,0,0);
    
    color.set(1.0, 0.0, 0.0);
//    color.setHue(ofRandom(0.3,0.8));
    color.setHue(ofRandom(0.8,1.0));
    
	damping = ofRandom(0.01f,0.1f);
}

//------------------------------------------------------------
void Particle::init(float _life){
    initLife = life = _life;
}

//------------------------------------------------------------
void Particle::addForce(ofPoint &_force){
    acc += _force;
}

//------------------------------------------------------------
void Particle::addNoise(float _angle, float _turbulence){
    
    float angle = ofSignedNoise( x * 0.005f, y * 0.005f) * _angle;
    
    ofPoint noiseVector( cos( angle ), sin( angle ) );
    
    acc += noiseVector * _turbulence * (1.0-ofMap(life,0.0,initLife,0.0,1.0));
}

//------------------------------------------------------------
void Particle::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit... for now, we don't
	bool bDampedOnCollision = false;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (x > maxx){
		x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (x < minx){
		x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (y > maxy){
		y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (y < miny){
		y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.9;
	}
	
}

void Particle::infinitWalls(){
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (x > maxx){
		x = minx;
	} else if (x < minx){
		x = maxx;
	}
	
	if (y > maxy){
		y = miny;
	} else if (y < miny){
		y = maxy;
	}
}

//------------------------------------------------------------
void Particle::update(){
	
    vel += acc;
    vel *= 1.0f - damping;
    *this += vel;
    
    acc *= 0;
    
    life -= 0.01;
}



//------------------------------------------------------------
void Particle::draw(ofImage &_image){
    
    ofPushStyle();
    
    ofSetColor(color, 255*sin(ofMap(life,0.0,initLife,0.0,PI)));
    ofSetRectMode(OF_RECTMODE_CENTER);
               
    float size = 580 * sin(ofMap(life,0.0,initLife,0.0,PI));
    _image.draw( *this, size, size );
    
    ofPopStyle();
}
