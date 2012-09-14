#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetCircleResolution(100);
    ofSetDrawBitmapMode(OF_BITMAPMODE_SIMPLE);
    ofEnableSmoothing();
    
    selectedPoint = -1; // means NO point is selected
    sDot.algorithm = ALGO_LINEAR;
}

//--------------------------------------------------------------
void testApp::update(){
    
    if ( points.size() > 1 ){
        sDot.update();
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    //  Draw all the points
    //
    ofFill();
    for (int i = 0; i < points.size(); i++){
        ofSetColor(255,100);
        ofDrawBitmapString(ofToString(i), points[i]->x+5, points[i]->y-5);
        
        ofSetColor(255,100);
        ofRect(points[i]->x-5, points[i]->y-5, 10, 10);
    }
    
    if ( points.size() > 1 ){
        sDot.draw();
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    
    //  Check if the user click over a point.
    //
    for (int i = 0; i < points.size(); i++){
        
        //  Ask each one of the dot if the mouse is over it
        //
        if ( points[i]->distance(ofPoint(x,y)) < 30 ){
            
            //  Remember the point clicked.
            //  In order to drag it until it's released
            //
            selectedPoint = i;
            break;
        }
    }
    
    //  if you click on an empty space.
    //  It will creat a new point.
    //
    if (selectedPoint == -1){
        ofPoint *newPoint = new ofPoint(x,y);
        points.push_back( newPoint );
        
        if ( points.size() > 1 ){
            sDot.init( points[ points.size()-2 ], points[ points.size()-1 ]);
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    
    //  If one point is selected move it to the new mouse position
    //
    if (selectedPoint != -1){
        points[selectedPoint]->set(x,y);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
    //  If a point is selected drop it
    //
    if (selectedPoint != -1){
        selectedPoint = -1;
    }
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