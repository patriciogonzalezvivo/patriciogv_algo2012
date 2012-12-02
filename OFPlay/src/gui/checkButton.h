//
//  checkButton.h
//  projectGenerator
//
//  Created by Patricio Gonzalez Vivo on 12/1/12.
//
//

#ifndef __projectGenerator__checkButton__
#define __projectGenerator__checkButton__

#include "ofMain.h"

class checkButton : public ofRectangle{
public:
    checkButton();
    
    void    draw();
    bool    checkMousePressed(ofPoint _mouse);
    
    bool operator=(bool v);
	operator bool & ();
    
    string  text;
    
    ofTrueTypeFont * font;
    
    float   marginW;
    float   marginH;
    
private:
    bool    bSelected;
};

#endif /* defined(__projectGenerator__checkButton__) */
