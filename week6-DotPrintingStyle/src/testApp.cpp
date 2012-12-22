#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    width = 640;
    height = 480;
    scale = 10;
    video.initGrabber(width, height);
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    blur.setRadius(5);
    normals.allocate(width, height);
    
    VF.setupField(width/scale,height/scale,ofGetWidth(), ofGetHeight());
//    VF.noiseField();
    
    for (int x = 0; x < width/scale; x++){
        for(int y = 0; y < height/scale; y++){
            Particle myParticle;
            myParticle.init(ofPoint(ofMap(x,0, width/scale,0, ofGetWidth()),
                                    ofMap(y,0,height/scale,0,ofGetHeight()),
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
    video.update();
    
    if ( video.isFrameNew() ){
        grayscale.setTexture( video.getTextureReference() );
        grayscale.update();
        blur << grayscale;
        blur.update();
        normals << blur;
        normals.update();
        
        ofPixels normPixels;
        normals.getTextureReference().readToPixels(normPixels);
        grayscale.getTextureReference().readToPixels(brightPixels);
        
        int scaledWidth = width/scale;
        int scaledHeight = height/scale;
        for(int x = 0; x <= scaledWidth; x++){
            for(int y = 0; y <= scaledHeight; y++){
                
                int scaledX = ofClamp(x*scale,0,width);
                int scaledY = ofClamp(y*scale,0,width);
                
                ofFloatColor normalColor = normPixels.getColor(scaledX, scaledY);
                
                int index = x + y * scaledWidth;
                ofPoint norm = ofPoint((normalColor.r - 0.5) * 2.0,
                                       (normalColor.g - 0.5) * 2.0, 0.0);
    
                float pct = 0.5;
                VF[index] = VF[index]*(1.0-pct) + norm * pct;
            }
        }
    }

    for (int i = 0; i < particles.size(); i++){
		for (int j = 0; j < i; j++){
            particles[i].addRepulsionForce(particles[j], 0.7);
		}
	}
    
    for (int i = 0; i < particles.size(); i++){
        
        ofPoint force = VF.getForceFromPos( particles[i]  );
        particles[i].addForce( force );
//        particles[i].infinitWalls();
        particles[i].bounceOffWalls();
        
        int xOnVideo = ofMap(particles[i].x, 0, ofGetWidth(), 0, width,true);
        int yOnVideo = ofMap(particles[i].y, 0, ofGetHeight(), 0, height,true);
        float size = ofMap(brightPixels.getColor(xOnVideo,yOnVideo).r,
                           0,255,
                           0.5,scale);
        particles[i].size = ofLerp(particles[i].size, size, 0.1);
        
        
        particles[i].update();
	}
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    if (bDrawField){
        ofSetColor(255);
        VF.draw();
    }
    
    for (int i = 0; i < particles.size(); i++){
        ofSetColor(255);
		particles[i].draw();
	}
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        particles.clear();
        
        for (int x = 0; x < width/scale; x++){
            for(int y = 0; y < height/scale; y++){
                Particle myParticle;
                myParticle.init(ofPoint(ofMap(x,0, width/scale,0, ofGetWidth()),
                                        ofMap(y,0,height/scale,0,ofGetHeight()),
                                        0.0),
                                ofPoint(0,0));
                myParticle.size = 2;
                particles.push_back(myParticle);
            }
        }
        
    } else if ( key == 'v'){
        bDrawField = !bDrawField;
	} else if ( key == 'f'){
        ofToggleFullscreen();
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