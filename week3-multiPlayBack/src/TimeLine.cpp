//
//  TimeLine.cpp
//  week3-multiLine
//
//  Created by Patricio Gonzalez Vivo on 9/28/12.
//
//

#include "TimeLine.h"

TimeLine::TimeLine(){
    startTime = -1;  // kind of NULL or not set
}

void TimeLine::addPoint(int _x, int _y){
    addPoint(ofPoint(_x,_y));
}

void TimeLine::addPoint(ofPoint _pos){
    
    if ( (startTime == -1) || (points.size() == 0) ){
        startTime = ofGetElapsedTimef();
    } 
    
    TimePoint newPoint;
    newPoint.set(_pos);
    newPoint.time = ofGetElapsedTimef() - startTime;
    
    points.push_back( newPoint );
}

int TimeLine::size(){
    points.size();
}

void TimeLine::clear(){
    points.clear();
    startTime = -1;
}

TimePoint& TimeLine::operator[](int _index){
    return points[_index] ;
}

float TimeLine::getDuration(){
	float duration = 0;
	if (points.size() > 0){
		duration =  points[points.size()-1].time;
	}
	return duration;
}

ofPoint	 TimeLine::getPositionForTime( float _time ){
	
	// are we recording?
	if ( !isFinish() ){
		return ofPoint(0,0,0);
	}
	
	// now, let's figure out where we are in the drawing...
	ofPoint pos;
	for (int i = 0; i < points.size()-1; i++){
		if ( (_time >= points[i].time) && (_time <= points[i+1].time) ){
			
			// calculate pct:
			float part = _time - points[i].time;
			float whole = points[i+1].time - points[i].time;
			float pct = part / whole;
			
			// figure out where we are between a and b
			pos.x = (1-pct) * points[i].x + (pct) * points[i+1].x;
			pos.y = (1-pct) * points[i].y + (pct) * points[i+1].y;
		}
	}
	
	return pos;
}

ofPoint	 TimeLine::getVelocityForTime( float _time ){
	
	// to get the velcoity, look back a bit of time, and at the current time
	// and get the difference
	// veclocity = pos at time 1 - pos at time 0...
	
	ofPoint prevPt = getPositionForTime( MAX(_time - 0.09f, 0));		// check for where we were 0.05 seconds ago
	ofPoint currPt = getPositionForTime(_time);							// check for where we are now.
	
	ofPoint diff;
	diff.x = currPt.x - prevPt.x;
	diff.y = currPt.y - prevPt.y;
	
	return diff;
}

float TimeLine::getAngleForTime( float _time ){
    ofPoint vel = getVelocityForTime( _time );
    
    // since velocity is a line, we want to know how long it is.
    return atan2(vel.y, vel.x);
}

void TimeLine::draw(){
    
    if (points.size() > 0){
        //  For making super smooth and gradient lines use ofMeshes.
        //  Them let you add vertex asociated to colors.
        //  OpenGL it's going to be in charge of making the gradient ; )
        //
        
        ofMesh lineMesh;
        lineMesh.setMode(OF_PRIMITIVE_LINE_STRIP);  //  There are other modes like: points, triangles and quads
        
        for (int i = 0 ; i < points.size() ; i++){
            
            //  Map the position on the array with the alpha to geting alfa gradient
            //
            //float alpha = ofMap(i+1, 1,points.size(), 0.0,1.0);
            
            //lineMesh.addColor(1.0); //ofFloatColor(_color, alpha*0.3) );   //  We are using ofFloatColors that goes from 0.0 - 1.0
            //  instead of 0-255 that's way you have better quality
            lineMesh.addVertex(points[i]);
        }
        
        //  Once this object call ofMesh it's done you can draw it.
        //
        ofSetColor(255);
        lineMesh.draw();
    }
}

bool TimeLine::isFinish(){
    if ( (points.size() > 0) && (startTime == -1)){
        return true;
    } else {
        return false;
    }
}

void TimeLine::setFinish(){
    startTime = -1;
}

bool TimeLine::isOver(int _x, int _y){
    isOver(ofPoint(_x,_y));
}

bool TimeLine::isOver(ofPoint _pos){
    bool over = false;
    
    for(int i = 0; i < points.size(); i++){
        if ( points[i].distance(_pos) < 30 ) {
            over = true;
            break;
        }
    }
    
    return over;
}