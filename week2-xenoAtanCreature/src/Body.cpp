//
//  Body.cpp
//  week2-xenoAtanCreature
//
//  Created by Patricio Gonzalez Vivo on 10/1/12.
//
//

#include "Body.h"

Body::Body(){
    catchUpSpeed = 0.06f;
    x = 0.0;
    y = 0.0;
    
    delay = 0;
    size = 30;
    color.set(198,246,55);
    prevPos.set(0.0,0.0);
    dst = NULL;
}

void Body::setDst(Body *_dst){
    dst = _dst;
}

void Body::update(){
    if (dst != NULL){
        x = catchUpSpeed * dst->x + (1-catchUpSpeed) * x;
        y = catchUpSpeed * dst->y + (1-catchUpSpeed) * y;
    }
    
	float dx = x - prevPos.x;
	float dy = y - prevPos.y;
	
	angle = atan2(dy, dx);
    
	prevPos.set(x,y);
}

void Body::draw() {
	ofFill();
	
	ofSetRectMode(OF_RECTMODE_CENTER); // center around the position
    ofSetColor(color);
    
    ofPushMatrix();
    ofTranslate(x,y+cos(ofGetElapsedTimef()*10-delay)*size*0.2,0);
    ofRotateZ( angle * RAD_TO_DEG);
    ofRect(0, 0, size, size);
    ofPopMatrix();
}