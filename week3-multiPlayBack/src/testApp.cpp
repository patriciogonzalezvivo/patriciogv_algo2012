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
        
        ofSetColor(255,0,0,10);
        drawPlayBack(storedLines[i], ofGetElapsedTimef()*0.9, 10);
        ofSetColor(255,0,0,200);
        drawPlayBack(storedLines[i], ofGetElapsedTimef()*0.99, 30);
        ofSetColor(255,0,0);
        drawPlayBack(storedLines[i], ofGetElapsedTimef(), 50);
        
        ofPushStyle();
        int total = 5;
        for(int j = 0; j < total; j++){
            float factor = ofMap(j,0.0,total,0.0,1.0);
            
            ofFloatColor color;
            color.set(1.0, 0.0, 0.0);
            color.setHue(0.3);
            
            float time = ofGetElapsedTimef()*(1.0-factor);
            
            ofSetColor(color,factor);
            drawPlayBack(storedLines[i], time, 30*factor);
        }
        ofPopStyle();
    }
}

void testApp::drawPlayBack(TimeLine &_tLine, float _onTime, float _size){
    if ( _tLine.isFinish() ){
        ofPushStyle();
        // figure out what time we are at, and make sure we playback cyclically (!)
        // use the duration here and make sure our timeToCheck is in the range of 0 - duration
        float timeToCheck = _onTime - _tLine[_tLine.size()-1].time;
        
        while (timeToCheck > _tLine.getDuration() && _tLine.getDuration() > 0){
            timeToCheck -= _tLine.getDuration();
        }
        
        ofFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        ofPushMatrix();
        ofTranslate( _tLine.getPositionForTime(timeToCheck));
        
        ofRotateZ( _tLine.getAngleForTime(timeToCheck) * RAD_TO_DEG);
        ofRect(0,0,_size + _tLine.getVelocityForTime(timeToCheck).length() * 0.2, _size);
        
        ofPopMatrix();
        ofPopStyle();
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