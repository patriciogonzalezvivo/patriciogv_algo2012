#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofEnableSmoothing();
    
    //  Load a font to make for vectorice
    //
    font.loadFont("verdana.ttf", 60, true, true, true);
    
    //  This is the sentence that is going to be vectorice
    //
    string text = "Maxine";
    
    //  Here we extract the letters (ofTTFCharater) of the font string
    //
    vector<ofTTFCharacter> letters = font.getStringAsPoints(text);
    
    //  Then we grabb each letter and extract the ofPolylines of it
    //
    for(int i = 0; i < letters.size(); i++){
        
        vector<ofPolyline> letterOutLine = letters[i].getOutline();
        
        //  Each letter have several ofPolylines.
        //  So lets go for each one and extract the ofPoints and
        //  insert it in our vector<ofPoint> stringDot
        //
        for (int j = 0; j < letterOutLine.size(); j++ ){
            
            //  One of the ofPolyline wonders are the fact you can smooth,
            //  simplify and resample it.
            //
            letterOutLine[j].simplify();
            letterOutLine[j].getResampledByCount(2000);
            
            for(int k = 0; k < letterOutLine[j].getVertices().size(); k++){
                wParticle newDot;
                newDot.set(letterOutLine[j].getVertices()[k]);
                stringDots.push_back(newDot);
            }
        }
    }
    
    //  We remember the width of the bounding box of the text in order to center it later
    //
    stringWidth = font.getStringBoundingBox(text, 0, 0).width;
    
    ofSetFullscreen(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    //  We are going to alter between chaos and order here
    //  when pct is going to be the radio of the dots.
    //  So when is 0.0 it's going to be in the right place and when
    //  it's on 1.0 it's going to be on the top going arround as crazy
    //
    //  So, for that I'm using sin() width abs() in order to have values between 0.0 and 1.0
    //  in stead of -1.0 and 1.0. But the time of order was significant small
    //  using powf() shaping capabilities to make it stay more time near 0.0
    //
    pct = powf( abs(sin( ofGetElapsedTimef()*0.1)), 5.0);
    
    for (int i = 0; i < stringDots.size(); i++){
        
        float radius = pct * 200;
        float angle = ofGetElapsedTimef() * (1 + i / 20.0);
        
        float x = stringDots[i].origin.x + radius * cos(angle* 0.9);    //  We move the points to spin round the
        float y = stringDots[i].origin.y + radius * -sin(angle* 0.8);   //  default position
        
        stringDots[i].moveTo(x, y);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()*0.5-stringWidth*0.5, ofGetHeight()*0.5);
    
    //  In order to see the border line we just need to connect the dots
    //
    for (int i = 1; i < stringDots.size(); i++){
        
        ofSetLineWidth(2);
        
        //  The color of it is going to depend on the distance between the particles (alpha),
        //  the pct porcentage between order (0.0) and chaos (1.0) (saturation),
        //  the moment in time (hue)
        //
        float dist = stringDots[i-1].distance(stringDots[i]);
        ofFloatColor color = ofFloatColor(1.0,0.0,0.0);
        color.setHue(abs(sin(ofGetElapsedTimef()*0.5))*ofMap(i,0,stringDots.size(),0.0,1.0));
        color.setSaturation(pct);
        
        if ( dist < 80){
            ofSetColor(ofFloatColor(color,1.0-ofMap(dist, 0.0, 80.0, 0.0, 1.0)));
            ofLine(stringDots[i-1],stringDots[i]);
        }
        
        //  Draw the paths of the points.
        //
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