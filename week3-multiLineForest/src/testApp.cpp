#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofEnableSmoothing();
}

//--------------------------------------------------------------
void testApp::update(){
    ofSetWindowTitle( ofToString( trees.size()));
}

void testApp::makeTreeFromLine( TimeLine& _timeLine ){
    
    //  Get Bounding box
    //
    ofRectangle box;
	refPoint = _timeLine.size();
    
	if( (refPoint > 0) && ( refPoint > 10)) {
		ofPoint first = _timeLine[0];
        
        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_LINES);
        
        for(int i = 1; (i < actualLine.size()) ; i++ ){
            mesh.addVertex( actualLine[i-1] );
            mesh.addVertex( actualLine[i] );
        }
        
        refPoint -= 8;
        addBranch(mesh, actualLine[8] , -PI*0.5, refPoint*0.2 );
        
        trees.push_back( mesh );
	}
}

void testApp::addBranch(ofMesh &_mesh, ofPoint _root, float _direction, float _depth) {
    
    if ((_depth <= 0) || (refPoint <= 1)) {
        //ellipse(rootX, rootY, 5, 5);
        return;
    }

    ofPoint pointA = actualLine[refPoint];
    ofPoint pointB = actualLine[refPoint-1];
    ofPoint diff = pointA - pointB;
    float   angle = atan2(diff.y, diff.x);
    
    int maxBranch = 5;
    float maxBranchAngle    =  angle;
    float minBranchLength   =  pointA.length(); //10;
    float maxBranchLength   =  pointB.length(); //50;
    
    int branches = int( ofRandom(1,maxBranch) );
    
    for (int i = 0; i < branches; i++) {
        float newBranchAngle = ofRandom(_direction - maxBranchAngle*0.5, _direction + maxBranchAngle*0.5);
        
        if (newBranchAngle >= TWO_PI) {
            newBranchAngle -= TWO_PI;
        }
        else if (newBranchAngle < 0) {
            newBranchAngle += TWO_PI;
        }
        
        float branchLength = _depth * 10;//ofRandom(minBranchLength, maxBranchLength);
        
        ofPoint tail;
        tail.x = _root.x + cos( newBranchAngle ) * branchLength;
        tail.y = _root.y + sin( newBranchAngle ) * branchLength;
        
        _mesh.addVertex(_root);
        _mesh.addVertex(tail);
        
        addBranch(_mesh, tail, newBranchAngle, _depth-1);
    }
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(0);
    
    ofSetColor(255);
    actualLine.draw();
    
    ofSetColor(255);
    for (int i = 0; i < trees.size(); i++){
        ofSetColor(255, ofMap(i, 0, trees.size(),0, 255));
        trees[i].draw();
    }
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    trees.clear();
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    actualLine.addPoint(x,y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    actualLine.addPoint(x,y);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    makeTreeFromLine(actualLine);
    actualLine.clear();
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