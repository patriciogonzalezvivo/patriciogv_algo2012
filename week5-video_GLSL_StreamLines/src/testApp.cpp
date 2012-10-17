#include "testApp.h"
#define STRINGIFY(A) #A

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    
    width = 640;
    height = 480;
    scale = 5;
    video.initGrabber(width, height);
    
    grayscale.allocate(width, height);
    blur.allocate(width, height);
    blur.setRadius(5);
    normals.allocate(width, height);
    
    canvas.allocate(ofGetWidth(),ofGetHeight());
    canvas.begin();
    ofClear(0);
    canvas.end();
    
    VF.setupField(width/scale,height/scale,ofGetWidth(), ofGetHeight());
    VF.noiseField(1.0);

    bDrawVideo = false;
    bDrawField = false;
    bGetNormals = true;
}

//--------------------------------------------------------------
void testApp::update(){
    video.update();
    
    if ( video.isFrameNew() ){
    
        if (bGetNormals){
            
            //  Use ofxFX to transform the image to grayscale
            //
            grayscale.setTexture( video.getTextureReference() );
            grayscale.update();
            
            //  Blur the grayscale image
            //
            blur << grayscale;
            blur.update();
            
            //  Calculate the normals
            //
            normals << blur;
            normals.update();
            
            //  Get the normals from the GPU and pass them to the vectorField
            //
            ofPixels normPixels;
            normals.getTextureReference().readToPixels(normPixels);
    
            int scaledWidth = width/scale;
            int scaledHeight = height/scale;
            for(int x = 0; x <= scaledWidth; x++){
                for(int y = 0; y <= scaledHeight; y++){
                    ofFloatColor normalColor = normPixels.getColor(x*scale, y*scale);
                    
                    int index = x + y * scaledWidth;
                    ofPoint norm = ofPoint((normalColor.r - 0.5) * 2.0,
                                           (normalColor.g - 0.5) * 2.0,
                                           0.0);
                    VF[index] =  norm * 3;
                }
            }
            
            bGetNormals = false;
            
            //  Clear the particles
            //
            particles.clear();
            canvas.begin();
            ofClear(0);
            canvas.end();
            
            //  Insert one particle for each pixel and give them a color
            //
            pixels.setFromPixels(video.getPixels(), video.getWidth(), video.getHeight(), 3);
            grayscale.getTextureReference().readToPixels(brightpixels);
        
            for (int x = 0; x < width/scale; x++){
                for(int y = 0; y < height/scale; y++){
                    Particle myParticle;
                    myParticle.init(ofPoint(ofMap(x*scale,0, width,0, ofGetWidth()),
                                            ofMap(y*scale,0,height,0,ofGetHeight()),
                                            0.0),
                                    ofPoint(0,0));
                    ofColor pixelColor = pixels.getColor(x*scale, y*scale);
                    myParticle.color.set(pixelColor);
                    myParticle.size = scale;
                    particles.push_back(myParticle);
                }
            }
        }
    }
    
    for (int i = 0; i < particles.size(); i++){
        if ((particles[i].x > 0) && (particles[i].x < ofGetWidth()) &&
            (particles[i].y > 0) && (particles[i].y < ofGetHeight())){
            ofPoint force = VF.getForceFromPos( particles[i]  );
            particles[i].addForce( force );
            particles[i].update();
            
            //int pixelX = ofMap(particles[i].x,0,ofGetWidth(),0,width-1,true);
            //int pixelY = ofMap(particles[i].y,0,ofGetHeight(),0,height-1,true);
            //particles[i].color.set( pixels.getColor( pixelX, pixelY) );
            //particles[i].size = scale * (brightpixels.getColor( pixelY, pixelY).r/255.0f) ;
        }
	}
    
    /*
    canvas.begin();
    ofSetColor(ofFloatColor(0.0,0.004));
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    for (int i = 0; i < particles.size(); i++){
		particles[i].draw(false);
	}
    canvas.end();
    */
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    if (bDrawVideo){
        ofSetColor(255);
        video.draw(0,0,ofGetWidth(),ofGetHeight());
    }
    
    ofSetColor(255);
    //canvas.draw(0, 0);
    
    for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}
    
    if ( bDrawField ){
        ofDisableSmoothing();
        ofSetColor(130,0,0, 200);
        VF.draw();
        ofEnableSmoothing();
    }
    
    ofSetColor(200,0,0);
    ofDrawBitmapString("Press 'v' to show/hide video, 'f' to show/hide the vectorField and SPACE_BAR to make a streamLine from the image ", 15,15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == 'v'){
        bDrawVideo = !bDrawVideo;
    } else if (key == 'f'){
        bDrawField = !bDrawField;
    } else {
        bGetNormals = true;
        
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