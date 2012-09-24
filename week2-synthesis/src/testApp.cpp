#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    for(int i = 0; i < BUFFER_SIZE ; i++  ){
        bufferA[i] = 0.0;
        bufferB[i] = 0.0;
    }
}

//--------------------------------------------------------------
void testApp::update(){
    
    //  Trying to apply this ( http://www.patriciogonzalezvivo.com/blog/?p=657 )
    //  y use on a shader but in one dimension
    //
    for(int i = 1; i < BUFFER_SIZE-1 ; i++  ){
        bufferB[i] = bufferA[i+1]*0.5+bufferA[i-1]*0.5;
        bufferB[i] = bufferB[i] - bufferA[i];
        bufferB[i] *= 0.52;
    }
    
    for(int i = 0; i < BUFFER_SIZE ; i++  ){
        bufferA[i] = bufferB[i];
        
        if (bufferA[i] > PI){
            bufferA[i] -= PI;
        }
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofNoFill();
    ofSetColor(255);
    
    ofBeginShape();
    ofVertex(0,ofGetHeight()*0.5);
    float dist = ofGetWidth()/BUFFER_SIZE;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        //ofCurveVertex(i*dist, ofGetHeight()*0.5 + bufferA[i] *10);//ofGetHeight()*0.5 + sin( bufferA[i] )*30 );
        ofCurveVertex(i*dist, ofGetHeight()*0.5 + sin( bufferA[i] )*30 );
    }
    ofVertex(ofGetWidth(),ofGetHeight()*0.5);
    ofEndShape();

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
    int index = ofMap(x, 0, ofGetWidth(), 0,BUFFER_SIZE, true);
    bufferA[ index ] = PI*1.5;
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