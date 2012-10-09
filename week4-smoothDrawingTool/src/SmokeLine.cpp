//
//  SmokeLine.cpp
//  drawingWithParticles
//
//  Created by Patricio Gonzalez Vivo on 10/8/12.
//
//

#include "SmokeLine.h"

SmokeLine::SmokeLine(){
    
}

int SmokeLine::size(){
    return particles.size();
}

void SmokeLine::clear(){
    particles.clear();
}

void SmokeLine::addParticle(ofPoint _pos, ofPoint _vel){
    Particle myParticle;
    myParticle.init(_pos, _vel*0.1, ofRandom(50,150));
    myParticle.size = ofClamp(_vel.length(), 0.0, 50);
	particles.push_back(myParticle);
}

//--------------------------------------------------------------
void SmokeLine::update(){
    
    ofRectangle viewPort = ofRectangle(0,0,ofGetWidth(),ofGetHeight());
    
	for (int i = particles.size()-1; i >= 0 ; i--){
        
        if ( viewPort.inside(particles[i]) && particles[i].life > -1.0){
            particles[i].addNoise( 2.13, 0.05 );
            particles[i].update();
        } else {
            particles.erase(particles.begin()+i);
        }
	}
}

//--------------------------------------------------------------
void SmokeLine::draw(){

	ofFill();
	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
	
	//ofFill()
	// picasso !!!
	
//	ofNoFill();
//	ofBeginShape();
//	for (int i = 0; i < particles.size(); i++){
//		ofVertex(particles[i].x, particles[i].y);
//	}
//	ofEndShape();
	
}