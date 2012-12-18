//
//  Brush.cpp
//  week11_DrawingTool
//
//  Created by Patricio Gonzalez Vivo on 12/17/12.
//
//

#include "Brush.h"

Brush::Brush(){
    width = 50;
    bDown = false;
    
    damp = 0.1;
    softness = 0.1;
    height = 10;
    passes = 0;
}

//-------------------------------------------------

void Brush::setWidth(float _width){
    width = _width;
    
    int n = springs.size();
    setNum(n);
}

void Brush::setNum(int _num){
    
    float angle = getAngle() + HALF_PI;
    float step = width/(float)_num;
    
    ofPoint top;
    top.x = x + cos(angle) * width*0.5;
    top.y = y + sin(angle) * width*0.5;
    ofPoint buttom;
    buttom.x = x + cos(angle+PI) * width*0.5;
    buttom.y = y + sin(angle+PI) * width*0.5;
    
    ofPoint diff = buttom - top;
    diff.normalize();
    diff *= step;
    
    for(int i = 0; i < springs.size(); i++){
        delete As[i];
        delete Bs[i];
    }
    As.clear();
    Bs.clear();
    springs.clear();
    tails.clear();
    
    for(int i = 0; i < _num; i++){
        Particle *a = new Particle();
        a->set( top + diff * i );
        a->size = step;
        a->bFixed = true;
        As.push_back(a);
        
        Particle *b = new Particle();
        b->set( top + diff * i );
        b->size = step;
        b->damping = damp;
        b->bFixed = false;
        Bs.push_back(b);
        
        Spring newSpring;
        newSpring.A = a;
        newSpring.B = b;
        newSpring.dist = height;
        newSpring.k = softness;
        springs.push_back(newSpring);
        
        ofPolyline newTail;
        tails.push_back(newTail);
    }
}

void Brush::setColor( ofColor _color ){
    color = _color;
}

void Brush::set(int _x, int _y){
    set(ofPoint(_x,_y));
}

void Brush::set(ofPoint _pos){
    
    if (bDown){
        prevPos.set(*this);
        ofPoint::set(_pos);
        
        if (path.size() == 0){
            startTime = ofGetElapsedTimef();
            _pos.z = 0;
        } else {
            _pos.z = ofGetElapsedTimef() - startTime;
        }
        path.addVertex(_pos);
        
        if (path.size() == 1){
            int n = springs.size();
            setNum(n);
        } else if ( path.size() > 0){
            float angle = getAngle() + HALF_PI;
            float step = width/(float)As.size();
            
            ofPoint top;
            top.x = x + cos(angle) * width*0.5;
            top.y = y + sin(angle) * width*0.5;
            ofPoint buttom;
            buttom.x = x + cos(angle+PI) * width*0.5;
            buttom.y = y + sin(angle+PI) * width*0.5;
            
            ofPoint diff = buttom - top;
            diff.normalize();
            diff *= step;
            
            for(int i = 0; i < As.size(); i++){
                As[i]->set( top + diff * i );
            }
            
        }
    }
}

//-------------------------------------------------

ofPoint Brush::getVel(){
    if (path.size() > 0)
        return  prevPos - *this;
    else
        return  ofPoint(0,0);
}

float Brush::getAngle(){
    if (path.size() > 0){
        ofPoint vel = getVel();
        return  atan2(vel.y, vel.x);
    } else {
        return 0.0f;
    }
}


float Brush::getDuration(){
	float duration = 0;
	if ( path.size() >= 2 ){
		duration =  path[path.size()-1].z;
	}
	return duration;
}

ofPoint	Brush::getPositionForTime( float time){
	
	// are we recording?
	if (path.size() < 2) {
		return ofPoint(0,0,0);
	}
	
	// now, let's figure out where we are in the drawing...
	ofPoint pos;
	for (int i = 0; i < path.size()-1; i++){
		if (time >= path[i].z && time <= path[i+1].z){
			
			// calculate pct:
			float part = time - path[i].z;
			float whole = path[i+1].z - path[i].z;
			float pct = part / whole;
			
			// figure out where we are between a and b
			pos.x = (1-pct) * path[i].x + (pct) * path[i+1].x;
			pos.y = (1-pct) * path[i].y + (pct) * path[i+1].y;
            
			
		}
	}
	
	return pos;
	
}

