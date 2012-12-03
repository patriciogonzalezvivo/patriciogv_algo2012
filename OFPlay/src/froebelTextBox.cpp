//
//  froebelTextBox.cpp
//  OFPlay
//
//  Created by Patricio Gonzalez Vivo on 12/2/12.
//
//

#include "froebelTextBox.h"

froebelTextBox::froebelTextBox(){
    
    fgActiveColor   = froebelColor(3);
    fgColor = fgDstColor = fgPasiveColor = froebelColor(2);
    bgActiveColor   = froebelColor(4);
    bgColor = bgDstColor = bgPasiveColor = froebelColor(5);
    
    endingShape.dstColor = bgDstColor;
    
    bSelected   = false;
    bLeftAlign  = true;
    
    bEdge       = false;
    bIcon       = false;
    
    text = "";
    prefix = "";
    deliminater = "";
    
    maxWidth = 600;
    size = 40;
    damp = 0.1;
}

void froebelTextBox::setSizeAndShapes(float _size, int _endingShape, int _iconShape){
    size = _size;
    height = size;
    width = size;
    
    if (_endingShape != -1){
        bEdge = true;
        endingShape.setShape(_endingShape, _size);
    } else {
        bEdge = false;
    }
    
    if (_iconShape != -1){
        bIcon = true;
        iconShape.setShape(_iconShape, _size*0.5);
    }
    
}

void froebelTextBox::setFont(ofTrueTypeFont &_font){
    font = &_font;
}

void froebelTextBox::draw(){
    
    //  Selection and Hover colors 
    //
    if( bSelected ){
        bgDstColor = bgActiveColor;
        fgDstColor = fgActiveColor;
        
        if (bEdge)
            endingShape.dstColor = bgActiveColor;
        
        if (bIcon)
            iconShape.dstColor = fgActiveColor;
            
    } else {
        if (inside(ofGetMouseX(), ofGetMouseY()) ){
            fgDstColor = bgActiveColor;
            
            if (bIcon)
                iconShape.dstColor = bgActiveColor;
            
        } else {
            fgDstColor = fgPasiveColor;
            
            if (bIcon)
                iconShape.dstColor = fgPasiveColor;
        }
        bgDstColor = bgPasiveColor;
        
        if (bEdge)
            endingShape.dstColor = bgPasiveColor;
    }
    
    if (bgColor != bgDstColor){
        bgColor.lerp(bgDstColor, damp);
    }
    
    if (fgColor != fgDstColor){
        fgColor.lerp(fgDstColor, damp);
    }
    
    //  Compose text
    //
    displayText = prefix + text;
    int   nEdges = 1;
    
    //  Calculate the size of the text
    //
    ofRectangle textBox = font->getStringBoundingBox( displayText , 0, 0);
    float margins = size;
    
    if ( bEdge )
        margins += size*0.5;
    
    if ( bIcon )
        margins += size*0.5;
    
    if ( textBox.width + margins >= width ){
        if ( textBox.width + margins <= maxWidth ){
            
            //  If it less that the max adjust the shape
            //
            width = textBox.width + margins;
            
        } else {
            
            //  other wise break it baby
            //
            string _newText = displayText;
            vector < string > breakUp;
            breakUp = ofSplitString(_newText, deliminater);
            
            ofPoint pos;
            pos.set(0,0);
            
            displayText = "";
            
            for (int i = 0; i < breakUp.size(); i++){
                string text = breakUp[i];
                if (i != breakUp.size() -1) text += deliminater;
                
                ofRectangle rect = font->getStringBoundingBox(text, pos.x, pos.y);
                
                if ((pos.x + rect.width) > maxWidth ){
                    displayText += "\n";
                    displayText += text;
                    pos.x = rect.width;
                } else {
                    displayText+= text;
                    pos.x += rect.width;
                }
            }
            
        }
        
        textBox = font->getStringBoundingBox( displayText , 0, 0);
        width = textBox.width + margins ;
        
        if ( textBox.height+size*0.5 > size ) {
            while ( textBox.height+size > size*nEdges ) {
                nEdges++;
                height = size*nEdges;
            }
        }
    }
    
    //  Render
    //
    ofRectangle smallBox;
    smallBox.set(*this);
    smallBox.width = width;
    smallBox.height = height ;

    if ( bEdge ){
         smallBox.width -= size*0.5;
    }
    
    ofPushMatrix();
    ofPushStyle();
    
    ofFill();
    ofSetColor(bgColor);
    ofRect(smallBox);
    ofTranslate(x, y);
    
    if (bEdge){
        for(int i = 0; i < nEdges; i++){
            endingShape.set(smallBox.width,size*0.5+size*i);
            endingShape.draw();
        }
    }
    
    if (bIcon){
        iconShape.set( width-size*0.5 ,size*0.5);
        iconShape.draw();
    }
    
    ofSetColor(fgColor);
    
    if (bLeftAlign){
        font->drawString(displayText, size*0.5, size-13);
    } else {
        font->drawString(displayText, width-(margins-size*0.5)-textBox.width, size-13);
    }
        
    ofPopMatrix();
    ofPopStyle();
    
}

ofFloatColor froebelTextBox::froebelColor(int _color){
    ofFloatColor color;
    
    switch (_color) {
        case 0:
            color = ofColor(220);
            break;
        case 1:
            color = ofColor(4, 49, 7);
            break;
        case 2:
            color = ofColor(220, 202, 185);
            break;
        case 3:
            color = ofColor(186, 1, 23);
            break;
        case 4:
            color = ofColor(247, 181, 55);
            break;
        case 5:
            color = ofColor(64, 79, 122);
            break;
        case 6:
            color = ofColor(62, 2, 35);
            break;
        case 7:
            color = ofColor(193, 66, 11);
            break;
        default:
            color = ofColor(0);
            break;
    }
    
    return color;
};