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
        
    loopPos.set(width*0.5, height*0.6);
    
    frame.loadImage("frame.png");
    font.loadFont("_aeiou2U.ttf", 20);
    
    clouds.load("clouds.xml", loopPos);
    bicefal.load("bicefalo.xml", ofPoint( width*0.5, height*0.35),0.7);
    ouroboros.load("ouroboros.xml", loopPos, 0.7);
    
    ofPoint dir = ( *(ouroboros[ ouroboros.size() - 1]) - loopPos);
    initialAngle = atan2(dir.y,dir.x);
    counter = 0;
    blockCounter = true;

    bPlay   = true;
    bNoise  = true;
    bLoop   = true;
    
    bDebug  = false;
    clouds.bDebug = bDebug;
    bicefal.bDebug = bDebug;
    ouroboros.bDebug = bDebug;
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
        bicefal.collideWith( &clouds );
        bicefal.update( );//&VF );
    
        clouds.collideWith( &ouroboros );
        clouds.update( &VF , false );
        ouroboros.update( &VF );
        
        ofPoint dir = (*(ouroboros[ ouroboros.size() - 1]) - loopPos);
        float angle = atan2(dir.y,dir.x) - initialAngle;
        
        if ( abs(angle) <= 0.1){
            if (!blockCounter){
                counter++;
                blockCounter = true;
            }
            
            inkColor.lerp(ofColor(255,0,0), 0.1);
        } else {
            blockCounter = false;
            inkColor.lerp(ofColor(0), 0.01);
            
//            if (counter > 3){
//                ouroboros.restart();
//                bicefal.restart();
//                clouds.restart();
//                counter = 0;
//                blockCounter = true;
//            }
            
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
    
    ofSetColor(255,200);
    ofCircle(loopPos, 200);
    ofSetColor(255);

    ofSetColor(255,200);
    clouds.draw();
    
    ofSetColor(inkColor,200);
    font.drawString("i = " + ofToString(counter), width*0.15, height*0.3);
    ofSetColor(0,200);
    font.drawString("i++", width*0.75, height*0.3);
    
    
    if (bDebug){
        ofDisableSmoothing();
        ofSetColor(0,20);
        VF.draw();
        ofEnableSmoothing();
    }
    
    ofSetColor(255);
    bicefal.draw();
    
    ofSetColor(255);
    ouroboros.draw();
    
    ofSetColor(255);
    frame.draw(0, 0);
    
    ofSetColor(0,200);
    font.drawString("The Eternal Recurrence", width*0.5-190,1010);
    
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
        clouds.restart();
        counter = 0;
        blockCounter = true;
    } else if (key == 'n'){
        bNoise = !bNoise;
    } else if (key == 'l'){
        bLoop = !bLoop;
    } else if (key == 'd'){
        bDebug = !bDebug;
        clouds.bDebug = bDebug;
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