ofPoint	Brush::getVelocityForTime( float time){
    
	ofPoint prevPt = getPositionForTime( MAX(time - 0.05f, 0));
	ofPoint currPt = getPositionForTime(time);
	
	ofPoint diff;
	diff.x = currPt.x - prevPt.x;
	diff.y = currPt.y - prevPt.y;
	
	return diff;
}

//-------------------------------------------------

void Brush::clear(){
    for(int i = 0; i < springs.size(); i++){
        delete As[i];
        delete Bs[i];
    }
    As.clear();
    Bs.clear();
    springs.clear();
    path.clear();
    tails.clear();
}

void Brush::begin(){
    if (!bDown){
        path.clear();
    }
    bDown = true;
}

void Brush::end(){
    bDown = false;
    playbackStartTime = ofGetElapsedTimef();
}

//-------------------------------------------------

void Brush::update(){
    if(bDown){
        for(int i = 0; i < springs.size(); i++){
            As[i]->update();
            
//            for (int j = 0; j < i; j++){
//                Bs[i]->addRepulsionForce( Bs[j], Bs[i]->size, 0.5);
//            }
            
            springs[i].update();
            
            Bs[i]->update();
            
            if (path.size() > 0){
                tails[i].addVertex( *Bs[i] );
//                tails[i] = tails[i].getSmoothed(2);
            }
        }
    }
}

//-------------------------------------------------

void Brush::draw(){
    ofPushStyle();
    ofSetColor(color);
    for(int i = 0; i < springs.size(); i++){
        tails[i].draw();
//        Bs[i]->draw();
    }
    ofPopStyle();
}

void Brush::drawDebug(){
    ofPushStyle();
    ofSetColor(0);
    for(int i = 0; i < springs.size(); i++){
        ofSetColor(255);
        As[i]->draw();
        springs[i].draw();
        Bs[i]->draw();
    }
    
    float angle = getAngle() + HALF_PI;
    ofPoint a;
    a.x = x + cos(angle) * width*0.5;
    a.y = y + sin(angle) * width*0.5;
    ofPoint b;
    b.x = x + cos(angle+PI) * width*0.5;
    b.y = y + sin(angle+PI) * width*0.5;
    
    ofSetColor(255, 0, 0);
    ofLine(a, b);
    ofSetColor(255, 0, 255);
    path.draw();

    ofPopStyle();
}

void Brush::drawPathFollower(){
    if (!bDown){
		float timeToCheck = ofGetElapsedTimef() - playbackStartTime;
		while (timeToCheck > getDuration() && getDuration() > 0){
			timeToCheck -= getDuration();
		}
		
		ofPoint pos = getPositionForTime(timeToCheck);
		ofPoint vel = getVelocityForTime(timeToCheck);
		
		float angle = atan2(vel.y, vel.x);
		
		float lengthVel = ofDist(0,0,vel.x, vel.y);
		
		ofFill();
		
		ofSetRectMode( OF_RECTMODE_CENTER );
		
		ofPushMatrix();
        ofTranslate(pos.x,pos.y);
        ofRotateZ(angle * RAD_TO_DEG);
        
        ofRect(0,0,width + lengthVel/2.0,width);
        
        ofSetRectMode( OF_RECTMODE_CORNER );
		ofPopMatrix();
    }
}

void Brush::drawPathFollower(ofImage &_image){
    if (!bDown){
        float timeToCheck = ofGetElapsedTimef() - playbackStartTime;
        while (timeToCheck > getDuration() && getDuration() > 0){
            timeToCheck -= getDuration();
        }
        
        ofPoint pos = getPositionForTime(timeToCheck);
        ofPoint vel = getVelocityForTime(timeToCheck);
        
        float angle = atan2(vel.y, vel.x);
        
        float lengthVel = ofDist(0,0,vel.x, vel.y);
        
        ofFill();
        
        ofPushMatrix();
        ofTranslate(pos.x,pos.y);
        ofRotateZ(angle * RAD_TO_DEG);
        
        //    ofRect(0,0,width + lengthVel/2.0,width);
        _image.draw(0,0);
        ofPopMatrix();
    }
}