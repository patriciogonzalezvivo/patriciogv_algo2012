//
//  textButton.cpp
//  projectGenerator
//
//  Created by Patricio Gonzalez Vivo on 11/30/12.
//
//

#include "textButton.h"

textButton::textButton(){
    bSelectable = true;
    bMouseOver = false;
    bDrawLong = true;
    
    text = "";
	cursorPosition = 0;
	cursorx = 0;
	cursory = 0;
    isEnabled = false;
	isEditing = false;
    bDrawCursor = false;
	mouseDownInRect = false;
    
    deliminater = "/";
}

void textButton::setText(string _newText){
    
    text = _newText;
    
    _newText = prefix + _newText;
    vector < string > breakUp;
    breakUp = ofSplitString(_newText, deliminater);
    
    ofPoint pos;
    pos.set(0,0);
    
    displayText = "";
    
    for (int i = 0; i < breakUp.size(); i++){
        string text = breakUp[i];
        if (i != breakUp.size() -1) text += deliminater;

        ofRectangle rect = font->getStringBoundingBox(text, pos.x, pos.y);
        ofRectangle rectSecText = secondFont->getStringBoundingBox(secondaryText, 0, 0);
        if ((pos.x + rect.width) > ((ofGetWidth() - x*2) - rectSecText.width)){
            displayText += "\n";
            displayText += text;
            pos.x = rect.width;
        } else {
            displayText+= text;
            pos.x += rect.width;
        }
    }
}

void textButton::calculateRect() {
    
    //  Update
    //
    setText(text);
    
    this->set(font->getStringBoundingBox( displayText, topLeftAnchor.x, topLeftAnchor.y));
    
    x -= 12;
    y -= 12;
    width += 23;
    height += 23;
    
}

void textButton::checkMousePressed(ofPoint mouse){
    if (bSelectable == false){ bMouseOver = false; return;}
    if (inside(mouse)){
        bMouseOver = true;
    } else {
        bMouseOver = false;
    }
}


void textButton::enableEditing(){
	if(!isEnabled){
		ofAddListener(ofEvents().mousePressed, this, &textButton::mousePressed);
		ofAddListener(ofEvents().mouseReleased, this, &textButton::mouseReleased);
		isEnabled = true;
	}
}

void textButton::disableEditing(){
	if(isEditing){
		endEditing();
	}
	if(isEnabled){
        ofRemoveListener(ofEvents().mousePressed, this, &textButton::mousePressed);
		ofRemoveListener(ofEvents().mouseReleased, this, &textButton::mouseReleased);
		isEnabled = false;
    }
	
}
void textButton::beginEditing() {
    if(!isEditing){
        ofAddListener(ofEvents().keyPressed, this, &textButton::keyPressed);
        ofSendMessage(TEXTFIELD_IS_ACTIVE);
        isEditing = true;
        bDrawCursor = true;
        cursory = 0;
        cursorPosition = cursorx = text.size();
    }
}

void textButton::endEditing() {
    if(isEditing){
        ofRemoveListener(ofEvents().keyPressed, this, &textButton::keyPressed);
        ofSendMessage(TEXTFIELD_IS_INACTIVE);
        ofNotifyEvent(textChanged, text, this);
        isEditing = false;
        bDrawCursor = false;
        
        for (int i = 0; i < text.size(); i++){
            int which = (int)text[i];
            if ((which >= 48 && which <= 57) ||
                (which >= 65 && which <= 90) ||
                (which >= 97 && which <= 122)){
            } else {
                text[i] = '_';
            }
        }
    }
}

bool textButton::getIsEditing(){
    return isEditing;
}

bool textButton::getIsEnabled(){
	return isEnabled;
}

void textButton::draw() {
    ofPushStyle();
    ofFill();
    
    if (bDrawLong == true){
        ofSetColor(220,220,220);
        ofFill();
        ofRect(x, y, ofGetWidth() - x*2, height);
        
        ofRectangle rectString = secondFont->getStringBoundingBox(secondaryText, 0, 0);
        rectString.y = (y + height/2) + (rectString.height)/2;
        rectString.x = (x + ofGetWidth() - x*2) - rectString.width - 10;
        
        ofFill();
        ofSetColor(0,0,0);
        secondFont->drawString(secondaryText, rectString.x, rectString.y);
    }
    
    
    if( bSelectable ){
        if ( bMouseOver && !getIsEditing() )
            ofSetColor(140,140,140);
        else
            ofSetColor(0,0,0);
    }else{
        ofSetColor(160, 160, 160);
    }
    ofRect(*this);
    
    ofPushMatrix();
    ofTranslate(topLeftAnchor);
    
    ofSetColor(74,255,203);
    font->drawString(displayText, 0, 0);
    
    if(bDrawCursor) {
        ofPushStyle();
        float timeFrac = 0.5 * sin(3.0f * ofGetElapsedTimef()) + 0.5;
        
        ofColor col = ofGetStyle().color;
		float cursorPos = font->stringWidth(prefix + text.substr(0,cursorx))+3;
        ofSetColor(col.r * timeFrac, col.g * timeFrac, col.b * timeFrac);
        ofSetLineWidth(3.0f);
        
        ofLine(cursorPos, 13.7*cursory+2,cursorPos, 13.7*cursory-font->stringHeight("L")-2);
        ofPopStyle();
    }
    ofPopMatrix();
    ofPopStyle();
}

void textButton::mousePressed(ofMouseEventArgs& args){
	mouseDownInRect = inside(args.x, args.y);
}

void textButton::mouseReleased(ofMouseEventArgs& args){
    
    if( inside(args.x, args.y)) {
        if(!isEditing && mouseDownInRect){
	        beginEditing();
        }
    }
    else if(isEditing){
		endEditing();
	}
}

void textButton::keyPressed(ofKeyEventArgs& args) {
	//ew: add charachter (non unicode sorry!)
	//jg: made a step closer to this with swappable renderers and ofxFTGL -- but need unicode text input...
	
	int key = args.key;
	if (key == OF_KEY_RETURN) {
        endEditing();
        return;
	}
	
	if (key >=32 && key <=126) {
		text.insert(text.begin()+cursorPosition, key);
		cursorPosition++;
	}
	
	
	if (key==OF_KEY_BACKSPACE) {
		if (cursorPosition>0) {
			text.erase(text.begin()+cursorPosition-1);
			--cursorPosition;
		}
	}
	
	if (key==OF_KEY_DEL) {
		if (text.size() > cursorPosition) {
			text.erase(text.begin()+cursorPosition);
		}
	}
	
	if (key==OF_KEY_LEFT){
		if (cursorPosition>0){
			--cursorPosition;
		}
	}
	
	if (key==OF_KEY_RIGHT){
		if (cursorPosition<text.size()){
			++cursorPosition;
		}
	}
	
	//for multiline:
	cursorx = cursory = 0;
	if(text.size() > 0){
		for (int i=0; i<cursorPosition; ++i) {
			if (*(text.begin()+i) == '\n') {
				++cursory;
				cursorx = 0;
			} else {
				cursorx++;
			}
		}
	}
    
    setText(text);
    calculateRect();
}

