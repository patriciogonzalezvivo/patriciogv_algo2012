//
//  froebelListBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//
//

#include "froebelListBox.h"

froebelListBox::froebelListBox(){
    subInfo     = NULL;
    
    setActiveColors(3, 4);
    setPasiveColors(2, 5);
    
    iconShape.dstColor = endingShape.dstColor = bgColor = bgDstColor = fgColor = fgDstColor = froebelColor(0);
    
    backgroundColor = ofColor(230.f);
    
    bSelected   = false;
    bLeftAlign  = true;
    
    bEdge       = false;
    bIcon       = false;
    
    text        = "";
    prefix      = "";
    deliminater = "";
    
    maxWidth = 600;
    size = 40;
    damp = 0.1;
}

void froebelListBox::addElement(string _value, bool _defVal, int _iconShape){
    
    //  Add element to the vector
    //
    froebelTextBox *newElement = new froebelTextBox();
    newElement->x = x;
    newElement->y = box.y+box.height;
    
    if ( _iconShape == -1 )
        newElement->setSizeAndShapes(size,3);
    else
        newElement->setSizeAndShapes(size,3,_iconShape);
        
    newElement->text = _value;
    newElement->font = font;
    newElement->bSelected = _defVal;
    newElement->bgPasiveColor = backgroundColor;
    newElement->bgActiveColor = froebelColor(7);
    newElement->fgPasiveColor = froebelColor(5);
    newElement->fgActiveColor = froebelColor(4);
    
    
    
    //  recalculate the bounding box
    //
    box.x = x;
    box.y = y + height;
    box.width = width;
    box.height = 0;
    
    if (bEdge)
        box.width -= size*0.5;
    
    for(int i = 0; i < elements.size(); i++){
        box.height += elements[i]->height;
    }
}

void froebelListBox::clear(){
    for(int i = 0; i < elements.size(); i++){
        delete elements[i];
    }
    elements.clear();
}

void froebelListBox::reset(){
    for(int i = 0; i < elements.size(); i++){
        elements[i]->bSelected = false;
    }
}

bool froebelListBox::select(string _value){
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->text == _value){
            elements[i]->bSelected = true;
            return true;
        }
    }
    return false;
}

vector<string> froebelListBox::getSelected(){
    vector<string> list;
    
    for(int i = 0; i < elements.size(); i++){
        if ( elements[i]->bSelected ){
            list.push_back( elements[i]->text );
        }
    }
    
    return list;
}

string froebelListBox::getSelectedAsString(){
    string list;
    
    for (int i = 0; i < elements.size(); i++){
        
        if ( elements[i]->bSelected ){
            
            if (list.length() > 0)
                list += deliminater;
            
            list += elements[i]->text;
        }
    }
    
    return list;
}

bool froebelListBox::checkMousePressed(ofPoint _mouse){
    
    if ( !froebelTextBox::checkMousePressed(_mouse) ){
    
        if (box.inside(_mouse)){
            for(int i = 0; i < elements.size(); i++){
                if (box.inside( elements[i]->getCenter() )){
                    if (elements[i]->checkMousePressed(_mouse))
                        break;
                }
            }
        }
    }
}

void froebelListBox::draw(){
    
    //  Calculate the mouse compensation
    //
    if (inside(ofGetMouseX(),ofGetMouseY()) && box.height > maxHeight ){
        float pct = ofMap(ofGetMouseY()-box.y, 0, box.height, 0,1,true);
        float diff = maxHeight - box.height;
        mouseOffSet = ofLerp( mouseOffSet, -diff * pct, damp);
    }
    
    //  Fix the position of the elements
    //
    float offSetY = 0;
    for(int i = 0; i < elements.size(); i++){
        elements[i]->x = x;
        elements[i]->y = y + offSetY + mouseOffSet;
        elements[i]->width = width;
        
        offSetY += elements[i]->height;
    }
    
    //  Render Elements
    //
    ofSetColor(backgroundColor);
    ofRect(box);
    
    for(int i = 0; i < elements.size(); i++){
        if (box.inside( elements[i]->getCenter() )){
            elements[i]->draw();
        }
    }
    
    froebelTextBox::draw();
    height = size;
}