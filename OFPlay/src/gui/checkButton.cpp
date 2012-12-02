//
//  checkButton.cpp
//  projectGenerator
//
//  Created by Patricio Gonzalez Vivo on 12/1/12.
//
//

#include "checkButton.h"

checkButton::checkButton(){
    bSelected = false;
    
    marginW = 12;
    marginH = 11;
}

bool checkButton::checkMousePressed(ofPoint _mouse){
    if (inside(_mouse)){
        bSelected = !bSelected;
        return true;
    }
    return false;
}

bool checkButton::operator=(bool v){
	bSelected = v;
	return v;
}

checkButton::operator bool & (){
	return bSelected;
}

void checkButton::draw(){
    if( bSelected ){
        ofSetColor(160, 160, 160);
    }else{
        if (inside(ofGetMouseX(), ofGetMouseY()) )
            ofSetColor(140,140,140);
        else
            ofSetColor(0,0,0);
    }
    ofRect(*this);
    
    ofSetColor(74,255,203);
    font->drawString(text, x+marginW, y+height-marginH);
}