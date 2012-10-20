#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    int width = 100*0.25;
    int height = 80*0.25;
    
    VF.setupField(width*4,height*4,ofGetWidth(), ofGetHeight());
    VF.noiseField();
    
    for (int x = 0; x < width; x++){
        for(int y = 0; y < height; y++){
            Particle myParticle;
            myParticle.init(ofPoint(ofMap(x,0, width,0, ofGetWidth()),
                                    ofMap(y,0,height,0,ofGetHeight()),
                                    0.0),
                            ofPoint(0,0));
            myParticle.size = 2;
            particles.push_back(myParticle);
        }
    }
    
    bDrawField = false;
}

//--------------------------------------------------------------
void testApp::update(){
   
    for (int i = 0; i < particles.size(); i++){
		
//		particles[i].addAttractionForce(ofPoint(mouseX,mouseY), 1000, 0.07f);
		
		for (int j = 0; j < i; j++){
//			particles[i].addRepulsionForce(particles[j], 200, 0.001);
//            particles[i].addCounterClockwiseForce(particles[j], 80, 0.09);
            
            particles[i].addRepulsionForce(particles[j], 10, 0.7);
//            particles[i].addAttractionForce(particles[j], 50, 0.001f);
		}
	}
    
    for (int i = 0; i < particles.size(); i++){
        
        if ((particles[i].x > 0) && (particles[i].x < ofGetWidth()) &&
            (particles[i].y > 0) && (particles[i].y < ofGetHeight())){
            ofPoint force = VF.getForceFromPos( particles[i]  );
            particles[i].addForce( force );
            particles[i].update();
            //particles[i].infinitWalls();
        }
	}
    
//    VF.propagate();
    VF.fadeField(0.99);
    VF.noiseField(0.01,0.0001,3,true);
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    if (bDrawField){
        VF.draw();
    }
    
    for (int i = 0; i < particles.size(); i++){
		particles[i].draw(false);
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        particles.clear();
        
        int width = 100*0.25;
        int height = 80*0.25;
        
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
        
    } else if ( key == 'v'){
        bDrawField = !bDrawField;
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
//    VF.addOutwardCircle((float)x, (float)y, 60, 0.3f);
    
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