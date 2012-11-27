#include "particle.h"
#include "ofMain.h"

//------------------------------------------------------------
Particle::Particle(){
	set(0,0,0);
    vel.set(ofRandom(-10,10), ofRandom(-10,10));
    acc.set(0,0,0);

	damping = NULL;
    
    color.set(0.0);
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
    vel *= 1.0f - *damping;
    *this += vel;
    
    acc *= 0;
}

//------------------------------------------------------------
void Particle::draw(){
    ofPushMatrix();
    ofSetColor(color);
    float w = size;
    float h = w * (1.0 + vel.length() * 2.0);
    float angle = atan2(vel.y,vel.x) + PI*0.5;
    ofTranslate(x, y);
    ofRotateZ(ofRadToDeg(angle));
    ofRect(-w*0.5, -h*0.5, w, h);
    
    ofPopMatrix();
    
}

void Particle::infinitWalls(){
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
}

//------------------------------------------------------------
void Particle::addRepulsionForce(ofPoint posOfForce, float radius, float scale){
    
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= *this - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
        diff.normalize();
        acc += diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(ofPoint posOfForce, float radius, float scale){
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle &p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		p.acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addRepulsionForce(Particle &p, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
	
	// ----------- (2) calculate the difference & length
	
	ofVec2f diff	= (*this) - posOfForce;
	float length	= diff.length();
    float radius    = size + p.size ;
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1.0 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc += diff * scale * pct;
		p.acc -= diff * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addAttractionForce(Particle & p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc -= diff * scale * pct;
		p.acc += diff * scale * pct;
    }
	
}

//------------------------------------------------------------
void Particle::addClockwiseForce(Particle &p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x -= diff.y * scale * pct;
        acc.y += diff.x * scale * pct;
		p.acc.x += diff.y * scale * pct;
        p.acc.y -= diff.x * scale * pct;
    }
}

//------------------------------------------------------------
void Particle::addCounterClockwiseForce(Particle &p, float radius, float scale){
	
	// ----------- (1) make a vector of where this particle p is:
	ofPoint posOfForce;
	posOfForce.set(p);
	
	// ----------- (2) calculate the difference & length
	
	ofPoint diff	= (*this) - posOfForce;
	float length	= diff.length();
	
	// ----------- (3) check close enough
	
	bool bAmCloseEnough = true;
    if (radius > 0){
        if (length > radius){
            bAmCloseEnough = false;
        }
    }
	
	// ----------- (4) if so, update force
    
	if (bAmCloseEnough == true){
		float pct = 1 - (length / radius);  // stronger on the inside
		diff.normalize();
		acc.x += diff.y * scale * pct;
        acc.y -= diff.x * scale * pct;
		p.acc.x -= diff.y * scale * pct;
        p.acc.y += diff.x * scale * pct;
		
    }
}

//------------------------------------------------------------
void Particle::bounceOffWalls(){
	
	// sometimes it makes sense to damped, when we hit
	bool bDampedOnCollision = true;
	bool bDidICollide = false;
	
	// what are the walls
	float minx = 0;
	float miny = 0;
	float maxx = ofGetWidth();
	float maxy = ofGetHeight();
	
	if (x > maxx){
		x = maxx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	} else if (x < minx){
		x = minx; // move to the edge, (important!)
		vel.x *= -1;
		bDidICollide = true;
	}
	
	if (y > maxy){
		y = maxy; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	} else if (y < miny){
		y = miny; // move to the edge, (important!)
		vel.y *= -1;
		bDidICollide = true;
	}
	
	if (bDidICollide == true && bDampedOnCollision == true){
		vel *= 0.3;
	}
	
}
