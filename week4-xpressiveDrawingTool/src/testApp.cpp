#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
	ofSetVerticalSync(true);
    ofEnableSmoothing();
	ofSetFrameRate(60);
    ofSetCircleResolution(100);
    
    ofSetFullscreen(true);
}

//--------------------------------------------------------------
void testApp::update(){

    actualLine.update();
    
	for (int i = 0; i < lines.size(); i++){
        lines[i].update();
	}

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(255);
    
    actualLine.draw();
    
	for (int i = 0; i < lines.size(); i++){
		lines[i].draw();
	}
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    
    if (key == ' '){
        actualLine.clear();
        lines.clear();
    } else {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	prevMouse = ofPoint(x,y);
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
    ofPoint mouse = ofPoint(x,y);
    ofPoint mouseVel = mouse - prevMouse;
    
    actualLine.addParticle( mouse , mouseVel );
    
    prevMouse = mouse;
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    lines.push_back(actualLine);
	actualLine.clear();
    
    for (int i = 0; i < lines.size(); i++){
        lines[i].color -= 0.5;
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(){
}
