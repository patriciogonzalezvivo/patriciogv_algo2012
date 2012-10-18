#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){	
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	
    width = 64;
    height = 48;
    
    VF.setupField(width,height,ofGetWidth(), ofGetHeight());
    VF.noiseField(1.0);
    
    for (int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            Particle myParticle;
            myParticle.init(ofPoint(ofMap(x,0, width,0, ofGetWidth()),
                                    ofMap(y,0,height,0,ofGetHeight()),
                                    0.0),
                            ofPoint(0,0));
            particles.push_back(myParticle);
        }
    }
    
    bDrawField = false;
    bSignedNoise = true;
    bFullscreen = false;
}

//--------------------------------------------------------------
void testApp::update(){

    
	for (int i = 0; i < particles.size(); i++){
        ofPoint force = VF.getForceFromPos( particles[i]  );
        particles[i].addForce( force );
        particles[i].update();
        particles[i].infinitWalls();        
	}
    
    VF.noiseField(0.01,0.0001,3,bSignedNoise);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
	ofBackground(0);
    
    if (bDrawField){
        ofSetColor(0,130,130, 200);
        VF.draw();
    }
	
	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
    
    ofSetColor(255,100);
    ofDrawBitmapString("Press 's' to toogle between signed and unsigned noise and SPACE_BART to see the vector field", 15,15);
	
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){ 
	
	if (key == ' '){
        bDrawField = !bDrawField;
	} else if (key == 's'){
        bSignedNoise = !bSignedNoise;
    } else if(key == 'f'){
        bFullscreen = !bFullscreen;
        ofSetFullscreen(bFullscreen);
    }
}

//--------------------------------------------------------------
void testApp::keyReleased  (int key){ 
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
void testApp::mouseReleased(){
}
