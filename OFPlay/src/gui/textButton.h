//
//  textButton.h
//  projectGenerator
//
//  Created by molmol on 9/17/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef projectGenerator_textButton_h
#define projectGenerator_textButton_h

#define TEXTFIELD_IS_ACTIVE "textfieldIsActive"
#define TEXTFIELD_IS_INACTIVE "textfieldIsInactive"

#include "ofMain.h"

class textButton : public ofRectangle {
public:

    textButton();
    
    void    enableEditing();
	void    disableEditing();
    bool    getIsEnabled();
	bool    getIsEditing();
	void    beginEditing();
	void    endEditing();
    
    void setText( string newText );
    void calculateRect();
    void checkMousePressed(ofPoint mouse);
    void draw();
    
    ofTrueTypeFont * font;
    ofTrueTypeFont * secondFont;
    
    ofPoint topLeftAnchor;
    
    string  prefix;
    string  text;
    string  secondaryText;
    string  deliminater;
    
    bool    bSelectable;
    bool    bDrawLong;
    bool    bMouseOver;
    
private:
    ofEvent<string> textChanged;
	void    keyPressed(ofKeyEventArgs &a);
    void    mousePressed(ofMouseEventArgs& args);
    void    mouseReleased(ofMouseEventArgs& args);
    
    string  displayText;
    int     cursorPosition;
    int		cursorx, cursory;
    bool 	isEnabled;
	bool	isEditing;
	bool	mouseDownInRect;
    bool    bDrawCursor;
};

#endif
