#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    brushPanel.setup("brush");
    brushPanel.add(brushWidth.setup("width",25,0,50));
    brushPanel.add(brushNumber.setup("number",10,1,50));
    brushPanel.add(brushDamp.setup("damp",0.245,0.0,0.5));
    brushPanel.add(brushSoftness.setup("softness",0.0825,0.0,0.5));
    brushPanel.add(brushHeight.setup("height",0.1,0.0,2.0));
    
    brush.setWidth(50);
    brush.setNum(50);
    
    canvas.allocate(ofGetScreenWidth(), ofGetScreenHeight());
    canvas.begin();
    ofClear(0,0);
    canvas.end();
    
    bDebug = false;
}

//--------------------------------------------------------------
void testApp::update(){
    brush.update();
    canvas.begin();
    brush.draw();
    canvas.end();
}

//--------------------------------------------------------------
void testApp::draw(){

    canvas.draw(0, 0);

    if (bDebug) {
        brush.drawDebug();
        brush.drawPathFollower();
    }
    
    brushPanel.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        bDebug = !bDebug;
    } else if (key == ' '){
        brush.clear();
        canvas.begin();
        ofClear(0,0);
        canvas.end();        
    } else if (key == 'f'){
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
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    brush.setNum(brushNumber);
    brush.setWidth(brushWidth);
    brush.damp = brushDamp;
    brush.height = brushHeight;
    brush.softness = brushSoftness;
    
    ofColor color(255,0,0);
    color.setHue(abs(sin(ofGetElapsedTimef()))*255);
    brush.setColor(color);
    brush.begin();
    brush.set(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    brush.set(x, y);
    brush.end();
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