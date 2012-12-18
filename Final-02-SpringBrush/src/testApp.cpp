#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
//    ofSetVerticalSync(true);
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
}

//--------------------------------------------------------------
void testApp::update(){
    brush.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    brush.draw();
    
    brushPanel.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'd'){
        brush.bDebug = !brush.bDebug;
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