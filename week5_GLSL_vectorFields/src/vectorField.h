#ifndef VECTORFIELD_H
#define VECTORFIELD_H


#include "ofMain.h"

class vectorField : public ofRectangle {
public:
	vectorField();
    virtual ~vectorField();
		
    void    setupField(int innerW, int innerH, int outerW, int outerH);   // pass in internal dimensions and outer dimensions
	
    int     size();
    void    clear();
    void    fadeField(float fadeAmount);
    void    randomizeField(float scale);
    
    ofVec2f& operator [](int _index);
    
    int     getIndexFor(float _x, float _y);
    ofVec2f	getForceFromPos(float xpos, float ypos);

    void    addOutwardCircle(float x, float y, float radius, float strength);
    void    addInwardCircle(float x, float y, float radius, float strength);
    void    addClockwiseCircle(float x, float y, float radius, float strength);
    void    addCounterClockwiseCircle(float x, float y, float radius, float strength);
    void    addVectorCircle(float x, float y, float vx, float vy, float radius, float strength);
    
    void    draw();
    
private:
    ofVec2f *field;
    
    int fieldWidth, fieldHeight, fieldSize;
    
};

#endif // VECTORFIELD_H
