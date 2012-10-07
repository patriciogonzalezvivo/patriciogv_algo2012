#include "particle.h"
#include "ofMain.h"


//------------------------------------------------------------
Particle::Particle(){
	set(0,0,0);
    vel.set(ofRandom(-10,10), ofRandom(-10,10));
    acc.set(0,0,0);
    
    trailLength = 10;
    
	damping = 0.01f;
    size = 2;
}

//------------------------------------------------------------
void Particle::addForce(ofPoint &_force){
    acc += _force;
}

//------------------------------------------------------------
void Particle::update(){
	
    vel += acc;
    vel *= 1.0f - damping;
    *this += vel;
    
    acc *= 0;
    
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
    
    //  For making super smooth and gradient lines use ofMeshes.
    //  Them let you add vertex asociated to colors.
    //  OpenGL it's going to be in charge of making the gradient ; )
    //
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);  //  There are other modes like: points, triangles and quads
    
    for (int i = 0 ; i < trail.size() ; i++){
        
        //  Map the position on the array with the alpha to geting alfa gradient
        //
        float alpha = ofMap(i+1, 1,trail.size(), 0.0,1.0);
        
        lineMesh.addColor( ofFloatColor( color, alpha*0.3) );   //  We are using ofFloatColors that goes from 0.0 - 1.0
        
        //  instead of 0-255 that's way you have better quality
        lineMesh.addVertex(trail[i]);
    }
    
    //  Once this object call ofMesh it's done you can draw it.
    //
    ofSetColor(255);
    lineMesh.draw();
    
    //  Draw particle
    //
    ofSetColor(color);
    ofCircle(*this, size);
}
