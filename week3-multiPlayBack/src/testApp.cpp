#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    bPingPongMode = true;
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
        ofSetColor(255,50);
        storedLines[i].draw();
        
        ofPushStyle();
        int total = 5;
        for(int j = 0; j < total; j++){
            float factor = ofMap(j,0.0,total,1.0,0.0);
            
            ofFloatColor color;
            color.set(1.0, 0.0, 0.0);
            color.setHue( ofMap(i,0.0,storedLines.size(),0.0,1.0) );
            color.a = factor;
            
            float time = storedLines[i].getTimeForPct( abs(sin(ofGetElapsedTimef()*0.8-j*0.1)));
            
            ofSetColor(color);
            drawPlayBack(storedLines[i], time, 50*factor);
        }
        ofPopStyle();
    }
}

void testApp::drawPlayBack(TimeLine &_tLine, float _onTime, float _size){
    if ( _tLine.isFinish() ){
        ofPushStyle();
        // figure out what time we are at, and make sure we playback cyclically (!)
        // use the duration here and make sure our timeToCheck is in the range of 0 - duration
        float timeToCheck = _onTime;
        
        while (timeToCheck > _tLine.getDuration() && _tLine.getDuration() > 0){
            timeToCheck -= _tLine.getDuration();
        }
        
        ofFill();
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        ofPushMatrix();
        ofTranslate( _tLine.getPositionForTime(timeToCheck));
        
        ofRotateZ( _tLine.getAngleForTime(timeToCheck) * RAD_TO_DEG);
        ofRect(0,0,_size + _tLine.getVelocityForTime( timeToCheck ).length() * 0.2, _size);
        
        ofPopMatrix();
        ofPopStyle();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        storedLines.clear();
    } else if (key == 'p'){
        bPingPongMode = !bPingPongMode;
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