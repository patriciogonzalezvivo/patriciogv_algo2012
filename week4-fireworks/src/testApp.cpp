#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
	ofBackground(255, 255, 255);
	
}

//--------------------------------------------------------------
void testApp::update(){
	
    for (int i = fireworks.size()-1; i >=0 ; i--) {
        
        if ( fireworks[i].finish() ){
            fireworks.erase(fireworks.begin()+i);
        } else {
            fireworks[i].update();
        }
    }
    
    if (fireworks.size() == 0){
        Firework newFirework;
        newFirework.init(ofGetWidth()*0.5+ofRandom(-300,300), ofRandom(ofGetHeight()*0.5));
        fireworks.push_back(newFirework);
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
	
    for (int i = 0; i< fireworks.size(); i++) {
        fireworks[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	Firework newFirework;
    
    newFirework.init(x, y);
    
    fireworks.push_back(newFirework);
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
