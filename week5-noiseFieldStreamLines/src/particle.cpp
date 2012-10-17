#include "particle.h"
#include "ofMain.h"

//------------------------------------------------------------
Particle::Particle(){
	set(0,0,0);
    vel.set(ofRandom(-10,10), ofRandom(-10,10));
    acc.set(0,0,0);
    
    trailLength = ofRandom(50,100);
	damping = 0.07f;
    
    color.set(1.0);
}

void Particle::init( ofPoint _pos, ofPoint _vel){
    this->set(_pos);
    vel.set(_vel);
}

//------------------------------------------------------------
void Particle::addForce(ofPoint &_force){
    acc += _force;
}

//------------------------------------------------------------
void Particle::update(){
	
    vel += acc;
    vel *= 1.0f - damping;
    *this += vel;
    
    acc *= 0;
    
    trail.push_back(*this);
    
    
    if( trail.size() > 500){
        trail.erase(trail.begin());
    }
    
}

//------------------------------------------------------------
void Particle::draw(){
    
    ofMesh mesh;
    mesh.setMode(OF_PRIMITIVE_LINE_STRIP);
    for (int i = 0; i < trail.size(); i++){
        float alpha = ofMap(i+1, 1,trail.size(), 0.0, 0.9);
        
        mesh.addColor(ofFloatColor( color, alpha) );
        mesh.addVertex(trail[i]);
    }
    
    
    /*
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    for (int i = 0; i < trail.size(); i++){
        
        int i_m_1 = MAX(i-1,0);
        int i_p_1 = MIN(i+1, trail.size()-1);
        
        ofPoint pta = trail[i_m_1];
        ofPoint ptb = trail[i];
        ofPoint ptc = trail[i_p_1];
        
        ofPoint diff = ptc - pta;
        float angle = atan2(diff.y, diff.x);
        angle += PI/2;
        
        float width = size*0.8 + diff.length()*0.2;
        
        ofPoint offsetA;
        offsetA.x = ptb.x + width * cos(angle);
        offsetA.y = ptb.y + width * sin(angle);
        offsetA.z = 0.0;
        
        ofPoint offsetB;
        offsetB.x = ptb.x - width * cos(angle);
        offsetB.y = ptb.y - width * sin(angle);
        offsetB.z = 0.0;
        
        //  Map the position on the array with the alpha to geting alfa gradient
        //
        float alpha = ofMap(i+1, 1,trail.size(), 0.0, 0.9);
        
        mesh.addColor(ofFloatColor( color, alpha) );
        mesh.addVertex(offsetA);
        mesh.addColor(ofFloatColor( color, alpha) );
        mesh.addVertex(offsetB);
    }*/
    
    // draw the mesh
    //
    ofSetColor( 255 );
    ofFill();
    mesh.draw();
}

void Particle::infinitWalls(){
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (x > maxx){
		x = minx;
        trail.clear();
	} else if (x < minx){
		x = maxx;
        trail.clear();
	}
	
	if (y > maxy){
		y = miny;
        trail.clear();
	} else if (y < miny){
		y = maxy;
        trail.clear();
	}
}
