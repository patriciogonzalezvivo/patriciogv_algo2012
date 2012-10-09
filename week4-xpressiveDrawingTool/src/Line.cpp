//
//  Line.cpp
//  drawingWithParticles
//
//  Created by Patricio Gonzalez Vivo on 10/8/12.
//
//

#include "Line.h"

Line::Line(){
    color.set(0);
}

void Line::clear(){
    path.clear();
    drops.clear();
    particles.clear();
}

void Line::addParticle(ofPoint _pos, ofPoint _vel){
    float line_width = 30 - ofClamp(_vel.length()*0.3,0.0,30);
    path.addVertex( _pos.x, _pos.y, line_width);
    
    if (path.size() % 5 == 0){
        path = path.getResampledBySpacing(10);
        path = path.getSmoothed(1,0.5);
        
        //  Paint Drop
        //
        if ( (path.size() > 5) && (_vel.length() < 7 ) ){
            Particle newDrop;
            newDrop.init(_pos, ofPoint(0,ofRandom(10)));
            newDrop.size = line_width*0.05;
            newDrop.color = &color;
            newDrop.alphaTrail = false;
            drops.push_back(newDrop);
        }
    }
    
    //  Velocity particles
    //
    Particle myParticle;
    myParticle.init(_pos, _vel*0.3);
    myParticle.size = line_width * 0.1;
    myParticle.color = &color;
    myParticle.alphaTrail = true;
	particles.push_back(myParticle);
}

//--------------------------------------------------------------
void Line::update(){
    
    ofRectangle viewPort = ofRectangle(0,0,ofGetWidth(),ofGetHeight());
    
	for (int i = 0; i < particles.size(); i++){
        particles[i].update();
	}
    
    for (int i = 0; i < drops.size() ; i++){
        drops[i].update();
	}
}

//--------------------------------------------------------------
void Line::draw(){

	ofFill();
	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
    
    for (int i = 0; i < drops.size() ; i++){
        drops[i].draw();
	}
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    vector < ofPoint > pts = path.getVertices();
    for (int i = 0; i < pts.size(); i++){
        
        int i_m_1 = MAX(i-1,0);
        int i_p_1 = MIN(i+1, pts.size()-1);
        
        ofPoint pta = pts[i_m_1];
        ofPoint ptb = pts[i];
        ofPoint ptc = pts[i_p_1];
        
        ofPoint diff = ptc - pta;
        float angle = atan2(diff.y, diff.x);
        angle += PI/2;
        
        float width = pts[i].z;
        
        ofPoint offsetA;
        offsetA.x = ptb.x + width * cos(angle);
        offsetA.y = ptb.y + width * sin(angle);
        
        ofPoint offsetB;
        offsetB.x = ptb.x - width * cos(angle);
        offsetB.y = ptb.y - width * sin(angle);
        
        mesh.addVertex(offsetA);
        mesh.addVertex(offsetB);
    }
    
    // draw the mesh
    //
    ofSetColor(color);
    ofFill();
    mesh.draw();
    
}