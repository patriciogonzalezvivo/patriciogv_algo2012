//
//  Dot.cpp
//  movement
//
//  Created by Patricio Gonzalez Vivo on 9/13/12.
//
//

#include "SuperDot.h"

SuperDot::SuperDot(){
    algorithm = ALGO_LINEAR;
    
    color = ofColor(255);
    
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
                wawArrive();
            
        } else if ( algorithm == ALGO_POWER){
            percentage += incrementFactor;
            
            float positionPorcentage = powf(percentage, shaper);
            set( (1.0f - positionPorcentage ) * (*org) + ( positionPorcentage ) * (*dst) );
            
            if ( percentage >= 1.0 )
                wawArrive();
            
        } else if ( algorithm == ALGO_XENO){
            set( incrementFactor * (*dst) + ( 1.0f - incrementFactor) * (*this) );
            
            if ( distance(*dst) <= 1 )
                wawArrive();
        }
    }
}

void SuperDot::wawArrive(){
    if (mode == MODE_STOP){
        bMoving = false;
    } else if ( mode == MODE_REPLAY){
        set( *org );
        percentage = 0.0;
        bMoving = true;
    } else if ( mode == MODE_PONG){
        ofPoint *tmp = dst;
        init( dst, org );
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
    
    stringstream text;
    
    text << "GO and ";
    
    if ( mode == MODE_STOP){
        text << "STOP" << endl;
    } else if ( mode == MODE_REPLAY ){
        text << "REPLAY" << endl;
    } else if ( mode == MODE_PONG ){
        text << "PONG" << endl;
    }
    
    text << "Increment ";
    if ( algorithm == ALGO_LINEAR){
        text << "LINEAR by " << incrementFactor << endl;
        text << "Percentage: " << percentage*100 << "%" << endl;
        text << "Total Time: " << (1.0/incrementFactor)/ofGetFrameRate() << "s" << endl;
        
    } else if ( algorithm == ALGO_POWER){
        text << "POWER by " << incrementFactor << " over " << shaper << endl;
        text << "Percentage: " << percentage*100 << "%" << endl;
        text << "Total Time: " << (1.0/incrementFactor)/ofGetFrameRate() << "s" << endl;
        
    } else if ( algorithm == ALGO_XENO){
        text << "XENO at " << incrementFactor*100 << "%"<< endl;
    }
    
    text << "Speed: " << ofToString(speed) << " px/sec" << endl;
    
    ofDrawBitmapString( text.str() , 5 , 15);
    
    ofRotate(ofRadToDeg(angle) + 90, 0, 0, 1 );
    ofLine(-10,0,10,0);

    ofPopMatrix();    
    
    //  Draw inner point
    //
    ofFill();
    ofSetColor(color,255);
    ofCircle(*this, radius*0.5);
    
    //  Draw out point
    //
    ofNoFill();
    ofSetColor(color,150);
    ofCircle(*this, radius);
    
    ofPopStyle();
}

bool SuperDot::mouseClick(int _x, int _y){
    bool hitSomething = false;
    
    if (( _x > dst->x + 5) && ( _y > dst->y + 5) &&
        ( _x < dst->x + 200 ) && ( _y < dst->y + 15 )){
        
        if ( mode == MODE_STOP){
            mode = MODE_REPLAY;
        } else if ( mode == MODE_REPLAY ){
            mode = MODE_PONG ;
        } else if ( mode == MODE_PONG ){
            mode = MODE_STOP;
        }
        
        hitSomething = true;
    } else if (( _x > dst->x + 5) && ( _y > dst->y + 15) &&
          ( _x < dst->x + 200 ) && ( _y < dst->y + 30 )){
        
        if ( algorithm == ALGO_LINEAR){
            algorithm = ALGO_POWER;
        } else if ( algorithm == ALGO_POWER){
            algorithm = ALGO_XENO;
        } else if ( algorithm == ALGO_XENO){
            algorithm = ALGO_LINEAR;
        }
        
        hitSomething = true;
    }
    
    return hitSomething;
}