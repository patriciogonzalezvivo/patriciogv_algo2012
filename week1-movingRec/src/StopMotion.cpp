//
//  StopMotion.cpp
//  week1-movingRec
//
//  Created by Patricio Gonzalez Vivo on 9/15/12.
//
//

#include "StopMotion.h"

StopMotion::StopMotion(){
    
}

void StopMotion::allocate( int _width, int _height ){
    width = _width;
    height = _height;
}

void StopMotion::load(string _path){
    
}

void StopMotion::save(string _path){
    ofImage tmpImgHolder;
    tmpImgHolder.allocate(width, height, OF_IMAGE_COLOR);
    
    for(int i = 0; i < buffer.size(); i++){
        tmpImgHolder.setFromPixels(buffer[i].pixels , width, height, OF_IMAGE_COLOR);
        tmpImgHolder.saveImage(_path+"/"+ofToString(i)+".jpg");
    }
}

void StopMotion::addFrame( unsigned char * _pixels ){
    
    //  If is not frames means it's the first one
    //
    if (buffer.size() == 0){
        startTime = ofGetElapsedTimeMillis();
    }
    
    Frame newFrame;
    newFrame.pixels = _pixels;
    newFrame.timeStamp = ofGetElapsedTimeMillis() - startTime;
    
    buffer.push_back( newFrame );
}

void StopMotion::nextFrame(){
    
}

void StopMotion::goFrame(){
    
}

void StopMotion::goPct(float _pct){
    
}

void StopMotion::draw(int _x, int _y, int _width, int _height){
    
}
