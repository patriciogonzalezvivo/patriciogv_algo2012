//
//  particle.cpp
//  drawingWithParticles
//
//  Created by Patricio Gonzalez Vivo on 10/8/12.
//
//

#include "particle.h"

//------------------------------------------------------------
Particle::Particle(){
	set(0,0,0);
    vel.set(ofRandom(-10,10), ofRandom(-10,10));
    acc.set(0,0,0);
    
    trailLength = ofRandom(50,100);
	damping = 0.05f;
    size = 50;
}

void Particle::init( ofPoint _pos, ofPoint _vel, float _life ){
    this->set(_pos);
    vel.set(_vel);
    
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
    
    acc += noiseVector * _turbulence;
}

//------------------------------------------------------------
void Particle::update(){
	
    vel += acc;
    vel *= 1.0f - damping;
    *this += vel;
    
    acc *= 0;
    
    life -= 0.5;
    
    //  It doesn't just move, it also record the position in order
    //  to make a trail
    //
	trail.push_back(*this);
    
    //  Erase the moust distant
    //
	while (trail.size() > trailLength ){
		trail.erase(trail.begin());
	}
}

//------------------------------------------------------------
void Particle::draw(){
    float lifeFactor = ofMap(life,0.0,initLife,0.0,1.0);
    //  For making super smooth and gradient lines use ofMeshes.
    //  Them let you add vertex asociated to colors.
    //  OpenGL it's going to be in charge of making the gradient ; )
    //
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);  //  There are other modes like: points, triangles and quads
    
    for (int i = 0 ; i < trail.size() ; i++){
        
        //  Map the position on the array with the alpha to geting alfa gradient
        //
        float alpha = ofMap(i+1, 1,trail.size(), 0.0, lifeFactor);
        
        lineMesh.addColor( ofFloatColor( color, alpha) );   //  We are using ofFloatColors that goes from 0.0 - 1.0
        
        //  instead of 0-255 that's way you have better quality
        lineMesh.addVertex(trail[i]);
    }
    
    //  Once this object call ofMesh it's done you can draw it.
    //
    ofSetColor(255);
    lineMesh.draw();
    
    ofSetColor(color, 255* lifeFactor);
    ofCircle(*this, size *lifeFactor);
}
