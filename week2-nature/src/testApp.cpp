#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::green, ofColor::gray);

    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5, ofGetHeight()*0.5);
    
    ofPoint pos = ofPoint(0,0);
    
    for(int i = 0; i < ofGetFrameNum(); i++ ){
        float grow = i * ofMap(ofGetElapsedTimef(), 0.0, 100.0, 0.0, 1.0,true);
        pos.x += grow * cos(GOLDEN_RATIO*TWO_PI*i);
        pos.y += grow * sin(GOLDEN_RATIO*TWO_PI*i);
        
        ofSetColor(ofFloatColor(1.0,1.0-0.5*((float)i/ofGetFrameNum()),0.1));
        ofCircle(pos, grow*0.01);
    }
    
    ofPopMatrix();
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