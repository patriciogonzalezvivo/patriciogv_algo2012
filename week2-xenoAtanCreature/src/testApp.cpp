#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    worm[0].setDst( &mouse );

    for(int i = 1; i < BODY_PARTS; i++){
        worm[i].setDst( &worm[i-1] );
        worm[i].size -= i*2;
        worm[i].color.setHue(50-i);
        worm[i].catchUpSpeed += i*0.001;
        worm[i].delay = i*0.8;
    }
}

//--------------------------------------------------------------
void testApp::update(){

    mouse.x = mouseX + cos(ofGetElapsedTimef()*2) * 100;
    mouse.y = mouseY + sin(ofGetElapsedTimef()*2) * 100;
    
    for(int i = 0; i < BODY_PARTS; i++){
        worm[i].update();
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    for(int i = 0; i < BODY_PARTS; i++){
        worm[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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