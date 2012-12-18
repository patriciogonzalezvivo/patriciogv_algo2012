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
}

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

void Brush::begin(){
    
    if (!bDown){
        tail.clear();
    }
    
    bDown = true;
}

void Brush::end(){
    bDown = false;
}

void Brush::set(int _x, int _y){
    set(ofPoint(_x,_y));
}

void Brush::set(ofPoint _pos){
    
    if (bDown){
        prevPos.set(*this);
        
        ofPoint::set(_pos);
        tail.addVertex(_pos);
        
        if (tail.size() == 1){
            int n = springs.size();
            setNum(n);
        } else if ( tail.size() > 0){
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

ofPoint Brush::getVel(){
    if (tail.size() > 0)
        return  prevPos - *this;
    else
        return  ofPoint(0,0);
}

float Brush::getAngle(){
    if (tail.size() > 0){
        ofPoint vel = getVel();
        return  atan2(vel.y, vel.x);
    } else {
        return 0.0f;
    }
}

void Brush::update(){
    if(bDown){
        for(int i = 0; i < springs.size(); i++){
            As[i]->update();
            Bs[i]->update();
            springs[i].update();
            
            if (tail.size() > 0){
                tails[i].addVertex( *Bs[i] );
            }
        }
    }
}

void Brush::draw(){
    ofPushStyle();
    ofSetColor(0);
    for(int i = 0; i < springs.size(); i++){
        As[i]->draw();
        springs[i].draw();
        Bs[i]->draw();
        tails[i].draw();
    }
    
    float angle = getAngle() + HALF_PI;
    ofPoint a;
    a.x = x + cos(angle) * width*0.5;
    a.y = y + sin(angle) * width*0.5;
    ofPoint b;
    b.x = x + cos(angle+PI) * width*0.5;
    b.y = y + sin(angle+PI) * width*0.5;
    
    if (bDebug){
        ofSetColor(255, 0, 0);
        ofLine(a, b);
        ofSetColor(255, 0, 255);
        tail.draw();
    }
    
    ofPopStyle();
}
