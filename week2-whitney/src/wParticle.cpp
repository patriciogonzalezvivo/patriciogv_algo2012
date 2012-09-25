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
    
    //  It doesn't just move, it also record the position in order
    //  to make a trail
    //
    ofPoint temp;
    x = temp.x = _x;
    y = temp.y = _y;
    
	points.push_back(temp);
    
    //  Erase the moust distant
    //
	if (points.size() > 50){
		points.erase(points.begin());
	}
}

void wParticle::drawTrail(ofFloatColor _color){
	
    //  For making super smooth and gradient lines use ofMeshes.
    //  Them let you add vertex asociated to colors.
    //  OpenGL it's going to be in charge of making the gradient ; )
    //
    
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);  //  There are other modes like: points, triangles and quads
    
    for (int i = 0 ; i < points.size() ; i++){
        
        //  Map the position on the array with the alpha to geting alfa gradient
        //
        float alpha = ofMap(i+1, 1,points.size(), 0.0,1.0);
        
        lineMesh.addColor( ofFloatColor(_color, alpha*0.3) );   //  We are using ofFloatColors that goes from 0.0 - 1.0
                                                                //  instead of 0-255 that's way you have better quality
        lineMesh.addVertex(points[i]);
    }
    
    //  Once this object call ofMesh it's done you can draw it.
    //
    ofSetColor(255);
    lineMesh.draw();
}