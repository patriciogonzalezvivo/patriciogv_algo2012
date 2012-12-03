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
    
    virtual bool checkMousePressed(ofPoint _mouse);
    
    virtual void update();
    virtual void draw();
    
    string  text;
    string  prefix;
    string  deliminater;
    
    ofFloatColor    fgActiveColor,fgPasiveColor;
    ofFloatColor    bgActiveColor,bgPasiveColor;
    
    ofTrueTypeFont  *font;
    froebelTextBox  *subInfo;
    
    float   damp;
    int     maxWidth;
    
    bool    bSelected;
    bool    bLeftAlign;
    
protected:
    ofFloatColor    froebelColor(int _colorFromPalet);
    
    froebelShape    endingShape;
    froebelShape    iconShape;
    
    string          displayText;
    
    ofFloatColor    fgColor, fgDstColor;
    ofFloatColor    bgColor, bgDstColor;
    
    ofRectangle     textBox;
    
    float   size;
    float   margins;
    
    int     nEdges;
    
    bool    bEdge;
    bool    bIcon;
};

#endif
