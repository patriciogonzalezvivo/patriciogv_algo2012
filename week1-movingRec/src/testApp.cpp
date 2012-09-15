#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetFrameRate(24);
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofEnableAlphaBlending();
    
    width = 640;
    height = 480;
    
    videoIn.initGrabber(width , height);

    sMotion.allocate(width, height);

    
    bConvert = false;
    bRecording = false;
    movieCounter = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    
    
    if ( bConvert ){
        //  If it have to convert a movie into stopMotion
        //
        movieIn.update();
        
        if ( movieIn.getIsMovieDone() ){
            sMotion.save( "convertedMovie" );
            movieIn.stop();
            bConvert = false;
        } else {
            if (movieIn.isFrameNew()){
                sMotion.addFrame( movieIn.getPixels() );
            }
        }
        
    } else {
        //
        //
        videoIn.update();
        if (videoIn.isFrameNew()){
            if (bRecording){
                sMotion.addFrame( videoIn.getPixels() );
            }
        }
    }
    
    //  Play StopMotion
    //
    sMotion.nextFrame();
    sMotion.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofDrawBitmapString("Press SPACEBAR to start recording or drop a MOVIE file on to the app to convert", 10, 15);
    ofDrawBitmapString("The StopMotion are going to be store in your data folder", 10, 30);
    
    //  Draw StopMotion
    //
    ofPushMatrix();
    ofTranslate(10, 45);
    ofSetColor(255);
    videoIn.draw(0, 0, width * 0.5, height * 0.5);
    if (bRecording){
        ofSetColor(255, 0, 0, abs ( sin( ofGetElapsedTimeMillis() * 0.001 ) ) * 255 );
        ofCircle(25, 25, 7);
        ofDrawBitmapString("REC", 35,30);
    }
    ofPopMatrix();
    
    //  Draw StopMotion
    //
    sMotion.draw(width*0.5 + 10 + 10, 45);
}

//--------------------------------------------------------------
void testApp::exit(){
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if (key == ' '){
        bRecording = !bRecording;
        if (!bRecording){
            sMotion.save( "movie" + ofToString(movieCounter++) );
        } else {
            sMotion.clear();
        }
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
    if (movieIn.loadMovie( dragInfo.files[0] )){
        
        sMotion.allocate(movieIn.getWidth(), movieIn.getHeight());
        movieIn.setLoopState(OF_LOOP_NONE);
        movieIn.play();
        
        bConvert = true;
    }
}