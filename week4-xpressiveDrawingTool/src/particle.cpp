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
	damping = 0.07f;
    
    alphaTrail = true;
}

void Particle::init( ofPoint _pos, ofPoint _vel){
    this->set(_pos);
    vel.set(_vel);
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
    
    //  It doesn't just move, it also record the position in order
    //  to make a trail
    //
    if(vel.length() > 0.1){
        trail.push_back(*this);
    }

}

//------------------------------------------------------------
void Particle::draw(){
    
    /*
    //  For making super smooth and gradient lines use ofMeshes.
    //  Them let you add vertex asociated to colors.
    //  OpenGL it's going to be in charge of making the gradient ; )
    //
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);  //  There are other modes like: points, triangles and quads
    
    for (int i = 0 ; i < trail.size() ; i++){
        
        //  Map the position on the array with the alpha to geting alfa gradient
        //
        float alpha = ofMap(i+1, 1,trail.size(), 0.0, 0.5);
        
        lineMesh.addColor( ofFloatColor( *color, alpha) );   //  We are using ofFloatColors that goes from 0.0 - 1.0
        
        //  instead of 0-255 that's way you have better quality
        lineMesh.addVertex(trail[i]);
    }
    
    //  Once this object call ofMesh it's done you can draw it.
    //
    ofSetColor(255);
    lineMesh.draw();
    */
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for (int i = 0; i < trail.size(); i++){
        
        int i_m_1 = MAX(i-1,0);
        int i_p_1 = MIN(i+1, trail.size()-1);
        
        ofPoint pta = trail[i_m_1];
        ofPoint ptb = trail[i];
        ofPoint ptc = trail[i_p_1];
        
        ofPoint diff = ptc - pta;
        float angle = atan2(diff.y, diff.x);
        angle += PI/2;
        
        float width = size*0.8 + diff.length()*0.2;
        
        ofPoint offsetA;
        offsetA.x = ptb.x + width * cos(angle);
        offsetA.y = ptb.y + width * sin(angle);
        
        ofPoint offsetB;
        offsetB.x = ptb.x - width * cos(angle);
        offsetB.y = ptb.y - width * sin(angle);
        
        //  Map the position on the array with the alpha to geting alfa gradient
        //
        float alpha = 1.0;
        
        if (alphaTrail){
            alpha = ofMap(i+1, 1,trail.size(), 0.0, 0.9);
        }
        
        mesh.addColor(ofFloatColor( *color, alpha) );
        mesh.addVertex(offsetA);
        mesh.addColor(ofFloatColor( *color, alpha) );
        mesh.addVertex(offsetB);
    }
    
    // draw the mesh
    //
    ofSetColor( *color);
    ofFill();
    mesh.draw();
    
    ofSetColor( *color, 255);
    ofCircle(*this, size );
}
