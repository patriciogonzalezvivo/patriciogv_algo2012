#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
//    ofEnableSmoothing();
    
    width = 1200;
    height = 1048; //width * 1.4142; // A4 Ratio
    ofSetWindowShape(width, height);
    
    VF.setupField( width/20, height/20, width, height);
    
    background.setMode(OF_PRIMITIVE_LINES);
    for (float i = 0; i < height; i += 5.0 ){
        background.addVertex( ofPoint( 0, i ));
        background.addVertex( ofPoint( width, i));
    }
    
    loopPos.set(width*0.5, height*0.6);
    
    frame.loadImage("frame.png");
    clouds.loadImage("clouds.png");
    clouds.setAnchorPercent(0.5, 0.5);
    
    font.loadFont("_aeiou2U.ttf", 20);
    
    bicefal.load("bicefalo.xml", ofPoint( width*0.5, height*0.3),0.7);
    ouroboros.load("ouroboros.xml", loopPos, 0.6 );
    
    ofPoint dir = ( *(ouroboros[ ouroboros.size() - 1]) - loopPos);
    initialAngle = atan2(dir.y,dir.x);
    
    bPlay   = true;
    bNoise  = true;
    bLoop   = true;
    
    bDebug  = false;
    bScreenShot = false;
}

//--------------------------------------------------------------
void testApp::update(){

    if (bPlay){
        VF.fadeField(0.99);
        
        if (bNoise)
            VF.noiseField(0.01f, 0.01, 3, true);
        
        if (bLoop){
            VF.addCounterClockwiseCircle( loopPos.x, loopPos.y, 200, 0.03);
        }
        
        bicefal.collideWith( &ouroboros );
        bicefal.update();   // &VF );
    
        ouroboros.update( &VF );
        
        ofPoint dir = (*(ouroboros[ ouroboros.size() - 1]) - loopPos);
        float angle = atan2(dir.y,dir.x) - initialAngle;
        
        if ( abs(angle) <= 0.01){
            if (!blockCounter){
                counter++;
                blockCounter = true;
            }
        } else {
            blockCounter = false;
        }
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    if (bScreenShot){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    ofBackground(255);
    
    ofSetColor(0,100);
//    background.draw();
    
    ofSetColor(255,200);
    ofCircle(loopPos, 200);
    ofSetColor(255);
    clouds.draw(loopPos);
    
    ofSetColor(0);
    font.drawString("i = " + ofToString(counter), width*0.15, height*0.3);
    font.drawString("i++", width*0.75, height*0.3);
    
    if (bDebug){
        ofDisableSmoothing();
        ofSetColor(0,20);
        VF.draw();
        ofEnableSmoothing();
    }
    
    ouroboros.draw();
    bicefal.draw();
    
    frame.draw(0, 0);
    
    if (bScreenShot){
        ofEndSaveScreenAsPDF();
        bScreenShot = false;
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'p'){
        bPlay = !bPlay;
    } else if (key == 'r'){
        ouroboros.restart();
        bicefal.restart();
        counter = 0;
    } else if (key == 'n'){
        bNoise = !bNoise;
    } else if (key == 'l'){
        bLoop = !bLoop;
    } else if (key == 'd'){
        bDebug = !bDebug;
        bicefal.bDebug = bDebug;
        ouroboros.bDebug = bDebug;
    } else if (key == 's'){
        bScreenShot = true;
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
    float diffx = x - prevMouseX;
    float diffy = y - prevMouseY;
        
    VF.addVectorCircle((float)x, (float)y, diffx*0.3, diffy*0.3, 60, 0.3f);
        
    prevMouseX = x;
    prevMouseY = y;
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