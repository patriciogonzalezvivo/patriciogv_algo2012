//
//  Dot.cpp
//  movement
//
//  Created by Patricio Gonzalez Vivo on 9/13/12.
//
//

#include "SuperDot.h"

SuperDot::SuperDot(){
    shape = SHAPE_LINEAR;
    loop = LOOP_PONG;
    
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
        if ( shape == SHAPE_XENO){
            set( incrementFactor * (*dst) + ( 1.0f - incrementFactor) * (*this) );
            
            if ( distance(*dst) <= 1 )
                wawArrive();
        } else {
            percentage += incrementFactor;
            
            if ( shape == SHAPE_LINEAR){
                set( (1.0f - percentage ) * (*org) + ( percentage ) * (*dst) );
                
            } else if ( shape == SHAPE_POWER){
                float positionPorcentage = powf(percentage, shaper);
                set( (1.0f - positionPorcentage ) * (*org) + ( positionPorcentage ) * (*dst) );
                
            } else if ( shape == SHAPE_SIN_IN){
                float positionPorcentage = sin( percentage * PI/2 );
                set( (1.0f - positionPorcentage ) * (*org) + ( positionPorcentage ) * (*dst) );
            } else if ( shape == SHAPE_SIN_OUT){
                float positionPorcentage = cos( percentage * PI/2 );
                set( (1.0f - positionPorcentage ) * (*org) + ( positionPorcentage ) * (*dst) );
            }

            
            if ( percentage >= 1.0 )
                wawArrive();
        }
    }
}

void SuperDot::wawArrive(){
    if (loop == LOOP_NONE){
        bMoving = false;
    } else if ( loop == LOOP_REPLAY){
        set( *org );
        percentage = 0.0;
        bMoving = true;
    } else if ( loop == LOOP_PONG){
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
    
    ofSetColor(255,0,255,50);
    ofRect(60,5,50,13);
    ofRect(85,18,180,13);
    
    ofSetColor(255);
    stringstream text;
    text << "GO and ";
    if ( loop == LOOP_NONE){
        text << "STOP" << endl;
    } else if ( loop == LOOP_REPLAY ){
        text << "REPLAY" << endl;
    } else if ( loop == LOOP_PONG ){
        text << "PONG" << endl;
    }
    
    text << "Increment ";
    
    if ( shape == SHAPE_XENO){
        text << "XENO at " << incrementFactor*100 << "%"<< endl;
    } else {
    
        if ( shape == SHAPE_LINEAR){
            text << "LINEAR by " << incrementFactor << endl;
            
        } else if ( shape == SHAPE_POWER){
            text << "POWER by " << incrementFactor << " over " << shaper << endl;
            
        } else if ( shape == SHAPE_SIN_IN){
            text << "SIN IN by " << incrementFactor << endl;
        } else if ( shape == SHAPE_SIN_OUT){
            text << "SIN OUT by " << incrementFactor << endl;
        }
        
        text << "Percentage: " << percentage*100 << "%" << endl;
        text << "Total Time: " << (1.0/incrementFactor)/ofGetFrameRate() << "s" << endl;
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
        
        if ( loop == LOOP_NONE){
            loop = LOOP_REPLAY;
        } else if ( loop == LOOP_REPLAY ){
            loop = LOOP_PONG ;
        } else if ( loop == LOOP_PONG ){
            loop = LOOP_NONE;
        }
        
        hitSomething = true;
    } else if (( _x > dst->x + 5) && ( _y > dst->y + 15) &&
          ( _x < dst->x + 200 ) && ( _y < dst->y + 30 )){
        
        if ( shape == SHAPE_LINEAR){
            shape = SHAPE_POWER;
        } else if ( shape == SHAPE_POWER){
            shape = SHAPE_SIN_IN;
        } else if ( shape == SHAPE_SIN_IN){
            shape = SHAPE_SIN_OUT;
        } else if ( shape == SHAPE_SIN_OUT){
            shape = SHAPE_XENO;
        } else if ( shape == SHAPE_XENO){
            shape = SHAPE_LINEAR;
        }
        
        hitSomething = true;
    }
    
    return hitSomething;
}