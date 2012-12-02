//
//  checkList.h
//  projectGenerator
//
//  Created by Patricio Gonzalez Vivo on 12/1/12.
//
//

#ifndef __projectGenerator__checkList__
#define __projectGenerator__checkList__

#include "ofMain.h"
#include "checkButton.h"

class checkList : public ofRectangle {
public:
    checkList();
    
    void    addElement(string _value, bool _defVal = false);
    bool    select(string _value);
    vector<string>  getSelected();
    string  getSelectedAsString();
    
    void    clear();
    void    reset();
    
    void    checkMousePressed(ofPoint _mouse);
    void    update();
    void    draw();
    
    vector<checkButton*> elements;
    ofTrueTypeFont * font;

    float   damp;
    float   elementHeight;
    string  title;
    string  divisor;
    
private:
    float   mouseOffSet;
};

#endif /* defined(__projectGenerator__checkList__) */
