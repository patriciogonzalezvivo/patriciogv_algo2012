#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    thing.init("u02.png", ofPoint(800,300));
    thing.startEditMode();
    
    bPlay = false;
}

//--------------------------------------------------------------
void testApp::update(){

    if (bPlay){
        thing.update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    
    thing.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'e'){
        thing.togleEditMode();
    } else if (key == 'p'){
        bPlay = !bPlay;
    } else if (key == 'r'){
        thing.restart();
    } else if (key == 'c'){
        thing.clear();
    }
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