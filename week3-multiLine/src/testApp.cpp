#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
}

//--------------------------------------------------------------
void testApp::update(){
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    actualLine.draw();
    
    ofSetColor(180);
    for (int i = 0; i < storedLines.size(); i++){
        storedLines[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    storedLines.clear();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    actualLine.addPoint(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    actualLine.addPoint(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    actualLine.setFinish();
    storedLines.push_back(actualLine);
    actualLine.clear();
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