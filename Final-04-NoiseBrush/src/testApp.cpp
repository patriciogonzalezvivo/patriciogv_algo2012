#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
//    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    size = 300;
    
    gui.setup("Panel");
    gui.add(noiseZoom.setup("noise_zoom",5,0,100));
    
    noise.allocate(size, size);
    brush.allocate(size, size);
    canvas.allocate(ofGetScreenWidth(), ofGetScreenHeight(),GL_RGBA);
    canvas.setBlendMode(BLEND_NORMAL);
    canvas.clear(0);
    canvas.begin(0);
//    ofClear(0,0);
    ofImage background("background.jpg");
    ofSetColor(255);
    background.draw(0, 0);
    canvas.end(0);
    
    ofImage brushImage("brush.png");
    brush.begin(1);
    ofClear(0,0);
    ofSetColor(255);
    brushImage.draw(0, 0,size,size);
    brush.end(1);
    
    hueCircle = 0.0;
    color.set(255,0,0);
}

//--------------------------------------------------------------
void testApp::update(){
    noise.setZoom(noiseZoom);
    noise.update();
    
    brush << noise;
    brush.update();
    
    hueCircle += 0.001;
    if (hueCircle >= 1.0){
        hueCircle = 0.0;
    }
    color.setHue(hueCircle*255);

    ofPoint diff = prevMouse - mouse;
    float angleTo = atan2(diff.y, diff.x);
    
    float diffAngle = (angleTo - angle);
    if (diffAngle < -PI) diffAngle += TWO_PI;
    if (diffAngle > PI) diffAngle -= TWO_PI;
    angle  += 0.1 * diffAngle;
    
    canvas.begin();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofClear(0, 0);
    ofFill();
    ofPushMatrix();
    ofTranslate(mouse);
    ofRotateZ(angle * RAD_TO_DEG);
//    ofScale(ofMap(diff.length(),2,10,1.0,3.0,true), ofMap(diff.length(),2,10,1.0,0.8,true));
    ofSetColor(color);
    brush.draw(-brush.getWidth()*0.4,-brush.getHeight()*0.5);
    ofPopMatrix();
    canvas.end();
    canvas.setBlendMode( BLEND_ADD );
    canvas.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackgroundGradient(ofColor::gray, ofColor::black);
    canvas.draw(0, 0);
    
    gui.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'f')
        ofToggleFullscreen();
    else if (key == ' '){
        canvas.clear(0);
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
    prevMouse = mouse;
    mouse.set(x,y);
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

}