#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    thing.init("bicefalo.png", ofPoint(800,ofGetHeight()*0.5));
//    thing.load("thing.xml", ofPoint(800,270) );
    thing.startEditMode();
    bEdit = true;
    
    VF.setupField( ofGetWidth()/20, ofGetHeight()/20, ofGetWidth(), ofGetHeight());
    bPlay = false;
    bNoise = false;
}

//--------------------------------------------------------------
void testApp::update(){

    if (bPlay){
        VF.fadeField(0.99);
        if (bNoise)
            VF.noiseField();
        
        thing.update( &VF );
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    VF.draw();
    
    thing.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(key == 'e'){
        bEdit = !bEdit;
        thing.togleEditMode();
        bPlay = !bEdit;
    } else if (key == 'p'){
        bPlay = !bPlay;
    } else if (key == 'r'){
        thing.restart();
    } else if (key == 'c'){
        thing.clear();
    } else if (key == 's'){
        string filename = "t"+ofToString(ofGetDay())+"-"+ofToString(ofGetHours())+"-"+ofToString(ofGetMinutes())+"-"+ofToString(ofGetSeconds());
        thing.save(filename+".xml");
    } else if (key == 'n'){
        bNoise = !bNoise;
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
    if (!bEdit){
        float diffx = x - prevMouseX;
        float diffy = y - prevMouseY;
        
        VF.addVectorCircle((float)x, (float)y, diffx*0.3, diffy*0.3, 60, 0.3f);
        
        prevMouseX = x;
        prevMouseY = y;
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    prevMouseX = x;
	prevMouseY = y;
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