#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofSetCircleResolution(100);
    ofEnableSmoothing();
    
    for(int i = 0; i < 30; i++){
        wParticle newParticle;
        particles.push_back(newParticle);
    }
}

//--------------------------------------------------------------
void testApp::update(){
    float xorig = ofGetWidth()*0.5;
	float yorig = ofGetHeight()*0.5;
	
	for (int i = 0; i < particles.size(); i++){
		float radius = 50 + i * 20;
		float angle = ofGetElapsedTimef() * (1 + i / 10.0);
        
		float x = xorig + radius * cos(angle* ofMap(ofGetMouseX(),0,ofGetScreenWidth(),0.0,1.0));
		float y = yorig + radius * -sin(angle* ofMap(ofGetMouseY(),0,ofGetScreenHeight(),0.0,1.0));
		
		particles[i].moveTo(x,y);
	}
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    float sinSwitcher = abs(sin(ofGetElapsedTimef()*0.1));
    
    if (!ofGetMousePressed()){
        drawParticlesLine((1.0-sinSwitcher)*0.8);
    }
    
    for (int i = 0; i < particles.size(); i++){
        
        if (!ofGetMousePressed()){
            particles[i].drawTrail(sinSwitcher*0.5);
        }
        
        int closest = 10000;
        for ( int j = 0; j < particles.size(); j++){
            if (i != j){
                int dist = particles[i].distance(particles[j]);
                
                if (dist < closest){
                    closest = dist;
                }
                
                if ((dist < 100) && ofGetMousePressed()){
                    ofSetColor(255, ofMap(closest,0,100,100,0,true) );
                    ofLine(particles[i], particles[j]);
                }
            }
        }
        
        particles[i].drawDot( ofMap(closest,0,100,1.0,0.0,true) );
    }
    
    ofSetColor(255);
    ofDrawBitmapString("Wait until", 5,15);
    ofSetColor(ofFloatColor(1.0-sinSwitcher*0.5));
    ofDrawBitmapString("figure", 5,30);
    ofSetColor(255);
    ofDrawBitmapString("becomes", 5,45);
    ofSetColor(ofFloatColor(sinSwitcher*0.5));
    ofDrawBitmapString("ground", 5,60);
}

void testApp::drawParticlesLine(float _alpha){
    ofPolyline macroLine;
    macroLine.addVertex(particles[0]);
    for (int i = 0 ; i < particles.size() ; i++){
        macroLine.curveTo(particles[i]);
    }
    macroLine.curveTo(particles[particles.size()-1]);
    macroLine = macroLine.getResampledByCount( 1000 );
    
    ofSetColor(255);
    ofMesh lineMesh;
    lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    for (int i = 0 ; i < macroLine.getVertices().size() ; i++){
        lineMesh.addColor(ofFloatColor(1.0, _alpha));
        lineMesh.addVertex(macroLine.getVertices()[i]);
    }
    ofSetColor(255);
    lineMesh.draw();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    ofToggleFullscreen();
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

}