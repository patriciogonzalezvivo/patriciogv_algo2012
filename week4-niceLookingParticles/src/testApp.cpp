#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    
    glow.loadImage("glow.png");
    
    for (int i = 0; i < 50; i++){
        Particle newParticle;
        
        newParticle.set(ofRandomWidth(), ofRandomHeight());
        newParticle.init(ofRandom(30));
        particles.push_back(newParticle);
    }
    
    ofSetFullscreen(true);
}

//--------------------------------------------------------------
void testApp::update(){
    
    ofPoint average;
    for (int i = 0; i < particles.size(); i++){
        average += particles[i];
    }
    average /= particles.size();
    
    
    for (int i = 0; i < particles.size(); i++){

        ofPoint toEpicenter = average;
        
        toEpicenter -= particles[i];// - toEpicenter;
        toEpicenter.normalize();
        toEpicenter *= cos(ofGetElapsedTimef()*0.1) * 0.1;
        particles[i].addForce( toEpicenter );
        
        particles[i].addNoise( PI , 0.3 );
        
        particles[i].infinitWalls();
        particles[i].update();
    }
    
    for (int i = particles.size()-1; i >= 0; i--){
        if ( particles[i].life < 0 ){
            particles.erase(particles.begin()+i);
        }
    }
    
    if ( particles.size() < ofRandom(40,50)){
        Particle newParticle;
        
        newParticle.set(ofRandomWidth(), ofRandomHeight());
        newParticle.init(ofRandom(30));
        particles.push_back(newParticle);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(50, 4, 15);
    //ofBackground(0);
    
    for (int i = 0; i < particles.size(); i++){
        particles[i].draw(glow);
    }
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