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

void wParticle::set(ofPoint _pos){
    x = origin.x = _pos.x;
    y = origin.y = _pos.y;
}

void wParticle::moveTo(float _x, float _y){
    ofPoint temp;
    
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
	if (points.size() > 50){
		points.erase(points.begin());
	}
}

void wParticle::drawTrail(ofFloatColor _color){
	
    ofSetColor(255);
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    for (int i = 0 ; i < points.size() ; i++){
        float alpha = ofMap(i+1, 1,points.size(), 0.0,1.0);
        
        lineMesh.addColor(ofFloatColor(_color, alpha*0.3));
        lineMesh.addVertex(points[i]);
    }
    
    ofSetColor(255);
    lineMesh.draw();
}