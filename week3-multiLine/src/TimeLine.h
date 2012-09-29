//
//  TimeLine.h
//  week3-multiLine
//
//  Created by Patricio Gonzalez Vivo on 9/28/12.
//
//

#ifndef TIMELINE
#define TIMELINE

#include "ofMain.h"

class TimePoint : public ofPoint {
public:
    float time;
};

class TimeLine {
public:
    
    TimeLine();
    
    //  Make TimeLine a vector like class
    //
    int         size();
    void        clear();
    TimePoint&  operator[](int _index);
    
    //  Overloading functions to them flexible
    //
    void    addPoint(int _x, int _y);
    void    addPoint(ofPoint _pos);
    
    bool    isFinish();
    void    setFinish();
    
    float   getDuration();
    ofPoint getPositionForTime( float _time);
    ofPoint getVelocityForTime( float _time);
   
    //  Draw the line
    //
    void    draw();
    
private:
    vector<TimePoint>   points;
    float               startTime;
};


#endif
