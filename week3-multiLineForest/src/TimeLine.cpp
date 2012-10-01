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
    return points.size();
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

float TimeLine::getTimeForPct( float _pct ){
    return getDuration()*_pct;
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
	ofPoint currPt = getPositionForTime( _time );							// check for where we are now.
	
	ofPoint diff;
	diff.x = currPt.x - prevPt.x;
	diff.y = currPt.y - prevPt.y;
	
	return diff;
}

float TimeLine::getAngleForTime( float _time ){
    ofPoint vel = getVelocityForTime( _time );
    return atan2(vel.y, vel.x);
}

void TimeLine::draw(){
    
    if (points.size() > 0){
        ofPushStyle();
        ofNoFill();
        ofBeginShape();
        for (int i = 0; i < points.size(); i++) {
            ofVertex(points[i]);
        }
        ofEndShape();
        ofPopStyle();
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