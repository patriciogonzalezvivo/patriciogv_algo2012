#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    gui.setup("brush");
    gui.add(brushWidth.setup("width",25,0,50));
    gui.add(brushNumber.setup("number",10,1,50));
    gui.add(brushDamp.setup("damp",0.245,0.0,0.5));
    
    gui.add(brushHeight.setup("height",0.1,0.0,20.0));
    gui.add(brushK.setup("k",0.0825,0.0,0.5));
    gui.add(brushLerp.setup("lerp", 0.1, 0.0, 1.0));
    
    gui.add(brushRepRad.setup("repulsion_rad", 5, 0, 20));
    gui.add(brushRepPct.setup("repulsion_pct", 0.5, 0.0, 1.0));
    
    gui.loadFromFile("settings.xml");
    
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
    
    gui.draw();
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
    brush.k = brushK;
    brush.lerp = brushLerp;
    brush.repPct = brushRepPct;
    brush.repRad = brushRepRad;
    
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