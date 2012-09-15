#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(24);
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    
    videoIn.initGrabber(width , height);
    
    sMotion.allocate(width, height);
    
    bRecording = false;
}

//--------------------------------------------------------------
void testApp::update(){
    videoIn.update();
    
    if (videoIn.isFrameNew()){
        
        if (bRecording){
            sMotion.addFrame( videoIn.getPixels() );
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    videoIn.draw(0, 0);
    
    if (bRecording){
        ofSetColor(255, 0, 0, abs ( sin( ofGetElapsedTimeMillis() * 0.001 ) ) * 255 );
        ofCircle(25, 25, 7);
        ofDrawBitmapString("REC", 35,30);
    }
}

//--------------------------------------------------------------
void testApp::exit(){
    sMotion.save("movie");
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    bRecording = !bRecording;
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