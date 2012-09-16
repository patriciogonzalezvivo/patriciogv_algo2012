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

    
    bPlay = true;
    bConvert = false;
    bRecording = false;
    movieCounter = 0;
    
    
    //  Load data folder
    //
    dataDir.listDir(".");
    
    selectedDir = -1;
    
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
    if (bPlay){
        sMotion.nextFrame();
    }
    sMotion.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    ofBackgroundGradient(ofColor(0), ofColor(50), OF_GRADIENT_LINEAR);
    
    //  Header
    //
    ofSetColor(255);
    ofDrawBitmapString("Press 'r' to start recording or drop a MOVIE file on to the app to convert", 10, 15);
    ofDrawBitmapString("Press 'SPACEBAR' to start and stop. RIGHT and LEFT keys makes it Next and Prev frame", 10, 30);
    
    ofPushMatrix();
    ofTranslate(10, 45);
    
    //  Draw StopMotion
    //
    ofSetColor(255);
    videoIn.draw(0, 0, width * 0.5, height * 0.5);
    if (bRecording){
        ofSetColor(255, 0, 0, abs ( sin( ofGetElapsedTimeMillis() * 0.001 ) ) * 255 );
        ofCircle(25, 25, 7);
        ofDrawBitmapString("REC", 35,30);
    }
    
    //  Draw Dir List
    //
    stringstream dirList;
    dirList << "Data: ( UP / DOWN keys ) " << endl;
    dirList << endl;
    for(int i = 0; i < dataDir.numFiles(); i++){
        dirList << dataDir.getFile(i).getFileName();
        if ( selectedDir != -1 ){
            if ( i == selectedDir ){
                dirList << " <--";
            }
        }
        dirList << endl;
    }
    ofDrawBitmapString(dirList.str(), 10, height*0.5 + 25 );
    
    ofPopMatrix();
    
    //  Draw StopMotion
    //
    sMotion.draw(width*0.5 + 10 + 10, 45);
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    if( key == ' '){
        bPlay = !bPlay;
    }
    
    if ( key == OF_KEY_RIGHT){
        sMotion.nextFrame();
    }
    
    if ( key == OF_KEY_LEFT){
        sMotion.prevFrame();
    }
    
    if (key == 'r'){
        bRecording = !bRecording;
        if (!bRecording){
            sMotion.save( "movie" + ofToString(movieCounter++) );
            dataDir.listDir(".");
        } else {
            sMotion.clear();
        }
    }
    
    if ( key == OF_KEY_DOWN){
        selectedDir = (selectedDir+1)%dataDir.numFiles();
        
        if ( dataDir.getFile(selectedDir).isDirectory() ) {
            sMotion.load( dataDir.getFile(selectedDir).getFileName() );
        }
            
    } else if ( key == OF_KEY_UP){
        
        if (selectedDir > 0 ) {
            selectedDir--;
        }
        
        if ( dataDir.getFile(selectedDir).isDirectory() ) {
            sMotion.load( dataDir.getFile(selectedDir).getFileName() );
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