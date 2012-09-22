#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetCircleResolution(100);
    ofEnableSmoothing();
    
    selectedPoint = -1; // means NO point is selected
    selectedDot = -1;
}

//--------------------------------------------------------------
void testApp::update(){
    for(int i = 0; i < dots.size(); i++){
        dots[i].update();
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    
    ofSetColor(255, 150);
    ofDrawBitmapString("Press LEFT and RIGHT keys to change the increment factor", 5,15);
    ofDrawBitmapString("Press UP and DOWN keys to change the shaper", 5,30);
    ofDrawBitmapString("Click over the ALGORITHM or the MODE to change it", 5, 45);
    
    //  Draw all the points
    //
    ofFill();
    for (int i = 0; i < points.size(); i++){
        ofSetColor(255,100);
        ofRect(points[i]->x-5, points[i]->y-5, 10, 10);
    }
    
    //  Draw all the dots
    //
    for(int i = 0; i < dots.size(); i++){
        dots[i].draw();
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (selectedDot != -1){
        if (key == OF_KEY_RIGHT){
            dots[selectedDot].incrementFactor += 0.0001;
            if (dots[selectedDot].incrementFactor > 1.0)
                dots[selectedDot].incrementFactor = 1.0;
                
        } else if (key == OF_KEY_LEFT){
            dots[selectedDot].incrementFactor -= 0.0001;
            if (dots[selectedDot].incrementFactor < 0.0)
                dots[selectedDot].incrementFactor = 0.0;
        }
        
        if (key == OF_KEY_UP){
            dots[selectedDot].shaper += 0.01;
        } else if (key == OF_KEY_DOWN){
            dots[selectedDot].shaper -= 0.01;
            if (dots[selectedDot].shaper < 0.0)
                dots[selectedDot].shaper = 0.0;
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
    
    //  Check if the user click over a dot.
    //
    bool hitSomething = false;
    for(int i = 0; i < dots.size(); i++){
        if (dots[i].distance(ofPoint(x,y)) < 30){
            selectedDot = i;
            dots[i].color = ofColor(255,0,255);
            hitSomething = true;
            break;
        } else if (dots[i].mouseClick(x, y)){
            hitSomething = true;
            break;
        }
    }
    
    //  if you click on an empty space.
    //  It will creat a new point.
    //
    if ((selectedPoint == -1) && !hitSomething ){
        ofPoint *newPoint = new ofPoint(x,y);
        points.push_back( newPoint );
        
        // Every two point it will assign a dot for them
        //
        if ( ( points.size() > 1 ) && ( points.size() % 2 == 0 ) ){
            SuperDot newDot;
            newDot.init( points[ points.size()-2 ], points[ points.size()-1 ]);
            
            dots.push_back( newDot );
            selectedDot = dots.size() - 1;
        }
    }
    
    //  Match the selectedDot with the selected color
    //
    for(int i = 0; i < dots.size(); i++){
        if (i == selectedDot){
            dots[i].color = ofColor(255,0,255);
        } else {
            dots[i].color = ofColor(255);
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