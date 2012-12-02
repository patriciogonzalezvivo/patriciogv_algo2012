//
//  checkList.cpp
//  projectGenerator
//
//  Created by Patricio Gonzalez Vivo on 12/1/12.
//
//

#include "checkList.h"

checkList::checkList(){
    elementHeight   = 35.0f;
    damp            = 0.1f;
    mouseOffSet     = 0.0f;
    title           = "";
    divisor         = ", ";
}

void checkList::addElement(string _value, bool _defVal){
    checkButton *newElement = new checkButton();
    newElement->set(x, y+height, width, elementHeight);
    newElement->text = _value;
    newElement->font = font;
    *newElement = _defVal;
    elements.push_back(newElement);
}

void checkList::clear(){
    for(int i = 0; i < elements.size(); i++){
        delete elements[i];
    }
    elements.clear();
}

void checkList::reset(){
    for(int i = 0; i < elements.size(); i++){
        *(elements[i]) = false;
    }
}

bool checkList::select(string _value){
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->text == _value){
            (*elements[i]) = true;
            return true;
        }
    }
    return false;
}

vector<string> checkList::getSelected(){
    vector<string> list;
    
    for(int i = 0; i < elements.size(); i++){
        if ( *elements[i] ){
            list.push_back( elements[i]->text );
        }
    }
    
    return list;
}

string checkList::getSelectedAsString(){
    string list;
    
    for (int i = 0; i < elements.size(); i++){
        
        if ( *elements[i] ){
            
            if (list.length() > 0)
                list += divisor;
            
            list += elements[i]->text;
        }
    }
    
    return list;
}

void checkList::checkMousePressed(ofPoint _mouse){
    
    if (inside(_mouse)){
        ofRectangle rect = *this;
        if (title != ""){
            rect.y      += elementHeight;
            rect.height -= elementHeight;
        }
        
        for(int i = 0; i < elements.size(); i++){
            if (rect.inside( elements[i]->getCenter() )){
                if (elements[i]->checkMousePressed(_mouse))
                    break;
            }
        }
    }
}

void checkList::update(){
    
    //  Calculate the total height of addons elements
    //
    float totalLenght = 0;
    for(int i = 0; i < elements.size(); i++){
        totalLenght += elements[i]->height;
    }
    
    float offSetY = 0;
    if (title != ""){
        offSetY += elementHeight;
    }
    
    ofRectangle rect = *this;
    if (title != ""){
        rect.y      += elementHeight*2.0;
        rect.height -= elementHeight*2.5;
    }
    
    if (inside(ofGetMouseX(),ofGetMouseY()) && totalLenght > rect.height){
        float pct = ofMap(ofGetMouseY()-rect.y, 0, rect.height, 0,1,true);
        float diff = totalLenght - rect.height;
        mouseOffSet = ofLerp( mouseOffSet, -diff * pct, damp);
    }

    for(int i = 0; i < elements.size(); i++){
        elements[i]->x = x;
        elements[i]->y = y + offSetY + mouseOffSet;
        elements[i]->width = width;
        
        offSetY += elements[i]->height;
    }
    
}

void checkList::draw(){
    ofSetColor(0,0,0);
    ofRect(*this);
    
    ofRectangle rect = *this;
    if (title != ""){
        rect.y      += elementHeight*0.5;
        rect.height -= elementHeight;
    }
        
    for(int i = 0; i < elements.size(); i++){
        if (rect.inside( elements[i]->getCenter() )){
            elements[i]->draw();
        }
    }
    
    if (title != ""){
        ofSetColor(220,220,220);
        ofRect(x, y, width, elementHeight);
        ofSetColor(0,0,0);
        font->drawString(title, x + 11 , y + elementHeight - 11);
    }
}