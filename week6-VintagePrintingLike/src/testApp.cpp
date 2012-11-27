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
    
    scale = 7.6;
    init(ofGetScreenWidth(),ofGetScreenHeight(),1024,768);
    video.initGrabber(640, 480);
    init(640,480,1024,768);
    bImage = false;
}

void testApp::init(int _width, int _height, int _screenWidth, int _screenHeight){
    width = _width;
    height = _height;
    ofSetWindowShape(_screenWidth,_screenHeight);
    
    for (int i = 0; i < particles.size(); i++){
        delete particles[i];
    }
    particles.clear();
    VF.clear();
    
    grayscale.allocate(width,height);
    blur.allocate(width, height);
    blur.setRadius(5);
    normals.allocate(width, height);
    brightPixels.allocate(width, height, 1);
    
    VF.setupField(width/scale,height/scale,_screenWidth,_screenHeight);
    
    for (int x = 0; x < width/scale; x++){
        for(int y = 0; y < height/scale; y++){
            Particle *myParticle = new Particle();
            myParticle->init(ofPoint(ofMap(x,0, width/scale,0, _screenWidth),
                                     ofMap(y,0,height/scale,0, _screenHeight),
                                     0.0),
                             ofPoint(0,0));
            myParticle->size = 1;
            particles.push_back(myParticle);
        }
    }
    
    bDrawField  = false;
    bPrintToPdf = false;
}

//--------------------------------------------------------------
void testApp::update(){
    
    if (!bImage){
        video.update();
        
        if ( video.isFrameNew() ){
            process(video.getTextureReference());
        }
    } else {
        process(image.getTextureReference());
    }

    // sort all the particle
    //
	sort( particles.begin(), particles.end(), comparisonFunction );
	
    for (int i = 0; i < particles.size(); i++){
		for (int j = i-1; j >= 0; j--){
            if ( fabs(particles[j]->x - particles[i]->x) > 20) break;
            particles[i]->addRepulsionForce( *particles[j], 0.8);
		}
	}
    
    for (int i = 0; i < particles.size(); i++){
        
        ofPoint force = VF.getForceFromPos( *particles[i]  );
        particles[i]->addForce( force );
        particles[i]->bounceOffWalls();
        
        int xOnVideo = ofMap(particles[i]->x, 0, ofGetWidth(), 0, width,true);
        int yOnVideo = ofMap(particles[i]->y, 0, ofGetHeight(), 0, height,true);
        float size = ofMap(brightPixels.getColor(xOnVideo,yOnVideo).r,
                           0,255,
                           0.2,1.0);
    
        particles[i]->color.set( size-0.2 );
        particles[i]->size = ofLerp(particles[i]->size, (1.0-size)*scale, 0.004);
        particles[i]->update();
	}
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

void testApp::process(ofTexture &_text){
    grayscale.setTexture( _text );
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
            ofPoint norm = ofPoint((normalColor.r - 0.5) * -2.0,
                                   (normalColor.g - 0.5) * -2.0, 0.0);
            
            float pct = 0.5;
            VF[index] = VF[index]*(1.0-pct) + norm * pct;
        }
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(255);
    
    if (bDrawField){
//        ofDisableSmoothing();
//        ofSetColor(100);
//        VF.draw();
//        ofEnableSmoothing();
        
//        blur.draw(0,0,ofGetWidth(),ofGetHeight());
        grayscale.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    
    if (bPrintToPdf){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    for (int i = 0; i < particles.size(); i++){
        ofSetColor(0);
		particles[i]->draw();
	}
    
    if (bPrintToPdf){
        ofEndSaveScreenAsPDF();
        bPrintToPdf = false;
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
                myParticle->init(ofPoint(ofMap(x,0, width/scale,0, ofGetWidth()),
                                         ofMap(y,0,height/scale,0,ofGetHeight()),
                                         0.0),
                                 ofPoint(0,0));
                myParticle->size = 2;
                particles.push_back(myParticle);
            }
        }
        
    } else if ( key == 'f'){
        bDrawField = !bDrawField;
	} else if ( key == 'v'){
        bImage = false;
        init(video.getWidth(),video.getHeight(),1024,768);
	} else if ( key == 'p'){
        bPrintToPdf = true;
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
    if(dragInfo.files.size() > 0){
        bImage = image.loadImage(dragInfo.files[0]);
        if (bImage){
            image.update();
            init(image.getWidth(),image.getHeight(),image.getWidth()*1.2,image.getHeight()*1.2);
            process(image.getTextureReference());
        }
    }
}