#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
//    ofEnableSmoothing();
    
    scale = 0.7;
    width = 1200*scale;
    height = 1048*scale;
    ofSetWindowShape(width, height);
    
    font.loadFont("_aeiou2U.ttf", 18);
    frame.loadImage("frame.png");
    label.loadImage("lable.png");
    
    loopPos.set(width*0.5, height*0.5);
    clouds.load("clouds.xml", loopPos,scale);
    ouroboros.load("ouroboros.xml", loopPos, 0.7*scale);
    VF.setupField( width/10, height/10, width, height);
    
    ofPoint dir = ( *(ouroboros[ ouroboros.size() - 1]) - loopPos);
    initialAngle = atan2(dir.y,dir.x);
    counter = 0;
    blockCounter = true;

    bPlay   = true;
    bNoise  = true;
    bLoop   = true;
    
    bDebug  = true;
    clouds.bDebug = bDebug;
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
    
    if (bDebug){
        ofDisableSmoothing();
        ofSetColor(0,20);
        VF.draw();
        ofEnableSmoothing();
    }
    
    ofSetColor(255,100);
    ofCircle(loopPos, 100);
    ofSetColor(255);

    ofSetColor(255);
    clouds.draw();
    
    ofSetColor(255);
    ouroboros.draw();
    
    ofSetColor(255);
    frame.draw(0, 0,width,height);
    
    ofSetColor(255);
    float _labelScale = 0.62;
    label.draw(width*0.5-label.getWidth()*0.5*_labelScale, height*0.1, label.getWidth()*_labelScale,label.getHeight()*_labelScale);
    
    ofPushMatrix();
    ofTranslate(width*0.121, height*0.295);
    ofRotateZ(-3);
    ofSetColor(inkColor,200);
    font.drawString("i= " + ofToString(counter), 0,0);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(width*0.8, height*0.31);
    ofRotateZ(10);
    ofSetColor(0,200);
    font.drawString("i++",0,0);
    ofPopMatrix();
    
    ofSetColor(0,200);
    font.drawString("i less than infinite", width*0.5- font.getStringBoundingBox("i less than infinite", 0, 0).width*0.5,height*0.18);
    
    ofSetColor(0,200);
    font.drawString("The Eternal Recurrence", width*0.5- font.getStringBoundingBox("The Eternal Recurrence", 0, 0).width*0.5,height*0.95);
    
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