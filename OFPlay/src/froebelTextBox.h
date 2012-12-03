//
//  froebelTextBox.h
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//
//

#ifndef FROEBELTEXTBOX
#define FROEBELTEXTBOX

#include "ofMain.h"
#include "froebelShape.h"

class froebelTextBox: public ofRectangle {
public:
    froebelTextBox();
    
    void    setActiveColors(int _fg, int _bg);
    void    setPasiveColors(int _fg, int _bg);
    void    setSizeAndShapes(float _size, int _endingShape = -1, int _iconShape = -1);
    
    void    setFont(ofTrueTypeFont &_font);
    
    virtual void draw();
    
    string  text;
    string  prefix;
    string  deliminater;
    
    float   damp;
    int     maxWidth;
    
    bool    bSelected;
    bool    bLeftAlign;
    
protected:
    ofFloatColor    froebelColor(int _colorFromPalet);
    
    froebelShape    endingShape;
    froebelShape    iconShape;
    
    string          displayText;
    
    ofFloatColor    fgActiveColor,fgPasiveColor;
    ofFloatColor    bgActiveColor,bgPasiveColor;
    
    ofFloatColor    fgColor, fgDstColor;
    ofFloatColor    bgColor, bgDstColor;
    
    ofTrueTypeFont  *font;
    
    float   size;
    
    bool    bEdge;
    bool    bIcon;
};

#endif
