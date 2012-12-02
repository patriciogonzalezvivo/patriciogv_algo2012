#include "testApp.h"
#define STRINGIFY(A) #A

// comparison routine for sort...
bool comparisonFunction(  Particle * a, Particle * b ) {
	return a->x < b->x;
}

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    init(ofGetScreenWidth(),ofGetScreenHeight(),1024,768);
    video.initGrabber(640, 480);
    init(640,480,1024,768);
    bImage = false;
    
    noise = 1.0f;
    
    bTrails = false;
}

void testApp::init(int _width, int _height, int _screenWidth, int _screenHeight){
    width = _width;
    height = _height;
    scale = 10;
    
    //  Setup
    //
    ofSetWindowShape(_screenWidth,_screenHeight);
    
    //  Pre-Process
    //
    grayscale.allocate(width,height);
    blur.allocate(width, height);
    blur.setRadius(10);
    blur.setPasses(10);
    normals.allocate(width, height);
    
    //  Process
    //
    VF.clear();
    VF.setupField(width/scale,height/scale,_screenWidth,_screenHeight);
    pixels.allocate(width, height, 4);
    for (int i = 0; i < particles.size(); i++){
        delete particles[i];
    }
    particles.clear();
    for (int x = 0; x < width/scale; x++){
        for(int y = 0; y < height/scale; y++){
            Particle *myParticle = new Particle();
            myParticle->init(ofPoint(ofMap(x,0, width/scale,0, _screenWidth),
                                     ofMap(y,0,height/scale,0, _screenHeight),
                                     0.0),
                             ofPoint(0,0));
            myParticle->size = 10;
            particles.push_back(myParticle);
        }
    }
    
    //  Render
    //
    fbo.allocate(_screenWidth, _screenHeight);
    fbo.begin();
    ofClear(0,0);
    fbo.end();
    
    bDrawField  = false;
    bPrintScreen = false;
}

//--------------------------------------------------------------
void testApp::preprocess(ofTexture &_text){
    grayscale.setTexture( _text );
    grayscale.update();
    blur << grayscale;
    blur.update();
    normals << blur;
    normals.update();
    
    ofPixels normPixels;
    normPixels.allocate(normals.getWidth(), normals.getHeight(), 4);
    normals.getTextureReference().readToPixels(normPixels);
    _text.readToPixels(pixels);
    
    int scaledWidth = width/scale;
    int scaledHeight = height/scale;
    for(int x = 0; x <= scaledWidth; x++){
        for(int y = 0; y <= scaledHeight; y++){
            
            int scaledX = ofClamp(x*scale,0,width-1);
            int scaledY = ofClamp(y*scale,0,height-1);
            
            ofFloatColor normalColor = normPixels.getColor(scaledX, scaledY);
            
            int index = x + y * scaledWidth;
            ofPoint norm = ofPoint((normalColor.r - 0.5) * 2.0,
                                   (normalColor.g - 0.5) * 2.0, 0.0);
            
            float pct = 0.5;
            VF[index] = VF[index]*(1.0-pct) + norm * pct;
        }
    }
}

void testApp::update(){
    
    //  INPUT + PREPROCESS
    //
    if (!bImage){
        video.update();
        if ( video.isFrameNew() )
            preprocess(video.getTextureReference());
    } else {
        preprocess(image.getTextureReference());
    }

    //  PROCESS
    //
    
    //  Sort all the particle
    //
	sort( particles.begin(), particles.end(), comparisonFunction );
	
    //  Particle - Particle Interaction
    //
    for (int i = 0; i < particles.size(); i++){
		for (int j = i-1; j >= 0; j--){
            if ( fabs(particles[j]->x - particles[i]->x) > 20) break;
            particles[i]->addRepulsionForce( *particles[j], 0.8);
		}
	}
    
    //  Particle - VF
    //
    VF.noiseField( noise, 0.01, 3, true);
    for (int i = 0; i < particles.size(); i++){
        
        //  Position
        //
        int xOnVideo = ofMap(particles[i]->x, 0, ofGetWidth(), 0, width-1,true);
        int yOnVideo = ofMap(particles[i]->y, 0, ofGetHeight(), 0, height-1,true);
        
        particles[i]->color.lerp( pixels.getColor(xOnVideo, yOnVideo) , 0.01);
        float  brigtness = particles[i]->color.getBrightness();
        
        particles[i]->size = (0.1 + (1.0 - brigtness)*0.9 ) * (scale*2.0);
        particles[i]->damping = ofMap( brigtness ,0.0,1.0,0.1,0.01);
        
        ofPoint force = VF.getForceFromPos( *particles[i] );
        particles[i]->addForce( force );
        particles[i]->bounceOffWalls();
        particles[i]->update();
	}
        
    ofSetWindowTitle("fps: "+ofToString(ofGetFrameRate()) + " noise: " + ofToString(noise));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    
    
    if (bTrails){
        
        fbo.begin();
        for (int i = 0; i < particles.size(); i++){
            ofSetColor(0);
            particles[i]->draw();
        }
        fbo.end();
        
        ofSetColor(255);
        fbo.draw(0, 0);
        
        if (bPrintScreen){
            ofSaveScreen("screenshot-"+ofGetTimestampString()+".png");
            bPrintScreen = false;
        }
    } 
    
    if (bPrintScreen){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    for (int i = 0; i < particles.size(); i++){
        ofSetColor(0);
        particles[i]->draw();
    }
    
    if (bPrintScreen){
        ofEndSaveScreenAsPDF();
        bPrintScreen = false;
    }
    
    
    if (bDrawField){
        ofSetColor(255,100);
        normals.draw(0,0,ofGetWidth(),ofGetHeight());
        ofDisableSmoothing();
        ofSetColor(100);
        VF.draw();
        ofEnableSmoothing();
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == ' '){
        for (int i = 0; i < particles.size(); i++){
            delete particles[i];
        }
        particles.clear();
        
        for (int x = 0; x < width/scale; x++){
            for(int y = 0; y < height/scale; y++){
                Particle *myParticle = new Particle();
//                myParticle->init(ofPoint(ofMap(x,0, width/scale,0, ofGetWidth()),ofMap(y,0,height/scale,0,ofGetHeight()),0.0),ofPoint(0,0));
                myParticle->init(ofPoint(ofRandom(ofGetWidth()),ofRandom(ofGetHeight()),0.0),ofPoint(0,0));
                myParticle->size = 2;
                particles.push_back(myParticle);
            }
        }
        
        fbo.begin();
        ofClear(0,0);
        fbo.end();
        
    } else if ( key == 'f'){
        bDrawField = !bDrawField;
	} else if ( key == 'v'){
        bImage = false;
        init(video.getWidth(),video.getHeight(),1024,768);
	} else if ( key == 'p'){
        bPrintScreen = true;
    } else if ( key == 't'){
        bTrails = !bTrails;
    }
    
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    noise   = ofMap(y, 0, ofGetHeight(), 0.1, 0.5);
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
    if(dragInfo.files.size() > 0){
        bImage = image.loadImage(dragInfo.files[0]);
        if (bImage){
            image.update();
            init(image.getWidth(),image.getHeight(),image.getWidth()*1.2,image.getHeight()*1.2);
            preprocess(image.getTextureReference());
        }
    }
}