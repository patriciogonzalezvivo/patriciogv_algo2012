//
//  Particle.cpp
//  week2_whitney
//
//  Created by Patricio Gonzalez Vivo on 9/20/12.
//
//

#include "wParticle.h"

wParticle::wParticle(){
    
}

void wParticle::moveTo(float _x, float _y){
    ofPoint temp;
    
    // Force it
    //
//	x = temp.x = _x;
//	y = temp.y = _y;
    
    
    float catchUpSpeed = 0.1;
    if ( points.size() > 0){
        // Xeno
        x = temp.x = catchUpSpeed * points[0].x + (1.0-catchUpSpeed) * _x;
        y = temp.y = catchUpSpeed * points[0].y + (1.0-catchUpSpeed) * _y;
    } else {
        // Move
        x = temp.x = _x;
        y = temp.y = _y;
    }
	points.push_back(temp);
	if (points.size() > 100){
		points.erase(points.begin());
	}
}

void wParticle::drawDot(float _alpha){
	
	ofFill();
    //ofSetColor(255,0,127);
    //ofSphere(x, y, z, 5);
    
    ofSetColor(255);
    ofCircle(x, y, 1);
    
    ofSetColor(ofFloatColor(1.0,_alpha*0.5));
    ofCircle(x, y, 5);
}

void wParticle::drawTrail(float _alpha){
	
    ofSetColor(255);
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    for (int i = 0 ; i < points.size() ; i++){
        float alpha = ofMap(i+1, 1,points.size(), 0.0,1.0);
        
        lineMesh.addColor(ofFloatColor(1.0, alpha*_alpha));
        lineMesh.addVertex(points[i]);
    }
    
    ofSetColor(255);
    lineMesh.draw();
}