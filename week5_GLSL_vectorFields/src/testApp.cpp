#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    width = 640;
    height = 480;
    scale = 5;
    video.initGrabber(width, height);
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    blur.setRadius(5);
    normals.allocate(width, height);
    
    VF.setupField(width/scale,height/scale,ofGetWidth(), ofGetHeight());
    
    for (int i = 0; i < 1000; i++){
		particle myParticle;
		myParticle.setInitialCondition(ofRandom(0,ofGetWidth()),ofRandom(0,ofGetHeight()),0,0);
		particles.push_back(myParticle);
	}
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if ( video.isFrameNew() ){
        grayscale.setTexture( video.getTextureReference() );
        grayscale.update();
        
        blur << grayscale;
        blur.update();
        normals << blur;
        normals.update();
        
        ofPixels normPixels;
        normals.getTextureReference().readToPixels(normPixels);
        ofPixels brightPixels;
        grayscale.getTextureReference().readToPixels(brightPixels);
        
        int scaledWidth = width/scale;
        int scaledHeight = height/scale;
        for(int x = 0; x <= scaledWidth; x++){
            for(int y = 0; y <= scaledHeight; y++){
                ofFloatColor normalColor = normPixels.getColor(x*scale, y*scale);
                ofFloatColor brightColor = brightPixels.getColor(x*scale, y*scale);
                
                int index = x + y * scaledWidth;
                ofVec2f norm = ofVec2f((normalColor.r - 0.5) * 2.0,
                                       (normalColor.g - 0.5) * 2.0);
                
                float pct = 0.2;//brightColor.r;
                VF[index] = VF[index]*(1.0-pct) + norm * pct;
            }
        }
    }
    
    VF.fadeField(0.99f);
    
    for (int i = 0; i < particles.size(); i++){
		particles[i].resetForce();
		
		// get the force from the vector field:
		ofVec2f frc;
		frc = VF.getForceFromPos(particles[i].pos.x, particles[i].pos.y);
		particles[i].addForce(frc.x, frc.y);
		particles[i].addDampingForce();
		particles[i].update();
	}
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(0,130,130, 200);
    VF.draw();
    
    for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    ofToggleFullscreen();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}