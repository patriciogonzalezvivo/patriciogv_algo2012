#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofEnableSmoothing();
    
    font.loadFont("verdana.ttf", 60, true, true, true);
    
    
    vector<ofTTFCharacter> letters = font.getStringAsPoints("John Whitney");
    for(int i = 0; i < letters.size(); i++){
        vector<ofPolyline> letterOutLine = letters[i].getOutline();
        
        for (int j = 0; j < letterOutLine.size(); j++ ){
            letterOutLine[j].simplify();
            letterOutLine[j].getResampledByCount(2000);
            for(int k = 0; k < letterOutLine[j].getVertices().size(); k++){
                wParticle newDot;
                newDot.set(letterOutLine[j].getVertices()[k]);
                stringDots.push_back(newDot);
            }
        }
    }
    
    stringWidth = font.getStringBoundingBox("John Whitney", 0, 0).width;
    stringHeight = font.getStringBoundingBox("John Whitney", 0, 0).height;
    
    ofSetFullscreen(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    pct = powf( abs(sin( ofGetElapsedTimef()*0.1)), 5.0);
    
    for (int i = 0; i < stringDots.size(); i++){
        
        float radius = pct * 200;
        float angle = ofGetElapsedTimef() * (1 + i / 20.0);
        
        float x = stringDots[i].origin.x + radius * cos(angle* 0.9);
        float y = stringDots[i].origin.y + radius * -sin(angle* 0.8);
        
        stringDots[i].moveTo(x, y);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5-stringWidth*0.5, ofGetHeight()*0.5);
    
    for (int i = 1; i < stringDots.size(); i++){
        ofSetLineWidth(2);
        
        float dist = stringDots[i-1].distance(stringDots[i]);
        ofFloatColor color = ofFloatColor(1.0,0.0,0.0);
        color.setHue(abs(sin(ofGetElapsedTimef()*0.5))*ofMap(i,0,stringDots.size(),0.0,1.0));
        color.setSaturation(pct);
        
        if ( dist < 80){
            ofSetColor(ofFloatColor(color,1.0-ofMap(dist, 0.0, 80.0, 0.0, 1.0)));
            ofLine(stringDots[i-1],stringDots[i]);
        }
        
        ofSetColor(255);
        ofSetLineWidth(1);
        stringDots[i].drawTrail( color );
    }
    
    ofPopMatrix();
    
//    ofDrawBitmapString("Pct: " + ofToString(pct), 15,15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofToggleFullscreen();
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