//
//  Dot.cpp
//  movement
//
//  Created by Patricio Gonzalez Vivo on 9/13/12.
//
//

#include "SuperDot.h"

SuperDot::SuperDot(){
    color = ofColor(255);
    
    algorithm = ALGO_LINEAR;
    
    incrementFactor = 0.001f;
    shaper = 2;
    percentage = 0.0f;
    
    bMoving = false;
}

void SuperDot::init(ofPoint *_start, ofPoint *_end){
    org = _start;
    dst = _end;
    
    set( *org );
    
    percentage = 0.0f;
    
    bMoving = true;
}

bool SuperDot::update(){
    prev = *this;
    
    if (bMoving){
        if ( algorithm == ALGO_LINEAR){
            percentage += incrementFactor;
            set( (1.0f - percentage ) * (*org) + ( percentage ) * (*dst) );
            
            if ( percentage >= 1.0 )
                bMoving = false;
            
        } else if ( algorithm == ALGO_POWER){
            percentage += incrementFactor;
            
            float positionPorcentage = powf(percentage, shaper);
            set( (1.0f - positionPorcentage ) * (*org) + ( positionPorcentage ) * (*dst) );
            
            if ( percentage >= 1.0 )
                bMoving = false;
            
        } else if ( algorithm == ALGO_XENO){
            set( incrementFactor * (*dst) + (1-incrementFactor) * (*this) );
            
            if ( distance(*dst) <= 1 )
                bMoving = false;
        }
    }
}

void SuperDot::draw(){
    float radius = 5.0;
    float angle = atan2( dst->y - org->y, dst->x - org->x );
    float speed = distance(prev) * ofGetFrameRate();
    
    ofPushStyle();
    
    //  Draw line
    //
    ofSetColor(255, 200);
    ofLine( *org, *dst);
    
    //  Draw start line
    //
    ofPushMatrix();
    ofTranslate(*org);
    ofRotate(ofRadToDeg(angle) + 90, 0, 0, 1 );
    ofLine(-10,0,10,0);
    ofPopMatrix();
    
    //  Draw end line and information
    //
    ofPushMatrix();
    ofTranslate(*dst);
    
    ofRotate(ofRadToDeg(angle), 0, 0, 1 );
    
    if ( algorithm == ALGO_LINEAR){
        ofDrawBitmapString("LINEAR", 5, 15);
        ofDrawBitmapString("speed: " + ofToString(speed) + " px/sec", 5, 45);
        ofDrawBitmapString("pos: " + ofToString(percentage*100) + "%", 5, 30);
        
    } else if ( algorithm == ALGO_POWER){
        ofDrawBitmapString("POWER", 5, 15);
        ofDrawBitmapString("speed: " + ofToString(speed) + " px/sec", 5, 45);
        ofDrawBitmapString("pos: " + ofToString(percentage*100) + "%", 5, 30);
        ofDrawBitmapString("shaper: " + ofToString(shaper) , 5, 45);
        
    } else if ( algorithm == ALGO_XENO){
         ofDrawBitmapString("XENO", 5, 15);
        ofDrawBitmapString("speed: " + ofToString(speed) , 5, 45);
    }
    
    
    ofRotate(90, 0, 0, 1 );
    ofLine(-10,0,10,0);

    ofPopMatrix();

    
    
    //  Draw inner point
    //
    ofFill();
    ofSetColor(color);
    ofCircle(*this, radius*0.5);
    
    //  Draw out point
    //
    ofNoFill();
    ofCircle(*this, radius);
    
    ofPopStyle();
}